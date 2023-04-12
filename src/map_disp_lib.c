/*=============================================================================*
 *  EUC-KR문자열을 UTF-8로 변환
 *=============================================================================*/
int euckr2utf(char *str, char *out)
{
  iconv_t convp;
  size_t  ileft = strlen(str), oleft = strlen(str)*2;
  int     err;

  convp = iconv_open("UTF-8", "euc-kr");
  err = iconv(convp, &str, &ileft, &out, &oleft);
  iconv_close(convp);
  return err;
}

/*=============================================================================*
 *  지형고도 표출
 *=============================================================================*/
#define TOPO_DIR  "/DATA/GIS/TOPO/KMA"   // 지형고도 디렉토리
short **topo;

int topo_disp(
  gdImagePtr im,    // 이미지영역
  float data_grid,  // 자료의 격자크기 (var.NX의 단위,km)
  int topo_color,   // 사용할 색상표
  int topo_mode,    // 표출 방법  (1:에코영역을 피함)
  int color_area    // 배경색상번호
)
{
  FILE  *fp;
  char  fname[120];
  //short **topo;
  long  offset1, offset2, offset3;
  float dg = (float)var.NX/(float)(var.NI);
  float zm = 1.0, xo = 0.0, yo = 0.0, zx, zy, x1, y1, ht;
  float data_topo[40];
  int   color_topo[40];
  int   num_color_topo;
  int   nx, ny, sx, sy;
  int   ix, iy, color1;
  int   i, j, k;
  int   mapHM_NX = 9198,  mapHM_NY = 7998, mapHM_SX = 4599, mapHM_SY = 5434;
  int   mapH5_NX = 5760,  mapH5_NY = 5760, mapH5_SX = 3328, mapH5_SY = 3328;
  int   mapEA_NX = 9640,   mapEA_NY = 6760,  mapEA_SX = 5680,  mapEA_SY = 2960;
  int   mapTP_NX = 11200,  mapTP_NY = 6880,  mapTP_SX = 4000,  mapTP_SY = 4640;

  // 1. 사용할 지형고도 색상표
  if (topo_color == 0) {        // 해륙구분만
    if (strstr(var.layer,"R")) color_topo[0] = gdImageColorAllocate(im, 255, 255, 255);
    else color_topo[0] = gdImageColorAllocate(im, 229, 255, 255);
    color_topo[1] = gdImageColorAllocate(im, 255, 255, 229);
    data_topo[0] = 0.1;   data_topo[1] = 9000;
    num_color_topo = 2;
  }
  else if (topo_color == 1) {   // 간략
    color_topo[0] = gdImageColorAllocate(im,   0,  39, 116);  data_topo[0] = 0.1;
    color_topo[1] = gdImageColorAllocate(im, 244, 183,   0);  data_topo[1] = 100;
    color_topo[2] = gdImageColorAllocate(im, 191, 138,   0);  data_topo[2] = 500;
    color_topo[3] = gdImageColorAllocate(im, 138, 100,   0);  data_topo[3] = 5000;
    num_color_topo = 4;
  }
  else if (topo_color == 2) {   // 간략(흑백)
    color_topo[0] = gdImageColorAllocate(im, 206, 229, 241);  data_topo[0] = 0.1;
    color_topo[1] = gdImageColorAllocate(im, 240, 240, 225);  data_topo[1] = 100;
    color_topo[2] = gdImageColorAllocate(im, 170, 170, 170);  data_topo[2] = 500;
    color_topo[3] = gdImageColorAllocate(im, 120, 120, 120);  data_topo[3] = 5000;
    num_color_topo = 4;
  }
  else if (topo_color == 3) {   // 간략(흑백full버전)
    color_topo[0]  = gdImageColorAllocate(im, 206, 229, 241);  data_topo[0]  = 0.1;   // 바다색
    color_topo[1]  = gdImageColorAllocate(im, 240, 240, 225);  data_topo[1]  = 50;    // - 50m
    color_topo[2]  = gdImageColorAllocate(im, 220, 220, 220);  data_topo[2]  = 100;   // -100m
    color_topo[3]  = gdImageColorAllocate(im, 210, 210, 210);  data_topo[3]  = 150;   // -150m
    color_topo[4]  = gdImageColorAllocate(im, 200, 200, 200);  data_topo[4]  = 200;   // -200m
    color_topo[5]  = gdImageColorAllocate(im, 190, 190, 190);  data_topo[5]  = 300;   // -300m
    color_topo[6]  = gdImageColorAllocate(im, 180, 180, 180);  data_topo[6]  = 400;   // -400m
    color_topo[7]  = gdImageColorAllocate(im, 170, 170, 170);  data_topo[7]  = 500;   // -500m
    color_topo[8]  = gdImageColorAllocate(im, 160, 160, 160);  data_topo[8]  = 600;   // -600m
    color_topo[9]  = gdImageColorAllocate(im, 150, 150, 150);  data_topo[9]  = 700;   // -700m
    color_topo[10] = gdImageColorAllocate(im, 140, 140, 140);  data_topo[10] = 800;   // -800m
    color_topo[11] = gdImageColorAllocate(im, 130, 130, 130);  data_topo[11] = 900;   // -900m
    color_topo[12] = gdImageColorAllocate(im, 120, 120, 120);  data_topo[12] = 1000;  // -1000m
    color_topo[13] = gdImageColorAllocate(im, 110, 110, 110);  data_topo[13] = 1500;  // -1500m
    color_topo[14] = gdImageColorAllocate(im, 100, 100, 100);  data_topo[14] = 3000;  // -5000m
    color_topo[15] = gdImageColorAllocate(im,  90,  90,  90);  data_topo[15] = 9000;  // -9000m
    num_color_topo = 16;
  }
  //color_river = gdImageColorAllocate(im,  98, 169, 213);   // 강

  // 2. 지형고도자료 읽기
  nx = (int)(var.NX/data_grid);  ny = (int)(var.NY/data_grid);
  sx = (int)(var.SX/data_grid);  sy = (int)(var.SY/data_grid);

  if (!strcmp(var.map,"CM") || !strcmp(var.map,"CO") || !strcmp(var.map,"HM")) {
    sprintf(fname, "%s/gmted_meaEA_CHT_1km.bin", TOPO_DIR, var.map);
    offset1 = (mapHM_SX - sx)*2;
    offset2 = (mapHM_NX - nx)*2 - offset1;
    offset3 = (mapHM_SY - sy)*(mapHM_NX + 1)*2;
  }
  else if (!strcmp(var.map,"NHEM")) {
    sprintf(fname, "%s/gmted_meaNHEM_5km.bin", TOPO_DIR);
  }
  else if (!strcmp(var.map,"WORLD")) {
    sprintf(fname, "%s/gmted_meaWORLD_5km.bin", TOPO_DIR);
  }
  else {
    if (!strcmp(var.map,"TP")) {
      sprintf(fname, "%s/gmted_meaTP_1km.bin", TOPO_DIR);
      offset1 = (mapTP_SX - sx)*2;
      offset2 = (mapTP_NX - nx)*2 - offset1;
      offset3 = (mapTP_SY - sy)*(mapTP_NX + 1)*2;
    }
    else {
      sprintf(fname, "%s/gmted_meaEA_CHT_1km.bin", TOPO_DIR);
      offset1 = (mapEA_SX - sx)*2;
      offset2 = (mapEA_NX - nx)*2 - offset1;
      offset3 = (mapEA_SY - sy)*(mapEA_NX + 1)*2;
    }
  }
  if ((fp = fopen(fname, "rb")) == NULL) return -1;

  topo = smatrix(0, ny, 0, nx);
  fseek(fp, (long)64, SEEK_SET);    // 해더(64byts) skip
  if (!strcmp(var.map,"NHEM") || !strcmp(var.map,"WORLD")) {
    for (j = 0; j <= ny; j++) {
      fread(topo[j], 2, nx+1, fp);
    }
  }
  else {
    fseek(fp, offset3, SEEK_CUR);
    for (j = 0; j <= ny; j++) {
      fseek(fp, offset1, SEEK_CUR);
      fread(topo[j], 2, nx+1, fp);
      fseek(fp, offset2, SEEK_CUR);
    }
  }
  fclose(fp);

  // 3. 확대시, 중심위치와 확대비율 계산
  for (i = 0; i < 7; i++, zm *= 1.5) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
    //xo += (float)nx/8.0*(zx-1)/zm;
    //yo += (float)ny/8.0*(zy-1)/zm;
    xo += (float)nx/24.0*(zx-1)/zm;
    yo += (float)ny/24.0*(zy-1)/zm;
  }

  // 4. 이미지 픽셀별로 계산 (에코가 없는 부분만 표시)
  dg = (float)nx/(float)(var.NI*zm);
  for (j = 1; j < var.NJ; j++) {
    y1 = dg*j + yo;
    iy = (int)(y1 + 0.49);
    if (iy < 0 || iy > ny) continue;

    for (i = 1; i < var.NI; i++) {
      x1 = dg*i + xo;
      ix = (int)(x1 + 0.49);
      if (ix < 0 || ix > nx) continue;

      ht = (float)topo[iy][ix];
      if (topo_mode == 0) {
        for (k = 0; k < num_color_topo; k++) {
          if (ht < data_topo[k]) {
            gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_topo[k]);
            break;
          }
        }
      }
      else if (ht > 0) {
        color1 = gdImageGetPixel(im, i, var.GJ-j);
        if (color1 == color_area) {
          for (k = 0; k < num_color_topo; k++) {
            if (ht < data_topo[k]) {
              gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_topo[k]);
              break;
            }
          }
        }
      }
    }
  }

  // 4. 지형고도자료 해제 (다시 사용하는 경우도 있음)
  //free_smatrix(topo, 0, var.NY, 0, var.NX);
  return 0;
}

