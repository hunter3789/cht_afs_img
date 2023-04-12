/*******************************************************************************
**
**  gts 전문 표출
**
**=============================================================================*
**
**     o 작성자 : 이정환
**     o 변경자 : 이창재(2020.04.14. php->c로 변환)
**
********************************************************************************/

/*******************************************************************************
 *  ca : 운량(1/8)
 *******************************************************************************/
int gts_sfc_ca(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  char tp[8],
  int ca, 
  int color_fg,
  int color_bg
)
{ 
  int    wh, wh2;
  int    xx;
  int    font_width;
  char   txt[128], txt_utf[128];
  int    brect[8];
  int    i;
  double font_size = 7.0;

  wh = 11;
  wh2 = wh/2;

  sprintf(txt, "%s", tp);
  for (i = 0; i < 128; i++) txt_utf[i] = 0;
  euckr2utf(txt, txt_utf);

  if (!strcmp(tp,"B"))
  {
    if (var.border > 0) {
      gdImageStringFT(im, &brect[0], color_bg, FONTTTF, font_size, 0.0, xs-2-2, ys-6+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, FONTTTF, font_size, 0.0, xs-2-0, ys-6+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, FONTTTF, font_size, 0.0, xs-2-1, ys-6+9, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, FONTTTF, font_size, 0.0, xs-2-1, ys-6+11, txt_utf);
    }
    gdImageStringFT(im, &brect[0], color_fg, FONTTTF, font_size, 0.0, xs-2-1, ys-6+10, txt_utf);
  }
  else if (!strcmp(tp,"H"))
  {
    if (var.border > 0) {
      gdImageStringFT(im, &brect[0], color_bg, FONTTTF, font_size, 0.0, xs-2-2, ys-6+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, FONTTTF, font_size, 0.0, xs-2-0, ys-6+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, FONTTTF, font_size, 0.0, xs-2-1, ys-6+9, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, FONTTTF, font_size, 0.0, xs-2-1, ys-6+11, txt_utf);
    }
    gdImageStringFT(im, &brect[0], color_fg, FONTTTF, font_size, 0.0, xs-2-1, ys-6+10, txt_utf);
  }
  else if (!strcmp(tp,"K"))
  {
    if (var.border > 0) {
      gdImageStringFT(im, &brect[0], color_bg, FONTTTF, font_size, 0.0, xs-2-2, ys-6+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, FONTTTF, font_size, 0.0, xs-2-0, ys-6+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, FONTTTF, font_size, 0.0, xs-2-1, ys-6+9, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, FONTTTF, font_size, 0.0, xs-2-1, ys-6+11, txt_utf);
    }
    gdImageStringFT(im, &brect[0], color_fg, FONTTTF, font_size, 0.0, xs-2-1, ys-6+10, txt_utf);
  }
  else
  {
    switch (ca)
    {
      case 0:
        break;
      case 1:
        gdImageLine(im, xs, ys-wh2, xs, ys+wh2, color_fg);
        break;
      case 2:
        gdImageFilledArc(im, xs, ys, wh, wh, 270, 360, color_fg, gdArc);
        break;
      case 3:
        gdImageFilledArc(im, xs, ys, wh, wh, 270, 360, color_fg, gdArc);
        gdImageLine(im, xs, ys-wh2, xs, ys+wh2, color_fg);
        break;
      case 4: 
        gdImageFilledArc(im, xs, ys, wh, wh, 270, 450, color_fg, gdArc);
        break;
      case 5: 
        gdImageFilledArc(im, xs, ys, wh, wh, 270, 450, color_fg, gdArc);
        gdImageLine(im, xs-wh2, ys, xs+wh2, ys, color_fg);
        break;
      case 6:
        gdImageFilledArc(im, xs, ys, wh, wh, 270, 540, color_fg, gdArc);
        break;
      case 7: 
        gdImageFilledArc(im, xs, ys, wh, wh, 0, 360, color_fg, gdArc);
        gdImageLine(im, xs, ys-wh2, xs, ys+wh2, color_fg);
        gdImageLine(im, xs-1, ys-wh2, xs-1, ys+wh2, color_fg);
        break;
      case 8: 
        gdImageFilledArc(im, xs, ys, wh, wh, 0, 360, color_fg, gdArc);
        break;
      default:
        gdImageLine(im, xs-3, ys-3, xs+3, ys+3, color_fg);
        gdImageLine(im, xs-3, ys+3, xs+3, ys-3, color_fg);
        break;
    }
  }
  if (var.border > 0) {
    gdImageArc(im, xs-1, ys, wh, wh, 0, 360, color_bg);
    gdImageArc(im, xs+1, ys, wh, wh, 0, 360, color_bg);
    gdImageArc(im, xs, ys-1, wh, wh, 0, 360, color_bg);
    gdImageArc(im, xs, ys+1, wh, wh, 0, 360, color_bg);
  }
  gdImageArc(im, xs, ys, wh, wh, 0, 360, color_fg);

  return;
}

