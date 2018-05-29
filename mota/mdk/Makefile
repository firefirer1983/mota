CXX_FLAGS   := -std=c++11 -ggdb3 -Wall -Werror
AR_FLAGS    = crs
MUDUO_DIR  := ../../muduo_stdc11
MOTA_DIR := ../../
HTTP_PARSER_DIR := ../../http-parser

include $(MOTA_DIR)/tool-chain.mk

INC     := .
INC     += $(MUDUO_DIR)
INC     += $(MOTA_DIR)
INC     += $(HTTP_PARSER_DIR)

INC_FLAGS := $(foreach path, $(INC), -I$(path))

SRCS    := $(wildcard *.cpp)
HEADS   := $(wildcard *.h)
OBJS    := $(patsubst %.cpp, %.o, $(SRCS))

OBJ_OUT_DIR	:= build/obj
LIB_OUT_DIR := build/lib

TARGET	:= $(LIB_OUT_DIR)/libmota_mdk.a

.PHONEY: clean

all: setup $(TARGET)

setup:
	@mkdir -p $(OBJ_OUT_DIR)
	@mkdir -p $(LIB_OUT_DIR)

	
$(TARGET): $(addprefix $(OBJ_OUT_DIR)/, $(OBJS))
	$(AR) $(AR_FLAGS) $@ $^
	$(RANLIB) $@

$(OBJ_OUT_DIR)/%.o:%.cpp $(HEADS)
	$(CXX) -o $(OBJ_OUT_DIR)/$(notdir $@) -c $(filter-out %.h,$^) $(CXX_FLAGS) $(INC_FLAGS)

clean:
	rm -rf build/