.PHONY: clean all

BASE_DIR := mota/base
MDK_DIR  := mota/mdk

all:
	make -C $(BASE_DIR)
	make -C $(MDK_DIR)

clean:
	make -C $(BASE_DIR) clean
	make -C $(MDK_DIR) clean
