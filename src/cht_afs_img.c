/*******************************************************************************
**
**  통합기상분석 API 중첩
**
**=============================================================================*
**
**     o 작성자 : 이창재 (2021.5.9)
**
********************************************************************************/

char* str(int size) {
    int i;

    char* string = (char*)malloc(sizeof(char) * size);

    for (i = 0; i < size; i++)
        string[i] = '\0';

    return string;
}

char** split(char *sentence, char separator, int *num_tokens) {
    char** tokens;
    int tokens_idx = 0;
    int token_idx = 0, i, j;
    *num_tokens = 1;

    for (i = 0; i < strlen(sentence); i++) {
        if (sentence[i] == separator)
            (*num_tokens)++;
    }

    tokens = (char**)malloc(sizeof(char*) * (*num_tokens));

    for (i = 0; i < *num_tokens; i++) {
        tokens[i] = str(INIT_SIZE);
        getword (tokens[i], sentence, separator);
        if (tokens[i][0] == '\n') strcpy(tokens[i], "");
    }

    return tokens;
}

char* str_replace(char* search, char* replace, char* subject) {
    int i, j, k;
    
    int searchSize = strlen(search);
    int replaceSize = strlen(replace);
    int size = strlen(subject);

    char* ret;

    if (size < searchSize) {
      return subject;
    }

    if (!searchSize) {
        ret = malloc(size + 1);
        for (i = 0; i <= size; i++) {
            ret[i] = subject[i];
        }
        return ret;
    }
    
    int retAllocSize = (strlen(subject) + 1) * 2; // Allocation size of the return string.
    // let the allocation size be twice as that of the subject initially
    ret = malloc(retAllocSize);

    int bufferSize = 0; // Found characters buffer counter
    char* foundBuffer = malloc(searchSize); // Found character bugger
    
    for (i = 0, j = 0; i <= size; i++) {
        /**
         * Double the size of the allocated space if it's possible for us to surpass it
         **/
        if (retAllocSize <= j + replaceSize) {
            retAllocSize *= 2;
            ret = (char*) realloc(ret, retAllocSize);
        }
        /**
         * If there is a hit in characters of the substring, let's add it to the
         * character buffer
         **/
        else if (subject[i] == search[bufferSize]) {
            foundBuffer[bufferSize] = subject[i];
            bufferSize++;

            /**
             * If the found character's bugger's counter has reached the searched substring's
             * length, then there's a hit. Let's copy the replace substring's characters
             * onto the return string.
             **/
            if (bufferSize == searchSize) {
                bufferSize = 0;
                for (k = 0; k < replaceSize; k++) {
                    ret[j++] = replace[k];
                }
            }
        }
        /**
         * If the character is a miss, let's put everything back from the buffer
         * to the return string, and set the found character buffer counter to 0.
         **/
        else {
            for (k = 0; k < bufferSize; k++) {
                ret[j++] = foundBuffer[k];
            }
            bufferSize = 0;
            /**
             * Add the current character in the subject string to the return string.
             **/
            ret[j++] = subject[i];
        }
    }

    /**
     * Free memory
     **/
    free(foundBuffer);
    
    return ret;
}

char* get_month (int MM) {
  char* month;

  month = (char*)malloc(sizeof(char*) * (4));

  switch (MM)
  {
    case 1:
      strcpy(month, "JAN");
      break;
    case 2:
      strcpy(month, "FEB");
      break;
    case 3:
      strcpy(month, "MAR");
      break;
    case 4:
      strcpy(month, "APR");
      break;
    case 5:
      strcpy(month, "MAY");
      break;
    case 6:
      strcpy(month, "JUN");
      break;
    case 7:
      strcpy(month, "JUL");
      break;
    case 8:
      strcpy(month, "AUG");
      break;
    case 9:
      strcpy(month, "SEP");
      break;
    case 10:
      strcpy(month, "OCT");
      break;
    case 11:
      strcpy(month, "NOV");
      break;
    case 12:
      strcpy(month, "DEC");
      break;
  }

  return month;
}

static size_t donotprint(char *ptr, size_t size, size_t nmemb, void *userdata) {
  //printf("%zu, %zu, %s\n\n", size, nmemb, ptr);
  return 0;
}

#define floor2(exp) ((long) exp)
int gdImageCopyResampled2 (gdImagePtr dst,
              gdImagePtr src,
              int dstX, int dstY,
              int srcX, int srcY,
              int dstW, int dstH, int srcW, int srcH)
{
  int x, y;
  double sy1, sy2, sx1, sx2;
  if (!dst->trueColor)
    {
      gdImageCopyResized (dst, src, dstX, dstY, srcX, srcY, dstW, dstH,
              srcW, srcH);
      return;
    }
  for (y = dstY; (y < dstY + dstH); y++)
    {
      sy1 = ((double) y - (double) dstY) * (double) srcH / (double) dstH;
      sy2 = ((double) (y + 1) - (double) dstY) * (double) srcH /
    (double) dstH;
      for (x = dstX; (x < dstX + dstW); x++)
    {
      double sx, sy;
      double spixels = 0;
      double red = 0.0, green = 0.0, blue = 0.0, alpha = 0.0;
      sx1 = ((double) x - (double) dstX) * (double) srcW / dstW;
      sx2 = ((double) (x + 1) - (double) dstX) * (double) srcW / dstW;
      sy = sy1;
      do
        {
          double yportion;
          if (floor2 (sy) == floor2 (sy1))
        {
          yportion = 1.0 - (sy - floor2 (sy));
          if (yportion > sy2 - sy1)
            {
              yportion = sy2 - sy1;
            }
          sy = floor2 (sy);
        }
          else if (sy == floor2 (sy2))
        {
          yportion = sy2 - floor2 (sy2);
        }
          else
        {
          yportion = 1.0;
        }
          sx = sx1;
          do
        {
          double xportion;
          double pcontribution;
          int p;
          if (floor2 (sx) == floor2 (sx1))
            {
              xportion = 1.0 - (sx - floor2 (sx));
              if (xportion > sx2 - sx1)
            {
              xportion = sx2 - sx1;
            }
              sx = floor2 (sx);
            }
          else if (sx == floor2 (sx2))
            {
              xportion = sx2 - floor2 (sx2);
            }
          else
            {
              xportion = 1.0;
            }
          pcontribution = xportion * yportion;
          /* 2.08: previously srcX and srcY were ignored. 
             Andrew Pattison */
          p = gdImageGetTrueColorPixel (src,
                        (int) sx + srcX,
                        (int) sy + srcY);
          red += gdTrueColorGetRed (p) * pcontribution;
          green += gdTrueColorGetGreen (p) * pcontribution;
          blue += gdTrueColorGetBlue (p) * pcontribution;
          alpha += gdTrueColorGetAlpha (p) * pcontribution;
          spixels += xportion * yportion;
          sx += 1.0;
        }
          while (sx < sx2);
          sy += 1.0;
        }
      while (sy < sy2);
      if (spixels != 0.0)
        {
          red /= spixels;
          green /= spixels;
          blue /= spixels;
          alpha /= spixels;
        }
      /* Clamping to allow for rounding errors above */
      if (red > 255.0)
        {
          red = 255.0;
        }
      if (green > 255.0)
        {
          green = 255.0;
        }
      if (blue > 255.0)
        {
          blue = 255.0;
        }
      if (alpha > gdAlphaMax)
        {
          alpha = gdAlphaMax;
        }

      if (alpha > 120) {
        alpha = gdAlphaMax;
      }

      gdImageSetPixel (dst,
               x, y,
               gdTrueColorAlpha ((int) red,
                         (int) green,
                         (int) blue, (int) alpha));
    }
    }
}

int getMaxGph(char model[4], int pres) {
  URL_FILE *fr; 
  char   url[500], buf[1000], nwp[32];
  float  lon1, lon2, lat1, lat2;
  int    YY, MM, DD, HH, MI;
  int    YY1, MM1, DD1, HH1, MI1;
  int    max;

  lon1 = 105.;
  lon2 = 150.;
  lat1 = 10.;
  lat2 = 40.;

  if (!strcmp(model, "G")) {
    strcpy(nwp, "UM");
  }
  else if (!strcmp(model, "GK")) {
    strcpy(nwp, "KIMG");
  }
  else if (!strcmp(model, "E")) {
    strcpy(nwp, "ECMWF");
  }

  seq2time(var.seq,    &YY, &MM, &DD, &HH, &MI, 'm', 'n'); //UTC
  seq2time(var.seq_fc, &YY1, &MM1, &DD1, &HH1, &MI1, 'm', 'n'); //UTC

  if (!strcmp(var.cht_mode, "gts")) {
    seq2time((int)(var.seq/(3*60))*(3*60), &YY, &MM, &DD, &HH, &MI, 'm', 'n');
  }

  //sprintf(url, "%s/cgi-bin/url/nph-nwp_bias_gts_txt1?gts=%d&varn=%s&model=%s&tm=%04d%02d%02d%02d00&tm_fc=%04d%02d%02d%02d00&lon1=%f&lon2=%f&lat1=%f&lat2=%f&calc_max=1",
  //          host, 500, "GH", nwp, YY, MM, DD, HH, YY1, MM1, DD1, HH1, lon1, lon2, lat1, lat2);
  sprintf(url, "http://cht.kma.go.kr/cgi-bin/url/nph-nwp_bias_gts_txt1?gts=%d&varn=%s&model=%s&tm=%04d%02d%02d%02d00&tm_fc=%04d%02d%02d%02d00&lon1=%f&lon2=%f&lat1=%f&lat2=%f&calc_max=1",
            pres, "GH", nwp, YY, MM, DD, HH, YY1, MM1, DD1, HH1, lon1, lon2, lat1, lat2);

  if (fr = url_fopen(url, "r")) {
    while (!url_feof(fr)) {
      url_fgets(buf, sizeof(buf), fr);
      if (buf[0] == '#') continue;
      sscanf(buf, "%d", &max);
    }

    url_fclose(fr);
  }

  return max;
}

