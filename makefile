OBJ=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
a.out:$(OBJ)
	g++ -o $@ $^
clean: 
	rm -f *.exe *.o *.out