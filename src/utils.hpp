#ifndef UTILS_HEADER
#define UTILS_HEADER

#include <iostream>
#include <vector>
#include <cmath>  
#include <cstdlib>
#include <ctime>
#include <fstream>

using std::string;
using std::vector;

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector< std::vector<T> >& matrix ) {
	
	T max = 0;
	stream.precision(8);
	stream << std::fixed;
    
	for (auto i = matrix.begin(); i != matrix.end(); ++i ) {
		T maxi = 0;
		
		for( auto j = i->begin(); j != i->end() ; ++j ) {
			T act = std::abs(*j);
			if(act > maxi) maxi = act;
		}
		
		if (maxi > max ) {
			max = maxi; 
		}		
	}
	for(auto i = matrix.begin(); i != matrix.end(); ++i ){
		for (auto j = i->begin(); j != i->end(); ++j ) {
			stream << (double)*j;			
			stream << " ";
			
			//if((std::abs(*j) < 1.0)) stream << (std::signbit(*j)) << " " << (int)log10(max);
			for(int k = (std::abs(*j) < 1.0 ? 0 : log10(std::abs(*j))) + std::signbit(*j); k < (int)log10(max)+1; ++k ) {
			stream << " "; 	
			}
			stream << "| ";
		}
		if(i+1 != matrix.end()) stream << std::endl;			
	}

	return stream;	

}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vec ) {
	stream << "( ";
	for(int i = 0; i < vec.size(); ++i) {
		stream << vec[i];
		if(i != vec.size()-1) stream << ", ";
	}
	stream << " )";
	return stream;
}

void generarParticion(unsigned int cantTrain, std::string archivoSalida){

if(cantTrain >42000)  return;
	std::srand(std::time(0));
	std::vector<int> particion(42000);

	if(cantTrain<21000){
	
		for(unsigned int i = 0 ; i < cantTrain; ++i){
	
			unsigned int pos = std::rand()%42000;
			
			while( particion[pos] == 1) pos = std::rand()%42000;
		
			particion[pos] = 1;
		
		}
	
	}else{
		for(unsigned int i = 0 ; i < particion.size(); ++i) particion[i] = 1;
		
		for(unsigned int i = 0 ; i < (particion.size()-cantTrain); ++i){
	
			unsigned int pos = std::rand()%42000;
			
			while( particion[pos] == 0) pos = std::rand()%42000;
		
			particion[pos] = 0;
		
			}
		
	}	
	
	std::ofstream salida;
	salida.open(archivoSalida, std::ios::trunc); //Abro el archivo de salida
	
	for(int j = 0; j < particion.size(); ++j) {
		salida << particion[j] <<" ";
	}
	
	salida.close();
}

std::vector<std::vector<bool> > generarParticionesRand(unsigned int cantTrain, unsigned int K){   //K = cantidad de particiones a crear

	std::srand(std::time(0));
	
	std::vector<std::vector<bool> > particiones(K);
	
	for( int j = 0 ; j < K ; j++){
	
		std::vector<bool> particion(42000);

		if(cantTrain<21000){
	
			for(unsigned int i = 0 ; i < cantTrain; ++i){
	
				unsigned int pos = std::rand()%42000;
			
				while( particion[pos] == 1) pos = std::rand()%42000;
		
				particion[pos] = 1;
		
			}
	
		}else{
			for(unsigned int i = 0 ; i < particion.size(); ++i) particion[i] = 1;
		
			for(unsigned int i = 0 ; i < (particion.size()-cantTrain); ++i){
	
				unsigned int pos = std::rand()%42000;
			
				while( particion[pos] == 0) pos = std::rand()%42000;
			
				particion[pos] = 0;
		
			}
		
		}	
		
		particiones[j] = particion;
		
	}	
return particiones;
}

vector<bool> ponerCeros(const vector<bool>& vec, int i, int j ){
	vector<bool>res(vec);
	for(int k = i; k<=j ; k++) res[k] = 0;
	return res;
}



vector<vector<vector<bool> > > KCrossValidation(vector<int> porcentajes, int totalDeMuestras){  //totalDeMuestras es 42000 en nuestro caso
	  
		vector<vector<vector<bool> > > particionesG(porcentajes.size());
		vector<bool> unos(totalDeMuestras,1);
		
		for (int p = 0; p< porcentajes.size(); p++){
				
				int K = 100/(100-porcentajes[p]);
				int cantTest = totalDeMuestras/K;
				vector<vector <bool> > particiones(K);
				
				for (int j = 0 ; j< K ; j++) particiones[j] = ponerCeros(unos, j*cantTest, (j+1)*cantTest-1);
				particionesG[p] = particiones;
		
		}
	
	
return particionesG;
}



void generarEntrada(string path, int k, int alpha, int gamma, unsigned int cantTrain, unsigned int K, string archivoSalida){   //K = cantidad de particiones a crear

	std::srand(std::time(0));
	
	std::vector<std::vector<int> > particiones(K);
	
	for( int j = 0 ; j < K ; j++){
	
		std::vector<int> particion(42000);

		if(cantTrain<21000){
	
			for(unsigned int i = 0 ; i < cantTrain; ++i){
	
				unsigned int pos = std::rand()%42000;
			
				while( particion[pos] == 1) pos = std::rand()%42000;
		
				particion[pos] = 1;
		
			}
	
		}else{
			for(unsigned int i = 0 ; i < particion.size(); ++i) particion[i] = 1;
		
			for(unsigned int i = 0 ; i < (particion.size()-cantTrain); ++i){
	
				unsigned int pos = std::rand()%42000;
			
				while( particion[pos] == 0) pos = std::rand()%42000;
			
				particion[pos] = 0;
		
			}
		
		}	
		
		particiones[j] = particion;
		
	}	
		std::ofstream salida;
		salida.open(archivoSalida, std::ios::trunc); //Abro el archivo de salida
	
		salida << path << " " << k << " " << alpha << " " << gamma << " " << K << std::endl; 
		
		for(int i = 0 ; i < K ; ++i){
			
			std::vector<int> particion = particiones[i];
			
			for(int j = 0; j < particion.size(); ++j) {
				salida << particion[j] <<" ";
			}
			salida<< std::endl;
		
		}		
		
	salida.close();
}

#endif
