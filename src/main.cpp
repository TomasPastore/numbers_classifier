#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <cassert>
#include "time.h"
#include <utility>  
#include <math.h> 
#include <algorithm>
#include "utils.hpp"
#include "knn.hpp"
#include "PCA.hpp"
#include "PLS.hpp"



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
	
	//*********************************** Parseando argumentos ***********************************************
	
	if (argv[1] == NULL || argv[2] == NULL ){
		cout << "Error: Deben ingresarse dos archivos";
		return 1;
	}
	char* archEntrada = argv[1]; //Esto ni hace falta, solamente se me hace más lindo
	char* archSalida  = argv[2];
	char modo;
	char* par = argv[3];
	while(*par == ' ') par++;
	
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
	
	//********************************* Leyendo el archivo .in **********************************************
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
	
	
	// **************************Variables globales para generar los archivos de salida***********************
	
	vector< vector<double> > autovaloresPCA(K);
	vector< vector<double> > autovaloresPLS(K); 
	vector<vector<int> >resultadosG;
	vector<vector<unsigned int> >indicesDeTestG;
	
	//********************************************************************************************************
	
	
	//*************************** Leyendo base de datos *******************************************************
	
	vector<vector <unsigned char > > imagenes(MUESTRAS);
	vector<int > labels(MUESTRAS);
	
	std::ifstream db;
	db.open((baseDeDatos+nombreDB).c_str(),std::ifstream::in);
	
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
	
	//**********************************************Seleccion de modo********************************************************	
	
   	switch (modo){
		
		case 0: {
		
			TIMER_START(KNN_TOTAL)	 
			
			for(int i = 0; i < K; i++){

				vector<vector<unsigned char> > train = generarTrain(imagenes, particiones[i]);
	   
				vector<vector<unsigned char> > testing = generarTest(imagenes, particiones[i]);
	   
				vector<unsigned int > indicesDeTest = generarIndicesDeTest(imagenes, particiones[i]);
	   		
				vector<int> labels2 = generarLabels(labels, particiones[i]);

				vector<int> resultados;
			
				cout << "k-NN. Iteración "<< (i+1) << " de "<< K << endl;
				TIMER_START(KNN_PARCIAL)	  
				//vector<vector<double> > promDeClases = promedioDeClases(labels2, train);  //solo hace falta calcularlo una vez(solo si se usa el método clasificar)
			
				for(int j = 0; j < testing.size(); j++){
	  
					vector<unsigned char>& imagen = testing[j];  	
					resultados.push_back(Knn(labels2, train,  imagen, 15));
					//resultados.push_back(clasificar(promDeClases, imagen));  
		       	     
				}
	        
				TIMER_STOP(KNN_PARCIAL)	        
				PRINT_M   //definido en metricas.hpp
				indicesDeTestG.push_back(indicesDeTest);
				resultadosG.push_back(resultados);
	        
			}
	 
			TIMER_STOP(KNN_TOTAL)   
			break;
	
		
		}
		case 1: {
		
			TIMER_START(PCA_TOTAL)

			for(int i = 0; i < K; i++){
			
				vector<vector<unsigned char> > train = generarTrain(imagenes, particiones[i]);
	   
				vector<vector<unsigned char> > testing = generarTest(imagenes, particiones[i]);
	  		
				vector<unsigned int > indicesDeTest = generarIndicesDeTest(imagenes, particiones[i]);
	  
				vector<int> labels2 = generarLabels(labels, particiones[i]);
			
				vector<int> resultados;
			
				cout<<"PCA + k-NN. Iteración "<< (i+1) << " de "<< K << endl;
				
				PCA estructura(train,alpha);
	   			
				for(int j = 0; j < testing.size(); j++){
	  
					vector<unsigned char>& imagen = testing[j];  
					resultados.push_back(KnnD(labels2, estructura.mTransformada(), estructura.transformarDigito(imagen), k));
		       
				}
			
				indicesDeTestG.push_back(indicesDeTest);
				resultadosG.push_back(resultados);
				cout<<"Calculando autovalores... "<< endl;
				autovaloresPCA[i] = estructura.autovalores();   //guardo los autovalores de la particion i ya calculados
				pls(train, labels2, gamma, autovaloresPLS[i]);	//modifico el vector de autovalores de PLS para la partición i
				
			}
		
			TIMER_STOP(PCA_TOTAL)	
			break;

		}
		
		case 2: {
		
			TIMER_START(PLS_TOTAL)
				
			for(int i = 0; i < K; i++){

				vector<vector<unsigned char> > train = generarTrain(imagenes, particiones[i]);
	   
				vector<vector<unsigned char> > testing = generarTest(imagenes, particiones[i]);
	  		
				vector<unsigned int > indicesDeTest = generarIndicesDeTest(imagenes, particiones[i]);
	  
				vector<int> labels2 = generarLabels(labels, particiones[i]);

				vector<int> resultados;
	   		

				cout << "PLS-DA + k-NN. Iteración "<< (i+1) << " de "<< K << endl;
		
				vector < vector<double> > cambioDeBase;
				vector < vector < double > > dbTransformada = tCaracteristica(train, labels2, gamma, cambioDeBase, autovaloresPLS[i]);
				const vector<double> Media = media(train);
				int n = testing.size();
			
				for(int j = 0; j < testing.size(); j++){
		
		        vector<unsigned char>& imagen = testing[j];  	
				resultados.push_back(KnnD(labels2, dbTransformada, plsTransformarDigito(imagen, cambioDeBase, Media, n), k));
		        
				}
	        
				
				indicesDeTestG.push_back(indicesDeTest);
				resultadosG.push_back(resultados);
				cout<<"Calculando autovalores... "<< endl;
				PCA estructura(train,alpha);
				autovaloresPCA[i] = estructura.autovalores();
	        
			}
		
			TIMER_STOP(PLS_TOTAL)
			break;
		
		}
		
		case 3:   {  // Kaggle
	
			vector<vector<unsigned char> > imgTest(28000);
			std::ifstream test;
			test.open((baseDeDatos+"test.csv").c_str(),std::ifstream::in);

			if(!test){
				cout << "No se pudo leer la base de test" << endl;
			}

			test.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); //La primera línea no me sirve

			for(int i = 0; i < 28000; ++i ){
				int numero;
				test >> numero;
				imgTest[i].push_back((unsigned char)numero);
				while(test.peek() == ',') {
					test.ignore(1,',');
					test >> numero;
					imgTest[i].push_back((unsigned char)numero);
				}

				if(imgTest[i].size() != 28*28) cout << imgTest[i].size() << " " << i << endl;
			}

			test.close();
			
			std::ofstream testSalida;
			testSalida.open(string(archSalida)+".csv",std::ofstream::trunc);

			testSalida << "ImageId,Label" << endl;
			cout << "ImageId,Label" << endl;
			//cout << "Llego a PCA" << endl;
			PCA estructura(imagenes,50);
			//cout << "Proceso la estructura ya";
			for(unsigned int i = 0; i < imgTest.size(); ++i) {
				vector<unsigned char>& imagen = imgTest[i];
				cout << i << ',' << KnnD(labels, estructura.mTransformada(), estructura.transformarDigito(imagen), 5) << endl;
				testSalida << i+1 << ',' << KnnD(labels, estructura.mTransformada(), estructura.transformarDigito(imagen), 5) << endl;
			}

			testSalida.close();
			break;
			
		}	
		default: {
			cout<< "Modo inválido"<< endl;
		}
	}
	
	//******************************** GUARDAMOS LA TASA DE RECONOCIMIENTO ****************************************************
	
	guardarTasaDeRec(resultadosG, indicesDeTestG, labels);  
	
	//*************************************** SALIDA DE AUTOVALORES ***********************************************************
	
	std::ofstream salidaAutovalores;
	salidaAutovalores.open(string(archSalida), std::ios::trunc); //Abro el archivo de salida
	assert(K == autovaloresPCA.size());
	assert(K == autovaloresPLS.size());
	
	for(int i = 0 ; i < K; i++){

		for(int j = 0; j < autovaloresPCA[i].size(); j++) salidaAutovalores<< std::scientific<< autovaloresPCA[i][j] << endl;
		
		for(int j = 0; j < autovaloresPLS[i].size(); j++) salidaAutovalores<< std::scientific<< autovaloresPLS[i][j] << endl;
	
	}
	
	salidaAutovalores.close();

	return 0;
}







