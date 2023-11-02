#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vf1_fsm.h"

#include "vbuddy.cpp"

#define MAX_SIM_CYC 1000000

int main(int argc, char* argv[]) {
	int simcyc, tick;

	Verilated::commandArgs(argc, argv);
	Vf1_fsm* top = new Vf1_fsm;
	Verilated::traceEverOn(true);

	if (vbdOpen() != 1)
		return 1;
	vbdHeader("L2T1: sreg");
	vbdSetMode(1); // one shot mode

	top->clk = 1;
	top->rst = 1; // reset for simcyc < 2
	top->en = 0;

	for (simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++) {
		for (tick = 0; tick < 2; tick++) {
			top->clk = !top->clk;
			top->eval();
		}

		top->rst = simcyc < 2;

		top->en = vbdFlag();

		vbdBar(top->data_out & 0xFF);

		vbdCycle(simcyc);

		if (Verilated::gotFinish()) {
			vbdClose();
			exit(0);
		}
	}

	vbdClose();
}
