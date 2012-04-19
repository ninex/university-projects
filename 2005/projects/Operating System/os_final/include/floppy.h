#ifndef FLOPPY_H
#define FLOPPY_H

/* ------ Size of a single sector in bytes ------------------------------------------------------ */
#define SECTOR_SIZE 512

/*
 * DESCRIPTION: Reads a sector or group of sectors from the specified
 *              diskette drive into a buffer
 *       INPUT: drive_number - diskette drive number
 *              sector - logical sector number (0 based)
 *              count - number of sectors to read
 *              buffer - pointer to buffer where data should be copied
 *              buf_size - the size of the buffer being used in bytes
 *      OUTPUT: Returns OK if successful or an error code indicating why
 *              the operation failed
 */
int floppy_read(int drive_number, int sector, int count, void *buffer, int buf_size);

/*
 * DESCRIPTION: Writes the contents of a buffer to a sector or group of sectors
 *              to the specified diskette drive
 *       INPUT: drive_number - diskette drive number
 *              sector - logical sector number (0 based)
 *              count - number of sectors to write
 *              buffer - pointer to buffer from where data should be copied
 *              buf_size - the size of the buffer being used in bytes
 *      OUTPUT: Returns OK if successful or an error code indicating why
 *              the operation failed
 */
int floppy_write(int drive_number, int sector, int count, void *buffer, int buf_size);

/*
 * DESCRIPTION: Detect and initialize NEC765 compatible diskette controllers. A maximum of
 *              two drives are supported. DMA channel 2 is permanetly allocated to this
 *              driver if the initialization procedure is successful.
 *       INPUT: None
 *      OUTPUT: None
 */
void floppy_initialize();

#endif
