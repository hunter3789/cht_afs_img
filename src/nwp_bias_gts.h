#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iconv.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <math.h>
#include <zlib.h>
#include <zconf.h>
#include <gd.h>
#include <gdfontt.h>
#include <gdfonts.h>
#include <gdfontmb.h>
#include <gdfontl.h>
#include <gdfontg.h>

#include "cgiutil.h"
#include "nrutil.h"
#include "stn_inf.h"
#include "map_ini.h"
#include "url_io.h"

#define  MAX_STN  50000
#define  NUM_ASOS 1000
#define  DEGRAD   0.01745329251994329576923690768489    // PI/180
#define  RADDEG   57.295779513082320876798154814105     // 180/PI
#define  CELKEL   273.15

#define  MAP_INI_FILE   "/DATA/GIS/MAP/map.ini" // 지도영역 정보파일
#define  VMAP_INI_FILE  "/fct/www/ROOT/fgd/cht/vmap.ini" // 수치모델 진단 변수별 정보파일
#define  MAP_DIR        "/DATA/GIS/MAP/bln"    // 지도파일(이진) 디렉토리
#define  COLOR_SET_DIR  "/fct/REF/COLOR/"      // 색상표 파일 임시저장소

#define  MAX_VARN  4
#define  UM_N128_NX 2560
#define  UM_N128_NY 1920
#define  UM_N768_NX 1536
#define  UM_N768_NY 1152
#define  UM_N512_NX 1024
#define  UM_N512_NY 769
#define  UM_N320_NX 640
#define  UM_N320_NY 481
#define  ECMWF_NX   1440
#define  ECMWF_NY   361
#define  KIMG_NX    2880
#define  KIMG_NY    1440

// 사용할 한글TTF
#define  FONTTTF9  "/usr/share/fonts/korean/TrueType/gulim.ttf"

//------------------------------------------------------------------------------
// 사용자 입력 변수
struct INPUT_VAR {
  // 지점별 자료 추출용
  char  gts[16];      // SFC, 기타
  char  obs[16];      // 변수 구분 (예: ta)
  int   seq;          // 기준시각 SEQ(분)
  int   seq_fc;       // 수치모델 발표시각 SEQ(분)
  int   seq_ef;       // 예측시간
  float missing;      // 이 값 이하이면 오류
  float max;          // 최대값
  int   calc_max;     // 최대값 계산여부(1: 계산 O)
  char  model[16];      // 수치모델 종류
  char  mod_type;
  float diff_max, diff_det; //편차(bias) 최대값, 시작값
  char  mod_varn[300];
  char  varn[8];
  int   wpf;      // 윈드프로파일러 호출(0: X, 1: O)
  int   amdar;    // AMDAR 호출(0: X, 1: O)

  // 지도 설정
  char  map[16];      // 사용할 지도코드
  float grid;         // 격자크기(km)
  float lon1, lon2, lat1, lat2;  // 영역범위
  int   topo;         // 지형고도 반영 여부(1:고도 반영, 0:반영하지 않음)
  char  zoom_x[16];   // X방향 확대
  char  zoom_y[16];   // Y방향 확대

  // 화면표출용
  int   size;         // 이미지크기(픽셀) : NI에 해당
  char  color[8];     // 칼라색상표
  int   title_pixel;  // 제목영역크기(픽셀)
  int   title;        // 제목표시(1) 여부
  int   sms;          // 평활화 여부
  char  sat[8];       // ct, ir1, wvp
  float ZRa;          // Z-R관계식의 계수(기본:200)  Z=aR^b
  float ZRb;          // Z-R관계식의 지수(기본:1.6)
  char  layer[8];     // 레이어 설정
  char  sat_ok;       // 위성자료 존재여부(1:있음, 0:없음)
  int   seq_sat;      // 위성시각 SEQ(분)
  int   num_ccn;      // 합성에 사용되는 채널수
  char  ccn[4];       // 통합채널번호
  float gamma[4];     // 감마값
  int   itv;          // 낙뢰표출 범위 +-itv분
  int   pnts;         // 간벌X(0) 간벌O(1)
  float font_size;    // 폰트 크기
  int   border;       // 글자 테두리

  // 산출된 자료
  int   num_stn;      // 표출한 gts 지점수
  int   NX, NY;       // 격자수, 따라서 격자점수는 각각에 1을 더하면 됨
  int   SX, SY;       // 기준 격자점 위치
  int   NI, NJ;       // 자료표출이미지 영역(픽셀)
  int   GI, GJ;       // 전체 이미지 영역(픽셀)
  int   num_color;    // 색상수
  int   disp;
  int   help;

  // 파일정보
  char  fname[120];   // 파일명
};

// 격자자료의 영역 정보
struct GRID_MAP {
  int   nx, ny;   // 배열크기 [0:ny][0:nx]
  float sx, sy;   // 기준점 위치(격자거리)
  float grid;     // 격자간격(km)
};

// 변수
// 지점 자료
struct STN_VAL {
  char  id[16];   // 지점번호
  char  name[32]; // 지점명(한글)
  char  tp[8];    // 지점특성
  char  tm[16];   // 시간
  float x, y;     // 위치
  float lon, lat; // 위경도
  float ht;       // 해발고도(m)
  float d;        // 값
  float v;        // 값(바람벡터시 V풍속 사용)
  float nwp;      // 수치모델 값
  int   ca;       // 운량
  int   cl;       // 하층운형
  int   cm;       // 중층운형
  int   ct;       // 상층운형
  int   cd;       // 중하층운량
  int   wp;       // 현재일기
  int   wc;       // 과거일기
  int   pt;       // 기압변화경향
  int   rh;       // 강수누적시간(6hr)
  float gh;       // 지위고도
  float ta;       // 기온
  float ta_max;   // 최고기온
  float ta_min;   // 최저기온
  float td;       // 노점온도
  float tw;       // 
  float wd;       // 풍향
  float ws;       // 풍속
  float wd1;      // 풍향
  float ws1;      // 풍속
  float ps;       // 해면기압
  float pa;       // 현지기압
  float pr;       // 기압변화량
  float rn;       // 강수량(mm)
  float hm;       // 습도
  float s;        // 임시저장소
  int   disp;     // 표출(0: 미표출, 1: 표출)
};