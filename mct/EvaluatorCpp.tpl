#include "Evaluator.h"

using namespace $project;

$projectEvaluator::$projectEvaluator(ProblemInstance& _p$project): // If necessary, add more parameters
p$project(_p$project)
{
	// Put the rest of your code here
}
	
$projectEvaluator::~$projectEvaluator()
{
}

Evaluation$project& $projectEvaluator::evaluate(const Rep$project& rep, const MY_ADS&)
{
    // 'rep' is the representation of the solution

    double fo = 0; // Evaluation Function Value

    return * new Evaluation$project(fo $initializedelta);
}

bool $projectEvaluator::betterThan(const Evaluation< $deltaproject >& e1, const Evaluation< $deltaproject >& e2)
{
    // $minmax
    return $epsilon
}
	
bool $projectEvaluator::isMinimization()
{
    // $minmax
    return " $minmax " == " MINIMIZATION ";
}