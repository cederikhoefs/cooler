#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "LinearCalculator.h"

using namespace std;

class Cooler
{

	ifstream file;
	string path;

	vector<vector<bool>> bitarray;
	vector<bool> octree;

public:

	Cooler(string path);
	Cooler(ifstream file);





};