// Full Adder/Subtractor template
module fas (
    input logic a,           // Input bit a
    input logic b,           // Input bit b
    input logic cin,         // Carry in
    input logic a_ns,        // A_nS (add/not subtract) control
    output logic s,          // Output S
    output logic cout        // Carry out
);

// Put your code here
// ------------------
logic xnors0out;
logic xnorc0out;
logic orcout;
logic nandc0out;
logic nandc1out;

XNOR2 #(.Tpdlh(4),.Tpdhl(4)) xs0 (.A(a),.B(b),.Z(xnors0out));
XNOR2 #(.Tpdlh(4),.Tpdhl(4)) xs1 (.A(cin),.B(xnors0out),.Z(s));
XNOR2 #(.Tpdlh(4),.Tpdhl(4)) xc0 (.A(a), .B(a_ns),.Z(xnorc0out));
OR2 #(.Tpdlh(10),.Tpdhl(10)) rs0 (.A(cin), .B(xnorc0out), .Z(orcout));
NAND2 #(.Tpdlh(10),.Tpdhl(10)) nd0 (.A(b),.B(orcout), .Z(nandc0out));
NAND2 #(.Tpdlh(10),.Tpdhl(10)) nd1 (.A(cin),.B(xnorc0out),.Z(nandc1out));
NAND2 #(.Tpdlh(10),.Tpdhl(10)) nd2 (.A(nandc1out),.B(nandc0out),.Z(cout));
// End of your code

endmodule
