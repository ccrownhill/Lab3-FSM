module f1_light #(
	parameter WAIT_CYCLES = 33
)(
	input logic clk,
	input logic rst,
	input logic trigger,

	output logic [7:0] data_out
);

logic cmd_seq, cmd_delay, tick, time_out;
logic [6:0] K;

clktick my_clktick (
	.N (WAIT_CYCLES),
	.en (cmd_seq),
	.rst (rst),
	.clk (clk),
	.tick (tick)
);

lfsr my_lfsr (
	.clk (clk),
	.rst (rst),
	.data_out (K)
);

delay #(7) my_delay (
	.clk (clk),
	.rst (rst),
	.trigger (cmd_delay),
	.n (K),
	.time_out (time_out)
);

f1_fsm my_f1_fsm (
	.clk (clk),
	.rst (rst),
	.en ((cmd_seq == 1'b1) ? tick : time_out),
	.trigger (trigger),

	.cmd_seq (cmd_seq),
	.cmd_delay (cmd_delay),
	.data_out (data_out)
);

endmodule
