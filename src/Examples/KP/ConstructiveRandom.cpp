#include "ConstructiveRandom.h"

using namespace KP;

ConstructiveRandom::ConstructiveRandom(ProblemInstance& _pKP)
  : pKP(_pKP)
{
}

ConstructiveRandom::~ConstructiveRandom()
{
}

//std::optional<SolutionKP>
//ConstructiveRandom::generateSolution(double timelimit)
std::optional<ESolutionKP>
ConstructiveRandom::initialSearch(const StopCriteria<EvaluationKP>& sosc)
{
   //double timelimit = sosc.timelimit;
   srand(5);
   RepKP newRep(pKP.N);

   for (int i = 0; i < pKP.N; i++)
      newRep[i] = rand() % 2;

   return make_optional(ESolutionKP(newRep, EvaluationKP()));
}
