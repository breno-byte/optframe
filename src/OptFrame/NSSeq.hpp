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

#ifndef OPTFRAME_NSSEQ_HPP_
#define OPTFRAME_NSSEQ_HPP_

#include "NS.hpp"
#include "NSIterator.hpp"
#include "NSBlockIterator.hpp"

using namespace std;

namespace optframe
{

template<class R, class ADS = OPTFRAME_DEFAULT_ADS, BaseSolution<R,ADS> S = CopySolution<R,ADS>>
class NSSeq: public NS<R, ADS, S>
{
public:

    virtual ~NSSeq()
    {
    }

////protected:
    virtual Move<R, ADS, S>* randomMove(const R&, const ADS*) = 0;

public:
    NSIterator<R, ADS, S>* getIteratorSolution(const S& s)
    {
        return this->getIterator(s.getR(), s.getADSptr());
    }

////protected:
    virtual NSIterator<R, ADS, S>* getIterator(const R& r, const ADS* ads) = 0;

    virtual NSBlockIterator<R, ADS, S>* getBlockIterator(const S& s)
    {
        NSIterator<R, ADS, S>* it = this->getIteratorSolution(s);
        return new DefaultNSBlockIterator<R, ADS, S>(*it);
    }

    // ============= For 'Local Optimum'-based methods =============

    // GET LOCAL OPTIMUM INFORMATION FROM SOLUTION (ADS PREFERED?)
    virtual LOS getLOS(const S& s)
    {
        return los_unknown;
    }

    // INSERT LOCAL OPTIMUM INFORMATION IN SOLUTION (IN ADS? USER DECIDES.)
    // MAYBE IT's BETTER TO USE ONLY IN ITERATORS! TODO: THINK ABOUT IT...
    //virtual void setLOS(LOS status, S& s)
    //{
    //}

public:
    static string idComponent()
    {
        stringstream ss;
        ss << NS<R, ADS, S>::idComponent() << ":NSSeq";
        return ss.str();
    }

    virtual string id() const
    {
        return idComponent();
    }

    virtual bool compatible(string s)
    {
        return (s == idComponent()) || (NS<R, ADS, S>::compatible(s));
    }
};

}

#endif /*OPTFRAME_NSSEQ_HPP_*/
