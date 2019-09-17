CXX ?= g++

CXXFLAGS += -c -Wall $(shell pkg-config --cflags opencv4) 
LDFLAGS += $(shell pkg-config --libs --static opencv4) 
LIBRARIES += opencv4_core opencv4_highgui opencv4_imgproc opencv4_videoio

all: opencv_example

opencv_example: example.o; $(CXX) $< -o $@ $(LDFLAGS)

%.o: %.cpp; $(CXX) $< -o $@ $(CXXFLAGS)

clean: ; rm -f example.o opencv_example
