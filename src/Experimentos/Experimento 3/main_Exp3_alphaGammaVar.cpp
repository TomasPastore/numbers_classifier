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
	
	vector< vector<double> > autovaloresPCA(alpha);
	vector< vector<double> > autovaloresPLS(gamma); //creo las variables globales
	
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
	
	//Esto las lee como vectores, entendi que las ibamos a usar así, aunque si no, solamente hay que cambiar el loop interno.
	//Tampoco sé si está bueno poner los números directamente, pero por ahora lo dejo así.
	
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
	
	vector<vector<int> >resultadosG;
	vector<vector<unsigned int> >indicesDeTestG;
		
	
   	if(modo == 0){
		
		TIMER_START(KNN_TOTAL)	 
		
	 	for(int i = 0; i < K; i++){

	 		vector<vector<unsigned char> > train = generarTrain(imagenes, particiones[i]);
	   
	  		vector<vector<unsigned char> > testing = generarTest(imagenes, particiones[i]);
	   
			vector<unsigned int > indicesDeTest = generarIndicesDeTest(imagenes, particiones[i]);
	   		
	   		vector<int> labels2 = generarLabels(labels, particiones[i]);

			vector<int> resultados;
			
			cout << "k-NN. Trabajando..." << endl;
			
			TIMER_START(KNN_PARCIAL)	  
			//vector<vector<double> > promDeClases = promedioDeClases(labels2, train);  //solo hace falta calcularlo una vez(solo si se usa el método clasificar)
			
	   		for(int j = 0; j < testing.size(); j++){
	  
		        vector<unsigned char>& imagen = testing[j];  	
				resultados.push_back(Knn(labels2, train,  imagen, 15));
		        //resultados.push_back(clasificar(promDeClases, imagen));  
		       	     
	        }
	        
	        TIMER_STOP(KNN_PARCIAL)	        
	        PRINT_M   //definido arriba
	        indicesDeTestG.push_back(indicesDeTest);
	        resultadosG.push_back(resultados);
	        
		}
	 
	TIMER_STOP(KNN_TOTAL)
	guardarTasaDeRec(resultadosG, indicesDeTestG, labels);	   
	
	}


	if(modo == 1){
		
		TIMER_START(PCA_TOTAL)
		std::vector<int> alphas = {1, 2, 5, 10, 20, 25, 30, 40, 50, 100, 250};
		std::vector<double> promHR(alphas.size());
	
		for(int a = 0 ; a < alphas.size() ; ++a){
				
		cout<< "---------------------------- alpha = " << alphas[a] << " -----------------------------------"<< endl;		
		
	 	for(int i = 0; i < K; i++){
			
	 		vector<vector<unsigned char> > train = generarTrain(imagenes, particiones[i]);
	   
	  		vector<vector<unsigned char> > testing = generarTest(imagenes, particiones[i]);
	  		
	  		vector<unsigned int > indicesDeTest = generarIndicesDeTest(imagenes, particiones[i]);
	  
	   		vector<int> labels2 = generarLabels(labels, particiones[i]);
			
			vector<int> resultados;
			
	   		cout<<"PCA + k-NN. Trabajando... iteracion: "<< (i+1) << " de "<< particiones.size() <<endl;
	   		
	   		TIMER_START(PCA_PARCIAL)
	   		PCA estructura(train,alphas[a]);
	   			
	   		for(int j = 0; j < testing.size(); j++){
	  
		        vector<unsigned char>& imagen = testing[j];  
				resultados.push_back(KnnD(labels2, estructura.mTransformada(), estructura.transformarDigito(imagen), k));
		       
	        }
	        
			promHR[a]+= hitRate(resultados, indicesDeTest, labels); //acumulo el hitRate de cada pasada para un alpha fijo
			
			/*cout<<"Espere..."<< endl;
			indicesDeTestG.push_back(indicesDeTest);
	        resultadosG.push_back(resultados);
			pls(train, labels2, gamma, autovaloresPLS[i]);	//modifico el vector de autovalores de PLS para la partición i
			autovaloresPCA[i] = estructura.autovalores();
	        
			*/
		}
		
		promHR[a] = (double)(promHR[a]/K);  //deja el promedio en la posición a para el a alphas[a]
		cout<< "PROMEDIO HR DE alpha =  "<< alphas[a] << ": "<<  promHR[a] << endl; 
				
		}
		
		unsigned int alphaOptimo = alphas[maxPos(promHR)];   //devuelvo el alpha tal que el hitrate promedio es máximo
		cout<< "alphaOptimo DEL EXPERIMENTO(con peso): "<< alphaOptimo << endl;
		TIMER_STOP(PCA_TOTAL)
		//guardarTasaDeRec(resultadosG, indicesDeTestG, labels);	
	}

	if(modo == 2){
		
		TIMER_START(PLS_TOTAL)
		std::vector<int> gammas = {1, 2, 5, 10, 20, 25, 30, 40, 50, 100, 250};
		std::vector<double> promHR(gammas.size());
	
		for(int a = 0 ; a < gammas.size() ; ++a){
				
		cout<< "---------------------------- gammas  = " << gammas[a] << " -----------------------------------"<< endl;		
		
		for(int i = 0; i < K; i++){

	 		vector<vector<unsigned char> > train = generarTrain(imagenes, particiones[i]);
	   
	  		vector<vector<unsigned char> > testing = generarTest(imagenes, particiones[i]);
	  		
	  		vector<unsigned int > indicesDeTest = generarIndicesDeTest(imagenes, particiones[i]);
	  
	   		vector<int> labels2 = generarLabels(labels, particiones[i]);

	   		vector<int> resultados;
	   		

			cout<<"PLS-DA + k-NN. Trabajando... iteracion: "<< (i+1) << " de "<< K <<endl;
		
			
			vector < vector<double> > cambioDeBase;
			vector < vector < double > > dbTransformada = tCaracteristica(train, labels2, gammas[a], cambioDeBase, autovaloresPLS[i]);
			const vector<double> Media = media(train);
			int n = testing.size();
			
	   		for(int j = 0; j < testing.size(); j++){
	  
		        vector<unsigned char>& imagen = testing[j];  	
				resultados.push_back(KnnD(labels2, dbTransformada, plsTransformarDigito(imagen, cambioDeBase, Media, n), k));
		        
	        }
	        
	        promHR[a]+= hitRate(resultados, indicesDeTest, labels); //acumulo el hitRate de cada pasada para un alpha fijo
	       
			
			/*cout<<"Espere..."<< endl;
			indicesDeTestG.push_back(indicesDeTest);
	        resultadosG.push_back(resultados);
			pls(train, labels2, gamma, autovaloresPLS[i]);	//modifico el vector de autovalores de PLS para la partición i
			autovaloresPCA[i] = estructura.autovalores();
	        
			*/
		}
		
		promHR[a] = (double)(promHR[a]/K);  //deja el promedio en la posición a para el a alphas[a]
		cout<< "PROMEDIO HR DE gammas =  "<< gammas[a] << ": "<<  promHR[a] << endl; 
				
		}
		
		unsigned int gammaOptimo = gammas[maxPos(promHR)];   //devuelvo el alpha tal que el hitrate promedio es máximo
		cout<< "gammaOptimo DEL EXPERIMENTO(con peso): "<< gammaOptimo << endl;
		TIMER_STOP(PLS_TOTAL)
		//guardarTasaDeRec(resultadosG, indicesDeTestG, labels);
		
	}
	


	if(modo == 3){

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

	}
	
	/*   Escribo la salida
	
	std::ofstream salidaAutovalores;
	salidaAutovalores.open(archSalida, std::ios::trunc); //Abro el archivo de salida
	
	for(int i = 0 ; i < autovaloresPCA.size(); i++){
		
		for(int j = 0; j < autovaloresPCA[i].size(); j++){
			
			salidaAutovalores<< autovaloresPCA[i][j] << endl;
			
		}
	
	}
	
	for(int i = 0 ; i < autovaloresPLS.size(); i++){
		
		for(int j = 0; j < autovaloresPLS[i].size(); j++){
			
			salidaAutovalores<< autovaloresPLS[i][j] << endl;
			
		}
	
	}
	
	
	salidaAutovalores.close();

*/

	return 0;
}






