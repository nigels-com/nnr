#CPP_SRC += src/nnr.cpp
CPP_SRC += src/sequence.cpp
CPP_SRC += src/test.cpp
#CPP_SRC += main.cpp

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

nnr: nnr.cpp Makefile $(CPP_SRC)
	$(CXX) $(CPP_SRC) $< ${CPP_FLAGS} -o $@

pes: pes.cpp Makefile $(CPP_SRC)
	$(CXX) $(CPP_SRC) $< ${CPP_FLAGS} -o $@

all: default

clean:
	rm -f nnr

.PHONY: default clean all