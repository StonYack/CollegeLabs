	component Midterm is
		port (
			clk_clk          : in    std_logic                     := 'X';             -- clk
			hex_1_export     : out   std_logic_vector(6 downto 0);                     -- export
			hex_0_export     : out   std_logic_vector(6 downto 0);                     -- export
			hex_2_export     : out   std_logic_vector(6 downto 0);                     -- export
			hex_3_export     : out   std_logic_vector(6 downto 0);                     -- export
			hex_4_export     : out   std_logic_vector(6 downto 0);                     -- export
			hex_5_export     : out   std_logic_vector(6 downto 0);                     -- export
			keys_export      : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			leds_export      : out   std_logic_vector(9 downto 0);                     -- export
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
			sw_export        : in    std_logic_vector(9 downto 0)  := (others => 'X')  -- export
		);
	end component Midterm;

	u0 : component Midterm
		port map (
			clk_clk          => CONNECTED_TO_clk_clk,          --        clk.clk
			hex_1_export     => CONNECTED_TO_hex_1_export,     --      hex_1.export
			hex_0_export     => CONNECTED_TO_hex_0_export,     --      hex_0.export
			hex_2_export     => CONNECTED_TO_hex_2_export,     --      hex_2.export
			hex_3_export     => CONNECTED_TO_hex_3_export,     --      hex_3.export
			hex_4_export     => CONNECTED_TO_hex_4_export,     --      hex_4.export
			hex_5_export     => CONNECTED_TO_hex_5_export,     --      hex_5.export
			keys_export      => CONNECTED_TO_keys_export,      --       keys.export
			leds_export      => CONNECTED_TO_leds_export,      --       leds.export
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
			sw_export        => CONNECTED_TO_sw_export         --         sw.export
		);

