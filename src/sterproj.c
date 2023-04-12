#include <stdio.h>
#include <math.h>
#include "map_ini.h"

/***************************************************************************
*
*  [ Stereographic Projection ]
*
*      o lon, lat : (longitude,latitude) at earth  [degree]
*      o x, y     : (x,y) cordinate in map  [grid]
*      o code = 0 : (lon,lat) --> (x,y)
*               1 : (x,y) --> (lon,lat)
*      o map      : map parameter
*
***************************************************************************/
int  sterproj( lon, lat, x, y, code, map )
  float  *lon, *lat;         /* Longitude, Latitude [degree]  */
  float  *x, *y;             /* Coordinate in Map   [grid]    */
  int    code;               /* (0) lon,lat -> x,y  (1) x,y -> lon,lat */
  struct ster_parameter *map;
{
  static int     first = 0, polar = 1;
  static double  re, slat, slon, olon, olat, xo, yo;
  static double  PI, DEGRAD, RADDEG;
  double         Re, alon, alat, ak, dd, xn, yn, a1, a2, ra, ac, theta;

  if ((*map).first == 0) {
    PI = asin(1.0)*2.0;
    DEGRAD = PI/180.0;
    RADDEG = 180.0/PI;

    slon = (*map).slon * DEGRAD;
    slat = (*map).slat * DEGRAD;
    olon = (*map).olon * DEGRAD;
    olat = (*map).olat * DEGRAD;

    /* if slat = 90N and 60N 통과 */
    if( (*map).polar == 1 )
      re = (*map).Re/(*map).grid * ( 1.0 + sin( 60.0*DEGRAD ) ) * 0.5;
    else
      re = (*map).Re/(*map).grid;

    ak = 1.0 + sin(slat)*sin(olat) + cos(slat)*cos(olat)*cos(olon-slon);
    if( ak <= 0.0 ) return -1;
    ak = 2.0/ak;
    xo = re*ak*cos(olat)*sin(olon-slon) - (*map).xo;
    yo = re*ak*( cos(slat)*sin(olat) - sin(slat)*cos(olat)*cos(olon-slon) ) - (*map).yo;
    (*map).first = 1;
  }

  if (code == 0) {
    alat = (*lat)*DEGRAD;
    alon = (*lon)*DEGRAD - slon;
    ak = 1.0 + sin(slat)*sin(alat) + cos(slat)*cos(alat)*cos(alon);
    if( ak <= 0.0 )
      return -1;
    else {
      ak = 2.0/ak;
      (*x) = re*ak*cos(alat)*sin(alon) - xo;
      (*y) = re*ak*( cos(slat)*sin(alat) - sin(slat)*cos(alat)*cos(alon) ) - yo;
    }
  }
  else {
    xn = (*x) + xo;
    yn = (*y) + yo;
    ra = xn*xn + yn*yn;
    if (ra <= 0.0) {
      ac = 0.0;
      alat = slat;
    }
    else {
      ra = sqrt(ra);
      ac = 2.0*atan(0.5*ra/re);
      alat = cos(ac)*sin(slat) + yn*sin(ac)*cos(slat)/ra;
      alat = asin(alat);
    }
    a1 = xn*sin(ac);
    a2 = ra*cos(slat)*cos(ac) - yn*sin(slat)*sin(ac);
    if (fabs(a1) <= 0.0)
      theta = 0.0;
    else if( abs(a2) <= 0.0 ) {
      theta = PI*0.5;
      if( a2 < 0.0 ) theta = -theta;
    }
    else
      theta = atan2(a1,a2);
    alon = theta + slon;
    *lon = alon*RADDEG;
    *lat = alat*RADDEG;
  }
  return 0;
}

/***************************************************************************
*
*  [ Stereographic Projection ] - ellipsoid (WGS84)
*
*      o lon, lat : (longitude,latitude) at earth  [degree]
*      o x, y     : (x,y) cordinate in map  [grid]
*      o code = 0 : (lon,lat) --> (x,y)
*               1 : (x,y) --> (lon,lat)
*      o map      : map parameter
*
***************************************************************************/
int  sterproj_ellp( lon, lat, x, y, code, map )
  float  *lon, *lat;         /* Longitude, Latitude [degree]  */
  float  *x, *y;             /* Coordinate in Map   [grid]    */
  int    code;               /* (0) lon,lat -> x,y  (1) x,y -> lon,lat */
  struct ster_parameter *map;
{
  static int     first = 0, polar = 1;
  static double  re, slat, slon, olon, olat, xo, yo, ea, f, ep, cons, ms1, x0, con;
  static double  PI, DEGRAD, RADDEG, EPSLN;
  double         alon, alat, ak, x1, ts, rh, ce, chi, phi, dphi;
  double         xn, yn, ra;
  int            i;

