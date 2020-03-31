#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <utility>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include "time.h"
#include "operadores.hpp"

using std::vector;
 

extern double norma2(const vector<double>& vec);
	
std::pair< vector<double>,double > mPotencia2(const vector<vector<double> >& A ) {

	std::srand(std::time(0));
	vector<double> v(A[0].size());
	double autovalor;

	for(int i = 0; i < (A[0]).size(); ++i) {
		v[i] = std::rand();
	}

	for(int i = 0; i < 2000; ++i ) {

		v = A*v;
		v /= sqrt(v*v);

	}
	autovalor = v*(A*v)/sqrt(v*v);
return std::make_pair(v,autovalor);

}
vector<double> media(const vector< vector<unsigned char> >& matrix){
	int n = matrix.size();   
	int cols = matrix[0].size();          
	         
	vector<double> vMedia(cols);
	
    for(int i = 0; i < n; ++i) {
			vMedia += matrix[i];
	}
	vMedia/= n;
	
return vMedia;

}

vector<double> media(const vector< vector<int> >& matrix){
	int n = matrix.size();   
	int cols = matrix[0].size();          
	         
	vector<double> vMedia(cols);
	
    for(int i = 0; i < n; ++i) {
			vMedia += matrix[i];
	}
	vMedia/= n;
	
return vMedia;

}

 
vector< vector<double> > matrixMediaC(const vector< vector<unsigned char> > & matrix){
	
	int n = matrix.size();   
	
	vector< double > vMedia = media(matrix);

    vector< vector< double > > matrix2(n);//matriz resultado
      
    for (int i = 0; i < n; ++i) {
		matrix2[i] = matrix[i] - vMedia;
		matrix2[i] /= sqrt(n-1);	
	}

return matrix2;
} 

vector< vector< double > > matrixMedia(const vector< vector<int> > & matrix){
	
	int n = matrix.size();         
	         
	vector< double > vMedia = media(matrix);

    vector< vector< double > > matrix2(n);//matriz resultado
      
    for (int i = 0; i < n; ++i) {
		matrix2[i] = matrix[i] - vMedia;
		matrix2[i] /= sqrt(n-1);	
	}

return matrix2;
} 

vector<vector<double> > generarY(const vector<int> & labels){
	
	vector<vector<int> > prey(labels.size());

	for(int i = 0; i < labels.size(); i++){
	 	for(int j = 0; j < 10; j++){
	 		prey[i].push_back( labels[i] == j ? 1 : -1);
	 	}
	 }
    
     vector< vector< double > > res = matrixMedia(prey); 
     
return res;
}

vector < vector < double > > transpuesta(const vector < vector < double > >& matrix){
	vector < vector < double > > res(matrix[0].size());
	
	for(int i = 0; i < matrix.size(); i++){
		for(int j = 0; j < matrix[0].size(); j++){
           res[j].push_back(matrix[i][j]);
		}
	}
	
return res;
}

vector < vector < double > > producto2(const vector< vector< double > >& X, const vector< vector< double > >& Y){
assert((X[0]).size() == Y.size());
	
	vector < vector < double > > res(X.size());
	vector < vector <double > > Yt = transpuesta(Y);
	
	for(unsigned int i = 0; i < X.size(); ++i) {
		for(unsigned int j = 0; j < Yt.size(); ++j){
			
			res[i].push_back(X[i]*Yt[j]);
		}		
	}

return res;
}

vector < vector<double> > pls(vector< vector< unsigned char > > & x, vector< int > & labels,  int gamma, vector<double>& autovalores){
    vector < vector < double > > Y = generarY(labels);
    vector < vector < double > > X = matrixMediaC(x);
	vector < vector < double > > res;
	autovalores.resize(gamma);
		
	for(int i = 0; i < gamma; i++){

		//calculo Mi  Xt*Y*Yt*X = (Xt*Y)*(Xt*Y)t = (Yt*X)t * (Yt*X)
	  		
		vector < vector <double > > YtX = producto2(transpuesta(Y), X);
		vector < vector <double > > M = producto2(transpuesta(YtX), YtX); 
		
		std::pair< vector<double>,double > autovecs2 = mPotencia2(M);  //calculo wi, mPotencia ya lo trae normalizado
		
		autovalores[i] = autovecs2.second;
		vector<double> wi = autovecs2.first;
		vector<double> ti = X*wi;					    //calculo ti
		
		double normati = norma2(ti); 		
		ti/=normati;									//normalizo ti
		 
		 
		vector < vector <double> > tiT;
		tiT.push_back(ti);
		vector < vector <double > > tiTX = producto2(tiT,X);  // vive en 1*m
		vector < vector <double > > tiTY = producto2(tiT,Y);  // vive en 1*10
		vector < vector <double > > tiM = transpuesta(tiT);   //vive en n*1
		
        
        vector<vector<double> > titiTX = producto2(tiM,tiTX);  //vive en n*m
        vector<vector<double> > titiTY = producto2(tiM,tiTY);  //vive en n*10
        
        X-= titiTX;
        Y-= titiTY;
                
        res.push_back(wi); 
		
     }
     
return res;

}   

vector< vector<double> > tCaracteristica(vector< vector< unsigned char > >& x, vector< int >& labels,  int gamma, vector<vector<double> >& cambioDeBase, vector<double>& autovalores){ //devuelvo X.AutovectoresTranspuestam, y modifica 2 parámatros para devolver los autovectores en la matriz cambioDeBase y los autovalores
	
	vector < vector < double > > X = matrixMediaC(x);
	
	cambioDeBase = pls(x, labels, gamma, autovalores);
	
return producto2(X,transpuesta(cambioDeBase));	
}	

vector<double> plsTransformarDigito(const std::vector<unsigned char>& digito, const vector <vector<double> >& cambioDeBase, const vector<double>& Media, int n){

			std::vector< double > digit(digito.size());

			for(int i = 0; i < digito.size(); ++i) {
				digit[i] = (double)digito[i];
			}
			
			digit -= Media;
			digit /= sqrt(n-1);

return cambioDeBase*digit;

}
