CPP_SRC += nnr.cpp
CPP_SRC += sequence.cpp
CPP_SRC += test.cpp
CPP_SRC += main.cpp

ifdef DEBUG
CPP_FLAGS += -g
else
CPP_FLAGS += -O3
endif

nnr: Makefile $(CPP_SRC)
	g++ $(CPP_SRC) ${CPP_FLAGS} -o $@

default: nnr

all: default

clean:
	rm nnr

.PHONY: default clean all