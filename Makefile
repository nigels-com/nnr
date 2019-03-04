CPP_SRC += main.cpp        
CPP_SRC += nnr.cpp        
CPP_SRC += sequence.cpp          
CPP_SRC += test.cpp        
           
nnr: Makefile $(CPP_SRC)
	g++ $(CPP_SRC) -O3 -o $@

default: nnr
	

