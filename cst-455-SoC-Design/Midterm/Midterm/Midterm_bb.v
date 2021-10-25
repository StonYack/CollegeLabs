
module Midterm (
	clk_clk,
	hex_1_export,
	hex_0_export,
	hex_2_export,
	hex_3_export,
	hex_4_export,
	hex_5_export,
	keys_export,
	leds_export,
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
	sw_export);	

	input		clk_clk;
	output	[6:0]	hex_1_export;
	output	[6:0]	hex_0_export;
	output	[6:0]	hex_2_export;
	output	[6:0]	hex_3_export;
	output	[6:0]	hex_4_export;
	output	[6:0]	hex_5_export;
	input	[3:0]	keys_export;
	output	[9:0]	leds_export;
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
	input	[9:0]	sw_export;
endmodule
