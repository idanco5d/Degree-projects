// 32X32 Multiplier test template
module mult32x32_test;

    logic clk;            // Clock
    logic reset;          // Reset
    logic start;          // Start signal
    logic [31:0] a;       // Input a
    logic [31:0] b;       // Input b
    logic busy;           // Multiplier busy indication
    logic [63:0] product; // Miltiplication product

// Put your code here
// ------------------
   mult32x32 test_inst(
        .clk(clk),
        .reset(reset),
        .start(start), 
        .a(a), 
        .b(b), 
        .busy(busy),
        .product(product)
     );
	 
 
	 initial begin
	 clk=1'b1;
	 a=32'd0;
	 b=32'd0;
	 reset=1'b1;
	 start=1'b0;
	 #80
	 reset=1'b0;
	 #20
	 a=32'd204701411;
	 b=32'd206489338;
	 #20
	 start=1'b1;
	 #20 
	 start=1'b0;
	 
	 end
	 always begin
       #10
	   clk = ~clk;
    end 
// End of your code

endmodule
