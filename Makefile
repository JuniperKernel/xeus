#SHELL := /bin/bash
.DELETE_ON_ERROR:

# for printing variable values
# usage: make print-VARIABLE
#        > VARIABLE = value_of_variable
print-%  : ; @echo $*=$($*)

SRC = src
CPP_FILES = $(wildcard $(SRC)/*cpp)
INC = -I./include
ZMQ_LIB = -L/usr/local/lib -lzmq

OBJ_FILES = $(patsubst $(SRC)/%.cpp,$(SRC)/%.o,$(CPP_FILES))
CPPFLAGS=-I/usr/local/include $(INC)
CXX = clang++ -std=c++11
LDFLAGS=-dynamiclib -Wl,-headerpad_max_install_names -undefined dynamic_lookup -single_module -multiply_defined suppress -L/usr/local/lib $(ZMQ_LIB) -Wl,-framework -Wl,CoreFoundation

default: libxeus.so

libxeus.so: $(OBJ_FILES)
	$(CXX) $(CPPFLAGS) -c $< -o $@
	$(CXX) $(LDFLAGS) -o $@ $^


.PHONY: clean
clean:
	rm -rf build
	rm -rf src/*o
