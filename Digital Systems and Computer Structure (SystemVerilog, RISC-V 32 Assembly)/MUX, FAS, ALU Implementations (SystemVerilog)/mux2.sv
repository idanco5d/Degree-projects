// 2->1 multiplexer template
module mux2 (
    input logic d0,          // Data input 0
    input logic d1,          // Data input 1
    input logic sel,         // Select input
    output logic z           // Output
);

// Put your code here
// ------------------
logic xnorina;
logic xnorinb;
logic nsel;
OR2 #(.Tpdlh(10),.Tpdhl(10)) or0(.A(d0),.B(sel),.Z(xnorina));
NAND2 #(.Tpdlh(10),.Tpdhl(10)) n0 (.A(sel),.B(sel),.Z(nsel));
OR2 #(.Tpdlh(10),.Tpdhl(10)) or1(.A(nsel), .B(d1), .Z(xnorinb));
XNOR2 #(.Tpdlh(4),.Tpdhl(4)) xnor0(.A(xnorina),.B(xnorinb),.Z(z));
// End of your code

endmodule
