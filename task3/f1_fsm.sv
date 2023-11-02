module f1_fsm #(
	parameter NUM_LIGHTS = 8
)(
	input logic en,
	input logic rst,
	input logic clk,

	output logic [NUM_LIGHTS-1:0] data_out
);

typedef enum {IDLE, S1, S2, S3, S4, S5, S6, S7, S8} my_state;
my_state current_state, next_state;

always_ff @ (posedge clk) begin
	if (rst) begin
		data_out <= {NUM_LIGHTS{1'b0}};
		current_state <= IDLE;
	end
	else if (en) begin
		current_state <= next_state;
		if (data_out == {NUM_LIGHTS{1'b1}})
			data_out <=  {NUM_LIGHTS{1'b0}};
		else
			data_out <= (data_out << 1) + 1'b1;
	end
end

// next state logic
always_comb
	case (current_state)
		IDLE:	next_state = S1;
		S1:	next_state = S2;
		S2:	next_state = S3;
		S3:	next_state = S4;
		S4:	next_state = S5;
		S5:	next_state = S6;
		S6:	next_state = S7;
		S7:	next_state = S8;
		S8:	next_state = IDLE;
	endcase

endmodule
