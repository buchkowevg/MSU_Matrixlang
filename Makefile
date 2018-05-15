CXX=g++
CXXFLAGS= -std=c++0x -Wextra -g 
OBJECTS       = ./bins/main.o \
		./bins/matrix.o \
		./bins/matrix_coord.o \
		./bins/my_exeptions.o \
		./bins/mystring.o \
		./bins/rational_number.o \
		./bins/vector.o \
		./bins/lex_analizator.o \
		./bins/sin_analizarot.o \
		./bins/poliz.o

first: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o analizator 
./bins/main.o: main.cpp ident.h \
		lex.h \
		./sparse/matrix.h \
		./sparse/matrix_coord.h \
		./sparse/rational_number.h \
		./sparse/mystring.h \
		./sparse/vector.h \
		lex_analizator.h \
		sin_analizarot.h
	$(CXX) -c $(CXXFLAGS)  -o ./bins/main.o ./main.cpp

./bins/matrix.o: ./sparse/matrix.cpp ./sparse/matrix.h \
		./sparse/matrix_coord.h \
		./sparse/rational_number.h \
		./sparse/mystring.h \
		./sparse/vector.h \
		./sparse/matrix_ex.h \
		./sparse/sparse_ex.h \
		./sparse/vector_ex.h \
		./sparse/rational_number_ex.h
	$(CXX) -c $(CXXFLAGS) -o ./bins/matrix.o ./sparse/matrix.cpp

./bins/matrix_coord.o: ./sparse/matrix_coord.cpp ./sparse/matrix_coord_row_column.h \
		./sparse/matrix.h \
		./sparse/matrix_coord.h \
		./sparse/rational_number.h \
		./sparse/mystring.h \
		./sparse/vector.h
	$(CXX) -c $(CXXFLAGS)  -o ./bins/matrix_coord.o ./sparse/matrix_coord.cpp

./bins/my_exeptions.o: ./sparse/my_exeptions.cpp ./sparse/matrix_ex.h \
		./sparse/matrix.h \
		./sparse/matrix_coord.h \
		./sparse/rational_number.h \
		./sparse/mystring.h \
		./sparse/vector.h \
		./sparse/sparse_ex.h \
		./sparse/vector_ex.h \
		./sparse/rational_number_ex.h
	$(CXX) -c $(CXXFLAGS)  -o ./bins/my_exeptions.o ./sparse/my_exeptions.cpp

./bins/mystring.o: ./sparse/mystring.cpp ./sparse/mystring.h
	$(CXX) -c $(CXXFLAGS)  -o ./bins/mystring.o ./sparse/mystring.cpp

./bins/rational_number.o: ./sparse/rational_number.cpp ./sparse/rational_number.h \
		./sparse/mystring.h \
		./sparse/rational_number_ex.h \
		./sparse/sparse_ex.h
	$(CXX) -c $(CXXFLAGS) -o ./bins/rational_number.o ./sparse/rational_number.cpp

./bins/vector.o: ./sparse/vector.cpp ./sparse/mystring.h \
		./sparse/vector.h \
		./sparse/rational_number.h \
		./sparse/vector_ex.h \
		./sparse/sparse_ex.h \
		./sparse/rational_number_ex.h
	$(CXX) -c $(CXXFLAGS) -o ./bins/vector.o ./sparse/vector.cpp

./bins/lex_analizator.o: lex_analizator.cpp lex_analizator.h \
		ident.h \
		lex.h \
		./sparse/matrix.h \
		./sparse/matrix_coord.h \
		./sparse/rational_number.h \
		./sparse/mystring.h \
		./sparse/vector.h
	$(CXX) -c $(CXXFLAGS) -o ./bins/lex_analizator.o lex_analizator.cpp

./bins/sin_analizarot.o: sin_analizarot.cpp sin_analizarot.h \
		lex.h \
		ident.h \
		./sparse/matrix.h \
		./sparse/matrix_coord.h \
		./sparse/rational_number.h \
		./sparse/mystring.h \
		./sparse/vector.h \
		./sparse/sparse_ex.h
	$(CXX) -c $(CXXFLAGS) -o ./bins/sin_analizarot.o sin_analizarot.cpp

./bins/poliz.o: poliz.cpp poliz.h \
		ident.h \
		lex.h \
		./sparse/matrix.h \
		./sparse/matrix_coord.h \
		./sparse/rational_number.h \
		./sparse/mystring.h \
		./sparse/vector.h \
		./sparse/sparse_ex.h
	$(CXX) -c $(CXXFLAGS) -o ./bins/poliz.o poliz.cpp
clean:
	rm $(OBJECTS) analizator 
