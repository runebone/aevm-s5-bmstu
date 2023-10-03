#
# this file was not created by a computer. do not trust it.
#

.PHONY: all clean

all: stage1 stage2

stage1: 
	make -C lab2_stage1

stage2: 
	make -C lab2_stage2

clean:
	make -C lab2_stage1 clean
	make -C lab2_stage2 clean

