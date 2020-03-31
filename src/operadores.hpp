#ifndef OPERADORES_HEADER
#define OPERADORES_HEADER
#include <vector>
#include <cassert>
#include <iostream>


template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b) {
	assert(a.size() == b.size());
	std::vector<T> res(a.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		res[i] = a[i]+b[i];
	}
	return res;
}


template <typename T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b) {
	assert(a.size() == b.size());
	std::vector<T> res(a.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		res[i] = a[i]-b[i];
	}
	return res;
}

template <typename T, typename U>
std::vector<U> operator-(const std::vector<T>& a, const std::vector<U>& b) {
	assert(a.size() == b.size());
	std::vector<U> res(a.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		res[i] = ((U)a[i])-b[i];
	}
	return res;
}
/*
template <typename T>
std::vector<T> operator/(const std::vector<T>& a, const std::vector<T>& b) {
	assert(a.size() == b.size());
	std::vector<T> res(a.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		res[i] =a[i]/b[i];
	}
	return res;
}
*/
template <typename T>
double operator*(const std::vector<T>& a, const std::vector<T>& b) {
	assert(a.size() == b.size());
	double res = (double)a[0]*(double)b[0];

	for(int i = 1; i < a.size(); ++i) {
		res += (double)a[i]*(double)b[i];
	}

	return res;
	
}


template <typename T>
std::vector<T> operator*(const std::vector<std::vector <T> >& A, const std::vector<T>& b) {
	assert(A[0].size() == b.size());
	std::vector<T> res;

	for(int i = 0; i < A.size(); ++i) {
		res.push_back(A[i]*b);
	}
	
	return res;
	
}


template <typename T,typename U>
std::vector<T> operator/(const std::vector<T>& a, const U& b) {
	std::vector<T> res(a.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		res[i] = (T)(a[i]/b);
	}
	return res;
}


template <typename T,typename U>
std::vector<T> operator/(const std::vector<T>& a, U b) {
	std::vector<T> res(a.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		res[i] = (T)(a[i]/b);
	}
	return res;
}

template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const T& b) {
	std::vector<T> res(a.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		res[i] = a[i]+b;
	}
	return res;
}


template <typename T>
std::vector<T> operator+(const std::vector<T>& a, T b) {
	std::vector<T> res(a.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		res[i] = a[i]+b;
	}
	return res;
}

template <typename T>
std::vector<T> operator-(const std::vector<T>& a, const T& b) {
	std::vector<T> res(a.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		res[i] = a[i]-b;
	}
	return res;
}


template <typename T>
std::vector<T> operator-(const std::vector<T>& a, T b) {
	std::vector<T> res(a.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		res[i] = a[i]-b;
	}
	return res;
}

template <typename T>
std::vector<T> operator*(const std::vector<T>& a, T b) {
	std::vector<T> res(a.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		res[i] = a[i]*b;
	}
	return res;
}

template <typename T>
std::vector<T> operator*(const std::vector<T>& a, const T& b) {
	std::vector<T> res(a.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		res[i] = a[i]*b;
	}
	return res;
}



template <typename T>
std::vector<T>& operator+=(std::vector<T>& a, const std::vector<T>& b ) {
	assert(a.size() == b.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		a[i] += b[i];
	}
	return a;
}

template <typename T, typename U>
std::vector<T>& operator+=(std::vector<T>& a, const std::vector<U>& b ) {
	
	assert(a.size() == b.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		a[i] += (T)b[i];
	}
	return a;
}




template <typename T>
std::vector<T>& operator-=(std::vector<T>& a, const std::vector<T>& b ) {
	assert(a.size() == b.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		a[i] -= b[i];
	}
	return a;
}

/*
template <typename T>
std::vector<T>& operator*=(std::vector<T>& a, const std::vector<T>& b ) {
	assert(a.size() == b.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		a[i] *= b[i];
	}
	return a;
}



template <typename T>
std::vector<T>& operator*=(std::vector<T>& a,std::vector<T> b ) {
	assert(a.size() == b.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		a[i] *= b[i];
	}
	return a;
}


template <typename T, typename U>
std::vector<T>& operator/=(std::vector<T>& a, const std::vector<U>& b ) {
	assert(a.size() == b.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		a[i] /= b[i];
	}
	return a;
}



template <typename T,typename U>
std::vector<T>& operator/=(std::vector<T>& a,std::vector<U> b ) {
	assert(a.size() == b.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		a[i] /= b[i];
	}
	return a;
}

*/



template <typename T,typename U>
std::vector<T>& operator+=(std::vector<T>& a, const U& b ) {
	
	for(unsigned int i = 0; i < a.size(); ++i) {
		a[i] += b;
	}
	return a;
}

template <typename T,typename U>
std::vector<T>& operator+=(std::vector<T>& a, U b ) {
	
	for(unsigned int i = 0; i < a.size(); ++i) {
		a[i] += b;
	}
	return a;
}



template <typename T,typename U>
std::vector<T>& operator-=(std::vector<T>& a, const U& b ) {
	
	for(unsigned int i = 0; i < a.size(); ++i) {
		a[i] -= b;
	}
	return a;
}



template <typename T,typename U>
std::vector<T>& operator-=(std::vector<T>& a,U b ) {
	
	for(unsigned int i = 0; i < a.size(); ++i) {
		a[i] -= b;
	}
	return a;
}


template <typename T,typename U>
std::vector<T>& operator*=(std::vector<T>& a,U b ) {
	
	for(unsigned int i = 0; i < a.size(); ++i) {
		a[i] *= b;
	}
	return a;
}






template <typename T,typename U>
std::vector<T>& operator/=(std::vector<T>& a, U b ) {
	for(unsigned int i = 0; i < a.size(); ++i) {
		a[i] /= b;
	}
	return a;
}


template <typename T>
std::vector<std::vector<T> >& operator+=(std::vector<std::vector<T> >& a, const std::vector<std::vector<T> >& b ) {
	assert(a.size() == b.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		for(unsigned int j = 0; j < a[i].size(); ++j) {
		a[i][j] += b[i][j];
		}
	}
	return a;
}
template <typename T>
std::vector<std::vector<T> >& operator-=(std::vector<std::vector<T> >& a, const std::vector<std::vector<T> >& b ) {
	assert(a.size() == b.size());
	for(unsigned int i = 0; i < a.size(); ++i) {
		for(unsigned int j = 0; j < a[i].size(); ++j) {
		a[i][j] -= b[i][j];
		}
	}
	return a;
}





#endif
