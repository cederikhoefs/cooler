#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "DifferentialSolver.h"
#include "LinearCalculator.h"

using namespace std;


int main() 
{

	Cooler cool("cool.png");

	LinearMaterialLoader load(cool);
	LinearRefiller refill;
	LinearCalculator calc;
	LinearHeatFlowCriterium heatflow;

	DifferentialSolver<State> dsolver(&load, &refill, &calc, &heatflow, 8, 1.0, 10e-5);

	dsolver.run(4);

	int x;
	cin >> x;

	return 0;
}