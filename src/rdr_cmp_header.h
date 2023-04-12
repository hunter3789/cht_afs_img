//-----------------------------------------------------------------------------
// ���̴� �ռ��ڷ��� ����� ����ü

// �ð� ����
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

// �⺻ ����
#pragma pack(push, 1) 
struct RDR_CMP_HEAD {
  char   version;       // ���� ����
  short  ptype;         // ����Product
  struct TIME_SS tm;    // ���̴� �����ð�
  struct TIME_SS tm_in; // �ռ��ڷ� �����ð�
  char   num_stn;       // �ռ��� ���� ���̴� ������
  char   map_code;      // �������� �ڵ�
  char   map_etc;       // ��Ÿ ���� �����ڵ�(����)
  short  nx;            // X�� �������� (1�� ���ڴ� 2 bytes�� ������)
  short  ny;            // Y�� ��������
  short  nz;            // Z�� ��������
  short  dxy;           // ���������� ����Ÿ�(m)
  short  dz;            // ���������� �����Ÿ�(m) (nz=1�̸� dz=0)
  short  z_min;         // nz > 1�� ���, ��������(m) (nz <= 1�̸� 0)
  char   num_data;      // (nx*ny*nz)�� 1�� �ڷ������ ������, ����� �ڷ����
  char   data_code[16]; // ����� �ڷ��º� Ư�� �ڵ�
  char   etc[15];
};
#pragma pack(pop) 
/* 
version : 2(��������)

ptype ����
  0(PPI0), 1(CAPPI), 2(CMAX), 3(ETOP), 4(EBASE), 5(HSR), 6(����ü), 7(VIL), 8(WIND)
  9(LNG), 10(��������,PCP),  15(NUM),
  30(3����CZ), 31(3����DR), 32(3����RH), 33(3����KD), 34(3����HC)
  50(��������)

map_code = 1 : HB map
  ���� ������ : LCC
  ���� ���浵 : 38N, 126E
  ���� ������ ��ġ : �¿��� 561��°, �ؿ��� 841��°
map_code = 2 : HC map
map_code = 3 : HR map
map_code = 5 : HL map
map_code = 7 : H3 map

data_code[] ���� ���� ���ǵǾ���ϴ� ���� ���
  data_out;     // ���̴� ���������� NULL��
  data_in;      // ���̴� ������ ��������� NULL��
  data_min;     // ���������� ǥ�ø� ���� �ּҰ�
  data_minus;   // ����� ������ �� ���� ���� ��
  data_scale;   // ���� �� ������ �� ������ ����� �ǰ��� ã��
  data_unit;    // �� ������� �ص��� �Ŀ� ����

data_code[0] = 1 (����)
  data_out = -30000 | data_in = -25000 | data_min = -20000 |
  data_minus = 0    | data_scale = 100 | data_unit = dBz
data_code[0] = 2 (��)
  data_out = -30000 | data_in = -25000 | data_min = -20000 |
  data_minus = 0    | data_scale = 1   | data_unit = m
data_code[0] = 3 (��������)
  data_out = -30000 | data_in = -25000 | data_min = -20000 |
  data_minus = 0    | data_scale = 1   | data_unit = ����
data_code[0] = 4 (�ڷ��) .. ������ ���� �ڷ��
  data_out = -30000 | data_in = -25000 | data_min = -20000 |
  data_minus = 0    | data_scale = 1   | data_unit = ����
data_code[0] = 5 (������)
  data_out = -30000 | data_in = -25000 | data_min = -20000 |
  data_minus = 0    | data_scale = 100 | data_unit = mm
data_code[0] = 6 (����ü)
  data_out = -300   | data_in = -250   | data_min = -200   |
  data_minus = 0    | data_scale = 1   | data_unit = ����
data_code[0] = 7 (��������) .. 1(��������), 2(��), 3(����), 4(��)
  data_out = 0      | data_in = 1      | data_min = 0   |
  data_minus = 0    | data_scale = 1   | data_unit = ����
data_code[0] = 8 (�µ�)
  data_out = -30000 | data_in = -25000 | data_min = -20000 |
  data_minus= 20000 | data_scale = 100 | data_unit = C
data_code[0] = 15 (3km���� ���� ���Ͽ� �ռ��� ������ �̻� ���ڰ� Ž���� Ƚ��)
  data_out = 250    | data_in =  245   | data_min =  0     |
  data_minus = 0    | data_scale = 0   | data_unit = ����

etc .. 3������ ���
  etc[0] = ���� ��,  etc[1] = ���� ��  (����� �� = ( ������ + ect[1] ) * etc[0] )
*/

