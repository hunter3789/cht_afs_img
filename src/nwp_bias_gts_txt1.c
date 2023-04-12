/*******************************************************************************
**
**  수치모델 - GTS 관측 간 BIAS 산출
**
**=============================================================================*
**
**     o 작성자 : 이창재 (2020.06.17)
**
********************************************************************************/
#include "nwp_bias_gts.h"

// 변수
struct INPUT_VAR var;  // 지점 자료
struct STN_VAL stn_data[MAX_STN];  // 지점 자료
char   host[256];

// 함수
/*******************************************************************************
 *
 *  MAIN
 *
 *******************************************************************************/
int main()
{
  int   err = 0;

  // 1. 초기화
  setvbuf(stdout, NULL, _IONBF, 0);
  alarm(30);

  printf("HTTP/1.0 200 OK\n");
  printf("Content-type: text/plain\n\n");

  // 2. 사용자 입력 변수 분석
  if ( user_input() < 0 ) {
    printf("user_input() error\n");
    return -1;
  }
  gethostname(host, sizeof(host));

  // 3. 자료 표출
  nwp_bias_disp();

  alarm(0);
  return 0;
}

/*******************************************************************************
 *  격자자료 웹 이미지 표출시 사용자 요청 분석 부분
 *******************************************************************************/
