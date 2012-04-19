#ifndef CMOS_H
#define CMOS_H

/* ------ I/O base addresses for CMOS registers ------------------------------------------------- */
#define CMOS_ADDRESS_REGISTER 0x70
#define CMOS_DATA_REGISTER    0x71

/* ------ Standard CMOS register offsets -------------------------------------------------------- */
#define CMOS_RTC_SECONDS       0x00
#define CMOS_RTC_SECONDS_ALARM 0x01
#define CMOS_RTC_MINUTES       0x02
#define CMOS_RTC_MINUTES_ALARM 0x03
#define CMOS_RTC_HOURS         0x04
#define CMOS_RTC_HOURS_ALARM   0x05
#define CMOS_RTC_DAY_OF_WEEK   0x06
#define CMOS_RTC_DAY_OF_MONTH  0x07
#define CMOS_RTC_MONTH         0x08
#define CMOS_RTC_YEAR          0x09
#define CMOS_SRA               0x0a /* Status Register A */
#define CMOS_SRB               0x0b /* Status Register B */
#define CMOS_SRC               0x0c /* Status Register C */
#define CMOS_SRD               0x0d /* Status Register D */
#define CMOS_DSR               0x0e /* Diagnostic Status Register */
#define CMOS_SHUTDOWN_STATUS   0x0f
#define CMOS_DDT               0x10 /* Diskette Drive Type */

/* ------ Bit masks for diskette drive type register -------------------------------------------- */
#define CMOS_DDT0_MASK 0xf0
#define CMOS_DDT0_NONE 0x00
#define CMOS_DDT0_360  0x10
#define CMOS_DDT0_1200 0x20
#define CMOS_DDT0_720  0x30
#define CMOS_DDT0_1440 0x40
#define CMOS_DDT0_2880 0x50

#define CMOS_DDT1_MASK 0x0f
#define CMOS_DDT1_NONE 0x00
#define CMOS_DDT1_360  0x01
#define CMOS_DDT1_1200 0x02
#define CMOS_DDT1_720  0x03
#define CMOS_DDT1_1440 0x04
#define CMOS_DDT1_2880 0x05

/*
 * DESCRIPTION: Read a register from CMOS memory.
 *       INPUT: cmos_register - CMOS register that must be read
 *      OUTPUT: Returns the contents of the CMOS register. A value of 0 will be returned
 *              if an invalid register was specified.
 */
unsigned char cmos_read_register(unsigned char cmos_register);

#endif
