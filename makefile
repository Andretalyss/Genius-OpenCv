CXXFLAGS += -std=c++11 -I include/ -g -c -Wall $(shell pkg-config --cflags opencv4)
LDFLAGS += $(shell pkg-config --libs --static opencv4)
LIBRARIES += opencv4_core opencv4_highgui opencv4_imgproc opencv4_videoio
COMP = g++

all: src/main.o src/Genius.o
	$(COMP) -o exe src/main.o src/Genius.o $(LDFLAGS)

Genius.o: src/Genius.cpp headers/Genius.hpp
	$(COMP) $(CXXFLAGS) src/Genius.cpp -o src/Genius.o $(LDFLAGS)

clean:
	rm -rf *o exe
	rm -rf a.out
	rm -rf src/*.o