/*******************************************************************************
 *  wd : 풍향(degree), ws : 풍속 (m/s)
 *******************************************************************************/
int gts_sfc_wd(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  float wd, 
  float ws,
  int color,
  int color_bg,
  int zoom_level
)
{
  float WR = 35.0, WR2 = 26.25;
  float wr_s, wd_s;
  int x1, y1, x2, y2, x3, y3;
  gdPoint xy[3], xy2[3], xy3[3], xy4[3], xy5[3];
  char   txt[128], txt_utf[128];
  double font_size = 8.0;
  int    brect[8];
  int    i;

  font_size *= var.font_size;
  if (wd > 360) wd -= 360;
  if (wd < 0 || wd > 360) return;
  if (ws < 0 || ws > 100) return;

  // 풍향
  wd *= DEGRAD;

  x1 = xs + (int)(6.0*sin(wd) + 0.5);
  y1 = ys - (int)(6.0*cos(wd) + 0.5);
  if (zoom_level >= 2) {
    x2 = xs + (int)(WR*sin(wd) + 0.5);
    y2 = ys - (int)(WR*cos(wd) + 0.5);
  }
  else {
    x2 = xs + (int)(WR2*sin(wd) + 0.5);
    y2 = ys - (int)(WR2*cos(wd) + 0.5);
  }
  if (ws > 0.2) {
    if (var.border > 0) {
      gdImageLine(im, x1, y1+1, x2, y2+1, color_bg);
      gdImageLine(im, x1, y1-1, x2, y2-1, color_bg);
      gdImageLine(im, x1+1, y1, x2+1, y2, color_bg);
      gdImageLine(im, x1-1, y1, x2-1, y2, color_bg);
    }
    gdImageLine(im, x1, y1, x2, y2, color);
  }

  // 풍속깃
  while (ws > 0.0)
  {
    if (zoom_level >= 2) {
      if (ws < 5)
        wr_s = 4.0*ws;
      else
        wr_s = 20.0;
    }
    else {
      if (ws < 5)
        wr_s = 3.0*ws;
      else
        wr_s = 15.0;
    }

    wd_s = wd + 60.0*DEGRAD;
    x1 = x2 + (int)(wr_s*sin(wd_s) + 0.5);
    y1 = y2 - (int)(wr_s*cos(wd_s) + 0.5);

    if (ws >= 25) {
      if (zoom_level >= 2) {
        WR -= 5.0;
        x3 = xs + (int)(WR*sin(wd) + 0.5);
        y3 = ys - (int)(WR*cos(wd) + 0.5);
      }
      else {
        WR2 -= 3.75;
        x3 = xs + (int)(WR2*sin(wd) + 0.5);
        y3 = ys - (int)(WR2*cos(wd) + 0.5);
      }

      xy[0].x = x1; xy2[0].x = x1;   xy3[0].x = x1;   xy4[0].x = x1-1; xy5[0].x = x1+1;
      xy[0].y = y1; xy2[0].y = y1-1; xy3[0].y = y1+1; xy4[0].y = y1;   xy5[0].y = y1;
      xy[1].x = x2; xy2[1].x = x2;   xy3[1].x = x2;   xy4[1].x = x2-1; xy5[1].x = x2+1;
      xy[1].y = y2; xy2[1].y = y2-1; xy3[1].y = y2+1; xy4[1].y = y2;   xy5[1].y = y2;
      xy[2].x = x3; xy2[2].x = x3;   xy3[2].x = x3;   xy4[2].x = x3-1; xy5[2].x = x3+1;
      xy[2].y = y3; xy2[2].y = y3-1; xy3[2].y = y3+1; xy4[2].y = y3;   xy5[2].y = y3;
      if (var.border > 0) {
        gdImageFilledPolygon(im, xy2, 3, color_bg);
        gdImageFilledPolygon(im, xy3, 3, color_bg);
        gdImageFilledPolygon(im, xy4, 3, color_bg);
        gdImageFilledPolygon(im, xy5, 3, color_bg);
      }
      gdImageFilledPolygon(im, xy, 3, color);

      ws -= 25.0;
      x2 = x3;
      y2 = y3;
    }
    else {
      if (var.border > 0) {
        gdImageLine(im, x1, y1+1, x2, y2+1, color_bg);
        gdImageLine(im, x1, y1-1, x2, y2-1, color_bg);
        gdImageLine(im, x1+1, y1, x2+1, y2, color_bg);
        gdImageLine(im, x1-1, y1, x2-1, y2, color_bg);
      }
      gdImageLine(im, x1, y1, x2, y2, color);

      ws -= 5.0;
      if (zoom_level >= 2) {
        WR -= 5.0;
        x2 = xs + (int)(WR*sin(wd) + 0.5);
        y2 = ys - (int)(WR*cos(wd) + 0.5);
      }
      else {
        WR2 -= 3.75;
        x2 = xs + (int)(WR2*sin(wd) + 0.5);
        y2 = ys - (int)(WR2*cos(wd) + 0.5);
      }
    }
  }
  return;
}

