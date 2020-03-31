CC=g++
CFLAGS=-std=c++11 -O2
DFLAGS=-ggdb -std=c++11 -Wall -O0
DEPS=knn.hpp PCA.hpp PLS.hpp operadores.hpp metricas.hpp utils.hpp macrostest.hpp
_OBJ=knn.o pca.o pls.o main.o 
ODIR=obj

 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ejec: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean all

clean:
	rm -f $(ODIR)/*.o



