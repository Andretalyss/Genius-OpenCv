src = $(wildcard src/*.cpp)
obj = $(src:.cpp=.o)

CXXFLAGS += -std=c++11 -I include/ -g -c -Wall $(shell pkg-config --cflags opencv4)
LDFLAGS += $(shell pkg-config --libs --static opencv4)
LIBRARIES += opencv4_core opencv4_highgui opencv4_imgproc opencv4_videoio

exe: $(obj)
	$(CXX) $< -o $@ $(LDFLAGS)
	$(CXX) $< -o $@ $(CXXFLAGS)
clean:
	rm -rf *o exe
	rm -rf a.out
	rm -rf src/*.o
