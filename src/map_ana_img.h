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
#include <hdf5.h>
#include <netcdf.h>
#include <curl/curl.h>

#include "cgiutil.h"
#include "nrutil.h"
#include "stn_inf.h"
#include "map_ini.h"
#include "url_io.h"
#include "projects.h"
#include "emess.h"
#include "cJSON.h"
#include "eccodes.h"

#define  MAX_STN  50000
#define  NUM_AWS  1000
#define  DEGRAD   0.01745329251994329576923690768489    // PI/180
#define  RADDEG   57.295779513082320876798154814105     // 180/PI
#define  CELKEL   273.15

#define  MAP_INI_FILE   "/DATA/GIS/MAP/map.ini" // 지도영역 정보파일
#define  VMAP_INI_FILE  "/fct/www/ROOT/fgd/cht/vmap.ini" // 수치모델 진단 변수별 정보파일
#define  CHT_INI_FILE   "/fct/www/ROOT/cht_new/cht_afs_fct.ini" // 통합기상분석 API 중첩 정보파일
#define  GTS_INI_FILE   "/fct/www/ROOT/gts/gts_afs_img.ini" // 통합기상분석 API 중첩 정보파일(GTS)
#define  IMG_DIR        "/fct/www/ROOT/img/cht"// 이미지 저장 디렉토리
#define  GTS_DIR        "/fct/www/ROOT/img/gts"// 이미지 저장 디렉토리(GTS)
#define  LEGEND_DIR     "/fct/www/ROOT/img/legend"// 범례 이미지 저장 디렉토리
#define  MAP_DIR        "/DATA/GIS/MAP/bln"    // 지도파일(이진) 디렉토리
#define  TOPO_DIR       "/DATA/GIS/TOPO/KMA"   // 지형고도 디렉토리
#define  COLOR_SET_DIR  "/fct/REF/COLOR/"      // 색상표 파일 임시저장소
#define  GK2A_L1B_DIR   "/GK2A/BIN/PRIMARY/L1B/COMPLETE"
#define  GK2A_L2_DIR    "/GK2A/BIN/L2/"
#define  GK2A_CONV_DIR  "/home/fct/REF/INI/GK2A"
#define  COMS_CONV_FILE "/fct/REF/INI/coms_conv.csv"
#define  COMS_BIN_DIR   "/DATA/SAT/COMS/BIN"   // 천리안위성 누년저장소
#define  HIMA_CONV_FILE "/fct/REF/INI/hima_conv.csv"
#define  HIMA_BIN_DIR   "/DATA/SAT/HIMA/BIN"
#define  RDR_CMP_DIR    "/DATA/RDR/CMP"         // 레이더합성파일 누년저장소
#define  RDR_OBS_DIR    "/DATA/RDR/OBS"         // 레이더 지상분석 누년보관소
#define  LGT_DIR        "/DATA/LGT/KMA"         // 낙뢰
#define  RDR_CMP_DIR    "/DATA/RDR/CMP"         // 레이더합성파일 누년저장소
#define  RDR_PCP_DIR    "/DATA/RDR/PCP"         // 누적강수량 누년저장소
#define  RDR_CMP_DIR_BK "/C4N2_DATA_BK/RDR/CMP" // 레이더합성파일 누년저장소
#define  RDR_CMP_R3D    "/DATA/RDR/R3D"         // 레이더 3차원합성파일 누년저장소
#define  GTS_BUFR_DIR   "/DATA/GTS/BUFR/"
#define  ASCAT_DIR      "/DATA/SAT/EUMETCAST/SCATWIND/"
#define  HY2_DIR        "/DATA/SAT/HY2/"

#define  AFS_HOST_URL   "http://afs.kma.go.kr/" // 선진예보시스템 host url

// COMS영역 (COMS_GRID 간격)
#define  COMS_NX   1769
#define  COMS_NY   1559
#define  COMS_SX   850
#define  COMS_SY   1060
#define  COMS_GRID 5

// 히마와리 격자영역 (2km 격자크기)
#define  HIMA_NX   4599
#define  HIMA_NY   3999
#define  HIMA_SX   2299
#define  HIMA_SY   2717
#define  HIMA_GRID 2

// HR-map영역(HR, km)
#define  HR_NX  1024
#define  HR_NY  1024
#define  HR_SX  440
#define  HR_SY  770

// HB-map(레이더240km합성) 영역(km)
#define  HB_NX  1152
#define  HB_NY  1440
#define  HB_SX  560
#define  HB_SY  840

// HC-map(레이더480km합성) 영역(km)
#define  HC_NX  1600
#define  HC_NY  1600
#define  HC_SX  800
#define  HC_SY  1000

// H3-map(레이더 한중일) 영역(km)
#define  H3_NX  3520
#define  H3_NY  3520
#define  H3_SX  2000
#define  H3_SY  2400

// H2-map(동아시아) 영역(km)
#define  H2_NX  4800
#define  H2_NY  4800
#define  H2_SX  2400
#define  H2_SY  2400

// H5-map(동아시아+) 영역(km)
#define  H5_NX  5760
#define  H5_NY  5760
#define  H5_SX  3328
#define  H5_SY  3328

// HW-map영역(HW, km)   WISSDOM
#define  HW_NX  960
#define  HW_NY  960
#define  HW_SX  438   // 440
#define  HW_SY  698   // 770

// 합성된 자료에 저장된 기본값
#define  BLANK1  -30000     // No Area
#define  BLANK2  -25000     // No Data
#define  BLANK3  -20000     // Minimum Data

