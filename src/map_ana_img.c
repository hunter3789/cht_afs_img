/*******************************************************************************
**
**  지도 표출
**
**=============================================================================*
**
**     o 작성자 : 이창재 (2020.5.24)
**
********************************************************************************/
#include "map_ana_img.h"
#include "rdr_cmp_header.h"

// 변수
struct INPUT_VAR var;  // 지점 자료
struct STN_VAL stn_data[MAX_STN];  // 지점 자료
struct RDR_CMP_HEAD  rdr_cmp_head;              // 합성자료 Header
struct RDR_CMP_STN_LIST  rdr_cmp_stn_list[48];  // 합성된 레이더 목록
int    num_color_sat, color_sat[255];
int    color_lgt;
int    num_color_rdr, color_rdr[40];
float  data_sat[255];
float  data_rdr[40];
float  *data[4];      // 위성 RGB 데이터
float  **map_lat, **map_lon;
short  **map_x, **map_y;
char   host[256];
struct timeval tv0, tv1, tv2;

// 함수
int map_disp(gdImagePtr, char *, float, int, int);
int grid_smooth(float **, int, int, float);
int grid_short_filter(short **, int, int, short);
float get_adj_angle_lcc(float);
/*******************************************************************************
 *
 *  MAIN
 *
 *******************************************************************************/
int main()
{
  int   err = 0;

  // 1. 초기화
  gettimeofday(&tv1, NULL);
  tv0 = tv2 = tv1;
  setvbuf(stdout, NULL, _IONBF, 0);
  alarm(30);

  printf("HTTP/1.0 200 OK\n");
  //printf("Content-type: text/plain\n\n");

  // 2. 사용자 입력 변수 분석
  if ( user_input() < 0 ) {
    printf("Content-type: text/plain\n\n");
    printf("user_input() error\n");
    return -1;
  }
  gethostname(host, sizeof(host));

  // 3. 자료 표출
  if (var.mode == 0) map_img_disp();
  else map_img_layer();

  alarm(0);
  return 0;
}

/*******************************************************************************
 *  격자자료 웹 이미지 표출시 사용자 요청 분석 부분
 *******************************************************************************/
int user_input() {
  char *qs;
  char tmp[256], item[32], value[256], tm[30], tm_fc[30];
  int  iYY, iMM, iDD, iHH, iMI, iSS;
  int  iseq, i, j, k;

  // 1. 변수 초기값 : 자료별 처리 프로그램에서 각자 상황에 맞게 설정
  strcpy(var.map, "HR");  // 레이더 영역
  var.size = 1000;
  strcpy(var.zoom_x, "0000000");
  strcpy(var.zoom_y, "0000000");
  strcpy(var.color, "B");
  strcpy(var.layer, "");
  strcpy(var.sat, "ir1");
  var.ZRa = 200;
  var.ZRb = 1.6;
  var.itv = 5;          // 낙뢰표출 범위 +-itv분
  var.lat1 = 0;
  var.lat2 = 70;
  var.lon1 = 50;
  var.lon2 = 180;
  var.pnts = 1;
  var.font_size = 1.0;
  var.border = 0;
  var.diff_det = 0.1;
  var.diff_max = 5.0;
  var.color_adjust = 0;
  var.bias_disp = 0;
  var.mode = 0;
  var.flag = 0;
  var.curl = 0;
  var.save = 0;
  var.wpf = 0;
  var.amdar = 0;
  var.color_wind = 0;
  strcpy(var.cht_mode, "fct");
  strcpy(var.host, "afs");
  var.gis = 0;
  var.legend = 0;
  var.legend_only = 0;

  // 2. GET 방식으로 전달된 사용자 입력변수들의 해독
  qs = getenv ("QUERY_STRING");
  if (qs == NULL) return -1;

  for (i = 0; qs[0] != '\0'; i++) {
    getword (value, qs, '&');
    getword (item, value, '=');
    if (strlen(value) == 0) continue;

    if ( !strcmp(item,"map"))            strcpy(var.map, value);
    else if ( !strcmp(item,"tm"))        strcpy(tm, value);
    else if ( !strcmp(item,"tm_fc"))     strcpy(tm_fc, value);
    else if ( !strcmp(item,"sat"))       strcpy(var.sat, value);
    else if ( !strcmp(item,"layer"))     strcpy(var.layer, value);
    else if ( !strcmp(item,"zoom_x"))    strcpy(var.zoom_x, value);
    else if ( !strcmp(item,"zoom_y"))    strcpy(var.zoom_y, value);
    else if ( !strcmp(item,"size"))      var.size = atoi(value);
    else if ( !strcmp(item,"itv"))       var.itv = atoi(value);
    else if ( !strcmp(item,"gts"))       strcpy(var.gts, value);
    else if ( !strcmp(item,"obs"))       strcpy(var.obs, value);
    else if ( !strcmp(item,"pnts"))      var.pnts = atoi(value);
    else if ( !strcmp(item,"font_size")) var.font_size = atof(value);
    else if ( !strcmp(item,"border"))    var.border = atoi(value);
    else if ( !strcmp(item,"model"))     strcpy(var.model, value);
    else if ( !strcmp(item,"color_adjust")) var.color_adjust = atoi(value);
    else if ( !strcmp(item,"bias_disp"))    var.bias_disp = atoi(value);
    else if ( !strcmp(item,"mode"))         var.mode = atoi(value);
    else if ( !strcmp(item,"cht_name"))     strcpy(var.cht_name, value);
    else if ( !strcmp(item,"cht_mode"))     strcpy(var.cht_mode, value);
    else if ( !strcmp(item,"flag"))         var.flag = atoi(value);
    else if ( !strcmp(item,"curl"))         var.curl = atoi(value);
    else if ( !strcmp(item,"save"))         var.save = atoi(value);
    else if ( !strcmp(item,"host"))         strcpy(var.host, value);
    else if ( !strcmp(item,"wpf"))          var.wpf = atoi(value);
    else if ( !strcmp(item,"amdar"))        var.amdar = atoi(value);
    else if ( !strcmp(item,"color_wind"))   var.color_wind = atoi(value);
    else if ( !strcmp(item,"gis"))          var.gis = atoi(value);
    else if ( !strcmp(item,"legend_only"))  var.legend_only = atoi(value);
  }

  // 3. 기본값 설정
  var.grid = 1.0;

  if (!strcmp(var.map,"NHEM")) {
    var.lat1 = 0;
    var.lat2 = 90;
    var.lon1 = -180;
    var.lon2 = 180;
  }
  else if (!strcmp(var.map,"WORLD")) {
    var.lat1 = -90;
    var.lat2 = 90;
    var.lon1 = -180;
    var.lon2 = 180;
  }

  // 4. 현재시간 및 재계산 지연시간 설정
  get_time(&iYY, &iMM, &iDD, &iHH, &iMI, &iSS);
  iseq = time2seq(iYY, iMM, iDD, iHH, iMI, 'm') - 3;
  seq2time(iseq, &iYY, &iMM, &iDD, &iHH, &iMI, 'm', 'n');

  // 5. 요청시간 설정
  if (strlen(tm) >= 10) {
    strncpy(tmp, &tm[0], 4);  tmp[4] = '\0';  iYY = atoi(tmp);
    strncpy(tmp, &tm[4], 2);  tmp[2] = '\0';  iMM = atoi(tmp);
    strncpy(tmp, &tm[6], 2);  tmp[2] = '\0';  iDD = atoi(tmp);
    strncpy(tmp, &tm[8], 2);  tmp[2] = '\0';  iHH = atoi(tmp);
    strncpy(tmp, &tm[10],2);  tmp[2] = '\0';  iMI = atoi(tmp);
  }
  var.seq = time2seq(iYY, iMM, iDD, iHH, iMI, 'm');

  // 6. 시작시간 설정 (있는 경우)
  if (strlen(tm_fc) >= 10) {
    strncpy(tmp, &tm_fc[0], 4);  tmp[4] = '\0';  iYY = atoi(tmp);
    strncpy(tmp, &tm_fc[4], 2);  tmp[2] = '\0';  iMM = atoi(tmp);
    strncpy(tmp, &tm_fc[6], 2);  tmp[2] = '\0';  iDD = atoi(tmp);
    strncpy(tmp, &tm_fc[8], 2);  tmp[2] = '\0';  iHH = atoi(tmp);
    strncpy(tmp, &tm_fc[10],2);  tmp[2] = '\0';  iMI = atoi(tmp);
  }
  var.seq_fc = time2seq(iYY, iMM, iDD, iHH, iMI, 'm');

  return 0;
}

#include "./map_disp_lib.c"
#include "./rdr_sat_lgt_img.c"
#include "./gts_cht_stn_lib.c"
#include "./cht_afs_img.c"
#include "./nwp_bias_lib.c"
/*******************************************************************************
 *  지도 이미지 중첩 표출
 *******************************************************************************/
int map_img_disp()
{
  gdImagePtr im;
  FILE  *fp;
  int   color_lvl[256];
  float data_lvl[256];
  float xo = 0, yo = 0, dx, dy;
  float x1, y1, ht;
  int   ix, iy;
  int   i, j, k, save_ok = 0;
  int   col, zx, zy, zmlvl;
  int   YY, MM, DD, HH, MI, SS, seq;
  float Re;
  char  fname[512];
  //double PI = 180*DEGRAD;

  // 1. 설정 확인
  // 1-1. 일기도-구름모의영상
  if (strstr(var.cht_name, "radm") && (strstr(var.cht_name, "rmwv") || strstr(var.cht_name, "rmir")) && strstr(var.layer,"A")) {
    if (!strstr(var.layer,"S")) strcat(var.layer, "S");

    if (!strcmp(var.cht_mode, "ana") || !strcmp(var.cht_mode, "ana2")) {
      if (strstr(var.cht_name, "rmwv_enhc")) strcpy(var.sat, "wv063_enhc");
      else if (strstr(var.cht_name, "rmir_enhc")) strcpy(var.sat, "ir1_enhc");
      else if (strstr(var.cht_name, "rmwv")) strcpy(var.sat, "wv063");
      else if (strstr(var.cht_name, "rmir")) strcpy(var.sat, "ir1");
    }
    else {
      if (strstr(var.cht_name, "kim")) strcpy(var.model, "KIMG");
      else strcpy(var.model, "UM");

      if (strstr(var.cht_name, "rmwv_enhc")) strcpy(var.sat, "rmwv_enhc");
      else if (strstr(var.cht_name, "rmir_enhc")) strcpy(var.sat, "rmir_enhc");
      else if (strstr(var.cht_name, "rmwv")) strcpy(var.sat, "rmwv");
      else if (strstr(var.cht_name, "rmir")) strcpy(var.sat, "rmir");
    }
  }

  if (var.flag != 0) {
    printf("Content-type: text/plain\n\n");
    // json 표출
    if (!strcmp(var.cht_mode, "gts")) {
      printf("{\n");
      printf("  \"data\": [\n");
    }
  }

  // 2. 최대 지도영역 확인
  if (!strcmp(var.map, "NHEM")) {
    Re = 6371.00877;
    var.NX = (int)((float)(Re*2*PI/2)/var.grid);  var.NY = (int)((float)(Re*2*PI/2)/var.grid);
    var.SX = (int)((float)(Re*PI/2)/var.grid);    var.SY = (int)((float)(Re*PI/2)/var.grid);
  }
  else if (!strcmp(var.map, "WORLD")) {
    Re = 6371.00877;
    var.NX = (int)((float)(Re*2*PI)/var.grid);  var.NY = (int)((float)(Re*PI)/var.grid);
    var.SX = (int)((float)(Re*PI)/var.grid);    var.SY = (int)((float)(Re*PI/2)/var.grid);
  }
  else map_ini_read();
  map_edge();

  // 3. 이미지 영역 설정
  var.NI = var.size;  var.NJ = (int)((float)(var.NY)/(float)(var.NX)*var.NI);
  if (strstr(var.layer,"A") && strcmp(var.cht_mode, "gts")) {
    if (!strcmp(var.map, "EA_CHT")) {
      var.NI = 854;       var.NJ = 598;
    }
    else if (!strcmp(var.map, "TP")) {
      var.NI = 854;       var.NJ = 525;
    }
    else if (!strcmp(var.map, "NHEM")) {
      var.NI = 654;       var.NJ = 654;
    }
    else if (!strcmp(var.map, "WORLD")) {
      var.NI = 1000;      var.NJ = 500;
    }
    else if (!strcmp(var.map, "E10")) {
      var.NI = 604;       var.NJ = 604;
    }

    if (var.gis == 0) {
      var.OI = 0;              var.OJ = 20;
      var.GI = var.NI + 50;    var.GJ = var.NJ + var.OJ + 36;
    }
    else {
      var.OI = 0;         var.OJ = 0;
      var.GI = var.NI;    var.GJ = var.NJ;
    }
  }
  else {
    var.OI = 0;         var.OJ = 0;
    var.GI = var.NI;    var.GJ = var.NJ;

    if (!strcmp(var.cht_mode, "gts") && strstr(var.obs, "bias") && var.bias_disp < 2 && var.gis == 0) {
      var.GI = var.NI + 50; 
    }
  }

  if (var.legend_only == 1) {
    var.NI = 0;                 var.NJ = 500;
    var.OI = 0;                 var.OJ = 20;
    var.GI = var.NI + 50;       var.GJ = var.NJ + var.OJ + 8;
  }

  if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD")) {
    if (strstr(var.layer,"R") || strstr(var.layer,"S") || strstr(var.layer,"W")) {
      map_lat = matrix(0, (int)(var.NJ/var.grid), 0, (int)(var.NI/var.grid));
      map_lon = matrix(0, (int)(var.NJ/var.grid), 0, (int)(var.NI/var.grid));
      map_x = smatrix(0, (int)(var.NJ/var.grid), 0, (int)(var.NI/var.grid));
      map_y = smatrix(0, (int)(var.NJ/var.grid), 0, (int)(var.NI/var.grid));
      if (!strcmp(var.map, "NHEM"))        map_stg_latlon((int)(var.NJ/var.grid), (int)(var.NI/var.grid));
      else if (!strcmp(var.map, "WORLD"))  map_eqdc_latlon((int)(var.NJ/var.grid), (int)(var.NI/var.grid));
    }
  }

  // 4. 이미지 구조체 설정 및 색상표 읽기
  if (var.gis == 0) {
    if (strstr(var.layer,"S") || strstr(var.layer,"R") || strstr(var.layer,"L") || strstr(var.layer,"G") || strstr(var.layer,"A") || strstr(var.layer,"E")) {
      im = gdImageCreateTrueColor(var.GI, var.GJ);
      color_table(im, color_lvl);
      gdImageFilledRectangle(im, 0, 0, var.GI, var.GJ, color_lvl[241]);
    }
    else {
      im = gdImageCreate(var.GI, var.GJ);
      color_table(im, color_lvl);
      //지도 투명화
      gdImageFilledRectangle(im, 0, 0, var.GI, var.GJ, 0);
      col = gdImageGetTransparent(im);
      gdImageColorAllocate(im, var.GI, var.GJ, col);
      gdImageColorTransparent(im, 0);
    }
  }
  else {
    im = gdImageCreateTrueColor(var.GI, var.GJ);
    col = gdImageColorAllocate(im, 255, 255, 255);
    gdImageColorTransparent(im, col);
    gdImageFilledRectangle(im, 0, 0, var.GI, var.GJ, col);
    color_table(im, color_lvl);
  }

  // 지도 표출 설정(1km)
  var.grid = 1.0; //1km

  var.seq += 9*60; //UTC->KST

  // 5. 지형 그리기
  if (strstr(var.layer,"T")) {
    if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD")) topo_disp(im, 5.0, 0, 0, color_lvl[0]);
    else topo_disp(im, 1.0, 0, 0, color_lvl[0]);
  }

  // 6. 위성자료 그리기
  if (strstr(var.layer,"S")) sat_bin_disp(im);

  // 7. 레이더자료 그리기
  if (strstr(var.layer,"R")) rdr_oth_disp(im);

  // 8. 낙뢰자료 그리기
  if (strstr(var.layer,"L")) lgt_disp(im);

  var.seq -= 9*60; //KST->UTC

  // 9. 통합기상분석 API 중첩
  if (strstr(var.layer,"A")) save_ok = cht_afs_disp(im, color_lvl);

  // 10. 위경도선 그리기
  if (strstr(var.layer,"C")) {
    if (var.color_adjust == 0 || (strstr(var.layer,"S") || strstr(var.layer,"R") || strstr(var.layer,"L"))) {
      if (!strcmp(var.cht_mode, "gts")) latlon_disp(im, 1.0, color_lvl[244]);
      else latlon_disp(im, 1.0, color_lvl[232]);
    }
    else {
      latlon_disp(im, 1.0, color_lvl[236]);
    }
  }

  // 11. 지도 그리기
  if (strstr(var.layer,"M")) {
    for (i = 0, zmlvl = 0; i < 7; i++, zmlvl++) {
      zx = var.zoom_x[i]-'0';
      zy = var.zoom_y[i]-'0';
      if (zx == 0 || zy == 0) break;
    }
    if (var.color_adjust == 0 || (strstr(var.layer,"S") || strstr(var.layer,"R") || strstr(var.layer,"L"))) {
      if (zmlvl > 4 && strcmp(var.map, "WORLD")) map_disp(im, "city", 1.0, color_lvl[249], color_lvl[249]);
      map_disp(im, "map", 1.0, color_lvl[244], color_lvl[244]);
    }
    else {
      if (zmlvl > 4 && strcmp(var.map, "WORLD")) map_disp(im, "city", 1.0, color_lvl[236], color_lvl[236]);
      map_disp(im, "map", 1.0, color_lvl[236], color_lvl[236]);
    }
  }

  // 12. ASCAT 바람장 그리기
  if (strstr(var.layer,"E")) ascat_disp(im, color_lvl);

  // 13. WISSDOM 바람장 그리기
  var.seq += 9*60; //UTC->KST
  if (strstr(var.layer,"W")) wis_disp(im, color_lvl);
  var.seq -= 9*60; //KST->UTC

  // 14. gts 데이터 그리기
  if (strstr(var.layer,"G")) {
    if (strstr(var.obs,"bias")) {
      if (!strcmp(var.obs, "bias_ta"))       strcpy(var.varn, "TA");
      else if (!strcmp(var.obs, "bias_td"))  strcpy(var.varn, "TD");
      else if (!strcmp(var.obs, "bias_wn"))  strcpy(var.varn, "WN");
      else if (!strcmp(var.obs, "bias_hm"))  strcpy(var.varn, "RH");
      else if (!strcmp(var.obs, "bias_mr"))  strcpy(var.varn, "MR");
      else if (!strcmp(var.obs, "bias_tp"))  strcpy(var.varn, "TP");
      else if (!strcmp(var.obs, "bias_ws"))  strcpy(var.varn, "WS");
      else if (!strcmp(var.obs, "bias_ps"))  strcpy(var.varn, "PS");
      else if (!strcmp(var.obs, "bias_wbt")) strcpy(var.varn, "WBT");
      else if (!strcmp(var.obs, "bias_gh"))  strcpy(var.varn, "GH");

      vmap_ini_read();
      bias_data_get();
      bias_data_plot(im);
    }
    else {
      gts_data_get();
      if (!strcmp(var.obs, "dust")) pm10_data_get();
      gts_data_plot(im);
    }
  }

  // 15. 테두리 그리기
  if (strstr(var.layer,"B")) map_border_disp(im);

  // 16. 범례 그리기
  if (!strcmp(var.cht_mode, "gts")) {
    if (strstr(var.layer,"R") || strstr(var.layer,"S")) map_legend_disp(im, color_lvl);
  }

  // 17. 파일명 만들기
  if (var.legend_only == 1) {
    seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
    if (!strcmp(var.map, "E10")) {
      sprintf(fname, "%s/%sm%02d_%s.png", LEGEND_DIR, var.cht_name, MM, var.map);
    }
    else {
      sprintf(fname, "%s/%sm%02d.png", LEGEND_DIR, var.cht_name, MM);
    }
  }
  else if (!strcmp(var.cht_mode, "fct") || !strcmp(var.cht_mode, "fct2")) {
    seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
    if (var.gis == 1) {
      sprintf(fname, "%s/%ss%03d_%04d%02d%02d%02d_area=%s_zoomx=%s_zoomy=%s_gis=%d.png", IMG_DIR, var.cht_name, (var.seq-var.seq_fc)/60, YY, MM, DD, HH, var.map, var.zoom_x, var.zoom_y, var.gis);
    }
    else {
      sprintf(fname, "%s/%ss%03d_%04d%02d%02d%02d_area=%s_zoomx=%s_zoomy=%s.png", IMG_DIR, var.cht_name, (var.seq-var.seq_fc)/60, YY, MM, DD, HH, var.map, var.zoom_x, var.zoom_y);
    }
  }
  else if (!strcmp(var.cht_mode, "ana0") || !strcmp(var.cht_mode, "ana") || !strcmp(var.cht_mode, "ana2") || !strcmp(var.cht_mode, "ana3")) {
    seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
    if (var.gis == 1) {
      sprintf(fname, "%s/%s%04d%02d%02d%02d_area=%s_zoomx=%s_zoomy=%s_gis=%d.png", IMG_DIR, var.cht_name, YY, MM, DD, HH, var.map, var.zoom_x, var.zoom_y, var.gis);
    }
    else {
      sprintf(fname, "%s/%s%04d%02d%02d%02d_area=%s_zoomx=%s_zoomy=%s.png", IMG_DIR, var.cht_name, YY, MM, DD, HH, var.map, var.zoom_x, var.zoom_y);
    }

    if (!strcmp(var.cht_mode, "ana0")) {
      save_ok = 1;

      get_time(&YY, &MM, &DD, &HH, &MI, &SS);
      seq = time2seq(YY, MM, DD, HH, MI, 'm');

      if ((var.seq + 9*60) > (seq - 1*60)) save_ok = 0;
    }
  }
  else if (!strcmp(var.cht_mode, "gts")) {
    seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
    sprintf(fname, "%s/gts_%s_%s_%04d%02d%02d%02d%02d_area=%s_layer=%s_size=%d_pnts=%d_fontsize=%.2f", GTS_DIR, var.gts, var.obs, YY, MM, DD, HH, MI, var.map, var.layer, var.size, var.pnts, var.font_size);
    sprintf(fname, "%s_zoomx=%s_zoomy=%s_border=%d_wpf=%d_amdar=%d_colorwind=%d_gis=%d", fname, var.zoom_x, var.zoom_y, var.border, var.wpf, var.amdar, var.color_wind, var.gis);
    if (strstr(var.layer, "S")) sprintf(fname, "%s_sat=%s", fname, var.sat);
    if ((strstr(var.layer, "A") && !strstr(var.cht_name, "gts")) || strstr(var.obs, "bias")) {
      seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
      sprintf(fname, "%s_tmfc=%04d%02d%02d%02d_model=%s", fname, YY, MM, DD, HH, var.model);
    }
    if (strstr(var.layer, "A")) sprintf(fname, "%s_cont=%s", fname, var.cht_name);
    if (strstr(var.obs, "bias")) sprintf(fname, "%s_biasdisp=%d", fname, var.bias_disp);
    sprintf(fname, "%s.png", fname);
  }

  // 18. 이미지 전송
  if (var.gis == 1) {
    gdImageLine(im, 0, var.GJ-1, var.GI, var.GJ-1, col);
  }

  if (var.flag == 0) {
    printf("Content-type: image/png\n\n");
    gdImagePng(im, stdout);
    if (var.save == 1 && save_ok == 1) {
      if ((fp = fopen(fname, "wb"))) {
        gdImagePng(im, fp);
        fclose(fp);
      }
    }
    gdImageDestroy(im);
  }
  else {
    if (!strcmp(var.cht_mode, "gts")) {
      if ((fp = fopen(fname, "wb"))) {
        gdImagePng(im, fp);
        fclose(fp);
      }
      gdImageDestroy(im);

      strcpy(fname, str_replace("/fct/www/ROOT/", "/", fname));
      printf("\n  ],\n");
      printf("  \"img\": \"%s\",\n", fname);
      printf("  \"bounds\": [[%f, %f], [%f, %f]],\n", var.starty, var.startx, var.endy, var.endx);
      printf("  \"center\": [%f, %f]\n", var.centery, var.centerx);
      printf("}\n");
    }
    else printf("%s\n", fname);
  }

  return 0;
}

