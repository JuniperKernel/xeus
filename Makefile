SRC = src
CPP_FILES = $(wildcard $(SRC)/*cpp)
INC = -I./include
ZMQ_LIB = -L/usr/local/lib -lzmq

OBJ_FILES = $(patsubst $(SRC)/%.cpp,$(SRC)/%.o,$(CPP_FILES))

CXX_STD      = CXX11
PKG_CPPFLAGS = $(INC)
PKG_LIBS = $(ZMQ_LIB)

default: libxeus.so

libxeus.so: $(OBJ_FILES)
	$(CXX) $(ALL_CPPFLAGS) -c $< -o $@
	$(CXX) $(ALL_LIBS) -o $@ $^


.PHONY: clean
clean:
	rm -rf build
	rm -rf src/*o
	rm -rf *so
