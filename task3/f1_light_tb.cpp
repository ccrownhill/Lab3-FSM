#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vf1_light.h"

#include "vbuddy.cpp"		 // include vbuddy code
#define MAX_SIM_CYC 100000
#define WAIT_CYCLES 33

int main(int argc, char **argv, char **env) {
	int simcyc;		 // simulation clock count
	int tick;			 // each clk cycle has two ticks for two edges
	int lights = 0; // state to toggle LED lights

	Verilated::commandArgs(argc, argv);
	// init top verilog instance
	Vf1_light * top = new Vf1_light;
	// init trace dump
	Verilated::traceEverOn(true);
	VerilatedVcdC* tfp = new VerilatedVcdC;
	top->trace (tfp, 99);
	tfp->open ("f1_light.vcd");
 
	// init Vbuddy
	if (vbdOpen()!=1) return(-1);
	vbdHeader("L3T2:Clktick");
	vbdSetMode(1); // Flag mode set to one-shot

	// initialize simulation inputs
	top->clk = 1;
	top->rst = 0;
	top->en = 0;
	top->N = vbdValue();
	
	// run simulation for MAX_SIM_CYC clock cycles
	for (simcyc=0; simcyc<MAX_SIM_CYC; simcyc++) {
		// dump variables into VCD file and toggle clock
		for (tick=0; tick<2; tick++) {
			tfp->dump (2*simcyc+tick);
			top->clk = !top->clk;
			top->eval ();
		}

		vbdBar(top->data_out & 0xFF);
		// set up input signals of testbench
		top->rst = simcyc < 2; // assert reset for 1st cycle
		top->en = simcyc >= 2;
		//top->N = vbdValue();
		top->N = WAIT_CYCLES; // hard code for ~ 1 second delay
		vbdCycle(simcyc);

		if (Verilated::gotFinish())
			exit(0);
	}

	vbdClose();
	tfp->close(); 
	exit(0);
}