/*******************************************************************************
 *  지도 이미지 레이어 생성
 *******************************************************************************/
int map_img_layer()
{
  gdImagePtr im;
  int   color_lvl[256];
  float data_lvl[256];
  float xo = 0, yo = 0, dx, dy;
  float x1, y1, ht;
  int   ix, iy;
  int   i, j, k;
  int   col, zx, zy, zmlvl;
  char  fname[256], gname[256];
  float  rate;
  struct lamc_parameter map;
  float  lon1, lat1, lon2, lat2, x, y;
  float  zm = 1.0;

  // 1. 최대 지도영역 확인
  map_ini_read();

  // 2. 이미지 영역 설정
  var.NI = var.size;  var.NJ = (int)((float)(var.NY)/(float)(var.NX)*var.NI);
  var.GI = var.NI;    var.GJ = var.NJ;

  for (i = 0, zmlvl = 0; i < 7; i++, zmlvl++, zm *= 1.5) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
    xo += (float)(var.NX)/24.0*(zx-1)/zm;
    yo += (float)(var.NY)/24.0*(zy-1)/zm;
  }

  if (img_file(fname, gname) == -10) {
    printf("HTTP/1.0 200 OK\n");
    printf("Content-type: text/plain\n\n");
    printf("@no data");
    return;
  }
  else if (img_file(fname, gname) < 0) {
    // 3. 이미지 구조체 설정 및 색상표 읽기
    //if (strstr(var.layer,"S") || strstr(var.layer,"R") || strstr(var.layer,"L") || strstr(var.layer,"G")) {
    if (strstr(var.layer,"S") || strstr(var.layer,"G")) {
      im = gdImageCreateTrueColor(var.GI, var.GJ);
      color_table(im, color_lvl);
      gdImageFilledRectangle(im, 0, 0, var.GI, var.GJ, color_lvl[241]);
    }
    else {
      im = gdImageCreate(var.GI, var.GJ);
      color_table(im, color_lvl);
      //지도 투명화
      gdImageFilledRectangle(im, 0, 0, var.GI, var.GJ, 0);
      col = gdImageGetTransparent(im);
      gdImageColorAllocate(im, var.GI, var.GJ, col);
      gdImageColorTransparent(im, 0);
    }

    // 지도 표출 설정(1km)
    var.grid = 1.0; //1km

    var.seq += 9*60; //UTC->KST

    // 4. 위성자료 그리기
    if (strstr(var.layer,"S")) sat_bin_disp(im);

    // 5. 레이더자료 그리기
    if (strstr(var.layer,"R")) rdr_oth_disp(im);

    // 6. 낙뢰자료 그리기
    if (strstr(var.layer,"L")) lgt_disp(im);

    var.seq -= 9*60; //KST->UTC

    // 7. 위경도선 그리기
    if (strstr(var.layer,"C")) {
      if (var.color_adjust == 0 || (strstr(var.layer,"S") || strstr(var.layer,"R") || strstr(var.layer,"L"))) {
        latlon_disp(im, 1.0, color_lvl[244]);
      }
      else {
        latlon_disp(im, 1.0, color_lvl[236]);
      }
    }

    // 8. 지도 그리기
    if (strstr(var.layer,"M")) {
      if (var.color_adjust == 0 || (strstr(var.layer,"S") || strstr(var.layer,"R") || strstr(var.layer,"L"))) {
        if (zmlvl > 3) map_disp(im, "city", 1.0, color_lvl[249], color_lvl[249]);
        map_disp(im, "map", 1.0, color_lvl[244], color_lvl[244]);
      }
      else {
        if (zmlvl > 3) map_disp(im, "city", 1.0, color_lvl[236], color_lvl[236]);
        map_disp(im, "map", 1.0, color_lvl[236], color_lvl[236]);
      }
    }

    // 9. gts 데이터 그리기
    if (strstr(var.layer,"G")) {
      if (strstr(var.obs,"bias")) {
        if (!strcmp(var.obs, "bias_ta"))       strcpy(var.varn, "TA");
        else if (!strcmp(var.obs, "bias_td"))  strcpy(var.varn, "TD");
        else if (!strcmp(var.obs, "bias_wn"))  strcpy(var.varn, "WN");
        else if (!strcmp(var.obs, "bias_hm"))  strcpy(var.varn, "RH");
        else if (!strcmp(var.obs, "bias_mr"))  strcpy(var.varn, "MR");
        else if (!strcmp(var.obs, "bias_tp"))  strcpy(var.varn, "TP");
        else if (!strcmp(var.obs, "bias_ws"))  strcpy(var.varn, "WS");
        else if (!strcmp(var.obs, "bias_ps"))  strcpy(var.varn, "PS");
        else if (!strcmp(var.obs, "bias_wbt")) strcpy(var.varn, "WBT");
        else if (!strcmp(var.obs, "bias_gh"))  strcpy(var.varn, "GH");

        vmap_ini_read();
        bias_data_get();
        bias_data_plot(im);
      }
      else {
        gts_data_get();
        gts_data_plot(im);
      }
    }

    // 10. 테두리 그리기
    if (strstr(var.layer,"B")) map_border_disp(im);

    // 11. 이미지 생성
    img_png(im, fname);
    gdImageDestroy(im);
  }

  // 지도 투영법
  rate = (float)(var.NI)/(float)(var.NX);
  map.Re    = 6371.00877;
  map.grid  = var.grid/(zm*rate);
  map.slat1 = 30.0;    map.slat2 = 60.0;
  map.olon  = 126.0;   map.olat  = 38.0;
  map.xo = (float)(var.SX - xo)*(zm*rate);
  map.yo = (float)(var.SY - yo)*(zm*rate);
  map.first = 0;

  x = 0.0;  y = var.NJ;
  lamcproj_ellp(&lon1, &lat1, &x, &y, 1, &map);

  x = var.NI;  y = 0;
  lamcproj_ellp(&lon2, &lat2, &x, &y, 1, &map);

  printf("HTTP/1.0 200 OK\n");
  printf("Content-type: text/plain\n\n");
  printf("#Filename,lat1,lon1,lat2,lon2\n");
  printf("%s,%f,%f,%f,%f", gname, lat1, lon1, lat2, lon2);

  return 0;
}

/*=============================================================================*
 *  격자별 위.경도 생성(북반구 일기도)
 *=============================================================================*/
int map_stg_latlon
(
  int ny,
  int nx
)
{
  struct lamc_parameter map;
  struct ster_parameter map2;
  float  zm = 1.0, xo = 0.0, yo = 0.0, x1, y1, x2, y2;
  float  lon, lat, xx, yy;
  int    ix, iy, zx, zy, mode;
  int    i, j, k, n;
  float  rate = (float)(var.NI)/(float)(var.NX);
  //double PI = 180*DEGRAD;

  // 1. 확대시, 중심위치와 확대비율 계산
  for (i = 0; i < 7; i++, zm *= 1.5) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
    //xo += (float)(var.NX)/8.0*(zx-1)/zm;
    //yo += (float)(var.NY)/8.0*(zy-1)/zm;
    xo += (float)(var.NX)/24.0*(zx-1)/zm;
    yo += (float)(var.NY)/24.0*(zy-1)/zm;
  }

  // 2. 지도 투영법
  map.Re    = 6371.00877;
  map.grid  = var.grid;
  map.slat1 = 30.0;    map.slat2 = 60.0;
  map.olon  = 126.0;   map.olat  = 38.0;
  map.xo = 0;
  map.yo = 0;
  map.first = 0;

  map2.Re    = 6371.00877;
  map2.grid  = var.grid;
  map2.slon  = 120.0;
  map2.slat  = 90.0;
  map2.olon  = 120.0;
  map2.olat  = 90.0;
  map2.xo = (float)(var.SX - xo);
  map2.yo = (float)(var.SY - yo);
  map2.first = 0;
  map2.polar = 0;

  for (j = 0; j <= ny; j++) {
    for (i = 0; i <= nx; i++) {
      xx = i/(zm*rate);
      yy = j/(zm*rate);
      sterproj_ellp(&lon, &lat, &xx, &yy, 1, &map2);
      map_lon[j][i] = lon;
      map_lat[j][i] = lat;
      lamcproj_ellp(&lon, &lat, &xx, &yy, 0, &map);
      map_x[j][i] = xx;
      map_y[j][i] = yy;
      //if (lon < 0) lon += 360.0;
      //lat += 90.0;
    }
  }
}

/*=============================================================================*
 *  격자별 위.경도 생성(전지구 일기도)
 *=============================================================================*/
int map_eqdc_latlon
(
  int ny,
  int nx
)
{
  struct lamc_parameter map;
  struct eqdc_parameter map2;
  float  zm = 1.0, xo = 0.0, yo = 0.0, x1, y1, x2, y2;
  float  lon, lat, xx, yy;
  int    ix, iy, zx, zy, mode;
  int    i, j, k, n;
  float  rate = (float)(var.NI)/(float)(var.NX);
  //double PI = 180*DEGRAD;

  // 1. 확대시, 중심위치와 확대비율 계산
  for (i = 0; i < 7; i++, zm *= 1.5) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
    //xo += (float)(var.NX)/8.0*(zx-1)/zm;
    //yo += (float)(var.NY)/8.0*(zy-1)/zm;
    xo += (float)(var.NX)/24.0*(zx-1)/zm;
    yo += (float)(var.NY)/24.0*(zy-1)/zm;
  }

  // 2. 지도 투영법
  map.Re    = 6371.00877;
  map.grid  = var.grid;
  map.slat1 = 30.0;    map.slat2 = 60.0;
  map.olon  = 126.0;   map.olat  = 38.0;
  map.xo = 0;
  map.yo = 0;
  map.first = 0;

  map2.Re    = 6371.00877;
  map2.grid  = var.grid;
  map2.slon  = 126.0;
  map2.slat  = 0.0;
  map2.olon  = 126.0;
  map2.olat  = 0.0;
  map2.xo = (float)(var.SX - xo);
  map2.yo = (float)(var.SY - yo);
  map2.first = 0;

  for (j = 0; j <= ny; j++) {
    for (i = 0; i <= nx; i++) {
      xx = i/(zm*rate);
      yy = j/(zm*rate);
      eqdcproj(&lon, &lat, &xx, &yy, 1, &map2);
      map_lon[j][i] = lon;
      map_lat[j][i] = lat;
      if (lat < -70) {
        map_x[j][i] = -999;
        map_y[j][i] = -999;
      }
      else {
        lamcproj_ellp(&lon, &lat, &xx, &yy, 0, &map);
        map_x[j][i] = xx;
        map_y[j][i] = yy;
      }
      //if (lon < 0) lon += 360.0;
      //lat += 90.0;
    }
  }
}

