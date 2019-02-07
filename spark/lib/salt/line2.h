/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Copyright (C) 2008,2009 SEU RoboCup Simulation Team, Southeast University , Nanjing, China.
   Copyright (C) 2010 RoboCup Soccer Server 3D Maintenance Group
   $Id$

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _LINE2_H
#define	_LINE2_H

#include <limits>
#include "tvector.h"

namespace salt {

template <typename DATATYPE>
class TLine2
{
  // a line is defined by the formula: ay + bx + c = 0
  DATATYPE mA;
  DATATYPE mB;
  DATATYPE mC;
public:

  /**
   * create line by given coefficients
   *
   * @param a the a coefficients
   * @param b the b coefficients
   * @param c the c coefficients
   */
  TLine2(DATATYPE a = 1, DATATYPE b = 0, DATATYPE c = -1)
  : mA(a), mB(b), mC(c)
  {
  }

  /**
   * create line from tow points
   *
   * @param pos1 the one point
   * @param pos2 the other point
   */
  template <typename P_T>
  TLine2(const P_T &pos1,
    const P_T &pos2)
  {
    // 1*y + bx + c = 0 => y = -bx - c
    // with -b the direction coefficient (or slope)
    // and c = - y - bx
    DATATYPE dTemp = pos2[0] - pos1[0]; // determine the slope
    if (fabs(dTemp) < std::numeric_limits<DATATYPE>::epsilon() ) {
      // ay + bx + c = 0 with vertical slope=> a = 0, b = 1
      mA = 0.0;
      mB = 1.0;
    } else {
      // y = (-b)x -c with -b the slope of the line
      mA = 1.0;
      mB = -(pos2[1] - pos1[1]) / dTemp;
    }
    // ay + bx + c = 0 ==> c = -a*y - b*x
    mC = -mA * pos2.y() - mB * pos2.x();
  }

  /**
   * create line from one point and an angle
   *
   * @param pos the point in line
   * @param angle the sclope angle
   */
  template <typename P_T>
  TLine2(const P_T &pos, DATATYPE angle)
  {
    // calculate point somewhat further in direction 'angle'
    // and make line from these two points.
    P_T pos2 = pos;
    pos2.x() += cosDeg(angle);
    pos2.y() += sinDeg(angle);
    *this = TLine2(pos, pos2);
  }

  DATATYPE a() const
  {
    return mA;
  }

  DATATYPE b() const
  {
    return mB;
  }

  DATATYPE c() const
  {
    return mC;
  }

  DATATYPE& a()
  {
    return mA;
  }

  DATATYPE& b()
  {
    return mB;
  }

  DATATYPE& c()
  {
    return mC;
  }

  /**
   * calculate the intersection of two lines
   *
   * @param line the other line
   *
   * @return the intersection
   */
  template <typename P_T>
  bool Intersection(const TLine2 &line, P_T &p) const
  {
    DATATYPE L = line.a() * mB - mA * line.b();
    if (abs(L) < std::numeric_limits<DATATYPE>::epsilon()) {
      // lines are parallel, no intersection
      return false;
    }
    if (mA == 0) {
      // bx + c = 0 and a2*y + b2*x + c2 = 0 ==> x = -c/b
      // calculate x using the current line
      p.x() = -mC / mB; // and calculate the y using the second line
      p.y() = line.YGivenX(p.x());
    } else if (line.a() == 0) {
      // ay + bx + c = 0 and b2*x + c2 = 0 ==> x = -c2/b2
      p.x() = -line.c() / line.b();
      // calculate x using
      p.y() = YGivenX(p.x());
      // 2nd line and calculate y using current line
    } else {
      // ay + bx + c = 0 and a2y + b2*x + c2 = 0
      // y = (-b2/a2)x - c2/a2
      // bx = -a*y - c =>  bx = -a*(-b2/a2)x -a*(-c2/a2) - c
      // ==> a2*bx = a*b2*x + a*c2 - a2*c
      // ==> x = (a*c2 - a2*c)/(a2*b - a*b2)
      p.x() = (mA * line.c() - line.a() * mC) / L;
      p.y() = YGivenX(p.x());
    }
    return true;
  }