int cht_afs_disp (
  gdImagePtr im,
  int color_lvl[256]
)
{
  FILE      *fp, *fp2;
  URL_FILE  *fr;
  int       err = 0, ok = 0, k = 0, i, j, num_layer, zoom_level, zx, zy, save_ok;
  int       YY, MM, DD, HH, MI, SS;
  char      url[2048], area_afs[256], buf[1024], cht_afs[max_layer][2048], type[4], model[4], title[256], level[4], path[256], fname[512];
  char      **tokens, title2[256], title3[256], title4[256], title5[256], title6[256];

  // 통합기상분석 api 변수
  int       num_tokens[max_layer], zmlvl[max_layer], vrtc[max_layer], contourLineEmphLineGap[max_layer], contourLineTextGap[max_layer], symblSize[max_layer], symblGap[max_layer];
  int       strmlnDensity[max_layer], strmlnResol[max_layer], strmlnArrowGap[max_layer], strmlnArrowSize[max_layer], contourLineSidoSize[max_layer], contourLineSidoDistance[max_layer];
  int       optimaTopFontSize[max_layer], optimaMiddleFontSize[max_layer], optimaBottomFontSize[max_layer], symbolScale[max_layer];
  int       basicTotSmtLvl[max_layer], basicPartSmtLvl[max_layer], basicSmtLvl[max_layer];
  float     GapArray[max_layer], contourLineThck[max_layer], contourLineGap[max_layer], contourLineBaseVal[max_layer], contourLineRangeMinVal[max_layer], contourLineRangeMaxVal[max_layer];
  float     contourFillRangeMinVal[max_layer], contourFillRangeMaxVal[max_layer], contourFillBaseVal[max_layer], contourFillGap[max_layer], strmlnThck[max_layer];
  char      menuGrpCd[max_layer][12], varListCd[max_layer][32], lyrsCd[max_layer][32], menu01[max_layer][32], menu02[max_layer][32], menu03[max_layer][32], menu04[max_layer][32];
  char      colorBarYn[max_layer][2], repDispCd[max_layer][2];
  char      contourLineColor[max_layer][32], contourLineDiv[max_layer][2], contourLineOptimaDispGbn[max_layer][2], contourLineTopOptimaColor[max_layer][32], contourLineBtmOptimaColor[max_layer][32];
  char      contourFillColorBarArray[max_layer][512], contourFillArray[max_layer][512], contourFillMaxValExtdYn[max_layer][2], contourFillMinValExtdYn[max_layer][2], contourFillExtdDirt[max_layer][2];
  char      contourFillColor[max_layer][32], curMdlDispType[max_layer][32], symblColor[max_layer][32], symblDispType[max_layer][2], wthrDataDtlCd[max_layer][32], contourLineArray[max_layer][512];
  int       maxGph; // mT 모니터링

  // proj4 변수
  static projPJ  fromProj, toProj;
  double    startx, starty, endx, endy;
  double    x, y, z;
  double    lat, lon;

  // json 변수
  char      json[10000], value[128], imgFile[max_layer][512], mdlVarUnit[128];
  cJSON*    root = NULL;
  cJSON*    data = NULL;    
  cJSON*    result = NULL;      
  cJSON*    tmp = NULL;     
  cJSON*    tmp2 = NULL;     
  cJSON*    colorBarDatas = NULL;    
  int       arr_size, nbar = 0, vred[100], vgreen[100], vblue[100];
  float     vbar[100];
  double    gisStartx[max_layer], gisStarty[max_layer], gisEndx[max_layer], gisEndy[max_layer];

  // curl 변수(API 호출 병렬 처리)
  CURL *handles[max_layer];
  CURLM *multi_handle;
  int still_running = 0; /* keep number of running handles */
  CURLMsg *msg; /* for picking up messages with the transfer status */
  int msgs_left; /* how many messages are left */

  // image 변수
  gdImagePtr im_cht[max_layer];
  float      ratex[max_layer], ratey[max_layer], diffx[max_layer], diffy[max_layer], diffx2[max_layer], diffy2[max_layer];
  int        color, seq;

  int        seq_gts;


////////////////////////////////////////////////////////////////////////////////////////
//                                    시     작                                        //
////////////////////////////////////////////////////////////////////////////////////////

  // 초기화
  for (i = 0; i < max_layer; i++) {
    contourLineEmphLineGap[i] = contourLineTextGap[i] = symblSize[i] = symblGap[i] = -999;
    strmlnDensity[i] = strmlnResol[i] = strmlnArrowGap[i] = strmlnArrowSize[i] = contourLineSidoSize[i] = contourLineSidoDistance[i] = -999;
    optimaTopFontSize[i] = optimaMiddleFontSize[i] = optimaBottomFontSize[i] = symbolScale[i] = -999;
    basicTotSmtLvl[i] = basicPartSmtLvl[i] = basicSmtLvl[i] = -999;

    contourLineThck[i] = contourLineGap[i] = contourLineBaseVal[i] = contourLineRangeMinVal[i] = contourLineRangeMaxVal[i] = -999.;
    contourFillRangeMinVal[i] = contourFillRangeMaxVal[i] = contourFillBaseVal[i] = contourFillGap[i] = strmlnThck[i] = -999.;

    ratex[i] = ratey[i] = 0;
    gisStartx[i] = gisStarty[i] = gisEndx[i] = gisEndy[i] = 0;

    strcpy(menuGrpCd[i], "");
    strcpy(varListCd[i], "");
    strcpy(lyrsCd[i], "");
    strcpy(menu01[i], "");
    strcpy(menu02[i], "");
    strcpy(menu03[i], "");
    strcpy(menu04[i], "");
    strcpy(colorBarYn[i], "");
    strcpy(repDispCd[i], "");
    strcpy(contourLineColor[i], "");
    strcpy(contourLineDiv[i], "");
    strcpy(contourLineOptimaDispGbn[i], "");
    strcpy(contourLineTopOptimaColor[i], "");
    strcpy(contourLineBtmOptimaColor[i], "");
    strcpy(contourFillColorBarArray[i], "");
    strcpy(contourFillArray[i], "");
    strcpy(contourFillMaxValExtdYn[i], "");
    strcpy(contourFillMinValExtdYn[i], "");
    strcpy(contourFillExtdDirt[i], "");
    strcpy(contourFillColor[i], "");
    strcpy(curMdlDispType[i], "");
    strcpy(symblColor[i], "");
    strcpy(symblDispType[i], "");
    strcpy(wthrDataDtlCd[i], "");
    strcpy(contourLineArray[i], "");

    im_cht[i] = NULL;
  }

  // 줌 레벨 체크
  for (i = 0, zoom_level = 0; i < 7; i++) {
    zx = var.zoom_x[i]-'0';
    zy = var.zoom_y[i]-'0';
    if (zx == 0 || zy == 0) break;
    zoom_level++;
  }

  //if (!strcmp(var.map, "NHEM") && zoom_level > 0) zoom_level--;

  // 200/300 경우 5~10월은 200, 11~4월은 300
  seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
  if (MM <= 4 || MM >= 11) {
    strcpy(var.cht_name, str_replace("200_", "300_", var.cht_name));
  }

  //if (!strcmp(var.host, "afsw")) strcpy(path, "http://afsw.kma.go.kr/");
  //else if (!strcmp(var.host, "api")) strcpy(path, "http://172.20.134.195/");
  //else strcpy(path, "http://afs.kma.go.kr/");
  strcpy(path, AFS_HOST_URL);


////////////////////////////////////////////////////////////////////////////////////////
//                              ini 정보 Read                                          //
////////////////////////////////////////////////////////////////////////////////////////
  if (!strcmp(var.cht_mode, "gts")) {
    strcpy(fname, GTS_INI_FILE);
    sprintf(var.cht_name, "%s_asia_%s", var.cht_name, var.gts);
  }
  else strcpy(fname, CHT_INI_FILE);

  if ((fp = fopen(fname, "r"))) {
    while (!feof(fp)) {
      fgets(buf, sizeof(buf), fp);
      if (buf[0] == '#') continue;
      tokens = split(buf, ':', &num_tokens[k]);

      strcpy(type, tokens[0]);
      if (!strcmp(type, "C")) {
        if (strstr(var.cht_name, tokens[2])) {
          //printf("%s\n", tokens[2]);
          for (i = 1; i < num_tokens[k]; i++) {
            switch (i)
            {
              case 1:
                strcpy(model, tokens[i]); //모델 종류
                break;
              case 3:
                strcpy(title, tokens[i]); //제목
                break;
              case 7:
                strcpy(level, tokens[i]); //층 구분(단일면, 등압면)
                break;
            }
          }
          if (!strcmp(model, "G") && (!strncmp(var.cht_name, "kim_", 4) || !strncmp(var.cht_name, "/kim_", 5))) continue; //kim 모델
          ok = 1;
        }
        else {
          if (ok == 1) {
             break;
          }
        }
      }
      else if (!strcmp(type, "L")) {
        if (ok == 1) {
          for (i = 1; i < num_tokens[k]; i++) {
            if (strlen(tokens[i]) == 0) continue;
            switch (i)
            {
              case 1:
                strcpy(menuGrpCd[k], tokens[i]);
                break;
              case 2:
                strcpy(lyrsCd[k], tokens[i]);
                vrtc[k] = atoi(lyrsCd[k]);    
                break;
              case 3:
                strcpy(varListCd[k], tokens[i]);
                break;
              case 4:
                strcpy(menu01[k], tokens[i]);
                break;
              case 5:
                strcpy(menu02[k], tokens[i]);
                break;
              case 6:
                strcpy(menu03[k], tokens[i]);
                if (!strcmp(var.cht_mode, "gts")) {
                  if (!strcmp(var.model, "UM")) {
                    strcpy(menu03[k], "GDAPS");
                    strcpy(model, "G");
                  }
                  else if (!strcmp(var.model, "ECMWF")) {
                    strcpy(menu03[k], "ECMWF_H");
                    strcpy(model, "E");
                  }
                  else if (!strcmp(var.model, "KIMG")) {
                    strcpy(menu03[k], "GDAPS_KIM");
                    strcpy(model, "GK");
                  }
                }
                break;
              case 7:
                strcpy(colorBarYn[k], tokens[i]);
                break;
              case 8:
                strcpy(contourLineColor[k], tokens[i]);
                break;
              case 9:
                strcpy(contourLineDiv[k], tokens[i]);
                break;
              case 10:
                contourLineThck[k] = atof(tokens[i]);
                break;
              case 11:
                strcpy(contourLineOptimaDispGbn[k], tokens[i]);
                break;
              case 12:
                strcpy(contourLineTopOptimaColor[k], tokens[i]);
                break;
              case 13:
                strcpy(contourLineBtmOptimaColor[k], tokens[i]);
                break;
              case 14:
                contourLineEmphLineGap[k] = atoi(tokens[i]);
                break;
              case 15:
                contourLineGap[k] = atof(tokens[i]);
                break;
              case 16:
                contourLineBaseVal[k] = atof(tokens[i]);
                break;
              case 17:
                contourLineTextGap[k] = atoi(tokens[i]);
                break;
              case 18:
                contourLineRangeMinVal[k] = atof(tokens[i]);
                break;
              case 19:
                contourLineRangeMaxVal[k] = atof(tokens[i]);
                break;
              case 20:
                strcpy(contourFillColorBarArray[k], tokens[i]);
                break;
              case 21:
                strcpy(repDispCd[k], tokens[i]);
                break;
              case 22: //최소값
                contourFillRangeMinVal[k] = atof(tokens[i]);
                break;
              case 23: //최대값
                contourFillRangeMaxVal[k] = atof(tokens[i]);
                break;
              case 24: //기준값
                contourFillBaseVal[k] = atof(tokens[i]);
                break;
              case 25: //면배열
                strcpy(contourFillArray[k], tokens[i]);
                break;
              case 26: //간격
                contourFillGap[k] = atof(tokens[i]);
                break;
              case 27: //색상표 최소값 이하 채움
                strcpy(contourFillMaxValExtdYn[k], tokens[i]);
                break;
              case 28: //색상표 최대값 이상 채움
                strcpy(contourFillMinValExtdYn[k], tokens[i]);
                break;
              case 29: //등치면 구간설정(A: 이상/미만, E: 초과/이하)
                strcpy(contourFillExtdDirt[k], tokens[i]);
                break;
              case 30: 
                strcpy(contourFillColor[k], tokens[i]);
                break;
              case 31: 
                strcpy(curMdlDispType[k], tokens[i]);
                break;
              case 32: 
                strmlnDensity[k] = atoi(tokens[i]);
                break;
              case 33: 
                strmlnResol[k] = atoi(tokens[i]);
                break;
              case 34: 
                strmlnArrowGap[k] = atoi(tokens[i]);
                break;
              case 35: 
                strmlnArrowSize[k] = atoi(tokens[i]);
                break;
              case 45: 
                contourLineSidoSize[k] = atoi(tokens[i]);
                break;
              case 46: 
                contourLineSidoDistance[k] = atoi(tokens[i]);
                break;
              case 47: 
                strcpy(symblColor[k], tokens[i]);
                break;
              case 48: 
                symblSize[k] = atoi(tokens[i]);
                break;
              case 49: 
                strcpy(symblDispType[k], tokens[i]);
                break;
              case 50: 
                symblGap[k] = atoi(tokens[i]);
                break;
              case 51: 
                optimaTopFontSize[k] = atoi(tokens[i]);
                break;
              case 52: 
                optimaMiddleFontSize[k] = atoi(tokens[i]);
                break;
              case 53: 
                optimaBottomFontSize[k] = atoi(tokens[i]);
                break;
              case 56: 
                strcpy(wthrDataDtlCd[k], tokens[i]);
                break;
              case 57: 
                strcpy(contourLineArray[k], tokens[i]);
                break;
              case 58: 
                strcpy(menu04[k], tokens[i]);
                break;
              case 59: 
                symbolScale[k] = atoi(tokens[i]);
                break;
              case 60: 
                strmlnThck[k] = atof(tokens[i]);
                break;
              case 61: 
                basicTotSmtLvl[k] = atoi(tokens[i]);
                break;
              case 62: 
                basicPartSmtLvl[k] = atoi(tokens[i]);
                break;   
            }
          }                          

          // 예외처리
          // 한반도 영역
          if (!strcmp(var.map, "E10") && strcmp(var.cht_mode, "fct2") && strcmp(var.cht_mode, "ana3") && !strstr(var.cht_name, "mT")) {
            if (!strcmp(model, "G")) {
              strcpy(menu03[k], "GDAPS_1H");
              if (!strcmp(varListCd[k], "APCP") || !strcmp(varListCd[k], "PRMSL") || !strcmp(varListCd[k], "ACSNOW") || !strcmp(varListCd[k], "NCPCP") || !strcmp(varListCd[k], "ACPCP")) {
                strcpy(lyrsCd[k], "0");
              }
              else if ((!strcmp(varListCd[k], "DPDK") || !strcmp(varListCd[k], "TMP") || !strcmp(varListCd[k], "DEWP")) && vrtc[k] == 2) {
                strcpy(lyrsCd[k], "1.5");
              }

              if (strstr(var.cht_name, "capcin")) {
                strcpy(lyrsCd[k], "0");
              }
            }
            else if (!strcmp(model, "GK")) {
              strcpy(menu03[k], "KIM_1H");
              if (!strcmp(varListCd[k], "APCP")) {
                strcpy(lyrsCd[k], "0");
              }
              else if (!strcmp(varListCd[k], "NCPCP")) {
                strcpy(lyrsCd[k], "-999999");
              }
              else if (!strcmp(varListCd[k], "2T")) {
                strcpy(menu01[k], "PRSS_TMP");
                strcpy(menu02[k], "TMP");
                strcpy(varListCd[k], "TMP");
              }
              else if (!strcmp(varListCd[k], "DPT") && vrtc[k] == 2) {
                strcpy(menu01[k], "PRSS_TMP");
                strcpy(menu02[k], "DEWP");
                strcpy(varListCd[k], "DEWP");
              }
            }
            else if (!strcmp(model, "E")) {
              if (strstr(var.cht_name, "kindex")) strcpy(menu03[k], "ECMWF_1H10G2");
              else strcpy(menu03[k], "ECMWF_1H10G1");

              if (!strcmp(varListCd[k], "TMP") && vrtc[k] < 0) {
                strcpy(lyrsCd[k], "0");
              }
              else if (!strcmp(varListCd[k], "WSPD_NV") && vrtc[k] < 0) {
                strcpy(lyrsCd[k], "0");
              }

              if (strstr(var.cht_name, "kindex")) {
                strcpy(lyrsCd[k], "-999999");
              }
              else if (strstr(var.cht_name, "capcin")) {
                strcpy(lyrsCd[k], "0");
              }
            }

            // 유선
            if (k != 0 && (!strcmp(varListCd[k], "STRLN") || !strcmp(varListCd[k], "10_STRLN"))) strmlnArrowSize[k]= 1;
          }

          // GTS 자료조회 PLUS
          if (!strcmp(var.cht_mode, "gts")) {
            if (!strcmp(varListCd[k], "PRMSL")) {
              if (!strcmp(var.model, "KIMG")) strcpy(lyrsCd[k], "0");
            }

            if (!strcmp(varListCd[k], "WSPD_NV") && !strcmp(menu01[k], "UNIS_SFC")) {
              if (!strcmp(var.model, "ECMWF")) strcpy(lyrsCd[k], "0");
            }
          }

          // 등치선 굵기 조정
          if (strcmp(var.map, "E10") && contourLineThck[k] == 1 && !strcmp(contourLineDiv[k], "A") && strcmp(var.cht_mode, "gts")) {
            if (!strcmp(contourLineColor[k], "BLACK") || !strcmp(contourLineColor[k], "BLUE") || !strcmp(contourLineColor[k], "RED")) {
              if (zoom_level != 4) contourLineThck[k] = 1.5;
            }
          }
          else if (strstr(var.cht_name, "wtem") && !strcmp(contourLineDiv[k], "D")) {
            if (strcmp(var.map, "E10")) {
              if (zoom_level == 4) contourLineThck[k] = 1.3;
              else contourLineThck[k] = 2;
            }
            else {
              if (zoom_level == 0 || zoom_level == 2) contourLineThck[k] = 1.5;
              else contourLineThck[k] = 2;
            }
          }

          // 등치선 간격 조정
          if (k != 0 || !strcmp(var.cht_mode, "ana2") || !strcmp(var.cht_mode, "gts")) {
            if (zoom_level != 0) {
              if (!strcmp(varListCd[k], "HGT") || !strcmp(menu02[k], "GTS_HGT")) {
                if (vrtc[k] == 10 || vrtc[k] == 50 || vrtc[k] == 100 || vrtc[k] == 150 || vrtc[k] == 200 || vrtc[k] == 250 || vrtc[k] == 300) {
                  GapArray[0] = 120;
                  GapArray[1] = 60; 
                  GapArray[2] = 40; 
                  GapArray[3] = 30; 
                  GapArray[4] = 20; 
                  GapArray[5] = 20; 
                  GapArray[6] = 20; 
                  GapArray[7] = 20; 
                  contourLineGap[k] = GapArray[zoom_level];
                }
                else if (vrtc[k] == 350 || vrtc[k] == 400 || vrtc[k] == 450 || vrtc[k] == 500) {
                  GapArray[0] = 60;
                  GapArray[1] = 30; 
                  GapArray[2] = 20; 
                  GapArray[3] = 20; 
                  GapArray[4] = 20; 
                  GapArray[5] = 20; 
                  GapArray[6] = 20; 
                  GapArray[7] = 20; 
                  contourLineGap[k] = GapArray[zoom_level];
                }
                else if (vrtc[k] == 550 || vrtc[k] == 600 || vrtc[k] == 650 || vrtc[k] == 700 || vrtc[k] == 750 || vrtc[k] == 800 || vrtc[k] == 850)  {
                  GapArray[0] = 30;
                  GapArray[1] = 15; 
                  GapArray[2] = 10; 
                  GapArray[3] = 10; 
                  GapArray[4] = 10; 
                  GapArray[5] = 10; 
                  GapArray[6] = 10; 
                  GapArray[7] = 10; 
                  contourLineGap[k] = GapArray[zoom_level];
                }
                else if (vrtc[k] == 875 || vrtc[k] == 900 || vrtc[k] == 925 || vrtc[k] == 950 || vrtc[k] == 975 || vrtc[k] == 1000) {
                  GapArray[0] = 20;
                  GapArray[1] = 10; 
                  GapArray[2] = 10; 
                  GapArray[3] = 10; 
                  GapArray[4] = 10; 
                  GapArray[5] = 10; 
                  GapArray[6] = 10; 
                  GapArray[7] = 10; 
                  contourLineGap[k] = GapArray[zoom_level];
                }
              }
              else if (!strcmp(varListCd[k], "PTHETA")) {
                if (vrtc[k] == 350 || vrtc[k] == 345) {
                  GapArray[0] = 10;
                  GapArray[1] = 4;
                  GapArray[2] = 4;
                  GapArray[3] = 2;
                  GapArray[4] = 2;
                  GapArray[5] = 2;
                  GapArray[6] = 2;
                  GapArray[7] = 2; 
                  contourLineGap[k] = GapArray[zoom_level];
                }
                else if (vrtc[k] == 340) {
                  GapArray[0] = 25;
                  GapArray[1] = 20;
                  GapArray[2] = 10;
                  GapArray[3] = 5;
                  GapArray[4] = 5;
                  GapArray[5] = 5;
                  GapArray[6] = 5;
                  GapArray[7] = 5; 
                  contourLineGap[k] = GapArray[zoom_level];
                }
                else {
                  GapArray[0] = 50;
                  GapArray[1] = 25;
                  GapArray[2] = 20;
                  GapArray[3] = 10;
                  GapArray[4] = 10;
                  GapArray[5] = 10;
                  GapArray[6] = 10;
                  GapArray[7] = 10; 
                  contourLineGap[k] = GapArray[zoom_level];
                }
              }
              else if ((!strcmp(varListCd[k], "PRMSL") || !strcmp(menu02[k], "GTS_PRMSL")) && contourLineGap[k] == 0) {
                contourLineGap[k] = 0;
              }
              else if ((!strcmp(varListCd[k], "PRMSL") || !strcmp(menu02[k], "GTS_PRMSL")) && !strcmp(var.cht_mode, "gts")) {
                if (strstr(var.cht_name, "diff")) contourLineGap[k] = 2;
                else if (zoom_level <= 1) contourLineGap[k] = 2;
                else {
                  contourLineGap[k] = contourLineGap[k]/(zoom_level+1);
                  contourLineGap[k] = (float)((int)(contourLineGap[k]+0.5));
                  if (contourLineGap[k] < 1) contourLineGap[k] = 1;
                }
              }
              else if (!strcmp(varListCd[k], "RVOA")) {
                contourLineGap[k] = 10;
              }
              else if (!strcmp(varListCd[k], "WSPD_25L")) {
                contourLineGap[k] = 5;
              }
              else if (!strcmp(varListCd[k], "WSPD_50L")) {
              }
              else if (!strcmp(varListCd[k], "TH_1000_850_1281") && !strcmp(model, "GK")) {
                contourLineGap[k] = 0;
              }
              else if (!strcmp(varListCd[k], "DEWP")) {
                contourLineGap[k] = 3;
              }
              else if ((!strcmp(varListCd[k], "TMP") || !strcmp(varListCd[k], "2T")) && vrtc[k] <= 2) {
                contourLineGap[k] = 2;
              }
              else if (!strcmp(varListCd[k], "TMP") || !strcmp(menu02[k], "GTS_TMP")) {
                if (strcmp(var.map, "E10") && zoom_level == 2) contourLineGap[k] = 2;
                else if (!strcmp(var.cht_mode, "gts") && strstr(var.cht_name, "diff")) contourLineGap[k] = 2;
                else {
                  contourLineGap[k] = contourLineGap[k]/(zoom_level+1);
                  contourLineGap[k] = (float)((int)(contourLineGap[k]+0.5));
                  if (contourLineGap[k] < 1) contourLineGap[k] = 1;
                }
              }
              else if (!strcmp(var.cht_mode, "gts") && !strcmp(varListCd[k], "EPOT")) {
                contourLineGap[k] = 3;
              }
              else {
                contourLineGap[k] = contourLineGap[k]/(zoom_level+1);
                contourLineGap[k] = (float)((int)(contourLineGap[k]+0.5));
                if (contourLineGap[k] < 1) contourLineGap[k] = 1;
              }
            }

            if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD")) contourLineGap[k] *= 2.0;
          }
          else {
            if (!strcmp(varListCd[k], "WSPD_25F") && vrtc[k] > 500 && zoom_level > 0) {
              contourLineGap[k] = 5;
            }
          }

          // ECMWF 1시간 일기도 여름철/겨울철 강수 cut-off 값 변경
          if ((MM <= 4 || MM >= 11)) {
            if (!strcmp(var.map, "E10") && (!strcmp(varListCd[k], "APCP") || !strcmp(varListCd[k], "ACPCP") || !strcmp(varListCd[k], "CP") || !strcmp(varListCd[k], "NCPCP") || !strcmp(varListCd[k], "LSP") || !strcmp(varListCd[k], "ACSNOW"))) {
              if (contourLineRangeMinVal[k] == 0.5) contourLineRangeMinVal[k] = 0.1;
              if (contourFillRangeMinVal[k] == 0.5) contourFillRangeMinVal[k] = 0.1; //최소값

              if (!strcmp(contourFillArray[k], "0.5|0.5")) strcpy(contourFillArray[k], "0.1|0.1"); //면배열
              else strcpy(contourFillArray[k], "0.1|3|6|9|12|15|18|21|24|27|30|33|36|39|42|45|48|51|54|57|60"); //면배열

              if (!strcmp(contourLineArray[k], "0.5|0.5")) strcpy(contourLineArray[k], "0.1|0.1"); 
            }
            else if (!strcmp(varListCd[k], "APCP") || !strcmp(varListCd[k], "ACPCP") || !strcmp(varListCd[k], "CP") || !strcmp(varListCd[k], "NCPCP") || !strcmp(varListCd[k], "LSP") || !strcmp(varListCd[k], "ACSNOW")) {
              if (contourLineRangeMinVal[k] == 0.5) contourLineRangeMinVal[k] = 0.25;
              if (contourFillRangeMinVal[k] == 0.5) contourFillRangeMinVal[k] = 0.25; //최소값

              if (!strcmp(contourFillArray[k], "0.5|0.5")) strcpy(contourFillArray[k], "0.25|0.25"); //면배열
              else strcpy(contourFillArray[k], "0.25|5|10|15|20|25|30|35|40|45|50|55|60|65|70|75|80|85|90|95|100"); //면배열

              if (!strcmp(contourLineArray[k], "0.5|0.5")) strcpy(contourLineArray[k], "0.25|0.25"); 
            }
          }
          else {
            if (!strcmp(var.map, "E10") && (!strcmp(varListCd[k], "APCP") || !strcmp(varListCd[k], "ACPCP") || !strcmp(varListCd[k], "CP") || !strcmp(varListCd[k], "NCPCP") || !strcmp(varListCd[k], "LSP") || !strcmp(varListCd[k], "ACSNOW"))) {
              if (contourLineRangeMinVal[k] == 0.5) contourLineRangeMinVal[k] = 0.25;
              if (contourFillRangeMinVal[k] == 0.5) contourFillRangeMinVal[k] = 0.25; //최소값

              if (!strcmp(contourFillArray[k], "0.5|0.5")) strcpy(contourFillArray[k], "0.25|0.25"); //면배열
              else strcpy(contourFillArray[k], "0.25|3|6|9|12|15|18|21|24|27|30|33|36|39|42|45|48|51|54|57|60"); //면배열

              if (!strcmp(contourLineArray[k], "0.5|0.5")) strcpy(contourLineArray[k], "0.25|0.25"); 
            }
          }

          // 지상바람 기온 분포 범례
          if (strstr(var.cht_name, "tgc2d") && (!strcmp(varListCd[k], "TMP") || !strcmp(varListCd[k], "2T"))) {
            if ((MM == 1 || MM == 2 || MM == 12)) {
              strcpy(contourFillArray[k], "-30|-27|-24|-21|-18|-15|-12|-9|-6|-3|0|3|6|9|12|15|18|21|24|27|30|30");
              contourFillRangeMinVal[k] = -30;
              contourFillRangeMaxVal[k] = 30;
              contourFillBaseVal[k] = 0;
            }
            else if ((MM == 3 || MM == 4 || MM == 5 || MM == 9 || MM == 10 || MM == 11)) {
              strcpy(contourFillArray[k], "-20|-17|-14|-11|-8|-5|-2|1|4|7|10|13|16|19|22|25|28|31|34|37|40|40");
              contourFillRangeMinVal[k] = -20;
              contourFillRangeMaxVal[k] = 40;
              contourFillBaseVal[k] = 10;
            }
          }
/*
          // 유선 조정
          if (k != 0 && !strcmp(varListCd[k], "STRLN") && (!strcmp(model, "E") || !strcmp(model, "E10")) && zoom_level > 0) {
            if (zoom_level < 3) {
              strmlnDensity[k]= 5 - zoom_level;
              if (strmlnDensity[k] < 0) strmlnDensity[k] = 0;
              strmlnResol[k]= zoom_level + 1;
              strmlnArrowGap[k]= (int)(strmlnArrowGap[k]/(zoom_level+1));
            }
            else {
              strmlnDensity[k]= 3;
              strmlnResol[k]= 3;
              strmlnArrowGap[k]= (int)(strmlnArrowGap[k]/3);
            }
            strmlnArrowSize[k]= 1;
          }
*/

          // 바람깃 크기/간격 조정
          if (!strcmp(varListCd[k], "WSPD_NW") || !strcmp(varListCd[k], "WSPD_15W") || !strcmp(varListCd[k], "WSPD_20W") || !strcmp(varListCd[k], "WSPD_25W") || !strcmp(varListCd[k], "WSPD_NV")) {
            if (!strcmp(var.map, "E10")) {
              if (zoom_level == 0) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 90;
                  else symbolScale[k]= 110;
                  symblGap[k]= 3;
                }
              }
              else if (zoom_level == 1) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 90;
                  else symbolScale[k]= 110;
                  symblGap[k]= 5;
                }
              }
              else if (zoom_level == 2) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 80;
                  symblGap[k]= 3;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 100;
                  else symbolScale[k]= 110;
                  symblGap[k]= 6;
                }
              }
              else if (zoom_level == 3) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 110;
                  symblGap[k]= 2;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 120;
                  else symbolScale[k]= 150;
                  symblGap[k]= 4;
                }
              }
              else if (zoom_level == 4) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 80;
                  symblGap[k]= 1;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 85;
                  else symbolScale[k]= 110;
                  symblGap[k]= 3;
                }
              }
              else if (zoom_level == 5) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 120;
                  symblGap[k]= 1;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 110;
                  else symbolScale[k]= 150;
                  symblGap[k]= 2;
                }
              }
              else if (zoom_level == 6) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 80;
                  symblGap[k]= 0;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 90;
                  else symbolScale[k]= 90;
                  symblGap[k]= 1;
                }
              }
              else if (zoom_level >= 7) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 80;
                  symblGap[k]= 0;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 60;
                  else symbolScale[k]= 60;
                  symblGap[k]= 0;
                }
              }
            }
            else if (!strcmp(var.map, "EA_CHT") || !strcmp(var.map, "TP")) {
              if (zoom_level == 0) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 60;
                  symblGap[k]= 2;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 90;
                  else symbolScale[k]= 110;
                  symblGap[k]= 5;
                }
              }
              else if (zoom_level == 1) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 60;
                  symblGap[k]= 1;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 100;
                  else symbolScale[k]= 120;
                  symblGap[k]= 3;
                }
              }
              else if (zoom_level == 2) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 80;
                  symblGap[k]= 2;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 100;
                  else symbolScale[k]= 120;
                  symblGap[k]= 4;
                }
              }
              else if (zoom_level == 3) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 100;
                  symblGap[k]= 2;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 100;
                  else symbolScale[k]= 130;
                  symblGap[k]= 3;
                }
              }
              else if (zoom_level == 4) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 70;
                  symblGap[k]= 3;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 80;
                  else symbolScale[k]= 100;
                  symblGap[k]= 5;
                }
              }
              else if (zoom_level == 5) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 90;
                  symblGap[k]= 2;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 100;
                  else symbolScale[k]= 120;
                  symblGap[k]= 3;
                }
              }
              else if (zoom_level == 6) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 70;
                  symblGap[k]= 1;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 60;
                  else symbolScale[k]= 80;
                  symblGap[k]= 2;
                }
              }
              else if (zoom_level >= 7) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 60;
                  symblGap[k]= 1;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 60;
                  else symbolScale[k]= 60;
                  symblGap[k]= 1;
                }
              }
            }
            else if (!strcmp(var.map, "NHEM")) {
              if (zoom_level == 0) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                }
                else {
                  symbolScale[k]= 110;
                  symblGap[k]= 8;
                }
              }
              else if (zoom_level == 1) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                }
                else {
                  symbolScale[k]= 110;
                  symblGap[k]= 6;
                }
              }
              else if (zoom_level == 2) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symblGap[k]= 2;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 100;
                  else symbolScale[k]= 120;
                  symblGap[k]= 4;
                }
              }
              else if (zoom_level == 3) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symblGap[k]= 1;
                }
                else {
                  if (!strcmp(var.cht_mode, "gts")) symbolScale[k]= 100;
                  else symbolScale[k]= 120;
                  symblGap[k]= 2;
                }
              }
            }
            else if (!strcmp(var.map, "WORLD")) {
              if (zoom_level == 1) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                }
                else {
                  symbolScale[k]= 90;
                  symblGap[k]= 3;
                }
              }
              else if (zoom_level == 2) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                }
                else {
                  symbolScale[k]= 90;
                  symblGap[k]= 2;
                }
              }
              else if (zoom_level == 3) {
                if (!strcmp(symblDispType[k], "V")) { // 화살표
                  symbolScale[k]= 50;
                  symblGap[k]= 0;
                }
                else {
                  symbolScale[k]= 90;
                  symblGap[k]= 1;
                }
              }
            }
          }
          else if (!strcmp(varListCd[k], "MOIF")) { // 수분속
            symbolScale[k]= 70;
            symblGap[k]= 10;
          }

          // mT 모니터링(500GPH 최대값 계산)
          if (strstr(var.cht_name, "mT")) {
            if (!strcmp(var.cht_mode, "gts")) {
              if (k == 1) {
                maxGph = getMaxGph(model, vrtc[k]);
                contourLineRangeMinVal[k] = (float)(maxGph - 50);
                contourLineRangeMaxVal[k] = (float)(maxGph - 50);
              }
            } 
            else {
              if (k == 2) {
                maxGph = getMaxGph(model, vrtc[k]);
                contourLineRangeMinVal[k] = (float)(maxGph - 50);
                contourLineRangeMaxVal[k] = (float)(maxGph - 50);
              }
            }
          }

          // 글자 크기 조정
          if (contourLineSidoSize[k] == 18) {
            if (!strcmp(var.map, "NHEM")) {
            }
            else if (!strcmp(var.map, "E10")) {
              if (zoom_level == 3 || zoom_level == 5) contourLineSidoSize[k] = 28;
            }
            else {
              if (zoom_level != 4) contourLineSidoSize[k] = 24;
            }
          }
          else if (!strcmp(var.cht_mode, "gts") && contourLineSidoSize[k] == 16) {
            if (!strcmp(var.map, "NHEM")) {
            }
            else if (!strcmp(var.map, "E10")) {
              if (zoom_level == 3 || zoom_level == 5) contourLineSidoSize[k] = 22;
            }
          }

          if (optimaTopFontSize[k] == 20) {
            if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD")) {
              if (zoom_level <= 1) optimaTopFontSize[k] = 42;
            }
            else if (!strcmp(var.map, "E10")) {
              if (zoom_level == 2 || zoom_level == 4 || zoom_level == 5 || zoom_level == 6) optimaTopFontSize[k] = 26;
              else if (zoom_level == 3) optimaTopFontSize[k] = 32;
            }
            else {
              if (zoom_level == 4) optimaTopFontSize[k] = 24; 
              else optimaTopFontSize[k] = 28;
            }
          }

          if (optimaBottomFontSize[k] == 20 || (!strcmp(varListCd[k], "APCP") || !strcmp(varListCd[k], "ACPCP") || !strcmp(varListCd[k], "CP") || !strcmp(varListCd[k], "ACSNOW"))) {
            if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD")) {
              if (zoom_level <= 1) optimaBottomFontSize[k] = 26;
            }
            else if (!strcmp(var.map, "E10")) {
              if (zoom_level == 2 || zoom_level == 4 || zoom_level == 5 || zoom_level == 6) optimaBottomFontSize[k] = 26;
              else if (zoom_level == 3) optimaBottomFontSize[k] = 32;
            }
            else {
              if (zoom_level == 4) optimaBottomFontSize[k] = 24;
              else optimaBottomFontSize[k] = 28;
            }
          }

          // 스무딩 레벨 조정
          if (!strcmp(var.map, "NHEM")) {
            basicTotSmtLvl[k] = 5 - (zoom_level + 1);
          }
          else {
            if (zoom_level >= 3) basicTotSmtLvl[k] = 5 - zoom_level;
            else basicTotSmtLvl[k] = 5;
          }
          if (basicTotSmtLvl[k] < 2) basicTotSmtLvl[k] = 2;
          if (!strcmp(var.map, "E10")) basicTotSmtLvl[k] = 2;
          if (!strcmp(varListCd[k], "APCP") || !strcmp(varListCd[k], "ACPCP") || !strcmp(varListCd[k], "NCPCP") || !strcmp(varListCd[k], "LSP") || !strcmp(varListCd[k], "CP") || !strcmp(varListCd[k], "ACSNOW")) basicTotSmtLvl[k] = 0;
          if (k == 0 && !strcmp(varListCd[k], "DPDK")) basicTotSmtLvl[k] = 1;
          if (!strcmp(varListCd[k], "PRMSL") && contourLineRangeMaxVal[k] == -50) basicTotSmtLvl[k] = 0;

          // 해상도 조절(40km, 20km, 10km)
          if (!strcmp(repDispCd[k], "F") && strstr(var.cht_name, "raster")) basicSmtLvl[k] = 1;
          else if (!strcmp(var.map, "E10")) {
            if (zoom_level >= 2) {
              if (!strcmp(varListCd[k], "HDIV_CON") || !strcmp(varListCd[k], "HDIV_DIV") || !strcmp(varListCd[k], "RVOA") || !strcmp(varListCd[k], "THA_1000_700") || !strcmp(varListCd[k], "TMP") || !strcmp(varListCd[k], "DEWP")) basicSmtLvl[k] = 2;
              else basicSmtLvl[k] = 1;
            }
            else if (zoom_level == 1 || (k != 0 && strstr(varListCd[k], "STRLN"))) basicSmtLvl[k] = 2;
          }
          else if ((k != 0 || !strcmp(var.cht_mode, "gts")) && strstr(varListCd[k], "STRLN") && zoom_level >= 3) basicSmtLvl[k] = 1;
          else if (zoom_level == 2 || zoom_level == 3) basicSmtLvl[k] = 2;
          else if (zoom_level >= 4) {
            if (!strcmp(varListCd[k], "HDIV_CON") || !strcmp(varListCd[k], "HDIV_DIV") || !strcmp(varListCd[k], "RVOA") || !strcmp(varListCd[k], "THA_1000_700") 
             || !strcmp(varListCd[k], "TMP") || !strcmp(varListCd[k], "DEWP") || !strcmp(varListCd[k], "TPW") || !strcmp(varListCd[k], "FROG")) basicSmtLvl[k] = 2;
            else basicSmtLvl[k] = 1;
          }
          
          k++;
        } //if (ok == 1) 의 끝.
      } //if (!strcmp(type, "C"))의 끝.
      free(tokens);
    } //while (!feof(fp))의 끝.
    num_layer = k;
    fclose(fp);
  } //if ((fp = fopen(fname, "r")))의 끝.