/*=============================================================================*
 *  지도 표출
 *=============================================================================*/
#define MAP_DIR "/DATA/GIS/MAP/bln"    // 지도파일이 있는 디저장소

int map_disp(
  gdImagePtr im,    // 이미지영역
  char  *map_type,  // 도시경계선 여부 (해안선은 map)
  float map_grid,   // var.NX의 격자간격(km)
  int   color_map,  // 지도 색상1
  int   color_map2  // 지도 색상2
  //int   depth       // 0(실선), 1(2배)
)
{
  FILE  *fp;
  char  fname[120], fs[8];
  float zm = 1.0, xo = 0.0, yo = 0.0, x1, y1, x2, y2, x3, y3, x4, y4;
  float rate = (float)(var.NI)/(float)(var.NX);
  float buf[2], ox, oy, km_px;
  int   ibuf[2];
  int   zx, zy, mode;
  int   i, j, k, n;
  int   mapHM_NX = 9198,  mapHM_NY = 7998, mapHM_SX = 4599, mapHM_SY = 5434;
  //int   mapH5_NX = 5760,  mapH5_NY = 5760, mapH5_SX = 3328, mapH5_SY = 3328;
  int   mapEA_NX = 9640,   mapEA_NY = 6760,  mapEA_SX = 5680,  mapEA_SY = 2960;
  int   mapTP_NX = 11200,  mapTP_NY = 6880,  mapTP_SX = 4000,  mapTP_SY = 4640;

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

  // 2. 지도 종류 선택 (상세, 단순)
  km_px = (float)(map_grid*var.NX)/(float)(var.NI*zm);
  if (km_px > 1.5)
    strcpy(fs,"s");
  else
    fs[0] = '\0';

  // 3. 지도 경계선 표출
  if (!strcmp(var.map,"CM") || !strcmp(var.map,"CO") || !strcmp(var.map,"HM")) {
    sprintf(fname, "%s/DFS_mapHM%s.bln", MAP_DIR, fs);
    ox = mapHM_SX/map_grid - var.SX;
    oy = mapHM_SY/map_grid - var.SY;
  }
  else if (!strcmp(var.map,"NHEM")) {
    if (!strcmp(map_type,"city") || !strcmp(map_type,"road") || !strcmp(map_type,"river")) {
      sprintf(fname, "%s/DFS_mapNHEM_%s.bln", MAP_DIR, map_type);
    }
    else if (strstr(var.layer,"A") && zm <= 2) {
      sprintf(fname, "%s/DFS_mapNHEMs3.bln", MAP_DIR, fs);
    }
    else sprintf(fname, "%s/DFS_mapNHEM%s.bln", MAP_DIR, fs);
    ox = oy = 0;
  }
  else if (!strcmp(var.map,"WORLD")) {
    if (!strcmp(map_type,"city") || !strcmp(map_type,"road") || !strcmp(map_type,"river")) {
      sprintf(fname, "%s/DFS_mapWORLD_%s.bln", MAP_DIR, map_type);
    }
    else if (zm > 2) {
      sprintf(fname, "%s/DFS_mapWORLD%s1.bln", MAP_DIR, fs);
    }
    else sprintf(fname, "%s/DFS_mapWORLD%s.bln", MAP_DIR, fs);
    ox = oy = 0;
  }
  else {
    if (!strcmp(map_type,"city") || !strcmp(map_type,"road") || !strcmp(map_type,"river")) {
      sprintf(fname, "%s/DFS_mapEA_CHT_%s.bln", MAP_DIR, map_type);
    }
    else {
      //sprintf(fname, "%s/DFS_map5%s.bln", MAP_DIR, fs);
      if (!strcmp(var.map,"TP")) {
        sprintf(fname, "%s/DFS_mapTP%s.bln", MAP_DIR, fs);
      }
      else {
        sprintf(fname, "%s/DFS_mapEA_CHT%s.bln", MAP_DIR, fs);
      }
    }
    //ox = mapH5_SX/map_grid - var.SX;
    //oy = mapH5_SY/map_grid - var.SY;
    if (!strcmp(var.map,"TP")) {
      ox = mapTP_SX/map_grid - var.SX;
      oy = mapTP_SY/map_grid - var.SY;
    }
    else {
      ox = mapEA_SX/map_grid - var.SX;
      oy = mapEA_SY/map_grid - var.SY;
    }
  }

  if ((fp = fopen(fname, "rb")) != NULL) {
    while (fread(ibuf, sizeof(int), 2, fp) == 2) {
      for (i = 0; i < ibuf[0]; i++) {
        if (fread(buf, sizeof(float), 2, fp) != 2) break;
        x2 = buf[0]/map_grid - ox;
        y2 = buf[1]/map_grid - oy;
        if (var.zoom_x[0] != '0') {
          x2 = zm*(x2 - xo);
          y2 = zm*(y2 - yo);
        }
        x2 *= rate;
        y2 *= rate;

        x3 = x1;
        y3 = y1;

        if (x1 < 0) {
          x3 = 0;
          y3 = (y2-y1)/(x2-x1)*(-x1) + y1;

          if (y3 < 0) {
            x3 = (x2-x1)/(y2-y1)*(-y1) + x1;
            y3 = 0;
          }
          else if (y3 > var.NJ) {
            x3 = (x2-x1)/(y2-y1)*(var.NJ-y1) + x1;
            y3 = var.NJ;
          }
        }
        else if (x1 > var.NI) {
          x3 = var.NI;
          y3 = (y2-y1)/(x2-x1)*(var.NI-x1) + y1;

          if (y3 < 0) {
            x3 = (x2-x1)/(y2-y1)*(-y1) + x1;
            y3 = 0;
          }
          else if (y3 > var.NJ) {
            x3 = (x2-x1)/(y2-y1)*(var.NJ-y1) + x1;
            y3 = var.NJ;
          }
        }
        else {
          if (y1 < 0) {
            x3 = (x2-x1)/(y2-y1)*(-y1) + x1;
            y3 = 0;
          }
          else if (y1 > var.NJ) {
            x3 = (x2-x1)/(y2-y1)*(var.NJ-y1) + x1;
            y3 = var.NJ;
          }
        }

        if (x3 > var.NI || x3 < 0) continue;

        x4 = x2;
        y4 = y2;

        if (x2 < 0) {
          x4 = 0;
          y4 = (y2-y1)/(x2-x1)*(-x1) + y1;

          if (y4 < 0) {
            x4 = (x2-x1)/(y2-y1)*(-y1) + x1;
            y4 = 0;
          }
          else if (y4 > var.NJ) {
            x4 = (x2-x1)/(y2-y1)*(var.NJ-y1) + x1;
            y4 = var.NJ;
          }
        }
        else if (x2 > var.NI) {
          x4 = var.NI;
          y4 = (y2-y1)/(x2-x1)*(var.NI-x1) + y1;

          if (y4 < 0) {
            x4 = (x2-x1)/(y2-y1)*(-y1) + x1;
            y4 = 0;
          }
          else if (y4 > var.NJ) {
            x4 = (x2-x1)/(y2-y1)*(var.NJ-y1) + x1;
            y4 = var.NJ;
          }
        } 
        else {
          if (y2 < 0) {
            x4 = (x2-x1)/(y2-y1)*(-y1) + x1;
            y4 = 0;
          }
          else if (y2 > var.NJ) {
            x4 = (x2-x1)/(y2-y1)*(var.NJ-y1) + x1;
            y4 = var.NJ;
          }
        }

        if (x4 > var.NI || x4 < 0) continue;

        if (i > 0) {
          if (!strcmp(var.map,"WORLD") && fabs(x2-x1) >= var.NI * 0.9) {
          }
          else gdImageLine(im, (int)x3+var.OI, var.NJ-(int)y3+var.OJ, (int)x4+var.OI, var.NJ-(int)y4+var.OJ, color_map);
          //if (depth) gdImageLine(im, (int)x1, var.GJ-(int)y1-1, (int)x2, var.GJ-(int)y2-1, color_map);
        }
        x1 = x2;
        y1 = y2;
      }
    }
    fclose(fp);
  }
  return 0;
}

