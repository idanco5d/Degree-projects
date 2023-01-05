// 4->1 multiplexer template
module mux4 (
    input logic d0,          // Data input 0
    input logic d1,          // Data input 1
    input logic d2,          // Data input 2
    input logic d3,          // Data input 3
    input logic [1:0] sel,   // Select input
    output logic z           // Output
);

// Put your code here
// ------------------
logic m0out;
logic m1out;
mux2 m0 (.d0(d0),.d1(d1),.sel(sel[0]),.z(m0out));
mux2 m1 (.d0(d2),.d1(d3),.sel(sel[0]),.z(m1out));
mux2 m2 (.d0(m0out),.d1(m1out),.sel(sel[1]),.z(z));
// End of your code

endmodule
