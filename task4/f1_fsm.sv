module f1_fsm (
	input logic en,
	input logic rst,
	input logic clk,
	input logic trigger,

	output logic cmd_seq,
	output logic cmd_delay,
	output logic [7:0] data_out
);

typedef enum {IDLE, TRIG, S1, S2, S3, S4, S5, S6, S7, S8} my_state;
my_state current_state, next_state;

// state transition
always_ff @ (posedge clk) begin
	if (rst) begin
		current_state <= IDLE;
		data_out <= 8'b0;
	end
	else if (trigger) begin
		current_state <= TRIG;
		data_out <= 8'b0;
	end
	else if (en) begin
		current_state <= next_state;
		if (data_out == 8'hff)
			data_out <= 8'b0;
		else
			data_out <= (data_out << 1) + 8'b1;
	end
end

// next state logic
always_comb
	case (current_state)
		IDLE:	next_state = IDLE;
		TRIG:	next_state = S1;
		S1:	next_state = S2;
		S2:	next_state = S3;
		S3:	next_state = S4;
		S4:	next_state = S5;
		S5:	next_state = S6;
		S6:	next_state = S7;
		S7:	next_state = S8;
		S8:	next_state = IDLE;
	endcase

// output logic
always_comb
	case (current_state)
		IDLE:	begin
			cmd_seq = 1'b0;
			cmd_delay = 1'b0;
		end
		S8:	begin
			cmd_seq = 1'b0;
			cmd_delay = 1'b1;
		end

		default:begin
			cmd_seq = 1'b1;
			cmd_delay = 1'b0;
		end
	endcase

endmodule