////////////////////////////////////////////////////////////////////////////////////////
//                              모델에 따른 지도 영역 설정                           //
////////////////////////////////////////////////////////////////////////////////////////
  //map_edge();

  fromProj = pj_init_plus("+proj=longlat +ellps=WGS84 +datum=WGS84");
  if (!strcmp(var.map, "NHEM"))       toProj = pj_init_plus("+proj=stere +lat_0=90 +lat_ts=90 +lon_0=120 +x_0=0 +y_0=0 +ellps=WGS84 +units=m");
  else if (!strcmp(var.map, "WORLD")) toProj = pj_init_plus("+proj=eqc +lat_0=0 +lat_ts=0 +lon_0=126 +x_0=0 +y_0=0 +ellps=WGS84 +units=m");
  else toProj = pj_init_plus("+proj=lcc +lat_1=30 +lat_2=60 +lat_0=0 +lon_0=126 +x_0=0 +y_0=0 +ellps=WGS84 +units=m");

  x = var.startx * DEG_TO_RAD;
  y = var.starty * DEG_TO_RAD;
  lon = var.startx;
  lat = var.starty;
  pj_transform( fromProj, toProj, 1, 0, &x, &y, &z );
  startx = x;
  starty = y;

  x = var.endx * DEG_TO_RAD;
  y = var.endy * DEG_TO_RAD;
  lon = var.endx;
  lat = var.endy;
  pj_transform( fromProj, toProj, 1, 0, &x, &y, &z );
  endx = x;
  endy = y;

  sprintf(area_afs, "&STARTX=%.7f&STARTY=%.7f&ENDX=%.7f&ENDY=%.7f", startx, starty, endx, endy);
  //printf("%s\n\n", area_afs);