/*=============================================================================*
 *  파일명 생성
 *=============================================================================*/
int img_file
(
  char *fname,  /* out:이미지 파일명(저장) */
  char *gname   /* out:이미지 파일명(주소) */
)
{
  char   head[32];
  struct stat st;
  int    YY, MM, DD, HH, MI;
  int    code = 0, seq;

  if (!strcmp(var.layer,"S")) {
    var.seq += 9*60; //UTC->KST
    var.seq_sat = -1;
    for (seq = var.seq; seq >= var.seq-40; seq--) {
      seq2time(seq-9*60, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
      if (!strcmp(var.map,"H2") || !strcmp(var.map,"H3") || !strcmp(var.map,"H4") || !strcmp(var.map,"H5")) {
        if (!strcmp(var.sat,"ir1"))
          strcpy(head, "gk2a_ami_le1b_ir105_ea020lc");
        else if (!strcmp(var.sat,"wv063"))
          strcpy(head, "gk2a_ami_le1b_wv063_ea020lc");
        else if (!strcmp(var.sat,"wv069"))
          strcpy(head, "gk2a_ami_le1b_wv069_ea020lc");
        else if (!strcmp(var.sat,"wv073"))
          strcpy(head, "gk2a_ami_le1b_wv073_ea020lc");
        else
          strcpy(head, "gk2a_ami_le1b_wv063_ea020lc");
        sprintf(fname, "%s/EA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
                GK2A_L1B_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      else if (!strcmp(var.map,"EA_CHT") || !strcmp(var.map,"TP")) {
        if (!strcmp(var.sat,"ir1"))
          strcpy(head, "gk2a_ami_le1b_ir105_tp020lc");
        else if (!strcmp(var.sat,"wv063"))
          strcpy(head, "gk2a_ami_le1b_wv063_tp020lc");
        else if (!strcmp(var.sat,"wv069"))
          strcpy(head, "gk2a_ami_le1b_wv069_tp020lc");
        else if (!strcmp(var.sat,"wv073"))
          strcpy(head, "gk2a_ami_le1b_wv073_tp020lc");
        else 
          strcpy(head, "gk2a_ami_le1b_wv063_tp020lc");
        sprintf(fname, "%s/TP/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
                GK2A_L1B_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      else {
        if (!strcmp(var.sat,"ir1"))
          strcpy(head, "gk2a_ami_le1b_ir105_ela020lc");
        else if (!strcmp(var.sat,"wv063"))
          strcpy(head, "gk2a_ami_le1b_wv063_ela020lc");
        else if (!strcmp(var.sat,"wv069"))
          strcpy(head, "gk2a_ami_le1b_wv069_ela020lc");
        else if (!strcmp(var.sat,"wv073"))
          strcpy(head, "gk2a_ami_le1b_wv073_ela020lc");
        else 
          strcpy(head, "gk2a_ami_le1b_wv063_ela020lc");
        sprintf(fname, "%s/ELA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
                GK2A_L1B_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      code = stat(fname, &st);
      if (code == 0 && st.st_size > 100) {
        var.seq_sat = seq;
        break;
      }
    }

    var.seq -= 9*60; //KST->UTC
    if (var.seq_sat < 0) return -10;

    seq2time(var.seq_sat-9*60, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
    sprintf(fname, "/fct/www/ROOT/img/typ/sat_%s_%s_%04d%02d%02d%02d%02d.png", var.sat, var.map, YY, MM, DD, HH, MI);
    sprintf(gname, "/img/typ/sat_%s_%s_%04d%02d%02d%02d%02d.png", var.sat, var.map, YY, MM, DD, HH, MI);
  }
  else if (!strcmp(var.layer,"R")) {
    var.seq += 9*60; //UTC->KST
    var.seq_rdr = -1;
    for (seq = var.seq; seq >= var.seq-30; seq -= 5) {
      seq2time(seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
      sprintf(fname, "%s/%04d%02d/%02d/RDR_CMP_OTH_%04d%02d%02d%02d%02d.bin.gz",
              RDR_CMP_DIR, YY, MM, DD, YY, MM, DD, HH, MI);
      code = stat(fname, &st);
      if (code == 0 && st.st_size > 100) {
        var.seq_rdr = seq;
        break;
      }
    }

    var.seq -= 9*60; //KST->UTC
    if (var.seq_rdr < 0) return -10;

    seq2time(var.seq_rdr-9*60, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
    sprintf(fname, "/fct/www/ROOT/img/typ/rdr_oth_%s_%04d%02d%02d%02d%02d.png", var.map, YY, MM, DD, HH, MI);
    sprintf(gname, "/img/typ/rdr_oth_%s_%04d%02d%02d%02d%02d.png", var.map, YY, MM, DD, HH, MI);
  }
  else return -10;

  if (stat(fname, &st) < 0) code = -1;
  else if (st.st_size < 30) code = -2;

  return code;
}

/*=============================================================================*
 *  이미지 생성
 *=============================================================================*/
int img_png
(
  gdImagePtr im,
  char *gname
)
{
  FILE *fp;
  if ( (fp = fopen(gname, "wb")) == NULL )
  {
    gdImageDestroy(im);
    return -1;
  }
  else
  {
    gdImagePng(im, fp);
    fclose(fp);
  }

  return 0;
}

/*=============================================================================*
 *  최대 지도영역 확인
 *=============================================================================*/
int map_ini_read()
{
  FILE  *fp;
  char  map_cd[8], buf[300], tmp[300];
  float max_sx, max_sy, max_dx, max_dy, max_nx = 0;
  int   i, j, k, n = 0;

  // 1. 지도설정파일 열기
  if ((fp = fopen(MAP_INI_FILE, "r")) == NULL) return -1;
  
  // 2. 해당되는 지도 정보 읽기
  while (fgets(buf, 300, fp) != NULL) {
    if (buf[0] == '#') continue;

    getword(map_cd, buf, ':');
    if (!strcmp(map_cd, var.map)) {
      getword(tmp, buf, ':');  var.NX = atof(tmp);
      getword(tmp, buf, ':');  var.NY = atof(tmp);
      getword(tmp, buf, ':');  var.SX = atof(tmp);
      getword(tmp, buf, ':');  var.SY = atof(tmp);
    }
  }
  fclose(fp);

  return 0;
}

/*=============================================================================*
 *  지도 모서리 정보 확인
 *=============================================================================*/
int map_edge()
{
  int   i, j, k;
  struct lamc_parameter map;
  struct ster_parameter map2;
  struct eqdc_parameter map3;
  float  zm = 1.0, xo = 0.0, yo = 0.0;
  int    zx, zy;
  float  lon, lat, x, y, lon1, lat1, x1, y1, lon2, lat2, x2, y2;

  //if (!strcmp(var.map, "NHEM")) {
  //  map2.Re    = 6371.00877;
  //  map2.grid  = var.grid;
  //  var.NX = (int)((float)(map2.Re*2*PI/2)/map2.grid);  var.NY = (int)((float)(map2.Re*2*PI/2)/map2.grid);
  //  var.SX = (int)((float)(map2.Re*PI/2)/map2.grid);    var.SY = (int)((float)(map2.Re*PI/2)/map2.grid);
  //}
  //else map_ini_read();

  // 1. 확대시, 중심위치와 확대비율 계산
  for (i = 0; i < 7; i++, zm *= 1.5) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
    xo += (float)(var.NX)/24.0*(zx-1)/zm;
    yo += (float)(var.NY)/24.0*(zy-1)/zm;
  }

  // 2. 지도 투영법
  if (!strcmp(var.map, "NHEM")) {
    map2.Re    = 6371.00877;
    map2.grid  = var.grid/(zm);
    map2.slon  = 120.0;
    map2.slat  = 90.0;
    map2.olon  = 120.0;
    map2.olat  = 90.0;
    map2.xo = (float)(var.SX - xo)*(zm);
    map2.yo = (float)(var.SY - yo)*(zm);
    map2.first = 0;
    map2.polar = 0;
  }
  else if (!strcmp(var.map, "WORLD")) {
    map3.Re    = 6371.00877;
    map3.grid  = var.grid/(zm);
    map3.slon  = 126.0;
    map3.slat  = 0.0;
    map3.olon  = 126.0;
    map3.olat  = 0.0;
    map3.xo = (float)(var.SX - xo)*(zm);
    map3.yo = (float)(var.SY - yo)*(zm);
    map3.first = 0;
  }
  else {
    map.Re    = 6371.00877;
    map.grid  = var.grid/(zm);
    map.slat1 = 30.0;    map.slat2 = 60.0;
    map.olon  = 126.0;   map.olat  = 38.0;
    map.xo = (float)(var.SX - xo)*(zm);
    map.yo = (float)(var.SY - yo)*(zm);
    map.first = 0;
  }

  x = 0;  y = (float)(var.NY);
  x1 = (float)(var.NX);  y1 = 0;
  x2 = (float)(var.NX)/2.;  y2 = (float)(var.NY)/2.;
  if (!strcmp(var.map, "NHEM")) {
    sterproj_ellp(&lon, &lat, &x, &y, 1, &map2);
    sterproj_ellp(&lon1, &lat1, &x1, &y1, 1, &map2);
    sterproj_ellp(&lon2, &lat2, &x2, &y2, 1, &map2);
  }
  else if (!strcmp(var.map, "WORLD")) {
    eqdcproj(&lon, &lat, &x, &y, 1, &map3);
    eqdcproj(&lon1, &lat1, &x1, &y1, 1, &map3);
    eqdcproj(&lon2, &lat2, &x2, &y2, 1, &map3);

    if (lat > 90) lat = 90.;
    else if (lat < -90) lat = -90.;

    if (lat1 > 90) lat1 = 90.;
    else if (lat1 < -90) lat1 = -90.;

    if (lat2 > 90) lat2 = 90.;
    else if (lat2 < -90) lat2 = -90.;
  }
  else {
    lamcproj_ellp(&lon, &lat, &x, &y, 1, &map);
    lamcproj_ellp(&lon1, &lat1, &x1, &y1, 1, &map);
    lamcproj_ellp(&lon2, &lat2, &x2, &y2, 1, &map);
  }

  var.startx = lon;  var.starty = lat;
  var.endx = lon1;   var.endy = lat1;
  var.centerx = lon2;   var.centery = lat2;

  return 0;
}

/*=============================================================================*
 *  지도 영역 테두리 그리기
 *=============================================================================*/
int map_border_disp(gdImagePtr im)
{
  float rate = (float)(var.NI)/(float)(var.NX);
  double font_size = 12.0;
  char   txt[30], txt_utf[30];
  int    brect[8];
  int   x1, y1, x2, y2;
  int   H, S, L, R, G, B;
  int   color_map[100];
  int   i, j, k;
  int   color, color2, color3, color4;
  struct lamc_parameter map;
  struct ster_parameter map2;
  struct eqdc_parameter map3;
  float  zm = 1.0, xo = 0.0, yo = 0.0;
  int    zx, zy;
  float  lon, lat, x, y, lon1, lat1;
  int    ddeg, lon_min, lon_max, lat_min, lat_max;
  //double PI = 180*DEGRAD;

  color = gdImageColorAllocate(im, 255, 255, 255);
  color2 = gdImageColorAllocate(im, 0, 0, 0);
  color3 = gdImageColorAllocate(im, 0, 0, 255);
  color4 = gdImageColorAllocate(im, 255, 0, 0);

  gdImageFilledRectangle(im, 0+var.OI, 0+var.OJ, var.NI+var.OI, 12+var.OJ, color);
  gdImageFilledRectangle(im, 0+var.OI, var.NJ-12+var.OJ, var.NI+var.OI, var.NJ+var.OJ, color);
  gdImageFilledRectangle(im, 0+var.OI, 0+var.OJ, 12+var.OI, var.NJ+var.OJ, color);
  gdImageFilledRectangle(im, var.NI-12+var.OI, 0+var.OJ, var.NI+var.OI, var.NJ+var.OJ, color);

  gdImageLine(im, 0+var.OI, 0+var.OJ, 0+var.OI, var.NJ-1+var.OJ, color2);
  gdImageLine(im, var.NI-1+var.OI, 0+var.OJ, var.NI-1+var.OI, var.NJ-1+var.OJ, color2);
  gdImageLine(im, 0+var.OI, 0+var.OJ, var.NI-1+var.OI, 0+var.OJ, color2);
  gdImageLine(im, 0+var.OI, var.NJ-1+var.OJ, var.NI-1+var.OI, var.NJ-1+var.OJ, color2);

  gdImageLine(im, 12+var.OI, 12+var.OJ, 12+var.OI, var.NJ-12+var.OJ, color2);
  gdImageLine(im, var.NI-12+var.OI, 12+var.OJ, var.NI-12+var.OI, var.NJ-12+var.OJ, color2);
  gdImageLine(im, 12+var.OI, 12+var.OJ, var.NI-12+var.OI, 12+var.OJ, color2);
  gdImageLine(im, 12+var.OI, var.NJ-12+var.OJ, var.NI-12+var.OI, var.NJ-12+var.OJ, color2);

  //if (!strcmp(var.cht_mode, "ana") || !strcmp(var.cht_mode, "ana2")) {
  //  gdImageLine(im, 0, 0, var.GI-1, 0, color4);
  //  gdImageLine(im, 0, 0, 0, var.GJ-1, color4);
  //  gdImageLine(im, 0, var.GJ-1, var.GI-1, var.GJ-1, color4);
  //  gdImageLine(im, var.GI-1, 0, var.GI-1, var.GJ-1, color4);
  //}

  // 1. 확대시, 중심위치와 확대비율 계산
  for (i = 0; i < 7; i++, zm *= 1.5) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
    //xo += (float)(var.NX)/8.0*(zx-1)/zm;
    //yo += (float)(var.NY)/8.0*(zy-1)/zm;
    xo += (float)(var.NX)/24.0*(zx-1)/zm;
    yo += (float)(var.NY)/24.0*(zy-1)/zm;
  }

  // 2. 지도 투영법
  if (!strcmp(var.map, "NHEM")) {
    map2.Re    = 6371.00877;
    map2.grid  = var.grid/(zm*rate);
    map2.slon  = 120.0;
    map2.slat  = 90.0;
    map2.olon  = 120.0;
    map2.olat  = 90.0;
    map2.xo = (float)(var.SX - xo)*(zm*rate);
    map2.yo = (float)(var.SY - yo)*(zm*rate);
    map2.first = 0;
    map2.polar = 0;
  }
  else if (!strcmp(var.map, "WORLD")) {
    map3.Re    = 6371.00877;
    map3.grid  = var.grid/(zm*rate);
    map3.slon  = 126.0;
    map3.slat  = 0.0;
    map3.olon  = 126.0;
    map3.olat  = 0.0;
    map3.xo = (float)(var.SX - xo)*(zm*rate);
    map3.yo = (float)(var.SY - yo)*(zm*rate);
    map3.first = 0;
  }
  else {
    map.Re    = 6371.00877;
    map.grid  = var.grid/(zm*rate);
    map.slat1 = 30.0;    map.slat2 = 60.0;
    map.olon  = 126.0;   map.olat  = 38.0;
    map.xo = (float)(var.SX - xo)*(zm*rate);
    map.yo = (float)(var.SY - yo)*(zm*rate);
    map.first = 0;
  }

  // 3. 위경도 간격
  if (zm > 3)
    ddeg = 5;
  else
    ddeg = 10;

  if (!strcmp(var.map, "WORLD")) ddeg *= 2;
  //printf("%f %d %d %f / %d %d %d %d\n", var.grid, var.NX, var.NY, rate, lon_min, lon_max, lat_min, lat_max);

  // 5. 경도선
  y = var.NJ - 12;
  lon = 0;
  for (x = 12; x <= var.NI-12; x += 1) {
    if (!strcmp(var.map, "NHEM"))      sterproj_ellp(&lon1, &lat1, &x, &y, 1, &map2);
    else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon1, &lat1, &x, &y, 1, &map3);
    else lamcproj_ellp(&lon1, &lat1, &x, &y, 1, &map);
    if (!strcmp(var.map, "NHEM")) {
      if ((int)(lon1*5)%(ddeg*5) != 0 || abs(lon1-lon) < 1) continue;
    }
    else if (!strcmp(var.map, "WORLD")) {
      if ((int)(lon1*2)%(ddeg*2) != 0 || abs(lon1-lon) < 1) continue;
    }
    else {
      if ((int)(lon1)%(ddeg) != 0 || abs(lon1-lon) < 1) continue;
    }
    lon = lon1;
    if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD")) {
      if (lon > 180) sprintf(txt, "%dW", (int)(360-lon+0.5));    
      else if (lon < 0) sprintf(txt, "%dW", (int)(-lon+0.5));   
      else sprintf(txt, "%dE", (int)(lon));  
    }
    else sprintf(txt, "%dE", (int)(lon));    
    gdImageString(im, gdFontSmall, x-strlen(txt)*2+var.OI, -1+var.OJ, txt, color3);  
  }

  y = 12;
  lon = 0;
  for (x = 12; x <= var.NI-12; x += 1) {
    if (!strcmp(var.map, "NHEM")) sterproj_ellp(&lon1, &lat1, &x, &y, 1, &map2);
    else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon1, &lat1, &x, &y, 1, &map3);
    else lamcproj_ellp(&lon1, &lat1, &x, &y, 1, &map);
    if (!strcmp(var.map, "NHEM")) {
      if ((int)(lon1*5)%(ddeg*5) != 0 || abs(lon1-lon) < 1) continue;
    }
    else if (!strcmp(var.map, "WORLD")) {
      if ((int)(lon1*2)%(ddeg*2) != 0 || abs(lon1-lon) < 1) continue;
    }
    else {
      if ((int)(lon1)%(ddeg) != 0 || abs(lon1-lon) < 1) continue;
    }
    lon = lon1;
    if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD")) {
      if (lon > 180) sprintf(txt, "%dW", (int)(360-lon+0.5));  
      else if (lon < 0) sprintf(txt, "%dW", (int)(-lon+0.5));
      else sprintf(txt, "%dE", (int)(lon));  
    }
    else sprintf(txt, "%dE", (int)(lon));        
    gdImageString(im, gdFontSmall, x-strlen(txt)*2+var.OI, -1+var.NJ-12+var.OJ, txt, color3);  
  }

  // 6. 위도선
  x = 12;
  lat = 0;
  for (y = 12; y <= var.NJ-12; y += 1) {
    if (!strcmp(var.map, "NHEM")) sterproj_ellp(&lon1, &lat1, &x, &y, 1, &map2);
    else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon1, &lat1, &x, &y, 1, &map3);
    else lamcproj_ellp(&lon1, &lat1, &x, &y, 1, &map);
    if ((int)(lat1*2)%(ddeg*2) != 0 || abs(lat1-lat) < 1) continue;
    lat = lat1;
    sprintf(txt, "%dN", (int)(lat));    
    gdImageStringUp(im, gdFontSmall, -1+var.OI, var.NJ-y+strlen(txt)*2+var.OJ, txt, color3);  
  }

  x = var.NI - 12;
  lat = 0;
  for (y = 12; y <= var.NJ-12; y += 1) {
    if (!strcmp(var.map, "NHEM")) sterproj_ellp(&lon1, &lat1, &x, &y, 1, &map2);
    else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon1, &lat1, &x, &y, 1, &map3);
    else lamcproj_ellp(&lon1, &lat1, &x, &y, 1, &map);
    if ((int)(lat1*2)%(ddeg*2) != 0 || abs(lat1-lat) < 1) continue;
    lat = lat1;
    sprintf(txt, "%dN", (int)(lat));    
    gdImageStringUp(im, gdFontSmall, -1+var.NI-12+var.OI, var.NJ-y+strlen(txt)*2+var.OJ, txt, color3);  
  }

  return 0;
}

/*=============================================================================*
 *  색상표
 *=============================================================================*/
int color_table(gdImagePtr im, int color_lvl[])
{
  FILE  *fp;
  char  color_file[120];
  float v1;
  int   R, G, B;

  // 5. 기타 색상표 설정
  color_lvl[231] = gdImageColorAllocate(im, 255, 127, 39);
  color_lvl[232] = gdImageColorAllocate(im, 137, 137, 137);
  color_lvl[233] = gdImageColorAllocate(im, 225, 175, 255);
  color_lvl[234] = gdImageColorAllocate(im, 255, 175, 175);
  color_lvl[235] = gdImageColorAllocate(im, 139, 0, 255);
  color_lvl[236] = gdImageColorAllocate(im, 30, 180, 30);     // 녹색1
  color_lvl[237] = gdImageColorAllocate(im, 150, 150, 150);   // 테두리색
  color_lvl[238] = gdImageColorAllocate(im, 250, 250, 00);    // 노란색
  //color_lvl[239] = gdImageColorAllocate(im, 30, 180, 30);     // 녹색
  color_lvl[239] = gdImageColorAllocate(im, 0, 180, 0);     // 녹색2
  //color_lvl[240] = gdImageColorAllocate(im, 180, 180, 180);   // 배경색1
  color_lvl[240] = gdImageColorAllocate(im, 200, 200, 200);   // 배경색1
  color_lvl[241] = gdImageColorAllocate(im, 255, 255, 255);   // 배경색2
  color_lvl[242] = gdImageColorAllocate(im, 30, 30, 30);      // 지도색
  color_lvl[243] = gdImageColorAllocate(im, 12, 28, 236);     // 제목
  color_lvl[244] = gdImageColorAllocate(im, 0, 0, 0);         // 검은색
  color_lvl[245] = gdImageColorAllocate(im, 240, 240, 240);
  color_lvl[246] = gdImageColorAllocate(im, 255, 0, 0);
  color_lvl[247] = gdImageColorAllocate(im, 0, 0, 255);
  color_lvl[248] = gdImageColorAllocate(im, 160, 160, 160);   // 배경색3
  color_lvl[249] = gdImageColorAllocate(im, 110, 110, 110);   // 시군경계
  color_lvl[250] = gdImageColorAllocate(im, 80, 80, 80);      // 지형고도
  color_lvl[251] = gdImageColorAllocate(im, 255, 0, 172);     // 단면선

  return 0;
}

/*=============================================================================*
 *  변환 : {H,S,L} color  --> {R,G,B} color
 *=============================================================================*/
int HSL2RGB(int H, int S, int L, int *R, int *G, int *B)
{
  float S1;
  float Max, Min;
  int   C[3];
  int   i, v;

  S1 = (255.0 - (float)S) * 0.5;
  if (L < 128) {
    Max = (255.0 - S1) / 127.0 * (float)L;
    Min = S1 / 127.0 * (float)L;
  }
  else {
    Min = 255.0 - (255.0 - S1) * (255.0 - (float)L) / 127.0;
    Max = 255.0 - S1 * (255.0 - (float)L) / 127.0;
  }

  for (i = 0; i < 3; i++) {
    if (i == 0)
      v = (H + 85) % 256;
    else if (i == 1)
      v = H;
    else
      v = (H + 170) % 256;

    if (v <= 42)
      C[i] = (int)( (Max - Min) * (float)v / 42.0 + Min );
    else if (v <= 127)
      C[i] = (int)Max;
    else if (v <= 170)
      C[i] = (int)( (Min - Max) * ((float)v - 127.0) / 43.0 + Max );
    else
      C[i] = (int)Min;
  }
  *R = C[0];
  *G = C[1];
  *B = C[2];
  return 0;
} 

/*******************************************************************************
 *  GTS 자료 읽기
 *******************************************************************************/
int gts_data_get()
{
  FILE   *fp;
  URL_FILE *fr; 
  char   buf[1000], url[500], tmp[500], v[30][50], bufr[256];
  char   stn_tp[8], stn_tm[16], flag[16], stn_id[16];
  float  vv, xx, yy, u1, v1;
  float  gh, rn, wd, ws, ta, td, pa, ps, pr, vs, hm, rn_day, ta_min1, ta_max1, tw, ts, wh;
  float  lat, lon, ht, ht_pa, ht_ta, ht_wd, ht_rn;
  int    pt, wc, wp, ca, cd, ch, cl, cm, ct, rh, msg;
  int    YY, MM, DD, HH, MI;
  int    seq, seq1;
  int    qc, code, rtn, now, i, j, k;
  int    GX, GY, SX, SY;
  struct lamc_parameter map;
  struct ster_parameter map2;
  struct eqdc_parameter map3;
  float  zm = 1.0, xo = 0.0, yo = 0.0, x1, y1, x2, y2;
  float  rate = (float)(var.NI)/(float)(var.NX);
  int    zx, zy;

  // 1. 확대시, 중심위치와 확대비율 계산
  for (i = 0; i < 7; i++, zm *= 1.5) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
    //xo += (float)(var.NX)/8.0*(zx-1)/zm;
    //yo += (float)(var.NY)/8.0*(zy-1)/zm;
    xo += (float)(var.NX)/24.0*(zx-1)/zm;
    yo += (float)(var.NY)/24.0*(zy-1)/zm;
  }

  // 2. 지도 투영법
  if (!strcmp(var.map, "NHEM")) {
    map2.Re    = 6371.00877;
    map2.grid  = var.grid/(zm*rate);
    map2.slon  = 120.0;
    map2.slat  = 90.0;
    map2.olon  = 120.0;
    map2.olat  = 90.0;
    map2.xo = (float)(var.SX - xo)*(zm*rate);
    map2.yo = (float)(var.SY - yo)*(zm*rate);
    map2.first = 0;
    map2.polar = 0;
  }
  else if (!strcmp(var.map, "WORLD")) {
    map3.Re    = 6371.00877;
    map3.grid  = var.grid/(zm*rate);
    map3.slon  = 126.0;
    map3.slat  = 0.0;
    map3.olon  = 126.0;
    map3.olat  = 0.0;
    map3.xo = (float)(var.SX - xo)*(zm*rate);
    map3.yo = (float)(var.SY - yo)*(zm*rate);
    map3.first = 0;
  }
  else {
    map.Re    = 6371.00877;
    map.grid  = var.grid/(zm*rate);
    map.slat1 = 30.0;    map.slat2 = 60.0;
    map.olon  = 126.0;   map.olat  = 38.0;
    map.xo = (float)(var.SX - xo)*(zm*rate);
    map.yo = (float)(var.SY - yo)*(zm*rate);
    map.first = 0;
  }

  // 3. 지점별 관측값 읽기
  var.num_stn = 0;
  //var.num_stn_obj = 0;
  //seq2time(var.seq-9*60, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
  seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n'); //UTC
  if (strcmp(var.gts,"SFC") == 0) {
    sprintf(url, "%s/url/gts_cht_syn2.php?tm=%04d%02d%02d%02d%02d&lon1=%f&lon2=%f&lat1=%f&lat2=%f&help=0&aws=1",
            host, YY, MM, DD, HH, MI, var.lon1, var.lon2, var.lat1, var.lat2);
    if ((fr = url_fopen(url, "r"))) {
      while (!url_feof(fr)) {
        url_fgets(buf, sizeof(buf), fr);
        if (buf[0] == '#') continue;

        qc = 0; ts = -99; wh = -99;
        sscanf(buf, "%s %s %s %f %f %f %d %f %f %f %f %d %d %d %d %d %d %d %d %d %f %f %d %f %f %f %f %f %f %f",
               stn_tp, stn_tm, stn_id, &lon, &lat, &ps, &pt, &pr, &ta, &td, &hm,
               &vs, &wc, &wp, &ca, &cd, &ch, &cl, &cm, &ct,
               &wd, &ws, &rh, &rn, &pa, &ta_min1, &ta_max1, &tw, &ts, &wh);

        if (!strcmp(var.obs,"ps")) {
          vv = ps;
          if (vv > 600 && vv < 1200) qc = 1;
        }
        else if (!strcmp(var.obs,"pa")) {
          vv = pa;
          if (vv > 600 && vv < 1200) qc = 1;
        }
        else if (!strcmp(var.obs,"ta")) {
          vv = ta;
          if (vv >= -50) qc = 1;
        }
        else if (!strcmp(var.obs,"td")) {
          vv = td;
          if (vv >= -50) qc = 1;
        }
        else if (!strcmp(var.obs,"wn")) {
          vv = ta - td;
          if (vv >= 0 && vv < 50) qc = 1;
        }
        else if (!strcmp(var.obs,"tw")) {
          vv = tw;
          if (vv >= -50) qc = 1;
        }
        else if (!strcmp(var.obs,"ta_min")) {
          vv = ta_min1;
          if (vv >= -50) qc = 1;
        }
        else if (!strcmp(var.obs,"ta_max")) {
          vv = ta_max1;
          if (vv >= -50) qc = 1;
        }
        else if (!strcmp(var.obs,"hm")) {
          vv = hm;
          if (vv >= 0) qc = 1;
        }
        else if (!strcmp(var.obs,"ca")) {
          vv = ca;
          if (vv >= 0) qc = 1;
        }
        else if (!strcmp(var.obs,"cd")) {
          vv = cd;
          if (vv >= 0) qc = 1;
        }
        else if (!strcmp(var.obs,"r06")) {
          if (rn >= 0 && rh == 1) {
            vv = rn;
            qc = 1;
          }
        }
        else if (!strcmp(var.obs,"r12")) {
          if (rn >= 0 && rh == 2) {
            vv = rn;
            qc = 1;
          }
        }
        else if (!strcmp(var.obs,"ws")) {
          vv = ws;
          if (vv >= 0) qc = 1;
        }
        else if (!strcmp(var.obs,"wd")) {
          if (wd >= 0 && ws >= 0) {
            u1 = -ws * sin( DEGRAD * wd );
            v1 = -ws * cos( DEGRAD * wd );
            qc = 1;
          }
        }
        //else
        //  break;

        // 좌표 변환후 확대시 위치 계산, 영역내 있는지 확인
        if (!strcmp(var.map, "NHEM"))       sterproj_ellp(&lon, &lat, &stn_data[var.num_stn].x, &stn_data[var.num_stn].y, 0, &map2);
        else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon, &lat, &stn_data[var.num_stn].x, &stn_data[var.num_stn].y, 0, &map3);
        else lamcproj_ellp(&lon, &lat, &stn_data[var.num_stn].x, &stn_data[var.num_stn].y, 0, &map);

        strcpy(stn_data[var.num_stn].id, stn_id);
        stn_data[var.num_stn].lon = lon;
        stn_data[var.num_stn].lat = lat;
        stn_data[var.num_stn].wd = wd;
        stn_data[var.num_stn].ws = ws;
        stn_data[var.num_stn].ca = ca;
        stn_data[var.num_stn].cl = cl;
        stn_data[var.num_stn].cm = cm;
        stn_data[var.num_stn].ct = ct;
        stn_data[var.num_stn].cd = cd;
        stn_data[var.num_stn].wp = wp;
        stn_data[var.num_stn].wc = wc;
        stn_data[var.num_stn].ta = ta;
        stn_data[var.num_stn].td = td;
        stn_data[var.num_stn].tw = tw;
        stn_data[var.num_stn].ta_max = ta_max1;
        stn_data[var.num_stn].ta_min = ta_min1;
        stn_data[var.num_stn].ps = ps;
        stn_data[var.num_stn].pa = pa;
        stn_data[var.num_stn].pr = pr;
        stn_data[var.num_stn].pt = pt;
        stn_data[var.num_stn].rn = rn;
        stn_data[var.num_stn].rh = rh;
        stn_data[var.num_stn].hm = hm;
        stn_data[var.num_stn].ts = ts;
        stn_data[var.num_stn].wh = wh;
        strcpy(stn_data[var.num_stn].tp, stn_tp);
/*
        if (qc) {
            strcpy(stn_data_obj[var.num_stn_obj].id, stn_id);
            stn_data_obj[var.num_stn_obj].x = stn_data[var.num_stn].x;
            stn_data_obj[var.num_stn_obj].y = stn_data[var.num_stn].y;
            if (!strcmp(var.obs,"wd")) {
              stn_data_obj[var.num_stn_obj].d = u1;
              stn_data_obj[var.num_stn_obj].v = v1;
            }
            else {
              stn_data_obj[var.num_stn_obj].d = vv;
            }
            stn_data_obj[var.num_stn_obj].wd = wd;
            stn_data_obj[var.num_stn_obj].ws = ws;
            var.num_stn_obj++;
        }
*/
        var.num_stn++;
      }
    }
    url_fclose(fr);
  }

  // 3. 고층자료 읽기
  else {
    sprintf(url, "%s/url/gts_cht_temp.sum.php?pa=%s&tm=%04d%02d%02d%02d%02d&lon1=%f&lon2=%f&lat1=%f&lat2=%f&help=0&wpf=%d&amdar=%d&map=%s",
            host, var.gts, YY, MM, DD, HH, MI, var.lon1, var.lon2, var.lat1, var.lat2, var.wpf, var.amdar, var.map);
    if ((fr = url_fopen(url, "r"))) {
      while (!url_feof(fr)) {
        url_fgets(buf, sizeof(buf), fr);
        if (buf[0] == '#') continue;

        qc = 0;
        sscanf(buf, "%s %s %f %f %f %f %f %f %f %f %s %f %s",
               stn_tm, stn_id, &lon, &lat, &pa, &gh, &ta, &td, &wd, &ws, flag, &ht, bufr);

        if (!strcmp(var.obs,"gh")) {
          vv = gh;
          if (vv >= -90) qc = 1;
        }
        else if (!strcmp(var.obs,"ta")) {
          vv = ta;
          if (vv >= -99) qc = 1;
        }
        else if (!strcmp(var.obs,"td")) {
          vv = td;
          if (vv >= -99) qc = 1;
        }
        else if (!strcmp(var.obs,"wn")) {
          vv = ta - td;
          if (vv >= 0 && vv < 99) qc = 1;
        }
        else if (!strcmp(var.obs,"ws")) {
          vv = ws;
          if (vv > 0) qc = 1;
        }
        else if (!strcmp(var.obs,"wd")) {
          if (wd >= 0 && ws > 0) {
            u1 = -ws * sin( DEGRAD * wd );
            v1 = -ws * cos( DEGRAD * wd );
            qc = 1;
          }
        }
        //else
        //  break;

        // 좌표 변환후 확대시 위치 계산, 영역내 있는지 확인
        if (!strcmp(var.map, "NHEM"))       sterproj_ellp(&lon, &lat, &stn_data[var.num_stn].x, &stn_data[var.num_stn].y, 0, &map2);
        else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon, &lat, &stn_data[var.num_stn].x, &stn_data[var.num_stn].y, 0, &map3);
        else lamcproj_ellp(&lon, &lat, &stn_data[var.num_stn].x, &stn_data[var.num_stn].y, 0, &map);

        strcpy(stn_data[var.num_stn].id, stn_id);
        stn_data[var.num_stn].lon = lon;
        stn_data[var.num_stn].lat = lat;
        stn_data[var.num_stn].gh = gh;
        stn_data[var.num_stn].ta = ta;
        stn_data[var.num_stn].td = td;
        stn_data[var.num_stn].wd = wd;
        stn_data[var.num_stn].ws = ws;
        if (flag[strlen(flag)-1] == 'W') strcpy(stn_data[var.num_stn].tp, "W");
        else if (flag[strlen(flag)-1] == 'A') {
          strcpy(stn_data[var.num_stn].tp, "A");
          strcpy(stn_data[var.num_stn].bufr, bufr);
          //stn_data[var.num_stn].msg = msg;
        }
        else if (flag[0] == '0' || flag[0] == '1') {
          strcpy(stn_data[var.num_stn].tp, "K");
        }
        else strcpy(stn_data[var.num_stn].tp, "U");

        //if (!strcmp(var.obs,"bias_gh")) {
        //  vv = gh;

        //  stn_data[var.num_stn].d = vv;
        //  stn_data[var.num_stn].nwp = get_nwp_data(lon, lat);
        //}
/*
        if (qc) {
            strcpy(stn_data_obj[var.num_stn_obj].id, stn_id);
            stn_data_obj[var.num_stn_obj].x = stn_data[var.num_stn].x;
            stn_data_obj[var.num_stn_obj].y = stn_data[var.num_stn].y;
            if (!strcmp(var.obs,"wd")) {
              stn_data_obj[var.num_stn_obj].d = u1;
              stn_data_obj[var.num_stn_obj].v = v1;
            }
            else {
              stn_data_obj[var.num_stn_obj].d = vv;
            }
            stn_data_obj[var.num_stn_obj].wd = wd;
            stn_data_obj[var.num_stn_obj].ws = ws;
            var.num_stn_obj++;
        }
*/
        var.num_stn++;
      }
    }
    url_fclose(fr);
  }
  return 0;
}

