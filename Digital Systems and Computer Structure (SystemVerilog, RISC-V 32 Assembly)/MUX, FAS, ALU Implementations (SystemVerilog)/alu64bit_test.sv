// 64-bit ALU test bench template
module alu64bit_test;

// Put your code here
// ------------------
logic [63:0] a;
logic [63:0] b;
logic cin;
logic [1:0] op;
logic [63:0] s;
logic  cout;

alu64bit uut(
.a(a),
.b(b),
.cin(cin),
.op(op),
.cout(cout),
.s(s)
);

initial begin
a={64{1'b0}};
b={64{1'b1}};
op[0]=1'b0;
op[1]=1'b1;
cin=1'b0; 

         

#3000
a={64{1'b0}};
b={64{1'b1}};
op[0]=1'b0;
op[1]=1'b1;
cin=1'b1; 

                

#3000
a={64{1'b0}};
b={64{1'b1}};
op[0]=1'b0;
op[1]=1'b1;
cin=1'b0;


end

// End of your code

endmodule
