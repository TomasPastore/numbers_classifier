#ifndef PLS_HEADER
#define PLS_HEADER

#include <math.h> 
#include <vector>
#include <algorithm>

using std::vector;

vector<double>  media(const vector< vector< unsigned char > >& matrix);

vector < vector<double> > pls(vector< vector< unsigned char > > & x, vector< int > & labels,  int gamma, vector<double>& autovalores);

vector< vector<double> > tCaracteristica(vector< vector< unsigned char > >& x, vector< int >& labels,  int gamma, vector<vector<double> >& cambioDeBase, vector<double>& autovalores );

vector<double> plsTransformarDigito(const std::vector<unsigned char>& digito, const vector <vector<double> >& cambioDeBase, const vector<double>& Media, int n);


#endif
