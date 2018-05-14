CXX=g++
CXXFLAGS= -Wextra -g 
OBJECTS       = main.o \
		matrix.o \
		matrix_coord.o \
		my_exeptions.o \
		mystring.o \
		rational_number.o \
		vector.o \
		lex_analizator.o \
		sin_analizarot.o \
		poliz.o

first: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o analizator 
main.o: main.cpp ident.h \
		lex.h \
		../qt/sparse/matrix.h \
		../qt/sparse/matrix_coord.h \
		../qt/sparse/rational_number.h \
		../qt/sparse/mystring.h \
		../qt/sparse/vector.h \
		lex_analizator.h \
		sin_analizarot.h
	$(CXX) -c $(CXXFLAGS)  -o main.o ../qt/analizator/main.cpp

matrix.o: ../qt/sparse/matrix.cpp ../qt/sparse/matrix.h \
		../qt/sparse/matrix_coord.h \
		../qt/sparse/rational_number.h \
		../qt/sparse/mystring.h \
		../qt/sparse/vector.h \
		../qt/sparse/matrix_ex.h \
		../qt/sparse/sparse_ex.h \
		../qt/sparse/vector_ex.h \
		../qt/sparse/rational_number_ex.h
	$(CXX) -c $(CXXFLAGS) -o matrix.o ../qt/sparse/matrix.cpp

matrix_coord.o: ../qt/sparse/matrix_coord.cpp ../qt/sparse/matrix_coord_row_column.h \
		../qt/sparse/matrix.h \
		../qt/sparse/matrix_coord.h \
		../qt/sparse/rational_number.h \
		../qt/sparse/mystring.h \
		../qt/sparse/vector.h
	$(CXX) -c $(CXXFLAGS)  -o matrix_coord.o ../qt/sparse/matrix_coord.cpp

my_exeptions.o: ../qt/sparse/my_exeptions.cpp ../qt/sparse/matrix_ex.h \
		../qt/sparse/matrix.h \
		../qt/sparse/matrix_coord.h \
		../qt/sparse/rational_number.h \
		../qt/sparse/mystring.h \
		../qt/sparse/vector.h \
		../qt/sparse/sparse_ex.h \
		../qt/sparse/vector_ex.h \
		../qt/sparse/rational_number_ex.h
	$(CXX) -c $(CXXFLAGS)  -o my_exeptions.o ../qt/sparse/my_exeptions.cpp

mystring.o: ../qt/sparse/mystring.cpp ../qt/sparse/mystring.h
	$(CXX) -c $(CXXFLAGS)  -o mystring.o ../qt/sparse/mystring.cpp

rational_number.o: ../qt/sparse/rational_number.cpp ../qt/sparse/rational_number.h \
		../qt/sparse/mystring.h \
		../qt/sparse/rational_number_ex.h \
		../qt/sparse/sparse_ex.h
	$(CXX) -c $(CXXFLAGS) -o rational_number.o ../qt/sparse/rational_number.cpp

vector.o: ../qt/sparse/vector.cpp ../qt/sparse/mystring.h \
		../qt/sparse/vector.h \
		../qt/sparse/rational_number.h \
		../qt/sparse/vector_ex.h \
		../qt/sparse/sparse_ex.h \
		../qt/sparse/rational_number_ex.h
	$(CXX) -c $(CXXFLAGS) -o vector.o ../qt/sparse/vector.cpp

lex_analizator.o: lex_analizator.cpp lex_analizator.h \
		ident.h \
		lex.h \
		../qt/sparse/matrix.h \
		../qt/sparse/matrix_coord.h \
		../qt/sparse/rational_number.h \
		../qt/sparse/mystring.h \
		../qt/sparse/vector.h
	$(CXX) -c $(CXXFLAGS) -o lex_analizator.o lex_analizator.cpp

sin_analizarot.o: sin_analizarot.cpp sin_analizarot.h \
		lex.h \
		ident.h \
		../qt/sparse/matrix.h \
		../qt/sparse/matrix_coord.h \
		../qt/sparse/rational_number.h \
		../qt/sparse/mystring.h \
		../qt/sparse/vector.h \
		../qt/sparse/sparse_ex.h
	$(CXX) -c $(CXXFLAGS) -o sin_analizarot.o sin_analizarot.cpp

poliz.o: poliz.cpp poliz.h \
		ident.h \
		lex.h \
		../qt/sparse/matrix.h \
		../qt/sparse/matrix_coord.h \
		../qt/sparse/rational_number.h \
		../qt/sparse/mystring.h \
		../qt/sparse/vector.h \
		../qt/sparse/sparse_ex.h
	$(CXX) -c $(CXXFLAGS) -o poliz.o poliz.cpp
clean:
	rm $(OBJECTS) analizator 