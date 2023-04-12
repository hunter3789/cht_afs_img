#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "projects.h"
#include "emess.h"
#include "cgiutil.h"
#include "nrutil.h"
#include "map_ini.h"

#define  MAP_INI_FILE   "/DATA/GIS/MAP/map.ini" // �������� ��������
#define  DEGRAD   0.01745329251994329576923690768489    // PI/180
#define  RADDEG   57.295779513082320876798154814105     // 180/PI

static projPJ   fromProj, toProj;

// ����� �Է� ����
struct INPUT_VAR {
  double startx;
  double starty;
  double endx;
  double endy;

  // ���� ����
  char  map[16];      // ����� �����ڵ�
  float grid;         // ����ũ��(km)
  char  zoom_x[16];   // X���� Ȯ��
  char  zoom_y[16];   // Y���� Ȯ��
  int   NX, NY;       // ���ڼ�, ���� ���������� ������ 1�� ���ϸ� ��
  int   SX, SY;       // ���� ������ ��ġ
  int   mode;         // (0) ���浵->GIS, (1) GIS->���浵
} var;

int main () {

    double x, y, z;
    double lat, lon;

    printf("HTTP/1.0 200 OK\n");
    printf("Content-type: text/plain\n\n");
    printf("#START7777\n");

    user_input();
    printf("#                  X                    Y                  LON                  LAT                  REF\n");

    z = 0;

    if (var.mode == 0) { //���浵 -> GIS ��ǥ��
      if (strcmp(var.map, "none")) map_edge();

      fromProj = pj_init_plus("+proj=longlat +ellps=WGS84 +datum=WGS84");
      if (!strcmp(var.map, "NHEM")) toProj = pj_init_plus("+proj=stere +lat_0=90 +lat_ts=90 +lon_0=120 +x_0=0 +y_0=0 +ellps=WGS84 +units=m");
      else toProj = pj_init_plus("+proj=lcc +lat_1=30 +lat_2=60 +lat_0=0 +lon_0=126 +x_0=0 +y_0=0 +ellps=WGS84 +units=m");

      x = var.startx * DEG_TO_RAD;
      y = var.starty * DEG_TO_RAD;
      lon = var.startx;
      lat = var.starty;
      pj_transform( fromProj, toProj, 1, 0, &x, &y, &z );
      printf("%20.7f %20.7f %20.7f %20.7f %20s\n", x, y, lon, lat, "leftTop");

      x = var.endx * DEG_TO_RAD;
      y = var.endy * DEG_TO_RAD;
      lon = var.endx;
      lat = var.endy;
      pj_transform( fromProj, toProj, 1, 0, &x, &y, &z );
      printf("%20.7f %20.7f %20.7f %20.7f %20s\n", x, y, lon, lat, "rightBottom");
    }
    else if (var.mode == 1) { //GIS ��ǥ�� -> ���浵
      if (!strcmp(var.map, "NHEM")) fromProj = pj_init_plus("+proj=stere +lat_0=90 +lat_ts=90 +lon_0=120 +x_0=0 +y_0=0 +ellps=WGS84 +units=m");
      else fromProj = pj_init_plus("+proj=lcc +lat_1=30 +lat_2=60 +lat_0=0 +lon_0=126 +x_0=0 +y_0=0 +ellps=WGS84 +units=m");
      toProj = pj_init_plus("+proj=longlat +ellps=WGS84 +datum=WGS84");

      x = var.startx;
      y = var.starty;
      pj_transform( fromProj, toProj, 1, 0, &x, &y, &z );
      lon = x / DEG_TO_RAD;
      lat = y / DEG_TO_RAD;
      printf("%20.7f %20.7f %20.7f %20.7f %20s\n", var.startx, var.starty, lon, lat, "leftTop");

      x = var.endx;
      y = var.endy;
      pj_transform( fromProj, toProj, 1, 0, &x, &y, &z );
      lon = x / DEG_TO_RAD;
      lat = y / DEG_TO_RAD;
      printf("%20.7f %20.7f %20.7f %20.7f %20s\n", var.endx, var.endy, lon, lat, "rightBottom");    
    }

    printf("#7777END\n");
    return 0;
}

