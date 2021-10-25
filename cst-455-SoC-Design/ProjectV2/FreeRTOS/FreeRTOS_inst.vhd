	component FreeRTOS is
		port (
			clk_clk          : in    std_logic                     := 'X';             -- clk
			hex_0_export     : out   std_logic_vector(6 downto 0);                     -- export
			piezo_0_export   : out   std_logic_vector(5 downto 0);                     -- export
			sdram_clk_clk    : out   std_logic;                                        -- clk
			sdram_wire_addr  : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_wire_ba    : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_wire_cas_n : out   std_logic;                                        -- cas_n
			sdram_wire_cke   : out   std_logic;                                        -- cke
			sdram_wire_cs_n  : out   std_logic;                                        -- cs_n
			sdram_wire_dq    : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			sdram_wire_dqm   : out   std_logic_vector(1 downto 0);                     -- dqm
			sdram_wire_ras_n : out   std_logic;                                        -- ras_n
			sdram_wire_we_n  : out   std_logic;                                        -- we_n
			servo_0_export   : out   std_logic_vector(5 downto 0);                     -- export
			servo_1_export   : out   std_logic_vector(5 downto 0);                     -- export
			sw_0_export      : in    std_logic_vector(3 downto 0)  := (others => 'X')  -- export
		);
	end component FreeRTOS;

	u0 : component FreeRTOS
		port map (
			clk_clk          => CONNECTED_TO_clk_clk,          --        clk.clk
			hex_0_export     => CONNECTED_TO_hex_0_export,     --      hex_0.export
			piezo_0_export   => CONNECTED_TO_piezo_0_export,   --    piezo_0.export
			sdram_clk_clk    => CONNECTED_TO_sdram_clk_clk,    --  sdram_clk.clk
			sdram_wire_addr  => CONNECTED_TO_sdram_wire_addr,  -- sdram_wire.addr
			sdram_wire_ba    => CONNECTED_TO_sdram_wire_ba,    --           .ba
			sdram_wire_cas_n => CONNECTED_TO_sdram_wire_cas_n, --           .cas_n
			sdram_wire_cke   => CONNECTED_TO_sdram_wire_cke,   --           .cke
			sdram_wire_cs_n  => CONNECTED_TO_sdram_wire_cs_n,  --           .cs_n
			sdram_wire_dq    => CONNECTED_TO_sdram_wire_dq,    --           .dq
			sdram_wire_dqm   => CONNECTED_TO_sdram_wire_dqm,   --           .dqm
			sdram_wire_ras_n => CONNECTED_TO_sdram_wire_ras_n, --           .ras_n
			sdram_wire_we_n  => CONNECTED_TO_sdram_wire_we_n,  --           .we_n
			servo_0_export   => CONNECTED_TO_servo_0_export,   --    servo_0.export
			servo_1_export   => CONNECTED_TO_servo_1_export,   --    servo_1.export
			sw_0_export      => CONNECTED_TO_sw_0_export       --       sw_0.export
		);