  if ((*map).first == 0) {
    PI = asin(1.0)*2.0;
    DEGRAD = PI/180.0;
    RADDEG = 180.0/PI;
    EPSLN = 1.0e-10;

    ea = 6378.138;              // 장반경 (km)
    f  = 1.0/298.257223563;     // 편평도 : (장반경-단반경)/장반경
    ep = sqrt(2.0*f - f*f);

    slon = (*map).slon * DEGRAD;
    slat = (*map).slat * DEGRAD;
    olon = (*map).olon * DEGRAD;
    olat = (*map).olat * DEGRAD;

    re = ea/(*map).grid;

    cons = sqrt(pow(1+ep,1+ep)*pow(1-ep,1-ep));
    if (fabs(cos(slat)) > EPSLN) {
      ms1 = cos(slat) / sqrt(1-ep*sin(slat)*ep*sin(slat)); 
      x0 = 2*atan(tan(0.5*(PI/2 + slat) * pow((1-sin(slat)*ep)/(1+sin(slat)*ep), ep*0.5))) - PI/2;
    }

    if (slat > 0) con = 1;
    else con = -1;

    (*map).first = 1;
  }

  if (code == 0) {
    alat = (*lat)*DEGRAD;
    alon = (*lon)*DEGRAD - slon;

    if (fabs(cos(slat)) <= EPSLN) {

      ts = tan(0.5*(PI/2 - alat*con)) / (pow(((1-ep*con*sin(alat))/(1+ep*con*sin(alat))), 0.5*ep));
      rh = 2 * ts / cons;

      (*x) = re*rh*sin(alon) + (*map).xo;
      (*y) = -con*re*rh*cos(alon) + (*map).yo;

      return 0;
    }
    else {
      x1 = 2*atan(tan(0.5*(PI/2 + alat) * pow((1-sin(alat)*ep)/(1+sin(alat)*ep), ep*0.5))) - PI/2;

      if (fabs(sin(slat)) <= EPSLN) {
        ak = 2 / (1 + cos(x1)*cos(alon));
        (*y) = re*ak*sin(x1);
      }
      else {
        ak = 2 * ms1 / (cos(x0)*(1+sin(x0)*sin(x1) + cos(x0)*cos(x1)*cos(alon)));
        (*y) = re*ak*(cos(x0)*sin(x1) - sin(x0)*cos(x1)*cos(alon)) + (*map).yo;
      }
      (*x) = re*ak*cos(x1)*sin(alon) + (*map).xo;
    }
  }
  else {
    xn = (*x) - (*map).xo;
    yn = (*y) - (*map).yo;
    ra = sqrt(xn*xn + yn*yn);

    if (fabs(cos(slat)) <= EPSLN) {
      if (ra <= EPSLN) {
        *lon = slon*RADDEG;
        *lat = slat*RADDEG;        
        return 0;
      }
      xn *= con;
      yn *= con;
      ts = ra * cons / (2*re);
      phi = PI/2 - 2*atan(ts);
      for (i=0; i<=15; i++) {
        dphi = PI/2 - 2*atan(ts*pow((1-ep*sin(phi))/(1+ep*sin(phi)),0.5*ep)) - phi;
        phi += dphi;
        if (fabs(dphi) <= EPSLN) break;
      }

      alat = con * phi;
      alon = con * (con*slon + atan2(xn,-yn));
    }
    else {
      ce = 2*atan(rh*cos(x0)/(2*re*ms1));
      alon = slon;
      if (rh <= EPSLN) chi = x0;
      else {
        chi = asin(cos(ce)*sin(x0) + yn*sin(ce)*cos(x0)/rh);
        alon = slon + atan2(xn*sin(ce),rh*cos(x0)*cos(ce) - yn*sin(x0)*sin(ce)); 
      }

      ts = tan(0.5*(PI/2 + chi));
      phi = PI/2 - 2*atan(ts);
      for (i=0; i<=15; i++) {
        dphi = PI/2 - 2*atan(ts*pow((1-ep*sin(phi))/(1+ep*sin(phi)),0.5*ep)) - phi;
        phi += dphi;
        if (fabs(dphi) <= EPSLN) break;
      }
      alat = -1*phi;
    }

    *lon = alon*RADDEG;
    *lat = alat*RADDEG;
  }
  return 0;
}
