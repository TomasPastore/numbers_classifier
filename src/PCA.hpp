#ifndef PCA_HEADER
#define PCA_HEADER
#include <vector>
#include <utility>
#include "operadores.hpp"

class PCA {
	
	public:

		std::vector<double> transformarDigito(const std::vector<unsigned char>& ) const;

		PCA(const std::vector<std::vector<unsigned char> >&, unsigned int);

		void media(std::vector < double >&, const std::vector<std::vector<double> > &);

		const std::vector < std::vector <double> >& mTransformada() const;
		
		const std::vector <double>& autovalores() const;

		static std::pair< std::vector<double>,double> mPotencia(const std::vector<std::vector<double> >&);

		void calcularAutoVectores(std::vector < std::vector < double > >&);		

		void prodTraspuesta(const std::vector < std::vector < double > >&, std::vector < std::vector < double > >&);

	private:

		unsigned int Divisor;
		std::vector< double > Media;
		std::vector < std::vector <double> > MTransformada;
		std::vector < std::vector <double> > Autovectores;
		std::vector <double>  Autovalores;

};

#endif
