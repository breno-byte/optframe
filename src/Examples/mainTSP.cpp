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

// ===================================
// Main.cpp file generated by OptFrame
// Project Traveling Salesman Problem
// ===================================

// mainTSP.cpp

#include <math.h>
#include <stdlib.h>

#include <iostream>

using namespace std;

#include <set>

#include "../OptFrame/Util/printable.h"
#include "../OptFrame/BaseConcepts.ctest.hpp" // tsting concepts
#include "../OptFrame/Solution.hpp"
#include "../OptFrame/Evaluation.hpp"
//#include "../OptFrame/Util/TestSolution.hpp"

#include "../OptFrame/Heuristics/EvolutionaryAlgorithms/BRKGA.hpp"
#include "../OptFrame/Loader.hpp"
////#include "../OptFrame/Util/BuildCommand.hpp"  // TODO: return after Concepts OptFrame v4
#include "../OptFrame/Util/CheckCommand.hpp"
#include "TSP.h"

using namespace TSP;
using namespace scannerpp;

#include "../OptFrame/Util/PackTypes.hpp"



int
main(int argc, char** argv)
{
   // ADS still exists, only because of ADSManager...
   Loader<RepTSP, OPTFRAME_DEFAULT_ADS, SolutionTSP, EvaluationTSP, ESolutionTSP> optframe;
   TSPProblemCommand tsp;

   File* file;

   try {
      file = new File("./TSP/tsplib/berlin52.txt");
   } catch (FileNotFound& f) {
      cout << "File not found" << endl;
      return 1;
   }

   Scanner scanner(file);
   tsp.load(scanner, optframe.factory, optframe.dictionary, optframe.ldictionary);

   /*
	FILE* outf = fopen("berlin52.mtx","w");
	fprintf(outf, "%d\n", tsp.p->n);
	for(unsigned i=0; i<tsp.p->n; i++) {
		for(unsigned j=0; j<tsp.p->n; j++)
			fprintf(outf, "%d\t",int(100*(*tsp.p->dist)(i,j)));
		fprintf(outf, "\n");
	}

	fclose(outf);
	exit(1);
	*/

   bool check_verbose = false;
   CheckCommand<RepTSP, OPTFRAME_DEFAULT_ADS, SolutionTSP> check(check_verbose);

   RandGenMersenneTwister rg(0);
   TSPEvaluator eval1(tsp.p); // Should not be Specific to TSP!! Won't work on Decoder..
   Evaluator<SolutionTSP>& eval = eval1;

   RandomInitialSolutionTSP randomTSP(tsp.p, eval, rg);
   NearestNeighborConstructive cnn(tsp.p, eval, rg);
   ConstructiveBestInsertion cbi(tsp.p, eval, rg);
   NSEnumSwap enumswap(tsp.p, rg);

   NSSeqTSP2Opt<int, OPTFRAME_DEFAULT_ADS, SolutionTSP, DeltaMoveTSP2Opt, ProblemInstance> nsseq_delta_2opt(tsp.p);
   NSSeqTSP2Opt<int, OPTFRAME_DEFAULT_ADS, SolutionTSP> tsp2opt;
   NSSeqTSPOrOptk<int, OPTFRAME_DEFAULT_ADS, SolutionTSP, DeltaMoveTSPOrOptk, ProblemInstance> nsseq_delta_or1(1, tsp.p);
   NSSeqTSPOrOptk<int, OPTFRAME_DEFAULT_ADS, SolutionTSP> tspor1(1);
   NSSeqTSPOrOptk<int, OPTFRAME_DEFAULT_ADS, SolutionTSP> tspor2(2);
   NSSeqTSPOrOptk<int, OPTFRAME_DEFAULT_ADS, SolutionTSP> tspor3(3);

   // TODO: we need to try NSSeqTSPOrOpt , because it requires adapters...
   NSSeqTSPOrOpt<int, OPTFRAME_DEFAULT_ADS, SolutionTSP> tspor_adapt;
   // Maybe S& should be the Representation itself over there.... no getR() inside there.
   // It makes more sense to pass RepTSP + ESolutionTSP... than SolutionTSP + ESolutionTSP
   // Then, should adapters just work for R,ADS pair on XBaseSolution concept?? TODO: think... 



   NSSeqTSPSwap<int, OPTFRAME_DEFAULT_ADS, SolutionTSP> tspswap;

   check.add(randomTSP);
   check.add(cnn);
   check.add(cbi);
   check.add(eval);
   check.add(enumswap);
   check.add(nsseq_delta_2opt);
   check.add(tsp2opt);
   check.add(nsseq_delta_or1);
   check.add(tspor1);
   check.add(tspor2);
   check.add(tspor3);
   check.add(tspswap);

   //check.run(100, 10);

   cout << "will test BRKGA (n=" << tsp.p->n << ")" << endl;
   //Evaluator<SolutionTSP>& eval2 = eval;
   EvaluatorPermutationRandomKeys<SolutionTSP> eprk(eval, 0, tsp.p->n - 1);
   BRKGA<RepTSP, SolutionTSP> brkga(eprk, tsp.p->n, 10000, 10, 0.4, 0.3, 0.6);

   StopCriteria<EvaluationTSP> sosc;
   // strange that this worked.... it's against 'override' pattern. Very strange!!
   /*
   pair<CopySolution<random_keys>, Evaluation<>>* r2 = brkga.search(sosc);
   r2->first.print();
   pair<Evaluation<>, CopySolution<vector<int>>*> pd = eprk.decode(r2->first.getR());
   pd.second->print();
   if (eval.verify(pd.second->getR()))
      cout << "CHECK: OK" << endl;
   pd.first.print();
   */

   //pair<SolutionTSP, Evaluation<>>* r2 = brkga.search(sosc);
   
   std::optional<ESolutionTSP> r2 = std::nullopt;
   brkga.search(r2, sosc.start());
   //virtual std::optional<pair<XRS, XEv>> search(StopCriteria<XEv>& stopCriteria, const std::optional<pair<XRS, XEv>> input)
   r2->first.print();
   r2->second.print();

   cout << "end BRKGA tests" << endl;


   // TODO: return after refactor on Concepts and OptFrame v4
   /*
   BuildCommand<RepTSP, OPTFRAME_DEFAULT_ADS, SolutionTSP> build;
   for (unsigned i = 0; i <= 7; i++) {
      stringstream ss;
      ss << "OptFrame:ComponentBuilder:LocalSearch:BI  OptFrame:Evaluator 0  OptFrame:NS:NSSeq " << i;
      string name = build.run(optframe.factory, optframe.dictionary, optframe.ldictionary, ss.str());
      cout << "BUILT: '" << name << "'" << endl;
   }
   */

   vector<LocalSearch<ESolutionTSP>*> ns_list;
   ns_list.push_back(new BestImprovement<ESolutionTSP>(eval, tsp2opt));
   ns_list.push_back(new BestImprovement<ESolutionTSP>(eval, tspor1));
   ns_list.push_back(new BestImprovement<ESolutionTSP>(eval, tspor2));
   ns_list.push_back(new BestImprovement<ESolutionTSP>(eval, tspor3));
   ns_list.push_back(new BestImprovement<ESolutionTSP>(eval, tspswap));
   for (unsigned i = 0; i < ns_list.size(); i++)
      ns_list[i]->setVerbose();

   VariableNeighborhoodDescent<ESolutionTSP> VND(eval, ns_list);
   VND.setVerbose();

   ILSLPerturbationLPlus2<ESolutionTSP> pert(eval, tsp2opt, rg);
   pert.add_ns(tspor1);
   pert.add_ns(tspor2);
   pert.add_ns(tspor3);
   pert.add_ns(tspswap);

   IteratedLocalSearchLevels<ESolutionTSP> ils(eval, randomTSP, VND, pert, 3, 2);
   //ils.setMessageLevel(4);
   ils.setVerbose();
   if (ils.information)
      cout << "infomation is on for ILS" << endl;

   cout << "will run ils" << endl;
   Timer tim;
   StopCriteria<EvaluationTSP> soscILS;
   soscILS.timelimit = 3; // 1000
   soscILS.target_f = EvaluationTSP(0.0);
   //pair<CopySolution<RepTSP>, Evaluation<>>& psol = *ils.search(soscILS, NULL, NULL);
   std::optional<ESolutionTSP> psol = std::nullopt;
   ils.search(psol, soscILS);
   cout << "finished ILS!" << endl;
   cout << tim.now() << " secs" << endl;

   psol->first.print();
   psol->second.print();
   cout << endl << endl;

   // ===========

   for (unsigned i = 0; i < ns_list.size(); i++)
      delete ns_list[i];

   vector<NS<ESolutionTSP>*> v_ns;
   vector<NSSeq<ESolutionTSP>*> v_nsseq = {&tsp2opt, &tspor1, &tspor2, &tspor3, &tspswap};
   //v_nsseq.push_back(&tsp2opt);
   //v_nsseq.push_back(&tspor1);
   //v_nsseq.push_back(&tspor2);
   //v_nsseq.push_back(&tspor3);
   //v_nsseq.push_back(&tspswap);
   for (unsigned i = 0; i < v_nsseq.size(); i++)
      v_ns.push_back(v_nsseq[i]);

   BasicVNS<ESolutionTSP> vns(eval, randomTSP, v_ns, v_nsseq);
   vns.setMessageLevel(3); // INFORMATION
   StopCriteria<EvaluationTSP> soscVNS;
   soscVNS.timelimit = 2; // 2 seconds
   soscVNS.target_f = EvaluationTSP(7550.0);

   // zero is better than any positive value
   assert(eval.betterThan(EvaluationTSP(0), soscVNS.target_f));

   //pair<CopySolution<RepTSP>, Evaluation<>>& psol2 = *vns.search(sosc, NULL, NULL);
   std::optional<ESolutionTSP> psol2 = std::nullopt;
   vns.search(psol2, soscVNS.start());
   psol2->first.print();
   psol2->second.print();

   // Remember the old times...
   /*
	 echo building VND
	 define vnd_list [ OptFrame:LocalSearch: 0 ,  OptFrame:LocalSearch: 1, OptFrame:LocalSearch: 2, OptFrame:LocalSearch: 3 ]
	 component.create_list $vnd_list OptFrame:LocalSearch: comp_vnd_list
	 build OptFrame:LocalSearch:VND   $Evaluator 0   $comp_vnd_list   vnd

	 %component.list

	 echo building ILS
	 build OptFrame:ComponentBuilder:SingleObjSearch:ILS:ILSLevels   $Evaluator 0    $Constructive 0    $vnd   OptFrame:ILS:LevelPert:LPlus2 0    100    8  meu_ils

	 test 2 3 7000 7000   $Evaluator 0   $meu_ils   output.txt   solucao_saida

	 evaluate $Evaluator 0 $solucao_saida
	 */

   cout << "Program ended successfully" << endl;

   return 0;
}
