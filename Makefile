SRC = src
CPP_FILES = $(wildcard $(SRC)/*cpp)
OBJ_FILES = $(patsubst $(SRC)/%.cpp,$(SRC)/%.o,$(CPP_FILES))

libxeus.so: $(OBJ_FILES)
	$(CXX) $(ALL_LIBS) -shared -o $@ $^

libxeus.dll: $(OBJ_FILES)
	$(CXX) $(ALL_LIBS) -shared -o $@ $^

%.o: %.cpp
	$(CXX) $(ALL_CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf build
	rm -rf src/*o
	rm -rf *so
