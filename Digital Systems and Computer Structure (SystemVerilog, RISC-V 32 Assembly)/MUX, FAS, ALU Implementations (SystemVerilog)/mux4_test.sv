// 4->1 multiplexer test bench template
module mux4_test;

// Put your code here
// ------------------
       logic d1;  //Data input 1
	   logic d0;  //Data input 0
	   logic d2;  //Data input 2
	   logic d3;  //Data input 3
	   logic [1:0] sel; //select input
	   logic z;
	  
	mux4 uut(
	    .d0(d0),
		.d1(d1),
		.d2(d2),
		.d3(d3),
		.sel(sel),
		.z(z)
	);
	
	initial begin
	sel={1'b0,1'b0};
	    d0=1'b0;
		d1=1'b0;
		d2=1'b0;
		d3=1'b0;
		
		#100
		sel={1'b0,1'b0};
	    d0=1'b1;
		d1=1'b0;
		d2=1'b0;
		d3=1'b0;
	      
		#100
		sel={1'b0,1'b0};
	    d0=1'b0;
		d1=1'b0;
		d2=1'b0;
		d3=1'b0;
    end
// End of your code
endmodule
