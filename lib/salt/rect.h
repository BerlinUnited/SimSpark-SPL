/* -*- mode: c++ -*-
   
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: rect.h,v 1.2 2003/05/19 21:32:39 fruit Exp $

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
#ifndef RECT_H__
#define RECT_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "defines.h"

namespace salt
{

//
// 2D rectangle
//
class Rect
{
public:
	/// con/des
	f_inline 		Rect()													{ }
	f_inline 		Rect(const Rect &inOther)								: mLeft(inOther.Left()), mTop(inOther.Top()), mRight(inOther.Right()), mBottom(inOther.Bottom()) { }
	f_inline 		Rect(int inLeft, int inTop, int inRight, int inBottom)	: mLeft(inLeft), mTop(inTop), mRight(inRight), mBottom(inBottom) { }

	// set
	f_inline void	Set(int inLeft, int inTop, int inRight, int inBottom)	{ mLeft=inLeft;	mTop=inTop;			mRight=inRight;							mBottom=inBottom; }

	// member access
	f_inline int	Left() const			{ return mLeft; }
	f_inline int	Right() const			{ return mRight; }
	f_inline int	Top() const				{ return mTop; }
	f_inline int	Bottom() const			{ return mBottom; }
	f_inline int	Width() const			{ return mRight-mLeft; }
	f_inline int	Height() const			{ return mBottom-mTop; }

	/// Actions
	f_inline void	Normalize()										{ if (mRight < mLeft) gSwap(mLeft, mRight); if (mBottom < mTop) gSwap(mTop, mBottom); }
	f_inline void	Widen(int inDelta)								{ mLeft-=inDelta; mTop-=inDelta; mRight+=inDelta; mBottom+=inDelta;	}
	f_inline void	Widen(int inDeltaWidth, int inDeltaHeight)		{ mRight+=inDeltaWidth;	mBottom+=inDeltaHeight;	}
	f_inline void	Widen(int inDeltaLeft, int inDeltaTop, int inDeltaRight, int inDeltaBottom)	{ mLeft-=inDeltaLeft; mTop-=inDeltaTop;	mRight+=inDeltaRight; mBottom+=inDeltaBottom;		}
	f_inline void	Shrink(int inDelta)								{ mLeft+=inDelta;	mTop+=inDelta;	mRight-=inDelta; mBottom-=inDelta;	}
	f_inline void	Shrink(int inDeltaWidth, int inDeltaHeight)		{ mRight-=inDeltaWidth;	mBottom-=inDeltaHeight;	}
	f_inline void	Shrink(int inDeltaLeft, int inDeltaTop, int inDeltaRight, int inDeltaBottom) { mLeft+=inDeltaLeft; mTop+=inDeltaTop; mRight-=inDeltaRight; mBottom-=inDeltaBottom;		}
	f_inline void	Offset(int inDeltaX, int inDeltaY)				{ mLeft+=inDeltaX;	mTop+=inDeltaY;	mRight+=inDeltaX;	mBottom+=inDeltaY; }
	f_inline bool	Intersects(const Rect &b) const					{ return !(mLeft > b.mRight || mRight < b.mLeft || mTop > b.mBottom || mBottom < b.mTop); }

	// assignment
	f_inline Rect&		operator=(const Rect &inOther)								{ mLeft=inOther.Left();	mTop=inOther.Top();	mRight=inOther.Right();	mBottom=inOther.Bottom(); return *this; }

	// comparison
	f_inline bool		operator==(const Rect &inRHS) const							{ return (mLeft==inRHS.Left()) && (mTop==inRHS.Top()) && (mRight==inRHS.Right()) && (mBottom==inRHS.Bottom()); }
	f_inline bool		operator!=(const Rect &inRHS) const							{ return (mLeft!=inRHS.Left()) || (mTop!=inRHS.Top()) || (mRight!=inRHS.Right()) || (mBottom!=inRHS.Bottom()); }

private:
	int		mLeft, mTop, mRight, mBottom;
};

}//namespace salt

#endif //RECT_H__