/*******************************************************************************
 *  wd : 풍향(degree), ws : 풍속 (m/s)
 *******************************************************************************/
int gts_sfc_wd_arrow(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  float wd, 
  float ws,
  int color,
  int color_bg
)
{
  float WR = 35.0;
  float wr_s, wd_s;
  int x1, y1, x2, y2, x3, y3;
  gdPoint xy[3], xy2[3], xy3[3], xy4[3], xy5[3];
  float  theta, theta1, a, b, hs;
  float  Vs, As = 6.0;  /* 10m/sec로 조정시 *0.5 */

  if (wd > 360) wd -= 360;
  if (wd < 0 || wd > 360) return;
  if (ws < 0 || ws > 100) return;

  // 풍향
  if (ws < 2.5) Vs = 6.0;
  else if (ws < 5.0) Vs = 9.0;
  else if (ws < 7.5) Vs = 12.0;
  else Vs = 15.0;
  x1 = xs + (int)(-Vs*sin(wd*DEGRAD) + 0.5);
  y1 = ys - (int)(-Vs*cos(wd*DEGRAD) + 0.5);
  x2 = xs + (int)(Vs*sin(wd*DEGRAD) + 0.5);
  y2 = ys - (int)(Vs*cos(wd*DEGRAD) + 0.5);

  if (ws > 0.2) {
    if (var.border > 0) {
      gdImageLine(im, x1, y1+1, x2, y2+1, color_bg);
      gdImageLine(im, x1, y1-1, x2, y2-1, color_bg);
      gdImageLine(im, x1+1, y1, x2+1, y2, color_bg);
      gdImageLine(im, x1-1, y1, x2-1, y2, color_bg);
    }
    gdImageLine(im, x1, y1, x2, y2, color);
  }

  //화살촉
  if (ws > 0.2) {
    theta = wd + 90 + 60;
    a  = As * sin(theta*DEGRAD);
    b  = As * cos(theta*DEGRAD);
    x2 = x1 - (int)(a+0.5);
    y2 = y1 + (int)(b+0.5);

    theta = wd - 90 - 60;
    a  = As * sin(theta*DEGRAD);
    b  = As * cos(theta*DEGRAD);
    x3 = x1 - (int)(a+0.5);
    y3 = y1 + (int)(b+0.5);

    xy[0].x = x1; xy2[0].x = x1;   xy3[0].x = x1;   xy4[0].x = x1-1; xy5[0].x = x1+1;
    xy[0].y = y1; xy2[0].y = y1-1; xy3[0].y = y1+1; xy4[0].y = y1;   xy5[0].y = y1;
    xy[1].x = x2; xy2[1].x = x2;   xy3[1].x = x2;   xy4[1].x = x2-1; xy5[1].x = x2+1;
    xy[1].y = y2; xy2[1].y = y2-1; xy3[1].y = y2+1; xy4[1].y = y2;   xy5[1].y = y2;
    xy[2].x = x3; xy2[2].x = x3;   xy3[2].x = x3;   xy4[2].x = x3-1; xy5[2].x = x3+1;
    xy[2].y = y3; xy2[2].y = y3-1; xy3[2].y = y3+1; xy4[2].y = y3;   xy5[2].y = y3;
    if (var.border > 0) {
      gdImageFilledPolygon(im, xy2, 3, color_bg);
      gdImageFilledPolygon(im, xy3, 3, color_bg);
      gdImageFilledPolygon(im, xy4, 3, color_bg);
      gdImageFilledPolygon(im, xy5, 3, color_bg);
    }
    gdImageFilledPolygon(im, xy, 3, color);

  }

  return;
}

/*******************************************************************************
 *  cl : 하층운형, cm : 중층운형, ct : 상층운영
 *******************************************************************************/
