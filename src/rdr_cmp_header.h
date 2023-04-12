//-----------------------------------------------------------------------------
// 레이더 합성자료의 헤더용 구조체

// 시간 정보
#pragma pack(push, 1) 
struct TIME_SS {
  short  YY;
  char   MM;
  char   DD;
  char   HH;
  char   MI;
  char   SS;
};
#pragma pack(pop) 

// 기본 정보
#pragma pack(push, 1) 
struct RDR_CMP_HEAD {
  char   version;       // 포맷 버전
  short  ptype;         // 생산Product
  struct TIME_SS tm;    // 레이더 관측시각
  struct TIME_SS tm_in; // 합성자료 생성시각
  char   num_stn;       // 합성에 사용된 레이더 지점수
  char   map_code;      // 지도정보 코드
  char   map_etc;       // 기타 지도 정보코드(예비)
  short  nx;            // X축 격자점수 (1개 격자당 2 bytes로 저장함)
  short  ny;            // Y축 격자점수
  short  nz;            // Z축 격자점수
  short  dxy;           // 격자점간의 수평거리(m)
  short  dz;            // 격자점간의 수직거리(m) (nz=1이면 dz=0)
  short  z_min;         // nz > 1인 경우, 최저고도값(m) (nz <= 1이면 0)
  char   num_data;      // (nx*ny*nz)를 1개 자료블럭으로 했을때, 저장된 자료블럭수
  char   data_code[16]; // 저장된 자료블력별 특성 코드
  char   etc[15];
};
#pragma pack(pop) 
/* 
version : 2(강수유무)

ptype 정의
  0(PPI0), 1(CAPPI), 2(CMAX), 3(ETOP), 4(EBASE), 5(HSR), 6(수상체), 7(VIL), 8(WIND)
  9(LNG), 10(누적강수,PCP),  15(NUM),
  30(3차원CZ), 31(3차원DR), 32(3차원RH), 33(3차원KD), 34(3차원HC)
  50(강수유무)

map_code = 1 : HB map
  지도 투영법 : LCC
  기준 위경도 : 38N, 126E
  기준 격자점 위치 : 좌에서 561번째, 밑에서 841번째
map_code = 2 : HC map
map_code = 3 : HR map
map_code = 5 : HL map
map_code = 7 : H3 map

data_code[] 값에 따라 정의되어야하는 변수 목록
  data_out;     // 레이더 관측영역밖 NULL값
  data_in;      // 레이더 영역내 비관측영역 NULL값
  data_min;     // 관측영역내 표시를 위한 최소값
  data_minus;   // 저장된 값에서 이 값을 먼저 뺌
  data_scale;   // 빼고 난 다음에 이 값으로 나누어서 실값을 찾음
  data_unit;    // 위 방식으로 해독한 후에 단위

data_code[0] = 1 (에코)
  data_out = -30000 | data_in = -25000 | data_min = -20000 |
  data_minus = 0    | data_scale = 100 | data_unit = dBz
data_code[0] = 2 (고도)
  data_out = -30000 | data_in = -25000 | data_min = -20000 |
  data_minus = 0    | data_scale = 1   | data_unit = m
data_code[0] = 3 (지점순서)
  data_out = -30000 | data_in = -25000 | data_min = -20000 |
  data_minus = 0    | data_scale = 1   | data_unit = 없음
data_code[0] = 4 (자료수) .. 누적에 사용된 자료수
  data_out = -30000 | data_in = -25000 | data_min = -20000 |
  data_minus = 0    | data_scale = 1   | data_unit = 없음
data_code[0] = 5 (강수량)
  data_out = -30000 | data_in = -25000 | data_min = -20000 |
  data_minus = 0    | data_scale = 100 | data_unit = mm
data_code[0] = 6 (수상체)
  data_out = -300   | data_in = -250   | data_min = -200   |
  data_minus = 0    | data_scale = 1   | data_unit = 없음
data_code[0] = 7 (강수유형) .. 1(강수없음), 2(비), 3(눈비), 4(눈)
  data_out = 0      | data_in = 1      | data_min = 0   |
  data_minus = 0    | data_scale = 1   | data_unit = 없음
data_code[0] = 8 (온도)
  data_out = -30000 | data_in = -25000 | data_min = -20000 |
  data_minus= 20000 | data_scale = 100 | data_unit = C
data_code[0] = 15 (3km이하 고도에 대하여 합성시 일정값 이상 에코가 탐지된 횟수)
  data_out = 250    | data_in =  245   | data_min =  0     |
  data_minus = 0    | data_scale = 0   | data_unit = 없음

etc .. 3차원인 경우
  etc[0] = 곱한 값,  etc[1] = 더한 값  (저장된 값 = ( 실제값 + ect[1] ) * etc[0] )
*/