////////////////////////////////////////////////////////////////////////////////////////
//                      ini 파라미터 바인딩 (단열선도, 연직시계열 포함)                 //
////////////////////////////////////////////////////////////////////////////////////////

  // 각 레이어 호출
  for (i = 0; i < num_layer; i++) {

    sprintf(cht_afs[i], "%safsiwa/iwa/api/iwaImgUrlApi/retImgUrl.kajx?", path);
    if (!strcmp(var.cht_mode, "gts")) {
      // GTS 관측 전문 객관분석 자료 등치선 표출
      if (!strcmp(menu01[i], "GTS")) {
        sprintf(cht_afs[i], "%smenuGrpCd=%s&menu01=%s&menu02=%s&varListCd=%s&vrtcLayrCd=%s", cht_afs[i], menuGrpCd[i], menu01[i], menu02[i], varListCd[i], lyrsCd[i]);
        seq2time((int)(var.seq/(3*60))*(3*60), &YY, &MM, &DD, &HH, &MI, 'm', 'n');
        sprintf(cht_afs[i], "%s&analTime=%04d%02d%02d%02d%02d", cht_afs[i], YY, MM, DD, HH, MI);
        sprintf(cht_afs[i], "%s&foreTime=%04d%02d%02d%02d%02d", cht_afs[i], YY, MM, DD, HH, MI);

        seq_gts  = time2seq(2021, 12, 23, 0, 0, 'm');
        if ((int)(var.seq/(3*60))*(3*60) < seq_gts) return 0;
      }
      else {
        sprintf(cht_afs[i], "%smenuGrpCd=%s&menu01=%s&menu02=%s&menu03=%s&varListCd=%s&vrtcLayrCd=%s", cht_afs[i], menuGrpCd[i], menu01[i], menu02[i], menu03[i], varListCd[i], lyrsCd[i]);
        seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
        sprintf(cht_afs[i], "%s&analTime=%04d%02d%02d%02d%02d", cht_afs[i], YY, MM, DD, HH, MI);
        if (!strcmp(var.map, "E10")) seq2time((int)(var.seq/(1*60))*(1*60), &YY, &MM, &DD, &HH, &MI, 'm', 'n');
        else seq2time((int)(var.seq/(3*60))*(3*60), &YY, &MM, &DD, &HH, &MI, 'm', 'n');
        sprintf(cht_afs[i], "%s&foreTime=%04d%02d%02d%02d%02d", cht_afs[i], YY, MM, DD, HH, MI);
      }
    }
    else {
      sprintf(cht_afs[i], "%smenuGrpCd=%s&menu01=%s&menu02=%s&menu03=%s&varListCd=%s&vrtcLayrCd=%s", cht_afs[i], menuGrpCd[i], menu01[i], menu02[i], menu03[i], varListCd[i], lyrsCd[i]);
      seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
      sprintf(cht_afs[i], "%s&analTime=%04d%02d%02d%02d%02d", cht_afs[i], YY, MM, DD, HH, MI);
      seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
      sprintf(cht_afs[i], "%s&foreTime=%04d%02d%02d%02d%02d", cht_afs[i], YY, MM, DD, HH, MI);
    }


    // 줌 레벨
    if (!strcmp(var.map, "EA_CHT") || !strcmp(var.map, "TP")) {
      if (zoom_level == 0) zmlvl[i] = 7;
      else if (zoom_level == 1) zmlvl[i] = 8;
      else if (zoom_level == 2) zmlvl[i] = 11;
      else if (zoom_level == 3) zmlvl[i] = 12;
      else if (zoom_level == 4) zmlvl[i] = 12;
      else if (zoom_level == 5) zmlvl[i] = 13;
      else if (zoom_level >= 6) zmlvl[i] = 13;
    }
    else if (!strcmp(var.map, "E10") || !strcmp(var.map, "H4")) {
      if (zoom_level == 0) zmlvl[i] = 8;
      else if (zoom_level == 1) zmlvl[i] = 11;
      else if (zoom_level == 2) zmlvl[i] = 12;
      else if (zoom_level == 3) zmlvl[i] = 13;
      else if (zoom_level == 4) zmlvl[i] = 13;
      else if (zoom_level == 5) zmlvl[i] = 14;
      else if (zoom_level >= 6) zmlvl[i] = 14;
    }
    else if (!strcmp(var.map, "NHEM")) {
      if (zoom_level == 0) zmlvl[i] = 5;
      else if (zoom_level == 1) zmlvl[i] = 6;
      else if (zoom_level == 2) zmlvl[i] = 7;
      else if (zoom_level == 3) zmlvl[i] = 8;
      else if (zoom_level == 4) zmlvl[i] = 9;
      else if (zoom_level == 5) zmlvl[i] = 10;
      else if (zoom_level >= 6) zmlvl[i] = 11;
    }
    else if (!strcmp(var.map, "WORLD")) {
      if (zoom_level == 0) zmlvl[i] = 4;
      else if (zoom_level == 1) zmlvl[i] = 5;
      else if (zoom_level == 2) zmlvl[i] = 6;
      else if (zoom_level == 3) zmlvl[i] = 7;
      else if (zoom_level == 4) zmlvl[i] = 8;
      else if (zoom_level == 5) zmlvl[i] = 9;
      else if (zoom_level >= 6) zmlvl[i] = 10;
    }
    sprintf(cht_afs[i], "%s%s&ZOOMLVL=%d", cht_afs[i], area_afs, zmlvl[i]);


    // 도법
    if (!strcmp(var.map, "NHEM")) {
      sprintf(cht_afs[i], "%s&PROJ=STG", cht_afs[i]);
    }
    else if (!strcmp(var.map, "WORLD")) {
      sprintf(cht_afs[i], "%s&PROJ=CYL", cht_afs[i]);
    }
    else if (!strcmp(var.map, "TP")) {
      sprintf(cht_afs[i], "%s&PROJ=LCC&isTypArea=true", cht_afs[i]);
    }
    else {
      sprintf(cht_afs[i], "%s&PROJ=LCC", cht_afs[i]);
    }


    // INI 파라미터
    if (strlen(contourLineColor[i]) > 0) {
      sprintf(cht_afs[i], "%s&contourLineColor=%s", cht_afs[i], contourLineColor[i]);
    }

    if (strlen(contourLineDiv[i]) > 0) {
      sprintf(cht_afs[i], "%s&contourLineDiv=%s", cht_afs[i], contourLineDiv[i]);
    }

    if (contourLineThck[i] != -999.) {
      if ((int)(fabs(contourLineThck[i])*10 + 0.5) % 10 == 0) {
        if (contourLineThck[i] >= 0) sprintf(cht_afs[i], "%s&contourLineThck=%d", cht_afs[i], (int)(contourLineThck[i]+0.5));
        else sprintf(cht_afs[i], "%s&contourLineThck=%d", cht_afs[i], (int)(contourLineThck[i]-0.5));
      }
      else if ((int)(fabs(contourLineThck[i])*100 + 0.5) % 10 == 0) {
        sprintf(cht_afs[i], "%s&contourLineThck=%.1f", cht_afs[i], contourLineThck[i]);
      }
      else {
        sprintf(cht_afs[i], "%s&contourLineThck=%.2f", cht_afs[i], contourLineThck[i]);
      }
    }

    if (strlen(contourLineOptimaDispGbn[i]) > 0) {
      sprintf(cht_afs[i], "%s&contourLineOptimaDispGbn=%s", cht_afs[i], contourLineOptimaDispGbn[i]);
    }

    if (strlen(contourLineTopOptimaColor[i]) > 0) {
      sprintf(cht_afs[i], "%s&contourLineTopOptimaColor=%s", cht_afs[i], contourLineTopOptimaColor[i]);
    }

    if (strlen(contourLineBtmOptimaColor[i]) > 0) {
      sprintf(cht_afs[i], "%s&contourLineBtmOptimaColor=%s", cht_afs[i], contourLineBtmOptimaColor[i]);
    }

    if (contourLineEmphLineGap[i] != -999) {
      sprintf(cht_afs[i], "%s&contourLineEmphLineGap=%d", cht_afs[i], contourLineEmphLineGap[i]);
    }

    if (contourLineGap[i] != -999.) {
      if ((int)(fabs(contourLineGap[i])*10 + 0.5) % 10 == 0) {
        if (contourLineGap[i] >= 0) sprintf(cht_afs[i], "%s&contourLineGap=%d", cht_afs[i], (int)(contourLineGap[i]+0.5));
        else sprintf(cht_afs[i], "%s&contourLineGap=%d", cht_afs[i], (int)(contourLineGap[i]-0.5));
      }
      else if ((int)(fabs(contourLineGap[i])*100 + 0.5) % 10 == 0) {
        sprintf(cht_afs[i], "%s&contourLineGap=%.1f", cht_afs[i], contourLineGap[i]);
      }
      else {
        sprintf(cht_afs[i], "%s&contourLineGap=%.2f", cht_afs[i], contourLineGap[i]);
      }
    }

    if (contourLineBaseVal[i] != -999.) {
      if ((int)(fabs(contourLineBaseVal[i])*10 + 0.5) % 10 == 0) {
        if (contourLineBaseVal[i] >= 0) sprintf(cht_afs[i], "%s&contourLineBaseVal=%d", cht_afs[i], (int)(contourLineBaseVal[i]+0.5));
        else sprintf(cht_afs[i], "%s&contourLineBaseVal=%d", cht_afs[i], (int)(contourLineBaseVal[i]-0.5));
      }
      else if ((int)(fabs(contourLineBaseVal[i])*100 + 0.5) % 10 == 0) {
        sprintf(cht_afs[i], "%s&contourLineBaseVal=%.1f", cht_afs[i], contourLineBaseVal[i]);
      }
      else {
        sprintf(cht_afs[i], "%s&contourLineBaseVal=%.2f", cht_afs[i], contourLineBaseVal[i]);
      }
    }

    if (contourLineTextGap[i] != -999) {
      sprintf(cht_afs[i], "%s&contourLineTextGap=%d", cht_afs[i], contourLineTextGap[i]);
    }

    if (contourLineRangeMinVal[i] != -999.) {
      if ((int)(fabs(contourLineRangeMinVal[i])*10 + 0.5) % 10 == 0) {
        if (contourLineRangeMinVal[i] >= 0) sprintf(cht_afs[i], "%s&contourLineRangeMinVal=%d", cht_afs[i], (int)(contourLineRangeMinVal[i]+0.5));
        else sprintf(cht_afs[i], "%s&contourLineRangeMinVal=%d", cht_afs[i], (int)(contourLineRangeMinVal[i]-0.5));
      }
      else if ((int)(fabs(contourLineRangeMinVal[i])*100 + 0.5) % 10 == 0) {
        sprintf(cht_afs[i], "%s&contourLineRangeMinVal=%.1f", cht_afs[i], contourLineRangeMinVal[i]);
      }
      else {
        sprintf(cht_afs[i], "%s&contourLineRangeMinVal=%.2f", cht_afs[i], contourLineRangeMinVal[i]);
      }
    }

    if (contourLineRangeMaxVal[i] != -999.) {
      if ((int)(fabs(contourLineRangeMaxVal[i])*10 + 0.5) % 10 == 0) {
        if (contourLineRangeMaxVal[i] >= 0) sprintf(cht_afs[i], "%s&contourLineRangeMaxVal=%d", cht_afs[i], (int)(contourLineRangeMaxVal[i]+0.5));
        else sprintf(cht_afs[i], "%s&contourLineRangeMaxVal=%d", cht_afs[i], (int)(contourLineRangeMaxVal[i]-0.5));
      }
      else if ((int)(fabs(contourLineRangeMaxVal[i])*100 + 0.5) % 10 == 0) {
        sprintf(cht_afs[i], "%s&contourLineRangeMaxVal=%.1f", cht_afs[i], contourLineRangeMaxVal[i]);
      }
      else {
        sprintf(cht_afs[i], "%s&contourLineRangeMaxVal=%.2f", cht_afs[i], contourLineRangeMaxVal[i]);
      }
    }

    if (strlen(contourFillColorBarArray[i]) > 0) {
      sprintf(cht_afs[i], "%s&contourFillColorBarArray=%s", cht_afs[i], contourFillColorBarArray[i]);
    }

    if (strlen(repDispCd[i]) > 0) {
      sprintf(cht_afs[i], "%s&repDispCd=%s", cht_afs[i], repDispCd[i]);
    }

    if (contourFillRangeMinVal[i] != -999.) {
      if ((int)(fabs(contourFillRangeMinVal[i])*10 + 0.5) % 10 == 0) {
        if (contourFillRangeMinVal[i] >= 0) sprintf(cht_afs[i], "%s&contourFillRangeMinVal=%d", cht_afs[i], (int)(contourFillRangeMinVal[i]+0.5));
        else sprintf(cht_afs[i], "%s&contourFillRangeMinVal=%d", cht_afs[i], (int)(contourFillRangeMinVal[i]-0.5));
      }
      else if ((int)(fabs(contourFillRangeMinVal[i])*100 + 0.5) % 10 == 0) {
        sprintf(cht_afs[i], "%s&contourFillRangeMinVal=%.1f", cht_afs[i], contourFillRangeMinVal[i]);
      }
      else {
        sprintf(cht_afs[i], "%s&contourFillRangeMinVal=%.2f", cht_afs[i], contourFillRangeMinVal[i]);
      }
    }

    if (contourFillRangeMaxVal[i] != -999.) {
      if ((int)(fabs(contourFillRangeMaxVal[i])*10 + 0.5) % 10 == 0) {
        if (contourFillRangeMaxVal[i] >= 0) sprintf(cht_afs[i], "%s&contourFillRangeMaxVal=%d", cht_afs[i], (int)(contourFillRangeMaxVal[i]+0.5));
        else sprintf(cht_afs[i], "%s&contourFillRangeMaxVal=%d", cht_afs[i], (int)(contourFillRangeMaxVal[i]-0.5));
      }
      else if ((int)(fabs(contourFillRangeMaxVal[i])*100 + 0.5) % 10 == 0) {
        sprintf(cht_afs[i], "%s&contourFillRangeMaxVal=%.1f", cht_afs[i], contourFillRangeMaxVal[i]);
      }
      else {
        sprintf(cht_afs[i], "%s&contourFillRangeMaxVal=%.2f", cht_afs[i], contourFillRangeMaxVal[i]);
      }
    }

    if (contourFillBaseVal[i] != -999.) {
      if ((int)(fabs(contourFillBaseVal[i])*10 + 0.5) % 10 == 0) {
        if (contourFillBaseVal[i] >= 0) sprintf(cht_afs[i], "%s&contourFillBaseVal=%d", cht_afs[i], (int)(contourFillBaseVal[i]+0.5));
        else sprintf(cht_afs[i], "%s&contourFillBaseVal=%d", cht_afs[i], (int)(contourFillBaseVal[i]-0.5));
      }
      else if ((int)(fabs(contourFillGap[i])*100 + 0.5) % 10 == 0) {
        sprintf(cht_afs[i], "%s&contourFillBaseVal=%.1f", cht_afs[i], contourFillBaseVal[i]);
      }
      else {
        sprintf(cht_afs[i], "%s&contourFillBaseVal=%.2f", cht_afs[i], contourFillBaseVal[i]);
      }
    }

    if (strlen(contourFillArray[i]) > 0) {
      sprintf(cht_afs[i], "%s&contourFillArray=%s", cht_afs[i], contourFillArray[i]);
    }

    if (contourFillGap[i] != -999.) {
      if ((int)(fabs(contourFillGap[i])*10 + 0.5) % 10 == 0) {
        if (contourFillGap[i] >= 0) sprintf(cht_afs[i], "%s&contourFillGap=%d", cht_afs[i], (int)(contourFillGap[i]+0.5));
        else sprintf(cht_afs[i], "%s&contourFillGap=%d", cht_afs[i], (int)(contourFillGap[i]-0.5));
      }
      else if ((int)(fabs(contourFillGap[i])*100 + 0.5) % 10 == 0) {
        sprintf(cht_afs[i], "%s&contourFillGap=%.1f", cht_afs[i], contourFillGap[i]);
      }
      else {
        sprintf(cht_afs[i], "%s&contourFillGap=%.2f", cht_afs[i], contourFillGap[i]);
      }
    }

    if (strlen(contourFillMaxValExtdYn[i]) > 0) {
      sprintf(cht_afs[i], "%s&contourFillMaxValExtdYn=%s", cht_afs[i], contourFillMaxValExtdYn[i]);
    }

    if (strlen(contourFillMinValExtdYn[i]) > 0) {
      sprintf(cht_afs[i], "%s&contourFillMinValExtdYn=%s", cht_afs[i], contourFillMinValExtdYn[i]);
    }

    if (strlen(contourFillExtdDirt[i]) > 0) {
      sprintf(cht_afs[i], "%s&contourFillExtdDirt=%s", cht_afs[i], contourFillExtdDirt[i]);
    }

    if (strlen(contourFillColor[i]) > 0) {
      sprintf(cht_afs[i], "%s&contourFillColor=%s", cht_afs[i], contourFillColor[i]);
    }

    if (strlen(curMdlDispType[i]) > 0) {
      sprintf(cht_afs[i], "%s&curMdlDispType=%s", cht_afs[i], curMdlDispType[i]);
    }

    if (strmlnDensity[i] != -999) {
      sprintf(cht_afs[i], "%s&strmlnDensity=%d", cht_afs[i], strmlnDensity[i]);
    }

    if (strmlnResol[i] != -999) {
      sprintf(cht_afs[i], "%s&strmlnResol=%d", cht_afs[i], strmlnResol[i]);
    }

    if (strmlnArrowGap[i] != -999) {
      sprintf(cht_afs[i], "%s&strmlnArrowGap=%d", cht_afs[i], strmlnArrowGap[i]);
    }

    if (strmlnArrowSize[i] != -999) {
      sprintf(cht_afs[i], "%s&strmlnArrowSize=%d", cht_afs[i], strmlnArrowSize[i]);
    }

    if (contourLineSidoSize[i] != -999) {
      sprintf(cht_afs[i], "%s&contourLineSidoSize=%d", cht_afs[i], contourLineSidoSize[i]);
    }

    if (contourLineSidoDistance[i] != -999) {
      sprintf(cht_afs[i], "%s&contourLineSidoDistance=%d", cht_afs[i], contourLineSidoDistance[i]);
    }

    if (strlen(symblColor[i]) > 0) {
      sprintf(cht_afs[i], "%s&symblColor=%s", cht_afs[i], symblColor[i]);
    }

    if (symblSize[i] != -999) {
      sprintf(cht_afs[i], "%s&symblSize=%d", cht_afs[i], symblSize[i]);
    }

    if (strlen(symblDispType[i]) > 0) {
      sprintf(cht_afs[i], "%s&symblDispType=%s", cht_afs[i], symblDispType[i]);
    }

    if (symblGap[i] != -999) {
      //sprintf(cht_afs[i], "%s&symblGap=%d", cht_afs[i], symblGap[i]);
      sprintf(cht_afs[i], "%s&symbl=%d", cht_afs[i], symblGap[i]);
    }

    if (optimaTopFontSize[i] != -999) {
      sprintf(cht_afs[i], "%s&optimaTopFontSize=%d", cht_afs[i], optimaTopFontSize[i]);
    }

    if (optimaMiddleFontSize[i] != -999) {
      sprintf(cht_afs[i], "%s&optimaMiddleFontSize=%d", cht_afs[i], optimaMiddleFontSize[i]);
    }

    if (optimaBottomFontSize[i] != -999) {
      sprintf(cht_afs[i], "%s&optimaBottomFontSize=%d", cht_afs[i], optimaBottomFontSize[i]);
    }

    if (strlen(wthrDataDtlCd[i]) > 0) {
      sprintf(cht_afs[i], "%s&wthrDataDtlCd=%s", cht_afs[i], wthrDataDtlCd[i]);
    }

    if (strlen(contourLineArray[i]) > 0) {
      sprintf(cht_afs[i], "%s&contourLineArray=%s", cht_afs[i], contourLineArray[i]);
    }

    if (strlen(menu04[i]) > 0) {
      sprintf(cht_afs[i], "%s&menu04=%s", cht_afs[i], menu04[i]);
    }

    if (symbolScale[i] != -999) {
      sprintf(cht_afs[i], "%s&symbolScale=%d", cht_afs[i], symbolScale[i]);
    }

    if (strmlnThck[i] != -999.) {
      if ((int)(fabs(strmlnThck[i])*10 + 0.5) % 10 == 0) {
        if (strmlnThck[i] >= 0) sprintf(cht_afs[i], "%s&strmlnThck=%d", cht_afs[i], (int)(strmlnThck[i]+0.5));
        else sprintf(cht_afs[i], "%s&strmlnThck=%d", cht_afs[i], (int)(strmlnThck[i]-0.5));
      }
      else if ((int)(fabs(strmlnThck[i])*100 + 0.5) % 10 == 0) {
        sprintf(cht_afs[i], "%s&strmlnThckl=%.1f", cht_afs[i], strmlnThck[i]);
      }
      else {
        sprintf(cht_afs[i], "%s&strmlnThck=%.2f", cht_afs[i], strmlnThck[i]);
      }
    }

    if (basicPartSmtLvl[i] != -999) {
      sprintf(cht_afs[i], "%s&basicPartSmtLvl=%d", cht_afs[i], basicPartSmtLvl[i]);
    }

    if (basicTotSmtLvl[i] != -999) {
      sprintf(cht_afs[i], "%s&basicTotSmtLvl=%d", cht_afs[i], basicTotSmtLvl[i]);
    }

    if (basicSmtLvl[i] != -999) {
      sprintf(cht_afs[i], "%s&basicSmtLvl=%d", cht_afs[i], basicSmtLvl[i]);
    }

    if (!strcmp(repDispCd[i], "F") && strstr(var.cht_name, "raster")) {
      sprintf(cht_afs[i], "%s&isRasterFillCheck=R", cht_afs[i]);
    }
  }


  if (var.flag == 1 && strcmp(var.cht_mode, "gts")) {
    for(i = 0; i < num_layer; i++) {
      printf("%s\n\n", cht_afs[i]);
    }
    return;
  }


