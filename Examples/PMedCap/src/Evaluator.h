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

#ifndef PCAP_EVALUATOR_HPP_
#define PCAP_EVALUATOR_HPP_

//#include <cmath>
#include <stdlib.h>
#include <math.h>

#include <OptFrame/Evaluation.hpp>
#include <OptFrame/Evaluator.hpp>

#include "Representation.h"
#include "Solution.h"
#include "Evaluation.h"

#include "ProblemInstance.h"

using namespace optframe;

#define EPSILON_PCAP 0.0001

class PCAPEvaluator: public Evaluator<SolutionPCAP>
{
private:
	PCAPProblemInstance& pPCAP;

	// Your private vars

public:
	//using Evaluator<ESolutionPCAP>::evaluate;

	PCAPEvaluator(PCAPProblemInstance& _pPCAP) : // If necessary, add more parameters
		pPCAP(_pPCAP)
	{
		// Put the rest of your code here
	}

	Evaluation<> evaluate(const SolutionPCAP& s) override
	{
      const RepPCAP& rep = s.getR();
		// 'rep' is the representation of the solution

		double fo = 0; // Evaluation<> Function Value
		double fo_Inv = 0;

		vector<int> utilizacao;
		for (unsigned int i = 0; i < rep.first.size(); i++)
			utilizacao.push_back(0);

		for (unsigned int i = 0; i < rep.second.size(); i++)
		{
			int med = rep.first[rep.second[i]];
			int indiceMed = rep.second[i];

			double x = pPCAP.vecCidades[i].x;
			double y = pPCAP.vecCidades[i].y;
			int d = pPCAP.vecCidades[i].demanda;

			utilizacao[indiceMed] += d;
			if (utilizacao[indiceMed] > pPCAP.vecCidades[med].capacidade)
				fo_Inv += utilizacao[indiceMed] - pPCAP.vecCidades[med].capacidade;

			double xMed = pPCAP.vecCidades[med].x;
			double yMed = pPCAP.vecCidades[med].y;

			fo += sqrt(pow((x - xMed), 2) + pow((y - yMed), 2));
			//cout << "i = " << i << " fo = " << fo << endl;
			//getchar();
		}

		double fo_medAlocada = 0; //Garante que a mediana eh atendida por ela propria

		for (unsigned int i = 0; i < rep.first.size(); i++)
		{
			int med = rep.first[i];
			if (rep.second[med] != (int)i)
				fo_medAlocada += 100000;

		}

		fo_Inv = fo_Inv * 10000 + fo_medAlocada;

		return Evaluation<>(fo, fo_Inv);
	}

	virtual bool betterThan(double f1, double f2)
	{
		// MINIMIZATION
		return (f1 < (f2 - EPSILON_PCAP));
	}

	virtual bool isMinimization() const
	{
		return true;
	}
};

static_assert(std::is_base_of<Evaluator<SolutionPCAP>, PCAPEvaluator>::value,  "not inherited from Evaluator");
static_assert(std::is_base_of<GeneralEvaluator<ESolutionPCAP>, PCAPEvaluator>::value,  "not inherited from GeneralEvaluator");

#endif /*PCAP_EVALUATOR_HPP_*/

