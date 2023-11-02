all: run

clean:
	rm -rf obj_dir
	rm -f *.vcd

f1_light: f1_light.sv clktick.sv f1_fsm.sv f1_light_tb.cpp
	verilator --Wall --cc --trace f1_light.sv clktick.sv f1_fsm.sv --exe f1_light_tb.cpp

run: clean f1_light
	make -j -C obj_dir/ -f Vf1_light.mk Vf1_light
	obj_dir/Vf1_light

.PHONY: clean f1_light run
