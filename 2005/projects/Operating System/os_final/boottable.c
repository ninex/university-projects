/*
 *        MODULE: boottable.c
 *   DESCRIPTION: This module provides access to the information inside the boot table
 *                that was created by the bootstrap loader.
 *        AUTHOR: Jacques Eloff
 *                University of Stellenbosch
 *                eloff@cs.sun.ac.za
 * LAST MODIFIED: 14.12.2004
 *     CHANGELOG: 14.12.2004 - Only non-empty SMAP entries are copied from
 *                             the boot table. The entries are also now
 *                             sorted according to their base addresses.
 *                17.09.2004 - Completed comments regarding access to the
 *                             SMAP table
 *                27.05.2004 - Completed SMAP definitions
 *                25.05.2004 - Additional comments to clarify certain entries
 *                20.12.2003 - Ported from original Oberon implementation (flouw@cs.sun.ac.za)
 */
#include "include/error.h"
#include "include/boottable.h"
#include "include/io.h"
#include "include/stddef.h"

/* ----- Physical address of the first boot table entry ----------------------------------------- */
#define BT_START      0x0ffc

/* ----- Physical address of the first SMAP entry inside the boot table ------------------------- */
#define BT_SMAP_START 0x0600

/* ------ Local copy of boot table created by bootstrap loader ---------------------------------- */
static int boot_table[MAX_BOOT_TABLE_ENTRIES];

/* ----- Local copy of the system memory map reported by int 0x15, 0xe820 ----------------------- */
static smap_entry smap_table[MAX_SMAP_ENTRIES];

void sort_smap_entries(smap_entry *list, int n) {
  int i;
  int j;
  int bottom;
  int top;
  int middle;
  smap_entry temp;

  if (list != NULL) {
    i = 1;
    while (i < n) {
      temp = list[i];
      bottom = 0;
      top = i-1;
      while (bottom <= top) {
        middle = (bottom+top)/2;
        if (temp.base < list[middle].base)
          top = middle-1;
        else
          bottom = middle+1;
      }
      j = i-1;
      while (j >= bottom) {
        list[j+1] = list[j];
        j = j-1;
      }
      list[bottom] = temp;
      i = i+1;
    }
  }
}

void bt_initialize() {
  int i;
  int j;
  smap_entry *s;

  i = 0;
  while (i < MAX_BOOT_TABLE_ENTRIES) {
    boot_table[i] = readl(BT_START-(i*4));
    i = i+1;
  }

  i = 0;
  j = 0;
  while (i < MAX_SMAP_ENTRIES) {
    s = (smap_entry*)(BT_SMAP_START+(i*sizeof(smap_entry)));
    if (s->type != 0) {
      smap_table[j] = *s;
      j++;
    }
    i = i+1;
  }

  sort_smap_entries(smap_table, j);
}

int bt_entry(int entry) {
  if ((entry >= 0) && (entry < MAX_BOOT_TABLE_ENTRIES))
    return boot_table[entry];
  else
    return -ERR_INVALID_BT_INDEX;
}

int bt_read_smap_entry(int entry, smap_entry* smap) {
  if ((entry >= 0) && (entry < bt_entry(BT_SMAP_TOTAL))) {
    *smap = smap_table[entry];
    return OK;
  } else {
    smap = NULL;
    return -ERR_INVALID_SMAP_INDEX;
  }
}
