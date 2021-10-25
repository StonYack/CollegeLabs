
module FreeRTOS (
	clk_clk,
	hex_0_export,
	piezo_0_export,
	sdram_clk_clk,
	sdram_wire_addr,
	sdram_wire_ba,
	sdram_wire_cas_n,
	sdram_wire_cke,
	sdram_wire_cs_n,
	sdram_wire_dq,
	sdram_wire_dqm,
	sdram_wire_ras_n,
	sdram_wire_we_n,
	servo_0_export,
	servo_1_export,
	sw_0_export);	

	input		clk_clk;
	output	[6:0]	hex_0_export;
	output	[5:0]	piezo_0_export;
	output		sdram_clk_clk;
	output	[12:0]	sdram_wire_addr;
	output	[1:0]	sdram_wire_ba;
	output		sdram_wire_cas_n;
	output		sdram_wire_cke;
	output		sdram_wire_cs_n;
	inout	[15:0]	sdram_wire_dq;
	output	[1:0]	sdram_wire_dqm;
	output		sdram_wire_ras_n;
	output		sdram_wire_we_n;
	output	[5:0]	servo_0_export;
	output	[5:0]	servo_1_export;
	input	[3:0]	sw_0_export;
endmodule
