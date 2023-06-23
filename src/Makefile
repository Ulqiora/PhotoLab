GXX=g++
BD=../build
CXX_FLAGS=-c -Wall -Werror -Wextra -std=c++17
GTESTFLAGS=-lgtest

ifeq ($(shell uname -s), Linux)
	GTESTFLAGS+= -lpthread -lrt
endif

MODEL_SOURCES= test.cpp Photo.cpp Filter.cpp SimpleFilter.cpp sliders.cpp controller.cpp 
MODEL_OBJECTS=$(addprefix $(BD)/,$(MODEL_SOURCES:.cpp=.o))

./$(BD)/%.o:%.cpp
	$(GXX) $(CXX_FLAGS) $< -o $@

./$(BD)/%.o: */%.cpp
	$(GXX) $(CXX_FLAGS) $< -o $@
 
./$(BD)/%.o: */*/%.cpp
	$(GXX) $(CXX_FLAGS) $< -o $@

./$(BD)/%.o: */*/*/%.cpp
	$(GXX) $(CXX_FLAGS) $< -o $@


all: test check clean

install: clean build copy

build:
	cd $(BD) && qmake ../src/PhotoLab/PhotoLab.pro && make

copy:
	mkdir $(HOME)/Applications/PhotoLab/
	cp -rf $(BD)/PhotoLab.app $(HOME)/Applications/PhotoLab/
	mkdir $(HOME)/Applications/PhotoLab/Save 
	open $(HOME)/Applications/PhotoLab/

uninstall:
	rm -rf $(HOME)/Applications/PhotoLab
	rm -rf $(BD)/*

dvi:
	open Readme/readme.html

dist:
	tar -cvf PhotoLab.tar .
	open .

test: clean $(MODEL_OBJECTS)
	$(GXX) $(MODEL_OBJECTS) -o $(BD)/$@ $(GTESTFLAGS)
	./$(BD)/$@

clean:
	rm -rf $(BD)/*
	rm -rf $(HOME)/Applications/PhotoLab/

check: 
	cppcheck --enable=all --suppress=missingInclude --suppress=unusedFunction --suppress=constParameter --language=c++ --suppress=unusedStructMember ../src/*.h ../src/*.cpp ../src/*/*.h ../src/*/*.cpp ../src/*/*/*.h ../src/*/*/*.cpp

clang:
	cp ../materials/linters/.clang-format . && \
	clang-format -n Controller/*.h Controller/*.cpp Model/*.h Model/*.cpp PhotoLab/*.h PhotoLab/*.cpp PhotoLab/*/*.h PhotoLab/*/*.cpp
	rm -f .clang-format
