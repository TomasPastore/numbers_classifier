#include "knn.hpp"
#include "operadores.hpp"
#include <queue> 
using std::vector;
using std::pair;
using std::make_pair;

int maxPos(const vector<int>& vec){
	int res = 0;
	for(int i = 1; i < vec.size(); i++){
		if( vec[i]> vec[res]) res = i;
	}
	
return res;
}

int maxPos(const vector<double>& vec){
	int res = 0;
	for(int i = 1; i < vec.size(); i++){
		if( vec[i]> vec[res]) res = i;
	}
	
return res;
}

double norma2(const vector<double>& vec){
	
	double res = 0;

	for(int i = 0; i < vec.size(); i++){
		res += vec[i]*vec[i];
	}

	return sqrt(res);                               //devuelvo la norma
}


unsigned int distanciaAlCuadrado(const vector<unsigned char>& imagen, const vector<unsigned char>& vectorDb){
	
	unsigned int res = 0;

	for(int i = 0; i < imagen.size(); i++){
		res += (unsigned int)pow((int)imagen[i]-(int)vectorDb[i],2);
	}

	return res;                               //devuelvo la distancia al cuadrado, minimizar la distancia es lo mismo que minimizar la distancia al cuadrado porque raiz cuadrada es monotona creciente.
}

double distanciaAlCuadrado(const vector<double>& imagen, const vector<double>& vectorDb){
	
	double res = 0;

	for(int i = 0; i < imagen.size(); i++){
		res += pow(imagen[i]-vectorDb[i],2);
	}

	return res;                               //devuelvo la distancia al cuadrado, minimizar la distancia es lo mismo que minimizar la distancia al cuadrado porque raiz cuadrada es monotona creciente.
}


double normaEuclidea(const vector<unsigned char>& imagen, const vector<unsigned char>& vectorDb){
	
	double res = 0;

	for(int i = 0; i < imagen.size(); i++){
		res += pow((double)imagen[i]-(double)vectorDb[i],2);
	}

	return sqrt(res);                               //devuelvo la norma
}

double normaEuclidea(const vector<unsigned char>& imagen, const vector<double>& vectorDb){
	double res = 0;

	for(int i = 0; i < imagen.size(); i++){
		res += pow((double)imagen[i]-vectorDb[i],2);
	}

	return sqrt(res);                               //devuelvo la norma
}

double normaEuclidea(const vector<double>& imagen, const vector<double>& vectorDb){
	double res = 0;

	for(int i = 0; i < imagen.size(); i++){
		res += pow(imagen[i]-vectorDb[i],2);
	}

	return sqrt(res);                               //devuelvo la norma
}

/* ***************  Primer Knn que hicimos ***************************
 
int Knn(const vector<int>& labels, const vector<vector<unsigned char> >& muestras, const vector< unsigned char >& imagen,  unsigned int k ){
	vector<pair<double, int> > knormas;
	for(int i = 0; i < labels.size(); i++){
		double norma = normaEuclidea(imagen, muestras[i]);
		knormas.push_back(make_pair(norma, labels[i]));
		
	}
	
	std::sort(knormas.begin(), knormas.end(), [](pair<double, int> a, pair<double, int> b ){ return a.first < b.first; });
	
	vector<int> res(k);
	for(int i = 0; i < k; i++){
		res[i] = knormas[i].second;
	}
	vector<int> cantApariciones(10);

	for (int i = 0 ; i<k ; i++){
		cantApariciones[res[i]]++;
	}
   
	int resultado = maxPos(cantApariciones);

return resultado;

}*/

// ********* Optimización con heap *************************
/*
int Knn(const vector<int>& labels, const vector<vector<unsigned char> >& muestras, const vector< unsigned char >& imagen,  unsigned int k ){
	
	
	std::priority_queue<std::pair<double, int> > heap;

	for(int i = 0; i < k; ++i) {
		heap.push(make_pair(normaEuclidea(imagen,muestras[i]),labels[i]));
	}
	for(int i = k; i < labels.size(); ++i) {
		double norma = normaEuclidea(imagen,muestras[i]);
		if(norma < heap.top().first) {
			heap.pop();
			heap.push(make_pair(norma,labels[i]));
		}
	}

	std::vector<int> counter(10,0);
	while(!heap.empty()) {
		++counter[heap.top().second];
		heap.pop();
	}
	return maxPos(counter);
	
}
*/  // Optimización final minimizando la distancia al cuadrado, usando unsigned int y dándole más peso a los más cercanos

