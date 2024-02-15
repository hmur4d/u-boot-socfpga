/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  Copyright (C) 2020 REFLEX CES <www.reflexces.com>
 */

#ifndef __CONFIG_SOCFPGA_ARRIA10_ACHILLES_V2_LITE_H__
#define __CONFIG_SOCFPGA_ARRIA10_ACHILLES_V2_LITE_H__

#include <asm/arch/base_addr_a10.h>

/* Booting Linux */
#define CONFIG_LOADADDR		0x01000000
#define CONFIG_SYS_LOAD_ADDR	CONFIG_LOADADDR

#define CONFIG_SYS_BOOTM_LEN	(32 * 1024 * 1024)

/*
 * U-Boot general configurations
 */

/* Memory configurations  */
#define PHYS_SDRAM_1_SIZE		0x80000000

/* Ethernet on SoC (EMAC) */

/*
 * U-Boot environment configurations
 */
//#define RXC_CONFIG_BOOT_FROM_MMC
//#define RXC_CONFIG_BOOT_FROM_NET
/*
 * Serial / UART configurations
 */
#define CONFIG_SYS_NS16550_MEM32
#define CONFIG_SYS_BAUDRATE_TABLE {4800, 9600, 19200, 38400, 57600, 115200}

/*
 * L4 OSC1 Timer 0
 */
/* reload value when timer count to zero */
#define TIMER_LOAD_VAL			0xFFFFFFFF

/*
 * Flash configurations
 */
#define CONFIG_SYS_MAX_FLASH_BANKS     1

/* SPL memory allocation configuration, this is for FAT implementation */
#define CONFIG_SYS_SPL_MALLOC_SIZE	0x00015000

/* QSPI BOOT not tested; NAND BOOT not available on Achilles */
#if defined(CONFIG_QSPI_BOOT) || defined(CONFIG_NAND_BOOT)
#define CONFIG_EXTRA_ENV_SETTINGS \
	"fdtfile=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"bootm_size=0xa000000\0" \
	"kernel_addr_r="__stringify(CONFIG_SYS_LOAD_ADDR)"\0" \
	"fdt_addr_r=0x02000000\0" \
	"scriptaddr=0x02100000\0" \
	"pxefile_addr_r=0x02200000\0" \
	"ramdisk_addr_r=0x02300000\0" \
	"socfpga_legacy_reset_compat=1\0" \
	"kernelfit_addr=0x1200000\0" \
	"fitimagesize=0x5F0000\0" \
	"qspiroot=/dev/mtdblock1\0" \
	"qspirootfstype=jffs2\0" \
	"qspiload=sf probe; sf read ${scriptaddr} ${kernelfit_addr}\0" \
	"qspiboot=setenv bootargs " CONFIG_BOOTARGS \
			"root=${qspiroot} rw rootfstype=${qspirootfstype}; " \
			"bootm ${scriptaddr}\0" \
	"nandroot=/dev/mtdblock1\0" \
	"nandrootfstype=jffs2\0" \
	"nandload=nand read ${scriptaddr} ${kernelfit_addr}\0" \
	"nandboot=setenv bootargs " CONFIG_BOOTARGS \
			"root=${nandroot} rw rootfstype=${nandrootfstype}; " \
			"bootm ${scriptaddr}\0" \

#endif

#if defined(RXC_CONFIG_BOOT_FROM_MMC) && defined(RXC_CONFIG_BOOT_FROM_NET)
#define CONFIG_EXTRA_ENV_SETTINGS \
	"fdtfile=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"mmcloadpart=2\0" \
	"mmcroot=/dev/mmcblk0p3\0" \
	"loadaddr=" __stringify(CONFIG_SYS_LOAD_ADDR) "\0" \
	"bootfile=Image\0" \
	"fdtaddr=8000000\0" \
	"mmcroot=/dev/mmcblk0p2\0" \
	"mmcboot=setenv bootargs " CONFIG_BOOTARGS \
		" root=${mmcroot} rw rootwait;" \
		"bootz ${loadaddr} - ${fdt_addr}\0" \
	"mmcload=mmc rescan;" \
		"load mmc 0:${mmcloadpart} ${loadaddr} ${bootfile};" \
		"load mmc 0:${mmcloadpart} ${fdtaddr} ${fdtfile}\0" \
	\
	"ethaddr=00:07:ed:00:64:04\0" \
	"ipaddr=192.168.1.155\0" \
	"serverip=192.168.1.200\0" \
	"tftppath=achilles\0" \
	"nfspath=/srv/nfs/achilles\0" \
	"loadfdt=tftp ${fdtaddr} ${tftppath}/${fdtfile}\0" \
	"loadbootfile= tftp ${loadaddr} ${tftppath}/${bootfile}\0" \
	"netboot=setenv bootargs " CONFIG_BOOTARGS \
		"ip=dhcp root=/dev/nfs rw rootwait nfsroot=${serverip}:${nfspath},nolock,rsize=4096,wsize=4096;" \
		"tftp ${fdtaddr} ${tftppath}/${fdtfile}; " \
		"tftp ${loadaddr} ${tftppath}/${bootfile}; " \
		"bootz ${loadaddr} - ${fdtaddr}\0" \
	"initramfs-dir=initramfs-achilles\0" \
	"initramfs-boot=setenv bootargs earlycon debug ignore_loglevel " \
		"console=ttyS0,115200 root=/dev/ram rw; " \
		"tftp ${loadaddr} ${initramfs-dir}/${bootfile}; " \
		"tftp ${fdtaddr} ${initramfs-dir}/${fdtfile}; " \
		"bootz ${loadaddr} - ${fdtaddr}\0" \
	\
	"scriptaddr=0x02100000\0" \
	"scriptfile=u-boot.scr\0" \
	"fatscript=if fatload mmc 0:2 ${scriptaddr} ${scriptfile};" \
			"then source ${scriptaddr}; fi\0" \
	"socfpga_legacy_reset_compat=1\0" \
	"prog_core=if load mmc 0:2 ${loadaddr} fit_spl_fpga.itb;" \
		   "then fpga loadmk 0 ${loadaddr}:fpga-core-1; fi\0" \
	SOCFPGA_BOOT_SETTINGS \
	BOOTENV

#endif

/* The rest of the configuration is shared */
#include <configs/socfpga_common.h>

/*
 * L4 Watchdog
 */
#ifdef CONFIG_HW_WATCHDOG
#undef CONFIG_DW_WDT_BASE
#define CONFIG_DW_WDT_BASE		SOCFPGA_L4WD1_ADDRESS
#endif

#ifdef CONFIG_NAND_DENALI_DT
#define CONFIG_SYS_NAND_BAD_BLOCK_POS        0x0
#endif

#endif	/* __CONFIG_SOCFPGA_ARRIA10_ACHILLES_V2_LITE_H__ */
