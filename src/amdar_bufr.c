/*******************************************************************************
**
**  AMDAR 해독
**
**=============================================================================*
**
**     o 작성자 : 이창재 (2022.1.21)
**
********************************************************************************/
#include "amdar_bufr.h"

// 변수
struct INPUT_VAR var;  // 지점 자료

// 함수
/*******************************************************************************
 *
 *  MAIN
 *
 *******************************************************************************/
int main()
{
  int   err = 0;

  // 1. 초기화
  setvbuf(stdout, NULL, _IONBF, 0);
  alarm(30);

  printf("HTTP/1.0 200 OK\n");
  printf("Content-type: text/plain\n\n");

  // 2. 사용자 입력 변수 분석
  if ( user_input() < 0 ) {
    printf("user_input() error\n");
    return -1;
  }

  // 3. 자료 표출
  if (var.flag == 0) amdar_data();
  else if (var.flag == 1) amdar_aircraft();
  else if (var.flag == 2) amdar_keys();
  else if (var.flag == 3) amdar_files();

  alarm(0);
  return 0;
}

/*******************************************************************************
 *  격자자료 웹 이미지 표출시 사용자 요청 분석 부분
 *******************************************************************************/
int user_input() {
  char *qs;
  char tmp[256], item[32], value[256], tm[30], tm_fc[30];
  int  iYY, iMM, iDD, iHH, iMI, iSS;
  int  iseq, i, j, k;

  // 1. 변수 초기값 : 자료별 처리 프로그램에서 각자 상황에 맞게 설정
  var.flag = 0;
  var.pa = 0;
  var.lat1 = 0;
  var.lat2 = 70;
  var.lon1 = 50;
  var.lon2 = 180;
  var.disp = 0;
  var.mode = 0;
  var.itv = 60;
  strcpy(var.map, "");

  // 2. GET 방식으로 전달된 사용자 입력변수들의 해독
  qs = getenv ("QUERY_STRING");
  if (qs == NULL) return -1;

  for (i = 0; qs[0] != '\0'; i++) {
    getword (value, qs, '&');
    getword (item, value, '=');
    if (strlen(value) == 0) continue;

    if      ( !strcmp(item,"tm"))        strcpy(tm, value);
    else if ( !strcmp(item,"lat1"))      var.lat1 = atof(value);
    else if ( !strcmp(item,"lon1"))      var.lon1 = atof(value);
    else if ( !strcmp(item,"lat2"))      var.lat2 = atof(value);
    else if ( !strcmp(item,"lon2"))      var.lon2 = atof(value);
    else if ( !strcmp(item,"flag"))      var.flag = atoi(value);
    else if ( !strcmp(item,"pa"))        var.pa = atoi(value);
    else if ( !strcmp(item,"disp"))      var.disp = atoi(value);
    else if ( !strcmp(item,"mode"))      var.mode = atoi(value);
    else if ( !strcmp(item,"map"))       strcpy(var.map, value);
    else if ( !strcmp(item,"fname"))     strcpy(var.fname, value);
    else if ( !strcmp(item,"aircraft"))  strcpy(var.aircraft, value);
    else if ( !strcmp(item,"message"))   var.message = atoi(value);
    else if ( !strcmp(item,"itv"))       var.itv = atoi(value);
  }

  // 4. 현재시간 및 재계산 지연시간 설정
  get_time(&iYY, &iMM, &iDD, &iHH, &iMI, &iSS);

  // 5. 요청시간 설정
  if (strlen(tm) >= 10) {
    strncpy(tmp, &tm[0], 4);  tmp[4] = '\0';  iYY = atoi(tmp);
    strncpy(tmp, &tm[4], 2);  tmp[2] = '\0';  iMM = atoi(tmp);
    strncpy(tmp, &tm[6], 2);  tmp[2] = '\0';  iDD = atoi(tmp);
    strncpy(tmp, &tm[8], 2);  tmp[2] = '\0';  iHH = atoi(tmp);
    strncpy(tmp, &tm[10],2);  tmp[2] = '\0';  iMI = atoi(tmp);
  }
  var.seq = time2seq(iYY, iMM, iDD, iHH, iMI, 'm');

  return 0;
}