////////////////////////////////////////////////////////////////////////////////////////
//                              CURL 처리(병렬 호출)                                      //
////////////////////////////////////////////////////////////////////////////////////////
  if (var.curl == 1) {
    for(i = 0; i < num_layer; i++) {
      // Allocate one CURL handle per transfer
      handles[i] = curl_easy_init();
      // set the option
      sprintf(url, "%s&meta=2", cht_afs[i]);
      curl_easy_setopt(handles[i], CURLOPT_URL, url);
      //curl_easy_setopt(handles[i], CURLOPT_HEADER, 0);
      curl_easy_setopt(handles[i], CURLOPT_WRITEFUNCTION, donotprint);
    }

    // init a multi stack
    multi_handle = curl_multi_init();

    // add the individual transfers
    for(i = 0; i < num_layer; i++)
      curl_multi_add_handle(multi_handle, handles[i]);

    // we start some action by calling perform right away
    curl_multi_perform(multi_handle, &still_running);

    while(still_running) {
      struct timeval timeout;
      int rc; // select() return code 
      CURLMcode mc; // curl_multi_fdset() return code 
 
      fd_set fdread;
      fd_set fdwrite;
      fd_set fdexcep;
      int maxfd = -1;
 
      long curl_timeo = -1;
 
      FD_ZERO(&fdread);
      FD_ZERO(&fdwrite);
      FD_ZERO(&fdexcep);

      timeout.tv_sec = 30;
      timeout.tv_usec = 0;

      curl_multi_timeout(multi_handle, &curl_timeo);
      if(curl_timeo >= 0) {
        timeout.tv_sec = curl_timeo / 1000;
        if(timeout.tv_sec > 1)
          timeout.tv_sec = 1;
        else
          timeout.tv_usec = (curl_timeo % 1000) * 1000;
      }

      // get file descriptors from the transfers
      mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);
 
      if(mc != CURLM_OK) {
        //fprintf(stderr, "curl_multi_fdset() failed, code %d.\n", mc);
        break;
      }

      if(maxfd == -1) {
        struct timeval wait = { 0, 100 * 1000 }; // 100ms
        rc = select(0, NULL, NULL, NULL, &wait);
      }
      else {
        rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
      }

      switch(rc) {
        case -1: // select error
          break;
        case 0:  // timeout
        default: // action
          curl_multi_perform(multi_handle, &still_running);
          break;
      }
    }

