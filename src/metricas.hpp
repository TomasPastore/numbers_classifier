#ifndef METRICAS_HEADER
#define METRICAS_HEADER

#include <iostream>
#include <fstream>
#include <math.h> 
#include <vector>
#include <algorithm>
#define TESTMATRIZ
#include "macrostest.hpp"

#ifdef IMPRIMIR_METRICAS
#define PRINT_M	imprimirMetricas(resultados, indicesDeTest, labels, i);  
#else
#define PRINT_M
#endif

#ifdef IMPRIMIR_HR
#define PRINT_HIT_RATE	imprimirHR(resultados, indicesDeTest, labels, i);  
#else
#define PRINT_HIT_RATE
#endif


using std::cout;
using std::endl;
using std::cin;
using std::vector;

double hitRate(const vector<int>& resultados, const vector<unsigned int>& indicesDeTest,const vector<int>& labels){  
	unsigned int N = resultados.size();
	double sum = 0;
	
	for( int i = 0 ; i < N ; ++i){
		if(resultados[i]==labels[(indicesDeTest[i])]) sum++;
	}
return (sum/N);

}

std::vector< std::vector <unsigned int> > confusionM(const vector<int>& resultados, const vector<unsigned int>& indicesDeTest,const vector<int>& labels){  
	unsigned int N = resultados.size();
	std::vector< std::vector<unsigned int> > M = { 	{0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0} ,
													{0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0} ,
											{0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0} , 
							  				{0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0} ,
							  				{0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0} ,
							  				{0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0} ,
							  				{0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0} ,
							  				{0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0} ,
							  				{0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0} ,
							  				{0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0} , 	};

	

	for( unsigned int i = 0 ; i < N ; ++i){
		++(M[labels[(indicesDeTest[i])]][resultados[i]]);
	}

	GUARDAR(M,"confusion.txt")

	return M;

}

double precisionXClase(const vector<int>& resultados, const vector<unsigned int>& indicesDeTest,const vector<int>& labels, int clase){  //indica la proporción de los que son REALMENTE positivos dentro de los que dieron positivos para una clase
	unsigned int N = resultados.size();
	
	while(clase>9){
	cout<< "Ingrese una clase válida: ";
	cin>> clase ;
	cout<<endl;
	} 
	
	int truePositive = 0 ;
	int falsePositive = 0 ;
	for(int i = 0; i<N; ++i){
		
		if(resultados[i]==clase){
			if(resultados[i]==labels[(indicesDeTest[i])]){
				truePositive++;
			}else {
				falsePositive++;
			}
		}	
	}
return ((double)truePositive/(truePositive+falsePositive));	
}

double precisionDelClasificador(const vector<int>& resultados, const vector<unsigned int>& indicesDeTest,const vector<int>& labels){ //Promedio de las precisiones de clases
	double sum = 0;
	for(int i =0 ; i<10; i++){
		sum+= precisionXClase(resultados, indicesDeTest, labels, i);
	}
return (sum/10);	
}

double recallXClase(const vector<int>& resultados, const vector<unsigned int>& indicesDeTest,const vector<int>& labels, int clase){  //Es una medida de qué tan bueno es un clasificador para, dada una clase particular,identificar correctamente a los pertenecientes a esa clase
	unsigned int N = resultados.size();
	while(clase>9){
	cout<< "Ingrese una clase válida: ";
	cin>> clase ;
	cout<<endl;
	} 
	
	int truePositive = 0 ;
	int falseNegative = 0 ;
	for(int i = 0; i<N; ++i){
		
		if(labels[(indicesDeTest[i])]==clase){
			if(resultados[i]==clase){
				truePositive++;
			}else {
				falseNegative++;
			}
		}	
	}
		
return ((double)truePositive/(truePositive+falseNegative));	
}

double recallDelClasificador(const vector<int>& resultados, const vector<unsigned int>& indicesDeTest, const vector<int>& labels){ //Promedio de los recall de clases
	double sum = 0;
	for(int i =0 ; i<10; i++){
		sum+= recallXClase(resultados, indicesDeTest, labels, i);
	}
return (sum/10);	
}

double f1Score(const vector<int>& resultados, const vector<unsigned int>& indicesDeTest, const  vector<int>& labels){ //relación entre recall y precisión del clasificador
	double precision = precisionDelClasificador(resultados, indicesDeTest, labels);
	double recall = recallDelClasificador(resultados, indicesDeTest, labels);
return ( (2*precision*recall)/ (precision+recall) );	
}

double kDeCohen(const vector<int>& resultadosCA,const vector<int>& resultadosCB, const vector<unsigned int>& indicesDeTest){ //Considera dos clasificadores, para un mismo test
	
	unsigned int N = resultadosCA.size();
	double sum = 0;
	
	for( int i = 0 ; i < N ; ++i){
		if(resultadosCA[i]==resultadosCB[i]) sum++;
	}
	double pa = 1/100;  //suponiendo independencia entre clasificadores (1/10)*(1/10)
	double po = sum/N;  //proporción de coincidencia observada entre clasificadores
	
return ((po-pa)/ (1-pa) );	
}

void imprimirMetricas(const vector<int>& resultados, const vector<unsigned int>& indicesDeTest, const vector<int>& labels, int i){

	double hR = hitRate(resultados, indicesDeTest, labels);
	cout<< "Hit Rate para K = "<< (i+1) << ": " << hR << endl << endl;
	        
	for(int j = 0; j<10 ; ++j){
		cout<< "Precision para clase "<< (j) << ": " << precisionXClase(resultados, indicesDeTest, labels, j)<< endl;
	}
	
	cout<< endl << "Precision del Clasificador: "<< precisionDelClasificador(resultados, indicesDeTest, labels)<< endl;
			
	for(int j = 0; j<10 ; ++j){
		cout<< "Recall clase "<< (j) << ": " << recallXClase(resultados, indicesDeTest, labels, j)<< endl;
	}
			
	cout<< endl <<"Recall del Clasificador: "<< recallDelClasificador(resultados, indicesDeTest, labels)<< endl<< endl;
			
	cout<< "F1 Score: "<< f1Score(resultados, indicesDeTest, labels)<< endl<< endl;			
}

void imprimirHR(const vector<int>& resultados, const vector<unsigned int>& indicesDeTest, vector<int>& labels, int i){

	double hR = hitRate(resultados, indicesDeTest, labels);
	cout<< "Hit Rate para K = "<< (i+1) << ": " << hR << endl << endl;
	        		
}

void guardarTasaDeRec(const vector< vector<int> >& resultados, const vector< vector<unsigned int> >& indicesDeTest, vector<int>& labels){
		
		std::ofstream salida;
		salida.open("Tasa de reconocimiento.txt", std::ios::trunc); //Abro el archivo de salida
	
		
		for(int i = 0 ; i < resultados.size() ; ++i){
			
				salida << " -----------------------------K = "<<(i+1)<<" -------------------------------------------------------" << endl;
				
				salida<< "Hit Rate: "<< hitRate(resultados[i], indicesDeTest[i], labels) << endl;
	        
				salida<< "Precision del Clasificador: "<< precisionDelClasificador(resultados[i], indicesDeTest[i], labels)<< endl;
			
				salida<<"Recall del Clasificador: "<< recallDelClasificador(resultados[i], indicesDeTest[i], labels)<< endl;
			
				salida<< "F1 Score: "<< f1Score(resultados[i], indicesDeTest[i], labels)<< endl;					
		
		}		
		
		salida << " ------------------------------------------------------------------------------------------" << endl;
		
	salida.close();
		
	}


#endif
