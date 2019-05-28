#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "DifferentialSolver.h"
#include "LinearCalculator.h"

using namespace std;


int main() 
{

	LinearRefiller refill;
	LinearCalculator calc;
	LinearHeatFlowCriterium heatflow;

	//MaterialChanIsTheCoolestUpdater mcitcu;

	DifferentialSolver<State, 1024> dsolver(&refill, &calc, &heatflow, 1, 1024);

	int x;
	cin >> x;

	return 0;
}