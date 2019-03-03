CPP_SRC += main.cpp        
CPP_SRC += nnr.cpp        
CPP_SRC += sequence.cpp          
           
nnr:
	g++ $(CPP_SRC) -o $@

default: nnr
	

