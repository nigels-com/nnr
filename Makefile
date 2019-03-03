CPP_SRC  = AltGroup.cpp Normalised.cpp  permute.cpp \
            generra.cpp      sequence.cpp gennnr.cpp \
           GeneticAlg.cpp                test.cpp \
            genall.cpp       \
           HillClimb.cpp         gendirect.cpp \
           gennik.cpp            
           
NAME     = pesu

GLT_HOME = ../../../../
INCLUDE  = -I./ -I$(GLT_HOME)/src
GLT_LIBS = -L$(GLT_HOME)/lib -lglutm -lglt -lpng -lz -lgmp -lpurenum -lga

ifdef WINDIR
LIBS     = $(GLT_LIBS) -lglut32 -lglu32 -lopengl32
else
LIBS     = $(GLT_LIBS) -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11 -lXmu
endif

default: prog
	
include $(GLT_HOME)/gcc/Makefile.inc

