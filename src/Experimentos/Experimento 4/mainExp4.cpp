#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "time.h"
#include <utility>  
#include <math.h> 
#include <algorithm>
#include "utils.hpp"
#include "knn.hpp"
#include "PCA.hpp"
#include "PLS.hpp"
#include <cassert>
#include <random>

#define IMPRIMIR_METRICAS
#include "metricas.hpp"
//#define PROFILING
#include "macrostest.hpp"

#define MUESTRAS 42000

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::pair;


vector<vector<unsigned char> > generarTrain(const vector<vector<unsigned char> >& imagenes, const vector<bool>& particion){
	vector<vector<unsigned char> > res;
	for(int i = 0; i < imagenes.size(); i++){
		if(particion[i]) res.push_back(imagenes[i]);
	}
return res;
}

vector<vector<unsigned char> > generarTest(const vector<vector<unsigned char> >& imagenes, vector<bool>& particion){
	vector<vector<unsigned char> > res;
	for(int i = 0; i < imagenes.size(); i++){
		if(!particion[i]) res.push_back(imagenes[i]);
	}
return res;
}

vector<unsigned int > generarIndicesDeTest(const vector<vector<unsigned char> >& imagenes, vector<bool>& particion){
	vector<unsigned int > res;
	for(int i = 0; i < imagenes.size(); i++){
		if(!particion[i]) res.push_back(i);
	}
return res;
}


vector<int> generarLabels(vector<int>& labels, vector<bool>& particion){
	vector<int> res;
	for(int i = 0; i < labels.size(); i++){
		if(particion[i]) res.push_back(labels[i]);
	}
return res;
}

