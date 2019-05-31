#pragma once
#include <string>

using namespace std;

typedef float Time;
typedef float Length;
typedef float Temperature;

struct Position
{

	int i, j;

};
struct Constants
{
	Length epsilon;
	Time tDelta;
	int res;
};

template <typename T>
class AbstractFunctor 
{
public:
	virtual void operator() (T const * oldbuffer, T* newbuffer, Constants c) = 0; 
};

template <typename T>
class ExitCriterium
{
public:
	virtual bool operator() (T* actual, Constants c) const = 0;
};

template <typename T, int Resolution> //TODO: Dimension 
class DifferentialSolver 
{
	T* Actual;	//[Resolution * Resolution]; 
	T* Last;	//[Resolution * Resolution];

	Constants c;

	AbstractFunctor<T>* initial;
	AbstractFunctor<T>* updator;
	AbstractFunctor<T>* calculator;
	
	ExitCriterium<T>* exitcriterium;

public:

	DifferentialSolver<T, Resolution>::DifferentialSolver(AbstractFunctor<T> * init, AbstractFunctor<T> * update, AbstractFunctor<T> * operation, ExitCriterium<T> * exit, Length scale, Time tDelta);

	~DifferentialSolver();

	inline void step();
	inline bool end();
	inline void swap();

	T& at(Position p);

	T* run(int maxsteps = 10000);

};

// Fricking Templates...
//******************************************************************************

template<typename T, int Resolution>
DifferentialSolver<T, Resolution>::DifferentialSolver(AbstractFunctor<T> * init, AbstractFunctor<T>* update, AbstractFunctor<T>* operation, ExitCriterium<T>* exit, Length scale, Time tDelta)
{
	// Dynamische Alloziierung für höhere Dimensionen?

	Actual = new T[Resolution * Resolution];
	Last = new T[Resolution * Resolution];

	c.epsilon = scale / Resolution;
	c.tDelta = tDelta;
	c.res = Resolution;

	initial = init;
	updator = update;
	calculator = operation;
	
	exitcriterium = exit;

}

template <typename T, int Resolution>
DifferentialSolver<T, Resolution>::~DifferentialSolver()
{
	// Dynamische Speicherverwaltung für höhere Dimensionen ?
	delete[] this->Actual;
	delete[] this->Last;
	
}

template<typename T, int Resolution>
inline bool DifferentialSolver<T, Resolution>::end()
{
	return (*exitcriterium)(Actual, c);
}

template<typename T, int Resolution>
inline void DifferentialSolver<T, Resolution>::step()
{
	(*calculator)(Last, Actual, c);
}

template<typename T, int Resolution>
inline T& DifferentialSolver<T, Resolution>::at(Position p)
{
	return Actual[p.i][p.j];
}

template<typename T, int Resolution>
inline void DifferentialSolver<T, Resolution>::swap()
{
	T* temp = Actual;
	Actual = Last;
	Last = temp;

}

template<typename T, int Resolution>
T * DifferentialSolver<T, Resolution>::run(int maxsteps)
{
	int steps = 0;

	do {
		swap(); //Arrays vertauschen
		step();
		steps++;
	} while (!end() && steps < maxsteps);
	return 
}

