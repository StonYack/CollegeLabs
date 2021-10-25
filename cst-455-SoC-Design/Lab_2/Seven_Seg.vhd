library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package decode is
	function sev_seg (A:integer) return STD_LOGIC_VECTOR;
	function final (A: integer) return STD_LOGIC_VECTOR;
end decode;

package body decode is
	function sev_seg (A:integer) return STD_LOGIC_VECTOR IS
	variable result: STD_LOGIC_VECTOR (6 downto 0);
	begin
		case (A) is
			when 0 => result := "1000000";
			when 1 => result := "1111001";
			when 2 => result := "0100100";
			when 3 => result := "0110000";
			when 4 => result := "0011001";
			when 5 => result := "0010010";
			when 6 => result := "0000010";
			when 7 => result := "1111000";
			when 8 => result := "0000000";
			when 9 => result := "0011000";
			when 10 => result := "0001000";
			when 11 => result := "0000011";
			when 12 => result := "1000110";
			when 13 => result := "0100001";
			when 14 => result := "0000110";
			when 15 => result := "0001110";
			when others => result := "1000000";
		end case;
	return result;
	end sev_seg;
	
	function final (A: integer) return STD_LOGIC_VECTOR is
	variable fin: STD_LOGIC_VECTOR (13 downto 0);
	begin
		case (A) is
		when 0 => fin := "10000001000000";
		when 1 => fin := "10000001111001";
		when 2 => fin := "10000000100100";
		when 3 => fin := "10000000110000";
		when 4 => fin := "10000000011001";
		when 5 => fin := "10000000010010";
		when 6 => fin := "10000000000010";
		when 7 => fin := "10000001111000";
		when 8 => fin := "10000000000000";
		when 9 => fin := "10000000011000";
		when 10 => fin := "10000000001000";
		when 11 => fin := "10000000000011";
		when 12 => fin := "10000001000110";
		when 13 => fin := "10000000100001";
		when 14 => fin := "10000000000110";
		when 15 => fin := "10000000001110";	-- F
		when 16 => fin := "11110011000000";	-- 10
		when 17 => fin := "11110011111001";	-- 11
		when 18 => fin := "11110010100100";	-- 12
		when 19 => fin := "11110010110000";	--	13
		when 20 => fin := "11110010011001";	-- 14
		when 21 => fin := "11110010010010";	-- 15
		when 22 => fin := "11110010000010";	-- 16
		when 23 => fin := "11110011111000";	-- 17
		when 24 => fin := "11110010000000";	-- 18
		when 25 => fin := "11110010011000";	-- 19
		when 26 => fin := "11110010001000";	-- 1A
		when 27 => fin := "11110010000011";	-- 1B
		when 28 => fin := "11110011000110";	-- 1C
		when 29 => fin := "11110010100001";	-- 1D
		when 30 => fin := "11110010000110";	-- 1E
		when 31 => fin := "11110010001110"; -- 1F
		when others => fin := "10000001000000";
		end case;
	return fin;
	end final;		
end decode;