int main (int argc, char **argv){
	
	//Parseando argumentos
	if (argv[1] == NULL || argv[2] == NULL ){
		cout << "Error: Deben ingresarse dos archivos";
		return 1;
	}
	char* archEntrada = argv[1]; //Esto ni hace falta, solamente se me hace más lindo
	char* archSalida  = argv[2];
	char modo;
	char* par = argv[3];
	while(*par == ' ') par++;
	//En realidad podria dejarlo simplemente como un char y comparar con '0' en vez de con 0, no sé qué prefieren xD
	if(*par == '0') {
		modo = 0;
	}
	else if (*par == '1') {
		modo = 1;
	}
	else if (*par == '2') {
		modo = 2;
	}
	else if (*par == '3') {
		modo = 3;
	}
	else { 
		cout << "El modo ingresado es inválido" << endl;
		return 1;
	}
	//Leyendo el archivo .in
	std::ifstream entrada;
	entrada.open(archEntrada, std::ifstream::in);
	if(!entrada) {
		cout << "Hubo un error leyendo el archivo" << endl;
		return 1;
	}
	string baseDeDatos;
	const string nombreDB("train.csv");
	int k;
	int alpha;
	int gamma;
	int K;
	
	entrada >> baseDeDatos >> k >> alpha >> gamma >> K;
	
	vector<vector < bool > > particiones(K);
		for(int i = 0; i < K; ++i) {	
		for (int j = 0; j < MUESTRAS; ++j) {
			bool valor;
			entrada >> valor;
			particiones[i].push_back(valor);
			
		}
	}
	
	entrada.close();
	
	//Leyendo base de datos 
	
	vector<vector <unsigned char > > imagenes(MUESTRAS);
	vector<int > labels(MUESTRAS);
	
	std::ifstream db;
	db.open((baseDeDatos+nombreDB).c_str(),std::ifstream::in);
	//cout << (baseDeDatos+nombreDB) << endl;

	
	if(!db) {
		cout << "No se pudo leer de la base de datos"<< endl;
		return 1;
	}
	
	db.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); //La primera línea no me sirve
	
	
	for(int i = 0; i < MUESTRAS; ++i ){
		int numero;
		db >> numero;
		labels[i] = numero;
		while(db.peek() == ',') {
			db.ignore(1,',');
			db >> numero;
			imagenes[i].push_back((unsigned char)numero);
		}
	}
	db.close();
	
	vector< vector<double> > autovaloresPCA(K);
	vector< vector<double> > autovaloresPLS(K); 
	
	vector<int> indices(MUESTRAS);
	for(int i = 0 ; i< MUESTRAS ; ++i) indices[i]= i;
	
	//shuffle(indices) por las dudas, para cambiar el orden de las imágenes y sus labels correspondientes
	std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);
    
    vector<vector <unsigned char > > imagenesCOPIA(MUESTRAS);
	vector<int > labelsCOPIA(MUESTRAS);
	
	for(int i = 0 ; i < MUESTRAS ; ++i){
			imagenesCOPIA[i] =  imagenes[indices[i]];
			labelsCOPIA[i] =  labels[indices[i]];
	}
	imagenes = imagenesCOPIA;   
	labels = labelsCOPIA; 		//AHORA imagenes y labels tienen el mismo shuffle aplicado
		

		
	vector<int> porcentajes = {75};
	vector<vector<vector<bool> > > particionesG = KCrossValidation(porcentajes, MUESTRAS);
	
	modo =1;	
	
	if(modo == 1){
		
		vector<double> promHR(porcentajes.size());
		vector<double> promF1S(porcentajes.size());
		
		TIMER_START(PCA_TOTAL)
		
		for(int p = 0 ; p < porcentajes.size() ; ++p){
				
		cout<< "---------------------------- p = " << "90%" << " -----------------------------------"<< endl;		
		
		
	 	for(int i = 0; i < K; i++){
			
	 		vector<vector<unsigned char> > train = generarTrain(imagenes, particiones[i]);
	   
	  		vector<vector<unsigned char> > testing = generarTest(imagenes, particiones[i]);
	  		
	  		vector<unsigned int > indicesDeTest = generarIndicesDeTest(imagenes, particiones[i]);
	  
	   		vector<int> labels2 = generarLabels(labels, particiones[i]);
			
			vector<int> resultados;
			
	   		cout<<"PCA + k-NN. Trabajando... particion: "<< (i+1) << " de "<< K <<endl;
	   		
	   		PCA estructura(train,alpha);
	   			
	   		for(int j = 0; j < testing.size(); j++){
	  
		        vector<unsigned char>& imagen = testing[j];  
				resultados.push_back(KnnD(labels2, estructura.mTransformada(), estructura.transformarDigito(imagen), k));
		       
	        }
	        
	        promHR[p]+= hitRate(resultados, indicesDeTest, labels); //acumulo el hitRate de cada pasada para un k  y alpha fijo
			promF1S[p]+= f1Score(resultados, indicesDeTest, labels); //acumulo el F1-Score de cada pasada para un k  y alpha fijo.
		
		}
		
		promHR[p] = (double)(promHR[p]/K);  //deja el promedio en la posición p para el p porcentajes[p]	
		promF1S[p] = (double)(promF1S[p]/K);
		
		cout<< "Promedio de Hit Rate de porcentaje =  "<< porcentajes[p] << "%: "<<  promHR[p] << endl; 
		cout<< "Promedio de F1-Score de porcentaje =  "<< porcentajes[p] << "%: "<<  promF1S[p] << endl; 			
		}
		
		
		TIMER_STOP(PCA_TOTAL)
		
		modo = 2;
	
	}

	if(modo == 2){
		
		vector<double> promHR(porcentajes.size());
		vector<double> promF1S(porcentajes.size());
		
		TIMER_START(PLS_TOTAL)
		
		for(int p = 0 ; p < porcentajes.size() ; ++p){
				
		cout<< "---------------------------- p = " << porcentajes[p] << " -----------------------------------"<< endl;		
		particiones = particionesG[p];
		K = particiones.size();
			
		for(int i = 0; i < K; i++){

	 		vector<vector<unsigned char> > train = generarTrain(imagenes, particiones[i]);
	   
	  		vector<vector<unsigned char> > testing = generarTest(imagenes, particiones[i]);
	  		
	  		vector<unsigned int > indicesDeTest = generarIndicesDeTest(imagenes, particiones[i]);
	  
	   		vector<int> labels2 = generarLabels(labels, particiones[i]);

	   		vector<int> resultados;
	   		

			cout << "PLS-DA + k-NN. Trabajando... particion: "<< (i+1) << " de "<< K <<endl;
		
			
			vector < vector<double> > cambioDeBase;
			vector < vector < double > > dbTransformada = tCaracteristica(train, labels2, gamma, cambioDeBase, autovaloresPLS[i]);
			const vector<double> Media = media(train);
			int n = testing.size();
			
	   		for(int j = 0; j < testing.size(); j++){
	  
		        vector<unsigned char>& imagen = testing[j];  	
				resultados.push_back(KnnD(labels2, dbTransformada, plsTransformarDigito(imagen, cambioDeBase, Media, n), k));
		        
	        }
	        
	        promHR[p]+= hitRate(resultados, indicesDeTest, labels); //acumulo el hitRate de cada pasada para un p fijo
			promF1S[p]+= f1Score(resultados, indicesDeTest, labels); //acumulo el F1-Score de cada pasada para un k  y gamma fijo.
		}
		
		promHR[p] = (double)(promHR[p]/K);  //deja el promedio en la posición p para el p porcentajes[p]	
		promF1S[p] = (double)(promF1S[p]/K);
		
		cout<< "Promedio de Hit Rate de porcentaje =  "<< porcentajes[p] << "%: "<<  promHR[p] << endl; 
		cout<< "Promedio de F1-Score de porcentaje =  "<< porcentajes[p] << "%: "<<  promF1S[p] << endl; 			
				
		}
		
		
		TIMER_STOP(PLS_TOTAL)

	}
	

	return 0;
}







