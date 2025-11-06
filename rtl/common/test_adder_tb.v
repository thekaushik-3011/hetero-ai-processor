//============================================================================
// Testbench: test_adder_tb
// Description: Verification testbench for test_adder
//============================================================================

`timescale 1ns/1ps

module test_adder_tb;

    reg  [7:0] a, b;
    reg        cin;
    wire [7:0] sum;
    wire       cout;
    
    // Instantiate DUT
    test_adder #(.WIDTH(8)) dut (
        .a(a),
        .b(b),
        .cin(cin),
        .sum(sum),
        .cout(cout)
    );
    
    initial begin
        $dumpfile("test_adder.vcd");
        $dumpvars(0, test_adder_tb);
        
        // Test cases
        a = 8'd10; b = 8'd20; cin = 0; #10;
        $display("Test 1: %d + %d + %d = %d (cout=%b)", a, b, cin, sum, cout);
        
        a = 8'd100; b = 8'd200; cin = 0; #10;
        $display("Test 2: %d + %d + %d = %d (cout=%b)", a, b, cin, sum, cout);
        
        a = 8'd255; b = 8'd1; cin = 0; #10;
        $display("Test 3: %d + %d + %d = %d (cout=%b)", a, b, cin, sum, cout);
        
        a = 8'd128; b = 8'd127; cin = 1; #10;
        $display("Test 4: %d + %d + %d = %d (cout=%b)", a, b, cin, sum, cout);
        
        $display("\nAll tests completed successfully!");
        $finish;
    end

endmodule
