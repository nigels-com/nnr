CPP_FLAGS += -Isrc
CPP_FLAGS += -Ithirdparty/include
CPP_FLAGS += -std=c++11

ifdef DEBUG
CPP_FLAGS += -g
else
CPP_FLAGS += -O3 -DNDEBUG
endif

#CPP_FLAGS += -Wall

default: nnr pes

nnr: nnr.cpp Makefile
	$(CXX) $(CPP_SRC) $< ${CPP_FLAGS} -o $@

pes: pes.cpp Makefile
	$(CXX) $(CPP_SRC) $< ${CPP_FLAGS} -o $@

all: default

clean:
	rm -f nnr pes

.PHONY: default clean all