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

#define  MAP_INI_FILE   "/DATA/GIS/MAP/map.ini" // �������� ��������
#define  VMAP_INI_FILE  "/fct/www/ROOT/fgd/cht/vmap.ini" // ��ġ�� ���� ������ ��������
#define  CHT_INI_FILE   "/fct/www/ROOT/cht_new/cht_afs_fct.ini" // ���ձ��м� API ��ø ��������
#define  GTS_INI_FILE   "/fct/www/ROOT/gts/gts_afs_img.ini" // ���ձ��м� API ��ø ��������(GTS)
#define  IMG_DIR        "/fct/www/ROOT/img/cht"// �̹��� ���� ���丮
#define  GTS_DIR        "/fct/www/ROOT/img/gts"// �̹��� ���� ���丮(GTS)
#define  LEGEND_DIR     "/fct/www/ROOT/img/legend"// ���� �̹��� ���� ���丮
#define  MAP_DIR        "/DATA/GIS/MAP/bln"    // ��������(����) ���丮
#define  TOPO_DIR       "/DATA/GIS/TOPO/KMA"   // ������ ���丮
#define  COLOR_SET_DIR  "/fct/REF/COLOR/"      // ����ǥ ���� �ӽ������
#define  GK2A_L1B_DIR   "/GK2A/BIN/PRIMARY/L1B/COMPLETE"
#define  GK2A_L2_DIR    "/GK2A/BIN/L2/"
#define  GK2A_CONV_DIR  "/home/fct/REF/INI/GK2A"
#define  COMS_CONV_FILE "/fct/REF/INI/coms_conv.csv"
#define  COMS_BIN_DIR   "/DATA/SAT/COMS/BIN"   // õ�������� ���������
#define  HIMA_CONV_FILE "/fct/REF/INI/hima_conv.csv"
#define  HIMA_BIN_DIR   "/DATA/SAT/HIMA/BIN"
#define  RDR_CMP_DIR    "/DATA/RDR/CMP"         // ���̴��ռ����� ���������
#define  RDR_OBS_DIR    "/DATA/RDR/OBS"         // ���̴� ����м� ���⺸����
#define  LGT_DIR        "/DATA/LGT/KMA"         // ����
#define  RDR_CMP_DIR    "/DATA/RDR/CMP"         // ���̴��ռ����� ���������
#define  RDR_PCP_DIR    "/DATA/RDR/PCP"         // ���������� ���������
#define  RDR_CMP_DIR_BK "/C4N2_DATA_BK/RDR/CMP" // ���̴��ռ����� ���������
#define  RDR_CMP_R3D    "/DATA/RDR/R3D"         // ���̴� 3�����ռ����� ���������
#define  GTS_BUFR_DIR   "/DATA/GTS/BUFR/"
#define  ASCAT_DIR      "/DATA/SAT/EUMETCAST/SCATWIND/"
#define  HY2_DIR        "/DATA/SAT/HY2/"

#define  AFS_HOST_URL   "http://afs.kma.go.kr/" // ���������ý��� host url

// COMS���� (COMS_GRID ����)
#define  COMS_NX   1769
#define  COMS_NY   1559
#define  COMS_SX   850
#define  COMS_SY   1060
#define  COMS_GRID 5

// �����͸� ���ڿ��� (2km ����ũ��)
#define  HIMA_NX   4599
#define  HIMA_NY   3999
#define  HIMA_SX   2299
#define  HIMA_SY   2717
#define  HIMA_GRID 2

// HR-map����(HR, km)
#define  HR_NX  1024
#define  HR_NY  1024
#define  HR_SX  440
#define  HR_SY  770

// HB-map(���̴�240km�ռ�) ����(km)
#define  HB_NX  1152
#define  HB_NY  1440
#define  HB_SX  560
#define  HB_SY  840

// HC-map(���̴�480km�ռ�) ����(km)
#define  HC_NX  1600
#define  HC_NY  1600
#define  HC_SX  800
#define  HC_SY  1000

// H3-map(���̴� ������) ����(km)
#define  H3_NX  3520
#define  H3_NY  3520
#define  H3_SX  2000
#define  H3_SY  2400

// H2-map(���ƽþ�) ����(km)
#define  H2_NX  4800
#define  H2_NY  4800
#define  H2_SX  2400
#define  H2_SY  2400

// H5-map(���ƽþ�+) ����(km)
#define  H5_NX  5760
#define  H5_NY  5760
#define  H5_SX  3328
#define  H5_SY  3328

// HW-map����(HW, km)   WISSDOM
#define  HW_NX  960
#define  HW_NY  960
#define  HW_SX  438   // 440
#define  HW_SY  698   // 770

// �ռ��� �ڷῡ ����� �⺻��
#define  BLANK1  -30000     // No Area
#define  BLANK2  -25000     // No Data
#define  BLANK3  -20000     // Minimum Data

// �̹��� ������ �Ϻ� �⺻��
#define  LEG_pixel   35     // ����ǥ�ÿ���
//#define  TITLE_pixel 20     // ����ǥ�ÿ���
#define  TITLE_pixel 0     // ����ǥ�ÿ���
#define  BORDER_pixel 0    // �׵θ�����

