# Legal Notice: (C)2020 Altera Corporation. All rights reserved.  Your
# use of Altera Corporation's design tools, logic functions and other
# software and tools, and its AMPP partner logic functions, and any
# output files any of the foregoing (including device programming or
# simulation files), and any associated documentation or information are
# expressly subject to the terms and conditions of the Altera Program
# License Subscription Agreement or other applicable license agreement,
# including, without limitation, that your use is for the sole purpose
# of programming logic devices manufactured by Altera and sold by Altera
# or its authorized distributors.  Please refer to the applicable
# agreement for further details.

#**************************************************************
# Timequest JTAG clock definition
#   Uncommenting the following lines will define the JTAG
#   clock in TimeQuest Timing Analyzer
#**************************************************************

#create_clock -period 10MHz {altera_reserved_tck}
#set_clock_groups -asynchronous -group {altera_reserved_tck}

#**************************************************************
# Set TCL Path Variables 
#**************************************************************

set 	FreeRTOS_FreeRTOSNios_cpu 	FreeRTOS_FreeRTOSNios_cpu:*
set 	FreeRTOS_FreeRTOSNios_cpu_oci 	FreeRTOS_FreeRTOSNios_cpu_nios2_oci:the_FreeRTOS_FreeRTOSNios_cpu_nios2_oci
set 	FreeRTOS_FreeRTOSNios_cpu_oci_break 	FreeRTOS_FreeRTOSNios_cpu_nios2_oci_break:the_FreeRTOS_FreeRTOSNios_cpu_nios2_oci_break
set 	FreeRTOS_FreeRTOSNios_cpu_ocimem 	FreeRTOS_FreeRTOSNios_cpu_nios2_ocimem:the_FreeRTOS_FreeRTOSNios_cpu_nios2_ocimem
set 	FreeRTOS_FreeRTOSNios_cpu_oci_debug 	FreeRTOS_FreeRTOSNios_cpu_nios2_oci_debug:the_FreeRTOS_FreeRTOSNios_cpu_nios2_oci_debug
set 	FreeRTOS_FreeRTOSNios_cpu_wrapper 	FreeRTOS_FreeRTOSNios_cpu_debug_slave_wrapper:the_FreeRTOS_FreeRTOSNios_cpu_debug_slave_wrapper
set 	FreeRTOS_FreeRTOSNios_cpu_jtag_tck 	FreeRTOS_FreeRTOSNios_cpu_debug_slave_tck:the_FreeRTOS_FreeRTOSNios_cpu_debug_slave_tck
set 	FreeRTOS_FreeRTOSNios_cpu_jtag_sysclk 	FreeRTOS_FreeRTOSNios_cpu_debug_slave_sysclk:the_FreeRTOS_FreeRTOSNios_cpu_debug_slave_sysclk
set 	FreeRTOS_FreeRTOSNios_cpu_oci_path 	 [format "%s|%s" $FreeRTOS_FreeRTOSNios_cpu $FreeRTOS_FreeRTOSNios_cpu_oci]
set 	FreeRTOS_FreeRTOSNios_cpu_oci_break_path 	 [format "%s|%s" $FreeRTOS_FreeRTOSNios_cpu_oci_path $FreeRTOS_FreeRTOSNios_cpu_oci_break]
set 	FreeRTOS_FreeRTOSNios_cpu_ocimem_path 	 [format "%s|%s" $FreeRTOS_FreeRTOSNios_cpu_oci_path $FreeRTOS_FreeRTOSNios_cpu_ocimem]
set 	FreeRTOS_FreeRTOSNios_cpu_oci_debug_path 	 [format "%s|%s" $FreeRTOS_FreeRTOSNios_cpu_oci_path $FreeRTOS_FreeRTOSNios_cpu_oci_debug]
set 	FreeRTOS_FreeRTOSNios_cpu_jtag_tck_path 	 [format "%s|%s|%s" $FreeRTOS_FreeRTOSNios_cpu_oci_path $FreeRTOS_FreeRTOSNios_cpu_wrapper $FreeRTOS_FreeRTOSNios_cpu_jtag_tck]
set 	FreeRTOS_FreeRTOSNios_cpu_jtag_sysclk_path 	 [format "%s|%s|%s" $FreeRTOS_FreeRTOSNios_cpu_oci_path $FreeRTOS_FreeRTOSNios_cpu_wrapper $FreeRTOS_FreeRTOSNios_cpu_jtag_sysclk]
set 	FreeRTOS_FreeRTOSNios_cpu_jtag_sr 	 [format "%s|*sr" $FreeRTOS_FreeRTOSNios_cpu_jtag_tck_path]

#**************************************************************
# Set False Paths
#**************************************************************

set_false_path -from [get_keepers *$FreeRTOS_FreeRTOSNios_cpu_oci_break_path|break_readreg*] -to [get_keepers *$FreeRTOS_FreeRTOSNios_cpu_jtag_sr*]
set_false_path -from [get_keepers *$FreeRTOS_FreeRTOSNios_cpu_oci_debug_path|*resetlatch]     -to [get_keepers *$FreeRTOS_FreeRTOSNios_cpu_jtag_sr[33]]
set_false_path -from [get_keepers *$FreeRTOS_FreeRTOSNios_cpu_oci_debug_path|monitor_ready]  -to [get_keepers *$FreeRTOS_FreeRTOSNios_cpu_jtag_sr[0]]
set_false_path -from [get_keepers *$FreeRTOS_FreeRTOSNios_cpu_oci_debug_path|monitor_error]  -to [get_keepers *$FreeRTOS_FreeRTOSNios_cpu_jtag_sr[34]]
set_false_path -from [get_keepers *$FreeRTOS_FreeRTOSNios_cpu_ocimem_path|*MonDReg*] -to [get_keepers *$FreeRTOS_FreeRTOSNios_cpu_jtag_sr*]
set_false_path -from *$FreeRTOS_FreeRTOSNios_cpu_jtag_sr*    -to *$FreeRTOS_FreeRTOSNios_cpu_jtag_sysclk_path|*jdo*
set_false_path -from sld_hub:*|irf_reg* -to *$FreeRTOS_FreeRTOSNios_cpu_jtag_sysclk_path|ir*
set_false_path -from sld_hub:*|sld_shadow_jsm:shadow_jsm|state[1] -to *$FreeRTOS_FreeRTOSNios_cpu_oci_debug_path|monitor_go
