#ifndef KNN_HEADER
#define KNN_HEADER

#include <math.h> 
#include <vector>
#include <utility>
#include <algorithm>
using std::vector;

int maxPos(const std::vector<int>& vec);

int maxPos(const std::vector<double>& vec);

double norma2(const vector<double>& vec);

double normaEuclidea(const std::vector<unsigned char>& imagen, const std::vector<unsigned char>& vectorDb);

double normaEuclidea(const std::vector<unsigned char>& imagen, const std::vector<double>& vectorDb);

double normaEuclidea(const std::vector<double>& imagen, const std::vector<double>& vectorDb);

unsigned int distanciaAlCuadrado(const vector<unsigned char>& imagen, const vector<unsigned char>& vectorDb);

double distanciaAlCuadrado(const vector<double>& imagen, const vector<double>& vectorDb);

int Knn(const std::vector<int>& labels, const std::vector<std::vector<unsigned char> >& muestras, const std::vector< unsigned char >& imagen,  unsigned int k );

int KnnD(const std::vector<int>& labels, const std::vector<std::vector<double> >& muestras, const std::vector< double >& imagen,  unsigned int k );

std::vector< std::vector<double> > promedioDeClases(const std::vector<int>& labels, const std::vector<std::vector<unsigned char> >& muestras );

int clasificar(const std::vector<std::vector<double> >promediosDeClases, const std::vector< unsigned char >& imagen);



//double normaEuclidea(const std::vector<double>& imagen, const std::vector<double>& vectorDb);

#endif
