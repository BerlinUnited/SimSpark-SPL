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

#ifndef _LINESEGMENT2_H
#define	_LINESEGMENT2_H

#include "line2.h"

namespace salt {

template <typename DATATYPE, typename P_T>
class TLineSegment2 : public TLine2<DATATYPE>
{
  /// endpoints
  P_T mP[2];
public:

  TLineSegment2()
  {
  }

  TLineSegment2(const P_T & p0, const P_T & p1)
  : TLine2<DATATYPE>(p0, p1)
  {
    mP[0] = p0;
    mP[1] = p1;
  }

  const P_T & p0() const
  {
    return mP[0];
  }

  const P_T & p1() const
  {
    return mP[1];
  }

  bool IsBetween(const P_T & p) const
  {
    return p[0] <= std::max(mP[0][0], mP[1][0])
      && p[0] >= std::min(mP[0][0], mP[1][0])
      && p[1] <= std::max(mP[0][1], mP[1][1])
      && p[1] >= std::min(mP[0][1], mP[1][1]);
  }

  DATATYPE Length() const
  {
    return ( mP[0] - mP[1]).Length();
  }

  bool IsContain(const P_T & p) const
  {
    return (p - mP[0]).Length() + (p - mP[1]).Length()
      < Length() + std::numeric_limits<DATATYPE>::epsilon();
  }

  /**
   * calculate the intersection of the segment and a given line
   *
   * @param line the given line
   * @param intersection retrun the intersection
   *
   * @return if the segment and line have intersection
   */
  bool Intersection(const TLine2<DATATYPE>& line,
    P_T & intersection)const
  {
    if ( TLine2<DATATYPE>::Intersection(line, intersection) )
    {
      return IsBetween(intersection);
    }
    return false;
  }

  /**
   * calculate the interseciton of two segment
   *
   * @param seg2 the other segment
   * @param intersection return the intersection
   *
   * @return if the tow segments have intersection
   */
  bool Intersection(const TLineSegment2& seg2,
    P_T & intersection)const
  {
    if (TLine2<DATATYPE>::Intersection(seg2, intersection)) {
      return IsBetween(intersection) && seg2.IsBetween(intersection);
    }
    return false;
  }

  /**
   * if there is an intersection of two segment
   *
   * @param seg2 the other segment
   *
   * @return if the tow segments have intersection
   */
  bool IsIntersect(const TLineSegment2& seg2) const
  {
    P_T intersection;
    return Intersection(seg2, intersection);
  }
};

typedef TLineSegment2<float, Vector2f> LineSegment2f;

} // namespace salt
#endif	/* _LINESEGMENT2_H */