int gts_sfc_ct(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  int cl, 
  int cm,
  int ct
)
{ 
  char gname[128];
  FILE  *fp;
  gdImagePtr in;
  int color;

  color = gdImageColorAllocate(im, 0, 0, 255);

  // 상층운형
  if (ct > 0 && ct < 10)
  {
    //gdImageFilledArc(im, xs, ys, 3, 3, 0, 360, color, gdArc);
    sprintf(gname, "/home/fct/www/ROOT/images/cloud/CH%02d.png", ct);
    fp = fopen(gname, "rb");
    if (fp)
    {
      in = gdImageCreateFromPng(fp);
      //gdImageCopy(im, in, xs-16, ys-15, 0, 0, 25, 25);
      gdImageCopy(im, in, xs-16, ys-45, 0, 0, 25, 25);
      fclose(fp);
      gdImageDestroy(in);
    }
  }

  // 중층운형
  if (cm > 0 && cm < 10)
  {
    sprintf(gname, "/home/fct/www/ROOT/images/cloud/CM%02d.png", cm);
    fp = fopen(gname, "rb");
    if (fp)
    {
      in = gdImageCreateFromPng(fp);
      //gdImageCopy(im, in, xs-16, ys, 0, 0, 25, 25);
      gdImageCopy(im, in, xs-16, ys-30, 0, 0, 25, 25);
      gdImageDestroy(in);
      fclose(fp);
    }
  }

  // 하층운형
  if (cl > 0 && cl < 10)
  {
    sprintf(gname, "/home/fct/www/ROOT/images/cloud/CL%02d.png", cl);
    fp = fopen(gname, "rb");
    if (fp)
    {
      in = gdImageCreateFromPng(fp);
      //gdImageCopy(im, in, xs-16, ys+25, 0, 0, 25, 25);
      gdImageCopy(im, in, xs-16, ys-5, 0, 0, 25, 25);
      fclose(fp);
      gdImageDestroy(in);
    }
  } 
  return;
}

/*******************************************************************************
 *  cd : 중하층운량
 *******************************************************************************/
int gts_sfc_cd(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  int cd, 
  double font_size,
  int color,
  int color_bg
)
{ 
  int    font_width;
  char   txt[128], txt_utf[128];
  int    brect[8];
  int    i;
  char   font_name[64];

  font_size *= var.font_size;
  font_width = font_size/2;

  if (var.font_size > 1) strcpy(font_name, FONTTTF1);
  else strcpy(font_name, FONTTTF);

  //gdImageFilledArc(im, xs, ys, 3, 3, 0, 360, color, gdArc);

  //  중하층운량
  if (cd >= 0)
  {
    sprintf(txt, "%d", cd);
    for (i = 0; i < 128; i++) txt_utf[i] = 0;
    euckr2utf(txt, txt_utf);
    if (var.border > 0) {
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+6+3, ys-2+19, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+6+5, ys-2+19, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+6+4, ys-2+18, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+6+4, ys-2+20, txt_utf);
    }
    gdImageStringFT(im, &brect[0], color, font_name, font_size, 0.0, xs+6+4, ys-2+19, txt_utf);
  }
  return;
}

/*******************************************************************************
 *  wc : 현재 일기, wp : 과거 일기
 *******************************************************************************/
int gts_sfc_ww(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  int wc, 
  int wp
)
{ 
  int w1, w2;
  char gname[128];
  FILE  *fp;
  gdImagePtr in;
  int color;

  color = gdImageColorAllocate(im, 0, 0, 255);

  // 현재 일기
  if (wc >= 4)
  {
    //gdImageFilledArc(im, xs, ys, 3, 3, 0, 360, color, gdArc);
    sprintf(gname, "/home/fct/www/ROOT/images/gts_ww/ww%02d.png", wc);
    fp = fopen(gname, "rb");
    if (fp)
    {
      in = gdImageCreateFromPng(fp);
      gdImageCopy(im, in, xs-27, ys-12, 0, 0, 25, 25);
      gdImageDestroy(in);
      fclose(fp);
    }
  }

  // 과거 일기
  if (wp < 10)
  {
    w1 = wp;
    w2 = w1;
  }
  else
  {
    w1 = wp/10;
    w2 = wp%10;
  }

  if (w1 >= 3 && w1 <= 9)
  {
    sprintf(gname, "/home/fct/www/ROOT/images/gts_ww/WW%d.png", w1);
    fp = fopen(gname, "rb");
    if (fp)
    {
      in = gdImageCreateFromPng(fp);
      gdImageCopy(im, in, xs+10, ys, 0, 0, 25, 25);
      gdImageDestroy(in);
      fclose(fp);
    }
  }

  if (w2 != w1 && (w2 >= 3 && w2 <= 9))
  {
    sprintf(gname, "/home/fct/www/ROOT/images/gts_ww/WW%d.png", w2);
    fp = fopen(gname, "rb");
    if (fp)
    {
      in = gdImageCreateFromPng(fp);
      gdImageCopy(im, in, xs+20, ys, 0, 0, 25, 25);
      gdImageDestroy(in);
      fclose(fp);
    }
  } 
  return;
}


