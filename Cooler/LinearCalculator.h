#pragma once

#include "DifferentialSolver.h"
#include "OctreeCooler.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

enum class Material { AMMONIA, COOLER, ALMIGHTY_CHAN_THE_COOLEST };
struct State
{

	Temperature temperature;
	Material material;
	Temperature refilled;

};

class LinearMaterialLoader : public AbstractFunctor<State>
{
public:
	void operator() (State const * oldbuffer, State * newbuffer, Constants c)
	{
		for (int i = 0; i < c.res; i++) {
			for (int j = 0; j < c.res; j++) {

				switch (newbuffer[i + j * c.res].material) {

				case Material::ALMIGHTY_CHAN_THE_COOLEST:
					newbuffer[i + j * c.res].refilled = 0 - oldbuffer[i + j * c.res].temperature;
					newbuffer[i + j * c.res].temperature = 0;
					break;

				case Material::AMMONIA:
					newbuffer[i + j * c.res].refilled = 1.0 - oldbuffer[i + j * c.res].temperature;
					newbuffer[i + j * c.res].temperature = 1.0;
					break;
				default:
					break;
				}

			}
		}
	}

	LinearMaterialLoader(Cooler chan)
	{



	}

};

class LinearCalculator : public AbstractFunctor<State>
{
public:
	void operator() (State const * oldbuffer, State * newbuffer, Constants c) 
	{
		for (int i = 0; i < c.res; i++) {
			for (int j = 0; j < c.res; j++) {
				State x = oldbuffer[i + j * c.res];
				x.temperature += Laplace({i, j}, oldbuffer, c.res) * c.tDelta;
				newbuffer[i + j * c.res] = x;
			}
		}
	}

	float Laplace(Position p, State const * buffer, int res)
	{
		float point = 2 * temp_at(p, buffer, res);
		float ddxddx = temp_at({p.i - 1, p.j}, buffer, res) + temp_at({p.i + 1, p.j}, buffer, res) - point;
		float ddyddy = temp_at({p.i, p.j - 1}, buffer, res) + temp_at({p.i, p.j + 1}, buffer, res) - point;
		return ddxddx + ddyddy;
	}

	float temp_at(Position p, State const* buffer, int res)
	{
		return buffer[p.i % res + (p.j % res) * res].temperature;
	}
};

class LinearRefiller: public AbstractFunctor<State>
{
public:
	void operator() (State const * oldbuffer, State * newbuffer, Constants c)
	{
		for (int i = 0; i < c.res; i++) {
			for (int j = 0; j < c.res; j++) {

				switch (newbuffer[i + j * c.res].material) {

				case Material::ALMIGHTY_CHAN_THE_COOLEST:
					newbuffer[i + j * c.res].refilled = 0-oldbuffer[i + j * c.res].temperature;
					newbuffer[i + j * c.res].temperature = 0;
					break;

				case Material::AMMONIA:
					newbuffer[i + j * c.res].refilled = 1.0 - oldbuffer[i + j * c.res].temperature;
					newbuffer[i + j * c.res].temperature = 1.0;
					break;
				default:
					break;
				}

			}
		}
	}
};

class LinearHeatFlowCriterium : public ExitCriterium<State>
{
public:
	bool operator() (State* actual, Constants c) const
	{
		cout << "Henlo ein Abbluchklitelium" << endl;
		return true;
	}
};



/* OPENCL-Quatsch
Platform platform = Platform::getDefault();					//Standardplattform
Context context(CL_DEVICE_TYPE_DEFAULT);					//Context mit Standarddevice
Device gpu = context.getInfo<CL_CONTEXT_DEVICES>()[0];		//Erstes Device aus Liste aller Devices im Context
CommandQueue queue(context); */