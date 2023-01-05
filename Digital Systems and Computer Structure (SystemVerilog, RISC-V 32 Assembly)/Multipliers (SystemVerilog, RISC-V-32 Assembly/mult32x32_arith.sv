// 32X32 Multiplier arithmetic unit template
module mult32x32_arith (
    input logic clk,             // Clock
    input logic reset,           // Reset
    input logic [31:0] a,        // Input a
    input logic [31:0] b,        // Input b
    input logic [1:0] a_sel,     // Select one byte from A
    input logic b_sel,           // Select one 2-byte word from B
    input logic [2:0] shift_sel, // Select output from shifters
    input logic upd_prod,        // Update the product register
    input logic clr_prod,        // Clear the product register
    output logic [63:0] product  // Miltiplication product
);

// Put your code here
// ------------------
 logic [7:0] a_out;
 logic [15:0] b_out;
 logic [63:0] mux8;
 logic [63:0] shifter0;
 logic [63:0] shifter1;
 logic [63:0] shifter2;  
 logic [63:0] shifter3;
 logic [63:0] shifter4;
 logic [63:0] shifter5;
 logic [23:0] multer;
 
 
 
 
 //mux A
always_comb begin
        case (a_sel)
            2'd0: a_out = a[7:0];
            2'd1: a_out = a[15:8];
            2'd2: a_out = a[23:16];
            2'd3: a_out = a[31:24];
        endcase
    end 
	 //mux B
always_comb begin
        case (b_sel)
            1'd0: b_out = b[15:0];
            1'd1: b_out = b[31:16];

        endcase
    end 	
	
	
	always_comb begin
 multer = a_out*b_out;
 end
 
 	always_comb begin
 
 shifter0 = {{40{1'b0}},multer[23:0]};
 shifter1 = {{32{1'b0}},multer[23:0],{8{1'b0}}};
 shifter2 = {{24{1'b0}},multer[23:0],{16{1'b0}}}; 
 shifter3 = {{16{1'b0}},multer[23:0],{24{1'b0}}};
 shifter4 = {{8{1'b0}},multer[23:0],{32{1'b0}}};
 shifter5 = {multer[23:0],{40{1'b0}}};
 end
	
	//mux C
always_comb begin
        case (shift_sel)
            3'd0: mux8 = shifter0;
            3'd1: mux8 = shifter1;
            3'd2: mux8 = shifter2;
            3'd3: mux8 = shifter3;
			3'd4: mux8 = shifter4;
            3'd5: mux8 = shifter5;
        endcase
    end 	
    

	

always_ff @(posedge clk, posedge reset) begin
        if (reset == 1'b1 || clr_prod==1'b1) begin
            product <=0;
        end
		else 
		if(upd_prod==1'b1) begin
		product<=product + mux8;
		 end
        end




// End of your code

endmodule
