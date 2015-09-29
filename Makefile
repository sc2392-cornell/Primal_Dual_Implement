#this compiles the program
all:
	g++-4.9 -std=c++11 main.cpp PSteiner.cpp PSteiner_TM.cpp -o Algo
#this executes the program with input file "pcstp/K100.dat"
execute_K100:
	./Algo pcstp/K100.dat

#more data files can be executed as above 

