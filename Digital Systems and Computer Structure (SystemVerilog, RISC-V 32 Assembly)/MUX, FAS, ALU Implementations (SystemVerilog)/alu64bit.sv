// 64-bit ALU template
module alu64bit (
    input logic [63:0] a,    // Input bit a
    input logic [63:0] b,    // Input bit b
    input logic cin,         // Carry in
    input logic [1:0] op,    // Operation
    output logic [63:0] s,   // Output S
    output logic cout        // Carry out
);

// Put your code here
// ------------------
logic [63:0] couts;
genvar i;
alu1bit alu0 (.a(a[0]),.b(b[0]),.cin(cin),.op(op),.s(s[0]),.cout(couts[0]));
generate
for (i=7'd1; i<7'd64; i=i+1) begin
alu1bit i_alu (.a(a[i]),.b(b[i]),.cin(couts[i-1]),.op(op),.s(s[i]),.cout(couts[i]));
end
endgenerate
assign cout = couts[63];
// End of your code
endmodule
