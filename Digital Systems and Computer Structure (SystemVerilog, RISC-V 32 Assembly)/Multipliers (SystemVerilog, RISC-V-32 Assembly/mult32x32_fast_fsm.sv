// 32X32 Multiplier FSM
module mult32x32_fast_fsm (
    input logic clk,              // Clock
    input logic reset,            // Reset
    input logic start,            // Start signal
    input logic a_msb_is_0,       // Indicates MSB of operand A is 0
    input logic b_msw_is_0,       // Indicates MSW of operand B is 0
    output logic busy,            // Multiplier busy indication
    output logic [1:0] a_sel,     // Select one byte from A
    output logic b_sel,           // Select one 2-byte word from B
    output logic [2:0] shift_sel, // Select output from shifters
    output logic upd_prod,        // Update the product register
    output logic clr_prod         // Clear the product register
);

// Put your code here
// ------------------
typedef enum {a,b,c,d,e,f,g,h,i} state;
    state current_state;
    state next_state;


always_ff @(posedge clk, posedge reset) begin
        if (reset == 1'b1) begin
            current_state <= a;
        end
        else begin
            current_state <= next_state;
        end
    end

always_comb begin
busy = 1'b0;
a_sel=0'b0;
b_sel=0'b0;
shift_sel=0'b0;
upd_prod=0;
clr_prod=0;
case(current_state)
      a: begin
	     if(start==1'b1) begin
		 clr_prod =1'b1;
		 next_state=b;
		 end
		 end
	  b: begin
	     next_state=c;
	     upd_prod=1'b1;
		 busy=1'b1;
		 end
	  c: begin
	     next_state=d;
		 busy=1'b1;
		 a_sel=2'd1;
		 b_sel=1'b0;
		 shift_sel=3'd1;
		 upd_prod=1'b1;
		 end 
      d: begin if(b_msw_is_0==1'b1 && a_msb_is_0==1'b1) begin
	  next_state=a;
	  end else if(b_msw_is_0==1'b0 && a_msb_is_0==1'b1) begin
	  next_state=f;
	  end else
	  begin next_state=e; end
		 busy=1'b1;
		 a_sel=2'd2;
		 b_sel=1'b0;
		 shift_sel=3'd2;
		 upd_prod=1'b1;
		 end 
      e: begin if(b_msw_is_0==1'b1 && a_msb_is_0==1'b0) begin
	  next_state=a;
	  end else begin next_state =f; end
		 busy=1'b1;
		 a_sel=2'd3;
		 b_sel=1'b0;
		 shift_sel=3'd3;
		 upd_prod=1'b1;
		 end 
	  f: begin
	     next_state=g;
		 busy=1'b1;
		 a_sel=2'd0;
		 b_sel=1'b1;
		 shift_sel=3'd2;
		 upd_prod=1'b1;
		 end 
      g: begin
	     next_state=h;
		 busy=1'b1;
		 a_sel=2'd1;
		 b_sel=1'b1;
		 shift_sel=3'd3;
		 upd_prod=1'b1;
		 end 
	  h: begin if(b_msw_is_0==1'b0 && a_msb_is_0==1'b1) begin
	  next_state=a;
	  end else begin next_state =i; end
		 busy=1'b1;
		 a_sel=2'd2;
		 b_sel=1'b1;
		 shift_sel=3'd4;
		 upd_prod=1'b1;
		 end
       i: begin
	     next_state=a;
		 busy=1'b1;
		 a_sel=2'd3;
		 b_sel=1'b1;
		 shift_sel=3'd5;
		 upd_prod=1'b1;
		 end 		 

endcase
end

// End of your code

endmodule