// ����� �ѱ�TTF
#define  FONTTTF   "/fct/REF/FONT/NanumGothic.ttf"
#define  FONTTTF1  "/fct/REF/FONT/NanumGothicBold.ttf"
#define  FONTTTF2  "/fct/REF/FONT/NanumGothicExtraBold.ttf"
#define  FONTTTF9  "/usr/share/fonts/korean/TrueType/gulim.ttf"

#define  max_layer  10    // �ϱ⵵ �ִ� ��ø ���̾� ��
#define  INIT_SIZE 256

//------------------------------------------------------------------------------
// ����� �Է� ����
struct INPUT_VAR {
  // ������ �ڷ� �����
  char  gts[16];      // SFC, ��Ÿ
  char  obs[16];      // ���� ���� (��: ta)
  int   seq;          // ���ؽð� SEQ(��)
  int   seq_fc;       // ���۽ð� SEQ(��) - ��ġ�� ��ǥ�ð�
  int   seq_ef;       // �����ð�
  float missing;      // �� �� �����̸� ����
  float max;          // �ִ밪
  char  model[16];    // ��ġ�� ����
  float diff_max, diff_det; // ����(bias) �ִ밪, ���۰�
  char  mod_varn[300];
  char  varn[8];
  char  cht_name[256];      // �ϱ⵵ ����
  char  cht_mode[32];       // �ϱ⵵ ����(fct: ������, fct2: ������(�ѹݵ� �ϱ⵵X), ana: �м���, ana2: �м���(API ���� X, ���̴� ���� ���), ana3: �м���(�ѹݵ� �ϱ⵵X), ana0: ����, gts: gts ��ȸ)
  int   gis;                // GIS ǥ��
  int   flag;               // ǥ�� �ɼ�(0: �ϱ⵵ �̹���, 1: ���̾� ���� ǥ��)
  int   curl;               // ���� ó��(0: X, 1: O)
  int   save;               // �̹��� ����(0: X, 1: O)
  char  host[16];           // API ȣ�� host
  int   wpf;                // �����������Ϸ� ȣ��(0: X, 1: O)
  int   amdar;              // AMDAR ȣ��(0: X, 1: O)
  int   legend;             // ���� ����
  int   legend_only;        // ���� �̹����� ����(0: X, 1: O)

  // ���� ����
  char  map[16];      // ����� �����ڵ�
  float grid;         // ����ũ��(km)
  float lon1, lon2, lat1, lat2;  // ��������
  int   topo;         // ������ �ݿ� ����(1:�� �ݿ�, 0:�ݿ����� ����)
  char  zoom_x[16];   // X���� Ȯ��
  char  zoom_y[16];   // Y���� Ȯ��
  double startx;
  double starty;
  double endx;
  double endy;
  double centerx, centery;

  // ȭ��ǥ���
  int   size;         // �̹���ũ��(�ȼ�) : NI�� �ش�
  char  color[8];     // Į�����ǥ
  int   title_pixel;  // ���񿵿�ũ��(�ȼ�)
  int   title;        // ����ǥ��(1) ����
  int   sms;          // ��Ȱȭ ����
  char  sat[16];      // ct, ir1, wvp
  float ZRa;          // Z-R������� ���(�⺻:200)  Z=aR^b
  float ZRb;          // Z-R������� ����(�⺻:1.6)
  char  layer[10];    // ���̾� ����
  char  sat_ok;       // �����ڷ� ���翩��(1:����, 0:����)
  int   seq_sat;      // �����ð� SEQ(��)
  int   seq_rdr;      // ���̴��ð� SEQ(��)
  int   num_ccn;      // �ռ��� ���Ǵ� ä�μ�
  char  ccn[4];       // ����ä�ι�ȣ
  float gamma[4];     // ������
  int   itv;          // ����ǥ�� ���� +-itv��
  int   pnts;         // ����X(0) ����O(1)
  float font_size;    // ��Ʈ ũ��
  int   border;       // ���� �׵θ�
  int   color_adjust; // �� �ڵ�����
  int   color_wind;   // �ٶ��� �� ����
  int   bias_disp;    // �⺻(0) ���ڷ� ǥ��(1)

  // ����� �ڷ�
  int   num_stn;      // ǥ���� gts ������
  int   NX, NY;       // ���ڼ�, ���� ���������� ������ 1�� ���ϸ� ��
  int   SX, SY;       // ���� ������ ��ġ
  int   NI, NJ;       // �ڷ�ǥ���̹��� ����(�ȼ�)
  int   GI, GJ;       // ��ü �̹��� ����(�ȼ�)
  int   OI, OJ;       // �ڷ�ǥ���̹��� ��ġ(�ȼ�)
  int   num_color;    // �����
  int   disp;
  int   help;

  // ��������
  char  fname[120];   // ���ϸ�
  int   mode;         // �⺻(0) ���̾� �̹��� ����(1)
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
  float ts;       // ��ǥ�µ�
  float tw;       // ����
  float wd;       // ǳ��
  float wd_0;     // ǳ��(���� ��)
  float ws;       // ǳ��
  float wd1;      // ǳ��
  float wd1_0;    // ǳ��(���� ��)
  float ws1;      // ǳ��
  float ps;       // �ظ���
  float pa;       // �������
  float pr;       // ��к�ȭ��
  float rn;       // ������(mm)
  float hm;       // ����
  float s;        // �ӽ������
  int   pm10;     // PM10 ��
  float wh;       // �����İ�
  char  bufr[256];// BUFR ���ϸ�
  int   msg;      // BUFR �޽���
};