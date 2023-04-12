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
#include <dirent.h>

#include "cgiutil.h"
#include "nrutil.h"
#include "url_io.h"
#include "eccodes.h"

#define  CELKEL   273.15
#define  DEGRAD   0.01745329251994329576923690768489    // PI/180
#define  RADDEG   57.295779513082320876798154814105     // 180/PI

#define  MAX_VAL_LEN 1024

#define  DATA_DIR      "/C4N2_DATA/GTS/BULL/BUFR/" // 데이터 저장 디렉토리(국내 AMDAR)
#define  DATA_DIR2     "/C4N2_DATA/GTS/BUFR/"      // 데이터 저장 디렉토리

//------------------------------------------------------------------------------
// 사용자 입력 변수
struct INPUT_VAR {
  // 지점별 자료 추출용
  int   seq;          // 기준시각 SEQ(분)
  int   flag;         // 0: 전체 AMDAR 자료 표출, 1: 특정 항공기 자료 표출
  int   mode;         // 0: 전체 표출, 1: 위경도 영역 제한
  int   disp;
  int   itv;          // 시간 간격
  int   pa;
  float missing;      // 이 값 이하이면 오류
  char  map[16];      // 사용할 지도코드
  float lon1, lat1, lon2, lat2;
  char  fname[256];   // BUFR 파일명
  char  aircraft[32]; // 항공기명
  int   message;      // message number
};