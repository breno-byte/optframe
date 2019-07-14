// OptFrame - Optimization Framework

// Copyright (C) 2009-2015
// http://optframe.sourceforge.net/
//
// This file is part of the OptFrame optimization framework. This framework
// is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License v3 as published by the
// Free Software Foundation.

// This framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License v3 for more details.

// You should have received a copy of the GNU Lesser General Public License v3
// along with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

#ifndef EtII_NSSEQRotate_HPP_
#define EtII_NSSEQRotate_HPP_

// Framework includes
#include "../../OptFrame/NSSeq.hpp"
#include "../../OptFrame/RandGen.hpp"

// Own includes
#include "ProblemInstance.h"
#include "Solution.h"

using namespace std;
using namespace optframe;

namespace EtII {

class MoveRotate : public Move<RepEtII>
{
protected:
   int nRot, x, y;

public:
   using Move<RepEtII>::apply;        // prevents name hiding
   using Move<RepEtII>::canBeApplied; // prevents name hiding

   MoveRotate(int _nRot, int _x, int _y)
     : nRot(_nRot)
     , x(_x)
     , y(_y)
   {
   }

   virtual ~MoveRotate()
   {
   }

   bool canBeApplied(const RepEtII& rep, const OPTFRAME_DEFAULT_ADS*) override
   {
      return true;
   }

   Move<RepEtII>* apply(RepEtII& rep, OPTFRAME_DEFAULT_ADS*) override
   {
      for (int i = 0; i < nRot; i++)
         rep(x, y).rotate();

      return new MoveRotate(4 - nRot, x, y);
   }

   Move<RepEtII>* applyUpdate(Evaluation& e, RepEtII& rep, OPTFRAME_DEFAULT_ADS* ads) override
   {
      int f = 0;
      if (rep(x, y).left == rep(x, y - 1).right)
         f++;
      if (rep(x, y).up == rep(x - 1, y).down)
         f++;
      if (rep(x, y).right == rep(x, y + 1).left)
         f++;
      if (rep(x, y).down == rep(x + 1, y).up)
         f++;

      Move<RepEtII>& rev = *apply(rep, ads);

      int f2 = 0;
      if (rep(x, y).left == rep(x, y - 1).right)
         f2++;
      if (rep(x, y).up == rep(x - 1, y).down)
         f2++;
      if (rep(x, y).right == rep(x, y + 1).left)
         f2++;
      if (rep(x, y).down == rep(x + 1, y).up)
         f2++;

      e.setObjFunction(e.getObjFunction() + (f2 - f));

      return &rev;
   }

   virtual bool operator==(const Move<RepEtII>& _m) const
   {
      const MoveRotate& m = (const MoveRotate&)_m;
      return (m.nRot == nRot) && (m.x == x) && (m.y == y);
   }

   void print() const
   {
      cout << "MoveRotate: " << nRot << " rotations on (" << x << "," << y << ")" << endl;
   }

   string id() const
   {
      return "OptFrame:Move:MoveRotate";
   }
};

class NSIteratorRotate : public NSIterator<RepEtII>
{
private:
   int nIntraRows, nIntraCols;
   int nRot, x, y;

public:
   NSIteratorRotate(int _nIntraRows, int _nIntraCols)
     : nIntraRows(_nIntraRows)
     , nIntraCols(_nIntraCols)
   {
      x = 0;
      y = 0;
      nRot = 1;
   }

   virtual ~NSIteratorRotate()
   {
   }

   virtual void first() override
   {
      x = 0;
      y = 0;
      nRot = 1;
   }

   virtual void next() override
   {
      nRot++;
      if (nRot > 3) {
         nRot = 1;
         y++;
         if (y >= nIntraCols) {
            y = 0;
            x++;
         }
      }
   }

   virtual bool isDone() override
   {
      return x >= nIntraRows;
   }

   virtual Move<RepEtII>* current() override
   {
      return new MoveRotate(nRot, x + 1, y + 1);
   }
};

template<class MOVE = MoveRotate>
class NSSeqRotate : public NSSeq<RepEtII>
{
private:
   RandGen& rg;

public:
   NSSeqRotate(RandGen& _rg)
     : rg(_rg)
   {
   }

   virtual ~NSSeqRotate()
   {
   }

   virtual Move<RepEtII>* randomMove(const RepEtII& rep, const OPTFRAME_DEFAULT_ADS*) override
   {
      // line 'x' and col 'y'
      int x = rg.rand((rep.getNumRows() - 2)) + 1;
      int y = rg.rand((rep.getNumCols() - 2)) + 1;
      int nRot = rg.rand(3) + 1;

      return new MOVE(nRot, x, y); // return a random move
   }

   virtual NSIterator<RepEtII>* getIterator(const RepEtII& rep, const OPTFRAME_DEFAULT_ADS*) override
   {
      // return an iterator to the neighbors of 'rep'
      return new NSIteratorRotate(rep.getNumRows() - 2, rep.getNumCols() - 2);
   }

   virtual void print() const
   {
      cout << "NSSeqRotate" << endl;
   }

   string id() const
   {
      return "OptFrame:NS:NSSeqRotate";
   }
};
}

#endif /*EtII_NSSEQRotate_HPP_*/