/*******************************************************************************
 *  PM10 자료 읽기
 *******************************************************************************/
int pm10_data_get()
{
  URL_FILE *fr; 
  char   buf[1000], url[500];
  char   stn_tm[8], stn_org[16], stn_id[16];
  int    YY, MM, DD, HH, MI;
  int    pm10;
  int    i, j, k, n;
  float  lon, lat;
  int    GX, GY, SX, SY;
  struct lamc_parameter map;
  struct ster_parameter map2;
  struct eqdc_parameter map3;
  float  zm = 1.0, xo = 0.0, yo = 0.0, x1, y1, x2, y2;
  float  rate = (float)(var.NI)/(float)(var.NX);
  int    zx, zy;

  // 1. 확대시, 중심위치와 확대비율 계산
  for (i = 0; i < 7; i++, zm *= 1.5) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
    //xo += (float)(var.NX)/8.0*(zx-1)/zm;
    //yo += (float)(var.NY)/8.0*(zy-1)/zm;
    xo += (float)(var.NX)/24.0*(zx-1)/zm;
    yo += (float)(var.NY)/24.0*(zy-1)/zm;
  }

  // 2. 지도 투영법
  if (!strcmp(var.map, "NHEM")) {
    map2.Re    = 6371.00877;
    map2.grid  = var.grid/(zm*rate);
    map2.slon  = 120.0;
    map2.slat  = 90.0;
    map2.olon  = 120.0;
    map2.olat  = 90.0;
    map2.xo = (float)(var.SX - xo)*(zm*rate);
    map2.yo = (float)(var.SY - yo)*(zm*rate);
    map2.first = 0;
    map2.polar = 0;
  }
  else if (!strcmp(var.map, "WORLD")) {
    map3.Re    = 6371.00877;
    map3.grid  = var.grid/(zm*rate);
    map3.slon  = 126.0;
    map3.slat  = 0.0;
    map3.olon  = 126.0;
    map3.olat  = 0.0;
    map3.xo = (float)(var.SX - xo)*(zm*rate);
    map3.yo = (float)(var.SY - yo)*(zm*rate);
    map3.first = 0;
  }
  else {
    map.Re    = 6371.00877;
    map.grid  = var.grid/(zm*rate);
    map.slat1 = 30.0;    map.slat2 = 60.0;
    map.olon  = 126.0;   map.olat  = 38.0;
    map.xo = (float)(var.SX - xo)*(zm*rate);
    map.yo = (float)(var.SY - yo)*(zm*rate);
    map.first = 0;
  }

  // 3. 지점별 관측값 읽기
  seq2time(var.seq+9*60, &YY, &MM, &DD, &HH, &MI, 'm', 'n'); //UTC->KST
  sprintf(url, "%s/url/dst_pm10_tot.php?tm=%04d%02d%02d%02d%02d&help=0", host, YY, MM, DD, HH, MI);
  if ((fr = url_fopen(url, "r"))) {
    while (!url_feof(fr)) {
      url_fgets(buf, sizeof(buf), fr);
      if (buf[0] == '#') continue;
      sscanf(buf, "%s %s %s %f %f %d", stn_tm, stn_org, stn_id, &lon, &lat, &pm10);
      n = -1;
      if (!strcmp(stn_org, "kma") || !strcmp(stn_org, "cma")) {
        for (i=0; i<var.num_stn; i++) {
          if (!strcmp(stn_id, stn_data[i].id)) {
            n = i;
            break;
          }
        }

        if (n != -1) stn_data[n].pm10 = pm10;
        else {
          if (!strcmp(var.map, "NHEM"))       sterproj_ellp(&lon, &lat, &stn_data[var.num_stn].x, &stn_data[var.num_stn].y, 0, &map2);
          else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon, &lat, &stn_data[var.num_stn].x, &stn_data[var.num_stn].y, 0, &map3);
          else lamcproj_ellp(&lon, &lat, &stn_data[var.num_stn].x, &stn_data[var.num_stn].y, 0, &map);
          strcpy(stn_data[var.num_stn].id, stn_id);
          stn_data[var.num_stn].lon = lon;
          stn_data[var.num_stn].lat = lat;
          stn_data[var.num_stn].pm10 = pm10;
          if (!strcmp(stn_org, "kma")) {
            strcpy(stn_data[var.num_stn].tp, "DK");
          }
          else if (!strcmp(stn_org, "cma")) {
            if (strstr(stn_data[var.num_stn].id,"A")) strcpy(stn_data[var.num_stn].tp, "DC");
            else {
              strcpy(stn_data[var.num_stn].tp, "S");
              stn_data[var.num_stn].ta = -999;
              stn_data[var.num_stn].td = -999;
              stn_data[var.num_stn].ws = -999;
            }
          }
          var.num_stn++;
        }
      }
      else continue;
    }
    url_fclose(fr);
  }
  return 0;
}

