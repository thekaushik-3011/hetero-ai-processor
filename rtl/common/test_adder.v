//============================================================================
// Module: test_adder
// Description: Simple 8-bit adder for toolchain verification
// Date: 2024-11-06
//============================================================================

module test_adder #(
    parameter WIDTH = 8
)(
    input  wire [WIDTH-1:0] a,
    input  wire [WIDTH-1:0] b,
    input  wire             cin,
    output wire [WIDTH-1:0] sum,
    output wire             cout
);

    assign {cout, sum} = a + b + {8'b0, cin};

endmodule
