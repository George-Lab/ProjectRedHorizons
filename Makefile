CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++17
SRC=source
OBJ=build/obj
BIN=build/bin
SRCS=$(SRC)/lift.cpp $(SRC)/citizen.cpp $(SRC)/citizens_generator.cpp $(SRC)/logger.cpp
OBJS=$(OBJ)/lift.o $(OBJ)/citizen.o $(OBJ)/citizens_generator.o $(OBJ)/logger.o
BINS=$(BIN)/lift.out $(BIN)/citizen.out $(BIN)/citizens_generator.out $(BIN)/logger.out


all:$(BINS)

$(BIN)/%.out: $(OBJ)/%.o $(OBJ)/message_queue.o $(OBJ)/shared_memory.o $(OBJ)/process_time.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ)/message_queue.o: $(SRC)/include/message_queue.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(OBJ)/shared_memory.o: $(SRC)/include/shared_memory.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(OBJ)/process_time.o: $(SRC)/include/process_time.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ)/* $(BIN)/*