/*=============================================================================*
 *  경도에 따른 풍향 각도 보정(LCC 도법)
 *=============================================================================*/
float get_adj_angle_lcc(float lon)
{
  float LCC_UNIT_ANGLE = 128.80203249251298/180.0;  //  LCC 격자의 경도에 따른 각도 보정을 위한 기본값
  float sLon = 126.0;
  float dDgree, k;

  k = LCC_UNIT_ANGLE;
  if (lon >= 306) {
    dDgree = k*((lon-360.) - sLon);
  }
  else {
    dDgree = k*(lon - sLon);
  }

  return dDgree;
} 

/*******************************************************************************
 *  GTS 자료 그리기
 *******************************************************************************/
int gts_data_plot(
  gdImagePtr im
)
{
  float  xx, yy;
  int    xs, ys, zx, zy;
  int    i, j, ii, jj, bi, bj;
  char   txt[128], txt2[128], txt_utf[128], obs[128];
  double font_size = 7.0;
  int    brect[8];
  int    color_lvl[256];
  float  box, box1 = 12, box2 = 24;
  int    nx_box, ny_box, dd;
  float  **boxs;
  struct lamc_parameter map;
  float  zm = 1.0, xo = 0.0, yo = 0.0, x1, y1, x2, y2;
  float  rate = (float)(var.NI)/(float)(var.NX);
  int    YY, MM, DD, HH, MI;
  int    iYY, iMM, iDD, iHH, iMI;
  int    num_plot = 0, data_no, ok, zoom_level = 0;
  float  adj_wd;

  for (i = 0; i < 7; i++, zoom_level++) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
  }

  // 0. 지도 관련 설정값
  color_table(im, color_lvl); // 색상테이블
  dd = 0; // 테두리의 폭

  // 간벌 설정
  if (!strcmp(var.gts,"SFC")) {
    if (!strcmp(var.obs,"jun")) box = box2;
    else if (!strcmp(var.obs,"wa")) box = 6;
    else box = box1;
  }
  else {
    box = box1;
  }
  //box *= var.font_size;

  nx_box = (int)(var.NI) + 1;
  ny_box = (int)(var.NJ) + 1;
  boxs = matrix(0, ny_box-1, 0, nx_box-1);
  for (j = 0; j < ny_box; j++) {
    for (i = 0; i < nx_box; i++) {
      boxs[j][i] = 0;
    }
  }

  for (i=0; i<var.num_stn; i++) {
    // 좌표 변환후 확대시 위치 계산, 영역내 있는지 확인
    xs = (int)(stn_data[i].x);
    ys = (int)(var.NJ - stn_data[i].y);
    if (stn_data[i].x < 0 || stn_data[i].x > var.NI || stn_data[i].y < 0 || stn_data[i].y > var.NJ) continue;
    if (!strcmp(var.cht_mode, "ana") || !strcmp(var.cht_mode, "ana2")) {
      if (stn_data[i].x < 40 || stn_data[i].x > var.NI - 40 || stn_data[i].y < 40 || stn_data[i].y > var.NJ - 40) continue;
    }

    // 이미지에서 격자 위치
    ii = (int)((xs - dd));
    jj = (int)((ys - dd));

    xs += var.OI;
    ys += var.OJ;

    data_no = 0;

    // 경도에 따른 풍향 각도 보정(도법)
    if (stn_data[i].wd >= 0) {
      stn_data[i].wd_0 = stn_data[i].wd;
      if (!strcmp(var.map,"NHEM")) {
        stn_data[i].wd -= (stn_data[i].lon - 120.0);
        if (stn_data[i].wd > 360) stn_data[i].wd -= 360;
        else if (stn_data[i].wd < 0) stn_data[i].wd += 360;
      }
      else if (strcmp(var.map,"WORLD")) {
        adj_wd = get_adj_angle_lcc(stn_data[i].lon);
        stn_data[i].wd -= adj_wd;
      }
    }

    ok = 1;
    if (var.pnts != 0) {
      for (bj = jj-box; bj < jj+box; bj++) {
        if (bj < 0 || bj >= ny_box) continue;
        for (bi = ii-box; bi < ii+box; bi++) {
          if (bi < 0 || bi >= nx_box) continue;          
          if (boxs[bj][bi] != 0) {
            ok = 0;
            break;
          }
        }
      }
    }

    if (!strcmp(var.gts,"SFC")) {
      if (ok == 1) {
        if (!strcmp(var.obs,"jun")) {
          if (stn_data[i].ca >= 0 || stn_data[i].cl >= 0 || stn_data[i].cm >= 0 || stn_data[i].ct >= 0 ||
              stn_data[i].wc >= 0 || stn_data[i].wp >= 0 || stn_data[i].wd >= 0 || stn_data[i].ws >= 0 ||
              stn_data[i].ps >  0 || stn_data[i].pr >= 0 || stn_data[i].pt >= 0 ||
              stn_data[i].ta >-90 || stn_data[i].td >-90) {
                gts_sfc_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[244], color_lvl[240], zoom_level);
                gts_sfc_ca(im, xs, ys, stn_data[i].tp, stn_data[i].ca, color_lvl[244], color_lvl[240]);
                gts_sfc_ct(im, xs, ys, stn_data[i].cl, stn_data[i].cm, stn_data[i].ct);
                gts_sfc_cd(im, xs, ys, stn_data[i].cd, font_size, color_lvl[244], color_lvl[240]);
                gts_sfc_ta(im, xs, ys, stn_data[i].ta, stn_data[i].td, font_size, color_lvl[244], color_lvl[240]);
                gts_sfc_ww(im, xs, ys, stn_data[i].wc, stn_data[i].wp);
                gts_sfc_ps(im, xs, ys, stn_data[i].ps, stn_data[i].pr, stn_data[i].pt, font_size, color_lvl[244], color_lvl[246], color_lvl[240]);
                gts_sfc_rn(im, xs, ys, stn_data[i].rn, stn_data[i].rh, font_size, color_lvl[244], color_lvl[240]);
                data_no++;
          }
        }
        else if (!strcmp(var.obs,"wd")) {
          if (stn_data[i].wd >= 0 || stn_data[i].ws >= 0) {
                //if (var.color_adjust == 0 || strstr(var.layer,"S")) {
                //  gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[239], color_lvl[237]);
                //}
                //else {
                //  gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[244], color_lvl[237]);
                //}
                gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[244], color_lvl[240], zoom_level);
                data_no++;
          }
        }
        else if (!strcmp(var.obs,"wa")) {
          if (stn_data[i].wd >= 0 || stn_data[i].ws >= 0) {
                gts_sfc_wd_arrow(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[246], color_lvl[234]);
                data_no++;
          }
        }
        else if (!strcmp(var.obs,"ca")) {
          if (stn_data[i].ca >= 0) {
                gts_sfc_ca(im, xs, ys, stn_data[i].tp, stn_data[i].ca, color_lvl[244], color_lvl[240]);
                data_no++;
          }
        }
        else if (!strcmp(var.obs,"wc")) {
          if (stn_data[i].wc >= 4) {
                gts_sfc_wc(im, xs, ys, stn_data[i].wc, 'W');
                data_no++;
          }
        }
        else if (!strcmp(var.obs,"ctm")) {
          if (stn_data[i].cm > 0 && stn_data[i].cm < 10) {
                gts_sfc_wc(im, xs, ys, stn_data[i].cm, 'M');
                data_no++;
          }
        }
        else if (!strcmp(var.obs,"ctt")) {
          if (stn_data[i].ct > 0 && stn_data[i].ct < 10) {
                gts_sfc_wc(im, xs, ys, stn_data[i].ct, 'T');
                data_no++;
          }
        }
        else if (!strcmp(var.obs,"ctl")) {
          if (stn_data[i].cl > 0 && stn_data[i].cl < 10) {
                gts_sfc_wc(im, xs, ys, stn_data[i].cl, 'L');
                data_no++;
          }
        }
        else if (!strcmp(var.obs,"dust")) {
          strcpy(txt, "");
          if (stn_data[i].pm10 > 0) sprintf(txt, "%d", stn_data[i].pm10);

          if (strlen(txt) > 0) data_no++;

          if ((stn_data[i].wc >= 6 && stn_data[i].wc <= 9) || (stn_data[i].wc >= 30 && stn_data[i].wc <= 35)) {
            if (data_no == 0) {
              gts_sfc_wc(im, xs, ys, stn_data[i].wc, 'W');
              data_no++;
            }
            else gts_sfc_wc(im, xs-12, ys+3, stn_data[i].wc, 'W');
          }

          if (strlen(txt) > 0) gts_txt(im, xs, ys, txt, font_size, color_lvl[244], color_lvl[247], color_lvl[240]);
        }
        else {
          strcpy(txt, "");
          if (!strcmp(var.obs,"id")) sprintf(txt, "%s", stn_data[i].id);
          else if (!strcmp(var.obs,"ps") && (stn_data[i].ps > 0)) sprintf(txt, "%.1f", stn_data[i].ps);
          else if (!strcmp(var.obs,"pa") && (stn_data[i].pa > 0)) sprintf(txt, "%.1f", stn_data[i].pa);
          else if (!strcmp(var.obs,"ta") && (stn_data[i].ta > -90)) sprintf(txt, "%.1f", stn_data[i].ta);
          else if (!strcmp(var.obs,"td") && (stn_data[i].td > -90)) sprintf(txt, "%.1f", stn_data[i].td);
          else if (!strcmp(var.obs,"wn") && (stn_data[i].ta > -90 && stn_data[i].td > -90)) sprintf(txt, "%.1f", stn_data[i].ta-stn_data[i].td);
          else if (!strcmp(var.obs,"tw") && (stn_data[i].tw > -90)) sprintf(txt, "%.1f", stn_data[i].tw);
          else if (!strcmp(var.obs,"ws") && (stn_data[i].ws >= 0))  sprintf(txt, "%.1f", stn_data[i].ws);
          else if (!strcmp(var.obs,"hm") && (stn_data[i].hm >= 0)) sprintf(txt, "%.1f", stn_data[i].hm);
          else if (!strcmp(var.obs,"wcv") && (stn_data[i].wc >= 0)) sprintf(txt, "%d", stn_data[i].wc);
          else if (!strcmp(var.obs,"cav") && (stn_data[i].ca >= 0)) sprintf(txt, "%d", stn_data[i].ca);
          else if (!strcmp(var.obs,"cdv") && (stn_data[i].cd >= 0)) sprintf(txt, "%d", stn_data[i].cd);
          else if (!strcmp(var.obs,"min") && (stn_data[i].ta_min > -90)) sprintf(txt, "%.1f", stn_data[i].ta_min);
          else if (!strcmp(var.obs,"max") && (stn_data[i].ta_max > -90)) sprintf(txt, "%.1f", stn_data[i].ta_max);
          else if (!strcmp(var.obs,"r06") && (stn_data[i].rn > 0 && (stn_data[i].rh == 1 || stn_data[i].rh == 6)))  sprintf(txt, "%.1f", stn_data[i].rn);
          else if (!strcmp(var.obs,"r12") && (stn_data[i].rn > 0 && (stn_data[i].rh == 2 || stn_data[i].rh == 12))) sprintf(txt, "%.1f", stn_data[i].rn);
          else if (!strcmp(var.obs,"ts")) {
            if (stn_data[i].ts > -90) sprintf(txt, "%.1f", stn_data[i].ts);
            else if (stn_data[i].tw > -90) sprintf(txt, "%.1f", stn_data[i].tw);
          }
          else if (!strcmp(var.obs,"wh") && (stn_data[i].wh > -90)) sprintf(txt, "%.1f", stn_data[i].wh);

          if (strlen(txt) > 0) {
            gts_txt(im, xs, ys, txt, font_size, color_lvl[244], color_lvl[247], color_lvl[240]);
            data_no++;
          }
        }
        if (data_no) {
          if (var.flag == 1 && !strcmp(var.cht_mode, "gts")) {
            if (num_plot > 0) printf(",\n");
            printf("    {\"stn_id\":\"%s\"", stn_data[i].id);
            printf(", \"stn_tp\":\"%s\"", stn_data[i].tp);
            printf(", \"x\":\"%d\"", xs);
            printf(", \"y\":\"%d\"", ys);
            printf(", \"wd\":\"%.0f\"", stn_data[i].wd_0);
            printf(", \"ws\":\"%.1f\"", stn_data[i].ws);
            printf(", \"ta\":\"%.1f\"", stn_data[i].ta);
            printf(", \"td\":\"%.1f\"", stn_data[i].td);
            printf(", \"ps\":\"%.1f\"", stn_data[i].ps);
            printf(", \"pm10\":\"%d\"", stn_data[i].pm10);
            printf(", \"wh\":\"%.1f\"", stn_data[i].wh);
            printf("}");
          }
          num_plot++;

          if (var.pnts != 0) {
            for (bj = jj-box; bj < jj+box; bj++) {
              if (bj < 0 || bj >= ny_box) continue;
              for (bi = ii-box; bi < ii+box; bi++) {
                if (bi < 0 || bi >= nx_box) continue;          
                boxs[bj][bi]++;
              }
            }
          }
        }
      }
    }
    else {
      if (ok == 1) {
        if (!strcmp(var.obs,"jun")) {
          if (stn_data[i].wd >= 0 || stn_data[i].ws >= 0 || stn_data[i].gh > 0 || stn_data[i].ta >-90 || stn_data[i].td >-90) {
            //if (var.color_adjust == 0 || strstr(var.layer,"S")) {
            //  gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[239], color_lvl[237]);
            //}
            //else {
            //  gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[244], color_lvl[237]);
            //}
            if (!strcmp(var.cht_mode, "gts") && var.color_wind == 1) {
              if (!strcmp(stn_data[i].tp, "W")) gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[235], color_lvl[233], zoom_level);
              else if (!strcmp(stn_data[i].tp, "A")) gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[247], color_lvl[233], zoom_level);
              else gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[246], color_lvl[234], zoom_level);
            }
            else gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[244], color_lvl[240], zoom_level);
            gts_upp_ta(im, xs, ys, stn_data[i].ta, stn_data[i].td, font_size, color_lvl[244], color_lvl[240]);
            gts_upp_gh(im, xs, ys, stn_data[i].gh, font_size, color_lvl[244], color_lvl[240]);
            data_no++;
          }
        }
        else if (!strcmp(var.obs,"wd")) {
          if (stn_data[i].wd >= 0 || stn_data[i].ws >= 0) {
            //if (var.color_adjust == 0 || strstr(var.layer,"S")) {
            //  gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[239], color_lvl[237]);
            //}
            //else {
            //  gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[244], color_lvl[237]);
            //}
            if (!strcmp(var.cht_mode, "gts") && var.color_wind == 1) {
              if (!strcmp(stn_data[i].tp, "W")) gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[235], color_lvl[233], zoom_level);
              else if (!strcmp(stn_data[i].tp, "A")) gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[247], color_lvl[233], zoom_level);
              else gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[246], color_lvl[234], zoom_level);
            }
            else gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[244], color_lvl[240], zoom_level);
            data_no++;
          }
        }
        else {
          strcpy(txt, "");
          if (!strcmp(var.obs,"id")) sprintf(txt, "%s", stn_data[i].id);
          else if (!strcmp(var.obs,"nm")) sprintf(txt, "%s", stn_data[i].id);
          else if (!strcmp(var.obs,"gh") && (stn_data[i].gh > 0))  sprintf(txt, "%d", (int)(stn_data[i].gh));
          else if (!strcmp(var.obs,"ta") && (stn_data[i].ta > -90)) sprintf(txt, "%.1f", stn_data[i].ta);
          else if (!strcmp(var.obs,"td") && (stn_data[i].td > -90)) sprintf(txt, "%.1f", stn_data[i].td);
          else if (!strcmp(var.obs,"wn") && (stn_data[i].ta > -90 && stn_data[i].td > -90)) sprintf(txt, "%.1f", stn_data[i].ta-stn_data[i].td);
          else if (!strcmp(var.obs,"ws") && (stn_data[i].ws >= 0))  sprintf(txt, "%.1f", stn_data[i].ws);
          else if (!strcmp(var.obs,"ept") && (stn_data[i].ta > -90 && stn_data[i].td > -90)) sprintf(txt, "%.1f", get_ept(stn_data[i].ta, stn_data[i].td, atof(var.gts)));

          if (strlen(txt) > 0) {
            gts_txt(im, xs, ys, txt, font_size, color_lvl[244], color_lvl[247], color_lvl[240]);
            data_no++;
          }
        }
        if (data_no) {
          if (var.flag == 1 && !strcmp(var.cht_mode, "gts")) {
            if (num_plot > 0) printf(",\n");
            printf("    {\"stn_id\":\"%s\"", stn_data[i].id);
            printf(", \"stn_tp\":\"%s\"", stn_data[i].tp);
            if (!strcmp(stn_data[i].tp, "A")) {
              printf(", \"bufr\":\"%s\"", stn_data[i].bufr);
              //printf(", \"msg\":\"%d\"", stn_data[i].msg);
            }
            printf(", \"x\":\"%d\"", xs);
            printf(", \"y\":\"%d\"", ys);
            printf(", \"wd\":\"%.0f\"", stn_data[i].wd_0);
            printf(", \"ws\":\"%.1f\"", stn_data[i].ws);
            printf(", \"ta\":\"%.1f\"", stn_data[i].ta);
            printf(", \"td\":\"%.1f\"", stn_data[i].td);
            printf(", \"gh\":\"%.1f\"}", stn_data[i].gh);
          }
          num_plot++;

          if (var.pnts != 0) {
            for (bj = jj-box; bj < jj+box; bj++) {
              if (bj < 0 || bj >= ny_box) continue;
              for (bi = ii-box; bi < ii+box; bi++) {
                if (bi < 0 || bi >= nx_box) continue;          
                boxs[bj][bi]++;
              }
            }
          }
        }
      }
    }
  }

  if (var.gis == 1) {
    return;
  }
  // 제목 표시
  //seq2time(var.seq-9*60, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
  //seq2time(var.seq, &iYY, &iMM, &iDD, &iHH, &iMI, 'm', 'n');
  if (!strstr(var.layer, "A") || !strcmp(var.cht_mode, "gts")) {
    seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n'); //UTC
    seq2time(var.seq+9*60, &iYY, &iMM, &iDD, &iHH, &iMI, 'm', 'n'); //KST

    strcpy(obs, "");
    if      (strstr(var.obs, "gh"))  strcpy(obs, "GPH(m)");
    else if (strstr(var.obs, "ta"))  strcpy(obs, "Temp.(C)");
    else if (strstr(var.obs, "td"))  strcpy(obs, "DewT.(C)");
    else if (strstr(var.obs, "wn"))  strcpy(obs, "TA-TD(C)");
    else if (strstr(var.obs, "ws"))  strcpy(obs, "WS(m/s)");
    else if (strstr(var.obs, "mr"))  strcpy(obs, "MR(g/Kg)");
    else if (strstr(var.obs, "hm"))  strcpy(obs, "HM(%)");
    else if (strstr(var.obs, "ps"))  strcpy(obs, "PSL(hPa)");
    else if (strstr(var.obs, "wbt")) strcpy(obs, "WBT(C)");
    else if (strstr(var.obs, "tp"))  strcpy(obs, "Poten.T(C)");

    if (strlen(obs) > 0) {
      if (!strcmp(var.gts,"SFC")) sprintf(txt, "OBS - %04d.%02d.%02d.%02dUTC(%02d.%02dKST) / %s / %s / %d#(%d#)", YY, MM, DD, HH, iDD, iHH, var.gts, obs, var.num_stn, num_plot);
      else sprintf(txt, "OBS - %04d.%02d.%02d.%02dUTC(%02d.%02dKST) / %shPa / %s / %d#(%d#)", YY, MM, DD, HH, iDD, iHH, var.gts, obs, var.num_stn, num_plot);
    }
    else {
      if (!strcmp(var.gts,"SFC")) sprintf(txt, "OBS - %04d.%02d.%02d.%02dUTC(%02d.%02dKST) / %s / %d#(%d#)", YY, MM, DD, HH, iDD, iHH, var.gts, var.num_stn, num_plot);
      else sprintf(txt, "OBS - %04d.%02d.%02d.%02dUTC(%02d.%02dKST) / %shPa / %d#(%d#)", YY, MM, DD, HH, iDD, iHH, var.gts, var.num_stn, num_plot);
    }
    gdImageFilledRectangle(im, 12, 12, strlen(txt)*7+8+12, 20+12, color_lvl[241]);
    gdImageRectangle(im, 12, 12, strlen(txt)*7+8+12, 20+12, color_lvl[244]);
    gdImageString(im, gdFontMediumBold, 5+12, 3+12, txt, color_lvl[244]);  

    if (!strcmp(var.cht_mode, "gts") && var.size > 950) {
      if (((strstr(var.layer, "A") && !strstr(var.cht_name, "gts")) || strstr(var.obs, "bias") || (strstr(var.sat, "rm") && strstr(var.layer, "S"))) && var.seq >= var.seq_fc) {
        seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n'); //UTC
        seq2time(var.seq_fc+9*60, &iYY, &iMM, &iDD, &iHH, &iMI, 'm', 'n'); //KST
        sprintf(txt, "%s / TIME: %04d.%02d.%02d.%02dUTC(%02d.%02dKST) / VALID: +%dh", var.model, YY, MM, DD, HH, iDD, iHH, (int)(((int)(var.seq/(3*60))*(3*60) - var.seq_fc)/60));
        if (strstr(var.sat,"rm") && strstr(var.layer,"S")) strcat(txt, "            ");

        if (strstr(var.cht_name, "diff_pres") || strstr(var.cht_name, "diff_temp")) {
          gdImageFilledRectangle(im, var.NI-12-(strlen(txt)*7+8), 12, var.NI-12, 35+12, color_lvl[241]);
          gdImageRectangle(im, var.NI-12-(strlen(txt)*7+8), 12, var.NI-12, 35+12, color_lvl[244]);
          gdImageLine(im, var.NI-12-strlen(txt)*7+35, 43, var.NI-12-strlen(txt)*7+52, 43, color_lvl[244]);
          strcpy(txt2, "등치선:       GTS 관측전문 내외삽(실선) / ...... 수치모델(점선)");
          for (i = 0; i < 128; i++) txt_utf[i] = 0;
          euckr2utf(txt2, txt_utf);
          gdImageStringFT(im, &brect[0], color_lvl[244], FONTTTF, 9.0, 0.0, var.NI-12-(strlen(txt)*7)-10+6, 30+12, txt_utf);
        }
        else {
          gdImageFilledRectangle(im, var.NI-12-(strlen(txt)*7+8), 12, var.NI-12, 20+12, color_lvl[241]);
          gdImageRectangle(im, var.NI-12-(strlen(txt)*7+8), 12, var.NI-12, 20+12, color_lvl[244]);
        }

        gdImageString(im, gdFontMediumBold, var.NI-12-(strlen(txt)*7+8)+5, 3+12, txt, color_lvl[244]);  
        if (strstr(var.sat,"rm") && strstr(var.layer,"S")) {
          sprintf(txt2, "(구름모의 +%dh)", (int)((var.seq-var.seq_fc)/60));
          for (i = 0; i < 128; i++) txt_utf[i] = 0;
          euckr2utf(txt2, txt_utf);
          gdImageStringFT(im, &brect[0], color_lvl[244], FONTTTF, 8.5, 0.0, var.NI-12-(strlen(txt2)*5)-10, 26, txt_utf);
        }
      }
    }
  }

  return 0;
}

