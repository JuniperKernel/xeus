SRC = src
CPP_FILES = $(wildcard $(SRC)/*cpp)
OBJ_FILES = $(patsubst $(SRC)/%.cpp,$(SRC)/%.o,$(CPP_FILES))

default: libxeus.so

libxeus.so: $(OBJ_FILES)
	$(CXX) $(ALL_LIBS) -o $@ $^

%.o: %.cpp
	$(CXX) $(ALL_CPPFLAGS) -I./include -c $< -o $@

.PHONY: clean
clean:
	rm -rf build
	rm -rf src/*o
	rm -rf *so
