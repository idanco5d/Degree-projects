// Full Adder/Subtractor test bench template
module fas_test;

// Put your code here
// ------------------

logic a;
logic b;
logic cin;
logic s;
logic cout;
logic a_ns;

fas uut(
.a(a),
.b(b),
.cin(cin),
.a_ns(a_ns),
.s(s),
.cout(cout)
);

initial begin
       a=1'b0;
       b=1'b1;
       cin=1'b0;
       a_ns=1'b0;
      
       #100
       a=1'b0;
       b=1'b1;
       cin=1'b0;
       a_ns=1'b1;
    
       #100
       a=1'b0;
       b=1'b1;
       cin=1'b0;
       a_ns=1'b0;
     
end     

// End of your code

endmodule