int user_input() {
  char *qs;
  char tmp[256], item[32], value[32], tm[30], tm_fc[30];
  int  iYY, iMM, iDD, iHH, iMI, iSS;
  int  iseq, i, j, k;

  // 1. 변수 초기값 : 자료별 처리 프로그램에서 각자 상황에 맞게 설정
  var.lat1 = 0;
  var.lat2 = 70;
  var.lon1 = 50;
  var.lon2 = 180;
  var.calc_max = 0;
  var.wpf = 0;
  var.amdar = 0;
  strcpy(var.map, "EA_CHT");

  // 2. GET 방식으로 전달된 사용자 입력변수들의 해독
  qs = getenv ("QUERY_STRING");
  if (qs == NULL) return -1;

  for (i = 0; qs[0] != '\0'; i++) {
    getword (value, qs, '&');
    getword (item, value, '=');
    if (strlen(value) == 0) continue;

    if ( !strcmp(item,"tm"))            strcpy(tm, value);
    else if ( !strcmp(item,"tm_fc"))    strcpy(tm_fc, value);
    else if ( !strcmp(item,"gts"))      strcpy(var.gts, value);
    else if ( !strcmp(item,"obs"))      strcpy(var.obs, value);
    else if ( !strcmp(item,"varn"))     strcpy(var.varn, value);
    else if ( !strcmp(item,"model"))    strcpy(var.model, value);
    else if ( !strcmp(item,"lat1"))     var.lat1 = atof(value);
    else if ( !strcmp(item,"lat2"))     var.lat2 = atof(value);
    else if ( !strcmp(item,"lon1"))     var.lon1 = atof(value);
    else if ( !strcmp(item,"lon2"))     var.lon2 = atof(value);
    else if ( !strcmp(item,"calc_max")) var.calc_max = atoi(value);
    else if ( !strcmp(item,"wpf"))      var.wpf = atoi(value);
    else if ( !strcmp(item,"amdar"))    var.amdar = atoi(value);
    else if ( !strcmp(item,"map"))      strcpy(var.map, value);
  }

  // 3. 기본값 설정
  var.grid = 1.0;
  
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

#include "./calc_var.c"
/*******************************************************************************
 *  MAIN
 *******************************************************************************/
int nwp_bias_disp()
{

  if (var.seq < var.seq_fc) return 0;
  if (var.seq % (3*60) != 0) return 0;

  // 1. 자료 조회
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
  else if (!strcmp(var.obs, "bias_ept")) strcpy(var.varn, "EPT");

  if (var.calc_max != 1) gts_data_get();
  if (var.lon1 < 0) {
    var.lon1 += 180;
    var.lon2 += 180;
  }

  if (vmap_ini_read() < 0) return -1;

  if (!strcmp(var.model,"UM")) {
    um_data_get();
  }
  else if (!strcmp(var.model,"ECMWF")) {
    ecmwf_data_get();
  }
  else if (!strcmp(var.model,"KIMG")) {
    kimg_data_get();
  }

  if (var.calc_max != 1) nwp_bias_data();

  return 0;
}

/*******************************************************************************
 *  UM 전구모델 자료 가져오기
 *******************************************************************************/
int um_data_get()
{
  URL_FILE   *fp;
  FILE       *fp2;
  int        YY, MM, DD, HH, MI, SS, seq_bk, seq_bk2, seq_bk3;
  char       buf[1000], url[500], mod_varn_arr[MAX_VARN][32];
  char       filename[512], idx[8], stn_ko[32];
  int        stn_id[NUM_ASOS], stn_x[NUM_ASOS], stn_y[NUM_ASOS], x, y, id, num_stn;
  int        i, j, k, n, i1, j1, n1, k1, nx, ny, tmp;
  int        X1, X2, Y1, Y2;
  int        level, ok = 0;
  float      ***g, g2[10], g3, g4, max;
  float      uu1, vv1, ws, wd;
  //------------------------------------------------------------------

  seq_bk  = time2seq(2018, 6, 7, 0, 0, 'm');
  seq_bk2 = time2seq(2016, 7, 1, 0, 0, 'm');
  seq_bk3 = time2seq(2011, 6, 1, 0, 0, 'm');
  if (var.seq_fc >= seq_bk) {
    nx = UM_N128_NX;
    ny = UM_N128_NY;
  }
  else if (var.seq_fc >= seq_bk2) {
    nx = UM_N768_NX;
    ny = UM_N768_NY;
  }
  else if (var.seq_fc >= seq_bk3) {
    nx = UM_N512_NX;
    ny = UM_N512_NY;
  }
  else {
    nx = UM_N320_NX;
    ny = UM_N320_NY;
  }

  X1 = (int)((var.lon1/(360.0/nx)+0.5))+1;
  X2 = (int)((var.lon2/(360.0/nx)+0.5))+1;
  Y1 = (int)(((var.lat1+90.0)/(180.0/(ny-1))+0.5))+1;
  Y2 = (int)(((var.lat2+90.0)/(180.0/(ny-1))+0.5))+1;

  if (X1 < 1) X1 = 1;
  if (X2 < 1) X2 = 1;
  if (Y1 < 1) Y1 = 1;
  if (Y2 < 1) Y2 = 1;
  if (X1 >= nx) X1 = nx-1;
  if (X2 >= nx) X2 = nx-1;
  if (Y1 >= ny) Y1 = ny-1;
  if (Y2 >= ny) Y2 = ny-1;

  strcpy(buf, var.mod_varn);

  k = 0;
  for (i = 0; buf[0] != '\0'; i++) {
    getword(mod_varn_arr[k], buf, ',');
    //printf("%s\n", mod_varn_arr[k]);
    k++;
  }

  // 배열을 설정한다.
  g=(float ***) malloc((unsigned) (Y2-Y1+1)*sizeof(float **));
  for(j=0; j<Y2-Y1+1; j++) {
    g[j]=(float **) malloc((unsigned) (X2-X1+1)*sizeof(float *));
  }
  for(j=0; j<Y2-Y1+1; j++) {
    for(i=0; i<X2-X1+1; i++) {
      g[j][i]=(float *) malloc((unsigned) (k)*sizeof(float));
    }
  }

  if (!strcmp(var.gts,"SFC")) level = 0;
  else level = atoi(var.gts);
  seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n');

  for (n = 0; n < k; n++) {
    if (var.seq_fc >= seq_bk) {
      sprintf(url, "%s/cgi-bin/url/nph-um_grib_xy_txt1?group=UMGL&nwp=N128&data=%c&varn=%s&level=%d&map=S&sub=%d,%d,%d,%d&sm=0&tmfc=%04d%02d%02d%02d%02d&hf=%d&disp=B",\
        host, var.mod_type, mod_varn_arr[n], level, X1, Y1, X2, Y2, YY, MM, DD, HH, MI, (var.seq-var.seq_fc)/60);
    }
    else if (var.seq_fc >= seq_bk2) {
      sprintf(url, "%s/cgi-bin/url/nph-um_grib_xy_txt1?group=UMGL&nwp=N768&data=%c&varn=%s&level=%d&map=S&sub=%d,%d,%d,%d&sm=0&tmfc=%04d%02d%02d%02d%02d&hf=%d&disp=B",\
        host, var.mod_type, mod_varn_arr[n], level, X1, Y1, X2, Y2, YY, MM, DD, HH, MI, (var.seq-var.seq_fc)/60);
    }
    else if (var.seq_fc >= seq_bk3) {
      sprintf(url, "%s/cgi-bin/url/nph-um_grib_xy_txt1?group=UMGL&nwp=N512&data=%c&varn=%s&level=%d&map=S&sub=%d,%d,%d,%d&sm=0&tmfc=%04d%02d%02d%02d%02d&hf=%d&disp=B",\
        host, var.mod_type, mod_varn_arr[n], level, X1, Y1, X2, Y2, YY, MM, DD, HH, MI, (var.seq-var.seq_fc)/60);
    }
    else {
      sprintf(url, "%s/cgi-bin/url/nph-um_grib_xy_txt1?group=UMGL&nwp=N320&data=%c&varn=%s&level=%d&map=S&sub=%d,%d,%d,%d&sm=0&tmfc=%04d%02d%02d%02d%02d&hf=%d&disp=B",\
        host, var.mod_type, mod_varn_arr[n], level, X1, Y1, X2, Y2, YY, MM, DD, HH, MI, (var.seq-var.seq_fc)/60);
    }

    printf("#%s\n", url);

    if((fp = url_fopen(url, "r"))) {
      url_fread(&tmp, 4, 1, fp);
      url_fread(&tmp, 4, 1, fp);

      for (j = 0; j < Y2-Y1+1; j++) {
        for (i = 0; i < X2-X1+1; i++) {
          url_fread(&g[j][i][n], 4, 1, fp);
        }
      }
      url_fclose(fp);
    }
  }

  if (var.calc_max == 1) {
    max = -999.;

    for (j = 0; j < Y2-Y1+1; j++) {
      for (i = 0; i < X2-X1+1; i++) {
        if (max < g[j][i][0]) max = g[j][i][0];
      }
    }

    printf("%d\n", (int)(max + 0.5));
    return 0;
  }

  if (var.seq_fc >= seq_bk) {
    sprintf(filename, "/fct/REF/STN/GDPS_stn_db.dat");
    if((fp2 = fopen(filename, "r"))) {
      num_stn = 0;
      while (!feof(fp2)) {
        fgets(buf, sizeof(buf), fp2);
        sscanf(buf, "%s %d %d %d %s", idx, &id, &x, &y, stn_ko);
        if (!strcmp(idx, "NUM")) continue;
        else {
          stn_id[num_stn] = id;
          stn_x[num_stn] = x - X1;  
          stn_y[num_stn] = y - Y1;
          num_stn++;
        }
      }
      fclose(fp2);
    }
  }

  for (n=0; n<var.num_stn; n++) {
    ok = 0;
    if (var.seq_fc >= seq_bk) {
      if (!strcmp(var.gts,"SFC")) {
        if (!strcmp(stn_data[n].tp, "A") || (!strcmp(stn_data[n].tp, "S") && atoi(stn_data[n].id) >= 47090 && atoi(stn_data[n].id) <= 47295)) {
          for (i=0; i<num_stn; i++) {
            if (stn_id[i] == atoi(stn_data[n].id) || stn_id[i] == atoi(stn_data[n].id) + 47000) {
              i1 = stn_x[i];
              j1 = stn_y[i];
              stn_data[n].disp = 1;
              ok = 1;
              break;
            }          
          }
        }
        else {
          stn_data[n].disp = 1;
        }
      }
      else {
        if (atoi(stn_data[n].id) >= 47090 && atoi(stn_data[n].id) <= 47295) {
          for (i=0; i<num_stn; i++) {
            if (stn_id[i] == atoi(stn_data[n].id) || stn_id[i] == atoi(stn_data[n].id) + 47000) {
              i1 = stn_x[i];
              j1 = stn_y[i];
              stn_data[n].disp = 1;
              ok = 1;
              break;
            }          
          }
        }
        else {
          stn_data[n].disp = 1;
        }
      }
    }
    else {
      stn_data[n].disp = 1;
    }

    if (ok == 0) {
      if (stn_data[n].lon < 0) stn_data[n].lon += 360;

      i1 = (int)((stn_data[n].lon/(360.0/nx)+0.5))+1 - X1;
      j1 = (int)(((stn_data[n].lat+90.0)/(180.0/(ny-1))+0.5))+1 - Y1;
    }

    if (i1 >= (X2-X1+1) || j1 >= (Y2-Y1+1) || i1 < 0 || j1 < 0) continue;

    if (!strcmp(var.varn,"TA")) {
      stn_data[n].nwp = g[j1][i1][0] - CELKEL;
    }
    else if (!strcmp(var.varn,"WS")) {
      stn_data[n].nwp = sqrt(pow(g[j1][i1][0],2)+pow(g[j1][i1][1],2));

      uu1 = g[j1][i1][0];
      vv1 = g[j1][i1][1];
      ws = sqrt(uu1*uu1 + vv1*vv1);
      wd = 180./3.141592 * atan(uu1/vv1);
      if ( vv1  > 0.0)                  wd = wd + 180;
      if ((vv1  < 0.0) && (uu1 >= 0.0)) wd = wd + 360;
      if ((vv1 == 0.0) && (uu1 >  0.0)) wd = 270.;
      if ((vv1 == 0.0) && (uu1 <  0.0)) wd = 90.;

      stn_data[n].wd1 = wd;
    }
    //else if (!strcmp(var.varn,"WD")) {
    //  stn_data[n].nwp = get_wind_direction(g[j1][i1][0], g[j1][i1][1]);
    //}
    else if (!strcmp(var.varn,"RH")) {
/*
      if (var.mod_type == 'U') {
        stn_data[n].nwp = get_relative_humidity(g[j1][i1][0]-CELKEL, g[j1][i1][1]-CELKEL);
      }
      else {
        stn_data[n].nwp = g[j1][i1][0];
      }
      */
      stn_data[n].nwp = g[j1][i1][0];
    }
    else if (!strcmp(var.varn,"TD")) {
      if (var.mod_type == 'P') {
        stn_data[n].nwp = get_dew_temp(g[j1][i1][0], g[j1][i1][1], (float)(level));
      }
      else {
        stn_data[n].nwp = g[j1][i1][0] - CELKEL;
      }
    }
    else if (!strcmp(var.varn,"PS")) {
      stn_data[n].nwp = g[j1][i1][0]*0.01;
    }
    else if (!strcmp(var.varn,"GH")) {
      stn_data[n].nwp = g[j1][i1][0];
    }
    else if (!strcmp(var.varn,"MR")) {
      if (var.mod_type == 'P') {
        g3 = get_dew_temp(g[j1][i1][0], g[j1][i1][1], (float)(level));
      }
      else {
        g3 = g[j1][i1][1] - CELKEL;
      }
  
      if (var.mod_type == 'P') {
        g4 = (float)(level);
      }
      else {
        g4 = g[j1][i1][2]*0.01;
      }

      stn_data[n].nwp = get_mixing_ratio2(g[j1][i1][0]-CELKEL, g3, g4);
    }
    else if (!strcmp(var.varn,"WN")) {
      if (var.mod_type == 'P') {
        g3 = get_dew_temp(g[j1][i1][0], g[j1][i1][1], (float)(level));
      }
      else {
        g3 = g[j1][i1][1] - CELKEL;
      }

      stn_data[n].nwp = (g[j1][i1][0] - CELKEL) - g3;
    }
    else if (!strcmp(var.varn,"TP")) {
      if (var.mod_type == 'P') {
        g3 = (float)(level);
      }
      else {
        g3 = g[j1][i1][1]*0.01;
      }

      stn_data[n].nwp = get_potential_temp(g[j1][i1][0]-CELKEL, g3);
    }
    else if (!strcmp(var.varn,"WBT")) {
      stn_data[n].nwp = get_wetbulb_temp(g[j1][i1][0]-CELKEL, g[j1][i1][1]);
    }
    else if (!strcmp(var.varn,"EPT")) {
      stn_data[n].nwp = get_ept(g[j1][i1][0]-CELKEL, get_dew_temp(g[j1][i1][0], g[j1][i1][1], (float)(level)), (float)(level));
    }

    //printf("%s %d %d %f\n", stn_data[n].id, j1, i1, g[j1][i1][0]);
  }

  return 0;
} // int um_data_get의 끝.


/*******************************************************************************
 *  ECMWF 자료 가져오기
 *******************************************************************************/
int ecmwf_data_get()
{
  URL_FILE   *fp;
  FILE       *fp2;
  int        YY, MM, DD, HH, MI, SS;
  char       buf[1000], url[500], mod_varn_arr[MAX_VARN][32], tmp[300];
  char       filename[512], idx[8], stn_ko[32];
  int        stn_id[NUM_ASOS], stn_x[NUM_ASOS], stn_y[NUM_ASOS], x, y, id, num_stn;
  int        i, j, k, n, i1, j1, n1, k1, nx, ny, ok;
  int        X1, X2, Y1, Y2, lon1, lon2, lat1, lat2;
  int        level;
  float      ***g, g2[10], g3, g4, max;
  float      uu1, vv1, ws, wd;
  //------------------------------------------------------------------

  lon1 = (int)(var.lon1*1000 + 0.5);
  lon2 = (int)(var.lon2*1000 + 0.5);
  lat1 = (int)(var.lat1*1000 + 0.5);
  lat2 = (int)(var.lat2*1000 + 0.5);
  if (lon1 < 0) lon1 += 360000;
  if (lon2 < 0) lon2 += 360000;
  X1 = (int)((int)((lon1 - 0)/250 + 0.5)) + 1;
  X2 = (int)((int)((lon2 - 0)/250 + 0.5)) + 1;
  Y1 = ECMWF_NY - ((int)((int)((lat1 - 90000)/(-250) + 0.5)) + 1) + 1;
  Y2 = ECMWF_NY - ((int)((int)((lat2 - 90000)/(-250) + 0.5)) + 1) + 1;

  if (X1 < 1) X1 = 1;
  if (X2 < 1) X2 = 1;
  if (Y1 < 1) Y1 = 1;
  if (Y2 < 1) Y2 = 1;
  if (X1 >= ECMWF_NX) X1 = ECMWF_NX-1;
  if (X2 >= ECMWF_NX) X2 = ECMWF_NX-1;
  if (Y1 >= ECMWF_NY) Y1 = ECMWF_NY-1;
  if (Y2 >= ECMWF_NY) Y2 = ECMWF_NY-1;

  strcpy(buf, var.mod_varn);

  k = 0;
  for (i = 0; buf[0] != '\0'; i++) {
    getword(mod_varn_arr[k], buf, ',');
    //printf("%s\n", mod_varn_arr[k]);
    k++;
  }

  // 배열을 설정한다.
  g=(float ***) malloc((unsigned) (Y2-Y1+1)*sizeof(float **));
  for(j=0; j<Y2-Y1+1; j++) {
    g[j]=(float **) malloc((unsigned) (X2-X1+1)*sizeof(float *));
  }
  for(j=0; j<Y2-Y1+1; j++) {
    for(i=0; i<X2-X1+1; i++) {
      g[j][i]=(float *) malloc((unsigned) (k)*sizeof(float));
    }
  }

  if (!strcmp(var.gts,"SFC")) level = 0;
  else level = atoi(var.gts);
  seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n');

  for (n = 0; n < k; n++) {
    sprintf(url, "%s/cgi-bin/url/nph-nwp_grb1_xy_txt1?group=ECMW&nwp=egnp&data=%c&varn=%s&level=%d&map=S&sub=%d,%d,%d,%d&sm=0&tmfc=%04d%02d%02d%02d%02d&hf=%d&disp=B",\
      host, var.mod_type, mod_varn_arr[n], level, X1, Y1, X2, Y2, YY, MM, DD, HH, MI, (var.seq-var.seq_fc)/60);

    printf("#%s\n", url);

    if((fp = url_fopen(url, "r"))) {
      url_fread(&nx, 4, 1, fp);
      url_fread(&ny, 4, 1, fp);

      for (j = 0; j < Y2-Y1+1; j++) {
        for (i = 0; i < X2-X1+1; i++) {
          url_fread(&g[j][i][n], 4, 1, fp);
        }
      }
      url_fclose(fp);
    }
  }

  if (var.calc_max == 1) {
    max = -999.;

    for (j = 0; j < Y2-Y1+1; j++) {
      for (i = 0; i < X2-X1+1; i++) {
        if (max < g[j][i][0]) max = g[j][i][0];
      }
    }

    printf("%d\n", (int)(max + 0.5));
    return 0;
  }

  sprintf(filename, "/fct/REF/STN/ECMW_stn_db.dat");
  if((fp2 = fopen(filename, "r"))) {
    num_stn = 0;
    while (!feof(fp2)) {
      fgets(buf, sizeof(buf), fp2);
      sscanf(buf, "%s %d %d %d %s", idx, &id, &x, &y, stn_ko);
      if (!strcmp(idx, "NUM")) continue;
      else {
        stn_id[num_stn] = id;
        stn_x[num_stn] = x - X1;  
        stn_y[num_stn] = y - Y1;
        num_stn++;
      }
    }
    fclose(fp2);
  }

  for (n=0; n<var.num_stn; n++) {
    if (stn_data[n].lat < 0) continue;

    ok = 0;
    if (!strcmp(var.gts,"SFC")) {
      if (!strcmp(stn_data[n].tp, "A") || (!strcmp(stn_data[n].tp, "S") && atoi(stn_data[n].id) >= 47090 && atoi(stn_data[n].id) <= 47295)) {
        for (i=0; i<num_stn; i++) {
          if (stn_id[i] == atoi(stn_data[n].id) || stn_id[i] == atoi(stn_data[n].id) + 47000) {
            i1 = stn_x[i];
            j1 = stn_y[i];
            stn_data[n].disp = 1;
            ok = 1;
            break;
          }          
        }
      }
      else {
        stn_data[n].disp = 1;
      }
    }
    else {
      if (atoi(stn_data[n].id) >= 47090 && atoi(stn_data[n].id) <= 47295) {
        for (i=0; i<num_stn; i++) {
          if (stn_id[i] == atoi(stn_data[n].id) || stn_id[i] == atoi(stn_data[n].id) + 47000) {
            i1 = stn_x[i];
            j1 = stn_y[i];
            stn_data[n].disp = 1;
            ok = 1;
            break;
          }          
        }
      }
      else {
        stn_data[n].disp = 1;
      }
    }

    if (ok == 0) {
      if (stn_data[n].lon < 0) stn_data[n].lon += 360;

      lon1 = (int)(stn_data[n].lon*1000 + 0.5);
      lat1 = (int)(stn_data[n].lat*1000 + 0.5);

      i1 = (int)((int)((lon1 - 0)/250 + 0.5)) + 1 - X1;
      j1 = ECMWF_NY - ((int)((int)((lat1 - 90000)/(-250) + 0.5)) + 1) + 1 - Y1;
    }

    if (i1 >= (X2-X1+1) || j1 >= (Y2-Y1+1) || i1 < 0 || j1 < 0) continue;

    if (!strcmp(var.varn,"TA")) {
      stn_data[n].nwp = g[j1][i1][0] - CELKEL;
    }
    else if (!strcmp(var.varn,"WS")) {
      stn_data[n].nwp = sqrt(pow(g[j1][i1][0],2)+pow(g[j1][i1][1],2));

      uu1 = g[j1][i1][0];
      vv1 = g[j1][i1][1];
      ws = sqrt(uu1*uu1 + vv1*vv1);
      wd = 180./3.141592 * atan(uu1/vv1);
      if ( vv1  > 0.0)                  wd = wd + 180;
      if ((vv1  < 0.0) && (uu1 >= 0.0)) wd = wd + 360;
      if ((vv1 == 0.0) && (uu1 >  0.0)) wd = 270.;
      if ((vv1 == 0.0) && (uu1 <  0.0)) wd = 90.;

      stn_data[n].wd1 = wd;
    }
    //else if (!strcmp(var.varn,"WD")) {
    //  stn_data[n].nwp = get_wind_direction(g[j1][i1][0], g[j1][i1][1]);
    //}
    else if (!strcmp(var.varn,"RH")) {
      if (var.mod_type == 'U') {
        stn_data[n].nwp = get_relative_humidity(g[j1][i1][0]-CELKEL, g[j1][i1][1]-CELKEL);
      }
      else {
        stn_data[n].nwp = g[j1][i1][0];
      }
    }
    else if (!strcmp(var.varn,"TD")) {
      if (var.mod_type == 'P') {
        stn_data[n].nwp = get_dew_temp(g[j1][i1][0], g[j1][i1][1], (float)(level));
      }
      else {
        stn_data[n].nwp = g[j1][i1][0] - CELKEL;
      }
    }
    else if (!strcmp(var.varn,"PS")) {
      stn_data[n].nwp = g[j1][i1][0]*0.01;
    }
    else if (!strcmp(var.varn,"GH")) {
      stn_data[n].nwp = g[j1][i1][0];
    }
    else if (!strcmp(var.varn,"MR")) {
      if (var.mod_type == 'P') {
        g3 = get_dew_temp(g[j1][i1][0], g[j1][i1][1], (float)(level));
      }
      else {
        g3 = g[j1][i1][1] - CELKEL;
      }
  
      if (var.mod_type == 'P') {
        g4 = (float)(level);
      }
      else {
        g4 = g[j1][i1][2]*0.01;
      }

      stn_data[n].nwp = get_mixing_ratio2(g[j1][i1][0]-CELKEL, g3, g4);
    }
    else if (!strcmp(var.varn,"WN")) {
      if (var.mod_type == 'P') {
        g3 = get_dew_temp(g[j1][i1][0], g[j1][i1][1], (float)(level));
      }
      else {
        g3 = g[j1][i1][1] - CELKEL;
      }

      stn_data[n].nwp = (g[j1][i1][0] - CELKEL) - g3;
    }
    else if (!strcmp(var.varn,"TP")) {
      if (var.mod_type == 'P') {
        g3 = (float)(level);
      }
      else {
        g3 = g[j1][i1][1]*0.01;
      }

      stn_data[n].nwp = get_potential_temp(g[j1][i1][0]-CELKEL, g3);
    }
    else if (!strcmp(var.varn,"WBT")) {
      g3 = get_relative_humidity(g[j1][i1][0]-CELKEL, g[j1][i1][1]-CELKEL);
      stn_data[n].nwp = get_wetbulb_temp(g[j1][i1][0]-CELKEL, g3);
    }
    else if (!strcmp(var.varn,"EPT")) {
      stn_data[n].nwp = get_ept(g[j1][i1][0]-CELKEL, get_dew_temp(g[j1][i1][0], g[j1][i1][1], (float)(level)), (float)(level));
    }

    //printf("%s %d %d %f\n", stn_data[n].id, j1, i1, g[j1][i1][0]);
  }

  return 0;
} // int ecmwf_data_get의 끝.


/*******************************************************************************
 *  KIM 전구모델 자료 가져오기
 *******************************************************************************/
int kimg_data_get()
{
  URL_FILE   *fp;
  FILE       *fp2;
  int        YY, MM, DD, HH, MI, SS;
  char       buf[1000], url[500], mod_varn_arr[MAX_VARN][32], tmp[300];
  char       filename[512], idx[8], stn_ko[32];
  int        stn_id[NUM_ASOS], stn_x[NUM_ASOS], stn_y[NUM_ASOS], x, y, id, num_stn;
  int        i, j, k, n, i1, j1, n1, k1, nx, ny, ok;
  int        X1, X2, Y1, Y2;
  int        level;
  float      ***g, g2[10], g3, g4, max;
  float      uu1, vv1, ws, wd;
  //------------------------------------------------------------------

  X1 = (int)((var.lon1/(360.0/KIMG_NX)+0.5))+1;
  X2 = (int)((var.lon2/(360.0/KIMG_NX)+0.5))+1;
  Y1 = (int)(((var.lat1+90.0)/(180.0/(KIMG_NY-1))+0.5))+1;
  Y2 = (int)(((var.lat2+90.0)/(180.0/(KIMG_NY-1))+0.5))+1;

  if (X1 < 1) X1 = 1;
  if (X2 < 1) X2 = 1;
  if (Y1 < 1) Y1 = 1;
  if (Y2 < 1) Y2 = 1;
  if (X1 >= KIMG_NX) X1 = KIMG_NX-1;
  if (X2 >= KIMG_NX) X2 = KIMG_NX-1;
  if (Y1 >= KIMG_NY) Y1 = KIMG_NY-1;
  if (Y2 >= KIMG_NY) Y2 = KIMG_NY-1;

  strcpy(buf, var.mod_varn);

  k = 0;
  for (i = 0; buf[0] != '\0'; i++) {
    getword(mod_varn_arr[k], buf, ',');
    //printf("%s\n", mod_varn_arr[k]);
    k++;
  }

  // 배열을 설정한다.
  g=(float ***) malloc((unsigned) (Y2-Y1+1)*sizeof(float **));
  for(j=0; j<Y2-Y1+1; j++) {
    g[j]=(float **) malloc((unsigned) (X2-X1+1)*sizeof(float *));
  }
  for(j=0; j<Y2-Y1+1; j++) {
    for(i=0; i<X2-X1+1; i++) {
      g[j][i]=(float *) malloc((unsigned) (k)*sizeof(float));
    }
  }

  if (!strcmp(var.gts,"SFC")) level = 0;
  else level = atoi(var.gts);
  seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n');

  for (n = 0; n < k; n++) {
    sprintf(url, "%s/cgi-bin/url/nph-kim_nc_xy_txt1?group=KIMG&nwp=NE36&data=%c&name=%s&level=%d&map=S&sub=%d,%d,%d,%d&sm=0&tmfc=%04d%02d%02d%02d%02d&hf=%d&disp=B",\
      host, var.mod_type, mod_varn_arr[n], level, X1, Y1, X2, Y2, YY, MM, DD, HH, MI, (var.seq-var.seq_fc)/60);

    printf("#%s\n", url);

    if((fp = url_fopen(url, "r"))) {
      url_fread(&nx, 4, 1, fp);
      url_fread(&ny, 4, 1, fp);

      for (j = 0; j < Y2-Y1+1; j++) {
        for (i = 0; i < X2-X1+1; i++) {
          url_fread(&g[j][i][n], 4, 1, fp);
        }
      }
      url_fclose(fp);
    }
  }

  if (var.calc_max == 1) {
    max = -999.;

    for (j = 0; j < Y2-Y1+1; j++) {
      for (i = 0; i < X2-X1+1; i++) {
        if (max < g[j][i][0]) max = g[j][i][0];
      }
    }

    printf("%d\n", (int)(max + 0.5));
    return 0;
  }

  sprintf(filename, "/fct/REF/STN/GKIM_stn_db.dat");
  if((fp2 = fopen(filename, "r"))) {
    num_stn = 0;
    while (!feof(fp2)) {
      fgets(buf, sizeof(buf), fp2);
      sscanf(buf, "%s %d %d %d %s", idx, &id, &x, &y, stn_ko);
      if (!strcmp(idx, "NUM")) continue;
      else {
        stn_id[num_stn] = id;
        stn_x[num_stn] = x - X1 + 1;  
        stn_y[num_stn] = y - Y1 + 1;
        num_stn++;
      }
    }
    fclose(fp2);
  }

  for (n=0; n<var.num_stn; n++) {
    ok = 0;
    if (!strcmp(var.gts,"SFC")) {
      if (!strcmp(stn_data[n].tp, "A") || (!strcmp(stn_data[n].tp, "S") && atoi(stn_data[n].id) >= 47090 && atoi(stn_data[n].id) <= 47295)) {
        for (i=0; i<num_stn; i++) {
          if (stn_id[i] == atoi(stn_data[n].id) || stn_id[i] == atoi(stn_data[n].id) + 47000) {
            i1 = stn_x[i];
            j1 = stn_y[i];
            stn_data[n].disp = 1;
            ok = 1;
            break;
          }          
        }
      }
      else {
        stn_data[n].disp = 1;
      }
    }
    else {
      if (atoi(stn_data[n].id) >= 47090 && atoi(stn_data[n].id) <= 47295) {
        for (i=0; i<num_stn; i++) {
          if (stn_id[i] == atoi(stn_data[n].id) || stn_id[i] == atoi(stn_data[n].id) + 47000) {
            i1 = stn_x[i];
            j1 = stn_y[i];
            stn_data[n].disp = 1;
            ok = 1;
            break;
          }          
        }
      }
      else {
        stn_data[n].disp = 1;
      }
    }

    if (ok == 0) {
      if (stn_data[n].lon < 0) stn_data[n].lon += 360;

      i1 = (int)((stn_data[n].lon/(360.0/KIMG_NX)+0.5))+1 - X1;
      j1 = (int)(((stn_data[n].lat+90.0)/(180.0/(KIMG_NY-1))+0.5))+1 - Y1;
    }

    if (i1 >= (X2-X1+1) || j1 >= (Y2-Y1+1) || i1 < 0 || j1 < 0) continue;

    if (!strcmp(var.varn,"TA") || !strcmp(var.varn,"TD") || !strcmp(var.varn,"WBT")) {
      stn_data[n].nwp = g[j1][i1][0] - CELKEL;
    }
    else if (!strcmp(var.varn,"MR")) {
      stn_data[n].nwp = g[j1][i1][0]*1000.;
    }
    else if (!strcmp(var.varn,"PS")) {
      stn_data[n].nwp = g[j1][i1][0]*0.01;
    }
    else if (!strcmp(var.varn,"EPT")) {
      stn_data[n].nwp = get_ept(g[j1][i1][0]-CELKEL, get_dew_temp(g[j1][i1][0], g[j1][i1][1], (float)(level)), (float)(level));
    }
    else if (!strcmp(var.varn,"WS")) {
      stn_data[n].nwp = sqrt(pow(g[j1][i1][0],2)+pow(g[j1][i1][1],2));

      uu1 = g[j1][i1][0];
      vv1 = g[j1][i1][1];
      ws = sqrt(uu1*uu1 + vv1*vv1);
      wd = 180./3.141592 * atan(uu1/vv1);
      if ( vv1  > 0.0)                  wd = wd + 180;
      if ((vv1  < 0.0) && (uu1 >= 0.0)) wd = wd + 360;
      if ((vv1 == 0.0) && (uu1 >  0.0)) wd = 270.;
      if ((vv1 == 0.0) && (uu1 <  0.0)) wd = 90.;

      stn_data[n].wd1 = wd;
    }
    else {
      stn_data[n].nwp = g[j1][i1][0];
    }

    //printf("%s %d %d %f\n", stn_data[n].id, j1, i1, g[j1][i1][0]);
  }

  return 0;
} // int kimg_data_get의 끝.


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
  float  gh, rn, wd, ws, ta, td, pa, ps, pr, vs, hm, rn_day, ta_min1, ta_max1, tw;
  float  lat, lon, ht, ht_pa, ht_ta, ht_wd, ht_rn;
  int    pt, wc, wp, ca, cd, ch, cl, cm, ct, rh;
  int    YY, MM, DD, HH, MI;
  int    seq, seq1;
  int    qc, code, rtn, now, i, j, k;
  int    level;

  // 1. 지점별 관측값 읽기
  var.num_stn = 0;
  //var.num_stn_obj = 0;
  seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');

  if (!strcmp(var.gts,"SFC")) level = 0;
  else level = atoi(var.gts);

  if (strcmp(var.gts,"SFC") == 0) {
    sprintf(url, "%s/url/gts_cht_syn2.php?tm=%04d%02d%02d%02d00&lon1=%f&lon2=%f&lat1=%f&lat2=%f&help=0",
            host, YY, MM, DD, HH, var.lon1, var.lon2, var.lat1, var.lat2);
    if ((fr = url_fopen(url, "r"))) {
      while (!url_feof(fr)) {
        url_fgets(buf, sizeof(buf), fr);
        if (buf[0] == '#') continue;

        qc = 0;
        sscanf(buf, "%s %s %s %f %f %f %d %f %f %f %f %d %d %d %d %d %d %d %d %d %f %f %d %f %f %f %f %f",
               stn_tp, stn_tm, stn_id, &lon, &lat, &ps, &pt, &pr, &ta, &td, &hm,
               &vs, &wc, &wp, &ca, &cd, &ch, &cl, &cm, &ct,
               &wd, &ws, &rh, &rn, &pa, &ta_min1, &ta_max1, &tw);

        strcpy(stn_data[var.num_stn].tp, stn_tp);
        strcpy(stn_data[var.num_stn].tm, stn_tm);
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
        strcpy(stn_data[var.num_stn].tp, stn_tp);
        stn_data[var.num_stn].disp = 0;

        if (!strcmp(var.varn, "TA"))       stn_data[var.num_stn].v = ta;       
        else if (!strcmp(var.varn, "TD"))  stn_data[var.num_stn].v = td;
        else if (!strcmp(var.varn, "WN")  && ta > -90 && td > -90)  stn_data[var.num_stn].v = ta-td;
        else if (!strcmp(var.varn, "RH"))  stn_data[var.num_stn].v = hm;
        else if (!strcmp(var.varn, "WS"))  stn_data[var.num_stn].v = ws;
        else if (!strcmp(var.varn, "PS"))  stn_data[var.num_stn].v = ps;
        else if (!strcmp(var.varn, "MR")  && ta > -90 && td > -90 && pa > 500)  stn_data[var.num_stn].v = get_mixing_ratio2(ta, td, pa);
        else if (!strcmp(var.varn, "TP")  && ta > -90 && pa > 500)              stn_data[var.num_stn].v = get_potential_temp(ta, pa);
        else if (!strcmp(var.varn, "WBT") && ta > -90 && hm >= 0)               stn_data[var.num_stn].v = get_wetbulb_temp(ta, hm);
        else                                                                    stn_data[var.num_stn].v = -999.;

        var.num_stn++;
      }
    }
    url_fclose(fr);
  }

  // 2. 고층자료 읽기
  else {
    sprintf(url, "%s/url/gts_cht_temp.sum.php?pa=%s&tm=%04d%02d%02d%02d00&lon1=%f&lon2=%f&lat1=%f&lat2=%f&help=0&wpf=%d&amdar=%d&map=%s",
            host, var.gts, YY, MM, DD, HH, var.lon1, var.lon2, var.lat1, var.lat2, var.wpf, var.amdar, var.map);

    if ((fr = url_fopen(url, "r"))) {
      while (!url_feof(fr)) {
        url_fgets(buf, sizeof(buf), fr);
        if (buf[0] == '#') continue;

        qc = 0;
        sscanf(buf, "%s %s %f %f %f %f %f %f %f %f %s %f %s",
               stn_tm, stn_id, &lon, &lat, &pa, &gh, &ta, &td, &wd, &ws, flag, &ht, bufr);

        //if (lon < var.lon1 || lon > var.lon2 || lat < var.lat1 || lat > var.lat2) {
        //  continue;
        //}

        strcpy(stn_data[var.num_stn].tm, stn_tm);
        strcpy(stn_data[var.num_stn].id, stn_id);
        stn_data[var.num_stn].lon = lon;
        stn_data[var.num_stn].lat = lat;
        stn_data[var.num_stn].gh = gh;
        stn_data[var.num_stn].ta = ta;
        stn_data[var.num_stn].td = td;
        stn_data[var.num_stn].wd = wd;
        stn_data[var.num_stn].ws = ws;
        stn_data[var.num_stn].disp = 1; // 고층은 항상 표출

        if (flag[strlen(flag)-1] == 'W') strcpy(stn_data[var.num_stn].tp, "W");
        else if (flag[strlen(flag)-1] == 'A') {
          strcpy(stn_data[var.num_stn].tp, "A");
          //strcpy(stn_data[var.num_stn].bufr, bufr);
        }
        else if (flag[0] == '0' || flag[0] == '1') {
          strcpy(stn_data[var.num_stn].tp, "K");
        }
        else strcpy(stn_data[var.num_stn].tp, "U");

        if (!strcmp(var.varn, "TA"))       stn_data[var.num_stn].v = ta;       
        else if (!strcmp(var.varn, "TD"))  stn_data[var.num_stn].v = td;
        else if (!strcmp(var.varn, "WS"))  stn_data[var.num_stn].v = ws;
        else if (!strcmp(var.varn, "GH"))  stn_data[var.num_stn].v = gh;
        else if (!strcmp(var.varn, "TP") && ta > -90)              stn_data[var.num_stn].v = get_potential_temp(ta, (float)(level));
        else if (!strcmp(var.varn, "WN") && ta > -90 && td > -90)  stn_data[var.num_stn].v = ta-td;
        else if (!strcmp(var.varn, "RH") && ta > -90 && td > -90)  stn_data[var.num_stn].v = get_relative_humidity(ta, td);
        else if (!strcmp(var.varn, "MR") && ta > -90 && td > -90)  stn_data[var.num_stn].v = get_mixing_ratio2(ta, td, (float)(level));
        else if (!strcmp(var.varn, "EPT") && ta > -90 && td > -90) stn_data[var.num_stn].v = get_ept(ta, td, (float)(level));
        else                                                       stn_data[var.num_stn].v = -999.;

        var.num_stn++;
      }
    }
    url_fclose(fr);
  }

  return 0;
}