/*******************************************************************************
 *  기호표시 (현재일기('W'), 상층운형('T'), 중층운형('M'), 하층운형('L'))
 *******************************************************************************/
int gts_sfc_wc(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  int vv, 
  char mode
)
{ 
  int code;
  char gname[128];
  FILE  *fp;
  gdImagePtr in;
  int color;

  code = 0;

  if (mode == 'W') {
    if (vv >= 4)
    {
      sprintf(gname, "/home/fct/www/ROOT/images/gts_ww/ww%02d.png", vv);
      code = 1;
    }
  }
  else if (mode == 'T') {
    if (vv > 0 && vv < 10)
    {
      sprintf(gname, "/home/fct/www/ROOT/images/cloud/CH%02d.png", vv);
      code = 1;
    }
  }
  else if (mode == 'M') {
    if (vv > 0 && vv < 10)
    {
      sprintf(gname, "/home/fct/www/ROOT/images/cloud/CM%02d.png", vv);
      code = 1;
    }
  }
  else if (mode == 'L') {
    if (vv > 0 && vv < 10)
    {
      sprintf(gname, "/home/fct/www/ROOT/images/cloud/CL%02d.png", vv);
      code = 1;
    }
  }

  if (code)
  {
    fp = fopen(gname, "rb");
    if (fp)
    {
      in = gdImageCreateFromPng(fp);
      gdImageCopy(im, in, xs-15, ys-15, 0, 0, 25, 25);
      gdImageDestroy(in);
      fclose(fp);
    }
  }
  return;
}

/*******************************************************************************
 *  ta : 기온(C),  td : 노점온도(C)
 *******************************************************************************/
int gts_sfc_ta(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  float ta, 
  float td, 
  double font_size,
  int color,
  int color_bg
)
{ 
  int    xx;
  int    font_width;
  char   txt[128], txt_utf[128];
  int    brect[8];
  int    i;
  char   font_name[64];

  font_size *= var.font_size;
  font_width = font_size/2;

  if (var.font_size > 1) strcpy(font_name, FONTTTF1);
  else strcpy(font_name, FONTTTF);

  //  기온
  if (ta > -50)
  {
    sprintf(txt, "%.1f", ta);
    for (i = 0; i < 128; i++) txt_utf[i] = 0;
    euckr2utf(txt, txt_utf);
    xx = xs - strlen(txt)*font_width - 10;
    if (var.border > 0) {
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx-5, ys-20+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx-3, ys-20+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx-4, ys-20+9, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx-4, ys-20+11, txt_utf);
    }
    gdImageStringFT(im, &brect[0], color, font_name, font_size, 0.0, xx-4, ys-20+10, txt_utf);
  }

  // 이슬점온도
  if (td > -50 && ta > -50)
  {
    sprintf(txt, "%.1f", td);
    for (i = 0; i < 128; i++) txt_utf[i] = 0;
    euckr2utf(txt, txt_utf);
    xx = xs - strlen(txt)*font_width - 10;
    if (var.border > 0) {
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx-5, ys+7+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx-3, ys+7+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx-4, ys+7+9, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx-4, ys+7+11, txt_utf);
    }
    gdImageStringFT(im, &brect[0], color, font_name, font_size, 0.0, xx-4, ys+7+10, txt_utf);
  } 
  return;
}

/*******************************************************************************
 *  ps : 해면기압(hPa),  pr : 기압변화량(hpa),  pt : 기압변화경향
 *******************************************************************************/
