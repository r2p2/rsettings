CXX?=g++
TARGET=rsettings_test
TARGET_PATH=../build

INCLUDES=-I../include

GTEST_DIR=googletest

all: run-test

libgtest.a: $(GTEST_DIR)/src/gtest-all.cc
	$(CXX) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread \
		-c $(GTEST_DIR)/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

$(TARGET_PATH)/$(TARGET): main.cc ../include/rsettings/error.hpp \
		../include/rsettings/ini.hpp libgtest.a
	mkdir -p $(TARGET_PATH)
	$(CXX) -isystem $(GTEST_DIR)/include \
			-pthread -ggdb -O0 -Wall -Wextra -Weffc++ \
			$(INCLUDES) main.cc \
			libgtest.a -o $(TARGET_PATH)/$(TARGET)

run-test: $(TARGET_PATH)/$(TARGET)
	$(TARGET_PATH)/$(TARGET)

clean:
	rm $(TARGET_PATH)/$(TARGET)
	rm *.o *.a

.PHONY: clean
