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

	//MaterialChanIsTheCoolestUpdater mcitcu;

	DifferentialSolver<State, 1024> dsolver(&load, &refill, &calc, &heatflow, 1.0, 10e-5);

	int x;
	cin >> x;

	return 0;
}