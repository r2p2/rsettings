CXX?=g++
TARGET=rsettings_test
TARGET_PATH=../build

INCLUDES=-I../include

all: run-test

$(TARGET_PATH)/$(TARGET): main.cc ../include/rsettings/rsettings.hpp ../include/rsettings/rtokenizer.hpp
	mkdir -p $(TARGET_PATH)
	$(CXX) -o $(TARGET_PATH)/$(TARGET) $(INCLUDES) -ggdb -O0 -Wall -Wextra -Weffc++ main.cc

run-test: $(TARGET_PATH)/$(TARGET)
	$(TARGET_PATH)/$(TARGET)

clean:
	rm $(TARGET_PATH)/$(TARGET)
