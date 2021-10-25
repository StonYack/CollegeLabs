module My_Key(
	address,
	clk,
	reset_n,
	Key_in,
	
    readdata);
	
	output[31:0] readdata;
	input[1:0] address;
	input clk;
	input reset_n;
	input[3:0] Key_in;
	
	wire clk_en;
	wire[3:0] data_in;
	wire[31:0] read_mux_out;
	reg[31:0] readdata;
	
	assign clk_en = 1;
	
	assign read_mux_out ={4 {(address ==0)}}& data_in;
	
	always@(posedge clk or negedge reset_n)
	begin
		if(reset_n == 0) 
			readdata <= 0;
		else if(clk_en)  
			readdata <= {32'b0 | read_mux_out};
	end
		
	assign data_in = Key_in;
		
	endmodule