CXX_FLAGS   := -std=c++11 -ggdb3 -Wall -Werror
AR_FLAGS    = crs
MUDUO_DIR  := ../../muduo_stdc11
MOTA_DIR := ../../

include $(MOTA_DIR)/tool-chain.mk

INC     := .
INC     += $(MUDUO_DIR)
INC     += $(MOTA_DIR)

INC_FLAGS := $(foreach path, $(INC), -I$(path))

SRCS    := $(wildcard *.cpp)
HEADS   := $(wildcard *.h)
OBJS    := $(patsubst %.cpp, %.o, $(SRCS))

TARGET	:= build/libmota_mdk.a

.PHONEY: clean

all: setup $(TARGET)

setup:
	@echo $(AR) $(RANLIB)
	@mkdir -p build
	
$(TARGET): $(OBJS)
	$(AR) $(AR_FLAGS) $@ build/$(notdir $^)
	$(RANLIB) $@

%.o:%.cpp $(HEADS)
	g++ -c -o build/$(notdir $@) $(filter-out %.h,$^) $(CXX_FLAGS) $(INC_FLAGS)

clean:
	rm -rf build/