int Knn(const vector<int>& labels, const vector<vector<unsigned char> >& muestras, const vector< unsigned char >& imagen,  unsigned int k ){
	
	
	std::priority_queue<std::pair<unsigned int, int> > heap;

	for(int i = 0; i < k; ++i) {
		heap.push(make_pair(distanciaAlCuadrado(imagen,muestras[i]),labels[i]));
	}
	for(int i = k; i < labels.size(); ++i) {
		unsigned int dist = distanciaAlCuadrado(imagen,muestras[i]);
		if(dist < heap.top().first) {
			heap.pop();
			heap.push(make_pair(dist,labels[i]));
		}
	}

	std::vector<double> counter(10,0);
	while(!heap.empty()) {
		counter[heap.top().second]+= (double)(1.0/(heap.top().first));
		heap.pop();
	}
	return maxPos(counter);
	
}
 
 
//*************************************** Knn DOUBLE ******************************************************************
int KnnD(const vector<int>& labels, const vector<vector<double> >& muestras, const vector< double >& imagen,  unsigned int k ){
	
	
	std::priority_queue<std::pair<double, int> > heap;

	for(int i = 0; i < k; ++i) {
		heap.push(make_pair(distanciaAlCuadrado(imagen,muestras[i]),labels[i]));
	}
	for(int i = k; i < labels.size(); ++i) {
		double dist = distanciaAlCuadrado(imagen,muestras[i]);
		if(dist < heap.top().first) {
			heap.pop();
			heap.push(make_pair(dist,labels[i]));
		}
	}

	std::vector<double> counter(10,0);
	while(!heap.empty()) {
		counter[heap.top().second]+= (double)(1.0/(heap.top().first));
		heap.pop();
	}
	return maxPos(counter);
	
}
 


/*
int KnnD(const vector<int>& labels, const vector<vector<double> >& muestras, const vector< double >& imagen,  unsigned int k ){
	vector<pair<double, int> > knormas;
	for(int i = 0; i < labels.size(); i++){
		double norma = normaEuclidea(imagen, muestras[i]);
		knormas.push_back(make_pair(norma, labels[i]));
		
	}
	
	std::sort(knormas.begin(), knormas.end(), [](pair<double, int> a, pair<double, int> b ){ return a.first < b.first; });
	
	vector<int> res(k);
	for(int i = 0; i < k; i++){
		res[i] = knormas[i].second;
	}
	vector<int> cantApariciones(10);

	for (int i = 0 ; i<k ; i++){
		cantApariciones[res[i]]++;
	}
   
	int resultado = maxPos(cantApariciones);

return resultado;

}
*/
//*************************************** Invento de estimador de clase, es muy rápido pero con hitrate 0.81 ******************************************************************

vector<vector<double> > promedioDeClases(const vector<int>& labels, const vector<vector<unsigned char> >& muestras ){
		
	vector<vector<double> > promediosDeClases(10);
	
	for(int i = 0 ; i<10 ; ++i){
		vector<double> vec(muestras[0].size());
		promediosDeClases[i] = vec;
	}
	
	vector<unsigned int > contarClases(10); 
	for(int i = 0; i < muestras.size(); ++i){
		contarClases[labels[i]]++;
		promediosDeClases[labels[i]]+= muestras[i];
	}
	
	for(int i = 0 ; i< 10 ; ++i){
		promediosDeClases[i]/= contarClases[i];
	}
return promediosDeClases;
}

int clasificar(const vector<vector<double> >promediosDeClases, const vector< unsigned char >& imagen){	
	//devuelvo la clase tal que la distancia entre su promedio y la nueva imagen sea mínima
	
	pair<double, int> min = make_pair(normaEuclidea(imagen,promediosDeClases[0]), 0);
	
	for(int i = 1 ; i<10 ; ++i){
			double distancia = normaEuclidea(imagen, promediosDeClases[i]);
			if(distancia < min.first) min = make_pair(distancia, i); 
	}	
return min.second;	
}
