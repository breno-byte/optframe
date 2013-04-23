// OptFrame - Optimization Framework

// Copyright (C) 2009, 2010, 2011
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

#ifndef OPTFRAME_MOVE_HPP_
#define OPTFRAME_MOVE_HPP_

#include "Solution.hpp"
#include "Evaluation.hpp"

#include "OptFrameComponent.hpp"

using namespace std;

typedef void OPTFRAME_DEFAULT_PROBLEM;

template<class R, class ADS = OPTFRAME_DEFAULT_ADS, class DS = OPTFRAME_DEFAULT_DS>
class Move : public OptFrameComponent
{
public:

	virtual ~Move()
	{
	}

	bool canBeApplied(const Solution<R, ADS>& s)
	{
		return canBeApplied(s.getR(), s.getADS());
	}

	virtual bool canBeApplied(const R&, const ADS&) = 0;

	Move<R, ADS, DS>& apply(Solution<R, ADS>& s)
	{
		return apply(s.getR(), s.getADS());
	}

	Move<R, ADS, DS>& apply(Evaluation<DS>& e, Solution<R, ADS>& s)
	{
		return apply(e.getDS(), s.getR(), s.getADS());
	}

	virtual Move<R, ADS, DS>& apply(R& r, ADS& ads) = 0;

	virtual Move<R, ADS, DS>& apply(DS& ds, R& r, ADS& ads)
	{
		Move<R, ADS, DS>& rev = apply(r, ads);
		updateNeighStatus(ads);
		return rev;
	}

	virtual void updateNeighStatus(ADS& ads) = 0;


	virtual pair<double, double>* cost(const Evaluation<DS>& e, const R& r, const ADS& ads)
	{
		return NULL;
	}

	virtual pair<double, double>* estimatedCost(const Evaluation<DS>& e, const R& r, const ADS& ads)
	{
		return cost(e, r, ads);
	}

	virtual bool operator==(const Move<R, ADS, DS>& m) const = 0;

	bool operator!=(const Move<R, ADS, DS>& m) const
	{
		return !(*this == m);
	}

   static string idComponent()
   {
      return "OptFrame:Move";
   }

   virtual string id() const
   {
      return idComponent();
   }

	virtual void print() const = 0;
};

#endif /*OPTFRAME_MOVE_HPP_*/
