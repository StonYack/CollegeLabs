#ifndef _ALTERA_HPS_0_H_
#define _ALTERA_HPS_0_H_

/*
 * This file was automatically generated by the swinfo2header utility.
 * 
 * Created from SOPC Builder system 'soc_system' in
 * file 'soc_system.swinfo'.
 */

/*
 * This file contains macros for module 'hps_0' and devices
 * connected to the following masters:
 *   h2f_axi_master
 *   h2f_lw_axi_master
 * 
 * Do not include this header file and another header file created for a
 * different module or master group at the same time.
 * Doing so may result in duplicate macro names.
 * Instead, use the system header file which has macros with unique names.
 */

/*
 * Macros for device 'onchip_memory2_0', class 'altera_avalon_onchip_memory2'
 * The macros are prefixed with 'ONCHIP_MEMORY2_0_'.
 * The prefix is the slave descriptor.
 */
#define ONCHIP_MEMORY2_0_COMPONENT_TYPE altera_avalon_onchip_memory2
#define ONCHIP_MEMORY2_0_COMPONENT_NAME onchip_memory2_0
#define ONCHIP_MEMORY2_0_BASE 0x0
#define ONCHIP_MEMORY2_0_SPAN 65536
#define ONCHIP_MEMORY2_0_END 0xffff
#define ONCHIP_MEMORY2_0_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEMORY2_0_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_MEMORY2_0_CONTENTS_INFO ""
#define ONCHIP_MEMORY2_0_DUAL_PORT 0
#define ONCHIP_MEMORY2_0_GUI_RAM_BLOCK_TYPE AUTO
#define ONCHIP_MEMORY2_0_INIT_CONTENTS_FILE soc_system_onchip_memory2_0
#define ONCHIP_MEMORY2_0_INIT_MEM_CONTENT 1
#define ONCHIP_MEMORY2_0_INSTANCE_ID NONE
#define ONCHIP_MEMORY2_0_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_MEMORY2_0_RAM_BLOCK_TYPE AUTO
#define ONCHIP_MEMORY2_0_READ_DURING_WRITE_MODE DONT_CARE
#define ONCHIP_MEMORY2_0_SINGLE_CLOCK_OP 0
#define ONCHIP_MEMORY2_0_SIZE_MULTIPLE 1
#define ONCHIP_MEMORY2_0_SIZE_VALUE 65536
#define ONCHIP_MEMORY2_0_WRITABLE 1
#define ONCHIP_MEMORY2_0_MEMORY_INFO_DAT_SYM_INSTALL_DIR SIM_DIR
#define ONCHIP_MEMORY2_0_MEMORY_INFO_GENERATE_DAT_SYM 1
#define ONCHIP_MEMORY2_0_MEMORY_INFO_GENERATE_HEX 1
#define ONCHIP_MEMORY2_0_MEMORY_INFO_HAS_BYTE_LANE 0
#define ONCHIP_MEMORY2_0_MEMORY_INFO_HEX_INSTALL_DIR QPF_DIR
#define ONCHIP_MEMORY2_0_MEMORY_INFO_MEM_INIT_DATA_WIDTH 64
#define ONCHIP_MEMORY2_0_MEMORY_INFO_MEM_INIT_FILENAME soc_system_onchip_memory2_0

/*
 * Macros for device 'Hex_2', class 'Hex'
 * The macros are prefixed with 'HEX_2_'.
 * The prefix is the slave descriptor.
 */
#define HEX_2_COMPONENT_TYPE Hex
#define HEX_2_COMPONENT_NAME Hex_2
#define HEX_2_BASE 0x0
#define HEX_2_SPAN 16
#define HEX_2_END 0xf

/*
 * Macros for device 'Hex_1', class 'Hex'
 * The macros are prefixed with 'HEX_1_'.
 * The prefix is the slave descriptor.
 */
#define HEX_1_COMPONENT_TYPE Hex
#define HEX_1_COMPONENT_NAME Hex_1
#define HEX_1_BASE 0x10
#define HEX_1_SPAN 16
#define HEX_1_END 0x1f

/*
 * Macros for device 'Hex_0', class 'Hex'
 * The macros are prefixed with 'HEX_0_'.
 * The prefix is the slave descriptor.
 */
#define HEX_0_COMPONENT_TYPE Hex
#define HEX_0_COMPONENT_NAME Hex_0
#define HEX_0_BASE 0x20
#define HEX_0_SPAN 16
#define HEX_0_END 0x2f

/*
 * Macros for device 'SW_0', class 'SW'
 * The macros are prefixed with 'SW_0_'.
 * The prefix is the slave descriptor.
 */
#define SW_0_COMPONENT_TYPE SW
#define SW_0_COMPONENT_NAME SW_0
#define SW_0_BASE 0x30
#define SW_0_SPAN 16
#define SW_0_END 0x3f

/*
 * Macros for device 'LED_0', class 'LED'
 * The macros are prefixed with 'LED_0_'.
 * The prefix is the slave descriptor.
 */
#define LED_0_COMPONENT_TYPE LED
#define LED_0_COMPONENT_NAME LED_0
#define LED_0_BASE 0x40
#define LED_0_SPAN 16
#define LED_0_END 0x4f

/*
 * Macros for device 'key_0', class 'key'
 * The macros are prefixed with 'KEY_0_'.
 * The prefix is the slave descriptor.
 */
#define KEY_0_COMPONENT_TYPE key
#define KEY_0_COMPONENT_NAME key_0
#define KEY_0_BASE 0x50
#define KEY_0_SPAN 16
#define KEY_0_END 0x5f

/*
 * Macros for device 'fib_0', class 'fib'
 * The macros are prefixed with 'FIB_0_'.
 * The prefix is the slave descriptor.
 */
#define FIB_0_COMPONENT_TYPE fib
#define FIB_0_COMPONENT_NAME fib_0
#define FIB_0_BASE 0x60
#define FIB_0_SPAN 16
#define FIB_0_END 0x6f

/*
 * Macros for device 'sysid_qsys', class 'altera_avalon_sysid_qsys'
 * The macros are prefixed with 'SYSID_QSYS_'.
 * The prefix is the slave descriptor.
 */
#define SYSID_QSYS_COMPONENT_TYPE altera_avalon_sysid_qsys
#define SYSID_QSYS_COMPONENT_NAME sysid_qsys
#define SYSID_QSYS_BASE 0x10000
#define SYSID_QSYS_SPAN 8
#define SYSID_QSYS_END 0x10007
#define SYSID_QSYS_ID 2899645186
#define SYSID_QSYS_TIMESTAMP 1607221441

/*
 * Macros for device 'jtag_uart', class 'altera_avalon_jtag_uart'
 * The macros are prefixed with 'JTAG_UART_'.
 * The prefix is the slave descriptor.
 */
#define JTAG_UART_COMPONENT_TYPE altera_avalon_jtag_uart
#define JTAG_UART_COMPONENT_NAME jtag_uart
#define JTAG_UART_BASE 0x20000
#define JTAG_UART_SPAN 8
#define JTAG_UART_END 0x20007
#define JTAG_UART_IRQ 2
#define JTAG_UART_READ_DEPTH 64
#define JTAG_UART_READ_THRESHOLD 8
#define JTAG_UART_WRITE_DEPTH 64
#define JTAG_UART_WRITE_THRESHOLD 8


#endif /* _ALTERA_HPS_0_H_ */