// �ռ��� ���� ����Ʈ�� �������� (��48�������� ����)
#pragma pack(push, 1) 
struct RDR_CMP_STN_LIST {
  char   stn_cd[6];       // ���̴� �����ڵ�
  struct TIME_SS tm;      // ���̴� �����ð�
  struct TIME_SS tm_in;   // ���̴� �ڷ� �����ð�
};
#pragma pack(pop) 

//-----------------------------------------------------------------------------
// �ռ��ڷ� ���������� �ڷ� ����ü
#pragma pack(push, 1) 
struct RDR_PCP_HEAD {
  char   version;       // ���� ����
  short  ptype;         // ����Product
  struct TIME_SS tm;    // ���̴� �����ð�
  struct TIME_SS tm_in; // �ռ��ڷ� �����ð�
  char   pcp_code;      // �ڷ�Ư�� �ڵ�
  char   map_code;      // �������� �ڵ�
  char   map_etc;       // ��Ÿ ���� �����ڵ�(����)
  short  nx;            // X�� �������� (1�� ���ڴ� 2 bytes�� ������)
  short  ny;            // Y�� ��������
  short  nz;            // Z�� ��������
  short  dxy;           // ���������� ����Ÿ�(m)
  short  dz;            // ���������� �����Ÿ�(m) (nz=1�̸� dz=0)
  short  z_min;         // nz > 1�� ���, ��������(m) (nz <= 1�̸� 0)
  char   num_data;      // (nx*ny*nz)�� 1�� �ڷ������ ������, ����� �ڷ����
  char   data_code[16]; // ����� �ڷ��º� Ư�� �ڵ�
  short  acc_min;       // �����Ⱓ(��)
  short  itv_min;       // �ڷᰣ ����(��)
  short  cmp_cnt;       // �ռ��� ���� �ڷ��
  char   etc[9];
};
#pragma pack(pop) 
/* 
pcp_code ����
  1(HSR_KMA), 2(HSR_EXT ����ŷ), 3(HSR_EXT)
*/

//-----------------------------------------------------------------------------
// 3���� �ռ��� �⺻ ����
#pragma pack(push, 1) 
struct RDR_R3D_HEAD {
  char   version;       // ���� ����
  short  ptype;         // ����Product
  struct TIME_SS tm;    // ���̴� �����ð�
  struct TIME_SS tm_in; // �ռ��ڷ� �����ð�
  char   num_stn;       // �ռ��� ���� ���̴� ������
  char   map_code;      // �������� �ڵ�
  char   map_etc;       // ��Ÿ ���� �����ڵ�(����)
  short  nx;            // X�� ��������
  short  ny;            // Y�� ��������
  short  nz;            // Z�� ��������
  short  dxy;           // ���������� ����Ÿ�(m)
  short  dz;            // ���������� �����Ÿ�(m) (nz=1�̸� dz=0)
  short  z_min;         // nz > 1�� ���, ��������(m) (nz <= 1�̸� 0)
  char   num_data;      // (nx*ny*nz)�� 1�� �ڷ������ ������, ����� �ڷ����
  short  dz2;           // 2��° �ܰ��� ������������ �Ÿ�(m)
  short  z_min2;        // 2��° �ܰ��� ���� ����(m)
  short  data_out;      // ���̴� ���������� NULL��
  short  data_in;       // ���̴� ������ ��������� NULL��
  short  data_min;      // ���������� ǥ�ø� ���� �ּҰ�
  short  data_minus;    // ����� ������ �� ���� ���� ��
  short  data_scale;    // ���� �� ������ �� ������ ����� �ǰ��� ã��
  char   data_unit;     // �� ������� �ص��� �Ŀ� ���� �ڵ�
  short  num_obs;       // 3���� �����м��� ���� �ڷ��
  short  etc[7];
};
#pragma pack(pop) 
/* 
version = 1 (������ �ڷ� ũ�Ⱑ 2byte, ������������� ����� ���Ե�)
          3 (������ �ڷ� ũ�Ⱑ 1byte, ������������� ����� ����)
ptype = 30(3����CZ), 31(3����DR), 32(3����RH), 33(3����KD), 34(3����HC)
        51(���), 52(�̽���), 53(���)
map_code = 3 (HR map)
����� �� = ������*data_scale + data_minus
data_unit = 0(������), 1(dBZ), 2(dB), 3(KD),
            21(C), 22(hPa)
*/
