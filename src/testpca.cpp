#include <utility>
#include <iostream>
#include <vector>
#include "operadores.hpp"
#include <math.h>
#include "PCA.hpp"
#include <algorithm>
#include "knn.hpp"
#include "utils.hpp"
using std::cout;
using std::endl;
using std::vector;

int main() {

	vector<vector<unsigned char> > matrizInit = { { 2 , 1 , 4 } ,
												  { 1 , 2 , 3 } , 
												  { 5 , 4 , 3 } ,
												 };


	PCA estructura(matrizInit, 1);
	
	return 0;


}