/*=============================================================================*
 *  위경도선 표출
 *=============================================================================*/
int latlon_disp(
  gdImagePtr im,
  float map_grid,   // var.NX의 격자간격(km)
  int  color_map
)
{
  FILE   *fp;
  struct lamc_parameter map;
  struct ster_parameter map2;
  struct eqdc_parameter map3;
  char   fname[120], fs[8];
  float  zm = 1.0, xo = 0.0, yo = 0.0, x1, y1, x2, y2, x3, y3, x4, y4;
  float  rate = (float)(var.NI)/(float)(var.NX);
  float  lon, lat, x, y;
  int    lon1, lat1, ddeg, lon_min, lon_max, lat_min, lat_max;
  int    styleDashed[4];
  int    ix, iy, zx, zy, mode;
  int    i, j, k, n;
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
  if (!strcmp(var.map, "NHEM")) {
    map2.Re    = 6371.00877;
    map2.grid  = map_grid/(zm*rate);
    map2.slon  = 120.0;
    map2.slat  = 90.0;
    map2.olon  = 120.0;
    map2.olat  = 90.0;
    map2.xo = (float)(var.SX - xo)*(zm*rate);
    map2.yo = (float)(var.SY - yo)*(zm*rate);
    map2.first = 0;
    map2.polar = 0;

    lat_min = -10;
    lat_max = 90;
    lon_min = 0;
    lon_max = 359;
  }
  else if (!strcmp(var.map, "WORLD")) {
    map3.Re    = 6371.00877;
    map3.grid  = map_grid/(zm*rate);
    map3.slon  = 126.0;
    map3.slat  = 0.0;
    map3.olon  = 126.0;
    map3.olat  = 0.0;
    map3.xo = (float)(var.SX - xo)*(zm*rate);
    map3.yo = (float)(var.SY - yo)*(zm*rate);
    map3.first = 0;

    lat_min = -90;
    lat_max = 90;
    lon_min = -54;
    lon_max = 305;
  }
  else {
    map.Re    = 6371.00877;
    map.grid  = map_grid/(zm*rate);
    map.slat1 = 30.0;    map.slat2 = 60.0;
    map.olon  = 126.0;   map.olat  = 38.0;
    map.xo = (float)(var.SX - xo)*(zm*rate);
    map.yo = (float)(var.SY - yo)*(zm*rate);
    map.first = 0;

    x = 0.0;  y = 0.0;
    lamcproj_ellp(&lon, &lat, &x, &y, 1, &map);
    lat_min = (int)lat;

    x = var.NI;  y = 0.0;
    lamcproj_ellp(&lon, &lat, &x, &y, 1, &map);
    if (lat < lat_min) lat_min = (int)lat;

    x = var.NI*0.5;  y = var.NJ;
    lamcproj_ellp(&lon, &lat, &x, &y, 1, &map);
    lat_max = (int)lat + 1;

    x = 0.0;  y = var.NJ;
    lamcproj_ellp(&lon, &lat, &x, &y, 1, &map);
    lon_min = (int)lon;

    x = var.NI;  y = var.NJ;
    lamcproj_ellp(&lon, &lat, &x, &y, 1, &map);
    lon_max = (int)lon + 1;
  }

  // 3. 색상표 정리
  styleDashed[0] = color_map;
  styleDashed[1] = color_map;
  styleDashed[2] = gdTransparent;
  styleDashed[3] = gdTransparent;
  gdImageSetStyle(im, styleDashed, 4);

  // 4. 위경도 간격
  /*
  if ((float)(lon_max-lon_min)/zm > 5)
    ddeg = 10;
  else
    ddeg = 5;
  */
  if (zm > 3)
    ddeg = 5;
  else
    ddeg = 10;
  //printf("%f %d %d %f / %d %d %d %d\n", var.grid, var.NX, var.NY, rate, lon_min, lon_max, lat_min, lat_max);

  // 5. 경도선
  if (!strcmp(var.map, "WORLD")) ddeg *= 2;
  for (lon1 = lon_min; lon1 <= lon_max; lon1++) {
    if (lon1%ddeg != 0) continue;
    lon = lon1;  lat = lat_min;
    if (!strcmp(var.map, "NHEM"))       sterproj_ellp(&lon, &lat, &x1, &y1, 0, &map2);
    else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon, &lat, &x1, &y1, 0, &map3);
    else                                lamcproj_ellp(&lon, &lat, &x1, &y1, 0, &map);
    lon = lon1;  lat = lat_max;

    if (!strcmp(var.map, "NHEM"))       sterproj_ellp(&lon, &lat, &x2, &y2, 0, &map2);
    else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon, &lat, &x2, &y2, 0, &map3);
    else                                lamcproj_ellp(&lon, &lat, &x2, &y2, 0, &map);

    x3 = x1;
    y3 = y1;

    if (x1 < 0) {
      x3 = 0;
      y3 = (y2-y1)/(x2-x1)*(-x1) + y1;

      if (y3 < 0) {
        x3 = (x2-x1)/(y2-y1)*(-y1) + x1;
        y3 = 0;
      }
      else if (y3 > var.NJ) {
        x3 = (x2-x1)/(y2-y1)*(var.NJ-y1) + x1;
        y3 = var.NJ;
      }
    }
    else if (x1 > var.NI) {
      x3 = var.NI;
      y3 = (y2-y1)/(x2-x1)*(var.NI-x1) + y1;

      if (y3 < 0) {
        x3 = (x2-x1)/(y2-y1)*(-y1) + x1;
        y3 = 0;
      }
      else if (y3 > var.NJ) {
        x3 = (x2-x1)/(y2-y1)*(var.NJ-y1) + x1;
        y3 = var.NJ;
      }
    }
    else {
      if (y1 < 0) {
        x3 = (x2-x1)/(y2-y1)*(-y1) + x1;
        y3 = 0;
      }
      else if (y1 > var.NJ) {
        x3 = (x2-x1)/(y2-y1)*(var.NJ-y1) + x1;
        y3 = var.NJ;
      }
    }

    if (x3 > var.NI || x3 < 0) continue;

    x4 = x2;
    y4 = y2;

    if (x2 < 0) {
      x4 = 0;
      y4 = (y2-y1)/(x2-x1)*(-x1) + y1;

      if (y4 < 0) {
        x4 = (x2-x1)/(y2-y1)*(-y1) + x1;
        y4 = 0;
      }
      else if (y4 > var.NJ) {
        x4 = (x2-x1)/(y2-y1)*(var.NJ-y1) + x1;
        y4 = var.NJ;
      }
    }
    else if (x2 > var.NI) {
      x4 = var.NI;
      y4 = (y2-y1)/(x2-x1)*(var.NI-x1) + y1;

      if (y4 < 0) {
        x4 = (x2-x1)/(y2-y1)*(-y1) + x1;
        y4 = 0;
      }
      else if (y4 > var.NJ) {
        x4 = (x2-x1)/(y2-y1)*(var.NJ-y1) + x1;
        y4 = var.NJ;
      }
    }
    else {
      if (y2 < 0) {
        x4 = (x2-x1)/(y2-y1)*(-y1) + x1;
        y4 = 0;
      }
      else if (y2 > var.NJ) {
        x4 = (x2-x1)/(y2-y1)*(var.NJ-y1) + x1;
        y4 = var.NJ;
      }
    }

    if (x4 > var.NI || x4 < 0) continue;

    //gdImageLine(im, x1+var.OI, var.NJ-y1+var.OJ, x2+var.OI, var.NJ-y2+var.OJ, gdStyled);
    gdImageLine(im, x3+var.OI, var.NJ-y3+var.OJ, x4+var.OI, var.NJ-y4+var.OJ, gdStyled);
  }

  // 6. 위도선
  if (!strcmp(var.map, "WORLD")) ddeg /= 2;
  for (lat1 = lat_min; lat1 <= lat_max; lat1++) {
    if (lat1%ddeg != 0) continue;
    lon = lon_min;  lat = lat1;
    if (!strcmp(var.map, "NHEM"))       sterproj_ellp(&lon, &lat, &x1, &y1, 0, &map2);
    else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon, &lat, &x1, &y1, 0, &map3);
    else                                lamcproj_ellp(&lon, &lat, &x1, &y1, 0, &map);

    for (lon = lon_min; lon <= lon_max; lon++) {
      lat = lat1;
      if (!strcmp(var.map, "NHEM"))       sterproj_ellp(&lon, &lat, &x2, &y2, 0, &map2);
      else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon, &lat, &x2, &y2, 0, &map3);
      else                                lamcproj_ellp(&lon, &lat, &x2, &y2, 0, &map);
      if (x2 < 0 || x2 > var.NI || y2 < 0 || y2 > var.NJ) {
        x1 = x2;
        y1 = y2;
        continue;
      }
      gdImageLine(im, x1+var.OI, var.NJ-y1+var.OJ, x2+var.OI, var.NJ-y2+var.OJ, gdStyled);
      x1 = x2;
      y1 = y2;
    }
  }

  return 0;
}