int gts_sfc_ps(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  float ps, 
  float pr, 
  int pt,
  double font_size,
  int color,
  int color_pt,
  int color_bg
)
{ 
  int    font_width;
  char   txt[128], txt_utf[128];
  int    brect[8];
  int    i, len;
  char   font_name[64];

  font_size *= var.font_size;
  font_width = font_size/2;

  if (var.font_size > 1) strcpy(font_name, FONTTTF1);
  else strcpy(font_name, FONTTTF);

  // 해면기압
  if (ps >= 0)
  {
    sprintf(txt, "%.1f", ps);
    for (i = 0; i < 128; i++) txt_utf[i] = 0;
    euckr2utf(txt, txt_utf);
    if (var.border > 0) {
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+8, ys-20+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+10, ys-20+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+9, ys-20+9, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+9, ys-20+11, txt_utf);
    }
    gdImageStringFT(im, &brect[0], color, font_name, font_size, 0.0, xs+9, ys-20+10, txt_utf);
  }

  // 기압변화량
  if (pr >= 0)
  {
    if      (pt <= 3) sprintf(txt, "+%.1f", pr);
    else if (pt == 4) sprintf(txt, " %.1f", pr);
    else if (pt <= 8) sprintf(txt, "-%.1f", pr);
    else              sprintf(txt, " ");

    for (i = 0; i < 128; i++) txt_utf[i] = 0;
    euckr2utf(txt, txt_utf);
    if (var.border > 0) {
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+8, ys-7+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+10, ys-7+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+9, ys-7+9, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+9, ys-7+11, txt_utf);
    }
    gdImageStringFT(im, &brect[0], color, font_name, font_size, 0.0, xs+9, ys-7+10, txt_utf);
  } 

  // 기압변화경향
  if (pt >= 0)
  {
    len = 15 + font_width*strlen(txt);
    xs += len;

    switch (pt)
    {
      case 0:
        gdImageLine(im, xs-3+6, ys+5, xs+3+6, ys-5, color_pt);
        gdImageLine(im, xs+3+6, ys-5, xs+5+6, ys-1, color_pt);
        break;
      case 1:
        gdImageLine(im, xs-3+6, ys+5, xs+2+6, ys-5, color_pt);
        gdImageLine(im, xs+2+6, ys-5, xs+6+6, ys-5, color_pt);
        break;
      case 2:
        gdImageLine(im, xs-3+6, ys+5, xs+4+6, ys-5, color_pt);
        break;
      case 3:
        gdImageLine(im, xs-3+6, ys+1, xs-1+6, ys+5, color_pt);
        gdImageLine(im, xs-1+6, ys+5, xs+5+6, ys-5, color_pt);
        break;
      case 4:
        gdImageLine(im, xs-4+6, ys, xs+4+6, ys, color);
        break;
      case 5:
        gdImageLine(im, xs-3+6, ys-5, xs+3+6, ys+5, color_pt);
        gdImageLine(im, xs+3+6, ys+5, xs+5+6, ys+1, color_pt);
        break;
      case 6:
        gdImageLine(im, xs-3+6, ys-5, xs+2+6, ys+5, color_pt);
        gdImageLine(im, xs+2+6, ys+5, xs+6+6, ys+5, color_pt);
        break;
      case 7:
        gdImageLine(im, xs-3+6, ys-5, xs+4+6, ys+5, color_pt);
        break;
      case 8:
        gdImageLine(im, xs-3+6, ys-1, xs-1+6, ys-5, color_pt);
        gdImageLine(im, xs-1+6, ys-5, xs+5+6, ys+5, color_pt);
        break;
    }
    xs -= len;
  } 
  return;
}

/*******************************************************************************
 *  rn : 강수량(mm),  rh : 강수누적시간(6Hr)
 *******************************************************************************/
int gts_sfc_rn(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  float rn, 
  int rh,
  double font_size,
  int color,
  int color_bg
)
{ 
  int    font_width;
  char   txt[128], txt_utf[128];
  int    brect[8];
  int    i;
  char   font_name[64];

  font_size *= var.font_size;
  font_width = font_size/2;

  if (var.font_size > 1) strcpy(font_name, FONTTTF1);
  else strcpy(font_name, FONTTTF);

  if (rn >= 0)
  {
    if (rh > 0) sprintf(txt, "%.1f/%d", rn, rh);
    else        sprintf(txt, "%.1f", rn);

    for (i = 0; i < 128; i++) txt_utf[i] = 0;
    euckr2utf(txt, txt_utf);
    if (var.border > 0) {
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+8, ys+20+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+10, ys+20+10, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+9, ys+20+9, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+9, ys+20+11, txt_utf);
    }
    gdImageStringFT(im, &brect[0], color, font_name, font_size, 0.0, xs+9, ys+20+10, txt_utf);
  }
  return;
}


/*******************************************************************************
 *  gh : 고도(km)
 *******************************************************************************/