/*=============================================================================*
 *  amdar 자료 해독
 *=============================================================================*/
int amdar_data()
{
  struct dirent *dirp;
  DIR *dp;
  FILE* in = NULL;
  codes_handle* h=NULL;
  char   dirname[512], filetime[32], filename[512], buf[32];
  long   *year, *month, *day, *hour, *minute;
  double *latitude, *longitude, *flightLevel, *windDirection, *windSpeed, *airTemperature, *dewpointTemperature;
  char   **aircraft;
  size_t len=0, len2, len3;
  int    YY, MM, DD, HH, MI, YY2, MM2, DD2, HH2, MI2;
  int    i, err, idx, message;
  float  ht;

  switch (var.pa)
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

  printf("#START7777\n");
  printf("# YYMMDDHHMI      STN            LON            LAT       HGT       TA       TD       WD       WS    BUFR\n");
  printf("#        UTC       ID            deg            deg         m        C        C   degree      m/s    Fname\n");

  seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
  sprintf(filetime, "%04d%02d%02d%02d", YY, MM, DD, HH);

  // 국내 AMDAR
  sprintf(dirname, "%s/%04d%02d/%02d/", DATA_DIR, YY, MM, DD);

  idx = 0;
  if ((dp=opendir(dirname)) != NULL) {
    while ((dirp=readdir(dp)) != NULL) {
      if (strstr(dirp->d_name, "IUA") && strstr(dirp->d_name, filetime)) {
      //if (strstr(dirp->d_name, "_IUA") && strstr(dirp->d_name, filetime) && !strstr(dirp->d_name, "KARP")) {
        idx++;
        //printf("%d, %s\n", idx, dirp->d_name);

        sprintf(filename, "%s%s", dirname, dirp->d_name);
        in = fopen(filename, "rb");
        if (in) {
          message = 0;
          /* loop over the messages in the BUFR file */
          while ((h = codes_handle_new_from_file(NULL, in, PRODUCT_BUFR, &err)) != NULL || err != CODES_SUCCESS) {
            if (h == NULL) {
              message++;
              codes_handle_delete(h);
              continue;
            }

            /* we need to instruct ecCodes to expand the descriptors
               i.e. unpack the data values */
            codes_set_long(h, "unpack", 1);
 
            /* get the size and allocate memory*/
            len = 0;
            codes_get_size(h, "year", &len);
            if (len <= 0) {
              codes_handle_delete(h);
              continue;
            }

            year   = (long*)malloc(len*sizeof(long));
            month  = (long*)malloc(len*sizeof(long));
            day    = (long*)malloc(len*sizeof(long));
            hour   = (long*)malloc(len*sizeof(long));
            minute = (long*)malloc(len*sizeof(long));
            latitude            = (double*)malloc(len*sizeof(double));
            longitude           = (double*)malloc(len*sizeof(double));
            flightLevel         = (double*)malloc(len*sizeof(double));
            windDirection       = (double*)malloc(len*sizeof(double));
            windSpeed           = (double*)malloc(len*sizeof(double));
            airTemperature      = (double*)malloc(len*sizeof(double));
            dewpointTemperature = (double*)malloc(len*sizeof(double));
            aircraft = (char**)malloc(len*sizeof(char*));

            for (i = 0; i < len; i++) {
              latitude[i] = longitude[i] = flightLevel[i] = -999.;
              windDirection[i] = windSpeed[i] = airTemperature[i] = dewpointTemperature[i] = -999.;
            }

            /* get the values*/
            codes_get_long_array(h, "year", year, &len);
            codes_get_long_array(h, "month", month, &len);
            codes_get_long_array(h, "day", day, &len);
            codes_get_long_array(h, "hour", hour, &len);
            codes_get_long_array(h, "minute", minute, &len);

            codes_get_double_array(h, "latitude", latitude, &len);
            codes_get_double_array(h, "longitude", longitude, &len);
            codes_get_double_array(h, "flightLevel", flightLevel, &len);
            codes_get_double_array(h, "windDirection", windDirection, &len);
            codes_get_double_array(h, "windSpeed", windSpeed, &len);
            codes_get_double_array(h, "airTemperature", airTemperature, &len);
            codes_get_double_array(h, "dewpointTemperature", dewpointTemperature, &len);

            codes_get_string_array(h, "aircraftRegistrationNumberOrOtherIdentification", aircraft, &len);

            if (len <= 0) {
              codes_handle_delete(h);
              continue;
            }

            for (i = 0; i < len; i++) {
              if (var.mode == 1) {
                if (latitude[i]  < var.lat1 || latitude[i]  > var.lat2) continue;
                if (longitude[i] < var.lon1 || longitude[i] > var.lon2) continue;
              }

              if (var.itv != 60) {
                if (minute[i] < MI - var.itv || minute[i] > MI + var.itv) continue;
              }

              if (var.pa > 0 && (flightLevel[i] < ht-100 || flightLevel[i] > ht+100)) continue;

              if (windDirection[i] <= -999.) windDirection[i] = -999.;

              if (windSpeed[i] <= -999.) windSpeed[i] = -999.;

              if (airTemperature[i] <= -999.) airTemperature[i] = -999.;
              else airTemperature[i] -= CELKEL;

              if (dewpointTemperature[i] <= -999.) dewpointTemperature[i] = -999.;
              else dewpointTemperature[i] -= CELKEL;

              if (var.disp == 0) {
                printf("%04d%02d%02d%02d%02d %8s  %13.8f  %13.8f  %8.1f  %7.1f  %7.1f  %7.1f  %7.1f    %s\n", 
                  year[i], month[i], day[i], hour[i], minute[i], aircraft[i], longitude[i], latitude[i], flightLevel[i], airTemperature[i], dewpointTemperature[i], windDirection[i], windSpeed[i], dirp->d_name);
              }
              else {
                printf("%04d%02d%02d%02d%02d,%s,%.8f,%.8f,%.1f,%.1f,%.1f,%.1f,%.1f,%s\n", 
                  year[i], month[i], day[i], hour[i], minute[i], aircraft[i], longitude[i], latitude[i], flightLevel[i], airTemperature[i], dewpointTemperature[i], windDirection[i], windSpeed[i], dirp->d_name);
              }
            }

            /* free allocated arrays */
            free(year);
            free(month);
            free(day);
            free(hour);
            free(minute);

            free(latitude);
            free(longitude);
            free(flightLevel);
            free(windDirection);
            free(windSpeed);
            free(airTemperature);
            free(dewpointTemperature);

            /* delete handle */
            codes_handle_delete(h);
            message++;
          }

          fclose(in);
        }
        //printf("\n\n");
      }
    }

    closedir(dp);
  }


  // 국외 AMDAR
  sprintf(dirname, "%s/%04d%02d/%02d/", DATA_DIR2, YY, MM, DD);

  idx = 0;
  if ((dp=opendir(dirname)) != NULL) {
    while ((dirp=readdir(dp)) != NULL) {
      if (strstr(dirp->d_name, "_IUA") && strstr(dirp->d_name, filetime)) {
        if (!strcmp(var.map, "NHEM") || !strcmp(var.map, "WORLD") || strlen(var.map) == 0) {
        }
        else {
          if (strstr(dirp->d_name, "KARP")) continue;
        }
        idx++;
        //printf("%d, %s\n", idx, dirp->d_name);

        sprintf(filename, "%s%s", dirname, dirp->d_name);
        in = fopen(filename, "rb");
        if (in) {
          message = 0;
          /* loop over the messages in the BUFR file */
          while ((h = codes_handle_new_from_file(NULL, in, PRODUCT_BUFR, &err)) != NULL || err != CODES_SUCCESS) {
            if (h == NULL) {
              codes_handle_delete(h);
              message++;
              continue;
            }

            /* we need to instruct ecCodes to expand the descriptors
               i.e. unpack the data values */
            codes_set_long(h, "unpack", 1);
 
            /* get the size and allocate memory*/
            len = 0;
            codes_get_size(h, "year", &len);
            if (len <= 0) {
              codes_handle_delete(h);
              continue;
            }

            year   = (long*)malloc(len*sizeof(long));
            month  = (long*)malloc(len*sizeof(long));
            day    = (long*)malloc(len*sizeof(long));
            hour   = (long*)malloc(len*sizeof(long));
            minute = (long*)malloc(len*sizeof(long));
            latitude            = (double*)malloc(len*sizeof(double));
            longitude           = (double*)malloc(len*sizeof(double));
            flightLevel         = (double*)malloc(len*sizeof(double));
            windDirection       = (double*)malloc(len*sizeof(double));
            windSpeed           = (double*)malloc(len*sizeof(double));
            airTemperature      = (double*)malloc(len*sizeof(double));
            dewpointTemperature = (double*)malloc(len*sizeof(double));
            aircraft  = (char**)malloc(len*sizeof(char*));

            for (i = 0; i < len; i++) {
              latitude[i] = longitude[i] = flightLevel[i] = -999.;
              windDirection[i] = windSpeed[i] = airTemperature[i] = dewpointTemperature[i] = -999.;
            }

            /* get the values*/
            codes_get_long_array(h, "year", year, &len);
            codes_get_long_array(h, "month", month, &len);
            codes_get_long_array(h, "day", day, &len);
            codes_get_long_array(h, "hour", hour, &len);
            codes_get_long_array(h, "minute", minute, &len);

            codes_get_double_array(h, "latitude", latitude, &len);
            codes_get_double_array(h, "longitude", longitude, &len);
            codes_get_double_array(h, "flightLevel", flightLevel, &len);
            codes_get_double_array(h, "windDirection", windDirection, &len);
            codes_get_double_array(h, "windSpeed", windSpeed, &len);
            codes_get_double_array(h, "airTemperature", airTemperature, &len);
            codes_get_double_array(h, "dewpointTemperature", dewpointTemperature, &len);

            codes_get_size(h, "aircraftRegistrationNumberOrOtherIdentification", &len2);
            if (len2 <= 0) {
              codes_get_size(h, "aircraftFlightNumber", &len3);
              if (len3 <= 0) {
                codes_get_string_array(h, "aircraftTailNumber", aircraft, &len);
              }
              else codes_get_string_array(h, "aircraftFlightNumber", aircraft, &len);
            }
            else {
              codes_get_string_array(h, "aircraftRegistrationNumberOrOtherIdentification", aircraft, &len);
            }

            if (len <= 0) {
              codes_handle_delete(h);
              continue;
            }

            for (i = 0; i < len; i++) {
              if (var.mode == 1) {
                if (latitude[i]  < var.lat1 || latitude[i]  > var.lat2) continue;
                if (longitude[i] < var.lon1 || longitude[i] > var.lon2) continue;
              }

              if (var.itv != 60) {
                if (minute[i] < MI - var.itv || minute[i] > MI + var.itv) continue;
              }

              if (var.pa > 0 && (flightLevel[i] < ht-100 || flightLevel[i] > ht+100)) continue;
              
              if (latitude[i] < -999 || longitude[i] < -999) continue;

              if (windDirection[i] <= -999.) windDirection[i] = -999.;

              if (windSpeed[i] <= -999.) windSpeed[i] = -999.;

              if (airTemperature[i] <= -999.) airTemperature[i] = -999.;
              else airTemperature[i] -= CELKEL;

              if (dewpointTemperature[i] <= -999.) dewpointTemperature[i] = -999.;
              else dewpointTemperature[i] -= CELKEL;

              getword(buf, aircraft[i], ' ');

              if (var.disp == 0) {
                printf("%04d%02d%02d%02d%02d %8s  %13.8f  %13.8f  %8.1f  %7.1f  %7.1f  %7.1f  %7.1f    %s\n", 
                  year[i], month[i], day[i], hour[i], minute[i], buf, longitude[i], latitude[i], flightLevel[i], airTemperature[i], dewpointTemperature[i], windDirection[i], windSpeed[i], dirp->d_name);
              }
              else {
                printf("%04d%02d%02d%02d%02d,%s,%.8f,%.8f,%.1f,%.1f,%.1f,%.1f,%.1f,%s\n",  
                  year[i], month[i], day[i], hour[i], minute[i], buf, longitude[i], latitude[i], flightLevel[i], airTemperature[i], dewpointTemperature[i], windDirection[i], windSpeed[i], dirp->d_name);
              }
            }

            /* free allocated arrays */
            free(year);
            free(month);
            free(day);
            free(hour);
            free(minute);

            free(latitude);
            free(longitude);
            free(flightLevel);
            free(windDirection);
            free(windSpeed);
            free(airTemperature);
            free(dewpointTemperature);

            /* delete handle */
            codes_handle_delete(h);
            message++;
          }

          fclose(in);
        }
        //printf("\n\n");
      }
    }

    closedir(dp);
  }

  printf("#7777END\n");

  return 0;
}

