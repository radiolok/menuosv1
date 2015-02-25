/* arrows bitmap file for GLCD library */
/* Bitmap created from arrows.bmp      */
/* Date: 14 Jan 2013      */
/* Image Pixels = 99    */
/* Image Bytes  = 18     */

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef arrows_H
#define arrows_H

static uint8_t arrows[] PROGMEM = {
  9, // width
  11, // height

  /* page 0 (lines 0-7) */
  0x50,0xd8,0xdc,0xde,0xdf,0xde,0xdc,0xd8,0x50,
  /* page 1 (lines 8-15) */
  0x0,0x0,0x1,0x3,0x7,0x3,0x1,0x0,0x0,
};
#endif
