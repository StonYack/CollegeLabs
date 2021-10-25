library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.decode.all;

ENTITY TOP IS
PORT(
SW: IN STD_LOGIC_VECTOR (7 downto 0);
HEX0 : OUT STD_LOGIC_VECTOR (6 downto 0);
HEX1 : OUT STD_LOGIC_VECTOR (6 downto 0);
HEX2 : OUT STD_LOGIC_VECTOR (6 downto 0);
HEX4 : OUT STD_LOGIC_VECTOR (6 downto 0);
KEY  : IN  STD_LOGIC_VECTOR (3 downto 0);
CLOCK_50: IN STD_LOGIC
);
END TOP;

architecture Main of TOP is
signal Num1: integer range 0 to 15 := 0;
signal Num2: integer range 0 to 15 := 0;
signal Num3: integer range 0 to 31 := 0;
signal fin: STD_LOGIC_VECTOR (13 downto 0);
begin
	Num1 <= to_integer(unsigned(SW(7 downto 4)));
	Num2 <= to_integer(unsigned(SW(3 downto 0)));
	HEX4 <= sev_seg(Num1);
	HEX2 <= sev_seg(Num2);
	
	Num3 <= Num1 + Num2;
	fin <= final(Num3);
	
	HEX0 <= fin(6 downto 0);
	HEX1 <= fin(13 downto 7);
end Main;