/*=============================================================================*
 *  amdar 자료 해독(특정 항공기)
 *=============================================================================*/
int amdar_aircraft()
{
  FILE* in = NULL;
  codes_handle* h=NULL;
  char   dirname[512], filetime[32], filename[512], buf[32];
  long   *year, *month, *day, *hour, *minute;
  double *latitude, *longitude, *flightLevel, *windDirection, *windSpeed, *airTemperature, *dewpointTemperature;
  char   **aircraft;
  size_t len=0, len2, len3;
  int    YY, MM, DD, HH, MI, YY2, MM2, DD2, HH2, MI2;
  int    i, err, idx, message, ok;
  float  ht;

  ok = 0;

  printf("#START7777\n");
  printf("# YYMMDDHHMI      STN            LON            LAT       HGT       TA       TD       WD       WS\n");
  printf("#        UTC       ID            deg            deg         m        C        C   degree      m/s\n");

  seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
  sprintf(filetime, "%04d%02d%02d%02d", YY, MM, DD, HH);

  if (!strncmp(var.fname, "IUA", 3)) {
    // 국내 AMDAR
    sprintf(dirname, "%s/%04d%02d/%02d/", DATA_DIR, YY, MM, DD);
  }
  else {
    // 국외 AMDAR
    sprintf(dirname, "%s/%04d%02d/%02d/", DATA_DIR2, YY, MM, DD);
  }

  sprintf(filename, "%s%s", dirname, var.fname);

  in = fopen(filename, "rb");
  if (in) {
    message = 0;
    /* loop over the messages in the BUFR file */
    while ((h = codes_handle_new_from_file(NULL, in, PRODUCT_BUFR, &err)) != NULL || err != CODES_SUCCESS) {
      if (h == NULL) {
        codes_handle_delete(h);
        message++;
        continue;
      }

      /* we need to instruct ecCodes to expand the descriptors
         i.e. unpack the data values */
      codes_set_long(h, "unpack", 1);
 
      /* get the size and allocate memory*/
      len = 0;
      codes_get_size(h, "year", &len);
      if (len <= 0) {
        codes_handle_delete(h);
        continue;
      }

      year   = (long*)malloc(len*sizeof(long));
      month  = (long*)malloc(len*sizeof(long));
      day    = (long*)malloc(len*sizeof(long));
      hour   = (long*)malloc(len*sizeof(long));
      minute = (long*)malloc(len*sizeof(long));
      latitude            = (double*)malloc(len*sizeof(double));
      longitude           = (double*)malloc(len*sizeof(double));
      flightLevel         = (double*)malloc(len*sizeof(double));
      windDirection       = (double*)malloc(len*sizeof(double));
      windSpeed           = (double*)malloc(len*sizeof(double));
      airTemperature      = (double*)malloc(len*sizeof(double));
      dewpointTemperature = (double*)malloc(len*sizeof(double));
      aircraft  = (char**)malloc(len*sizeof(char*));

      for (i = 0; i < len; i++) {
        latitude[i] = longitude[i] = flightLevel[i] = -999.;
        windDirection[i] = windSpeed[i] = airTemperature[i] = dewpointTemperature[i] = -999.;
      }

      /* get the values*/
      codes_get_long_array(h, "year", year, &len);
      codes_get_long_array(h, "month", month, &len);
      codes_get_long_array(h, "day", day, &len);
      codes_get_long_array(h, "hour", hour, &len);
      codes_get_long_array(h, "minute", minute, &len);

      codes_get_double_array(h, "latitude", latitude, &len);
      codes_get_double_array(h, "longitude", longitude, &len);
      codes_get_double_array(h, "flightLevel", flightLevel, &len);
      codes_get_double_array(h, "windDirection", windDirection, &len);
      codes_get_double_array(h, "windSpeed", windSpeed, &len);
      codes_get_double_array(h, "airTemperature", airTemperature, &len);
      codes_get_double_array(h, "dewpointTemperature", dewpointTemperature, &len);

      codes_get_size(h, "aircraftRegistrationNumberOrOtherIdentification", &len2);
      if (len2 <= 0) {
        codes_get_size(h, "aircraftFlightNumber", &len3);
        if (len3 <= 0) {
          codes_get_string_array(h, "aircraftTailNumber", aircraft, &len);
        }
        else codes_get_string_array(h, "aircraftFlightNumber", aircraft, &len);
      }
      else {
        codes_get_string_array(h, "aircraftRegistrationNumberOrOtherIdentification", aircraft, &len);
      }

      if (len <= 0) {
        codes_handle_delete(h);
        continue;
      }

      for (i = 0; i < len; i++) {
        if (latitude[i] < -999 || longitude[i] < -999) continue;
        getword(buf, aircraft[i], ' ');

        if (!strcmp(buf, var.aircraft)) {
          if (strstr(filename, "KARP") || strstr(filename, "BABJ")|| strstr(filename, "VHHH")) {
          }
          else if (message != var.message) {
          //  continue;
          }

          if (windDirection[i] <= -999.) windDirection[i] = -999.;

          if (windSpeed[i] <= -999.) windSpeed[i] = -999.;

          if (airTemperature[i] <= -999.) airTemperature[i] = -999.;
          else airTemperature[i] -= CELKEL;

          if (dewpointTemperature[i] <= -999.) dewpointTemperature[i] = -999.;
          else dewpointTemperature[i] -= CELKEL;

          if (var.disp == 0) {
            printf("%04d%02d%02d%02d%02d %8s  %13.8f  %13.8f  %8.1f  %7.1f  %7.1f  %7.1f  %7.1f\n", 
              year[i], month[i], day[i], hour[i], minute[i], buf, longitude[i], latitude[i], flightLevel[i], airTemperature[i], dewpointTemperature[i], windDirection[i], windSpeed[i]);
          }
          else {
            printf("%04d%02d%02d%02d%02d,%s,%.8f,%.8f,%.1f,%.1f,%.1f,%.1f,%.1f\n",  
              year[i], month[i], day[i], hour[i], minute[i], buf, longitude[i], latitude[i], flightLevel[i], airTemperature[i], dewpointTemperature[i], windDirection[i], windSpeed[i]);
          }

          //ok = 1;
          //break;
        }
      }

      /* free allocated arrays */
      free(year);
      free(month);
      free(day);
      free(hour);
      free(minute);

      free(latitude);
      free(longitude);
      free(flightLevel);
      free(windDirection);
      free(windSpeed);
      free(airTemperature);
      free(dewpointTemperature);

      /* delete handle */
      codes_handle_delete(h);
      message++;

      //if (message > var.message || ok == 1) break;
    }

    fclose(in);
  }

  printf("#7777END\n");

  return 0;
}