/*******************************************************************************
 *  수치모델 BIAS 자료 읽기
 *******************************************************************************/
int bias_data_get()
{
  FILE   *fp;
  URL_FILE *fr; 
  char   buf[1000], url[500], tmp[500], v[30][50];
  char   stn_tp[8], stn_tm[16], flag[16], stn_id[16];
  float  vv, xx, yy, u1, v1;
  float  gh, rn, wd, ws, ta, td, pa, ps, pr, vs, hm, rn_day, ta_min1, ta_max1, tw, wd1, ws1;
  float  lat, lon, ht, ht_pa, ht_ta, ht_wd, ht_rn;
  int    pt, wc, wp, ca, cd, ch, cl, cm, ct, rh;
  int    YY, MM, DD, HH, MI;
  int    YY1, MM1, DD1, HH1, MI1;
  int    seq, seq1;
  int    qc, code, rtn, now, i, j, k;
  int    GX, GY, SX, SY;
  struct lamc_parameter map;
  struct ster_parameter map2;
  struct eqdc_parameter map3;
  float  zm = 1.0, xo = 0.0, yo = 0.0, x1, y1, x2, y2;
  float  rate = (float)(var.NI)/(float)(var.NX);
  int    zx, zy;
  float  obs, nwp;

  // 1. 확대시, 중심위치와 확대비율 계산
  for (i = 0; i < 7; i++, zm *= 1.5) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
    //xo += (float)(var.NX)/8.0*(zx-1)/zm;
    //yo += (float)(var.NY)/8.0*(zy-1)/zm;
    xo += (float)(var.NX)/24.0*(zx-1)/zm;
    yo += (float)(var.NY)/24.0*(zy-1)/zm;
  }

  // 2. 지도 투영법
  if (!strcmp(var.map, "NHEM")) {
    map2.Re    = 6371.00877;
    map2.grid  = var.grid/(zm*rate);
    map2.slon  = 120.0;
    map2.slat  = 90.0;
    map2.olon  = 120.0;
    map2.olat  = 90.0;
    map2.xo = (float)(var.SX - xo)*(zm*rate);
    map2.yo = (float)(var.SY - yo)*(zm*rate);
    map2.first = 0;
    map2.polar = 0;
  }
  else if (!strcmp(var.map, "WORLD")) {
    map3.Re    = 6371.00877;
    map3.grid  = var.grid/(zm*rate);
    map3.slon  = 126.0;
    map3.slat  = 0.0;
    map3.olon  = 126.0;
    map3.olat  = 0.0;
    map3.xo = (float)(var.SX - xo)*(zm*rate);
    map3.yo = (float)(var.SY - yo)*(zm*rate);
    map3.first = 0;
  }
  else {
    map.Re    = 6371.00877;
    map.grid  = var.grid/(zm*rate);
    map.slat1 = 30.0;    map.slat2 = 60.0;
    map.olon  = 126.0;   map.olat  = 38.0;
    map.xo = (float)(var.SX - xo)*(zm*rate);
    map.yo = (float)(var.SY - yo)*(zm*rate);
    map.first = 0;
  }

  // 3. 지점별 관측값 읽기
  var.num_stn = 0;
  //var.num_stn_obj = 0;
  seq2time(var.seq,    &YY, &MM, &DD, &HH, &MI, 'm', 'n'); //UTC
  seq2time(var.seq_fc, &YY1, &MM1, &DD1, &HH1, &MI1, 'm', 'n'); //UTC

  //sprintf(url, "%s/cgi-bin/url/nph-nwp_bias_gts_txt1?gts=%s&obs=%s&model=%s&tm=%04d%02d%02d%02d00&tm_fc=%04d%02d%02d%02d00&lon1=%f&lon2=%f&lat1=%f&lat2=%f&help=0",
  //          host, var.gts, var.obs, var.model, YY, MM, DD, HH, YY1, MM1, DD1, HH1, var.lon1, var.lon2, var.lat1, var.lat2);
  sprintf(url, "http://cht.kma.go.kr/cgi-bin/url/nph-nwp_bias_gts_txt1?gts=%s&obs=%s&model=%s&tm=%04d%02d%02d%02d00&tm_fc=%04d%02d%02d%02d00&lon1=%f&lon2=%f&lat1=%f&lat2=%f&help=0&wpf=%d&amdar=%d&map=%s",
            var.gts, var.obs, var.model, YY, MM, DD, HH, YY1, MM1, DD1, HH1, var.lon1, var.lon2, var.lat1, var.lat2, var.wpf, var.amdar, var.map);

  if ((fr = url_fopen(url, "r"))) {
    while (!url_feof(fr)) {
      url_fgets(buf, sizeof(buf), fr);
      if (buf[0] == '#') continue;

      if (!strcmp(var.gts,"SFC")) {
        sscanf(buf, "%s %s %s %f %f %f %f", stn_tp, stn_tm, stn_id, &lon, &lat, &obs, &nwp);
        strcpy(stn_data[var.num_stn].tp, stn_tp);
      }
      else {
        if (!strcmp(var.obs, "bias_ws")) {
          sscanf(buf, "%s %s %s %f %f %f %f %f %f", stn_tp, stn_tm, stn_id, &lon, &lat, &obs, &nwp, &wd, &wd1);
          ws = obs;
          ws1 = nwp;
        }
        else sscanf(buf, "%s %s %s %f %f %f %f", stn_tp, stn_tm, stn_id, &lon, &lat, &obs, &nwp);
        strcpy(stn_data[var.num_stn].tp, stn_tp);
      }

      // 좌표 변환후 확대시 위치 계산, 영역내 있는지 확인
      if (!strcmp(var.map, "NHEM"))       sterproj_ellp(&lon, &lat, &stn_data[var.num_stn].x, &stn_data[var.num_stn].y, 0, &map2);
      else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon, &lat, &stn_data[var.num_stn].x, &stn_data[var.num_stn].y, 0, &map3);
      else lamcproj_ellp(&lon, &lat, &stn_data[var.num_stn].x, &stn_data[var.num_stn].y, 0, &map);

      strcpy(stn_data[var.num_stn].tm, stn_tm);
      strcpy(stn_data[var.num_stn].id, stn_id);
      stn_data[var.num_stn].lon = lon;
      stn_data[var.num_stn].lat = lat;
      stn_data[var.num_stn].v   = obs;
      stn_data[var.num_stn].nwp = nwp;
      if (!strcmp(var.obs, "bias_ws") && strcmp(var.gts,"SFC")) {
        stn_data[var.num_stn].wd = wd;
        stn_data[var.num_stn].ws = ws;
        stn_data[var.num_stn].wd1 = wd1;
        stn_data[var.num_stn].ws1 = ws1;
      }

      var.num_stn++;
    }

    url_fclose(fr);
  }

  return 0;
}

