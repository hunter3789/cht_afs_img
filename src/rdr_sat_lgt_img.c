/*******************************************************************************
 *  천리안 or 히마와리 위성 표출 (IR1,WVP)
 *******************************************************************************/
int sat_bin_disp(gdImagePtr im)
{
  struct stat st;
  char   fname[256], head[64];
  int    seq;
  int    YY, MM, DD, HH, MI;
  int    code;

  // 구름모의영상
  if (strstr(var.sat,"rmir") || strstr(var.sat,"rmwv")) {
    var.seq_sat = var.seq;
    sat_fcst_disp(im);

    return;
  }

  // 요청시간에서 과거로 갈때 가장 가까운 자료를 사용
  // 천리안2-히마와리-천리안1호 순서로 찾음
  // 천리안2 먼저 찾음(40분이내)
  var.seq_sat = -1;
  var.sat_ok = -1;

  for (seq = var.seq; seq >= var.seq-40; seq--) {
    seq2time(seq-9*60, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
    if (!strcmp(var.map,"H2") || !strcmp(var.map,"H3") || !strcmp(var.map,"H4") || !strcmp(var.map,"H5")) {
      if (!strcmp(var.sat,"ir1") || !strcmp(var.sat,"ir1_enhc"))
        strcpy(head, "gk2a_ami_le1b_ir105_ea020lc");
      else if (!strcmp(var.sat,"wv063") || !strcmp(var.sat,"wv063_enhc"))
        strcpy(head, "gk2a_ami_le1b_wv063_ea020lc");
      else if (!strcmp(var.sat,"wv069") || !strcmp(var.sat,"wv069_enhc"))
        strcpy(head, "gk2a_ami_le1b_wv069_ea020lc");
      else if (!strcmp(var.sat,"wv073") || !strcmp(var.sat,"wv073_enhc"))
        strcpy(head, "gk2a_ami_le1b_wv073_ea020lc");
      else if (!strcmp(var.sat,"rgb_daynight") || !strcmp(var.sat,"rgb_dust") || !strcmp(var.sat,"rgb_natural") || !strcmp(var.sat,"rgb_wv"))
        strcpy(head, "gk2a_ami_le1b_rgb-daynight_ea020lc");
      else if (!strcmp(var.sat,"sst") || !strcmp(var.sat,"lst"))
        sprintf(head, "gk2a_ami_le2_%s_ea020lc", var.sat);
      else if (!strcmp(var.sat,"ctt") || !strcmp(var.sat,"cth"))
        sprintf(head, "gk2a_ami_le2_ctps_ea020lc");
      else
        strcpy(head, "gk2a_ami_le1b_wv063_ea020lc");

      if (!strcmp(var.sat,"rgb_daynight") || !strcmp(var.sat,"rgb_dust") || !strcmp(var.sat,"rgb_natural") || !strcmp(var.sat,"rgb_wv")) {
        sprintf(fname, "%s/EA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.bin",
              GK2A_L1B_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      else if (!strcmp(var.sat,"sst")) {
        sprintf(fname, "%s/SST/EA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
              GK2A_L2_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      else if (!strcmp(var.sat,"lst")) {
        sprintf(fname, "%s/LST/EA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
              GK2A_L2_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      else if (!strcmp(var.sat,"ctt") || !strcmp(var.sat,"cth")) {
        sprintf(fname, "%s/CTPS/EA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
              GK2A_L2_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      else {
        sprintf(fname, "%s/EA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
              GK2A_L1B_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
    }
    else if (!strcmp(var.map,"EA_CHT") || !strcmp(var.map,"TP") || !strcmp(var.map,"NHEM") || !strcmp(var.map,"WORLD")) {
      if (!strcmp(var.sat,"ir1") || !strcmp(var.sat,"ir1_enhc"))
        strcpy(head, "gk2a_ami_le1b_ir105_tp020lc");
      else if (!strcmp(var.sat,"wv063") || !strcmp(var.sat,"wv063_enhc"))
        strcpy(head, "gk2a_ami_le1b_wv063_tp020lc");
      else if (!strcmp(var.sat,"wv069") || !strcmp(var.sat,"wv069_enhc"))
        strcpy(head, "gk2a_ami_le1b_wv069_tp020lc");
      else if (!strcmp(var.sat,"wv073") || !strcmp(var.sat,"wv073_enhc"))
        strcpy(head, "gk2a_ami_le1b_wv073_tp020lc");
      else if (!strcmp(var.sat,"rgb_daynight") || !strcmp(var.sat,"rgb_dust") || !strcmp(var.sat,"rgb_natural") || !strcmp(var.sat,"rgb_wv"))
        strcpy(head, "gk2a_ami_le1b_rgb-daynight_tp020lc");
      else if (!strcmp(var.sat,"sst") || !strcmp(var.sat,"lst"))
        sprintf(head, "gk2a_ami_le2_%s_ea020lc", var.sat);
      else if (!strcmp(var.sat,"ctt") || !strcmp(var.sat,"cth"))
        sprintf(head, "gk2a_ami_le2_ctps_ea020lc");
      else 
        strcpy(head, "gk2a_ami_le1b_wv063_tp020lc");

      if (!strcmp(var.sat,"rgb_daynight") || !strcmp(var.sat,"rgb_dust") || !strcmp(var.sat,"rgb_natural") || !strcmp(var.sat,"rgb_wv")) {
        sprintf(fname, "%s/TP/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.bin",
              GK2A_L1B_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      else if (!strcmp(var.sat,"sst")) {
        sprintf(fname, "%s/SST/EA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
              GK2A_L2_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      else if (!strcmp(var.sat,"lst")) {
        sprintf(fname, "%s/LST/EA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
              GK2A_L2_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      else if (!strcmp(var.sat,"ctt") || !strcmp(var.sat,"cth")) {
        sprintf(fname, "%s/CTPS/EA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
              GK2A_L2_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      else {
        sprintf(fname, "%s/TP/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
              GK2A_L1B_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
    }
    else {
      if (!strcmp(var.sat,"ir1") || !strcmp(var.sat,"ir1_enhc"))
        strcpy(head, "gk2a_ami_le1b_ir105_ela020lc");
      else if (!strcmp(var.sat,"wv063") || !strcmp(var.sat,"wv063_enhc"))
        strcpy(head, "gk2a_ami_le1b_wv063_ela020lc");
      else if (!strcmp(var.sat,"wv069") || !strcmp(var.sat,"wv069_enhc"))
        strcpy(head, "gk2a_ami_le1b_wv069_ela020lc");
      else if (!strcmp(var.sat,"wv073") || !strcmp(var.sat,"wv073_enhc"))
        strcpy(head, "gk2a_ami_le1b_wv073_ela020lc");
      else if (!strcmp(var.sat,"rgb_daynight") || !strcmp(var.sat,"rgb_dust") || !strcmp(var.sat,"rgb_natural") || !strcmp(var.sat,"rgb_wv"))
        strcpy(head, "gk2a_ami_le1b_rgb-daynight_ela020lc");
      else if (!strcmp(var.sat,"sst") || !strcmp(var.sat,"lst"))
        sprintf(head, "gk2a_ami_le2_%s_ea020lc", var.sat);
      else if (!strcmp(var.sat,"ctt") || !strcmp(var.sat,"cth"))
        sprintf(head, "gk2a_ami_le2_ctps_ea020lc");
      else 
        strcpy(head, "gk2a_ami_le1b_wv063_ela020lc");

      if (!strcmp(var.sat,"rgb_daynight") || !strcmp(var.sat,"rgb_dust") || !strcmp(var.sat,"rgb_natural") || !strcmp(var.sat,"rgb_wv")) {
        sprintf(fname, "%s/ELA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.bin",
              GK2A_L1B_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      else if (!strcmp(var.sat,"sst")) {
        sprintf(fname, "%s/SST/EA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
              GK2A_L2_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      else if (!strcmp(var.sat,"lst")) {
        sprintf(fname, "%s/LST/EA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
              GK2A_L2_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      else if (!strcmp(var.sat,"ctt") || !strcmp(var.sat,"cth")) {
        sprintf(fname, "%s/CTPS/EA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
              GK2A_L2_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
      else {
        sprintf(fname, "%s/ELA/%04d%02d/%02d/%02d/%s_%04d%02d%02d%02d%02d.nc",
              GK2A_L1B_DIR, YY, MM, DD, HH, head, YY, MM, DD, HH, MI);
      }
    }
    code = stat(fname, &st);
    if (code == 0 && st.st_size > 100) {
      var.seq_sat = seq;
      var.sat_ok = 3;
      break;
    }
  }

  // 천리안1호
  if (var.sat_ok < 0) {
    for (seq = var.seq; seq >= var.seq-60; seq--) {
      seq2time(seq-9*60, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
      if (!strcmp(var.sat,"ir1"))
        strcpy(head, "coms_le1b_ir1_ch1_nwpn");
      else
        strcpy(head, "coms_le1b_wv_ch3_nwpn");
      sprintf(fname, "%s/%04d%02d/%02d/%s_%04d%02d%02d%02d%02d.bin.gz",
              COMS_BIN_DIR, YY, MM, DD, head, YY, MM, DD, HH, MI);
      code = stat(fname, &st);
      if (code == 0 && st.st_size > 100) {
        var.seq_sat = seq;
        var.sat_ok = 1;
        break;
      }
    }
  }

  // 히마와리
  if (var.sat_ok < 0) {
    for (seq = var.seq; seq >= var.seq-60; seq--) {
      seq2time(seq-9*60, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
      if (!strcmp(var.sat,"ir1"))
        strcpy(head, "himawari8_ahi_le1b_ch13_nwpn");
      else
        strcpy(head, "himawari8_ahi_le1b_ch08_nwpn");
      sprintf(fname, "%s/%04d%02d/%02d/%s_%04d%02d%02d%02d%02d.bin.gz",
              HIMA_BIN_DIR, YY, MM, DD, head, YY, MM, DD, HH, MI);
      code = stat(fname, &st);
      if (code == 0 && st.st_size > 100) {
        var.seq_sat = seq;
        var.sat_ok = 2;
        break;
      }
    }
  }

  if (var.sat_ok < 0) return -1;

  // 천리안1호
  if (var.sat_ok == 1) {
    if (!strcmp(var.sat,"rgb_day") || !strcmp(var.sat,"rgb_night") || !strcmp(var.sat,"rgb_daynight") || !strcmp(var.sat,"rgb_dust") || !strcmp(var.sat,"rgb_natural") || !strcmp(var.sat,"rgb_wv")) {
      return;
    }

    if (coms_bin_disp(im, fname) < 0) var.sat_ok = 0;
  }
  // 히마와리
  else if (var.sat_ok == 2) {
    if (!strcmp(var.sat,"rgb_day") || !strcmp(var.sat,"rgb_night") || !strcmp(var.sat,"rgb_daynight") || !strcmp(var.sat,"rgb_dust") || !strcmp(var.sat,"rgb_natural") || !strcmp(var.sat,"rgb_wv")) {
      return;
    }

    if (hima_bin_disp(im, fname) < 0) var.sat_ok = 0;
  }
  // GK2A
  else if (var.sat_ok == 3) {
    if (!strcmp(var.sat,"rgb_day") || !strcmp(var.sat,"rgb_night")) {
      gk2a_rgb_disp(im, var.seq_sat);
    }
    else if (!strcmp(var.sat,"rgb_daynight") || !strcmp(var.sat,"rgb_dust") || !strcmp(var.sat,"rgb_natural") || !strcmp(var.sat,"rgb_wv")) {
      gk2a_rgb_bin_disp(im, var.seq_sat);
    }
    else {
      gk2a_bin_disp(im, var.seq_sat);
    }
  }

  return 0;
}

/*=============================================================================*
 *  Histogram Equalization
 *=============================================================================*/
int make_wgtstrch(
  float **g,
  int ny,
  int nx,
  int min,
  int max,
  float missing
)
{
  int   iscl = 250;
  int   ncnt[iscl+1], nval[iscl+1];
  int   nsum, nn;
  int   i, j, k;
  float bval = 255.0, gscl = 250.0;
  float dc, dd, df;
  float **g1, **g2;
 
  dc = max - min;
  g1 = matrix(0, ny, 0, nx);
  g2 = matrix(0, ny, 0, nx);
  nn = 0;

  for (j=0; j<=ny; j++) {
    for (i=0; i<=nx; i++) {
      if (g[j][i] == missing) continue;
      if (g[j][i] > max) df = 0;
      else if (g[j][i] <= min) df = max - min;
      else df = max - g[j][i];
      //df = abs(g[j][i] - max);
      g1[j][i] = (float)((int)(iscl*df/dc));

      if (g1[j][i] > 249) g1[j][i] = 249.;
      else if (g1[j][i] < 0) g1[j][i] = 0.;
      nn++;
    }
  }

  for (i=0; i<=iscl; i++) {
    ncnt[i] = 0;
    nval[i] = 0;
  }
  nsum = 0;
  dd = (float)(nn/(iscl+1));

  for (j=0; j<=ny; j++) {
    for (i=0; i<=nx; i++) {
      if (g[j][i] == missing) continue;
      g2[j][i] = iscl - (iscl/bval) * g1[j][i];
    }
  }

  for (j=0; j<=ny; j++) {
    for (i=0; i<=nx; i++) {
      if (g[j][i] == missing) continue;
      k = (int)(g2[j][i]);
      ncnt[k]++;
    }
  }

  for (k=0; k<=iscl; k++) {
    nsum += ncnt[k];
    nval[k] = (int)(nsum/dd);
  }

  for (j=0; j<=ny; j++) {
    for (i=0; i<=nx; i++) {
      if (g[j][i] == missing) continue;
      k = (int)(g2[j][i]);
      g[j][i] = (float)(nval[k]);
    }
  }

  free_matrix(g1, 0, ny, 0, nx);
  free_matrix(g2, 0, ny, 0, nx);
  
  return;
}

/*******************************************************************************
 *  천리안 위성 표출 (IR1,WVP)
 *******************************************************************************/
int coms_bin_disp(gdImagePtr im, char *fname)
{
  FILE  *fp;
  gzFile fg;
  struct stat st;
  char   head[32];
  char   buf[1024], tmp[1024];
  float  **g;
  short  com1;
  unsigned char p[(COMS_NX+1)*2];
  float  cnv[1024], g1;
  float  dg, dgt;
  float  zm = 1.0, xo = 0.0, yo = 0.0, zx, zy, x1, y1, v1;
  float  xd, yd, xt, yt, avg, std;
  float  ta, ta_min, ta_max, t1;
  int    offsetx, offsety;
  int    nx, ny, sx, sy;
  int    YY, MM, DD, HH, MI;
  int    H1, H2, S, L, R, G, B;
  int    ok = 0, seq, code, chn, c1;
  int    i, j, k, n, i1, j1, ix1, iy1, ix2, iy2;
  float  dxo = COMS_SX*COMS_GRID - var.SX;
  float  dyo = COMS_SY*COMS_GRID - var.SY;
  float  grid_zoom = var.grid/COMS_GRID;
  float  missing = -999;

  // 1. 변환표 파일 읽기
  if (!strcmp(var.sat,"ir1"))
    chn = 1;  // IR1
  else
    chn = 5;  // WVP

  if ((fp = fopen(COMS_CONV_FILE,"r")) == NULL) return -1;
  while (fgets(buf, 1024, fp) != NULL) {
    if (buf[0] == '#') continue;
    getword(tmp, buf, ',');  n = atoi(tmp);
    for (i = 0; i < 10; i++) {
      getword(tmp, buf, ',');
      if (chn == i) {
        cnv[n] = atof(tmp) - 273.15;
        break;
      }
    }
  }
  fclose(fp);

  // 3. 자료 읽기
  if ((fg = gzopen(fname,"rb")) == NULL) return -1;
  g = matrix(0, COMS_NY, 0, COMS_NX);
  gzbuffer(fg, 64*1024);
  for (j = 0; j <= COMS_NY; j++) {
    j1 = COMS_NY - j;
    n = gzread(fg, p, (COMS_NX+1)*2);
    for (i = 0; i <= COMS_NX; i++) {
      com1 = p[i*2]*256 + p[i*2+1];
      c1 = com1;

      if (c1 >= 0 && c1 < 1024) g[j1][i] = cnv[c1];
      else g[j1][i] = missing;
    }
  }
  gzclose(fg);

  // 4. 최대,최소값 기반으로 범위 설정
  if (!strcmp(var.sat,"ir1")) {
    ta_min = -60;  ta_max = 40;
  }
  else {
    ta_min = -60;  ta_max = 0;
  }

  if (!strcmp(var.sat,"ir1")) {
    ta_min = 190; ta_max = 320;
  }
  else {
    ta_min = 200; ta_max = 275;
  }
  ta_max -= 273.15;
  ta_min -= 273.15;

  make_wgtstrch(g, COMS_NY, COMS_NX, (int)ta_min, (int)ta_max, missing);

  ta_max = 249;
  ta_min = 0;

  // 5. 색상표 설정 (흑백 or 칼라)
  for (num_color_sat = 0, ta = ta_min; ta <= ta_max; ta += 1, num_color_sat++) {
    data_sat[num_color_sat] = ta;

    L = (int)(240*(ta_max - ta)/(ta_max - ta_min) + 16);
    if (L < 0) L = 0;
    if (L > 254) L = 254;

    color_sat[num_color_sat] = gdImageColorAllocate(im, L, L, L);
  }

  // 6. 확대시, 중심위치와 확대비율 계산
  if (var.zoom_x[0] != '0') {
    for (i = 0; i < 7; i++, zm *= 1.5) {
      zx = var.zoom_x[i]-'0';
      zy = var.zoom_y[i]-'0';
      if (zx == 0 || zy == 0) break;

      xo += ((float)(var.NX)/24.0*(zx-1)/zm);
      yo += ((float)(var.NY)/24.0*(zy-1)/zm);
    }
  }
  dg = (float)(var.NX)/(float)(var.NI);

  if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD")) {
    // 7. 이미지 픽셀별로 계산
    for (j = 1; j < var.NJ; j++) {
      for (i = 1; i < var.NI; i++) {
        if (map_y[j][i] == -999) continue;
        y1 = map_y[j][i]/COMS_GRID + COMS_SY;
        iy1 = (int)(y1 + 0.49);
        if (iy1 < 0 || iy1 > COMS_NY) continue;

        x1 = map_x[j][i]/COMS_GRID + COMS_SX;
        ix1 = (int)(x1 + 0.49);
        if (ix1 < 0 || ix1 > COMS_NX) continue;

        // 위성값과 지형고도에 따라 색상표 결정
        g1 = g[iy1][ix1];
        if (g1 > -990) {
          for (k = 0; k < num_color_sat; k++) {
            if (g1 < data_sat[k]) {
              gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_sat[k]);
              break;
            }
          }
          if (k >= num_color_sat) {
            gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_sat[num_color_sat-1]);
          }
        }
      }
    }
  }
  else {
    // 7. 이미지 픽셀별로 계산
    for (j = 1; j < var.NJ; j++) {
      y1 = dg*j/zm + yo + dyo;
      iy1 = (int)(y1*grid_zoom + 0.49);
      if (iy1 < 0 || iy1 > COMS_NY) continue;

      for (i = 1; i < var.NI; i++) {
        x1 = dg*i/zm + xo + dxo;
        ix1 = (int)(x1*grid_zoom + 0.49);
        if (ix1 < 0 || ix1 > COMS_NX) continue;

        // 위성값과 지형고도에 따라 색상표 결정
        g1 = g[iy1][ix1];
        if (g1 > -990) {
          for (k = 0; k < num_color_sat; k++) {
            if (g1 < data_sat[k]) {
              gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_sat[k]);
              break;
            }
          }
          if (k >= num_color_sat) {
            gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_sat[num_color_sat-1]);
          }
        }
      }
    }
  }

  free_matrix(g, 0, COMS_NY, 0, COMS_NX);

  return 0;
}

/*******************************************************************************
 *  히마와리 위성 표출 (IR1,WVP)
 *******************************************************************************/
int hima_bin_disp(gdImagePtr im, char *fname)
{
  FILE  *fp;
  gzFile fg;
  struct stat st;
  char   head[32];
  char   buf[1024], tmp[1024];
  float  **g;
  short  com1;
  unsigned char p[(HIMA_NX+1)*2];
  float  cnv[18000], g1;
  float  dg, dgt;
  float  zm = 1.0, xo = 0.0, yo = 0.0, zx, zy, x1, y1, v1;
  float  xd, yd, xt, yt, avg, std;
  float  ta, ta_min, ta_max, t1;
  int    offsetx, offsety;
  int    nx, ny, sx, sy;
  int    YY, MM, DD, HH, MI;
  int    H1, H2, S, L, R, G, B;
  int    ok = 0, seq, code, chn, chn2, c1;
  int    i, j, k, n, i1, j1, ix1, iy1, ix2, iy2;
  float  dxo = HIMA_SX*HIMA_GRID - var.SX;
  float  dyo = HIMA_SY*HIMA_GRID - var.SY;
  float  grid_zoom = var.grid/HIMA_GRID;
  float  missing = -999;

  // 1. 변환표 파일 읽기
  if (!strcmp(var.sat,"ir1"))
    chn = 13;  // IR1
  else
    chn = 8;  // WVP
  chn2 = chn*2 - 1;

  if ((fp = fopen(HIMA_CONV_FILE,"r")) == NULL) return -1;
  while (fgets(buf, 1024, fp) != NULL) {
    if (buf[0] == '#') continue;
    getword(tmp, buf, ',');  n = atoi(tmp);
    for (i = 0; i < 32; i++) {
      getword(tmp, buf, ',');
      if (chn2 == i) {
        cnv[n] = atof(tmp) - 273.15;
        break;
      }
    }
  }
  fclose(fp);

  // 3. 자료 읽기
  if ((fg = gzopen(fname,"rb")) == NULL) return -1;
  g = matrix(0, HIMA_NY, 0, HIMA_NX);
  gzbuffer(fg, 64*1024);
  for (j = 0; j <= HIMA_NY; j++) {
    j1 = HIMA_NY - j;
    n = gzread(fg, p, (HIMA_NX+1)*2);
    for (i = 0; i <= HIMA_NX; i++) {
      com1 = p[i*2+1]*256 + p[i*2];
      c1 = com1;

      if (c1 >= 0 && c1 < 18000) g[j1][i] = cnv[c1];
      else g[j1][i] = missing;
    }
  }
  gzclose(fg);

  // 4. 최대,최소값 기반으로 범위 설정
  if (!strcmp(var.sat,"ir1")) {
    ta_min = 190; ta_max = 320;
  }
  else {
    ta_min = 200; ta_max = 275;
  }
  ta_max -= 273.15;
  ta_min -= 273.15;

  make_wgtstrch(g, HIMA_NY, HIMA_NX, (int)ta_min, (int)ta_max, missing);

  ta_max = 249;
  ta_min = 0;

  // 5. 색상표 설정 (흑백 or 칼라)
  for (num_color_sat = 0, ta = ta_min; ta <= ta_max; ta += 1, num_color_sat++) {
    data_sat[num_color_sat] = ta;

    L = (int)(240*(ta_max - ta)/(ta_max - ta_min) + 16);
    if (L < 0) L = 0;
    if (L > 254) L = 254;

    color_sat[num_color_sat] = gdImageColorAllocate(im, L, L, L);
  }

  // 6. 확대시, 중심위치와 확대비율 계산
  if (var.zoom_x[0] != '0') {
    for (i = 0; i < 7; i++, zm *= 1.5) {
      zx = var.zoom_x[i]-'0';
      zy = var.zoom_y[i]-'0';
      if (zx == 0 || zy == 0) break;

      xo += ((float)(var.NX)/24.0*(zx-1)/zm);
      yo += ((float)(var.NY)/24.0*(zy-1)/zm);
    }
  }
  dg = (float)(var.NX)/(float)(var.NI);

  if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD")) {
    // 7. 이미지 픽셀별로 계산
    for (j = 1; j < var.NJ; j++) {
      for (i = 1; i < var.NI; i++) {
        if (map_y[j][i] == -999) continue;
        y1 = map_y[j][i]/HIMA_GRID + HIMA_SY;
        iy1 = (int)(y1 + 0.49);
        if (iy1 < 0 || iy1 > HIMA_NY) continue;

        x1 = map_x[j][i]/HIMA_GRID + HIMA_SX;
        ix1 = (int)(x1 + 0.49);
        if (ix1 < 0 || ix1 > HIMA_NX) continue;

        // 위성값과 지형고도에 따라 색상표 결정
        g1 = g[iy1][ix1];
        if (g1 > -990) {
          for (k = 0; k < num_color_sat; k++) {
            if (g1 < data_sat[k]) {
              gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_sat[k]);
              break;
            }
          }
          if (k >= num_color_sat) {
            gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_sat[num_color_sat-1]);
          }
        }
      }
    }
  }
  else {
    // 7. 이미지 픽셀별로 계산
    for (j = 1; j < var.NJ; j++) {
      y1 = dg*j/zm + yo + dyo;
      iy1 = (int)(y1*grid_zoom + 0.49);
      if (iy1 < 0 || iy1 > HIMA_NY) continue;

      for (i = 1; i < var.NI; i++) {
        x1 = dg*i/zm + xo + dxo;
        ix1 = (int)(x1*grid_zoom + 0.49);
        if (ix1 < 0 || ix1 > HIMA_NX) continue;

        // 위성값과 지형고도에 따라 색상표 결정
        g1 = g[iy1][ix1];
        if (g1 > -990) {
          for (k = 0; k < num_color_sat; k++) {
            if (g1 < data_sat[k]) {
              gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_sat[k]);
              break;
            }
          }
          if (k >= num_color_sat) {
            gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_sat[num_color_sat-1]);
          }
        }
      }
    }
  }

  free_matrix(g, 0, HIMA_NY, 0, HIMA_NX);

  return 0;
}

/*******************************************************************************
 *  천리안2 자료 가져오기
 *******************************************************************************/
int gk2a_get(
  gdImagePtr im,
  int  seq,     // 시간(KST)
  char *band,   // 채널&파장
  char *scn,    // 자료영역
  struct GRID_MAP gm2,  // 결과격자영역
  float **g,     // 결과: 격자값
  float missing  // 자료없음값
)
{
  FILE    *fp1;
  int     ncid;                       /* NetCDF ID */
  int     ndims;                      /* number of dimensions */
  int     nvars;                      /* number of variables */
  int     ngatts;                     /* number of global attributes */
  int     unlimdimid;                 /* unlimited dimension ID */
  char    dim_name[NC_MAX_NAME+1];    /* dimension name */
  size_t  recs, x_recs, y_recs, h_recs, rec1;   /* number of records */
  int     status;                     /* error status */
  int     var_id;                     /* variable ID */
  char    var_name[NC_MAX_NAME+1];    /* variable name */
  nc_type var_type;                   /* variable type */
  int     var_ndims;                  /* number of dims */
  int     var_dims[NC_MAX_VAR_DIMS];  /* variables shape */
  int     var_natts;                  /* number of attributes */
  int     att_id;                     /* attribute ID */
  char    att_name[NC_MAX_NAME+1];    /* attribute name */
  nc_type att_type;                   /* attribute type */
  size_t  att_len;                    /* attribute length */
  char    *cp;                        /* char pointer */
  short   *sp;                        /* short pointer */
  int     *ip;                        /* int pointer */
  float   *p2;                        /* float pointer */
  double  *dp;                        /* double pointer */
  unsigned short  *up1, valid_min, valid_max;
  char   fname[120];
  double sx, sy;
  struct GRID_MAP gm1;
  float  scale_factor, offset;
  float  rate, xo, yo;
  float  cnv[20000];
  int    num_cnv;
  int    x1, y1, nskip, v1;
  int    code, i, j, k, n, i1, j1;

  // 1. 파일 존재여부 확인
  if (gk2a_file(seq, band, scn, fname) < 0) return -10;
  //printf("%s\n", fname);

  // 2. nc파일 열기
  status = nc_open(fname, 0, &ncid);
  if (status != NC_NOERR) return -11;

  // 3. 자료 정보 읽기
  if (!strcmp(band, "LST") || !strcmp(band, "SST") || !strcmp(band, "CTT") || !strcmp(band, "CTH")) strcpy(var_name, band);
  else strcpy(var_name, "image_pixel_values");
  status = nc_inq_varid(ncid, var_name, &var_id);
  status = nc_inq_varndims(ncid, var_id, &ndims);
  status = nc_inq_vardimid(ncid, var_id, var_dims);

  for (recs = 1, i = 0; i < ndims; i++) {
    status = nc_inq_dim(ncid, var_dims[i], dim_name, &rec1);
    recs *= rec1;

    if (i == 0)
      gm1.ny = rec1 - 1;
    else if (i == 1)
      gm1.nx = rec1 - 1;
  }
  if (!strcmp(band,"vi006")) gm1.grid = 0.5;       // 500m
  else if (!strncmp(band,"vi",2)) gm1.grid = 1.0;  // 1km
  else gm1.grid = 2.0;   // 2km

  //status = nc_inq_att(ncid, NC_GLOBAL, "upper_left_easting", &att_type, &att_len);
  //if (att_type == NC_FLOAT) printf("---1---\n");
  //if (att_type == NC_DOUBLE) printf("---2---\n");
  if (!strcmp(band, "LST") || !strcmp(band, "SST") || !strcmp(band, "CTT") || !strcmp(band, "CTH")) {
    status = nc_inq_varid(ncid, "gk2a_imager_projection", &var_id);
    status = nc_get_att_double(ncid, var_id, "upper_left_easting", &sx);
    status = nc_get_att_double(ncid, var_id, "lower_left_northing", &sy);

    status = nc_inq_varid(ncid, var_name, &var_id);
    status = nc_get_att_float(ncid, var_id, "add_offset", &offset);
    status = nc_get_att_float(ncid, var_id, "scale_factor", &scale_factor);
    status = nc_get_att_ushort(ncid, var_id, "valid_min", &valid_min);
    status = nc_get_att_ushort(ncid, var_id, "valid_max", &valid_max);

    // 4. 위성관측자료 읽고, 파일 닫음
    up1 = (unsigned short *) malloc(recs * sizeof(unsigned short));
    status = nc_get_var_ushort(ncid, var_id, up1);
    if(status != NC_NOERR) {
      status = nc_close(ncid);
      return -12;
    }
    status = nc_close(ncid);
  }
  else {
    status = nc_get_att_double(ncid, NC_GLOBAL, "upper_left_easting", &sx);
    status = nc_get_att_double(ncid, NC_GLOBAL, "lower_left_northing", &sy);

    // 4. 위성관측자료 읽고, 파일 닫음
    up1 = (short *) malloc(recs * sizeof(unsigned short));
    status = nc_get_var_ushort(ncid, var_id, up1);
    if (status != NC_NOERR) {
      status = nc_close(ncid);
      return -12;
    }
    status = nc_close(ncid);

    // 5. 변환테이블 읽음
    num_cnv = gk2a_conv_table(band, cnv);
  }

  gm1.sx = -(sx*0.001)/gm1.grid;    // m -> km
  gm1.sy = -(sy*0.001)/gm1.grid;
  //printf("%f %f %f %f\n", sx, sy, gm1.sx, gm1.sy);

/*
  // 6. 초기화
  for (j = 0; j <= gm2.ny; j++) {
    for (i = 0; i <= gm2.nx; i++) {
      g[j][i] = missing;
    }
  }
*/

  // 7. gm1영역 자료 -> gm2영역 자료
  // 7.1. 격자간격이 같은 경우
  if (fabs(gm1.grid - gm2.grid) < 0.0001) {
    for (j = 0; j <= gm2.ny; j++) {
      j1 = (int)(j + gm1.sy - gm2.sy + 0.5);
      if (j1 < 0 || j1 > gm1.ny) continue;

      for (i = 0; i <= gm2.nx; i++) {
        i1 = (int)(i + gm1.sx - gm2.sx + 0.5);
        if (i1 < 0 || i1 > gm1.nx) continue;

        k = (gm1.nx+1)*(gm1.ny-j1) + i1;

        if (!strcmp(band, "LST") || !strcmp(band, "SST") || !strcmp(band, "CTT")) {
          if (g[j][i] == missing) {
            if (up1[k] >= valid_min && up1[k] <= valid_max) {
              g[j][i] = offset + up1[k]*scale_factor - 273.15;
            }
          }
        }
        else if (!strcmp(band, "CTH")) {
          if (g[j][i] == missing) {
            if (up1[k] >= valid_min && up1[k] <= valid_max) {
              g[j][i] = offset + up1[k]*scale_factor;
            }
          }
        }
        else {
          n = up1[k]&0x3fff;
          if (n >= 0 && n < num_cnv) g[j][i] = cnv[n];
          else {
            g[j][i] = missing;
          }
        }
      }
    }
  }
  // 7.2. 격자간격이 다른 경우
  else {
    rate = gm2.grid/gm1.grid;
    yo = gm1.sy - gm2.sy*rate;
    xo = gm1.sx - gm2.sx*rate;

    for (j = 0; j <= gm2.ny; j++) {
      j1 = (int)(j*rate + yo + 0.5);
      if (j1 < 0 || j1 > gm1.ny) continue;

      for (i = 0; i <= gm2.nx; i++) {
        i1 = (int)(i*rate + xo + 0.5);
        if (i1 < 0 || i1 > gm1.nx) continue;

        k = (gm1.nx+1)*(gm1.ny-j1) + i1;
        if (!strcmp(band, "LST") || !strcmp(band, "SST") || !strcmp(band, "CTT")) {
          if (g[j][i] == missing) {
            if (up1[k] >= valid_min && up1[k] <= valid_max) {
              g[j][i] = offset + up1[k]*scale_factor - 273.15;
            }
          }
        }
        else if (!strcmp(band, "CTH")) {
          if (g[j][i] == missing) {
            if (up1[k] >= valid_min && up1[k] <= valid_max) {
              g[j][i] = offset + up1[k]*scale_factor;
            }
          }
        }
        else {
          n = up1[k]&0x3fff;
          if (n >= 0 && n < num_cnv) g[j][i] = cnv[n];
          else {
            g[j][i] = missing;
          }
        }
      }
    }
  }

  // 8. 배열 해제
  free((char*) (up1));

  return 0;
}

/*=============================================================================*
 *  천리안2 위성자료 변환표 읽기
 *=============================================================================*/
int gk2a_conv_table(
  char *band,   // 채널&파장
  float cnv[]   // 변환테이블
)
{
  FILE  *fp;
  char  fname[120];
  int   i, j, k, n;

  // 1. 변환테이블 파일 읽음
  if (strstr(band,"vi") || strstr(band,"nr"))
    sprintf(fname, "%s/%s_con_alb.txt", GK2A_CONV_DIR, band);
  else
    sprintf(fname, "%s/%s_con_bt.txt", GK2A_CONV_DIR, band);
  if ((fp = fopen(fname,"r")) == NULL) return -1;

  n = 0;
  while (fscanf(fp, "%f", &cnv[n]) != EOF) {
    n++;
  }
  fclose(fp);

  // 2. 가시가 아닌 경우는 섭씨온도로 변환
  if (!strstr(band,"vi") && !strstr(band,"nr")) {
    for (i = 0; i < n; i++) {
      if (cnv[i] > 0) cnv[i] -= 273.15;
    }
  }
  else {
    for (i = 0; i < n; i++) {
      if (cnv[i] > 0) cnv[i] *= 100.;
    }
  }

  return n;
}

/*=============================================================================*
 *  천리안2 위성자료 존재 여부 확인 (파일명은 UTC)
 *=============================================================================*/
int gk2a_file(
  int   seq,    // 시간(KST)
  char *band,   // 채널&파장
  char *scn,    // 자료영역
  char *fname
)
{
  struct stat st;
  char   dir_code[8], dir1[60];
  int    grid;
  int    YY, MM, DD, HH, MI;
  int    code;

  // 1. 시간(KST->UTC)
  seq2time(seq-9*60, &YY, &MM, &DD, &HH, &MI, 'm', 'n');

  // 2. BAND별 격자크기
  if (!strcmp(band,"vi006")) grid = 5;        // 500m
  else if (!strncmp(band,"vi",2)) grid = 10;  // 1km
  else grid = 20;   // 2km

  // 3. 디렉토리
  if      (!strcmp(scn,"ea"))  strcpy(dir_code,"EA");   // 동아시아
  else if (!strcmp(scn,"ela")) strcpy(dir_code,"ELA");  // 확장 국지
  else if (!strcmp(scn,"enh")) strcpy(dir_code,"ENH");  // 확장 북반구
  else if (!strcmp(scn,"fd"))  strcpy(dir_code,"FD");   // 전구
  else if (!strcmp(scn,"ko"))  strcpy(dir_code,"KO");   // 한반도
  else if (!strcmp(scn,"la"))  strcpy(dir_code,"LA");   // 국지
  else if (!strcmp(scn,"nko")) strcpy(dir_code,"NKO");  // 북한
  else if (!strcmp(scn,"sko")) strcpy(dir_code,"SKO");  // 남한
  else if (!strcmp(scn,"tp"))  strcpy(dir_code,"TP");   // 태풍

  if (!strcmp(band, "LST")) {
    sprintf(dir1, "%s/LST/EA/%04d%02d/%02d/%02d", GK2A_L2_DIR, YY, MM, DD, HH);
  }
  else if (!strcmp(band, "SST")) {
    sprintf(dir1, "%s/SST/EA/%04d%02d/%02d/%02d", GK2A_L2_DIR, YY, MM, DD, HH);
  }
  else if (!strcmp(band, "CTT") || !strcmp(band, "CTH")) {
    sprintf(dir1, "%s/CTPS/EA/%04d%02d/%02d/%02d", GK2A_L2_DIR, YY, MM, DD, HH);
  }
  else sprintf(dir1, "%s/%s/%04d%02d/%02d/%02d", GK2A_L1B_DIR, dir_code, YY, MM, DD, HH);

  // 4. 파일명
  if (!strcmp(band, "LST")) {
    sprintf(fname, "%s/gk2a_ami_le2_lst_%s%03dlc_%04d%02d%02d%02d%02d.nc",
          dir1, scn, grid, YY, MM, DD, HH, MI);
  }
  else if (!strcmp(band, "SST")) {
    sprintf(fname, "%s/gk2a_ami_le2_sst_%s%03dlc_%04d%02d%02d%02d%02d.nc",
          dir1, scn, grid, YY, MM, DD, HH, MI);
  }
  else if (!strcmp(band, "CTT") || !strcmp(band, "CTH")) {
    sprintf(fname, "%s/gk2a_ami_le2_ctps_%s%03dlc_%04d%02d%02d%02d%02d.nc",
          dir1, scn, grid, YY, MM, DD, HH, MI);
  }
  else sprintf(fname, "%s/gk2a_ami_le1b_%s_%s%03dlc_%04d%02d%02d%02d%02d.nc",
          dir1, band, scn, grid, YY, MM, DD, HH, MI);
  code = stat(fname, &st);
  //printf("%s (%d)\n", fname, code);

  // 5. 존재여부
  if (code < 0 || st.st_size <= 100) return -1;
  return 0;
}

/*=============================================================================*
 *  채널 순서 -> 밴드&파장
 *=============================================================================*/
int gk2a_chn2band(
  int  chn,     // 채널순서
  char *band    // 천리안2 밴드명
)
{
  switch (chn) {
    case 1:  strcpy(band,"vi004");  break;
    case 2:  strcpy(band,"vi005");  break;
    case 3:  strcpy(band,"vi006");  break;
    case 4:  strcpy(band,"vi008");  break;
    case 5:  strcpy(band,"nr013");  break;
    case 6:  strcpy(band,"nr016");  break;
    case 7:  strcpy(band,"sw038");  break;
    case 8:  strcpy(band,"wv063");  break;
    case 9:  strcpy(band,"wv069");  break;
    case 10:  strcpy(band,"wv073");  break;
    case 11:  strcpy(band,"ir087");  break;
    case 12:  strcpy(band,"ir096");  break;
    case 13:  strcpy(band,"ir105");  break;
    case 14:  strcpy(band,"ir112");  break;
    case 15:  strcpy(band,"ir123");  break;
    case 16:  strcpy(band,"ir133");  break;
  }
  return 0;
}

/*******************************************************************************
 *  천리안2 위성 표출 (IR1,WVP)
 *******************************************************************************/
int gk2a_bin_disp(gdImagePtr im, int seq)
{
  FILE  *fp;
  gzFile fg;
  struct stat st;
  struct GRID_MAP gm2;
  char   fname[120];
  char   band[8];
  char   buf[1024], tmp[1024];
  float  **g, g1;
  float  missing = -999;
  float  dg, dgt;
  float  zm = 1.0, xo = 0.0, yo = 0.0, zx, zy, x1, y1, v1;
  float  xd, yd, xt, yt, avg, std;
  float  ta, ta_min, ta_max, t1;
  int    offsetx, offsety;
  int    YY, MM, DD, HH, MI;
  int    H1, H2, S, L, R, G, B;
  int    ok = 0, code, c1;
  int    i, j, k, n, i1, j1, ix1, iy1, ix2, iy2;

  char   txt[256], txt_utf[256];
  int    brect[8];
  int    color;
  double font_size = 8.0;

  // 1. 자료 읽기
  gm2.grid = 2.0;
  gm2.nx = (int)(var.NX/gm2.grid);
  gm2.ny = (int)(var.NY/gm2.grid);
  gm2.sx = var.SX/gm2.grid;
  gm2.sy = var.SY/gm2.grid;

  g = matrix(0, gm2.ny, 0, gm2.nx);
  // 2. 초기화
  for (j = 0; j <= gm2.ny; j++) {
    for (i = 0; i <= gm2.nx; i++) {
      g[j][i] = missing;
    }
  }

  if (!strcmp(var.sat,"ir1") || !strcmp(var.sat,"ir1_enhc"))
    strcpy(band, "ir105");
  else if (!strcmp(var.sat,"wv063") || !strcmp(var.sat,"wv063_enhc"))
    strcpy(band, "wv063");
  else if (!strcmp(var.sat,"wv069") || !strcmp(var.sat,"wv069_enhc"))
    strcpy(band, "wv069");
  else if (!strcmp(var.sat,"wv073") || !strcmp(var.sat,"wv073_enhc"))
    strcpy(band, "wv073");
  else
    strcpy(band, "wv063");

  if (!strcmp(var.sat,"lst") || !strcmp(var.sat,"sst")) {
    code = gk2a_get(im, seq, "LST", "ea", gm2, g, missing);
    code = gk2a_get(im, seq, "SST", "ea", gm2, g, missing);
  }
  else if (!strcmp(var.sat,"ctt")) {
    code = gk2a_get(im, seq, "CTT", "ea", gm2, g, missing);
  }
  else if (!strcmp(var.sat,"cth")) {
    code = gk2a_get(im, seq, "CTH", "ea", gm2, g, missing);
  }
  else if (!strcmp(var.map,"H2") || !strcmp(var.map,"H3") || !strcmp(var.map,"H4") || !strcmp(var.map,"H5"))
    code = gk2a_get(im, seq, band, "ea", gm2, g, missing);
  else if (!strcmp(var.map,"EA_CHT") || !strcmp(var.map,"TP") || !strcmp(var.map,"NHEM") || !strcmp(var.map, "WORLD"))
    code = gk2a_get(im, seq, band, "tp", gm2, g, missing);
  else
    code = gk2a_get(im, seq, band, "ela", gm2, g, missing);

  if (code < 0) return -1;

  // 5. 최대,최소값 기반으로 범위 설정
  if (!strcmp(var.sat,"ir1")) {
    ta_min = 190; ta_max = 320;
  }
  else if (!strcmp(var.sat,"wv063")) {
    ta_min = 200; ta_max = 275;
  }
  else if (!strcmp(var.sat,"wv069")) {
    ta_min = 200; ta_max = 275;
  }
  else if (!strcmp(var.sat,"wv073")) {
    ta_min = 200; ta_max = 275;
  }
  ta_max -= 273.15;
  ta_min -= 273.15;

  if (strcmp(var.sat,"lst") && strcmp(var.sat,"sst") && strcmp(var.sat,"ctt") && strcmp(var.sat,"cth") && !strstr(var.sat,"enhc")) make_wgtstrch(g, gm2.ny, gm2.nx, (int)ta_min, (int)ta_max, missing);

  ta_max = 249;
  ta_min = 0;

  // 6. 색상표 설정 (흑백 or 칼라)
  if (!strcmp(var.sat,"lst") || !strcmp(var.sat,"sst") || !strcmp(var.sat,"ctt") || !strcmp(var.sat,"cth")) {
    if (!strcmp(var.sat,"lst"))      sprintf(fname, "%s/color_sst.rgb", COLOR_SET_DIR);
    else if (!strcmp(var.sat,"sst")) sprintf(fname, "%s/color_sst.rgb", COLOR_SET_DIR);
    else if (!strcmp(var.sat,"ctt")) sprintf(fname, "%s/color_sat_CTT.rgb", COLOR_SET_DIR);
    else if (!strcmp(var.sat,"cth")) sprintf(fname, "%s/color_sat_CTH.rgb", COLOR_SET_DIR);
    seq2time(seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
    if ((fp = fopen(fname,"r")) == NULL) return -1;
    num_color_sat = 0;
    while (fscanf(fp, "%d %d %d %f\n", &R, &G, &B, &v1) != EOF) {
      color_sat[num_color_sat] = gdImageColorAllocate(im, R, G, B);
      if (MM >=5 && MM <= 10 && (!strcmp(var.sat,"lst") || !strcmp(var.sat,"sst"))) data_sat[num_color_sat] = v1 + 10;
      else data_sat[num_color_sat] = v1;
      num_color_sat++;
    }
    fclose(fp);
  }
  else if (strstr(var.sat,"enhc")) {
    if (!strcmp(var.sat,"wv063_enhc") || !strcmp(var.sat,"wv069_enhc")) sprintf(fname, "%s/color_sat_enhc_wv63.rgb", COLOR_SET_DIR);
    else if (!strcmp(var.sat,"wv073_enhc")) sprintf(fname, "%s/color_sat_enhc_wv73.rgb", COLOR_SET_DIR);
    else if (!strcmp(var.sat,"ir1_enhc"))    sprintf(fname, "%s/color_sat_enhc_ir.rgb", COLOR_SET_DIR);
    seq2time(seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
    if ((fp = fopen(fname,"r")) == NULL) return -1;
    num_color_sat = 0;
    while (fscanf(fp, "%d %d %d %f\n", &R, &G, &B, &v1) != EOF) {
      color_sat[num_color_sat] = gdImageColorAllocate(im, R, G, B);
      data_sat[num_color_sat] = v1;
      num_color_sat++;
    }
    fclose(fp);
  }
  else {
    for (num_color_sat = 0, ta = ta_min; ta <= ta_max; ta += 1, num_color_sat++) {
      data_sat[num_color_sat] = ta;

      //L = (int)(240*(ta_max - ta)/(ta_max - ta_min) + 16);
      L = (int)(255*(ta_max - ta)/(ta_max - ta_min));
      if (L < 0) L = 0;
      if (L > 254) L = 254;

      //color_sat1[num_color_sat] = gdImageColorAllocate(im, L, L, L);
      color_sat[num_color_sat] = gdImageColorAllocate(im, L, L, L);
    }
  }

  // 7. 확대시, 중심위치와 확대비율 계산
  if (var.zoom_x[0] != '0') {
    for (i = 0; i < 7; i++, zm *= 1.5) {
      zx = var.zoom_x[i]-'0';
      zy = var.zoom_y[i]-'0';
      if (zx == 0 || zy == 0) break;

      //xo += ((float)(gm2.nx)/8.0*(zx-1)/zm);
      //yo += ((float)(gm2.ny)/8.0*(zy-1)/zm);
      xo += ((float)(gm2.nx)/24.0*(zx-1)/zm);
      yo += ((float)(gm2.ny)/24.0*(zy-1)/zm);
    }
  }
  dg = (float)(gm2.nx)/(float)(var.NI)/zm;
  dgt = (float)(var.NX)/(float)(var.NI)/zm;
  xt = xo*gm2.grid;
  yt = yo*gm2.grid;

  if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD")) {
    // 8. 이미지 픽셀별로 계산
    for (j = 1; j < var.NJ; j++) {
      for (i = 1; i < var.NI; i++) {
        if (map_y[j][i] == -999) continue;
        y1 = map_y[j][i]/gm2.grid + gm2.sy;
        iy1 = (int)(y1 + 0.45);
        if (iy1 < 0 || iy1 >= gm2.ny) continue;

        x1 = map_x[j][i]/gm2.grid + gm2.sx;
        ix1 = (int)(x1 + 0.97);
        if (ix1 < 0 || ix1 >= gm2.nx) continue;


        // 위성값과 지형고도에 따라 색상표 결정
        g1 = g[iy1][ix1];
        if (g1 > -990) {
          for (k = 0; k < num_color_sat; k++) {
            if (g1 < data_sat[k]) {
              gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_sat[k]);
              break;
            }
          }
          if (k >= num_color_sat) {
            gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_sat[num_color_sat-1]);
          }
        }
      }
    }
  }
  else {
    // 8. 이미지 픽셀별로 계산
    for (j = 1; j < var.NJ; j++) {
      y1 = dg*j + yo;
      iy1 = (int)(y1 + 0.45);
      if (iy1 < 0 || iy1 >= gm2.ny) continue;
      iy2 = (int)(dgt*j + yt + 0.49);    // 지형고도 위치

      for (i = 1; i < var.NI; i++) {
        x1 = dg*i + xo;
        ix1 = (int)(x1 + 0.97);
        if (ix1 < 0 || ix1 >= gm2.nx) continue;
        ix2 = (int)(dgt*i + xt + 0.49);

        /*
        // 해당 위치의 지형고도 
        if (iy2 < 0 || iy2 > var.NY || ix2 < 0 || ix2 > var.NX)
          t1 = -1;
        else {
          if (var.topo_ok > 0)
            t1 = (float)topo[iy2][ix2];
          else
            t1 = -1;
        }
        */

        // 위성값과 지형고도에 따라 색상표 결정
        g1 = g[iy1][ix1];
        if (g1 > -990) {
          for (k = 0; k < num_color_sat; k++) {
            if (g1 < data_sat[k]) {
              //if (t1 > 0)
              //  gdImageSetPixel(im, i, var.GJ-j, color_sat1[k]);
              //else
                gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_sat[k]);
              break;
            }
          }
          if (k >= num_color_sat) {
            //if (t1 > 0)
            //  gdImageSetPixel(im, i, var.GJ-j, color_sat1[num_color_sat-1]);
            //else
              gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_sat[num_color_sat-1]);
          }
        }
      }
    }
  }

  free_matrix(g, 0, gm2.ny, 0, gm2.nx);
  return 0;
}

/*******************************************************************************
 *  천리안2 위성 RGB 합성 표출(bin 파일)
 *******************************************************************************/
int gk2a_rgb_bin_disp(gdImagePtr im, int seq)
{
  FILE  *fp;
  struct stat st;
  struct GRID_MAP gm1;
  char   fname[256], dir_code[8], dir1[60], scn[8];
  unsigned char  *rgb;
  float  missing = -999;
  float  dg, dgt, dgx, dgy;
  float  zm = 1.0, xo = 0.0, yo = 0.0, zx, zy, x1, y1, v1, dxo, dyo, rate;
  float  xd, yd, xt, yt, avg, std;
  int    offsetx, offsety;
  int    YY, MM, DD, HH, MI;
  int    ok = 0, code, c1;
  int    i, j, k, n, i1, j1, ix1, iy1, ix2, iy2;
  int    ix, iy;
  int    R, G, B;
  int    sx, sy, nx, ny, grid;
  size_t recs;

  // 1. 시간(KST->UTC)
  seq2time(seq-9*60, &YY, &MM, &DD, &HH, &MI, 'm', 'n');

  // 2. 위성 영역 설정
  if (!strcmp(var.map,"H2") || !strcmp(var.map,"H3") || !strcmp(var.map,"H4") || !strcmp(var.map,"H5")) {
    nx = 3000; ny = 2600; sx = 2999; sy = 2599; strcpy(dir_code,"EA"); strcpy(scn,"ea");
  }
  else if (!strcmp(var.map,"EA_CHT") || !strcmp(var.map,"TP") || !strcmp(var.map,"NHEM") || !strcmp(var.map, "WORLD")) {
    nx = 5750; ny = 4250; sx = 4349; sy = 5539; strcpy(dir_code,"TP"); strcpy(scn,"tp");
  }
  else {
    nx = 1950; ny = 1550; sx = 2149; sy = 1539; strcpy(dir_code,"ELA"); strcpy(scn,"ela");
  }

  // 3. 파일명
  grid = 20;
  sprintf(dir1, "%s/%s/%04d%02d/%02d/%02d", GK2A_L1B_DIR, dir_code, YY, MM, DD, HH);
  if (!strcmp(var.sat,"rgb_daynight")) {
    sprintf(fname, "%s/gk2a_ami_le1b_rgb-daynight_%s%03dlc_%04d%02d%02d%02d%02d.bin",
            dir1, scn, grid, YY, MM, DD, HH, MI);
  }
  else if (!strcmp(var.sat,"rgb_dust")) {
    sprintf(fname, "%s/gk2a_ami_le1b_rgb-dust_%s%03dlc_%04d%02d%02d%02d%02d.bin",
            dir1, scn, grid, YY, MM, DD, HH, MI);
  }
  else if (!strcmp(var.sat,"rgb_natural")) {
    sprintf(fname, "%s/gk2a_ami_le1b_rgb-natural_%s%03dlc_%04d%02d%02d%02d%02d.bin",
            dir1, scn, grid, YY, MM, DD, HH, MI);
  }
  else if (!strcmp(var.sat,"rgb_wv")) {
    sprintf(fname, "%s/gk2a_ami_le1b_rgb-wv_%s%03dlc_%04d%02d%02d%02d%02d.bin",
            dir1, scn, grid, YY, MM, DD, HH, MI);
  }
  code = stat(fname, &st);
  if (code < 0 || st.st_size <= 100) return -1;

  // 5. 변수 설정
  gm1.grid = 2.0;
  gm1.nx = nx - 1;
  gm1.ny = ny - 1;
  gm1.sx = sx;
  gm1.sy = sy;

  recs = nx * ny * 4;
  rgb = (unsigned char *) malloc(recs * sizeof(unsigned char));

  // 6. 파일 읽기
  if ((fp = fopen(fname,"rb")) != NULL) {
    fread(rgb, sizeof(unsigned char), recs, fp);
    fclose(fp);
  }

  // 7. 확대시, 중심위치와 확대비율 계산
  if (var.zoom_x[0] != '0') {
    for (i = 0; i < 7; i++, zm *= 1.5) {
      zx = var.zoom_x[i]-'0';
      zy = var.zoom_y[i]-'0';
      if (zx == 0 || zy == 0) break;

      xo += ((float)(var.NX)/24.0*(zx-1)/zm);
      yo += ((float)(var.NY)/24.0*(zy-1)/zm);
    }
  }
  dg = (float)(var.NX)/(float)(var.NI)/zm;
  rate = var.grid/gm1.grid;
  dxo = gm1.sx - var.SX;
  dyo = gm1.sy - var.SY;

  if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD")) {
    // 8. 이미지 픽셀별로 계산
    for (j = 1; j < var.NJ; j++) {
      for (i = 1; i < var.NI; i++) {
        if (map_y[j][i] == -999) continue;
        y1 = map_y[j][i] + gm1.sy;
        iy1 = (int)(y1*rate + 0.49);
        if (iy1 < 0 || iy1 >= gm1.ny) continue;

        ok = 1;
        x1 = map_x[j][i] + gm1.sx;
        ix1 = (int)(x1*rate + 0.49);
        if (ix1 < 0 || ix1 >= gm1.nx) continue;

        // 해당 격자점에 색상 설정
        if (ok) {
          n = (int)(((int)(rgb[((gm1.nx+1)*(gm1.ny-iy1)+ix1)*4+2])*256 + (int)(rgb[((gm1.nx+1)*(gm1.ny-iy1)+ix1)*4+1]))*256 + (int)(rgb[((gm1.nx+1)*(gm1.ny-iy1)+ix1)*4]));
          if ((int)(rgb[((gm1.nx+1)*(gm1.ny-iy1)+ix1)*4+2]) == 255 && (int)(rgb[((gm1.nx+1)*(gm1.ny-iy1)+ix1)*4+1]) == 255 && (int)(rgb[((gm1.nx+1)*(gm1.ny-iy1)+ix1)*4]) == 255) {
            n = (254*256 + 254)*256 + 254;
          }
          gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, n);
        }
      }
    }
  }
  else {
    // 8. 이미지 픽셀별로 계산
    for (j = 1; j < var.NJ; j++) {
      y1 = dg*j + yo + dyo;
      iy1 = (int)(y1*rate + 0.49);
      if (iy1 < 0 || iy1 >= gm1.ny) continue;

      for (i = 1; i < var.NI; i++) {
        ok = 1;
        x1 = dg*i + xo + dxo;
        ix1 = (int)(x1*rate + 0.49);
        if (ix1 < 0 || ix1 >= gm1.nx) continue;

        // 해당 격자점에 색상 설정
        if (ok) {
          n = (int)(((int)(rgb[((gm1.nx+1)*(gm1.ny-iy1)+ix1)*4+2])*256 + (int)(rgb[((gm1.nx+1)*(gm1.ny-iy1)+ix1)*4+1]))*256 + (int)(rgb[((gm1.nx+1)*(gm1.ny-iy1)+ix1)*4]));
          if ((int)(rgb[((gm1.nx+1)*(gm1.ny-iy1)+ix1)*4+2]) == 255 && (int)(rgb[((gm1.nx+1)*(gm1.ny-iy1)+ix1)*4+1]) == 255 && (int)(rgb[((gm1.nx+1)*(gm1.ny-iy1)+ix1)*4]) == 255) {
            n = (254*256 + 254)*256 + 254;
          }
          gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, n);
        }
      }
    }
  }

  free((char*) (rgb));
  return 0;
}

/*******************************************************************************
 *  천리안2 위성 RGB 합성 표출
 *******************************************************************************/
int gk2a_rgb_disp(gdImagePtr im, int seq)
{
/*
  FILE  *fp;
  gzFile fg;
  struct stat st;
  struct GRID_MAP gm2;
  char   tmp[1024], txt[256];
  float  **g, g1;
  float  missing = -999;
  float  dg, dgt;
  float  zm = 1.0, xo = 0.0, yo = 0.0, zx, zy, x1, y1, v1;
  float  xd, yd, xt, yt, avg, std;
  int    offsetx, offsety;
  int    YY, MM, DD, HH, MI;
  int    H1, H2, S, L, R, G, B;
  int    ok = 0, code, c1;
  int    i, j, k, n, i1, j1, ix1, iy1, ix2, iy2;
  float  cnv[4][20000], gv[4];
  int    num_cnv[4];
  int    ix, iy, ci[4];
  int    iR, iG, iB;
  int    rgb_cd, ivs[4];
  short  *gp[4], s1;
  char   band[17][8] = {"vi004","vi005","vi006","vi008","nr013","nr016","","sw038",
           "wv063","wv069","wv073","ir087","ir096","ir105","ir112","ir123","ir133"};
  float  minv[4], maxv[4], maxmin[4];

  // 0. 전처리
  gm2.grid = 2.0;
  gm2.nx = (int)(var.NX/gm2.grid);
  gm2.ny = (int)(var.NY/gm2.grid);
  gm2.sx = var.SX/gm2.grid;
  gm2.sy = var.SY/gm2.grid;

  if (!strcmp(var.sat,"RGB1")) {    // 천연색
    var.ccn[0] = 3;  var.ccn[1] = 2;  var.ccn[2] = 1;  var.ccn[3] = 4;  var.num_ccn = 4;  rgb_cd = 0;
  }
  else if (!strcmp(var.sat,"RGB2")) {   // 자연색
    var.ccn[0] = 6;  var.ccn[1] = 4;  var.ccn[2] = 3;  var.num_ccn = 3;  rgb_cd = 1;
  }
  else if (!strcmp(var.sat,"RGB3")) {   // 수증기
    var.ccn[0] = 14; var.ccn[1] = 9;  var.ccn[2] = 11; var.num_ccn = 3;  rgb_cd = 2;
  }
  else if (!strcmp(var.sat,"rgb_day")) {    // 주간
    var.ccn[0] = 3;  var.ccn[1] = 14; var.ccn[2] = 16; var.num_ccn = 3;  rgb_cd = 3;
    var.gamma[0] = 0.5; var.gamma[1] = 0.6; var.gamma[2] = 0.6; 
  }
  else if (!strcmp(var.sat,"rgb_night")) {  // 야간
    var.ccn[0] = 8;  var.ccn[1] = 14; var.ccn[2] = 16; var.num_ccn = 3;  rgb_cd = 4;
    var.gamma[0] = 0.5; var.gamma[1] = 0.5; var.gamma[2] = 0.5; 
  }

  for (k = 0; k < var.num_ccn; k++) {
    if (var.ccn[k] <= 7)
      ivs[k] = 0;
    else
      ivs[k] = 1;
  }

  // 색상표 설정 (RGB 각각 128단계로 설정)
  for (n = 0, R = 0; R < 256; R += 2) {
    for (G = 0; G < 256; G += 2) {
      for (B = 0; B < 256; B += 2, n++) {
        color_rgb[n] = gdImageColorAllocate(im, R, G, B);
      }
    }
  }
  // 기타 색상표 설정
  color_rgb[2100240] = gdImageColorAllocate(im, 220, 220, 220);   // 배경색1
  color_rgb[2100241] = gdImageColorAllocate(im, 255, 255, 255);   // 배경색2
  color_rgb[2100242] = gdImageColorAllocate(im, 30, 30, 30);      // 지도색
  color_rgb[2100243] = gdImageColorAllocate(im, 12, 28, 236);     // 제목
  color_rgb[2100244] = gdImageColorAllocate(im, 0, 0, 0);         // 검은색
  color_rgb[2100245] = gdImageColorAllocate(im, 240, 240, 240);
  color_rgb[2100246] = gdImageColorAllocate(im, 255, 0, 0);
  color_rgb[2100247] = gdImageColorAllocate(im, 0, 0, 255);
  color_rgb[2100248] = gdImageColorAllocate(im, 160, 160, 160);   // 배경색3
  color_rgb[2100249] = gdImageColorAllocate(im, 90, 90, 90);      // 시군경계
  color_rgb[2100250] = gdImageColorAllocate(im, 80, 80, 80);      // 지형고도
  color_rgb[2100251] = gdImageColorAllocate(im, 255, 0, 172);     // 단면선
  color_rgb[2100252] = gdImageColorAllocate(im, 255, 239, 0);     // 노라색 라인
  color_rgb[2100253] = gdImageColorAllocate(im, 0, 255, 255);     // 하늘색 라인


  // 1. 위성 채널별 자료 가져오기
  for (k = 0; k < var.num_ccn; k++) {
    g = matrix(0, gm2.ny, 0, gm2.nx);

    if (!strcmp(var.map,"H2") || !strcmp(var.map,"H3") || !strcmp(var.map,"H4") || !strcmp(var.map,"H5"))
      code = gk2a_get(im, seq, band[var.ccn[k]-1], "ea", gm2, g, missing);
    else if (!strcmp(var.map,"EA_CHT") || !strcmp(var.map,"TP") || !strcmp(var.map,"NHEM") || !strcmp(var.map, "WORLD"))
      code = gk2a_get(im, seq, band[var.ccn[k]-1], "tp", gm2, g, missing);
    else
      code = gk2a_get(im, seq, band[var.ccn[k]-1], "ela", gm2, g, missing);

    if (code < 0) return -1;

    data[k] = (float *) malloc((gm2.nx+1)*(gm2.ny+1) * sizeof(float));
    n = 0;
    for (j = 0; j <= gm2.ny; j++) {
      for (i = 0; i <= gm2.nx; i++, n++) {
        data[k][n] = g[j][i];
      }
    }

    free_matrix(g, 0, gm2.ny, 0, gm2.nx);
  }

  for (k = 0; k < var.num_ccn; k++) {
    sat_data_range(k, (gm2.nx+1)*(gm2.ny+1), data[k], &minv[k], &maxv[k]);
    maxmin[k] = (float)(maxv[k] - minv[k]);

    //sprintf(txt, "maxmin = %1.f, maxv = %.1f, minv = %.1f", maxmin[k], maxv[k], minv[k]);
    //gdImageString(im, gdFontLarge, 100, k*10+50, txt, color_rgb[2100246]);
  }

  // 2. 확대시, 중심위치와 확대비율 계산
  if (var.zoom_x[0] != '0') {
    for (i = 0; i < 7; i++, zm *= 1.5) {
      zx = var.zoom_x[i]-'0';
      zy = var.zoom_y[i]-'0';
      if (zx == 0 || zy == 0) break;

      //xo += ((float)(gm2.nx)/8.0*(zx-1)/zm);
      //yo += ((float)(gm2.ny)/8.0*(zy-1)/zm);
      xo += ((float)(gm2.nx)/24.0*(zx-1)/zm);
      yo += ((float)(gm2.ny)/24.0*(zy-1)/zm);
    }
  }
  dg = (float)(gm2.nx)/(float)(var.NI)/zm;
  dgt = (float)(var.NX)/(float)(var.NI)/zm;
  xt = xo*gm2.grid;
  yt = yo*gm2.grid;

  // 3. 이미지 픽셀별로 계산
  for (j = 1; j < var.NJ; j++) {
    y1 = dg*j + yo;
    iy1 = (int)(y1 + 0.45);
    if (iy1 < 0 || iy1 >= gm2.ny) continue;

    for (i = 1; i < var.NI; i++) {
      ok = 1;
      x1 = dg*i + xo;
      ix1 = (int)(x1 + 0.97);
      if (ix1 < 0 || ix1 >= gm2.nx) continue;

      // 0~127 로 인덱스된 칼라
      for (k = 0; k < 3; k++) {
        v1 = data[k][(gm2.nx+1)*iy1+ix1];
        if (v1 == missing) {
          ok = 0;
          continue;
        }

        if (maxmin[k] < 2) {
          ci[k] = 0;
        }
        else {
          v1 = (v1 - minv[k])/maxmin[k];
          if (ivs[k]) v1 = 1.0 - v1;
          ci[k] = (int)(128*pow(v1,var.gamma[k]));
          if (ci[k] < 0)
            ci[k] = 0;
          else if (ci[k] > 127)
            ci[k] = 127;
        }
      }

      // 해당 격자점에 색상 설정
      if (ok) {
        n = (ci[0]*128 + ci[1])*128 + ci[2];
        gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_rgb[n]);    
      }
    }
  }
*/
  return 0;
}

/*=============================================================================*
 *  통계적 의미에서 대략적인 관측값의 범위
 *=============================================================================*/
int sat_data_range (
  int   nccn,
  int   nrec,     // data1의 자료수
  float *data1,   // 채널값
  float *minv,    // [결과] 아래범위
  float *maxv     // [결과] 위범위
)
{
  float freq[200];
  float v1;
  int   vis = 0;
  int   i, j, k, n;
  float missing = -999;

  // 0. 초기화
  for (i = 0; i < 200; i++)
    freq[i] = 0;

  // 1. 가시채널들인 경우
  if      (!strcmp(var.sat,"RGB1")) vis = 1;
  else if (!strcmp(var.sat,"RGB2")) vis = 2;
  else if (!strcmp(var.sat,"rgb_day") && nccn == 0) vis = 1;
  else vis = 0;

  // 2. 도수분포 계산
  for (n = 0, i = 0; i < nrec; i += 7) {
    v1 = data1[i];
    if (v1 == missing) continue;

    if (vis) {
      k = (int)v1;    // 0~100% 범위내
      if (k < 0)
        k = 0;
      else if (k >= 100)
        k = 100;
      freq[k] += 1;
    }
    else {
      k = (int)(v1 + 100);    // 영하100도부터
      if (k < 0)
        k = 0;
      else if (k >= 200)
        k = 199;
      freq[k] += 1;
    }
    n++;
  }

  // 3. 누적도수분포 계산
  for (i = 1; i < 200; i++) {
    freq[i] += freq[i-1];
  }

  for (i = 0; i < 200; i++) {
    freq[i] = 100.0*freq[i]/(float)n;
  }

  // 4. 아래 범위 확인
  *minv = 0;
  for (i = 1; i < 199; i++) {
    if (freq[i-1] < 3 && freq[i] >= 3) {
      *minv = i;
      break;
    }
  }
  if (vis == 0) *minv -= 100;

  // 5. 위 범위 확인
  *maxv = 150;
  for (i = 1; i < 199; i++) {
    if (freq[i-1] < 98 && freq[i] >= 98) {
      *maxv = i;
      break;
    }
  }
  if (vis == 0) *maxv -= 100;
  return 0;
}

/*=============================================================================*
 *  구름모의영상 자료 읽기
 *=============================================================================*/
int sat_fcst_disp(gdImagePtr im)
{
  FILE   *fp;
  char   fname[120];
  double dx, dy;
  float  dg = (float)var.NX/(float)(var.NI);
  struct lamc_parameter map;
  int    i, j, k, n, nx, ny, gx, gy, dgx, dgy, i1, j1, iy1, ix1, L;
  float  missing = -999;
  float  lat, lon, lat_min = 999., lat_max = -999., lon_min = 999., lon_max = -999., x1, y1;
  float  zm = 1.0, xo = 0.0, yo = 0.0, zx, zy, rate, dxo, dyo;
  float  ta, ta_min, ta_max, mask, buf1, buf2, di, dj, d1, d2, g, il, jl, v1;
  float  **g1, **g2;
  int    YY, MM, DD, HH, MI;
  int    R, G, B;


  char   txt[256], txt_utf[256];
  int    brect[8];
  int    color;
  double font_size = 10.0;

  // 1. 위.경도 파일 조회
  if (!strcmp(var.model,"UM")) {
    sprintf(fname, "/fct/REF/MAP/gdps_pdarea_latlon.dat");
    nx = 2560; ny = 1920;
  }
  else if (!strcmp(var.model,"KIMG")) {
    sprintf(fname, "/fct/REF/MAP/kimgdps_pdarea_latlon.dat");
    //nx = 2880; ny = 1440;
    nx = 2560; ny = 1920;
  }
  if ((fp = fopen(fname,"r")) == NULL) return -1;

  n = 0;
  x1 = -999.; y1 = -999.; gx = 0; gy = 0;
  while (fscanf(fp, "%f %f %f\n", &lat, &lon, &mask) != EOF) {
    if (y1 != lat) {
      y1 = lat;
      gy++;

      if (lat > lat_max) lat_max = lat;
      if (lat < lat_min) lat_min = lat;
    }

    if (gy <= 1 && x1 != lon) {
      x1 = lon;
      gx++;

      if (lon > lon_max) lon_max = lon;
      if (lon < lon_min) lon_min = lon;
    }
  }
  fclose(fp);

  if (lon_max < 0) lon_max += 360.0;
  if (lon_min < 0) lon_min += 360.0;
  lat_max += 90.0;  lat_min += 90.0;


  // 2. 구름모의 영상 파일 조회
  seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
  if (!strcmp(var.model,"UM")) {
    sprintf(fname, "/C4N2_DATA/NWP/APPM/%04d%02d/%02d/RDTB_gdps_gkompsat2_%04d%02d%02d%02d_f%03d.dat", YY, MM, DD, YY, MM, DD, HH, (var.seq - var.seq_fc - 9*60)/60);
  }
  else if (!strcmp(var.model,"KIMG")) {
    sprintf(fname, "/C4N2_DATA/NWP/APPM/%04d%02d/%02d/RDTB_kimgdps_gkompsat2_%04d%02d%02d%02d_f%03d.dat", YY, MM, DD, YY, MM, DD, HH, (var.seq - var.seq_fc - 9*60)/60);
  }
  if ((fp = fopen(fname,"r")) == NULL) return -1;

  g1 = matrix(0, gy, 0, gx);
  i = 0; j = 0;
  while (fscanf(fp, "%f %f\n", &buf1, &buf2) != EOF) {
    if (strstr(var.sat, "rmwv")) {
      g1[j][i] = buf1;
    }
    else if (strstr(var.sat, "rmir")) {
      g1[j][i] = buf2;
    }
    i++;
    if (i >= gx) {
      i = 0; j++;
    }
  }
  fclose(fp);

  // 3. Histogram Equalization
  if (strstr(var.sat,"rmir")) {
    ta_min = 190; ta_max = 320;
  }
  else if (strstr(var.sat,"rmwv")) {
    ta_min = 200; ta_max = 275;
  }

  if (!strstr(var.sat,"enhc")) {
    make_wgtstrch(g1, gy, gx, (int)ta_min, (int)ta_max, missing);
  }


  // 4. LCC 도법으로 변환
  // 1. 확대시, 중심위치와 확대비율 계산
  for (i = 0; i < 7; i++, zm *= 1.5) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
    xo += (float)(var.NX)/24.0*(zx-1)/zm;
    yo += (float)(var.NY)/24.0*(zy-1)/zm;
  }

  // 2. 지도 투영법
  map.Re    = 6371.00877;
  map.grid  = 1.;
  map.slat1 = 30.0;    map.slat2 = 60.0;
  map.olon  = 126.0;   map.olat  = 38.0;
  rate = var.grid/map.grid;
  map.xo = (float)(var.SX)*(rate) - xo*(rate);
  map.yo = (float)(var.SY)*(rate) - yo*(rate);
  map.first = 0;

  dx = 360.0/nx; dy = 180.0/ny;

  g2 = matrix(0, (int)(var.NJ/map.grid)+1, 0, (int)(var.NI/map.grid)+1);

  dgy = (int)(lat_min / dy); dgx = (int)(lon_min / dx);
  for (j = 0; j <= (int)(var.NJ/map.grid); j++) {
    for (i = 0; i <= (int)(var.NI/map.grid); i++) {

      il = (float)(i*dg/zm); jl = (float)(j*dg/zm);
      if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD")) {
        lon = map_lon[j][i];
        lat = map_lat[j][i];
      }
      else lamcproj_ellp(&lon, &lat, &il, &jl, 1, &map);
      if (lon < 0) lon += 360.0;
      lat += 90.0;

      i1 = (int)(lon / dx);   j1 = (int)(lat / dy);
      di = lon/dx - i1;       dj = lat/dy - j1;

      if ((j1-dgy) < 0 || (j1-dgy) >= gy || (i1-dgx) < 0 || (i1-dgx) >= gx) {
        g2[j][i] = missing;
      }
      else {
        d1 = ( g1[j1-dgy][i1-dgx]*(1.0-di) + g1[j1-dgy][i1-dgx+1]*di );
        d2 = ( g1[j1-dgy+1][i1-dgx]*(1.0-di) + g1[j1-dgy+1][i1-dgx+1]*di );
        g2[j][i] = ( d1*(1.0-dj) + d2*dj );
      }

    }
  }

  // 5. 색상표 설정 (흑백)
  ta_max = 249;
  ta_min = 0;

  if (strstr(var.sat,"enhc")) {
    if (strstr(var.sat,"rmwv")) sprintf(fname, "%s/color_sat_enhc_wv63.rgb", COLOR_SET_DIR);
    else if (strstr(var.sat,"rmir")) sprintf(fname, "%s/color_sat_enhc_ir.rgb", COLOR_SET_DIR);
    if ((fp = fopen(fname,"r")) == NULL) return -1;
    num_color_sat = 0;
    while (fscanf(fp, "%d %d %d %f\n", &R, &G, &B, &v1) != EOF) {
      color_sat[num_color_sat] = gdImageColorAllocate(im, R, G, B);
      data_sat[num_color_sat] = v1 + 273.15;
      num_color_sat++;
    }
    fclose(fp);
  }
  else {
    for (num_color_sat = 0, ta = ta_min; ta <= ta_max; ta += 1, num_color_sat++) {
      data_sat[num_color_sat] = ta;

      L = (int)(240*(ta_max - ta)/(ta_max - ta_min) + 16);
      if (L < 0) L = 0;
      if (L > 254) L = 254;

      //color_sat1[num_color_sat] = gdImageColorAllocate(im, L, L, L);
      color_sat[num_color_sat] = gdImageColorAllocate(im, L, L, L);
    }
  }

  // 6. 이미지 픽셀별로 계산
  for (j = 1; j < var.NJ; j++) {
    iy1 = (int)(j*rate + 0.5);
    if (iy1 < 0 || iy1 >= (int)(var.NJ/map.grid)+1) continue;

    for (i = 1; i < var.NI; i++) {
      ix1 = (int)(i*rate + 0.5);
      if (ix1 < 0 || ix1 >= (int)(var.NI/map.grid)+1) continue;

      // 위성값과 지형고도에 따라 색상표 결정
      g = g2[iy1][ix1];
      if (g > -990) {
        for (k = 0; k < num_color_sat; k++) {
          if (g < data_sat[k]) {
            gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_sat[k]);
            break;
          }
        }
        if (k >= num_color_sat) {
          gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_sat[num_color_sat-1]);
        }
      }
    }
  }

  //sprintf(txt, "fname: %s", fname);
  //for (i = 0; i < 256; i++) txt_utf[i] = 0;
  //euckr2utf(txt, txt_utf);
  //gdImageStringFT(im, &brect[0], color, FONTTTF, font_size, 0.0, 100, 100, txt_utf);

  free_matrix(g1, 0, gy, 0, gx);
  free_matrix(g2, 0, (int)(var.NJ/map.grid)+1, 0, (int)(var.NI/map.grid)+1);

  return 0;
}

/*******************************************************************************
 *  국외 레이더 합성 표출
 *******************************************************************************/
int rdr_oth_disp(gdImagePtr im)
{
  FILE *fp;
  gzFile fg;
  URL_FILE *fr;
  struct lamc_parameter map;
  char   url[120], value[32];
  struct stat st;
  char   fname[120];
  short  **rdr1, r1;
  float  dg = (float)var.NX/(float)(var.NI);
  float  zm = 1.0, xo = 0.0, yo = 0.0, zx, zy, x1, y1, v1, rt;
  float  dxo = H3_SX - var.SX;
  float  dyo = H3_SY - var.SY;
  float  dbz1, data_min, data_max, grid_zoom = 1.0;
  float  data_lvl[120];
  int    color_rdr_area;
  int    nx, ny, ng;
  int    YY, MM, DD, HH, MI;
  int    R, G, B;
  int    ok = 0, seq, code, c1, seq_bk, seq_bk2;
  int    i, j, k, j1, ix, iy, n;

  seq_bk  = time2seq(2017, 1, 1, 0, 0, 'm');
  seq_bk2 = time2seq(2014, 1, 1, 0, 0, 'm');
  var.seq_rdr = -1;

  // 1. 먼저 합성된 파일이 있는지 확인하고 있으면 그것을 먼저 사용함
  ok = 0;
  for (seq = var.seq; seq >= var.seq-30; seq -= 5) {
    seq2time(seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
    if (seq >= seq_bk) {
      sprintf(fname, "%s/%04d%02d/%02d/RDR_CMP_OTH_%04d%02d%02d%02d%02d.bin.gz",
              RDR_CMP_DIR, YY, MM, DD, YY, MM, DD, HH, MI);
    }
    else if (seq >= seq_bk2) {
      sprintf(fname, "%s/%04d%02d/%02d/RDR_CMP_OTH_%04d%02d%02d%02d%02d.bin.gz",
              RDR_CMP_DIR_BK, YY, MM, DD, YY, MM, DD, HH, MI);
    }
    else {
      sprintf(fname, "%s/%04d%02d/%02d/RDR_CMP_PPI_QCD_%04d%02d%02d%02d%02d.bin.gz",
              RDR_CMP_DIR_BK, YY, MM, DD, YY, MM, DD, HH, MI);

      dxo = HB_SX - var.SX;
      dyo = HB_SY - var.SY;
    }
    code = stat(fname, &st);
    if (code == 0 && st.st_size > 100) {
      ok = 1;
      var.seq_rdr = seq;
      break;
    }
  }
  if (ok) {
    if ((fg = gzopen(fname,"rb")) != NULL) {
      gzbuffer(fg, 64*1024);
      gzread(fg, &rdr_cmp_head, sizeof(rdr_cmp_head));
      gzread(fg, rdr_cmp_stn_list, sizeof(rdr_cmp_stn_list));
      grid_zoom = var.grid/((rdr_cmp_head.dxy)*0.001);
      rdr1 = smatrix(0, rdr_cmp_head.ny-1, 0, rdr_cmp_head.nx-1);
      for (j = 0; j < rdr_cmp_head.ny; j++)
        gzread(fg, rdr1[j], rdr_cmp_head.nx*2);
      gzclose(fg);
    }
    else
      ok = 0;
  }

  // 2. 파일이 없으면 URL 방식으로 합성자료를 직접 요청
  if (ok == 0) {
    seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
    sprintf(url, "http://rdr.kma.go.kr/cgi-bin/rdr/nph-rdr_cmp_oth_data?tm=%04d%02d%02d%02d%02d&disp=B", YY, MM, DD, HH, MI);
    //printf("%s\n", url);
    if ((fr = url_fopen(url, "r"))) {
      n = url_fread(&rdr_cmp_head, sizeof(struct RDR_CMP_HEAD), 1, fr);
      n = url_fread(rdr_cmp_stn_list, sizeof(struct RDR_CMP_STN_LIST), 48, fr);
      rdr1 = smatrix(0, (int)(rdr_cmp_head.ny-1), 0, (int)(rdr_cmp_head.nx-1));
      for (j = 0; j < rdr_cmp_head.ny; j++)
        n = url_fread(rdr1[j], 2, rdr_cmp_head.nx, fr);
      url_fclose(fr);
    }
    else
      return -1;
  }

  // 3. 확대시, 중심위치와 확대비율 계산
  if (var.zoom_x[0] != '0') {
    for (i = 0; i < 7; i++, zm *= 1.5) {
      zx = var.zoom_x[i]-'0';
      zy = var.zoom_y[i]-'0';
      if (zx == 0 || zy == 0) break;

      //xo += ((float)(var.NX)/8.0*(zx-1)/zm);
      //yo += ((float)(var.NY)/8.0*(zy-1)/zm);
      xo += ((float)(var.NX)/24.0*(zx-1)/zm);
      yo += ((float)(var.NY)/24.0*(zy-1)/zm);
    }
  }

  // 4. 최대값으로 대치
  rt = rdr_cmp_head.nx/(var.NI*zm);
  if      (rt >= 6) ng = 8;
  else if (rt >= 3) ng = 4;
  else if (rt >= 1.5) ng = 2;
  else ng = 1;

  grid_short_filter(rdr1, (int)(rdr_cmp_head.nx), (int)(rdr_cmp_head.ny), BLANK3);
  //if (ng > 1) grid_short_max(rdr1, (int)(rdr_cmp_head.nx), (int)(rdr_cmp_head.ny), ng);

  //printf("rdr_cmp_head.nx = %d, rdr_cmp_head.ny = %d\n", rdr_cmp_head.nx, rdr_cmp_head.ny);
  //printf("%04d.%02d.%02d.%02d:%02d\n", rdr_cmp_head.tm.YY, rdr_cmp_head.tm.MM, rdr_cmp_head.tm.DD, rdr_cmp_head.tm.HH, rdr_cmp_head.tm.MI);
  //for (i = 0; i < 10; i++)
  //  printf("i = %d, %d\n", i, rdr_cmp_stn_list[i].tm.YY);

  // 5. 레이더 색상표 (mm/h)
  //sprintf(fname, "%s/color_rdr_noaa.rgb", COLOR_SET_DIR);   // 미국기상청 색상표
  //sprintf(fname, "%s/color_rdr_rn1.rgb", COLOR_SET_DIR);    // 간략 색상표
  //if (!strcmp(var.cht_mode, "ana") || !strcmp(var.cht_mode, "ana2") || !strcmp(var.cht_mode, "gts")) {
    sprintf(fname, "%s/color_rdr_echo.rgb", COLOR_SET_DIR);    // 레이더 색상표 
  //}
  if ((fp = fopen(fname,"r")) == NULL) return -1;
  num_color_rdr = 0;
  while (fscanf(fp, "%d %d %d %f\n", &R, &G, &B, &v1) != EOF) {
    color_rdr[num_color_rdr] = gdImageColorAllocate(im, R, G, B);
    data_rdr[num_color_rdr] = v1;
    num_color_rdr++;
    if (num_color_rdr >= 120) break;
  }
  fclose(fp);
  color_rdr_area = gdImageColorAllocate(im, 0, 233, 233);   // 레이더영역 테두리색

  // 6. 범례값 조정 및 표출범위 설정
  for (k = 0; k < num_color_rdr; k++) {
    dbz_rain_conv(&dbz1, &data_rdr[k], 1);
    data_lvl[k] = dbz1*100;
  }
  data_min = 100;     // 1dBZ
  data_max = 20000;   // 200dBZ

  if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD")) {
    // 7. 이미지 픽셀별로 계산
    for (j = 1; j < var.NJ; j++) {
      for (i = 1; i < var.NI; i++) {
        if (map_y[j][i] == -999) continue;
        y1 = map_y[j][i] + H3_SY;
        iy = (int)(y1*grid_zoom + 0.49);
        if (iy < 1 || iy >= rdr_cmp_head.ny-1) continue;
        //printf("j = %d, %d, %d\n", j, iy, rdr1[j][1000]);

        // 7.2.1. 자료 X방향 배열 위치
        x1 = map_x[j][i] + H3_SX;
        ix = (int)(x1*grid_zoom + 0.49);
        if (ix < 1 || ix >= rdr_cmp_head.nx-1) continue;

        // 7.2.2. 일정값 이상이면 표출
        v1 = rdr1[iy][ix];
        if (v1 >= data_min && v1 < data_max) {
          for (k = 0; k < num_color_rdr; k++) {
            if (v1 < data_lvl[k]) {
              gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_rdr[k]);
              break;
            }
          }
        }
      }
    }
  }
  else {
    // 7. 이미지 픽셀별로 계산
    for (j = 1; j < var.NJ; j++) {
      // 7.1. 자료 Y방향 배열 위치
      y1 = dg*j/zm + yo + dyo;
      iy = (int)(y1*grid_zoom + 0.49);
      if (iy < 1 || iy >= rdr_cmp_head.ny-1) continue;
      //printf("j = %d, %d, %d\n", j, iy, rdr1[j][1000]);

      // 7.2. X방향
      for (i = 1; i < var.NI; i++) {
        // 7.2.1. 자료 X방향 배열 위치
        x1 = dg*i/zm + xo + dxo;
        ix = (int)(x1*grid_zoom + 0.49);
        if (ix < 1 || ix >= rdr_cmp_head.nx-1) continue;

        // 7.2.2. 일정값 이상이면 표출
        v1 = rdr1[iy][ix];
        if (v1 >= data_min && v1 < data_max) {
          for (k = 0; k < num_color_rdr; k++) {
            if (v1 < data_lvl[k]) {
              gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_rdr[k]);
              break;
            }
          }
        }
      }

      // 7.3. 레이더 관측영역 경계선 표시(X방향)
      for (ix = 1; ix < rdr_cmp_head.nx-1; ix++) {
        if (rdr1[iy][ix] == BLANK1) {
          if (rdr1[iy][ix-1] > BLANK1 || rdr1[iy][ix+1] > BLANK1) {
            i = ix/grid_zoom - dxo -  xo;
            i = (int)(i*zm/dg);
            if (i < 0 || i > var.NI) continue;
            gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_rdr_area);
          }
        }
      }
    }

    // 8. 레이더 관측영역 경계선 표시(Y방향)
    for (i = 1; i < var.NI; i++) {
      x1 = dg*i/zm + xo + dxo;
      ix = (int)(x1*grid_zoom + 0.49);
      if (ix < 1 || ix >= rdr_cmp_head.nx-1) continue;

      for (iy = 1; iy < rdr_cmp_head.ny-1; iy++) {
        if (rdr1[iy][ix] == BLANK1) {
          if (rdr1[iy-1][ix] > BLANK1 || rdr1[iy+1][ix] > BLANK1) {
            j = iy/grid_zoom - dyo -  yo;
            j = (int)(j*zm/dg);
            if (j < 0 || j > var.NJ) continue;
            gdImageSetPixel(im, i+var.OI, var.NJ-j+var.OJ, color_rdr_area);
          }
        }
      }
    }
  }

  // 9. 범례 표출(분석장)
  if (!strcmp(var.cht_mode, "ana2") && (var.gis == 0 || var.legend_only == 1)) {
    if (var.legend_only == 1) {
      gdImageString(im, gdFontSmall, 0, 1, "(mm/h)", 0);   
    }

    for (i = 1; i < num_color_rdr; i++) {
      if (i < num_color_rdr - 1) {
        gdImageFilledRectangle(im, var.GI-45, var.OJ+(float)(var.NJ-1)*((float)(num_color_rdr-i-2)/(float)(num_color_rdr-2)), var.GI-30, var.OJ+(float)(var.NJ-1)*((float)(num_color_rdr-i-1)/(float)(num_color_rdr-2)), color_rdr[i]);
        gdImageRectangle(im, var.GI-45, var.OJ+(float)(var.NJ-1)*((float)(num_color_rdr-i-2)/(float)(num_color_rdr-2)), var.GI-30, var.OJ+(float)(var.NJ-1)*((float)(num_color_rdr-i-1)/(float)(num_color_rdr-2)), 0);
      }

      if ((int)(fabs(data_rdr[i-1])*10 + 0.5) % 10 == 0) {
        if (data_rdr[i-1] >= 0) sprintf(value, "%d", (int)(data_rdr[i-1]+0.5));
        else sprintf(value, "%d", (int)(data_rdr[i-1]-0.5));
      }
      else if ((int)(fabs(data_rdr[i-1])*100 + 0.5) % 10 == 0) {
        sprintf(value, "%.1f", data_rdr[i-1]);
      }
      else {
        sprintf(value, "%.2f", data_rdr[i-1]);
      }
      gdImageString(im, gdFontSmall, var.GI-27, var.OJ+(float)(var.NJ-1)*((float)(num_color_rdr-i-1)/(float)(num_color_rdr-2))-6, value, 0);
    }
  }

  // 10. 표출후 자료 저장소 배열 해제
  free_smatrix(rdr1, 0, (int)(rdr_cmp_head.ny-1), 0, (int)(rdr_cmp_head.nx-1));
  return 0;
}

/*=============================================================================*
 *  Z-R 관계식
 *     - mode : 0(dBZ->강수량), 1(강수량->dBZ)
 *=============================================================================*/
int dbz_rain_conv(float *dbz1, float *rain1, int mode)
{
  static int first = 0;
  static float za, zb;

  if (first == 0) {
    za = 0.1/var.ZRb;
    zb = log10(var.ZRa)/var.ZRb;
    first = 1;
  }

  if (mode == 0) {
    //*rain1 = (*dbz1*0.1 - log10(var.ZRa) )/var.ZRb;
    *rain1 = *dbz1*za - zb;
    *rain1 = pow(10.0, *rain1);
  }
  else if (mode == 1) {
    *dbz1 = 10.0 * log10( var.ZRa * pow(*rain1, var.ZRb) );
  }
  return 0;
}

/*******************************************************************************
 *  Grid data Smoothing (by 1-2-1 Smoothing)
 *  by 이정환 (1997. 3. 15)
 *******************************************************************************/
int grid_short_filter (
  short **g,      // input -> output
  int   nx,       // 영역 [0:nx-1,0:ny-1]
  int   ny,
  short missing   // 이하는 자료 없음
)
{
  short  *e1, *e2, *e3, *e4;
  short  v1, v2, *v4;
  int    i, j;

  // 1. Y축 기준으로, X축 방향으로 평활화한다.
  for (j = 0; j < ny; j++) {
    e1 = &g[j][0];
    e2 = &g[j][1];
    e3 = &g[j][2];
    v1 = *e1;

    for (v1 = *e1, i = 1; i < nx-1; i++, e1++, e2++, e3++) {
      if (*e1 > missing && *e2 > missing && *e3 > missing)
        v2 = (*e1 + *e2 + *e2 + *e3) / 4;
      else if (*e1 > missing && *e2 <= missing && *e3 > missing)
        v2 = (*e1 + *e3) / 2;
      else
        v2 = *e2;

      (*e1) = v1;
      v1 = v2;
    }
    (*e1) = v1;
  }

  // 2. X축 기준으로, Y축 방향으로 평활화한다.
  v4 = svector(0, nx-1);    // 배열 준비
  for (j = 1; j < ny-1; j++) {
    e1 = &g[j-1][0];
    e2 = &g[j][0];
    e3 = &g[j+1][0];
    e4 = &v4[0];

    for (i = 0; i < nx; i++, e1++, e2++, e3++, e4++) {
      if (*e1 > missing && *e2 > missing && *e3 > missing)
        v2 = (*e1 + *e2 + *e2 + *e3) / 4;
      else if (*e1 > missing && *e2 <= missing && *e3 > missing)
        v2 = (*e1 + *e3) / 2;
      else
        v2 = *e2;

      if (j > 1) (*e1) = *e4;
      *e4 = v2;
    }
  }
  for (i = 0; i < nx; i++)
    (*e1) = *e4;

  // 배열 정리
  free_svector(v4, 0, nx-1);
  return 0;
}

/*******************************************************************************
 *  낙뢰 표출
 *******************************************************************************/
int lgt_disp(gdImagePtr im)
{
  FILE   *fp;
  struct lamc_parameter map;
  struct ster_parameter map2;
  struct eqdc_parameter map3;
  char   fname[120], buf[1000], tmp[100], tp[8], gc, wkt[100];
  float  zm = 1.0, xo = 0.0, yo = 0.0, zx, zy, x1, y1;
  float  x, y, lat, lon, ht, power, nsf, errd, f1, f2, f3, f5;
  int    id, mt, gcd, d4, nscan, t1;
  int    YY, MM, DD, HH, MI, SS, nsec, seq;
  int    YY1, MM1, DD1, HH1, MI1;
  int    YY2, MM2, DD2, HH2, MI2;
  int    nday, iseq, diff, ix, iy, depth = 4;
  int    i, j, k, code, color_bg;

  // 1. 낙뢰 색상표
  //color_lgt = gdImageColorAllocate(im, 255, 255, 0);
  color_lgt = gdImageColorAllocate(im, 255, 0, 0);
  color_bg  = gdImageColorAllocate(im, 250, 250, 250);

  // 2. 확대를 반영한 지도변환 설정
  if (var.zoom_x[0] != '0') {
    for (i = 0; i < 7; i++, zm *= 1.5) {
      zx = var.zoom_x[i]-'0';
      zy = var.zoom_y[i]-'0';
      if (zx == 0 || zy == 0) break;

      //xo += ((float)(var.NX)/8.0*(zx-1)/zm);
      //yo += ((float)(var.NY)/8.0*(zy-1)/zm);
      xo += ((float)(var.NX)/24.0*(zx-1)/zm);
      yo += ((float)(var.NY)/24.0*(zy-1)/zm);
    }
  }
  zm *= ((float)(var.NI)/(float)(var.NX));

  if (!strcmp(var.map, "NHEM")) {
    map2.Re    = 6371.00877;
    map2.grid  = var.grid/zm;
    map2.slon  = 120.0;
    map2.slat  = 90.0;
    map2.olon  = 120.0;
    map2.olat  = 90.0;
    map2.xo = (float)var.SX/map2.grid - xo*zm;
    map2.yo = (float)var.SY/map2.grid - yo*zm;
    map2.first = 0;
    map2.polar = 0;
  }
  else if (!strcmp(var.map, "WORLD")) {
    map3.Re    = 6371.00877;
    map3.grid  = var.grid/zm;
    map3.slon  = 126.0;
    map3.slat  = 0.0;
    map3.olon  = 126.0;
    map3.olat  = 0.0;
    map3.xo = (float)var.SX/map3.grid - xo*zm;
    map3.yo = (float)var.SY/map3.grid - yo*zm;
    map3.first = 0;
  }
  else {
    map.Re    = 6371.00877;
    map.grid  = var.grid/zm;
    map.slat1 = 30.0;    map.slat2 = 60.0;
    map.olon  = 126.0;   map.olat  = 38.0;
    map.xo = (float)var.SX/map.grid - xo*zm;
    map.yo = (float)var.SY/map.grid - yo*zm;
    map.first = 0;
  }

  // 3. 읽을 파일의 수 확인
  seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
  seq2time(var.seq-var.itv, &YY1, &MM1, &DD1, &HH1, &MI1, 'm', 'n');
  if (DD1 == DD) nday = 0;
  else           nday = 1;

  //if (var.zoom_x[0] != '0') depth = 4;
  //if (var.zoom_x[2] != '0') depth = 6;

  // 4. 자료 파일을 순차적으로 읽어서 처리
  for (k = nday; k >= 0; k--) {
    seq2time(var.seq-k*24*60, &YY1, &MM1, &DD1, &HH1, &MI1, 'm', 'n');
    code = lgt_file(fname, YY1, MM1, DD1);
    if (code < 0) continue;
    if ((fp = fopen(fname, "r")) == NULL) continue;

    while (fgets(buf, 1024, fp) != NULL) {
      if (strstr(fname,"NX1")) {
        if (strlen(buf) < 80) continue;
        sscanf(buf, "%d %d-%d-%d %d:%d:%d.%d+%s %d %s %f %f %f %f %f %d %d",
               &id, &YY, &MM, &DD, &HH, &MI, &SS, &nsec, tmp, &nsf, wkt, &lon, &lat, &ht, &power, &errd, &gcd, &nscan);
        if (gcd != 1) continue;     // 대지방전(1)만 표출 (2:구름방전)
        if (nscan < 4) continue;    // 4개 이상 센서가 감지한 경우만 표출

        iseq = time2seq(YY, MM, DD, HH, MI, 'm');
        diff = abs(var.seq - iseq);
      }
      else {
        nscan = 3;  // 낙뢰 감지 센터가 3개 이상인 경우만 표출 (최근 포맷에만 있음. 과거는 그냥 표출)
        if (code == 0) {
          if (strlen(buf) > 80) {
            sscanf(buf, "%d/%d/%d %d:%d:%f %f %f %f %d %f %f %f %d %f %d %c",
              &MM, &DD, &YY, &HH, &MI, &SS, &lat, &lon, &power, &mt, &f1, &f2, &f3, &d4, &f5, &nscan, &gc);
          }
          else {
            sscanf(buf, "%d/%d/%d %d:%d:%f %f %f %f %d", &MM, &DD, &YY, &HH, &MI, &SS, &lat, &lon, &power, &mt);
            gc = 'G';
          }
        }
        else if (code == 1) {
          YY = YY1;  MM = MM1;  DD = DD1;
          sscanf(buf, "%d:%d:%f %f %f %f %d", &HH, &MI, &SS, &lat, &lon, &power, &mt);
          gc = 'G';
        }
        else if (code == 2) {
          sscanf(buf, "%d/%d/%d %d:%d:%f %f %f %f %d", &MM, &DD, &YY, &HH, &MI, &SS, &lat, &lon, &power, &mt);
          gc = 'G';
        }
        else if (code == 3) {
          getword(tmp,buf,' ');
          getword(tp, buf,' ');
          if (strcmp(tp,"FG") == 0) gc = 'G';
          else                      gc = 'C';

          getword(tmp,buf,'/');  MM = atoi(tmp);
          getword(tmp,buf,'/');  DD = atoi(tmp);
          getword(tmp,buf,' ');  YY = atoi(tmp) + 2000;
          getword(tmp,buf,':');  HH = atoi(tmp);
          getword(tmp,buf,':');  MI = atoi(tmp);
          getword(tmp,buf,'.');  SS = atoi(tmp);
          getword(tmp,buf,' ');  nsec = atoi(tmp);

          getword(tmp,buf,'=');  lat = atof(buf);
          getword(tmp,buf,'=');  lon = atof(buf);
          getword(tmp,buf,'=');  power = atof(buf);
        }
        if (gc != 'G' || nscan < 3) continue;

        if (YY < 80) YY += 2000;
        else if (YY <= 99) YY += 1900;

        iseq = time2seq(YY, MM, DD, HH, MI, 'm');
        if (code == 3) iseq += 9*60;
        diff = abs(var.seq - iseq);
      }

      if (diff >= 0 && diff <= var.itv) {
        if (!strcmp(var.map, "NHEM")) sterproj_ellp(&lon, &lat, &x, &y, 0, &map2);
        else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon, &lat, &x, &y, 0, &map3);
        else lamcproj_ellp(&lon, &lat, &x, &y, 0, &map);

        if (x > 2.0 && x < (float)(var.NI-2) && y > 2.0 && y < (float)(var.NJ-2)) {
          ix = (int)(x + 0.5) + var.OI;
          iy = var.NJ - (int)(y + 0.5) + var.OJ;

          if (var.border > 0) {
            gdImageLine(im, ix-depth, iy+1, ix+depth, iy+1, color_bg);
            gdImageLine(im, ix-depth, iy-1, ix+depth, iy-1, color_bg);
            gdImageLine(im, ix-depth+1, iy, ix+depth+1, iy, color_bg);
            gdImageLine(im, ix-depth-1, iy, ix+depth-1, iy, color_bg);
            gdImageLine(im, ix, iy-depth+1, ix, iy+depth+1, color_bg);
            gdImageLine(im, ix, iy-depth-1, ix, iy+depth+1, color_bg);
            gdImageLine(im, ix+1, iy-depth, ix+1, iy+depth, color_bg);
            gdImageLine(im, ix-1, iy-depth, ix-1, iy+depth, color_bg);
          }

          gdImageLine(im, ix-depth, iy, ix+depth, iy, color_lgt);
          gdImageLine(im, ix, iy-depth, ix, iy+depth, color_lgt);
          //if (var.zoom_x[2] != '0') {
            //gdImageLine(im, ix-depth, iy-1, ix+depth, iy-1, color_lgt);
            //gdImageLine(im, ix+1, iy-depth, ix+1, iy+depth, color_lgt);
          //}
        }
      }
      else if (diff < 0)
        break;
    }
    fclose(fp);
  }
  return 0;
}

/*=============================================================================*
 *  낙뢰 파일명과 존재 여부 (기상청)
 *=============================================================================*/
int lgt_file(char *fname, int YY, int MM, int DD)
{
  struct stat st1, st2, st3;
  int    code = 0;

  if ((YY*100+MM)*100+DD >= 20150401) {
    sprintf(fname, "%s/%04d/LGT_KMA_NX1_%02d%02d%02d.asc", LGT_DIR, YY, YY, MM, DD);
    if (stat(fname, &st1) < 0)
      code = -3;
    else
      code = 0;
  }
  else {
    sprintf(fname, "%s/%04d/LGT_KMA_%02d%02d%02d.asc", LGT_DIR, YY, YY, MM, DD);
    if (stat(fname, &st1) < 0) {
      sprintf(fname, "%s/%04d/LGT_TDM_%04d%02d%02d.asc", LGT_DIR, YY, YY, MM, DD);
      if (stat(fname, &st2) < 0) {
        sprintf(fname, "%s/%04d/LGT_LVL_%04d%02d%02d.asc", LGT_DIR, YY, YY, MM, DD);
        if (stat(fname, &st3) < 0)
          code = -3;
        else
          code = 2;
      }
      else
        code = 1;
    }
    else if (st1.st_size < 10) {
      if (st1.st_size < 10)
        code = -2;
      else
        code = 0;
    }
  }
  return code;
}

/*******************************************************************************
 *  wissdom 바람 표출
 *******************************************************************************/
int wis_wd(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  float uu1, 
  float vv1,
  float lon,
  int color_wd
)
{
  float WR = 12.0;
  float wr_s, wd_s, ws, wd, adj_wd;
  int x1, y1, x2, y2, x3, y3;
  gdPoint xy[3], xy2[3], xy3[3], xy4[3], xy5[3];
  char   txt[128], txt_utf[128];
  double font_size = 8.0;
  int    brect[8];
  int    i;

  font_size *= var.font_size;

  ws = sqrt(uu1*uu1 + vv1*vv1);
  wd = 180./3.141592 * atan(uu1/vv1);
  if ( vv1  > 0.0)                  wd = wd + 180;
  if ((vv1  < 0.0) && (uu1 >= 0.0)) wd = wd + 360;
  if ((vv1 == 0.0) && (uu1 >  0.0)) wd = 270.;
  if ((vv1 == 0.0) && (uu1 <  0.0)) wd = 90.;

  if (!strcmp(var.map,"NHEM")) {
    wd -= (lon - 120.0);
  }
  else if (strcmp(var.map, "WORLD")) {
    adj_wd = get_adj_angle_lcc(lon);
    wd -= adj_wd;
  }

  if (wd > 360) wd -= 360;
  else if (wd < 0) wd += 360;

  if (wd > 360) wd -= 360;
  if (wd < 0 || wd > 360) return;
  if (ws < 0 || ws > 100) return;

  // 풍향
  wd *= DEGRAD;

  x1 = xs;
  y1 = ys;
  x2 = xs + (int)(WR*sin(wd) + 0.5);
  y2 = ys - (int)(WR*cos(wd) + 0.5);
  if (ws > 0.2) {
    gdImageLine(im, x1, y1, x2, y2, color_wd);
  }

  // 풍속깃
  while (ws > 0.0)
  {
    if (ws < 5)
      wr_s = 1.5*ws;
    else
      wr_s = 7.5;

    wd_s = wd + 60.0*DEGRAD;
    x1 = x2 + (int)(wr_s*sin(wd_s) + 0.5);
    y1 = y2 - (int)(wr_s*cos(wd_s) + 0.5);

    if (ws >= 25) {
      WR -= 2.5;
      x3 = xs + (int)(WR*sin(wd) + 0.5);
      y3 = ys - (int)(WR*cos(wd) + 0.5);

      xy[0].x = x1; xy2[0].x = x1;   xy3[0].x = x1;   xy4[0].x = x1-1; xy5[0].x = x1+1;
      xy[0].y = y1; xy2[0].y = y1-1; xy3[0].y = y1+1; xy4[0].y = y1;   xy5[0].y = y1;
      xy[1].x = x2; xy2[1].x = x2;   xy3[1].x = x2;   xy4[1].x = x2-1; xy5[1].x = x2+1;
      xy[1].y = y2; xy2[1].y = y2-1; xy3[1].y = y2+1; xy4[1].y = y2;   xy5[1].y = y2;
      xy[2].x = x3; xy2[2].x = x3;   xy3[2].x = x3;   xy4[2].x = x3-1; xy5[2].x = x3+1;
      xy[2].y = y3; xy2[2].y = y3-1; xy3[2].y = y3+1; xy4[2].y = y3;   xy5[2].y = y3;
      gdImageFilledPolygon(im, xy, 3, color_wd);

      ws -= 25.0;
      x2 = x3;
      y2 = y3;
    }
    else {
      gdImageLine(im, x1, y1, x2, y2, color_wd);
      ws -= 5.0;
      WR -= 2.5;
      x2 = xs + (int)(WR*sin(wd) + 0.5);
      y2 = ys - (int)(WR*cos(wd) + 0.5);
    }
  }

  //gdImageFilledArc(im, xs, ys, 3, 3, 0, 360, color_wd, gdArc);
  return;
}

/*******************************************************************************
 *  WISSDOM 바람장 표출
 *******************************************************************************/
int wis_disp(gdImagePtr im, int color_lvl[256])
{
  struct lamc_parameter map;
  char   fname[120];
  struct stat st;
  short  *s, *ts=NULL;
  float  **g1, **g2, uu1, vv1, wd, ws, lat, lon, x, y;
  int    *rdr_ht;
  int    YY, MM, DD, HH, MI;
  int    ok = 0, seq, code;
  int    i, j, k, ix, iy, i1, j1;
  float  zm = 1.0, xo = 0.0, yo = 0.0, zx, zy, x1, y1, v1, rt;
  int    nc_id, var_id, dim_id;
  int    num_stn;
  float  data_minus, data_scale, data_out, data_in, grid;
  size_t start[3]={0,}, count[3]={0,}, nx, ny, nz;
  nc_type var_type;
  int    ht_idx;
  float  *dd=NULL, ht, ht_diff;
  float  dg = (float)var.NX/(float)(var.NI);
  float  rate = (float)(var.NI)/(float)(var.NX);
  float  grid_zoom;
  float  dxo = HW_SX - var.SX;
  float  dyo = HW_SY - var.SY;

  char   txt[256], txt_utf[256];
  int    brect[8];
  int    color;
  double font_size = 8.0;

  // 1. 자료 확인
  ok = 0;
  for (seq = var.seq; seq >= var.seq-10; seq -= 5) {
    seq2time(seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
    sprintf(fname, "%s/%04d%02d/%02d/RDR_R3D_KMA_WD_%04d%02d%02d%02d%02d.nc",
          RDR_CMP_R3D, YY, MM, DD, YY, MM, DD, HH, MI);

    code = stat(fname, &st);
    if (code == 0 && st.st_size > 100) {
      ok = 1;
      break;
    }
  }

  if (ok) {
    // 1. 파일 읽기
    code = nc_open(fname, NC_NOWRITE, &nc_id);
    if (code != NC_NOERR) return -1;

    // 2. 특성 정보
    // 2.1. 격자 정보
    nc_inq_dimid(nc_id, "nx", &dim_id);
    nc_inq_dimlen(nc_id, dim_id, &nx);
    nc_inq_dimid(nc_id, "ny", &dim_id);
    nc_inq_dimlen(nc_id, dim_id, &ny);
    nc_inq_dimid(nc_id, "nz", &dim_id);
    nc_inq_dimlen(nc_id, dim_id, &nz);

    nc_get_att_float(nc_id, NC_GLOBAL, "data_minus", &data_minus);
    nc_get_att_float(nc_id, NC_GLOBAL, "data_scale", &data_scale);
    nc_get_att_float(nc_id, NC_GLOBAL, "grid_size",  &grid);
 
    grid_zoom = var.grid/grid*1000;
    //grid_zoom = 1;
 
    g1 = matrix(0, ny, 0, nx);
    g2 = matrix(0, ny, 0, nx);
    count[0] = 1; count[1] = ny; count[2] = nx;

    // 3. 자료 읽기
    // 3.1. 고도 읽기
    rdr_ht = (int *) malloc( sizeof(int) * nz );
    nc_inq_varid(nc_id, "height", &var_id);
    nc_get_var_int(nc_id, var_id, rdr_ht);
 
    // 특정 고도의 가장 가까운 한 층만 읽기
    // 가장 가까운 고도 찾기
    switch (atoi(var.gts))
    {
      case 1000:
        ht = 100;
        break;
      case 925:
        ht = 750;
        break;
      case 850:
        ht = 1500;
        break;
      case 700:
        ht = 3000;
        break;
      case 500:
        ht = 5500;
        break;
      case 300:
        ht = 9000;
        break;
      case 200:
        ht = 12000;
        break;
      case 100:
        ht = 16000;
        break;
      default:
        ht = 0;
        break;
    }

    ht_idx = 0;
    ht_diff = fabs(ht - rdr_ht[0]);
    for (k = 1; k < nz; k++) {
      if ( ht_diff > fabs(ht - rdr_ht[k]) ) {
        ht_diff = fabs(ht - rdr_ht[k]);
        ht_idx = k;
      }
    }

    //sprintf(txt, "ht: %.0f / ht_idx: %d / rdr_ht: %d", ht, ht_idx, rdr_ht[ht_idx]);
    //for (i = 0; i < 256; i++) txt_utf[i] = 0;
    //euckr2utf(txt, txt_utf);
    //gdImageStringFT(im, &brect[0], color, FONTTTF, font_size, 0.0, 100, 100, txt_utf);

    // 3.2. U 벡터 읽기
    nc_inq_varid(nc_id, "u_component", &var_id);
    s = (short *) malloc( sizeof(short) * nx * ny );

    start[0] = ht_idx; start[1] = 0; start[2] = 0;
    nc_get_vara_short(nc_id, var_id, start, count, s);
 
    for (ts = s, j = 0; j < ny; j++) {
      for (dd = g1[j], i = 0; i < nx; i++, dd++, ts++) {
        *dd = (*ts - data_minus) / data_scale;
      }
    }
    free(s);

     // 3.2. V 벡터 읽기
    nc_inq_varid(nc_id, "v_component", &var_id);
    s = (short *) malloc( sizeof(short) * nx * ny );

    start[0] = ht_idx; start[1] = 0; start[2] = 0;
    nc_get_vara_short(nc_id, var_id, start, count, s);
 
    for (ts = s, j = 0; j < ny; j++) {
      for (dd = g2[j], i = 0; i < nx; i++, dd++, ts++) {
        *dd = (*ts - data_minus) / data_scale;
      }
    }
    free(s);

    // 4. 확대시, 중심위치와 확대비율 계산
    if (var.zoom_x[0] != '0') {
      for (i = 0; i < 7; i++, zm *= 1.5) {
        zx = var.zoom_x[i]-'0';
        zy = var.zoom_y[i]-'0';
        if (zx == 0 || zy == 0) break;

        xo += ((float)(var.NX)/24.0*(zx-1)/zm);
        yo += ((float)(var.NY)/24.0*(zy-1)/zm);
      }
    }

    if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD")) {
      // 5. 이미지 픽셀별로 계산
      for (j = 1; j < var.NJ; j += 20) {
        // 5.2. X방향
        for (i = 1; i < var.NI; i += 20) {
          if (map_y[j][i] == -999) continue;
          // 5.1. 자료 Y방향 배열 위치
          y1 = map_y[j][i] + HW_SY;
          iy = (int)(y1*grid_zoom + 0.49);
          if (iy < 1 || iy >=ny-1) continue;

          // 5.2.1. 자료 X방향 배열 위치
          x1 = map_x[j][i] + HW_SX;
          ix = (int)(x1*grid_zoom + 0.49);
          if (ix < 1 || ix >= nx-1) continue;

          // 5.2.2. 자료 정상이면 표출
          uu1 = g1[iy][ix];
          vv1 = g2[iy][ix];
          if (uu1 < -90 || vv1 < -90) continue;

          wis_wd(im, i+var.OI, var.NJ-j+var.OJ, uu1, vv1, map_lon[j][i], color_lvl[242]);
        }
      }
    }
    else {
      // 0. 지도 투영법
      map.Re    = 6371.00877;
      map.grid  = var.grid/(zm*rate);
      map.slat1 = 30.0;    map.slat2 = 60.0;
      map.olon  = 126.0;   map.olat  = 38.0;
      map.xo = (float)(var.SX - xo)*(zm*rate);
      map.yo = (float)(var.SY - yo)*(zm*rate);
      map.first = 0;

      // 5. 이미지 픽셀별로 계산
      for (j = 1; j < var.NJ; j += 20) {
        // 5.1. 자료 Y방향 배열 위치
        y1 = dg*j/zm + yo + dyo;
        iy = (int)(y1*grid_zoom + 0.49);
        if (iy < 1 || iy >=ny-1) continue;

        // 5.2. X방향
        for (i = 1; i < var.NI; i += 20) {
          // 5.2.1. 자료 X방향 배열 위치
          x1 = dg*i/zm + xo + dxo;
          ix = (int)(x1*grid_zoom + 0.49);
          if (ix < 1 || ix >= nx-1) continue;

          // 5.2.2. 자료 정상이면 표출
          uu1 = g1[iy][ix];
          vv1 = g2[iy][ix];
          if (uu1 < -90 || vv1 < -90) continue;

          x = (float)i; y = (float)j;
          lamcproj_ellp(&lon, &lat, &x, &y, 1, &map);
          wis_wd(im, i+var.OI, var.NJ-j+var.OJ, uu1, vv1, lon, color_lvl[242]);
        }
      }
    }

    // 6. 배열 해제
    free_matrix(g1, 0, ny, 0, nx);
    free_matrix(g2, 0, ny, 0, nx);
  }

  return 0;
}

/*******************************************************************************
 *  ASCAT 바람 표출
 *******************************************************************************/
int ascat_wd(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  float wd, 
  float ws,
  float lon,
  int color_wd
)
{
  float WR = 12.0;
  float wr_s, wd_s, adj_wd;
  int x1, y1, x2, y2, x3, y3;
  gdPoint xy[3], xy2[3], xy3[3], xy4[3], xy5[3];
  char   txt[128], txt_utf[128];
  double font_size = 8.0;
  int    brect[8];
  int    i;

  if (!strcmp(var.map,"NHEM")) {
    wd -= (lon - 120.0);
  }
  else if (strcmp(var.map, "WORLD")) {
    adj_wd = get_adj_angle_lcc(lon);
    wd -= adj_wd;
  }

  if (wd > 360) wd -= 360;
  else if (wd < 0) wd += 360;

  if (wd > 360) wd -= 360;
  if (wd < 0 || wd > 360) return;
  if (ws < 0 || ws > 100) return;

  // 풍향
  wd *= DEGRAD;

  x1 = xs;
  y1 = ys;
  x2 = xs + (int)(WR*sin(wd) + 0.5);
  y2 = ys - (int)(WR*cos(wd) + 0.5);
  if (ws > 0.2) {
    gdImageLine(im, x1, y1, x2, y2, color_wd);
  }

  // 풍속깃
  while (ws > 0.0)
  {
    if (ws < 5)
      wr_s = 1.5*ws;
    else
      wr_s = 7.5;

    wd_s = wd + 60.0*DEGRAD;
    x1 = x2 + (int)(wr_s*sin(wd_s) + 0.5);
    y1 = y2 - (int)(wr_s*cos(wd_s) + 0.5);

    if (ws >= 25) {
      WR -= 2.5;
      x3 = xs + (int)(WR*sin(wd) + 0.5);
      y3 = ys - (int)(WR*cos(wd) + 0.5);

      xy[0].x = x1; xy2[0].x = x1;   xy3[0].x = x1;   xy4[0].x = x1-1; xy5[0].x = x1+1;
      xy[0].y = y1; xy2[0].y = y1-1; xy3[0].y = y1+1; xy4[0].y = y1;   xy5[0].y = y1;
      xy[1].x = x2; xy2[1].x = x2;   xy3[1].x = x2;   xy4[1].x = x2-1; xy5[1].x = x2+1;
      xy[1].y = y2; xy2[1].y = y2-1; xy3[1].y = y2+1; xy4[1].y = y2;   xy5[1].y = y2;
      xy[2].x = x3; xy2[2].x = x3;   xy3[2].x = x3;   xy4[2].x = x3-1; xy5[2].x = x3+1;
      xy[2].y = y3; xy2[2].y = y3-1; xy3[2].y = y3+1; xy4[2].y = y3;   xy5[2].y = y3;
      gdImageFilledPolygon(im, xy, 3, color_wd);

      ws -= 25.0;
      x2 = x3;
      y2 = y3;
    }
    else {
      gdImageLine(im, x1, y1, x2, y2, color_wd);
      ws -= 5.0;
      WR -= 2.5;
      x2 = xs + (int)(WR*sin(wd) + 0.5);
      y2 = ys - (int)(WR*cos(wd) + 0.5);
    }
  }

  //gdImageFilledArc(im, xs, ys, 3, 3, 0, 360, color_wd, gdArc);
  return;
}

/*******************************************************************************
 *  ASCAT 바람장 표출
 *******************************************************************************/
int ascat_disp(gdImagePtr im, int color_lvl[256])
{
  struct dirent *dirp;
  DIR *dp;
  FILE* in = NULL;
  codes_handle* h=NULL;
  struct lamc_parameter map;
  struct ster_parameter map2;
  struct eqdc_parameter map3;
  struct stat st;
  char   fname[512], dirname[512], filetime[512];
  double *latitude, *longitude, *windSpeedAt10M, *windDirectionAt10M, *indexOfSelectedWindVector;
  size_t size;
  int    YY, MM, DD, HH, MI;
  int    err;
  int    ok = 0, seq, code;
  int    i, j, k, ix, iy, i1, j1;
  float  xs, ys, lat, lon;
  float  zm = 1.0, xo = 0.0, yo = 0.0, zx, zy;
  float  rate = (float)(var.NI)/(float)(var.NX);
  int    nx_box, ny_box, box, plot, ii, jj, bi, bj, color1, color2;
  float  **boxs;

  short   *s, *ts=NULL, smv;
  int     *p, *tp=NULL, imv;
  float   **g1, **g2, **lats, **lons, *dd=NULL;
  int     nc_id, var_id, dim_id, status;
  double  offset, scale_factor;
  size_t  nx, ny;

  color1 = gdImageColorAllocate(im, 255, 0, 255);

  // 1. 확대시, 중심위치와 확대비율 계산
  if (var.zoom_x[0] != '0') {
    for (i = 0; i < 7; i++, zm *= 1.5) {
      zx = var.zoom_x[i]-'0';
      zy = var.zoom_y[i]-'0';
      if (zx == 0 || zy == 0) break;

      xo += ((float)(var.NX)/24.0*(zx-1)/zm);
      yo += ((float)(var.NY)/24.0*(zy-1)/zm);
    }
  }

  // 2. 지도 투영법
  if (!strcmp(var.map, "NHEM")) {
    map2.Re    = 6371.00877;
    map2.grid  = var.grid/(zm*rate);
    map2.slon  = 120.0;
    map2.slat  = 90.0;
    map2.olon  = 120.0;
    map2.olat  = 90.0;
    map2.xo = (float)(var.SX - xo)*(zm*rate);
    map2.yo = (float)(var.SY - yo)*(zm*rate);
    map2.first = 0;
    map2.polar = 0;
  }
  else if (!strcmp(var.map, "WORLD")) {
    map3.Re    = 6371.00877;
    map3.grid  = var.grid/(zm*rate);
    map3.slon  = 126.0;
    map3.slat  = 0.0;
    map3.olon  = 126.0;
    map3.olat  = 0.0;
    map3.xo = (float)(var.SX - xo)*(zm*rate);
    map3.yo = (float)(var.SY - yo)*(zm*rate);
    map3.first = 0;
  }
  else {
    map.Re    = 6371.00877;
    map.grid  = var.grid/(zm*rate);
    map.slat1 = 30.0;    map.slat2 = 60.0;
    map.olon  = 126.0;   map.olat  = 38.0;
    map.xo = (float)(var.SX - xo)*(zm*rate);
    map.yo = (float)(var.SY - yo)*(zm*rate);
    map.first = 0;
  }

  // 간벌 설정
  box = 6;
  nx_box = (int)(var.NI) + 1;
  ny_box = (int)(var.NJ) + 1;
  boxs = matrix(0, ny_box-1, 0, nx_box-1);
  for (j = 0; j < ny_box; j++) {
    for (i = 0; i < nx_box; i++) {
      boxs[j][i] = 0;
    }
  }

  // Metop-C 자료
  // 1. 자료 확인
  ok = 0;
  seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
  sprintf(fname, "%s/%04d%02d/%02d/EHDB_ISXX_%04d%02d%02d%02d.bfr", GTS_BUFR_DIR, YY, MM, DD, YY, MM, DD, HH);

  code = stat(fname, &st);
  if (code == 0 && st.st_size > 100) {
    ok = 1;
  }

  if (ok) {
    in = fopen(fname, "rb");
    if (in) {
      /* loop over the messages in the BUFR file */
      while ((h = codes_handle_new_from_file(NULL, in, PRODUCT_BUFR, &err)) != NULL || err != CODES_SUCCESS) {
        if (h == NULL) {
          codes_handle_delete(h);
          continue;
        }

        /* we need to instruct ecCodes to expand the descriptors
           i.e. unpack the data values */
        codes_set_long(h, "unpack", 1);
 
        /* get the size and allocate memory*/
        size = 0;
        codes_get_size(h, "latitude", &size);

        latitude            = (double*)malloc(size*sizeof(double));
        longitude           = (double*)malloc(size*sizeof(double));
        windSpeedAt10M      = (double*)malloc(size*sizeof(double));
        windDirectionAt10M  = (double*)malloc(size*sizeof(double));
        indexOfSelectedWindVector = (double*)malloc(size*sizeof(double));

        /* get the values*/
        codes_get_double_array(h, "latitude", latitude, &size);
        codes_get_double_array(h, "longitude", longitude, &size);
        codes_get_double_array(h, "windSpeedAt10M", windSpeedAt10M, &size);
        codes_get_double_array(h, "windDirectionAt10M", windDirectionAt10M, &size);
        codes_get_double_array(h, "indexOfSelectedWindVector", indexOfSelectedWindVector, &size);

        for (i = 0; i < size; i++) {
          if (windDirectionAt10M[i] < var.missing || windSpeedAt10M[i] < var.missing) continue;
          if (latitude[i] < var.lat1 || latitude[i] > var.lat2 || longitude[i] < var.lon1 || longitude[i] > var.lon2) continue;
          if (indexOfSelectedWindVector[i] == 2) continue;

          lon = longitude[i]; lat = latitude[i];
          if (!strcmp(var.map, "NHEM"))       sterproj_ellp(&lon, &lat, &xs, &ys, 0, &map2);
          else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon, &lat, &xs, &ys, 0, &map3);
          else lamcproj_ellp(&lon, &lat, &xs, &ys, 0, &map);
          if (xs < 0 || xs > var.NI || ys < 0 || ys > var.NJ) continue;

          // 이미지에서 격자 위치
          ii = (int)(xs);
          jj = (int)(ys);

          plot = 1;
          for (bj = jj-box; bj < jj+box; bj++) {
            if (bj < 0 || bj >= ny_box) continue;
            for (bi = ii-box; bi < ii+box; bi++) {
              if (bi < 0 || bi >= nx_box) continue;          
              if (boxs[bj][bi] != 0) {
                plot = 0;
                break;
              }
            }
          }

          if (plot == 1) {
            if (windSpeedAt10M[i] >= 15) {
              ascat_wd(im, xs + var.OI, var.NJ - ys + var.OJ, windDirectionAt10M[i], windSpeedAt10M[i], lon, color1);
            }
            else {
              ascat_wd(im, xs + var.OI, var.NJ - ys + var.OJ, windDirectionAt10M[i], windSpeedAt10M[i], lon, color_lvl[231]);
            }

            for (bj = jj-box; bj < jj+box; bj++) {
              if (bj < 0 || bj >= ny_box) continue;
              for (bi = ii-box; bi < ii+box; bi++) {
                if (bi < 0 || bi >= nx_box) continue;          
                boxs[bj][bi]++;
              }
            }
          }
        }

        /* free allocated arrays */
        free(latitude);
        free(longitude);
        free(windSpeedAt10M);
        free(windDirectionAt10M);
        free(indexOfSelectedWindVector);

        /* delete handle */
        codes_handle_delete(h);
      }

      fclose(in);
    }
  }


  // Metop-B 자료
  sprintf(dirname, "%s/%04d%02d/%02d/", ASCAT_DIR, YY, MM, DD);
  sprintf(filetime, "%04d%02d%02d_%02d", YY, MM, DD, HH);

  if ((dp=opendir(dirname)) != NULL) {
    while ((dirp=readdir(dp)) != NULL) {
      if (strstr(dirp->d_name, "eps_o_coa_ovw") && strstr(dirp->d_name, filetime) && strstr(dirp->d_name, "metopb")) {

        sprintf(fname, "%s%s", dirname, dirp->d_name);
        in = fopen(fname, "rb");
        if (in) {
          /* loop over the messages in the BUFR file */
          while ((h = codes_handle_new_from_file(NULL, in, PRODUCT_BUFR, &err)) != NULL || err != CODES_SUCCESS) {
            if (h == NULL) {
              codes_handle_delete(h);
              continue;
            }

            /* we need to instruct ecCodes to expand the descriptors
               i.e. unpack the data values */
            codes_set_long(h, "unpack", 1);
 
            /* get the size and allocate memory*/
            size = 0;
            codes_get_size(h, "latitude", &size);

            latitude            = (double*)malloc(size*sizeof(double));
            longitude           = (double*)malloc(size*sizeof(double));
            windSpeedAt10M      = (double*)malloc(size*sizeof(double));
            windDirectionAt10M  = (double*)malloc(size*sizeof(double));
            indexOfSelectedWindVector = (double*)malloc(size*sizeof(double));

            /* get the values*/
            codes_get_double_array(h, "latitude", latitude, &size);
            codes_get_double_array(h, "longitude", longitude, &size);
            codes_get_double_array(h, "windSpeedAt10M", windSpeedAt10M, &size);
            codes_get_double_array(h, "windDirectionAt10M", windDirectionAt10M, &size);
            codes_get_double_array(h, "indexOfSelectedWindVector", indexOfSelectedWindVector, &size);

            for (i = 0; i < size; i++) {
              if (windDirectionAt10M[i] < var.missing || windSpeedAt10M[i] < var.missing) continue;
              if (latitude[i] < var.lat1 || latitude[i] > var.lat2 || longitude[i] < var.lon1 || longitude[i] > var.lon2) continue;
              if (indexOfSelectedWindVector[i] == 2) continue;

              lon = longitude[i]; lat = latitude[i];
              if (!strcmp(var.map, "NHEM"))       sterproj_ellp(&lon, &lat, &xs, &ys, 0, &map2);
              else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon, &lat, &xs, &ys, 0, &map3);
              else lamcproj_ellp(&lon, &lat, &xs, &ys, 0, &map);
              if (xs < 0 || xs > var.NI || ys < 0 || ys > var.NJ) continue;

              // 이미지에서 격자 위치
              ii = (int)(xs);
              jj = (int)(ys);

              plot = 1;
              for (bj = jj-box; bj < jj+box; bj++) {
                if (bj < 0 || bj >= ny_box) continue;
                for (bi = ii-box; bi < ii+box; bi++) {
                  if (bi < 0 || bi >= nx_box) continue;          
                  if (boxs[bj][bi] != 0) {
                    plot = 0;
                    break;
                  }
                }
              }

              if (plot == 1) {
                if (windSpeedAt10M[i] >= 15) {
                  ascat_wd(im, xs + var.OI, var.NJ - ys + var.OJ, windDirectionAt10M[i], windSpeedAt10M[i], lon, color1);
                }
                else {
                  ascat_wd(im, xs + var.OI, var.NJ - ys + var.OJ, windDirectionAt10M[i], windSpeedAt10M[i], lon, color_lvl[231]);
                }

                for (bj = jj-box; bj < jj+box; bj++) {
                  if (bj < 0 || bj >= ny_box) continue;
                  for (bi = ii-box; bi < ii+box; bi++) {
                    if (bi < 0 || bi >= nx_box) continue;          
                    boxs[bj][bi]++;
                  }
                }
              }
            }

            /* free allocated arrays */
            free(latitude);
            free(longitude);
            free(windSpeedAt10M);
            free(windDirectionAt10M);
            free(indexOfSelectedWindVector);

            /* delete handle */
            codes_handle_delete(h);
          }

          fclose(in);
        }
      }
    }
  }

  // HY 해상풍 자료
  sprintf(dirname, "%s/%04d%02d/%02d/", HY2_DIR, YY, MM, DD);
  sprintf(filetime, "%04d%02d%02d_%02d", YY, MM, DD, HH);

  if ((dp=opendir(dirname)) != NULL) {
    while ((dirp=readdir(dp)) != NULL) {
      if (strstr(dirp->d_name, "_250_") && strstr(dirp->d_name, filetime) && !strstr(dirp->d_name, "nc.gz")) {
        sprintf(fname, "%s%s", dirname, dirp->d_name);
        //printf("%s\n", fname);

        // 1. 파일 읽기
        code = nc_open(fname, NC_NOWRITE, &nc_id);
        if (code != NC_NOERR) return -1;

        // 2. 특성 정보
        // 2.1. 격자 정보
        nc_inq_dimid(nc_id, "NUMROWS", &dim_id);
        nc_inq_dimlen(nc_id, dim_id, &nx);
        nc_inq_dimid(nc_id, "NUMCELLS", &dim_id);
        nc_inq_dimlen(nc_id, dim_id, &ny);

        lats = matrix(0, ny, 0, nx);
        lons = matrix(0, ny, 0, nx);
        g1 = matrix(0, ny, 0, nx);
        g2 = matrix(0, ny, 0, nx);

        // 3. 자료 읽기
        // 3.1. 위도 읽기
        nc_inq_varid(nc_id, "lat", &var_id);
        status = nc_get_att_double(nc_id, var_id, "add_offset", &offset);
        status = nc_get_att_double(nc_id, var_id, "scale_factor", &scale_factor);
        status = nc_get_att_int(nc_id, var_id, "missing_value", &imv);

        p = (int *) malloc( sizeof(int) * nx * ny );
        nc_get_var_int(nc_id, var_id, p);

        for (tp = p, j = 0; j < ny; j++) {
          for (dd = lats[j], i = 0; i < nx; i++, dd++, tp++) {
            if (*tp == imv) {
              *dd = var.missing;
            }
            else {
              *dd = (*tp * scale_factor) + offset;
            }
          }
        }
        free(p);

        // 3.2. 경도 읽기
        nc_inq_varid(nc_id, "lon", &var_id);
        status = nc_get_att_double(nc_id, var_id, "add_offset", &offset);
        status = nc_get_att_double(nc_id, var_id, "scale_factor", &scale_factor);
        status = nc_get_att_int(nc_id, var_id, "missing_value", &imv);

        p = (int *) malloc( sizeof(int) * nx * ny );
        nc_get_var_int(nc_id, var_id, p);

        for (tp = p, j = 0; j < ny; j++) {
          for (dd = lons[j], i = 0; i < nx; i++, dd++, tp++) {
            if (*tp == imv) {
              *dd = var.missing;
            }
            else {
              *dd = (*tp * scale_factor) + offset;
            }
          }
        }
        free(p);

        // 3.3. 풍속 읽기
        nc_inq_varid(nc_id, "wind_speed", &var_id);
        status = nc_get_att_double(nc_id, var_id, "add_offset", &offset);
        status = nc_get_att_double(nc_id, var_id, "scale_factor", &scale_factor);
        status = nc_get_att_short(nc_id, var_id, "missing_value", &smv);

        s = (short *) malloc( sizeof(short) * nx * ny );
        nc_get_var_short(nc_id, var_id, s);

        for (ts = s, j = 0; j < ny; j++) {
          for (dd = g1[j], i = 0; i < nx; i++, dd++, ts++) {
            if (*ts == smv) {
              *dd = var.missing;
            }
            else {
              *dd = (*ts * scale_factor) + offset;
            }
          }
        }
        free(s);

        // 3.4. 풍향 읽기
        nc_inq_varid(nc_id, "wind_dir", &var_id);
        status = nc_get_att_double(nc_id, var_id, "add_offset", &offset);
        status = nc_get_att_double(nc_id, var_id, "scale_factor", &scale_factor);
        status = nc_get_att_short(nc_id, var_id, "missing_value", &smv);

        s = (short *) malloc( sizeof(short) * nx * ny );
        nc_get_var_short(nc_id, var_id, s);

        for (ts = s, j = 0; j < ny; j++) {
          for (dd = g2[j], i = 0; i < nx; i++, dd++, ts++) {
            if (*ts == smv) {
              *dd = var.missing;
            }
            else {
              *dd = ((*ts * scale_factor) + offset) - 180;
            }
          }
        }
        free(s);

        // 표출
        for (j = 0; j < ny; j++) {
          for (i = 0; i < nx; i++) {
            if (lats[j][i] == var.missing || lons[j][i] == var.missing || g1[j][i] == var.missing || g2[j][i] == var.missing) continue;
            if (lats[j][i] < var.lat1 || lats[j][i] > var.lat2 || lons[j][i] < var.lon1 || lons[j][i] > var.lon2) continue;
            //if (indexOfSelectedWindVector[i] == 2) continue;      

            lon = lons[j][i]; lat = lats[j][i];
            if (!strcmp(var.map, "NHEM"))       sterproj_ellp(&lon, &lat, &xs, &ys, 0, &map2);
            else if (!strcmp(var.map, "WORLD")) eqdcproj(&lon, &lat, &xs, &ys, 0, &map3);
            else lamcproj_ellp(&lon, &lat, &xs, &ys, 0, &map);
            if (xs < 0 || xs > var.NI || ys < 0 || ys > var.NJ) continue;

            //printf("%f %f %f %f\n", lon, lat, g1[j][i], g2[j][i]);

            // 이미지에서 격자 위치
            ii = (int)(xs);
            jj = (int)(ys);

            plot = 1;
            for (bj = jj-box; bj < jj+box; bj++) {
              if (bj < 0 || bj >= ny_box) continue;
              for (bi = ii-box; bi < ii+box; bi++) {
                if (bi < 0 || bi >= nx_box) continue;          
                if (boxs[bj][bi] != 0) {
                  plot = 0;
                  break;
                }
              }
            }

            if (plot == 1) {
              if (g1[j][i] >= 15) {
                ascat_wd(im, xs + var.OI, var.NJ - ys + var.OJ, g2[j][i], g1[j][i], lon, color1);
              }
              else {
                ascat_wd(im, xs + var.OI, var.NJ - ys + var.OJ, g2[j][i], g1[j][i], lon, color_lvl[231]);
              }

              for (bj = jj-box; bj < jj+box; bj++) {
                if (bj < 0 || bj >= ny_box) continue;
                for (bi = ii-box; bi < ii+box; bi++) {
                  if (bi < 0 || bi >= nx_box) continue;          
                  boxs[bj][bi]++;
                }
              }
            }
          }
        }

        // 6. 배열 해제
        free_matrix(lats, 0, ny, 0, nx);
        free_matrix(lons, 0, ny, 0, nx);
        free_matrix(g1, 0, ny, 0, nx);
        free_matrix(g2, 0, ny, 0, nx);
      }
    }
  }


  return 0;
}