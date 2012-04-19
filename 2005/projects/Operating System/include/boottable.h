#ifndef BOOT_TABLE_H
#define BOOT_TABLE_H

/* ----- Linear start address of display memory in text mode ------------------------------------ */
#define BT_DISPLAY_BASE_ADDRESS     0x00
/* ----- Flag indicating whether or not a floppy device is installed ---------------------------- */
#define BT_DISKETTE_INSTALLED       0x01
/* ----- Number of diskette drives installed ---------------------------------------------------- */
#define BT_DISKETTE_TOTAL           0x02
/* ----- Number of serial ports ----------------------------------------------------------------- */
#define BT_SERIAL_TOTAL             0x03
/* ----- I/O base address for serial ports ------------------------------------------------------ */
#define BT_COM4_BASE_ADDRESS        0x04
#define BT_COM3_BASE_ADDRESS        0x05
#define BT_COM2_BASE_ADDRESS        0x06
#define BT_COM1_BASE_ADDRESS        0x07
/* ----- I/O base address for the CRT address register ------------------------------------------ */
#define BT_DISPLAY_ADDRESS_REGISTER 0x08
/* ----- Linear address of the kernel stack top ------------------------------------------------- */
#define BT_KERNEL_STACK_TOP         0x09
/* ----- Size of kernel stack in bytes ---------------------------------------------------------- */
#define BT_KERNEL_STACK_SIZE        0x0a
/* ----- Extended memory in 1K blocks reported by BIOS interrupt 0x15, function 0x88 ------------ */
#define BT_EXTENDED_MEMORY          0x0b
/* ----- Amount of extended memory reported by int 0x15, function 0xe801 ------------------------ */
#define BT_EXTENDED_MEMORY_E801     0x0c
/* ----- Number of smap entries reported by the bootstrap loader -------------------------------- */
#define BT_SMAP_TOTAL               0x0d

/* ----- Maximum number of smap entries that will be reported by the bootstrap loader ----------- */
#define MAX_SMAP_ENTRIES       32

/* ----- Number of entries inside the boot table ------------------------------------------------ */
#define MAX_BOOT_TABLE_ENTRIES 14

/*
 * SMAP entry types (based on ACPI 3.0 specification)
 *   ARM    - Address Range Memory: Suitable for memory allocation by O/S
 *   ARR    - Address Range Reserved: Not suitable for allocation by O/S
 *   ARACPI - Address Range ACPI: ACPI Reclaim memory, can be used after O/S
 *            reads ACPI tables from memory
 *   ARNVS  - Address Range NVS: Address range used during non-volatile
 *            sleep (NVS) for saving and restoring context information
 *   ARU    - Address Range Unusable: Defective memory blocks containing errors
 */
#define SMAP_TYPE_ARM    1
#define SMAP_TYPE_ARR    2
#define SMAP_TYPE_ARACPI 3
#define SMAP_TYPE_ARNVS  4
#define SMAP_TYPE_ARU    5

/*
 * System Memory Map entry definition (based on the ACPI 3.0 specification)
 */
typedef struct smap_entry {
  unsigned long long base;
  unsigned long long size;
  int type;
} smap_entry;

/*
 * DESCRIPTION: Copies the contents of the boot table from low memory to an internal structure.
 *              The boot table is stored inside the first 4K of memory and becomes inaccessible
 *              once paging is enabled because the first 4K page is marked as not-present
 *       INPUT: None
 *      OUTPUT: None
 */
void bt_initialize();

/*
 * DESCRIPTION: This function is used to retrieve the value of a specific
 *              entry from the boot table. For example,
 *              bt_entry(BT_EXTENDED_MEMORY) will return the amount of
 *              extended memory in 1K blocks as reported by the BIOS
 *              (interrupt 0x15, function 0x88)
 *       INPUT: entry - Index of the entry that must be read from the boot
 *                      table. 0 <= entry < MAX_BOOT_TABLE_ENTRIES
 *      OUTPUT: Returns the value of the specified entry in the boot table if
 *              a valid entry is specified, otherwise returns -ERR_INVALID_BT_ENTRY
 */
int bt_entry(int entry);

/*
 * DESCRIPTION: This function is used to retrieve the contents of a System
 *              Memory Map (SMAP) entry. The entries can be used to
 *              determine the amount of installed memory
 *       INPUT: entry - The specific SMAP entry number that must be retrieved.
 *                      The BT_SMAP_TOTAL entry in the boot table should be used to
 *                      determine the number of SMAP entries created by the bootstrap
 *                      loader.
 *      OUTPUT: Returns a copy of the specific smap entry in 'smap' and OK
 *              if successful, otherwise 'smap' is set to NULL and
 *              -ERR_INVALID_SMAP_INDEX is returned
 */
int bt_read_smap_entry(int entry, smap_entry* smap);

#endif
