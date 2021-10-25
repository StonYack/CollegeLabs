module Fib(
// inputs:
	address,
	chipselect,
	clk,
	reset_n,
	write,
	read,
	writedata,

// outputs:
	readdata);

	output[31:0] readdata;
	input[1:0] address;
	input chipselect;
	input clk;
	input reset_n;
	input write;
	input read;
	input[31:0] writedata;

	wire clk_en;

	reg [31:0] num1;
	reg [31:0] num2;
	reg [31:0] nextNum;
	
	reg [31:0] InitVal;
	reg Go;
	reg done;
	reg [31:0] result;
	
	always @(posedge clk or negedge reset_n)
		begin	
			if(reset_n == 0)
			begin	
				InitVal = 0;
				Go = 0;
				done = 0;
				result = 0;
			end	
			else if(chipselect && write && ~Go)
			begin	
				InitVal = writedata;
				Go = 1;
				num1 = 0;
				num2 = 1;
				nextNum = 0;
				done = 0;
				result = 1;
			end
			else if (nextNum >= InitVal)
			begin	
				done = 1;
				Go = 0;
			end
			else if (Go && !done)
			begin
				num1 = num2;
				num2 = nextNum;
				nextNum = num1 + num2;
				result = result + 1;
			end
		end
  assign readdata = result;

endmodule