/*=============================================================================*
 *  amdar 자료 해독
 *=============================================================================*/
int amdar_keys()
{
    //const char* filename = "/C4N2_DATA/GTS/BULL/BUFR/202201/28/IUAC01_RKSL_202201280331]HL7717.bfr";
    const char* filename = "/C4N2_DATA/GTS/BUFR/202201/20/AMMC_IUAX_2022012000.bfr";
    FILE* in       = NULL;

    /* message handle. Required in all the eccodes calls acting on a message.*/
    codes_handle* h = NULL;
    int err = 0, cnt = 0;
    int keyType = 0;
    int subsetNumber = 0;

    char value[MAX_VAL_LEN];
    size_t vlen = MAX_VAL_LEN;
    size_t klen = 0;


    in = fopen(filename, "rb");
    if (!in) {
        fprintf(stderr, "Error: unable to open file %s\n", filename);
        return 1;
    }

    /* loop over the messages in the BUFR file */
    while ((h = codes_handle_new_from_file(NULL, in, PRODUCT_BUFR, &err)) != NULL || err != CODES_SUCCESS) {
        codes_bufr_keys_iterator* kiter = NULL;
        if (h == NULL) {
            fprintf(stderr, "Error: unable to create handle for message %d\n", cnt);
            cnt++;
            continue;
        }

        printf("message: %d\n", cnt);

        /* we need to instruct ecCodes to unpack the data values */
        CODES_CHECK(codes_set_long(h, "unpack", 1), 0);

        /* get BUFR key iterator */
        kiter = codes_bufr_keys_iterator_new(h, 0);
        if (!kiter) {
            fprintf(stderr, "Error: Unable to create BUFR keys iterator\n");
            exit(1);
        }

        /* loop over the keys */
        while (codes_bufr_keys_iterator_next(kiter)) {
            /* get key name */
            char* name = codes_bufr_keys_iterator_get_name(kiter);
            if (strcmp(name, "subsetNumber") == 0) {
                subsetNumber++;
                printf("\n  Subset %d:\n", subsetNumber);
                continue;
            }
            printf("  %s=", name);

            CODES_CHECK(codes_get_native_type(h, name, &keyType), 0);

            /* get key size to see if it is an array */
            CODES_CHECK(codes_get_size(h, name, &klen), 0);

            if (klen == 1) {
                /* not array */
                vlen = MAX_VAL_LEN;
                memset(value, 0, vlen);
                codes_get_string(h, name, value, &vlen);
                printf("%s\n", value);
            }
            else {
                /* for arrays */
                printf("(array of %lu)\n", (unsigned long)klen);
            }
        }

        /* delete key iterator */
        codes_bufr_keys_iterator_delete(kiter);

        /* delete handle */
        codes_handle_delete(h);

        cnt++;
    }

    fclose(in);
    return 0;
}

/*=============================================================================*
 *  amdar 파일 목록
 *=============================================================================*/
int amdar_files()
{
  DIR *dp;
  struct dirent *dirp;
  int  YY, MM, DD, HH, MI, YY2, MM2, DD2, HH2, MI2;
  int  i;
  char dirname[512], filetime[32];
  
  seq2time(var.seq, &YY, &MM, &DD, &HH, &MI, 'm', 'n');
  sprintf(dirname, "%s/%04d%02d/%02d/", DATA_DIR2, YY, MM, DD);
  sprintf(filetime, "%04d%02d%02d%02d", YY, MM, DD, HH);

  i = 0;
  if ((dp=opendir(dirname)) != NULL) {
    while ((dirp=readdir(dp)) != NULL) {
      if (strstr(dirp->d_name, "_IUA") && strstr(dirp->d_name, filetime)) {
        i++;
        printf("%d, %s\n", i, dirp->d_name);
      }
    }

    closedir(dp);
  }
}