module f1_light #(
	parameter NUM_LIGHTS = 8,
		  WAIT_TICK_BITS = 16
)(
	input logic en,
	input logic clk,
	input logic rst,

	input [WAIT_TICK_BITS-1:0] N,

	output logic [NUM_LIGHTS-1:0] data_out
);

logic light_en;

clktick #(WAIT_TICK_BITS) my_clktick (
	.clk (clk),
	.rst (rst),
	.en (en),
	.N (N),
	.tick (light_en)
);

f1_fsm #(NUM_LIGHTS) my_f1_fsm (
	.en (light_en),
	.rst (rst),
	.clk (clk),
	.data_out (data_out)
);

endmodule