/*
    while((msg = curl_multi_info_read(multi_handle, &msgs_left))) {
      if(msg->msg == CURLMSG_DONE) {
        int idx;
 
        // Find out which handle this message is about
        for(idx = 0; idx<max_layer; idx++) {
          int found = (msg->easy_handle == handles[idx]);
          if(found)
            break;
        }
 
        printf("%d: %d\n\n", idx, msg->data.result);
      }
    }
*/

    curl_multi_cleanup(multi_handle);
 
    /* Free the CURL handles */
    for(i = 0; i < num_layer; i++) {
      curl_easy_cleanup(handles[i]);
    }
  }


////////////////////////////////////////////////////////////////////////////////////////
//                              JSON 처리                                     //
////////////////////////////////////////////////////////////////////////////////////////
  for (i = 0; i < num_layer; i++) {
    sprintf(url, "%s&meta=2", cht_afs[i]);
    //printf("%s\n", url);

    if ((fr = url_fopen(url, "r"))) {
      url_fgets(json, sizeof(json), fr);
      url_fclose(fr);

      root = cJSON_Parse(json);
      if (root != NULL) {
        data = cJSON_GetObjectItem(root, "data");
        if (data != NULL) {
          result = cJSON_GetObjectItem(data, "result");
          tmp = cJSON_GetObjectItem(result, "leftTopGisX"); 
          if (tmp != NULL) {
            strcpy(buf, cJSON_Print(tmp));
            if (buf[0] == '"') {
              getword(value, buf+1, '"');
            }
            else strcpy(value, buf);
            gisStartx[i] = atof(value);
          }

          tmp = cJSON_GetObjectItem(result, "leftTopGisY"); 
          if (tmp != NULL) {
            strcpy(buf, cJSON_Print(tmp));
            if (buf[0] == '"') {
              getword(value, buf+1, '"');
            }
            else strcpy(value, buf);
            gisStarty[i] = atof(value);
          }

          tmp = cJSON_GetObjectItem(result, "rightBottomGisX"); 
          if (tmp != NULL) {
            strcpy(buf, cJSON_Print(tmp));
            if (buf[0] == '"') {
              getword(value, buf+1, '"');
            }
            else strcpy(value, buf);
            gisEndx[i] = atof(value);
          }

          tmp = cJSON_GetObjectItem(result, "rightBottomGisY"); 
          if (tmp != NULL) {
            strcpy(buf, cJSON_Print(tmp));
            if (buf[0] == '"') {
              getword(value, buf+1, '"');
            }
            else strcpy(value, buf);
            gisEndy[i] = atof(value);
          }

          tmp = cJSON_GetObjectItem(result, "imgFileUrl"); 
          if (tmp != NULL) {
            strcpy(buf, cJSON_Print(tmp));
            if (buf[0] == '"') {
              getword(value, buf+1, '"');
            }
            else strcpy(value, buf);
            strcpy(imgFile[i], value);
            fp2 = fopen(imgFile[i], "rb");
            if (fp2) {
              im_cht[i] = gdImageCreateFromPng(fp2);
              fclose(fp2);
            }
          }
  
          // 색상표 정보
          if (i == 0 || (strstr(var.cht_name, "cvrain") || strstr(var.cht_name, "acsnow") && i == 1)) {
            tmp = cJSON_GetObjectItem(result, "mdlVarUnit"); 
            if (tmp != NULL) {
              strcpy(buf, cJSON_Print(tmp));
              if (buf[0] == '"') {
                getword(value, buf+1, '"');
              }
              else strcpy(value, buf);
              strcpy(mdlVarUnit, value);
            }

            colorBarDatas = cJSON_GetObjectItem(result, "returnColorBarDatas");       
            if (colorBarDatas != NULL) {
              arr_size = cJSON_GetArraySize(colorBarDatas);
              for (j = 0; j < arr_size; j++) {
                tmp = cJSON_GetArrayItem(colorBarDatas, j); 
                if (tmp != NULL) {
                  tmp2 = cJSON_GetObjectItem(tmp, "lvlVal");  
                  if (tmp2 != NULL) {
                    strcpy(buf, cJSON_Print(tmp2));
                    if (buf[0] == '"') {
                      getword(value, buf+1, '"');
                    }
                    else strcpy(value, buf);
                    vbar[j] = atof(value);
                    if (j > 0 && vbar[j] != vbar[j-1]) {
                      nbar = j+1;
                    }
                  }

                  tmp2 = cJSON_GetObjectItem(tmp, "redCd");  
                  if (tmp2 != NULL) {
                    strcpy(buf, cJSON_Print(tmp2));
                    if (buf[0] == '"') {
                      getword(value, buf+1, '"');
                    }
                    else strcpy(value, buf);
                    vred[j] = atoi(value);
                  }

                  tmp2 = cJSON_GetObjectItem(tmp, "greenCd");  
                  if (tmp2 != NULL) {
                    strcpy(buf, cJSON_Print(tmp2));
                    if (buf[0] == '"') {
                      getword(value, buf+1, '"');
                    }
                    else strcpy(value, buf);
                    vgreen[j] = atoi(value);
                  }

                  tmp2 = cJSON_GetObjectItem(tmp, "blueCd");  
                  if (tmp2 != NULL) {
                    strcpy(buf, cJSON_Print(tmp2));
                    if (buf[0] == '"') {
                      getword(value, buf+1, '"');
                    }
                    else strcpy(value, buf);
                    vblue[j] = atoi(value);
                  }
                }
              }
            }
          }        
        }
        cJSON_Delete(root);
      }
    }
  }


