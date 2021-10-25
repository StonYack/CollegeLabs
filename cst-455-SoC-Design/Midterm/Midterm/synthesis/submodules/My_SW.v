module My_SW(
	address,
	clk,
	reset_n,
	SW_in,
	
    readdata);
	
	output[31:0] readdata;
	input[1:0] address;
	input clk;
	input reset_n;
	input[9:0] SW_in;
	
	wire clk_en;
	wire[9:0] data_in;
	wire[31:0] read_mux_out;
	reg[31:0] readdata;
	
	assign clk_en = 1;
	
	assign read_mux_out ={10{(address ==0)}}& data_in;
	
	always@(posedge clk or negedge reset_n)
	begin
		if(reset_n == 0) 
			readdata <= 0;
		else if(clk_en)  
			readdata <= {32'b0 | read_mux_out};
	end
		
	assign data_in = SW_in;
		
	endmodule