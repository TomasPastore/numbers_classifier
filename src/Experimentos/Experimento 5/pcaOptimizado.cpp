#include <vector>
#include <algorithm>
#include <cassert>
#include <math.h>
#include "operadores.hpp"
#include <utility>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include "PCA.hpp"
//#include "utils.hpp"


using std::vector;
using std::cout;
using std::endl;


const std::vector<std::vector<double> >& PCA::mTransformada() const {
	return this->MTransformada;
}

const std::vector <double>& PCA::autovalores() const{
	return this->Autovalores;
}

void PCA::media(vector < double > & vMedia, const vector<vector<double> > & db) {

	int limite = (500 > db.size() ? db.size(): 500);
	
	for(int i = 0; i < limite; ++i) {
			vMedia += db[i];
	}
	
	for(int i = 0; i < vMedia.size(); ++i) {
		vMedia[i] = vMedia[i]/limite;
	}

}

std::vector<double> PCA::transformarDigito(const std::vector<unsigned char>& digito) const {

			std::vector< double > digit(digito.size());

			for(int i = 0; i < digito.size(); ++i) {
				digit[i] = (double)digito[i];
			}

			digit -= Media;
			digit /= Divisor;

			return Autovectores*digit;

}



std::pair< vector<double>,double > PCA::mPotencia(const vector<vector<double> >& A ) {

	std::srand(std::time(0));
	vector<double> v(A[0].size());
	double autovalor;

	for(int i = 0; i < (A[0]).size(); ++i) {
		v[i] = std::rand();
	}

	for(int i = 0; i < 200; ++i ) {

		v = A*v;
		v /= sqrt(v*v);

	}
	autovalor = v*(A*v)/sqrt(v*v);
	return std::make_pair(v,autovalor);

}

void PCA::calcularAutoVectores(vector < vector < double > >& M) {

	//COUT("Calcular autovectores")

	std::pair<vector<double>, double> potres = mPotencia(M);

	this->Autovectores[0] = potres.first;
	this->Autovalores[0] = potres.second;

	for(int i = 1; i < Autovectores.size(); ++i) {
		vector<vector<double> > matrizResta(potres.first.size());

		for(int h = 0; h < potres.first.size(); ++h) {
			for(int  j = 0; j < potres.first.size(); ++j ){
				matrizResta[h].push_back(potres.first[h]*potres.first[j]);
			}
		} 
		matrizResta *= (-1.0)*potres.second;
	
		M += matrizResta;
		
		potres = mPotencia(M);

		this->Autovectores[i] = potres.first;
		this->Autovalores[i] = potres.second;

	}

}

void PCA::prodTraspuesta(const vector < vector < double > >& X, vector < vector < double > >& M) {

	int limite = (500 > X.size() ? X.size(): 500);
	
	for(int i = 0; i < M.size(); i++) {
			
		M[i].resize(X[0].size());

		for(int j = 0; j < M[i].size(); ++j ) {
			double valor = 0;
			for( int k = 0; k < limite; ++k) {  // optimización k < (500 > X.size() ? X.size(): 500) ver experimento 5 para la justificación de ese 500 en vez de n.
				valor+= X[k][i]*X[k][j];
			}
			M[i][j]=valor;
		}

	}	

}

PCA::PCA(const vector<vector<unsigned char> >& db1, unsigned int alpha) :
	    Autovectores(alpha), 
	    Autovalores(alpha),
		Divisor((500 > db1.size() ? sqrt(db1.size()-1): sqrt(500-1)) ), 
		MTransformada(db1.size()),
		Media(db1.at(0).size())

	{
	
	unsigned int m = db1.at(0).size();
	unsigned int n = db1.size();

	vector<vector<double> > db(db1.size());

	for(int i = 0; i < db.size(); ++i) {
		for(int j = 0; j < db1.at(i).size(); ++j) {
			db[i].push_back((double)db1[i][j]);
		}
	}
	
	media(this->Media, db);

	vector < vector < double > > X(db.size());

	for (int i = 0; i < X.size(); ++i) {
		X[i] = db[i] - this->Media;
		X[i] /= Divisor;	
	}

	vector < vector < double > > M(m);
	
	//COUT("La X es:")
	//COUT(X)

	this->prodTraspuesta(X,M);

	//COUT(M)
	
	//Autovectores...

	this->calcularAutoVectores(M);

	for(int i = 0; i < this->MTransformada.size(); ++i) {
		for(int j = 0; j < alpha; ++j ) {
			this->MTransformada[i].push_back(this->Autovectores[j]*X[i]);
		}
	}

}
