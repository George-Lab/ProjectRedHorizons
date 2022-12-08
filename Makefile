CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++17

BINS = build/bin/Lift_starter.out build/bin/Traveler.out build/bin/Generator.out

all: $(BINS)

build/bin/%.out: build/obj/%.o build/obj/actor.o build/obj/lift.o build/obj/message_queue.o
	$(CXX) $(CXXFLAGS) -g $^ -o $@

build/obj/actor.o: source/include/actor.cpp
	$(CXX) $(CXXFLAGS) -g -c $^ -o $@

build/obj/lift.o: source/include/lift.cpp
	$(CXX) $(CXXFLAGS) -g -c $^ -o $@

build/obj/message_queue.o: source/include/message_queue.cpp
	$(CXX) $(CXXFLAGS) -g -c $^ -o $@

build/obj/%.o: source/%.cpp
	$(CXX) $(CXXFLAGS) -g -c $< -o $@

clean:
	$(RM) -r build/bin/* build/obj/*
