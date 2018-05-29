.PHONY: clean all

MDK_DIR  := mota/mdk

all:
	make -C $(MDK_DIR)
	make -C mota/examples/ clean
	make -C mota/examples/

clean:
	make -C $(MDK_DIR) clean
