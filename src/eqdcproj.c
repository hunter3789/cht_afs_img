#include <stdio.h>
#include <math.h>
#include "map_ini.h"

/***************************************************************************
*
*  [ Equidistant Cylindrical Projection ]
*
*      o lon, lat : (longitude,latitude) at earth  [degree]
*      o x, y     : (x,y) cordinate in map  [grid]
*      o code = 0 : (lon,lat) --> (x,y)
*               1 : (x,y) --> (lon,lat)
*
***************************************************************************/
int  eqdcproj( lon, lat, x, y, code, map )
  float  *lon, *lat;         /* Longitude, Latitude [degree]  */
  float  *x, *y;             /* Coordinate in Map   [grid]    */
  int    code;               /* (0) lon,lat -> x,y  (1) x,y -> lon,lat */
  struct eqdc_parameter *map;
{
  static double  PI, DEGRAD, RADDEG;
  static double  re, slat, slon, olon, olat, xo, yo;
  double         xn;

  if ((*map).first == 0) {
    PI = asin(1.0)*2.0;
    DEGRAD = PI/180.0;
    RADDEG = 180.0/PI;

    re = (*map).Re/(*map).grid;
    slon = (*map).slon * DEGRAD;
    slat = (*map).slat * DEGRAD;
    olon = (*map).olon * DEGRAD;
    olat = (*map).olat * DEGRAD;

    xn = olon - slon;
    if (xn > PI)  xn -= 2.0*PI;
    if (xn < -PI) xn += 2.0*PI;
    xo = re*xn*cos(slat) - (*map).xo;
    yo = (*map).yo;
    (*map).first = 1;
  }

  if (code == 0) {
    xn = (*lon)*DEGRAD - slon;
    if (xn >  PI) xn -= 2.0*PI;
    if (xn < -PI) xn += 2.0*PI;
    *x = re*xn*cos(slat) - xo;
    *y = re*((*lat)*DEGRAD - olat) + yo;
  }
  else {
    *lat = ((*y - yo)/re + olat) * RADDEG;
    *lon = ((*x + xo)/(re*cos(slat)) + slon) * RADDEG;
  }
  return 0;
}
