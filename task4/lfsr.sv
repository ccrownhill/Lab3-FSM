module lfsr (
	input logic clk,
	input logic rst,

	output logic [6:0] data_out
);

logic [6:0] sreg;

always_ff @ (posedge clk) begin
	if (rst)	sreg <= {7{1'b1}};
	else		sreg <= {sreg[5:0], sreg[6] ^ sreg[2]};
end

assign data_out = sreg;

endmodule