/*=============================================================================*
 *  단면선 표시
 *=============================================================================*/
int cross_line_disp(
  gdImagePtr im,    // 이미지영역
  int color_line,   // 위경도선 색상
  int x1,   // 단면도용 시작과 끝점
  int y1,   // [x1,y1 : x2,y2]
  int x2,
  int y2
)
{
  float zm = 1.0, xo = 0.0, yo = 0.0, zx, zy;
  float grid_nx, grid_ny, dxy;
  int   map_code;
  int   xp1, yp1, xp2, yp2;
  int   i;

  // 1. 단면선이 있는지 판단
  if (x1 <= 0) return;

  // 2. 2배 확대시, 중심위치와 확대비율 계산
  for (i = 0; i < 7; i++, zm *= 1.5) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
    //xo += (var.NX/8.0*(zx-1)/zm);
    //yo += (var.NY/8.0*(zy-1)/zm);
    xo += (var.NX/24.0*(zx-1)/zm);
    yo += (var.NY/24.0*(zy-1)/zm);
  }

  // 4. 확대를 포함하여 단면선의 픽셀 위치 파악
  xp1 = (x1 - xo)*zm*(float)var.NI/(float)var.NX;
  xp2 = (x2 - xo)*zm*(float)var.NI/(float)var.NX;
  yp1 = var.GJ - (y1 - yo)*zm*(float)var.NJ/(float)var.NY;
  yp2 = var.GJ - (y2 - yo)*zm*(float)var.NJ/(float)var.NY;

  // 5. 단면선 표시
  gdImageLine(im, xp1, yp1, xp2, yp2, color_line);
  gdImageLine(im, xp1, yp1-1, xp2, yp2-1, color_line);
  gdImageFilledArc(im, xp1, yp1, 5, 5, 0, 360, color_line, gdArc);
  gdImageFilledArc(im, xp2, yp2, 5, 5, 0, 360, color_line, gdArc);
  return 0;
}
