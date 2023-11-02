module lfsr #(
	parameter BITS = 7
)(
	input logic clk,
	input logic en,
	input logic rst,

	output logic [BITS-1:0] data_out
);

logic [BITS-1:0] sreg;

always_ff @ (posedge clk, posedge rst) begin
	if (rst)	sreg <= {BITS{1'b1}};
	else if (en)	sreg <= {sreg[BITS-2:0], sreg[6] ^ sreg[2]};
end

assign data_out = sreg;

endmodule