/*******************************************************************************
 *  수치모델 BIAS 자료 그리기
 *******************************************************************************/
int bias_data_plot(
  gdImagePtr im
)
{
  float  xx, yy, adj_wd;
  int    xs, ys, zx, zy;
  int    i, j, ii, jj, bi, bj;
  char   txt[128], txt2[128], txt_utf[128], value[128], obs[128];
  double font_size = 7.0;
  int    brect[8];
  int    color_lvl[256];
  float  box, box1 = 3, box2 = 12;
  int    nx_box, ny_box, dd, wh;
  float  **boxs;
  struct lamc_parameter map;
  float  zm = 1.0, xo = 0.0, yo = 0.0, x1, y1, x2, y2;
  float  rate = (float)(var.NI)/(float)(var.NX);
  int    YY, MM, DD, HH, MI;
  int    iYY, iMM, iDD, iHH, iMI;
  int    num_plot = 0, data_no, ok;
  int    color_diff, color_img_red[10], color_img_blue[10]; 
  float  diff, dp[6], dn[6], vbar, cbh1, cbh2, vbh;
  int    nbar, color, zoom_level = 0;

  for (i = 0; i < 7; i++, zoom_level++) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
  }

  // 0. 지도 관련 설정값
  color_table(im, color_lvl); // 색상테이블
  dd = 0; // 테두리의 폭

  // 간벌 설정
  if (var.bias_disp == 0) box = box1;
  else box = box2;

  nx_box = (int)(var.NI) + 1;
  ny_box = (int)(var.NJ) + 1;
  boxs = matrix(0, ny_box-1, 0, nx_box-1);
  for (j = 0; j < ny_box; j++) {
    for (i = 0; i < nx_box; i++) {
      boxs[j][i] = 0;
    }
  }

  // 색상 및 크기 설정
  //빨간색 (번호가 클수록 진해짐)
  color_img_red[0]= gdImageColorAllocate(im, 252, 241, 245);
  color_img_red[1]= gdImageColorAllocate(im, 255, 216, 218);
  color_img_red[2]= gdImageColorAllocate(im, 255, 166, 166);
  color_img_red[3]= gdImageColorAllocate(im, 254, 113, 112);
  color_img_red[4]= gdImageColorAllocate(im, 255,  61,  61);
  color_img_red[5]= gdImageColorAllocate(im, 255,  12,  13);
  color_img_red[6]= gdImageColorAllocate(im, 211,   0,   0);
  color_img_red[7]= gdImageColorAllocate(im, 184,   0,   0);
  color_img_red[8]= gdImageColorAllocate(im, 157,   0,   0);

  //파란색 (번호가 클수록 진해짐)
  color_img_blue[0]= gdImageColorAllocate(im, 241, 240, 255);
  color_img_blue[1]= gdImageColorAllocate(im, 216, 217, 255);
  color_img_blue[2]= gdImageColorAllocate(im, 166, 164, 255);
  color_img_blue[3]= gdImageColorAllocate(im, 111, 114, 252);
  color_img_blue[4]= gdImageColorAllocate(im,  66,  64, 255);
  color_img_blue[5]= gdImageColorAllocate(im,  13,  14, 255);
  color_img_blue[6]= gdImageColorAllocate(im,   0,   0, 211);
  color_img_blue[7]= gdImageColorAllocate(im,   1,   0, 176);
  color_img_blue[8]= gdImageColorAllocate(im,   2,   0, 138);

  wh = 7;

  dp[0] = var.diff_det;  dn[0] = -1.0*var.diff_det;
  for (i=1; i<=5; i++) {
    dp[i] = var.diff_max/5.0*(float)(i);  dn[i] = -1.0*dp[i];
  }

  for (i=0; i<var.num_stn; i++) {
    // 좌표 변환후 확대시 위치 계산, 영역내 있는지 확인
    xs = (int)(stn_data[i].x);
    ys = (int)(var.NJ - stn_data[i].y);
    if (stn_data[i].x < 0 || stn_data[i].x > var.NI || stn_data[i].y < 0 || stn_data[i].y > var.NJ) continue;

    // 이미지에서 격자 위치
    ii = (int)((xs - dd));
    jj = (int)((ys - dd));

    xs += var.OI;
    ys += var.OJ;

    data_no = 0;

    ok = 1;
    if (var.pnts != 0) {
      for (bj = jj-box; bj < jj+box; bj++) {
        if (bj < 0 || bj >= ny_box) continue;
        for (bi = ii-box; bi < ii+box; bi++) {
          if (bi < 0 || bi >= nx_box) continue;          
          if (boxs[bj][bi] != 0) {
            ok = 0;
            break;
          }
        }
      }
    }

    if (ok == 1) {
      diff = stn_data[i].nwp - stn_data[i].v;

      if      (diff >= dp[5])                 color_diff = color_img_red[6];
      else if (diff >= dp[4] && diff < dp[5]) color_diff = color_img_red[5];
      else if (diff >= dp[3] && diff < dp[4]) color_diff = color_img_red[4];
      else if (diff >= dp[2] && diff < dp[3]) color_diff = color_img_red[3];
      else if (diff >= dp[1] && diff < dp[2]) color_diff = color_img_red[2];
      else if (diff >= dp[0] && diff < dp[1]) color_diff = color_img_red[1];
      else if (diff >=  0.0  && diff < dp[0]) color_diff = color_img_red[0];

      else if (diff >= dn[0] && diff <  0.0)  color_diff = color_img_blue[0];
      else if (diff >= dn[1] && diff < dn[0]) color_diff = color_img_blue[1];
      else if (diff >= dn[2] && diff < dn[1]) color_diff = color_img_blue[2];
      else if (diff >= dn[3] && diff < dn[2]) color_diff = color_img_blue[3];
      else if (diff >= dn[4] && diff < dn[3]) color_diff = color_img_blue[4];
      else if (diff >= dn[5] && diff < dn[4]) color_diff = color_img_blue[5];
      else if (diff <  dn[5])                 color_diff = color_img_blue[6];

      //if (var.border > 0) {
      //  gdImageArc(im, xs-1, ys, wh, wh, 0, 360, color_lvl[237]);
      //  gdImageArc(im, xs+1, ys, wh, wh, 0, 360, color_lvl[237]);
      //  gdImageArc(im, xs, ys-1, wh, wh, 0, 360, color_lvl[237]);
      //  gdImageArc(im, xs, ys+1, wh, wh, 0, 360, color_lvl[237]);
      //}
      if (!strcmp(var.obs, "bias_ta") || !strcmp(var.obs, "bias_td") || !strcmp(var.obs, "bias_wbt")) {
        if (stn_data[i].nwp > -90 && stn_data[i].v > -90) {
          if (var.bias_disp == 0) gdImageFilledArc(im, xs, ys, wh, wh, 0, 360, color_diff, gdArc);
          else if (var.bias_disp == 1) {
            sprintf(txt, "%.1f", diff);
            gts_txt(im, xs, ys, txt, font_size, color_diff, color_lvl[247], color_lvl[240]);
          }
          else if (var.bias_disp == 2) {
            sprintf(txt, "%.1f", diff);
            gts_txt(im, xs, ys, txt, font_size, color_lvl[244], color_lvl[247], color_lvl[240]);
          }
          data_no++;
        }
      }
      else {
        if (stn_data[i].nwp >= 0 && stn_data[i].v >= 0) {
          if (!strcmp(var.obs, "bias_ws") && strcmp(var.gts,"SFC") && (stn_data[i].wd >= 0 || stn_data[i].ws >= 0)) {
            // 경도에 따른 풍향 각도 보정(도법)
            if (stn_data[i].wd >= 0) {
              stn_data[i].wd_0 = stn_data[i].wd;
              stn_data[i].wd1_0 = stn_data[i].wd1;
              if (!strcmp(var.map,"NHEM")) {
                stn_data[i].wd -= (stn_data[i].lon - 120.0);
                if (stn_data[i].wd > 360) stn_data[i].wd -= 360;
                else if (stn_data[i].wd < 0) stn_data[i].wd += 360;

                stn_data[i].wd1 -= (stn_data[i].lon - 120.0);
                if (stn_data[i].wd1 > 360) stn_data[i].wd1 -= 360;
                else if (stn_data[i].wd1 < 0) stn_data[i].wd1 += 360;
              }
              else if (strcmp(var.map,"WORLD")) {
                adj_wd = get_adj_angle_lcc(stn_data[i].lon);
                stn_data[i].wd -= adj_wd;
                stn_data[i].wd1 -= adj_wd;
              }
            }
            gts_upp_wd(im, xs, ys, stn_data[i].wd1, stn_data[i].ws1, color_lvl[239], color_lvl[240], zoom_level);
            if (!strcmp(var.cht_mode, "gts") && var.color_wind == 1) {
              if (!strcmp(stn_data[i].tp, "W")) gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[235], color_lvl[233], zoom_level);
              else if (!strcmp(stn_data[i].tp, "A")) gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[247], color_lvl[233], zoom_level);
              else gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[246], color_lvl[234], zoom_level);
            }
            else gts_upp_wd(im, xs, ys, stn_data[i].wd, stn_data[i].ws, color_lvl[244], color_lvl[240], zoom_level);
          }
          if (var.bias_disp == 0) gdImageFilledArc(im, xs, ys, wh, wh, 0, 360, color_diff, gdArc);
          else if (var.bias_disp == 1) {
            sprintf(txt, "%.1f", diff);
            gts_txt(im, xs, ys, txt, font_size, color_diff, color_lvl[247], color_lvl[240]);
          }
          else if (var.bias_disp == 2) {
            sprintf(txt, "%.1f", diff);
            gts_txt(im, xs, ys, txt, font_size, color_lvl[244], color_lvl[247], color_lvl[240]);
          }
          data_no++;
        }
      }

      if (data_no) {
        if (var.flag == 1 && !strcmp(var.cht_mode, "gts")) {
          if (num_plot > 0) printf(",\n");
          printf("    {\"stn_id\":\"%s\"", stn_data[i].id);
          printf(", \"stn_tp\":\"%s\"", stn_data[i].tp);
          printf(", \"x\":\"%d\"", xs);
          printf(", \"y\":\"%d\"", ys);
          printf(", \"fct\":\"%.1f\"", stn_data[i].nwp);
          printf(", \"obs\":\"%.1f\"", stn_data[i].v);
          if (!strcmp(var.obs, "bias_ws")) {
            printf(", \"wd\":\"%.1f\"", stn_data[i].wd_0);
            printf(", \"wd1\":\"%.1f\"", stn_data[i].wd1_0);
          }
          printf(", \"bias\":\"%.1f\"}", diff);
        }
        num_plot++;

        if (var.pnts != 0) {
          for (bj = jj-box; bj < jj+box; bj++) {
            if (bj < 0 || bj >= ny_box) continue;
            for (bi = ii-box; bi < ii+box; bi++) {
              if (bi < 0 || bi >= nx_box) continue;          
              boxs[bj][bi]++;
            }
          }
        }
      }
    }
  }

  if (var.gis == 1) {
    if (!strcmp(var.cht_mode, "gts") && var.flag == 1 && var.bias_disp < 2) {
      if (var.legend == 0) {
        printf("\n  ],\n");
        printf("  \"legend\": [\n");
      }
      else {
        printf(",\n");
      }

      printf("    { \"type\":\"bias\",");
      strcpy(obs, "");
      if      (strstr(var.obs, "gh"))  strcpy(obs, "m");
      else if (strstr(var.obs, "ta"))  strcpy(obs, "C");
      else if (strstr(var.obs, "td"))  strcpy(obs, "C");
      else if (strstr(var.obs, "wn"))  strcpy(obs, "C");
      else if (strstr(var.obs, "ws"))  strcpy(obs, "m/s");
      else if (strstr(var.obs, "mr"))  strcpy(obs, "g/Kg");
      else if (strstr(var.obs, "hm"))  strcpy(obs, "%");
      else if (strstr(var.obs, "ps"))  strcpy(obs, "hPa");
      else if (strstr(var.obs, "wbt")) strcpy(obs, "C");
      else if (strstr(var.obs, "tp"))  strcpy(obs, "C");
      printf(" \"unit\":\"%s\",\n", obs);
      printf("      \"data\": [\n");
      nbar = 14;
      for (i = 1; i <= nbar; i++) {
        switch (i)
        {
          case 1:
            color = color_img_blue[6]; vbar = dn[5];
            break;
          case 2:
            color = color_img_blue[5]; vbar = dn[4];
            break;
          case 3:
            color = color_img_blue[4]; vbar = dn[3];
            break;
          case 4:
            color = color_img_blue[3]; vbar = dn[2];
            break;
          case 5:
            color = color_img_blue[2]; vbar = dn[1];
            break;
          case 6:
            color = color_img_blue[1]; vbar = dn[0];
            break;
          case 7:
            color = color_img_blue[0]; vbar = 0;    
            break;
          case 8:
            color = color_img_red[0];  vbar = 0;    
            break;
          case 9:
            color = color_img_red[1];  vbar = dp[0];
            break;
          case 10:
            color = color_img_red[2];  vbar = dp[1];
            break;
          case 11:
            color = color_img_red[3];  vbar = dp[2];
            break;
          case 12:
            color = color_img_red[4];  vbar = dp[3];
            break;
          case 13:
            color = color_img_red[5];  vbar = dp[4];
            break;
          case 14:
            color = color_img_red[6];  vbar = dp[5];
            break;
        }

        if (i != 1) printf(",\n");
        printf("        {\"R\":\"%d\"", color/(256*256));
        printf(", \"G\":\"%d\"", (color%(256*256))/(256));
        printf(", \"B\":\"%d\"", color%(256));
        printf(", \"value\":\"%.1f\"}", vbar);
      }
      printf("\n      ]\n");
      printf("    }");

      var.legend += 1;
    }

    return;
  }

  // 제목 표시
  if (!strstr(var.layer, "A") || !strcmp(var.cht_mode, "gts")) {
    seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n'); //UTC
    seq2time(var.seq+9*60, &iYY, &iMM, &iDD, &iHH, &iMI, 'm', 'n'); //KST

    strcpy(obs, "");
    if      (strstr(var.obs, "gh"))  strcpy(obs, "GPH(m)");
    else if (strstr(var.obs, "ta"))  strcpy(obs, "Temp.(C)");
    else if (strstr(var.obs, "td"))  strcpy(obs, "DewT.(C)");
    else if (strstr(var.obs, "wn"))  strcpy(obs, "TA-TD(C)");
    else if (strstr(var.obs, "ws"))  strcpy(obs, "WS(m/s)");
    else if (strstr(var.obs, "mr"))  strcpy(obs, "MR(g/Kg)");
    else if (strstr(var.obs, "hm"))  strcpy(obs, "HM(%)");
    else if (strstr(var.obs, "ps"))  strcpy(obs, "PSL(hPa)");
    else if (strstr(var.obs, "wbt")) strcpy(obs, "WBT(C)");
    else if (strstr(var.obs, "tp"))  strcpy(obs, "Poten.T(C)");

    if (strlen(obs) > 0) {
      if (!strcmp(var.gts,"SFC")) sprintf(txt, "BIAS - %04d.%02d.%02d.%02dUTC(%02d.%02dKST) / %s / %s / %d#(%d#)", YY, MM, DD, HH, iDD, iHH, var.gts, obs, var.num_stn, num_plot);
      else sprintf(txt, "BIAS - %04d.%02d.%02d.%02dUTC(%02d.%02dKST) / %shPa / %s / %d#(%d#)", YY, MM, DD, HH, iDD, iHH, var.gts, obs, var.num_stn, num_plot);
    }
    else {
      if (!strcmp(var.gts,"SFC")) sprintf(txt, "BIAS - %04d.%02d.%02d.%02dUTC(%02d.%02dKST) / %s / %d#(%d#)", YY, MM, DD, HH, iDD, iHH, var.gts, var.num_stn, num_plot);
      else sprintf(txt, "BIAS - %04d.%02d.%02d.%02dUTC(%02d.%02dKST) / %shPa / %d#(%d#)", YY, MM, DD, HH, iDD, iHH, var.gts, var.num_stn, num_plot);
    }
    gdImageFilledRectangle(im, 12, 12, strlen(txt)*7+8+12, 20+12, color_lvl[241]);
    gdImageRectangle(im, 12, 12, strlen(txt)*7+8+12, 20+12, color_lvl[244]);
    gdImageString(im, gdFontMediumBold, 5+12, 3+12, txt, color_lvl[244]);  

    if (!strcmp(var.cht_mode, "gts") && var.size > 950) {
      if (((strstr(var.layer, "A") && !strstr(var.cht_name, "gts")) || strstr(var.obs, "bias") || (strstr(var.sat, "rm") && strstr(var.layer, "S"))) && var.seq >= var.seq_fc) {
        seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n'); //UTC
        seq2time(var.seq_fc+9*60, &iYY, &iMM, &iDD, &iHH, &iMI, 'm', 'n'); //KST
        sprintf(txt, "%s / TIME: %04d.%02d.%02d.%02dUTC(%02d.%02dKST) / VALID: +%dh", var.model, YY, MM, DD, HH, iDD, iHH, (int)(((int)(var.seq/(3*60))*(3*60) - var.seq_fc)/60));
        if (strstr(var.sat,"rm") && strstr(var.layer,"S")) strcat(txt, "            ");

        if (strstr(var.cht_name, "diff_pres") || strstr(var.cht_name, "diff_temp")) {
          gdImageFilledRectangle(im, var.NI-12-(strlen(txt)*7+8), 12, var.NI-12, 35+12, color_lvl[241]);
          gdImageRectangle(im, var.NI-12-(strlen(txt)*7+8), 12, var.NI-12, 35+12, color_lvl[244]);
          gdImageLine(im, var.NI-12-strlen(txt)*7+35, 43, var.NI-12-strlen(txt)*7+52, 43, color_lvl[244]);
          strcpy(txt2, "등치선:       GTS 관측전문 내외삽(실선) / ...... 수치모델(점선)");
          for (i = 0; i < 128; i++) txt_utf[i] = 0;
          euckr2utf(txt2, txt_utf);
          gdImageStringFT(im, &brect[0], color_lvl[244], FONTTTF, 9.0, 0.0, var.NI-12-(strlen(txt)*7)-10+6, 30+12, txt_utf);
        }
        else {
          gdImageFilledRectangle(im, var.NI-12-(strlen(txt)*7+8), 12, var.NI-12, 20+12, color_lvl[241]);
          gdImageRectangle(im, var.NI-12-(strlen(txt)*7+8), 12, var.NI-12, 20+12, color_lvl[244]);
        }

        gdImageString(im, gdFontMediumBold, var.NI-12-(strlen(txt)*7+8)+5, 3+12, txt, color_lvl[244]);  
        if (strstr(var.sat,"rm") && strstr(var.layer,"S")) {
          sprintf(txt2, "(구름모의 +%dh)", (int)((var.seq-var.seq_fc)/60));
          for (i = 0; i < 128; i++) txt_utf[i] = 0;
          euckr2utf(txt2, txt_utf);
          gdImageStringFT(im, &brect[0], color_lvl[244], FONTTTF, 8.5, 0.0, var.NI-12-(strlen(txt2)*5)-10, 26, txt_utf);
        }
      }
    }
  }

  // 범례 표시
  if (!strcmp(var.cht_mode, "gts") && strstr(var.obs, "bias") && var.bias_disp < 2) {
    nbar = 14;
    for (i = 1; i <= nbar; i++) {
      switch (i)
      {
        case 1:
          color = color_img_blue[6]; vbar = dn[5];  vbh = (float)(var.NJ-1)/12.0*11.0;  cbh1 = (float)(var.NJ-1)/12.0*12.0;  cbh2 = (float)(var.NJ-1)/12.0*11.0;
          break;
        case 2:
          color = color_img_blue[5]; vbar = dn[4];  vbh = (float)(var.NJ-1)/12.0*10.0;  cbh1 = (float)(var.NJ-1)/12.0*11.0;  cbh2 = (float)(var.NJ-1)/12.0*10.0;
          break;
        case 3:
          color = color_img_blue[4]; vbar = dn[3];  vbh = (float)(var.NJ-1)/12.0*9.0;  cbh1 = (float)(var.NJ-1)/12.0*10.0;  cbh2 = (float)(var.NJ-1)/12.0*9.0;
          break;
        case 4:
          color = color_img_blue[3]; vbar = dn[2];  vbh = (float)(var.NJ-1)/12.0*8.0;  cbh1 = (float)(var.NJ-1)/12.0*9.0;  cbh2 = (float)(var.NJ-1)/12.0*8.0;
          break;
        case 5:
          color = color_img_blue[2]; vbar = dn[1];  vbh = (float)(var.NJ-1)/12.0*7.0;  cbh1 = (float)(var.NJ-1)/12.0*8.0;  cbh2 = (float)(var.NJ-1)/12.0*7.0;
          break;
        case 6:
          color = color_img_blue[1]; vbar = dn[0];  vbh = (float)(var.NJ-1)/12.0*6.2;  cbh1 = (float)(var.NJ-1)/12.0*7.0;  cbh2 = (float)(var.NJ-1)/12.0*6.2;
          break;
        case 7:
          color = color_img_blue[0]; vbar = 0;      vbh = (float)(var.NJ-1)/12.0*6.0;  cbh1 = (float)(var.NJ-1)/12.0*6.2;  cbh2 = (float)(var.NJ-1)/12.0*6.0;
          break;
        case 8:
          color = color_img_red[0];  vbar = 0;      vbh = (float)(var.NJ-1)/12.0*6.0;  cbh1 = (float)(var.NJ-1)/12.0*6.0;  cbh2 = (float)(var.NJ-1)/12.0*5.8;
          break;
        case 9:
          color = color_img_red[1];  vbar = dp[0];  vbh = (float)(var.NJ-1)/12.0*5.8;  cbh1 = (float)(var.NJ-1)/12.0*5.8;  cbh2 = (float)(var.NJ-1)/12.0*5.0;
          break;
        case 10:
          color = color_img_red[2];  vbar = dp[1];  vbh = (float)(var.NJ-1)/12.0*5.0;  cbh1 = (float)(var.NJ-1)/12.0*5.0;  cbh2 = (float)(var.NJ-1)/12.0*4.0;
          break;
        case 11:
          color = color_img_red[3];  vbar = dp[2];  vbh = (float)(var.NJ-1)/12.0*4.0;  cbh1 = (float)(var.NJ-1)/12.0*4.0;  cbh2 = (float)(var.NJ-1)/12.0*3.0;
          break;
        case 12:
          color = color_img_red[4];  vbar = dp[3];  vbh = (float)(var.NJ-1)/12.0*3.0;  cbh1 = (float)(var.NJ-1)/12.0*3.0;  cbh2 = (float)(var.NJ-1)/12.0*2.0;
          break;
        case 13:
          color = color_img_red[5];  vbar = dp[4];  vbh = (float)(var.NJ-1)/12.0*2.0;  cbh1 = (float)(var.NJ-1)/12.0*2.0;  cbh2 = (float)(var.NJ-1)/12.0*1.0;
          break;
        case 14:
          color = color_img_red[6];  vbar = dp[5];  vbh = (float)(var.NJ-1)/12.0*1.0;  cbh1 = (float)(var.NJ-1)/12.0*1.0;  cbh2 = 0;
          break;
      }

      gdImageFilledRectangle(im, var.GI-45, var.OJ+cbh2, var.GI-30, var.OJ+cbh1, color);
      gdImageRectangle(im, var.GI-45, var.OJ+cbh2, var.GI-30, var.OJ+cbh1, color_lvl[240]);

      if ((int)(fabs(vbar)*10 + 0.5) % 10 == 0) {
        if (vbar >= 0) sprintf(value, "%d", (int)(vbar+0.5));
        else sprintf(value, "%d", (int)(vbar-0.5));
      }
      else if ((int)(fabs(vbar)*100 + 0.5) % 10 == 0) {
        sprintf(value, "%.1f", vbar);
      }
      else {
        sprintf(value, "%.2f", vbar);
      }
      gdImageString(im, gdFontSmall, var.GI-27, var.OJ+vbh-6, value, color_lvl[244]);
    }
  }

  return 0;
}

