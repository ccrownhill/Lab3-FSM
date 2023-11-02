all: run

clean:
	rm -rf obj_dir
	rm -f lfsr.vcd

lfsr: lfsr.sv lfsr_tb.cpp
	verilator --Wall --cc --trace lfsr.sv --exe lfsr_tb.cpp

run: clean lfsr
	make -j -C obj_dir/ -f Vlfsr.mk Vlfsr
	obj_dir/Vlfsr

.PHONY: clean lfsr run
