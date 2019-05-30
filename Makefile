lib = -lcurses
idir = -I include/ -I . 
srcs = src/data.cpp src/display.cpp

VIM: main.cpp $(srcs)
	g++ -o VIM main.cpp $(srcs) $(lib) $(idir)


#build/src.o: $(srcs)
#	g++ -c $(srcs) $(lib) $(dir) -o build/

#build/main.o: main.cpp
#	g++ -o build/main.o -c main.cpp

