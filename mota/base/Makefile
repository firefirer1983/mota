
CXX_FLAGS   := -std=c++11 -ggdb3 -Wall -Werror
MUDUO_DIR  := ../../muduo_stdc11
MOTA_DIR := ../../

INC     := .
INC     += $(MUDUO_DIR)
INC     += $(MOTA_DIR)

INC_FLAGS := $(foreach path, $(INC), -I$(path))

SRCS    := $(wildcard *.cpp)
HEADS   := $(wildcard *.h)
OBJS    := $(patsubst %.cpp, %.o, $(SRCS))

TARGET	:= libmota_base

.PHONEY: clean

all: setup $(TARGET)

setup:
	@mkdir -p build/

$(TARGET): $(OBJS)

%.o:%.cpp $(HEADS)
	g++ -c -o build/$(notdir $@) $(filter-out %.h,$^) $(CXX_FLAGS) $(INC_FLAGS)

clean:
	rm -rf build/