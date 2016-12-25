// #ifndef NONOGRAMSOLVER
// #define NONOGRAMSOLVER

// #include <vector>
// #include <iostream>
// #include <sstream>
// #include <fstream>
// #include <string.h>
// #include <iomanip>
#include "NonogramSolver.h"
using namespace std ;

typedef char State ;
// const State empty = 2 ;
// const State full = 1 ;
// const State unknown = 0 ;


Range::Range()
{
	cntBlack=0;
}

Record_type::Record_type()
{
	LinePutToLimit = 0;
	AllPutToLimit = 0;
}


// #endif