/*******************************************************************************
 *  �����ڷ� �� �̹��� ǥ��� ����� ��û �м� �κ�
 *******************************************************************************/
int user_input() {
  char *qs;
  char tmp[256], item[32], value[32];
  int  i;

  // 1. ���� �ʱⰪ : �ڷẰ ó�� ���α׷����� ���� ��Ȳ�� �°� ����
  var.startx = 0;
  var.starty = 0;
  var.endx   = 0;
  var.endy   = 0;
  strcpy(var.map, "none");
  strcpy(var.zoom_x, "0000000");
  strcpy(var.zoom_y, "0000000");
  var.grid   = 1.0;
  var.mode   = 0;

  // 2. GET ������� ���޵� ����� �Էº������� �ص�
  qs = getenv ("QUERY_STRING");
  if (qs == NULL) return -1;

  for (i = 0; qs[0] != '\0'; i++) {
    getword (value, qs, '&');
    getword (item, value, '=');
    if (strlen(value) == 0) continue;

    if ( !strcmp(item,"startx"))       var.startx = atof(value);
    else if ( !strcmp(item,"starty"))  var.starty = atof(value);
    else if ( !strcmp(item,"endx"))    var.endx = atof(value);
    else if ( !strcmp(item,"endy"))    var.endy = atof(value);
    else if ( !strcmp(item,"map"))     strcpy(var.map, value);
    else if ( !strcmp(item,"zoom_x"))  strcpy(var.zoom_x, value);
    else if ( !strcmp(item,"zoom_y"))  strcpy(var.zoom_y, value);
    else if ( !strcmp(item,"mode"))    var.mode = atoi(value);
  }

  return 0;
}

/*=============================================================================*
 *  ���� �𼭸� ���� Ȯ��
 *=============================================================================*/
int map_edge()
{
  int   i, j, k;
  struct lamc_parameter map;
  struct ster_parameter map2;
  float  zm = 1.0, xo = 0.0, yo = 0.0;
  int    zx, zy;
  float  lon, lat, x, y, lon1, lat1, x1, y1;

  if (!strcmp(var.map, "NHEM")) {
    map2.Re    = 6371.00877;
    map2.grid  = var.grid;

    var.NX = (int)((float)(map2.Re*2*PI/2)/map2.grid);  var.NY = (int)((float)(map2.Re*2*PI/2)/map2.grid);
    var.SX = (int)((float)(map2.Re*PI/2)/map2.grid);    var.SY = (int)((float)(map2.Re*PI/2)/map2.grid);
  }
  else map_ini_read();

  // 1. Ȯ���, �߽���ġ�� Ȯ����� ���
  for (i = 0; i < 7; i++, zm *= 2) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
    xo += (float)(var.NX)/16.0*(zx-1)/zm;
    yo += (float)(var.NY)/16.0*(zy-1)/zm;
  }

  // 2. ���� ������
  if (!strcmp(var.map, "NHEM")) {
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
  if (!strcmp(var.map, "NHEM")) {
    sterproj_ellp(&lon, &lat, &x, &y, 1, &map2);
    sterproj_ellp(&lon1, &lat1, &x1, &y1, 1, &map2);
  }
  else {
    lamcproj_ellp(&lon, &lat, &x, &y, 1, &map);
    lamcproj_ellp(&lon1, &lat1, &x1, &y1, 1, &map);
  }
  //printf("%f, %f, %f, %f\n", lon, lat, lon1, lat1);

  var.startx = lon;  var.starty = lat;
  var.endx = lon1;   var.endy = lat1;

  return 0;
}

/*=============================================================================*
 *  �ִ� �������� Ȯ��
 *=============================================================================*/
int map_ini_read()
{
  FILE  *fp;
  char  map_cd[8], buf[300], tmp[300];
  float max_sx, max_sy, max_dx, max_dy, max_nx = 0;
  int   i, j, k, n = 0;

  // 1. ������������ ����
  if ((fp = fopen(MAP_INI_FILE, "r")) == NULL) return -1;
  
  // 2. �ش�Ǵ� ���� ���� �б�
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