////////////////////////////////////////////////////////////////////////////////////////
//                                  이미지 병합                                        //
////////////////////////////////////////////////////////////////////////////////////////
  if (num_layer > 0 && im_cht[0] && var.NI > 0 && var.NJ > 0) {
    diffx[0] = (gisStartx[0] - startx)/(gisStartx[0] - gisEndx[0])*gdImageSX(im_cht[0]);
    diffy[0] = (gisStarty[0] - starty)/(gisStarty[0] - gisEndy[0])*gdImageSY(im_cht[0]);

    if (gisEndx[0] >= endx) {
      diffx2[0] = (endx - gisEndx[0])/(gisStartx[0] - gisEndx[0])*gdImageSX(im_cht[0]);
      if (gisStartx[0] >= startx) ratex[0] = (gisStartx[0] - endx)/(startx - endx);
      else ratex[0] = (startx - endx)/(startx - endx);
    }
    else {
      diffx2[0] = 0;
      if (gisStartx[0] >= startx) ratex[0] = (gisStartx[0] - gisEndx[0])/(startx - endx);
      else ratex[0] = (startx - gisEndx[0])/(startx - endx);
    }

    if (gisEndy[0] >= endy) {
      diffy2[0] = 0;
      if (gisStarty[0] >= starty) ratey[0] = (starty - gisEndy[0])/(starty - endy);
      else ratey[0] = (gisStarty[0] - gisEndy[0])/(starty - endy);
    }
    else {
      diffy2[0] = (endy - gisEndy[0])/(gisStarty[0] - gisEndy[0])*gdImageSY(im_cht[0]);
      if (gisStarty[0] >= starty) ratey[0] = (starty - endy)/(starty - endy);
      else ratey[0] = (gisStarty[0] - endy)/(starty - endy);
    }
  }

  if (num_layer >= 2 && im_cht[0]) {
    for (i = 1; i < num_layer; i++) {
      if (!im_cht[i]) continue;

      if (gisStartx[0] != gisStartx[i] || gisStarty[0] != gisStarty[i] || gisEndx[0] != gisEndx[i] || gisEndy[0] != gisEndy[i]) {
        diffx[i] = (gisStartx[i] - gisStartx[0])/(gisStartx[i] - gisEndx[i])*gdImageSX(im_cht[i]);
        diffy[i] = (gisStarty[i] - gisStarty[0])/(gisStarty[i] - gisEndy[i])*gdImageSY(im_cht[i]);

        if (gisEndx[i] >= gisEndx[0]) {
          diffx2[i] = (gisEndx[0] - gisEndx[i])/(gisStartx[i] - gisEndx[i])*gdImageSX(im_cht[i]);
          if (gisStartx[i] >= gisStartx[0]) ratex[i] = (gisStartx[i] - gisEndx[0])/(gisStartx[0] - gisEndx[0]);
          else ratex[i] = (gisStartx[0] - gisEndx[0])/(gisStartx[0] - gisEndx[0]);
        }
        else {
          diffx2[i] = 0;
          if (gisStartx[i] >= gisStartx[0]) ratex[i] = (gisStartx[i] - gisEndx[i])/(gisStartx[0] - gisEndx[0]);
          else ratex[i] = (gisStartx[0] - gisEndx[i])/(gisStartx[0] - gisEndx[0]);
        }

        if (gisEndy[i] >= gisEndy[0]) {
          diffy2[i] = 0;
          if (gisStarty[i] >= gisStarty[0]) ratey[i] = (gisStarty[0] - gisEndy[i])/(gisStarty[0] - gisEndy[0]);
          else ratey[i] = (gisStarty[i] - gisEndy[i])/(gisStarty[0] - gisEndy[0]);
        }
        else {
          diffy2[i] = (gisEndy[0] - gisEndy[i])/(gisStarty[i] - gisEndy[i])*gdImageSY(im_cht[i]);
          if (gisStarty[i] >= gisStarty[0]) ratey[i] = (gisStarty[0] - gisEndy[0])/(gisStarty[0] - gisEndy[0]);
          else ratey[i] = (gisStarty[i] - gisEndy[0])/(gisStarty[0] - gisEndy[0]);
        }

        if (diffx[i] < 0 && diffy[i] < 0) {
          diffx[i] *= gdImageSX(im_cht[0])*ratex[i]/gdImageSX(im_cht[i]);
          diffy[i] *= gdImageSY(im_cht[0])*ratey[i]/gdImageSY(im_cht[i]);
          gdImageCopyResampled(im_cht[0], im_cht[i], -diffx[i], -diffy[i], 0, 0, gdImageSX(im_cht[0])*ratex[i], gdImageSY(im_cht[0])*ratey[i], gdImageSX(im_cht[i]), gdImageSY(im_cht[i]));
        }
        else if (diffx[i] < 0) {
          diffx[i] *= gdImageSX(im_cht[0])*ratex[i]/gdImageSX(im_cht[i]);
          gdImageCopyResampled(im_cht[0], im_cht[i], -diffx[i], 0, 0, diffy[i], gdImageSX(im_cht[0])*ratex[i], gdImageSY(im_cht[0])*ratey[i], gdImageSX(im_cht[i]), gdImageSY(im_cht[i])-diffy[i]-diffy2[i]);
        }
        else if (diffy[i] < 0) {
          diffy[i] *= gdImageSY(im_cht[0])*ratey[i]/gdImageSY(im_cht[i]);
          gdImageCopyResampled(im_cht[0], im_cht[i], 0, -diffy[i], diffx[i], 0, gdImageSX(im_cht[0])*ratex[i], gdImageSY(im_cht[0])*ratey[i], gdImageSX(im_cht[i])-diffx[i]-diffx2[i], gdImageSY(im_cht[i]));
        }
        else gdImageCopyResampled(im_cht[0], im_cht[i], 0, 0, diffx[i], diffy[i], gdImageSX(im_cht[0])*ratex[i], gdImageSY(im_cht[0])*ratey[i], gdImageSX(im_cht[i])-diffx[i]-diffx2[i], gdImageSY(im_cht[i])-diffy[i]-diffy2[i]);
      }
      else {
        if (zmlvl[i] != zmlvl[0]) gdImageCopyResampled(im_cht[0], im_cht[i], 0, 0, 0, 0, gdImageSX(im_cht[0]), gdImageSY(im_cht[0]), gdImageSX(im_cht[i]), gdImageSY(im_cht[i]));
        else gdImageCopy(im_cht[0], im_cht[i], 0, 0, 0, 0, gdImageSX(im_cht[i]), gdImageSY(im_cht[i]));
      }
    }
  }

  if (num_layer > 0 && im_cht[0]) {
    if (diffx[0] < 0 && diffy[0] < 0) {
      diffx[0] *= (var.NI)*ratex[0]/gdImageSX(im_cht[0]);
      diffy[0] *= (var.NJ)*ratey[0]/gdImageSY(im_cht[0]);
      if (var.gis == 0) {
        gdImageCopyResampled(im, im_cht[0], var.OI-diffx[0], var.OJ-diffy[0], 0, 0, (var.NI)*ratex[0], (var.NJ)*ratey[0], gdImageSX(im_cht[0]), gdImageSY(im_cht[0]));
      }
      else {
        gdImageCopyResampled2(im, im_cht[0], var.OI-diffx[0], var.OJ-diffy[0], 0, 0, (var.NI)*ratex[0], (var.NJ)*ratey[0], gdImageSX(im_cht[0]), gdImageSY(im_cht[0]));
      }
    }
    else if (diffx[0] < 0) {
      diffx[0] *= (var.NI)*ratex[0]/gdImageSX(im_cht[0]);
      if (var.gis == 0) {
        gdImageCopyResampled(im, im_cht[0], var.OI-diffx[0], var.OJ, 0, diffy[0], (var.NI)*ratex[0], (var.NJ)*ratey[0], gdImageSX(im_cht[0]), gdImageSY(im_cht[0])-diffy[0]-diffy2[0]);
      }
      else {
        gdImageCopyResampled2(im, im_cht[0], var.OI-diffx[0], var.OJ, 0, diffy[0], (var.NI)*ratex[0], (var.NJ)*ratey[0], gdImageSX(im_cht[0]), gdImageSY(im_cht[0])-diffy[0]-diffy2[0]);
      }
    }
    else if (diffy[0] < 0) {
      diffy[0] *= (var.NJ)*ratey[0]/gdImageSY(im_cht[0]);
      if (var.gis == 0) {
        gdImageCopyResampled(im, im_cht[0], var.OI, var.OJ-diffy[0], diffx[0], 0, (var.NI)*ratex[0], (var.NJ)*ratey[0], gdImageSX(im_cht[0])-diffx[0]-diffx2[0], gdImageSY(im_cht[0]));
      }
      else {
        gdImageCopyResampled2(im, im_cht[0], var.OI, var.OJ-diffy[0], diffx[0], 0, (var.NI)*ratex[0], (var.NJ)*ratey[0], gdImageSX(im_cht[0])-diffx[0]-diffx2[0], gdImageSY(im_cht[0]));
      }
    }
    else {
      if (var.gis == 0) {
        gdImageCopyResampled(im, im_cht[0], var.OI, var.OJ, diffx[0], diffy[0], (var.NI)*ratex[0], (var.NJ)*ratey[0], gdImageSX(im_cht[0])-diffx[0]-diffx2[0], gdImageSY(im_cht[0])-diffy[0]-diffy2[0]);
      }
      else {
        gdImageCopyResampled2(im, im_cht[0], var.OI, var.OJ, diffx[0], diffy[0], (var.NI)*ratex[0], (var.NJ)*ratey[0], gdImageSX(im_cht[0])-diffx[0]-diffx2[0], gdImageSY(im_cht[0])-diffy[0]-diffy2[0]);
      }
    }
  }

  for (i = 0; i < num_layer; i++) {
    if (im_cht[i]) gdImageDestroy(im_cht[i]);
  }


  if (var.legend_only == 1) {
    ////////////////////////////////////////////////////////////////////////////////////////
    //                                  범례 생성                                         //
    ////////////////////////////////////////////////////////////////////////////////////////
    if (!strcmp(var.cht_mode, "fct") || !strcmp(var.cht_mode, "fct2") || !strcmp(var.cht_mode, "ana") || !strcmp(var.cht_mode, "ana3")) {
      sprintf(title2, "(%s)", mdlVarUnit);
      gdImageString(im, gdFontSmall, 0, 1, title2, color_lvl[244]);      

      for (i = 0; i < nbar; i++) {
        if (i < nbar - 1) {
          color = gdImageColorAllocate(im, vred[i], vgreen[i], vblue[i]);
          gdImageFilledRectangle(im, var.GI-45, var.OJ+(float)(var.NJ-1)*((float)(nbar-i-2)/(float)(nbar-1)), var.GI-30, var.OJ+(float)(var.NJ-1)*((float)(nbar-i-1)/(float)(nbar-1)), color);
          gdImageRectangle(im, var.GI-45, var.OJ+(float)(var.NJ-1)*((float)(nbar-i-2)/(float)(nbar-1)), var.GI-30, var.OJ+(float)(var.NJ-1)*((float)(nbar-i-1)/(float)(nbar-1)), color_lvl[244]);
        }

        if ((int)(fabs(vbar[i])*10 + 0.5) % 10 == 0) {
          if (vbar[i] >= 0) sprintf(value, "%d", (int)(vbar[i]+0.5));
          else sprintf(value, "%d", (int)(vbar[i]-0.5));
        }
        else if ((int)(fabs(vbar[i])*100 + 0.5) % 10 == 0) {
          sprintf(value, "%.1f", vbar[i]);
        }
        else {
          sprintf(value, "%.2f", vbar[i]);
        }
        gdImageString(im, gdFontSmall, var.GI-27, var.OJ+(float)(var.NJ-1)*((float)(nbar-i-1)/(float)(nbar-1))-6, value, color_lvl[244]);
      }
    }
  }
  else if (var.gis == 0) {
    ////////////////////////////////////////////////////////////////////////////////////////
    //                                  범례 생성                                         //
    ////////////////////////////////////////////////////////////////////////////////////////
    if (!strcmp(var.cht_mode, "fct") || !strcmp(var.cht_mode, "fct2") || !strcmp(var.cht_mode, "ana") || !strcmp(var.cht_mode, "ana3")) {
      for (i = 0; i < nbar; i++) {
        if (i < nbar - 1) {
          color = gdImageColorAllocate(im, vred[i], vgreen[i], vblue[i]);
          gdImageFilledRectangle(im, var.GI-45, var.OJ+(float)(var.NJ-1)*((float)(nbar-i-2)/(float)(nbar-1)), var.GI-30, var.OJ+(float)(var.NJ-1)*((float)(nbar-i-1)/(float)(nbar-1)), color);
          gdImageRectangle(im, var.GI-45, var.OJ+(float)(var.NJ-1)*((float)(nbar-i-2)/(float)(nbar-1)), var.GI-30, var.OJ+(float)(var.NJ-1)*((float)(nbar-i-1)/(float)(nbar-1)), color_lvl[244]);
        }

        if ((int)(fabs(vbar[i])*10 + 0.5) % 10 == 0) {
          if (vbar[i] >= 0) sprintf(value, "%d", (int)(vbar[i]+0.5));
          else sprintf(value, "%d", (int)(vbar[i]-0.5));
        }
        else if ((int)(fabs(vbar[i])*100 + 0.5) % 10 == 0) {
          sprintf(value, "%.1f", vbar[i]);
        }
        else {
          sprintf(value, "%.2f", vbar[i]);
        }
        gdImageString(im, gdFontSmall, var.GI-27, var.OJ+(float)(var.NJ-1)*((float)(nbar-i-1)/(float)(nbar-1))-6, value, color_lvl[244]);
      }
    }


    ////////////////////////////////////////////////////////////////////////////////////////
    //                                  제목 및 시간입력                                 //
    ////////////////////////////////////////////////////////////////////////////////////////
    if (!strcmp(model, "G"))
      strcpy(title2, "GDAPS(UM)");
    else if (!strcmp(model, "E"))
      strcpy(title2, "ECMWF");
    else if (!strcmp(model, "GK"))
      strcpy(title2, "GDAPS(KIM)");

    if((!strcmp(var.cht_mode, "ana") || !strcmp(var.cht_mode, "ana2") || !strcmp(var.cht_mode, "ana3")) && (!strcmp(level, "rmir") || !strcmp(level, "rmwv"))) {
      strcpy(title2, "OBS");
    }
  
    if (!strcmp(var.cht_mode, "fct") || !strcmp(var.cht_mode, "fct2")) {
      seq2time(var.seq+9*60, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
      sprintf(title3, "       %02d %02d %s %04d KST(+%02dH)", HH, DD, get_month(MM), YY, (var.seq-var.seq_fc)/60);
      seq2time(var.seq_fc+9*60, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
      sprintf(title4, "      %02d %02d %s %04d KST", HH, DD, get_month(MM), YY);
      seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
      sprintf(title5, "VALID: %02d %02d %s %04d UTC(+%02dH)", HH, DD, get_month(MM), YY, (var.seq-var.seq_fc)/60);
      seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
      sprintf(title6, "TIME: %02d %02d %s %04d UTC", HH, DD, get_month(MM), YY);
    }
    else if (!strcmp(var.cht_mode, "ana") || !strcmp(var.cht_mode, "ana2") || !strcmp(var.cht_mode, "ana3")) {
      seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
      sprintf(title5, "%02d %02d %s %04d UTC ANALYSIS CHART", HH, DD, get_month(MM), YY);
      seq2time(var.seq + 9*60, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
      sprintf(title6, "      %02d %02d %s %04d KST", HH, DD, get_month(MM), YY);
    }


  ////////////////////////////////////////////////////////////////////////////////////////
  //                              이미지 String 조정                                     //
  ////////////////////////////////////////////////////////////////////////////////////////
    if (!strcmp(var.cht_mode, "fct") || !strcmp(var.cht_mode, "fct2") || !strcmp(var.cht_mode, "ana") || !strcmp(var.cht_mode, "ana2") || !strcmp(var.cht_mode, "ana3")) {
      gdImageString(im, gdFontLarge, var.GI-strlen(title2)*5-85, 1, title2, color_lvl[244]);
    }

    if (!strcmp(var.cht_mode, "fct") || !strcmp(var.cht_mode, "fct2")) {
      if(!strcmp(model, "G")){
        if(!strcmp(level, "rmir") || !strcmp(level, "rmwv")) {
          gdImageString(im, gdFontLarge, 5, 1, title, color_lvl[244]);
        }
        else if(!strcmp(var.map, "E10") && !strcmp(var.cht_mode, "fct")){
          //sprintf(title, "%s", title, (var.seq-var.seq_fc)/60-1, (var.seq-var.seq_fc)/60);
          gdImageString(im, gdFontLarge, 5, 1, title, color_lvl[244]);
        }
        else {
          if((var.seq-var.seq_fc)/60 > 84 && !strcmp(level, "pres")){
            sprintf(title, "%s [%03d-%03d]", title, (var.seq-var.seq_fc)/60-6, (var.seq-var.seq_fc)/60);
            gdImageString(im, gdFontLarge, 5, 1, title, color_lvl[244]);
          }
          else{
            sprintf(title, "%s [%03d-%03d]", title, (var.seq-var.seq_fc)/60-3, (var.seq-var.seq_fc)/60);
            gdImageString(im, gdFontLarge, 5, 1, title, color_lvl[244]);
          }
        }
      }
      else if(!strcmp(model, "GK")){
        if(!strcmp(level, "rmir") || !strcmp(level, "rmwv")) {
          gdImageString(im, gdFontLarge, 5, 1, title, color_lvl[244]);
        }
        else if(!strcmp(var.map, "E10") && !strcmp(var.cht_mode, "fct")){
          //sprintf(title, "%s [%03d-%03d]", title, (var.seq-var.seq_fc)/60-1, (var.seq-var.seq_fc)/60);
          gdImageString(im, gdFontLarge, 5, 1, title, color_lvl[244]);
        }
        else {
          if((var.seq-var.seq_fc)/60 > 168 && !strcmp(level, "pres")){
            sprintf(title, "%s [%03d-%03d]", title, (var.seq-var.seq_fc)/60-6, (var.seq-var.seq_fc)/60);
            gdImageString(im, gdFontLarge, 5, 1, title, color_lvl[244]);
          }
          else{
            sprintf(title, "%s [%03d-%03d]", title, (var.seq-var.seq_fc)/60-3, (var.seq-var.seq_fc)/60);
            gdImageString(im, gdFontLarge, 5, 1, title, color_lvl[244]);
          }
        }
      }
      else if(!strcmp(model, "E")){
        if(!strcmp(var.map, "E10") && !strcmp(var.cht_mode, "fct")){
          if((var.seq-var.seq_fc)/60 > 144){
            sprintf(title, "%s [%03d-%03d]", title, (var.seq-var.seq_fc)/60-6, (var.seq-var.seq_fc)/60);
            gdImageString(im, gdFontLarge, 5, 1, title, color_lvl[244]);
          }
          else if((var.seq-var.seq_fc)/60 > 90){
            sprintf(title, "%s [%03d-%03d]", title, (var.seq-var.seq_fc)/60-3, (var.seq-var.seq_fc)/60);
            gdImageString(im, gdFontLarge, 5, 1, title, color_lvl[244]);
          }
          else{
            sprintf(title, "%s [%03d-%03d]", title, (var.seq-var.seq_fc)/60-1, (var.seq-var.seq_fc)/60);
            gdImageString(im, gdFontLarge, 5, 1, title, color_lvl[244]);
          }
        }
        else {
          if((var.seq-var.seq_fc)/60 > 144){
            sprintf(title, "%s [%03d-%03d]", title, (var.seq-var.seq_fc)/60-6, (var.seq-var.seq_fc)/60);
            gdImageString(im, gdFontLarge, 5, 1, title, color_lvl[244]);
          }
          else{
            sprintf(title, "%s [%03d-%03d]", title, (var.seq-var.seq_fc)/60-3, (var.seq-var.seq_fc)/60);
            gdImageString(im, gdFontLarge, 5, 1, title, color_lvl[244]);
          }
        }
      }
    }
    else if (!strcmp(var.cht_mode, "ana") || !strcmp(var.cht_mode, "ana2") || !strcmp(var.cht_mode, "ana3")) {
      gdImageString(im, gdFontLarge, 5, 1, title, color_lvl[244]);
    }

    if (!strcmp(var.cht_mode, "fct") || !strcmp(var.cht_mode, "fct2")) {
      gdImageString(im, gdFontLarge, 5, var.NJ+var.OJ+18, title3, color_lvl[244]);
      gdImageString(im, gdFontLarge, var.GI-250, var.NJ+var.OJ+18, title4, color_lvl[244]);
      gdImageString(im, gdFontLarge, 5, var.NJ+var.OJ+3, title5, color_lvl[246]);
      gdImageString(im, gdFontLarge, var.GI-250, var.NJ+var.OJ+3, title6, color_lvl[246]);
    }
    else if (!strcmp(var.cht_mode, "ana") || !strcmp(var.cht_mode, "ana2") || !strcmp(var.cht_mode, "ana3")) {
      gdImageString(im, gdFontLarge, 5, var.NJ+var.OJ+3, title5, color_lvl[246]);
      gdImageString(im, gdFontLarge, var.GI-250, var.NJ+var.OJ+3, title6, color_lvl[246]);
    }

  }


////////////////////////////////////////////////////////////////////////////////////////
//                              이미지 저장 여부                                     //
////////////////////////////////////////////////////////////////////////////////////////
  save_ok = 0;
  if (num_layer == 0) {
    if ((!strcmp(level, "rmir") || !strcmp(level, "rmwv"))) save_ok = 1;
  }
  else {
    for (i = 0; i < num_layer; i++) {
      if (im_cht[i]) save_ok++;
    }

    if (save_ok == num_layer) save_ok = 1;
    else save_ok = 0;

    if (var.legend_only == 1 || (ratex[0] > 0 && ratey[0] > 0)) {
    }
    else save_ok = 0;
  }

  // idx 파일 오류 시 삭제
  if (save_ok == 0 && var.curl == 1) {
    if (!strcmp(model, "G") || !strcmp(model, "E")) {
      seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
      if (!strcmp(var.map, "E10")) {
        if (!strcmp(model, "G")) sprintf(url, "http://afsw.kma.go.kr/php/cht/cht_afs_del_grb_idx.php?tmfc=%04d%02d%02d%02d&hf=%d&model=G10", YY, MM, DD, HH, (var.seq-var.seq_fc)/60);
        else if (!strcmp(model, "GK")) sprintf(url, "http://afsw.kma.go.kr/php/cht/cht_afs_del_grb_idx.php?tmfc=%04d%02d%02d%02d&hf=%d&model=GK10", YY, MM, DD, HH, (var.seq-var.seq_fc)/60);
        else if (!strcmp(model, "E"))  sprintf(url, "http://afsw.kma.go.kr/php/cht/cht_afs_del_grb_idx.php?tmfc=%04d%02d%02d%02d&hf=%d&model=E10", YY, MM, DD, HH, (var.seq-var.seq_fc)/60);
      }
      else {
        if (!strcmp(model, "G")) {
          if (!strcmp(level, "unis"))      sprintf(url, "http://afsw.kma.go.kr/php/cht/cht_afs_del_grb_idx.php?tmfc=%04d%02d%02d%02d&hf=%d&model=UM&level=unis", YY, MM, DD, HH, (var.seq-var.seq_fc)/60);
          else if (!strcmp(level, "pres")) sprintf(url, "http://afsw.kma.go.kr/php/cht/cht_afs_del_grb_idx.php?tmfc=%04d%02d%02d%02d&hf=%d&model=UM&level=pres", YY, MM, DD, HH, (var.seq-var.seq_fc)/60);
          else if (!strcmp(level, "isen")) sprintf(url, "http://afsw.kma.go.kr/php/cht/cht_afs_del_grb_idx.php?tmfc=%04d%02d%02d%02d&hf=%d&model=UM&level=isen", YY, MM, DD, HH, (var.seq-var.seq_fc)/60);
        }
        else if (!strcmp(model, "E"))   sprintf(url, "http://afsw.kma.go.kr/php/cht/cht_afs_del_grb_idx.php?tmfc=%04d%02d%02d%02d&hf=%d&model=ECMWF", YY, MM, DD, HH, (var.seq-var.seq_fc)/60);
      }

      if (fr = url_fopen(url, "r")) {
        while (!url_feof(fr)) {
          url_fgets(buf, sizeof(buf), fr);
        }
        url_fclose(fr);
      }
    }
  }

  // 파일명 원복
  seq2time(var.seq_fc, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
  if (MM <= 4 || MM >= 11) {
    strcpy(var.cht_name, str_replace("300_", "200_", var.cht_name));
  }

  // 분석장의 경우 현재시각으로부터 3시간 지나지 않은 자료는 저장하지 않음
  if (!strcmp(var.cht_mode, "ana") || !strcmp(var.cht_mode, "ana2") || !strcmp(var.cht_mode, "ana3")) {
    get_time(&YY, &MM, &DD, &HH, &MI, &SS);
    seq = time2seq(YY, MM, DD, HH, MI, 'm');

    if ((var.seq + 9*60) > (seq - 3*60)) save_ok = 0;
  }

  return save_ok;
}