CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Isrc
BUILD := build
COMMON := src/common.cpp

.PHONY: all run-tests clean app

all: run-tests

run-tests: $(BUILD)/test-common $(BUILD)/test-warehouse $(BUILD)/test-hashmap $(BUILD)/test-route
  $(BUILD)/test-common
  $(BUILD)/test-warehouse
  $(BUILD)/test-hashmap
  $(BUILD)/test-route

$(BUILD)/test-common: src/tests/test-common.cpp $(COMMON) | $(BUILD)
  $(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/test-warehouse: src/tests/test-warehouse.cpp src/warehouse.cpp $(COMMON) | $(BUILD)
  $(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/test-hashmap: src/tests/test-hashmap.cpp src/warehouse.cpp $(COMMON) | $(BUILD)
  $(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/test-route: src/tests/test-route.cpp src/io.cpp src/astar.cpp src/warehouse.cpp $(COMMON) | $(BUILD)
  $(CXX) $(CXXFLAGS) $^ -o $@

app: src/main.cpp src/command-manager.cpp src/io.cpp src/astar.cpp src/warehouse.cpp $(COMMON) | $(BUILD)
  $(CXX) $(CXXFLAGS) $^ -o $(BUILD)/app

$(BUILD):
  mkdir -p $(BUILD)

clean:
  rm -rf $(BUILD)
