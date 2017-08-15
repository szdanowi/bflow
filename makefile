SRC_DIR = src
TARGET_DIR=build
TARGET=$(TARGET_DIR)/tests

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TESTS = $(SRCS:$(SRC_DIR)/%Tests.cpp=%Tests)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(TARGET_DIR)/%.o)

GTEST_DIR = ext/googletest
GMOCK_DIR = ext/googlemock

CPPFLAGS += -isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include
CXXFLAGS += -g -Wall -Wextra -pthread -std=c++17
CXX = clang++

GTEST_HEADERS = $(wildcard $(GTEST_DIR)/include/gtest/*.h) \
                $(wildcard $(GTEST_DIR)/include/gtest/internal/*.h)
GMOCK_HEADERS = $(wildcard $(GMOCK_DIR)/include/gmock/*.h) \
                $(wildcard $(GMOCK_DIR)/include/gmock/internal/*.h) \
                $(GTEST_HEADERS)

all : $(TARGET_DIR) $(TARGET)
	@echo ""
	@echo "  done."

run_ut : $(TARGET)
	@$(TARGET)

$(TARGET) : $(OBJS) $(TARGET_DIR)/gmock_main.a
	@echo "  LD    $@"
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

clean :
	@echo "  RM    $(TARGET_DIR)"
	@rm -rf $(TARGET_DIR)

GTEST_SRCS_ = $(wildcard $(GTEST_DIR)/src/*.cc) $(wildcard $(GTEST_DIR)/src/*.h) $(GTEST_HEADERS)
GMOCK_SRCS_ = $(wildcard $(GMOCK_DIR)/src/*.cc) $(GMOCK_HEADERS)

$(TARGET_DIR)/gtest-all.o : $(GTEST_SRCS_)
	@echo "  OBJ   $@"
	@$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
            -c $(GTEST_DIR)/src/gtest-all.cc -o $@

$(TARGET_DIR)/gmock-all.o : $(GMOCK_SRCS_)
	@echo "  OBJ   $@"
	@$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
            -c $(GMOCK_DIR)/src/gmock-all.cc -o $@

$(TARGET_DIR)/gmock_main.o : $(GMOCK_SRCS_)
	@echo "  OBJ   $@"
	@$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
            -c $(GMOCK_DIR)/src/gmock_main.cc -o $@

$(TARGET_DIR)/gmock.a : $(TARGET_DIR)/gmock-all.o $(TARGET_DIR)/gtest-all.o
	@echo "  AR    $@"
	@$(AR) $(ARFLAGS) $@ $^ > /dev/null

$(TARGET_DIR)/gmock_main.a : $(TARGET_DIR)/gmock-all.o $(TARGET_DIR)/gtest-all.o $(TARGET_DIR)/gmock_main.o
	@echo "  AR    $@"
	@$(AR) $(ARFLAGS) $@ $^ > /dev/null

$(TARGET_DIR)/%.o : $(SRC_DIR)/%.cpp $(GMOCK_HEADERS)
	@echo "  OBJ   $@"
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(@:$(TARGET_DIR)/%.o=$(SRC_DIR)/%.cpp) -o $@

$(TARGET_DIR):
	@echo "  MKDIR $@"
	@mkdir -p $@
