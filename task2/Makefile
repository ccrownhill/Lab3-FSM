all: run

clean:
	rm -rf obj_dir
	rm -f f1_fsm.vcd

f1_fsm: f1_fsm.sv f1_fsm_tb.cpp
	verilator --Wall --cc --trace f1_fsm.sv --exe f1_fsm_tb.cpp

run: clean f1_fsm
	make -j -C obj_dir/ -f Vf1_fsm.mk Vf1_fsm
	obj_dir/Vf1_fsm

.PHONY: clean f1_fsm run
