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

template <typename T> //TODO: Dimension 
class DifferentialSolver 
{
	T* New;	//[Resolution * Resolution]; 
	T* Old;	//[Resolution * Resolution];

	Constants c;

	int Resolution;

	AbstractFunctor<T>* initial;
	AbstractFunctor<T>* updator;
	AbstractFunctor<T>* calculator;
	
	ExitCriterium<T>* exitcriterium;

public:

	DifferentialSolver<T>::DifferentialSolver(AbstractFunctor<T> * init, AbstractFunctor<T> * update, AbstractFunctor<T> * operation, ExitCriterium<T> * exit, int resolution, Length scale, Time tDelta);

	~DifferentialSolver();

	inline void step();
	inline bool end();
	inline void swap();

	T& at(Position p);

	T* run(int maxsteps = 10000);

};

// Fricking Templates...
//******************************************************************************

template<typename T>
DifferentialSolver<T>::DifferentialSolver(AbstractFunctor<T> * init, AbstractFunctor<T>* update, AbstractFunctor<T>* operation, ExitCriterium<T>* exit, int resolution, Length scale, Time tDelta)
{
	// Dynamische Alloziierung für höhere Dimensionen?

	Resolution = resolution;

	New = new T[Resolution * Resolution];
	Old = new T[Resolution * Resolution];

	c.epsilon = scale / Resolution;
	c.tDelta = tDelta;
	c.res = Resolution;

	initial = init;
	updator = update;
	calculator = operation;
	
	exitcriterium = exit;

}

template <typename T>
DifferentialSolver<T>::~DifferentialSolver()
{
	// Dynamische Speicherverwaltung für höhere Dimensionen ?
	delete[] this->New;
	delete[] this->Old;
	
}

template<typename T>
inline bool DifferentialSolver<T>::end()
{
	return (*exitcriterium)(New, c);
}

template<typename T>
inline void DifferentialSolver<T>::step()
{
	(*updator)(Old, New, c);
	(*calculator)(Old, New, c);
}

template<typename T>
inline T& DifferentialSolver<T>::at(Position p)
{
	return New[p.i][p.j];
}

template<typename T>
inline void DifferentialSolver<T>::swap()
{
	T* temp = New;
	New = Old;
	Old = temp;

}

template<typename T>
T * DifferentialSolver<T>::run(int maxsteps)
{
	int steps = 0;

	(*initial)(New, Old, c); //Beide Arrays initialisieren
	(*initial)(Old, New, c);

	do {
		swap(); //Arrays vertauschen
		step();
		steps++;
	} while (!end() && steps < maxsteps);
	return New;
}

