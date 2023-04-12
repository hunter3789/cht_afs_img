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

#define  MAP_INI_FILE   "/DATA/GIS/MAP/map.ini" // �������� ��������
#define  VMAP_INI_FILE  "/fct/www/ROOT/fgd/cht/vmap.ini" // ��ġ�� ���� ������ ��������
#define  MAP_DIR        "/DATA/GIS/MAP/bln"    // ��������(����) ���丮
#define  COLOR_SET_DIR  "/fct/REF/COLOR/"      // ����ǥ ���� �ӽ������

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

// ����� �ѱ�TTF
#define  FONTTTF9  "/usr/share/fonts/korean/TrueType/gulim.ttf"

//------------------------------------------------------------------------------
// ����� �Է� ����
struct INPUT_VAR {
  // ������ �ڷ� �����
  char  gts[16];      // SFC, ��Ÿ
  char  obs[16];      // ���� ���� (��: ta)
  int   seq;          // ���ؽð� SEQ(��)
  int   seq_fc;       // ��ġ�� ��ǥ�ð� SEQ(��)
  int   seq_ef;       // �����ð�
  float missing;      // �� �� �����̸� ����
  float max;          // �ִ밪
  int   calc_max;     // �ִ밪 ��꿩��(1: ��� O)
  char  model[16];      // ��ġ�� ����
  char  mod_type;
  float diff_max, diff_det; //����(bias) �ִ밪, ���۰�
  char  mod_varn[300];
  char  varn[8];
  int   wpf;      // �����������Ϸ� ȣ��(0: X, 1: O)
  int   amdar;    // AMDAR ȣ��(0: X, 1: O)

  // ���� ����
  char  map[16];      // ����� �����ڵ�
  float grid;         // ����ũ��(km)
  float lon1, lon2, lat1, lat2;  // ��������
  int   topo;         // ������ �ݿ� ����(1:�� �ݿ�, 0:�ݿ����� ����)
  char  zoom_x[16];   // X���� Ȯ��
  char  zoom_y[16];   // Y���� Ȯ��

  // ȭ��ǥ���
  int   size;         // �̹���ũ��(�ȼ�) : NI�� �ش�
  char  color[8];     // Į�����ǥ
  int   title_pixel;  // ���񿵿�ũ��(�ȼ�)
  int   title;        // ����ǥ��(1) ����
  int   sms;          // ��Ȱȭ ����
  char  sat[8];       // ct, ir1, wvp
  float ZRa;          // Z-R������� ���(�⺻:200)  Z=aR^b
  float ZRb;          // Z-R������� ����(�⺻:1.6)
  char  layer[8];     // ���̾� ����
  char  sat_ok;       // �����ڷ� ���翩��(1:����, 0:����)
  int   seq_sat;      // �����ð� SEQ(��)
  int   num_ccn;      // �ռ��� ���Ǵ� ä�μ�
  char  ccn[4];       // ����ä�ι�ȣ
  float gamma[4];     // ������
  int   itv;          // ����ǥ�� ���� +-itv��
  int   pnts;         // ����X(0) ����O(1)
  float font_size;    // ��Ʈ ũ��
  int   border;       // ���� �׵θ�

  // ����� �ڷ�
  int   num_stn;      // ǥ���� gts ������
  int   NX, NY;       // ���ڼ�, ���� ���������� ������ 1�� ���ϸ� ��
  int   SX, SY;       // ���� ������ ��ġ
  int   NI, NJ;       // �ڷ�ǥ���̹��� ����(�ȼ�)
  int   GI, GJ;       // ��ü �̹��� ����(�ȼ�)
  int   num_color;    // �����
  int   disp;
  int   help;

  // ��������
  char  fname[120];   // ���ϸ�
};

// �����ڷ��� ���� ����
struct GRID_MAP {
  int   nx, ny;   // �迭ũ�� [0:ny][0:nx]
  float sx, sy;   // ������ ��ġ(���ڰŸ�)
  float grid;     // ���ڰ���(km)
};

// ����
// ���� �ڷ�
struct STN_VAL {
  char  id[16];   // ������ȣ
  char  name[32]; // ������(�ѱ�)
  char  tp[8];    // ����Ư��
  char  tm[16];   // �ð�
  float x, y;     // ��ġ
  float lon, lat; // ���浵
  float ht;       // �ع߰�(m)
  float d;        // ��
  float v;        // ��(�ٶ����ͽ� Vǳ�� ���)
  float nwp;      // ��ġ�� ��
  int   ca;       // �
  int   cl;       // ��������
  int   cm;       // ��������
  int   ct;       // ��������
  int   cd;       // �������
  int   wp;       // �����ϱ�
  int   wc;       // �����ϱ�
  int   pt;       // ��к�ȭ����
  int   rh;       // ���������ð�(6hr)
  float gh;       // ������
  float ta;       // ���
  float ta_max;   // �ְ���
  float ta_min;   // �������
  float td;       // �����µ�
  float tw;       // 
  float wd;       // ǳ��
  float ws;       // ǳ��
  float wd1;      // ǳ��
  float ws1;      // ǳ��
  float ps;       // �ظ���
  float pa;       // �������
  float pr;       // ��к�ȭ��
  float rn;       // ������(mm)
  float hm;       // ����
  float s;        // �ӽ������
  int   disp;     // ǥ��(0: ��ǥ��, 1: ǥ��)
};