// 합성에 사용된 사이트별 파일정보 (총48개까지로 제한)
#pragma pack(push, 1) 
struct RDR_CMP_STN_LIST {
  char   stn_cd[6];       // 레이더 지점코드
  struct TIME_SS tm;      // 레이더 관측시각
  struct TIME_SS tm_in;   // 레이더 자료 생성시각
};
#pragma pack(pop) 

//-----------------------------------------------------------------------------
// 합성자료 누적강수량 자료 구조체
#pragma pack(push, 1) 
struct RDR_PCP_HEAD {
  char   version;       // 포맷 버전
  short  ptype;         // 생산Product
  struct TIME_SS tm;    // 레이더 관측시각
  struct TIME_SS tm_in; // 합성자료 생성시각
  char   pcp_code;      // 자료특성 코드
  char   map_code;      // 지도정보 코드
  char   map_etc;       // 기타 지도 정보코드(예비)
  short  nx;            // X축 격자점수 (1개 격자당 2 bytes로 저장함)
  short  ny;            // Y축 격자점수
  short  nz;            // Z축 격자점수
  short  dxy;           // 격자점간의 수평거리(m)
  short  dz;            // 격자점간의 수직거리(m) (nz=1이면 dz=0)
  short  z_min;         // nz > 1인 경우, 최저고도값(m) (nz <= 1이면 0)
  char   num_data;      // (nx*ny*nz)를 1개 자료블럭으로 했을때, 저장된 자료블럭수
  char   data_code[16]; // 저장된 자료블력별 특성 코드
  short  acc_min;       // 누적기간(분)
  short  itv_min;       // 자료간 간격(분)
  short  cmp_cnt;       // 합성에 사용된 자료수
  char   etc[9];
};
#pragma pack(pop) 
/* 
pcp_code 정의
  1(HSR_KMA), 2(HSR_EXT 마스킹), 3(HSR_EXT)
*/

//-----------------------------------------------------------------------------
// 3차원 합성의 기본 정보
#pragma pack(push, 1) 
struct RDR_R3D_HEAD {
  char   version;       // 포맷 버전
  short  ptype;         // 생산Product
  struct TIME_SS tm;    // 레이더 관측시각
  struct TIME_SS tm_in; // 합성자료 생성시각
  char   num_stn;       // 합성에 사용된 레이더 지점수
  char   map_code;      // 지도정보 코드
  char   map_etc;       // 기타 지도 정보코드(예비)
  short  nx;            // X축 격자점수
  short  ny;            // Y축 격자점수
  short  nz;            // Z축 격자점수
  short  dxy;           // 격자점간의 수평거리(m)
  short  dz;            // 격자점간의 수직거리(m) (nz=1이면 dz=0)
  short  z_min;         // nz > 1인 경우, 최저고도값(m) (nz <= 1이면 0)
  char   num_data;      // (nx*ny*nz)를 1개 자료블럭으로 했을때, 저장된 자료블럭수
  short  dz2;           // 2번째 단계의 수직격자점간 거리(m)
  short  z_min2;        // 2번째 단계의 시작 고도값(m)
  short  data_out;      // 레이더 관측영역밖 NULL값
  short  data_in;       // 레이더 영역내 비관측영역 NULL값
  short  data_min;      // 관측영역내 표시를 위한 최소값
  short  data_minus;    // 저장된 값에서 이 값을 먼저 뺌
  short  data_scale;    // 빼고 난 다음에 이 값으로 나누어서 실값을 찾음
  char   data_unit;     // 위 방식으로 해독한 후에 단위 코드
  short  num_obs;       // 3차원 관측분석에 사용된 자료수
  short  etc[7];
};
#pragma pack(pop) 
/* 
version = 1 (격자점 자료 크기가 2byte, 지점목록정보가 헤더에 포함됨)
          3 (격자점 자료 크기가 1byte, 지점목록정보가 헤더에 없음)
ptype = 30(3차원CZ), 31(3차원DR), 32(3차원RH), 33(3차원KD), 34(3차원HC)
        51(기온), 52(이슬점), 53(기압)
map_code = 3 (HR map)
저장된 값 = 실제값*data_scale + data_minus
data_unit = 0(무차원), 1(dBZ), 2(dB), 3(KD),
            21(C), 22(hPa)
*/