// 이미지 영역중 일부 기본값
#define  LEG_pixel   35     // 범례표시영역
//#define  TITLE_pixel 20     // 제목표시영역
#define  TITLE_pixel 0     // 제목표시영역
#define  BORDER_pixel 0    // 테두리영역

// 사용할 한글TTF
#define  FONTTTF   "/fct/REF/FONT/NanumGothic.ttf"
#define  FONTTTF1  "/fct/REF/FONT/NanumGothicBold.ttf"
#define  FONTTTF2  "/fct/REF/FONT/NanumGothicExtraBold.ttf"
#define  FONTTTF9  "/usr/share/fonts/korean/TrueType/gulim.ttf"

#define  max_layer  10    // 일기도 최대 중첩 레이어 수
#define  INIT_SIZE 256

//------------------------------------------------------------------------------
// 사용자 입력 변수
struct INPUT_VAR {
  // 지점별 자료 추출용
  char  gts[16];      // SFC, 기타
  char  obs[16];      // 변수 구분 (예: ta)
  int   seq;          // 기준시각 SEQ(분)
  int   seq_fc;       // 시작시각 SEQ(분) - 수치모델 발표시각
  int   seq_ef;       // 예측시간
  float missing;      // 이 값 이하이면 오류
  float max;          // 최대값
  char  model[16];    // 수치모델 종류
  float diff_max, diff_det; // 편차(bias) 최대값, 시작값
  char  mod_varn[300];
  char  varn[8];
  char  cht_name[256];      // 일기도 종류
  char  cht_mode[32];       // 일기도 형태(fct: 예상장, fct2: 예상장(한반도 일기도X), ana: 분석장, ana2: 분석장(API 범례 X, 레이더 범례 사용), ana3: 분석장(한반도 일기도X), ana0: 관측, gts: gts 조회)
  int   gis;                // GIS 표출
  int   flag;               // 표출 옵션(0: 일기도 이미지, 1: 레이어 정보 표출)
  int   curl;               // 병렬 처리(0: X, 1: O)
  int   save;               // 이미지 저장(0: X, 1: O)
  char  host[16];           // API 호출 host
  int   wpf;                // 윈드프로파일러 호출(0: X, 1: O)
  int   amdar;              // AMDAR 호출(0: X, 1: O)
  int   legend;             // 범례 정보
  int   legend_only;        // 범례 이미지만 생성(0: X, 1: O)

  // 지도 설정
  char  map[16];      // 사용할 지도코드
  float grid;         // 격자크기(km)
  float lon1, lon2, lat1, lat2;  // 영역범위
  int   topo;         // 지형고도 반영 여부(1:고도 반영, 0:반영하지 않음)
  char  zoom_x[16];   // X방향 확대
  char  zoom_y[16];   // Y방향 확대
  double startx;
  double starty;
  double endx;
  double endy;
  double centerx, centery;

  // 화면표출용
  int   size;         // 이미지크기(픽셀) : NI에 해당
  char  color[8];     // 칼라색상표
  int   title_pixel;  // 제목영역크기(픽셀)
  int   title;        // 제목표시(1) 여부
  int   sms;          // 평활화 여부
  char  sat[16];      // ct, ir1, wvp
  float ZRa;          // Z-R관계식의 계수(기본:200)  Z=aR^b
  float ZRb;          // Z-R관계식의 지수(기본:1.6)
  char  layer[10];    // 레이어 설정
  char  sat_ok;       // 위성자료 존재여부(1:있음, 0:없음)
  int   seq_sat;      // 위성시각 SEQ(분)
  int   seq_rdr;      // 레이더시각 SEQ(분)
  int   num_ccn;      // 합성에 사용되는 채널수
  char  ccn[4];       // 통합채널번호
  float gamma[4];     // 감마값
  int   itv;          // 낙뢰표출 범위 +-itv분
  int   pnts;         // 간벌X(0) 간벌O(1)
  float font_size;    // 폰트 크기
  int   border;       // 글자 테두리
  int   color_adjust; // 색 자동조정
  int   color_wind;   // 바람깃 색 변경
  int   bias_disp;    // 기본(0) 숫자로 표출(1)

  // 산출된 자료
  int   num_stn;      // 표출한 gts 지점수
  int   NX, NY;       // 격자수, 따라서 격자점수는 각각에 1을 더하면 됨
  int   SX, SY;       // 기준 격자점 위치
  int   NI, NJ;       // 자료표출이미지 영역(픽셀)
  int   GI, GJ;       // 전체 이미지 영역(픽셀)
  int   OI, OJ;       // 자료표출이미지 위치(픽셀)
  int   num_color;    // 색상수
  int   disp;
  int   help;

  // 파일정보
  char  fname[120];   // 파일명
  int   mode;         // 기본(0) 레이어 이미지 생성(1)
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
  float ts;       // 지표온도
  float tw;       // 수온
  float wd;       // 풍향
  float wd_0;     // 풍향(보정 전)
  float ws;       // 풍속
  float wd1;      // 풍향
  float wd1_0;    // 풍향(보정 전)
  float ws1;      // 풍속
  float ps;       // 해면기압
  float pa;       // 현지기압
  float pr;       // 기압변화량
  float rn;       // 강수량(mm)
  float hm;       // 습도
  float s;        // 임시저장소
  int   pm10;     // PM10 농도
  float wh;       // 유의파고
  char  bufr[256];// BUFR 파일명
  int   msg;      // BUFR 메시지
};