  /**
   * ay + bx + c = 0 -> y = (-b/a)x + (-c/a)
   * tangent: y = (a/b)*x + C1 -> by - ax + C2 = 0 => C2 = ax - by
   * with pos.y = y, pos.x = x
   *
   * @param pos the tangent intersection
   *
   * @return the tangent line of this line
   */
  template<typename P_T>
  TLine2 TangentLine(const P_T &pos) const
  {
    return TLine2(mB, -mA, mA * pos.x() - mB * pos.y());
  }

  /**
   * calculate the closest point of a given point to this line
   *
   * @param pos the given point
   *
   * @return the closest point
   */
  template<typename P_T>
  P_T  PointOnLineClosestTo(const P_T &pos) const
  {
    TLine2 l2 = TangentLine(pos); // get tangent line
    // and intersection between the two lines
    return Intersection(l2);
  }

  /**
   * calculate the distance from a given point to this line
   *
   * @param pos the given point
   *
   * @return the distance from the pos to this line
   */
  template<typename P_T>
  DATATYPE DistanceToPoint(const P_T &pos) const
  {
    return (PointOnLineClosestTo(pos) - pos).length();
  }

  /**
   * calculate the Y value by given X value
   *
   * @param x the given x coordinate value
   *
   * @return the Y coordinate value
   */
  DATATYPE YGivenX(DATATYPE x) const
  {
    if (mA == 0) {
      std::cerr << __FILE__ << __LINE__ << __FUNCTION__
        << " Cannot calculate Y coordinate: " << std::endl;
      return 0;
    }
    // ay + bx + c = 0 ==> y = -(b*x + c)/a
    return -(mB * x + mC) / mA;
  }

  /**
   * calculate the X value by given Y value
   *
   * @param y the given Y coordinate value
   *
   * @return the X coordinate value
   */
  DATATYPE XGivenY(DATATYPE y) const
  {
    if (mB == 0) {
      std::cerr << __FILE__ << __LINE__ << __FUNCTION__
        << "Cannot calculate X coordinate\n" << std::endl;
      return 0;
    }
    // ay + bx + c = 0 ==> x = -(a*y + c)/b
    return -(mA * y + mC) / mB;
  }

  /**
   * calculate the location of point to the line
   *
   * @param p the given point
   *
   * @return +1 means upper the line
   *          0 means on the line
   *         -1 means lower the line
   */
  template <typename P_T>
  int Location(const P_T & p)const
  {
    return sign(mA * p.y() + mB * p.x() + mC);
  }

  /**
   * calculate the ang for line
   */
  DATATYPE GetAngle()
  {
    return atan2(-mB, mA);
  }

  /**
   * @param[in] pos1 first point
   * @param pos2 second point
   * @return
   */
  template<typename P_T>
  static TLine2 MidperpendicularFromTwoPoints(const P_T &pos1, const P_T &pos2)
  {
    TLine2 l1(pos1, pos2);
    P_T posMid = (pos1 + pos2)*0.5;
    return l1.TangentLine(posMid);
  }

};

/// dump

template<typename DATATYPE>
std::ostream & operator <<(std::ostream & os, TLine2<DATATYPE> l)
{
  DATATYPE a = l.a();
  DATATYPE b = l.b();
  DATATYPE c = l.c();
  // ay + bx + c = 0 -> y = -b/a x - c/a
  if (a == 0) {
    os << "x = " << -c / b;
  } else {
    os << "y = ";
    if (b != 0)
      os << -b / a << "x ";
    if (c > 0)
      os << "- " << fabs(c / a);
    else if (c < 0)
      os << "+ " << fabs(c / a);
  }
  return os;
}

} // namespace salt
#endif	/* _LINE2_H */

