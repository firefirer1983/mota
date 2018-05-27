CXX_FLAGS   := -std=c++11 -ggdb3 -Wall -Werror
AR_FLAGS    = crs
MUDUO_DIR  := ../../muduo_stdc11
MUDUO_LIB_DIR := $(MUDUO_DIR)/build/lib

MOTA_DIR := ../..
MDK_DIR  := $(MOTA_DIR)/mota/mdk
MDK_LIB_DIR := $(MDK_DIR)/build/lib

include $(MOTA_DIR)/tool-chain.mk

INC     := .
INC     += $(MUDUO_DIR)
INC     += $(MOTA_DIR)

INC_FLAGS := $(foreach path, $(INC), -I$(path))

SRCS    := $(wildcard *.cpp)
HEADS   := $(wildcard *.h)
OBJS    := $(patsubst %.cpp, %.o, $(SRCS))

OBJ_OUT_DIR	:= build/obj
BIN_OUT_DIR := build/bin

TARGET	:= $(BIN_OUT_DIR)/single_client

.PHONEY: clean

all: setup $(TARGET)

setup:
	@mkdir -p $(OBJ_OUT_DIR)
	@mkdir -p $(BIN_OUT_DIR)

	
$(TARGET): $(addprefix $(OBJ_OUT_DIR)/, $(OBJS))
	$(CXX) -o $@ $^ -L$(MDK_LIB_DIR) -lmota_mdk -L$(MUDUO_LIB_DIR) -lmuduo -lpthread -lcares -lz

$(OBJ_OUT_DIR)/%.o:%.cpp $(HEADS)
	$(CXX) -o $(OBJ_OUT_DIR)/$(notdir $@) -c $(filter-out %.h,$^) $(CXX_FLAGS) $(INC_FLAGS)

clean:
	rm -rf build/