int gts_upp_gh(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  float gh, 
  double font_size,
  int color,
  int color_bg
)
{
  int    font_width;
  char   txt[128], txt_utf[128];
  int    brect[8];
  int    i;
  char   font_name[64];

  font_size *= var.font_size;
  font_width = font_size/2;

  if (var.font_size > 1) strcpy(font_name, FONTTTF1);
  else strcpy(font_name, FONTTTF);

  // 고도
  if (gh >= 0)
  {
    sprintf(txt, "%d", (int)gh);
    for (i = 0; i < 128; i++) txt_utf[i] = 0;
    euckr2utf(txt, txt_utf);
    if (var.border > 0) {
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+5, ys-2, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+7, ys-2, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+6, ys-3, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xs+6, ys-1, txt_utf);
    }
    gdImageStringFT(im, &brect[0], color, font_name, font_size, 0.0, xs+6, ys-2, txt_utf);
  }
  return;
}

/*******************************************************************************
 *  ta : 기온(C),  td : 노점온도(C)
 *******************************************************************************/
int gts_upp_ta(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  float ta, 
  float td,
  double font_size,
  int color,
  int color_bg
)
{
  int    xx;
  int    font_width;
  char   txt[128], txt_utf[128];
  int    brect[8];
  int    i;
  char   font_name[64];

  font_size *= var.font_size;
  font_width = font_size/2;

  if (var.font_size > 1) strcpy(font_name, FONTTTF1);
  else strcpy(font_name, FONTTTF);

  //  기온
  if (ta > -90)
  {
    sprintf(txt, "%.1f", ta);
    for (i = 0; i < 128; i++) txt_utf[i] = 0;
    euckr2utf(txt, txt_utf);
    xx = xs - strlen(txt)*font_width - 10;
    if (var.border > 0) {
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx-1, ys-2, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx+1, ys-2, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx, ys-3, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx, ys-1, txt_utf);
    }
    gdImageStringFT(im, &brect[0], color, font_name, font_size, 0.0, xx, ys-2, txt_utf);
  }

  // 이슬점온도
  if (td > -90 && ta > -90)
  {
    sprintf(txt, "%.1f", ta-td);
    for (i = 0; i < 128; i++) txt_utf[i] = 0;
    euckr2utf(txt, txt_utf);
    xx = xs - strlen(txt)*font_width - 10;
    if (var.border > 0) {
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx-1, ys+8+4, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx+1, ys+8+4, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx, ys+8+3, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx, ys+8+5, txt_utf);
    }
    gdImageStringFT(im, &brect[0], color, font_name, font_size, 0.0, xx, ys+8+4, txt_utf);
  } 
  return;
}

/*******************************************************************************
 *  wd : 풍향(degree), ws : 풍속 (m/s)
 *******************************************************************************/
