CXX?=g++
TARGET=rsettings_test
TARGET_PATH=../build

all: run-test

$(TARGET_PATH)/$(TARGET): main.cc
	mkdir -p $(TARGET_PATH)
	$(CXX) -o $(TARGET_PATH)/$(TARGET) main.cc

run-test: $(TARGET_PATH)/$(TARGET)
	$(TARGET_PATH)/$(TARGET)

clean:
	rm $(TARGET_PATH)/$(TARGET)
