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

#define  DATA_DIR      "/C4N2_DATA/GTS/BULL/BUFR/" // ������ ���� ���丮(���� AMDAR)
#define  DATA_DIR2     "/C4N2_DATA/GTS/BUFR/"      // ������ ���� ���丮

//------------------------------------------------------------------------------
// ����� �Է� ����
struct INPUT_VAR {
  // ������ �ڷ� �����
  int   seq;          // ���ؽð� SEQ(��)
  int   flag;         // 0: ��ü AMDAR �ڷ� ǥ��, 1: Ư�� �װ��� �ڷ� ǥ��
  int   mode;         // 0: ��ü ǥ��, 1: ���浵 ���� ����
  int   disp;
  int   itv;          // �ð� ����
  int   pa;
  float missing;      // �� �� �����̸� ����
  char  map[16];      // ����� �����ڵ�
  float lon1, lat1, lon2, lat2;
  char  fname[256];   // BUFR ���ϸ�
  char  aircraft[32]; // �װ����
  int   message;      // message number
};