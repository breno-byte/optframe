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

#ifndef TSP_BEST_INSERTION_CONSTRUCTIVE_HPP_
#define TSP_BEST_INSERTION_CONSTRUCTIVE_HPP_

#include <OptFrame/Constructive.hpp>
//#include "../../OptFrame/Util/TestSolution.hpp" // DEPRECATED

#include "ProblemInstance.h"

#include "Representation.h"
#include "Solution.h"

#include "Evaluator.h"
#include <OptFrame/RandGen.hpp>
#include <list>

#include <algorithm>
#include <stdlib.h>

using namespace std;

namespace TSP
{

//class ConstructiveBestInsertion: public Constructive<SolutionTSP>
class ConstructiveBestInsertion: public InitialSearch<ESolutionTSP, EvaluationTSP>
{
private:
	ProblemInstance* pI;
   GeneralEvaluator<ESolutionTSP>& eval;
	RandGen& rg;

    static bool compare(const pair<double, pair<int,int> >& p1, const pair<double, pair<int,int> >& p2)
    {
        return p1.first < p2.first;
    }

public:

	ConstructiveBestInsertion(ProblemInstance* pI, GeneralEvaluator<ESolutionTSP>& _eval, RandGen& _rg) :
      eval(_eval), rg(_rg) // If necessary, add more parameters
	{
		this->pI = pI;
		// Put the rest of your code here
	};

	virtual ~ConstructiveBestInsertion()
	{
	}

	//std::optional<SolutionTSP> generateSolution(double timelimit) override
   std::pair<std::optional<ESolutionTSP>, SearchStatus> initialSearch(const StopCriteria<>& sosc) override
	{
        //cout << "Generating solution" << endl;
		RepTSP newRep;
        vector<bool> used(pI->n, false);

        int first = rg.rand(pI->n);

        newRep.push_back(first);
        used[first] = true;

        int second = rg.rand(pI->n);
        while(second == first)
        	second = rg.rand(pI->n);

        newRep.push_back(second);
        used[second] = true;


        while(((int)newRep.size()) < pI->n)
        {
            vector<pair<double, pair<int,int> > > candidates;

            //cout << "BASE: " << newRep << endl;

            for(unsigned i=0; i<used.size(); i++)
                if(!used[i])
                	for(unsigned j=0; j<newRep.size(); j++)
                	{
                		// city i will be in position j
                		unsigned bj = j-1;
                		if(j==0)
                			bj = newRep.size()-1;

                		double cost = -(*pI->dist)(newRep[bj], newRep[j]); // remove arc
                		//cout << "Candidate: " << " i=" <<i << " j=" << j << " cost=" << cost << endl;
                		cost += (*pI->dist)(newRep[bj], i);
                		//cout << "Candidate: " << " i=" <<i << " j=" << j << " cost=" << cost << endl;
                		cost += (*pI->dist)(i, newRep[j]);
                		//cout << "Candidate: " << " i=" <<i << " j=" << j << " cost=" << cost << endl;
                		candidates.push_back(make_pair( cost , make_pair(i,j) ));

                	}


            //cout << "before sort: " << newRep << endl;
            sort(candidates.begin(), candidates.end(), compare);

            int best_pos  = candidates[0].second.second;
            int best_city = candidates[0].second.first;

            // CHECK

            /*
            cout << "Solution is now: " << newRep << endl;
            cout << "Best insertion is: city=" << best_city << " in position=" << best_pos << " value=" << candidates[0].first << endl;
            */

            newRep.insert(newRep.begin()+best_pos, best_city);
            used[best_city] = true;

            /*

            int bef_pos = best_pos-1;
            int aft_pos = best_pos+1;
            if(best_pos==0)
            	bef_pos = ((int)newRep.size())-1;
            if(best_pos==((int)newRep.size()))
            	aft_pos = 0;

            double calcCost = -(*pI->dist)(newRep[bef_pos], newRep[aft_pos]);
            cout << "calcCost = -d(" << newRep[bef_pos] << "," << newRep[aft_pos] << ") ";
            calcCost += (*pI->dist)(newRep[bef_pos], newRep[best_pos]);
            cout << "calcCost += d(" << newRep[bef_pos] << "," << newRep[best_pos] << ") ";
            calcCost += (*pI->dist)(newRep[best_pos], newRep[aft_pos]);
            cout << "calcCost += d(" << newRep[best_pos] << "," << newRep[aft_pos] << ") " << endl;
            cout << "Solution became: " << newRep << endl;


            if(candidates[0].first != calcCost)
            {
            	cout << "DIFFERENCE BETWEEN COSTS! Best: "<<candidates[0].first << " CALC: " << calcCost << endl;
            	exit(1);
            }
            */

            //cout << "after sort: " << newRep << endl;
        }

		//return new CopySolution<RepTSP>(newRep);
      //return make_optional(SolutionTSP(newRep));
      EvaluationTSP etsp;
      ESolutionTSP se(SolutionTSP(newRep), etsp);
      eval.reevaluate(se);
      return make_pair(make_optional(se), SearchStatus::NO_REPORT);
	}

};

}

#endif /*TSP_BEST_INSERTION_CONSTRUCTIVE_HPP_*/