/*=============================================================================*
 *  설정값 불러오기
 *=============================================================================*/
int vmap_ini_read()
{
  FILE  *fp;
  char  buf[1000], tmp[300], model[8], level[8], varn[8], vinfo[128], unit[128];
  int   i, j, k, ok = 0;

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
      getword(var.mod_varn, buf, ':');
      getword(tmp, buf, ':');
      getword(tmp, buf, ':');  strcpy(vinfo, tmp);
      getword(tmp, buf, ':');  strcpy(unit, tmp);
      getword(tmp, buf, ':');  var.diff_max = atof(tmp);
      getword(tmp, buf, ':');  var.diff_det = atof(tmp);
      ok = 1;
      break;
    }
  }

  if (ok == 0) return -1;
  printf("#%s %s %s %s %f %f %s(%s)\n", model, level, varn, var.mod_varn, var.diff_max, var.diff_det, vinfo, unit);

  if (!strcmp(var.gts,"SFC"))
  {
    var.mod_type = 'U';
  }
  else
  {
    var.mod_type = 'P';
  }

  return 0;
} 


/*=============================================================================*
 *  설정값 불러오기
 *=============================================================================*/
int nwp_bias_data()
{
  int   i;

  if (!strcmp(var.varn,"WS") && strcmp(var.gts,"SFC")) {
    printf("#   YYMMDDHHMI              STN     LON    LAT        OBS        NWP    WD(OBS)    WD(NWP)\n");
    printf("#          UTC               ID     deg    deg                              deg        deg\n");
    printf("#                                  0.01   0.01       0.01       0.01          1          1\n");

    for (i=0; i<var.num_stn; i++) {
      if (stn_data[i].v >= 10000000 || stn_data[i].v <= -1000000 || stn_data[i].nwp >= 10000000 || stn_data[i].nwp <= -1000000) continue;
      if (stn_data[i].disp == 0) continue;
      printf("%1s %s %16s %7.2f %6.2f %10.2f %10.2f %10.0f %10.0f\n", stn_data[i].tp, stn_data[i].tm, stn_data[i].id, stn_data[i].lon, stn_data[i].lat, stn_data[i].v, stn_data[i].nwp, stn_data[i].wd, stn_data[i].wd1);

      if (i!=var.num_stn-1 && strcmp(stn_data[i].tp,"K") && !strcmp(stn_data[i+1].tp,"K")) {
        printf("#   YYMMDDHHMI              STN     LON    LAT        OBS        NWP    WD(OBS)    WD(NWP)\n");
        printf("#          KST               ID     deg    deg                              deg        deg\n");
        printf("#                                  0.01   0.01       0.01       0.01          1          1\n");
      }
    }
  }
  else {
    printf("#   YYMMDDHHMI              STN     LON    LAT        OBS        NWP\n");
    printf("#          UTC               ID     deg    deg                      \n");
    printf("#                                  0.01   0.01       0.01       0.01\n");

    for (i=0; i<var.num_stn; i++) {
      if (stn_data[i].v >= 10000000 || stn_data[i].v <= -1000000 || stn_data[i].nwp >= 10000000 || stn_data[i].nwp <= -1000000) continue;
      if (stn_data[i].disp == 0) continue;
      printf("%1s %s %16s %7.2f %6.2f %10.2f %10.2f\n", stn_data[i].tp, stn_data[i].tm, stn_data[i].id, stn_data[i].lon, stn_data[i].lat, stn_data[i].v, stn_data[i].nwp);

      if (i!=var.num_stn-1 && strcmp(stn_data[i].tp,"K") && !strcmp(stn_data[i+1].tp,"K")) {
        printf("#   YYMMDDHHMI              STN     LON    LAT        OBS        NWP\n");
        printf("#          KST               ID     deg    deg                      \n");
        printf("#                                  0.01   0.01       0.01       0.01\n");
      }
    }
  }

  return 0;
} 