int gts_upp_wd(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  float wd, 
  float ws,
  int color_wd,
  int color_bg,
  int zoom_level
)
{
  float WR = 32.0, WR2 = 24.0;
  float wr_s, wd_s;
  int x1, y1, x2, y2, x3, y3;
  gdPoint xy[3], xy2[3], xy3[3], xy4[3], xy5[3];
  char   txt[128], txt_utf[128];
  double font_size = 8.0;
  int    brect[8];
  int    i;

  font_size *= var.font_size;
  //if (var.border > 0) gdImageFilledArc(im, xs, ys, 5, 5, 0, 360, color_bg, gdArc);
  //gdImageFilledArc(im, xs, ys, 3, 3, 0, 360, color_wd, gdArc);

  if (wd > 360) wd -= 360;
  if (wd < 0 || wd > 360) return;
  if (ws < 0 || ws > 100) return;

  // 풍향
  wd *= DEGRAD;

  x1 = xs;
  y1 = ys;
  if (zoom_level >= 2) {
    x2 = xs + (int)(WR*sin(wd) + 0.5);
    y2 = ys - (int)(WR*cos(wd) + 0.5);
  }
  else {
    x2 = xs + (int)(WR2*sin(wd) + 0.5);
    y2 = ys - (int)(WR2*cos(wd) + 0.5);
  }
  if (ws > 0.2) {
    if (var.border > 0) {
      gdImageLine(im, x1, y1+1, x2, y2+1, color_bg);
      gdImageLine(im, x1, y1-1, x2, y2-1, color_bg);
      gdImageLine(im, x1+1, y1, x2+1, y2, color_bg);
      gdImageLine(im, x1-1, y1, x2-1, y2, color_bg);
    }
    gdImageLine(im, x1, y1, x2, y2, color_wd);
  }

  // 풍속깃
  while (ws > 0.0)
  {
    if (zoom_level >= 2) {
      if (ws < 5)
        wr_s = 4.0*ws;
      else
        wr_s = 20.0;
    }
    else {
      if (ws < 5)
        wr_s = 3.0*ws;
      else
        wr_s = 15.0;
    }

    wd_s = wd + 60.0*DEGRAD;
    x1 = x2 + (int)(wr_s*sin(wd_s) + 0.5);
    y1 = y2 - (int)(wr_s*cos(wd_s) + 0.5);

    if (ws >= 25) {
      if (zoom_level >= 2) {
        WR -= 5.0;
        x3 = xs + (int)(WR*sin(wd) + 0.5);
        y3 = ys - (int)(WR*cos(wd) + 0.5);
      }
      else {
        WR2 -= 3.75;
        x3 = xs + (int)(WR2*sin(wd) + 0.5);
        y3 = ys - (int)(WR2*cos(wd) + 0.5);
      }

      xy[0].x = x1; xy2[0].x = x1;   xy3[0].x = x1;   xy4[0].x = x1-1; xy5[0].x = x1+1;
      xy[0].y = y1; xy2[0].y = y1-1; xy3[0].y = y1+1; xy4[0].y = y1;   xy5[0].y = y1;
      xy[1].x = x2; xy2[1].x = x2;   xy3[1].x = x2;   xy4[1].x = x2-1; xy5[1].x = x2+1;
      xy[1].y = y2; xy2[1].y = y2-1; xy3[1].y = y2+1; xy4[1].y = y2;   xy5[1].y = y2;
      xy[2].x = x3; xy2[2].x = x3;   xy3[2].x = x3;   xy4[2].x = x3-1; xy5[2].x = x3+1;
      xy[2].y = y3; xy2[2].y = y3-1; xy3[2].y = y3+1; xy4[2].y = y3;   xy5[2].y = y3;
      if (var.border > 0) {
        gdImageFilledPolygon(im, xy2, 3, color_bg);
        gdImageFilledPolygon(im, xy3, 3, color_bg);
        gdImageFilledPolygon(im, xy4, 3, color_bg);
        gdImageFilledPolygon(im, xy5, 3, color_bg);
      }
      gdImageFilledPolygon(im, xy, 3, color_wd);

      ws -= 25.0;
      x2 = x3;
      y2 = y3;
    }
    else {
      if (var.border > 0) {
        gdImageLine(im, x1, y1+1, x2, y2+1, color_bg);
        gdImageLine(im, x1, y1-1, x2, y2-1, color_bg);
        gdImageLine(im, x1+1, y1, x2+1, y2, color_bg);
        gdImageLine(im, x1-1, y1, x2-1, y2, color_bg);
      }
      gdImageLine(im, x1, y1, x2, y2, color_wd);
      ws -= 5.0;
      if (zoom_level >= 2) {
        WR -= 5.0;
        x2 = xs + (int)(WR*sin(wd) + 0.5);
        y2 = ys - (int)(WR*cos(wd) + 0.5);
      }
      else {
        WR2 -= 3.75;
        x2 = xs + (int)(WR2*sin(wd) + 0.5);
        y2 = ys - (int)(WR2*cos(wd) + 0.5);
      }
    }
  }

  gdImageFilledArc(im, xs, ys, 3, 3, 0, 360, color_wd, gdArc);
  return;
}

/*******************************************************************************
 *  한개의 값을 기록
 *******************************************************************************/
int gts_txt(
  gdImagePtr im,    // 이미지영역
  int xs,  
  int ys,  
  char txt[128],
  double font_size,
  int color_fg,
  int color_pt,
  int color_bg
)
{
  int    xx;
  int    font_width;
  char   txt_utf[128];
  int    brect[8];
  int    i;
  char   font_name[64];

  font_size *= var.font_size;
  font_width = font_size/2;

  if (var.font_size > 1) strcpy(font_name, FONTTTF1);
  else strcpy(font_name, FONTTTF);

  gdImageFilledArc(im, xs, ys, 3, 3, 0, 360, color_pt, gdArc);

  if (strlen(txt) > 0)
  {
    xx = xs - strlen(txt)*font_width*0.75;
    for (i = 0; i < 128; i++) txt_utf[i] = 0;
    euckr2utf(txt, txt_utf);
    if (var.border > 0) {
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx-1, ys-4, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx+1, ys-4, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx, ys-3, txt_utf);
      gdImageStringFT(im, &brect[0], color_bg, font_name, font_size, 0.0, xx, ys-5, txt_utf);
    }
    gdImageStringFT(im, &brect[0], color_fg, font_name, font_size, 0.0, xx, ys-4, txt_utf);
  }
  return;
}