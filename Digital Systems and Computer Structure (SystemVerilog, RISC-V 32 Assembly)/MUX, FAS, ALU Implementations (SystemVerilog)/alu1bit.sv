// 1-bit ALU template
module alu1bit (
    input logic a,           // Input bit a
    input logic b,           // Input bit b
    input logic cin,         // Carry in
    input logic [1:0] op,    // Operation
    output logic s,          // Output S
    output logic cout        // Carry out
);

// Put your code here
// ------------------
logic orout;
logic xnorout;
logic norout;
logic xorout;
logic fassout;
logic nop0;
OR2 #(.Tpdlh(10),.Tpdhl(10)) r (.A(a),.B(b),.Z(orout));
XNOR2 #(.Tpdlh(4),.Tpdhl(4)) xn (.A(a),.B(b),.Z(xnorout));
NAND2 #(.Tpdlh(10),.Tpdhl(10)) nor0 (.A(orout),.B(orout),.Z(norout));
NAND2 #(.Tpdlh(10),.Tpdhl(10)) xor0 (.A(xnorout),.B(xnorout),.Z(xorout));
NAND2 #(.Tpdlh(10),.Tpdhl(10)) nopz (.A(op[0]),.B(op[0]),.Z(nop0));
fas f (.a(a),.b(b),.cin(cin),.a_ns(nop0),.s(fassout),.cout(cout));
mux4 m (.d0(norout),.d1(xorout),.d2(fassout),.d3(fassout),.sel(op),.z(s));
// End of your code

endmodule
