#ifndef MACROS_TEST
#define MACROS_TEST

#ifdef USEDEBUG
#include <iostream>
#define COUT(X)	std::cout << X << std::endl << std::endl;
#define COUT2(X,Y) std::cout << X << " " << Y << std::endl << std::endl;
#else
#define COUT(X)
#define COUT2(X,Y)
#endif

#ifdef TESTMATRIZ
#include <fstream>
std::ofstream out;
#define GUARDAR(X,Y) out.open(Y,std::ifstream::trunc);\
					for(int i = 0; i < X.size(); ++i){\
						for(int j = 0; j < X[i].size();++j){\
						out << X[i][j];\
						if(j != X[i].size()-1) out << ",";\
						}\
						if(i != X.size()-1) out << ";";\
					}\
					out.close();
#else
#define GUARDAR(X,Y)
#endif



//#ifdef PROFILING
#include <chrono>
#include <iostream>
#define GLUE(X,Y) X##Y
#define TIMER_START(X) std::chrono::high_resolution_clock::time_point X = std::chrono::high_resolution_clock::now();
#define TIMER_STOP(X) std::chrono::high_resolution_clock::time_point GLUE(X,F) = std::chrono::high_resolution_clock::now();\
					  std::chrono::duration<double> GLUE(time_span,X) = std::chrono::duration_cast<std::chrono::duration<double>>(GLUE(X,F) - X);\
					  std::cout << #X << " : " << GLUE(time_span,X).count() << std::endl;
//#else
//#define TIMER_START(X)
//#define TIMER_STOP(X)
//#endif

#endif
