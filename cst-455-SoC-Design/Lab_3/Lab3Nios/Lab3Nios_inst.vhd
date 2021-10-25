	component Lab3Nios is
		port (
			clk_clk         : in  std_logic                     := 'X';             -- clk
			hex_export      : out std_logic_vector(20 downto 0);                    -- export
			switches_export : in  std_logic_vector(9 downto 0)  := (others => 'X')  -- export
		);
	end component Lab3Nios;

	u0 : component Lab3Nios
		port map (
			clk_clk         => CONNECTED_TO_clk_clk,         --      clk.clk
			hex_export      => CONNECTED_TO_hex_export,      --      hex.export
			switches_export => CONNECTED_TO_switches_export  -- switches.export
		);

