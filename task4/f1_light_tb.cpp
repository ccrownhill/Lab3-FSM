#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vf1_light.h"
#include <cstdint>

#include "vbuddy.cpp"		 // include vbuddy code
#define MAX_SIM_CYC 100000

enum State {WAIT_TRIG, TRIGGERED, RUNNING, WAIT_REACT};

// use shift and add 3 algorithm
uint16_t decimal_to_bcd(uint16_t x) {
	uint32_t result = x;
	int i, n, box4;
	
	for (i = 1; i <= 16; i++) {
		for (int n = 0; n < 4; n++) {
			box4 = (result >> (16 + n*4)) & 0xf;
			if (box4 >= 5) {
				result += 3 << (16 + n*4);
			}
		}
		result <<= 1;
	}
	return (uint16_t)(result>>16);
}

int main(int argc, char **argv, char **env) {
	int simcyc; // simulation clock count
	int tick; // each clk cycle has two ticks for two edges
	int elapsed_ms; // reaction time in ms
	
	int lights = 0; // state to toggle LED lights

	Verilated::commandArgs(argc, argv);
	// init top verilog instance
	Vf1_light* top = new Vf1_light;
	// init trace dump
	Verilated::traceEverOn(true);
	VerilatedVcdC* tfp = new VerilatedVcdC;
	top->trace (tfp, 99);
	tfp->open ("f1_light.vcd");
 
	// init Vbuddy
	if (vbdOpen() != 1)
		return -1;
	vbdHeader("React t (ms)");
	vbdSetMode(1);	// Flag mode set to one-shot

	// initialize simulation inputs
	top->clk = 1;
	top->rst = 0;
	top->trigger = 0;
	State curr_state = WAIT_TRIG, next_state;
	
	// run simulation for MAX_SIM_CYC clock cycles
	for (simcyc=0; simcyc<MAX_SIM_CYC; simcyc++) {
		// dump variables into VCD file and toggle clock
		for (tick=0; tick<2; tick++) {
			tfp->dump (2*simcyc+tick);
			top->clk = !top->clk;
			top->eval();
		}

		// set up input signals of testbench
		top->rst = (simcyc < 2); // assert reset for 1st cycle
		switch (curr_state) {
			case WAIT_TRIG:
				if (vbdFlag()) {
					top->trigger = 1;
					next_state = TRIGGERED;
				}
				break;
			case TRIGGERED:
				top->trigger = 0;
				if (top->data_out != 0)
					next_state = RUNNING;
				break;
			case RUNNING:
				if (top->data_out == 0) {
					vbdInitWatch();
					next_state = WAIT_REACT;
				}
				break;
			case WAIT_REACT:
				if (vbdFlag()) {
					elapsed_ms = vbdElapsed();
					elapsed_ms = decimal_to_bcd(elapsed_ms);		
					vbdHex(1, elapsed_ms & 0xf);
					vbdHex(2, (elapsed_ms >> 4) & 0xf);
					vbdHex(3, (elapsed_ms >> 8) & 0xf);
					vbdHex(4, (elapsed_ms >> 12) & 0xf);
					next_state = WAIT_TRIG;
				}
				break;
		}
		curr_state = next_state;
		vbdBar(top->data_out & 0xFF);
		
		vbdCycle(simcyc);

		if (Verilated::gotFinish())
			exit(0);
	}

	vbdClose();
	tfp->close(); 
	exit(0);
}