/*=============================================================================*
 *  설정값 불러오기
 *=============================================================================*/
int vmap_ini_read()
{
  FILE  *fp;
  char  buf[1000], tmp[300], model[8], level[8], varn[8];
  float max_sx, max_sy, max_dx, max_dy, max_nx = 0;
  int   i, j, k, n = 0;

  // 1. 지도설정파일 열기
  if ((fp = fopen(VMAP_INI_FILE, "r")) == NULL) return -1;
  
  // 2. 해당되는 지도 정보 읽기
  while (fgets(buf, 1000, fp) != NULL) {
    if (buf[0] == '#') continue;

    getword(model, buf, ':');
    getword(level, buf, ':');
    getword(varn, buf, ':');
    if (!strcmp(model, var.model) && !strcmp(level, var.gts) && !strcmp(varn, var.varn)) {
      getword(tmp, buf, ':');
      getword(tmp, buf, ':');  //var.maxv = atof(tmp);
      getword(tmp, buf, ':');  //var.minv = atof(tmp);
      getword(tmp, buf, ':');  //var.scale = atof(tmp);
      getword(tmp, buf, ':');
      getword(tmp, buf, ':');
      getword(tmp, buf, ':');
      getword(tmp, buf, ':');
      getword(tmp, buf, ':');  var.diff_max = atof(tmp);
      getword(tmp, buf, ':');  var.diff_det = atof(tmp);
      break;
    }
  }

  return 0;
} 

/*******************************************************************************
 *  범례 표출
 *******************************************************************************/
int map_legend_disp(gdImagePtr im, int color_lvl[])
{
  int    i, j, k;
  char   value[32];

  if (strstr(var.layer, "S") && (!strcmp(var.sat, "lst") || !strcmp(var.sat, "sst") || !strcmp(var.sat, "ctt") || !strcmp(var.sat, "cth"))) {
    if (var.gis == 1) {
      if (var.flag == 1) {
        if (var.legend == 0) {
          printf("\n  ],\n");
          printf("  \"legend\": [\n");
        }
        else {
          printf(",\n");
        }

        printf("    { \"type\":\"sat\",");
        if (!strcmp(var.sat, "cth")) {
          printf(" \"unit\":\"%s\",\n", "km");
        }
        else {
          printf(" \"unit\":\"%s\",\n", "C");
        }
        printf("      \"data\": [\n");
        for (i = num_color_sat; i > 0; i--) {
          if (i != num_color_sat) printf(",\n");
          printf("        {\"R\":\"%d\"", color_sat[num_color_sat-i]/(256*256));
          printf(", \"G\":\"%d\"", (color_sat[num_color_sat-i]%(256*256))/(256));
          printf(", \"B\":\"%d\"", color_sat[num_color_sat-i]%(256));
          printf(", \"value\":\"%.1f\"}", data_sat[num_color_sat-i]);
        }
        printf("\n      ]\n");
        printf("    }");

        var.legend += 1;
      }
    }
    else {
      gdImageFilledRectangle(im, 12+var.OJ, var.NJ-27+var.OJ, 58+var.OJ, var.NJ-12+var.OJ, color_lvl[241]);
      gdImageRectangle(im, 12+var.OJ, var.NJ-27+var.OJ, 58+var.OJ, var.NJ-12+var.OJ, 0);
      if (!strcmp(var.sat, "cth")) gdImageString(im, gdFontSmall, 15+var.OJ, var.NJ-26+var.OJ, "sat(km)", 0);
      else gdImageString(im, gdFontSmall, 15+var.OJ, var.NJ-26+var.OJ, "sat(C)", 0);

      for (i = 0; i < num_color_sat; i++) {
        gdImageFilledRectangle(im, 58+var.OJ+(float)(var.NI-70)*((float)(num_color_sat-i-1)/(float)(num_color_sat)), var.NJ-27+var.OJ, 58+var.OJ+(float)(var.NI-70)*((float)(num_color_sat-i)/(float)(num_color_sat)), var.NJ-12+var.OJ, color_sat[num_color_sat-i-1]);
        gdImageRectangle(im, 58+var.OJ+(float)(var.NI-70)*((float)(num_color_sat-i-1)/(float)(num_color_sat)), var.NJ-27+var.OJ, 58+var.OJ+(float)(var.NI-70)*((float)(num_color_sat-i)/(float)(num_color_sat)), var.NJ-12+var.OJ, 0);

        if ((int)(fabs(data_sat[num_color_sat-i-1])*10 + 0.5) % 10 == 0) {
          if (data_sat[num_color_sat-i-1] >= 0) sprintf(value, "%d", (int)(data_sat[num_color_sat-i-1]+0.5));
          else sprintf(value, "%d", (int)(data_sat[num_color_sat-i-1]-0.5));
        }
        else if ((int)(fabs(data_sat[num_color_sat-i-1])*100 + 0.5) % 10 == 0) {
          sprintf(value, "%.1f", data_sat[num_color_sat-i-1]);
        }
        else {
          sprintf(value, "%.2f", data_sat[num_color_sat-i-1]);
        }
        gdImageString(im, gdFontSmall, 61+var.OJ+(float)(var.NI-70)*((float)(num_color_sat-i-1)/(float)(num_color_sat)), var.NJ-26+var.OJ, value, 0);
      }
    }
  }

  if (strstr(var.layer, "R")) {
    if (var.gis == 1) {
      if (var.flag == 1) {
        if (var.legend == 0) {
          printf("\n  ],\n");
          printf("  \"legend\": [\n");
        }
        else {
          printf(",\n");
        }

        printf("    { \"type\":\"rdr\",");
        printf(" \"unit\":\"%s\",\n", "mm/h");
        printf("      \"data\": [\n");
        for (i = num_color_rdr; i > 0; i--) {
          if (i != num_color_rdr) printf(",\n");
          printf("        {\"R\":\"%d\"", color_rdr[num_color_rdr-i]/(256*256));
          printf(", \"G\":\"%d\"", (color_rdr[num_color_rdr-i]%(256*256))/(256));
          printf(", \"B\":\"%d\"", color_rdr[num_color_rdr-i]%(256));
          printf(", \"value\":\"%.1f\"}", data_rdr[num_color_rdr-i]);
        }
        printf("\n      ]\n");
        printf("    }");

        var.legend += 1;
      }
    }
  }

  return 0;
}