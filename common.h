// ### BOILERPLATE ###
// Meter Clock Firmware 
// Copyright (C) 2007 Peter Todd <pete@petertodd.org>
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
// ### BOILERPLATE ###

#include <pic18fregs.h>
#include <delay.h>
#include <stdbool.h>
#include <stdint.h>

// Easy way to define a constant from a bunch of bits.
//
// Usage: b(00101010)
#define b(x) \
	( ((0x##x##L & 0x00000001L) ? 0x01 : 0)  \
	| ((0x##x##L & 0x00000010L) ? 0x02 : 0)  \
	| ((0x##x##L & 0x00000100L) ? 0x04 : 0)  \
	| ((0x##x##L & 0x00001000L) ? 0x08 : 0)  \
	| ((0x##x##L & 0x00010000L) ? 0x10 : 0)  \
	| ((0x##x##L & 0x00100000L) ? 0x20 : 0)  \
	| ((0x##x##L & 0x01000000L) ? 0x40 : 0)  \
	| ((0x##x##L & 0x10000000L) ? 0x80 : 0))

// Same idea as the PORTAbits types, only generic.
typedef union {
  struct {
    unsigned b0:1;
    unsigned b1:1;
    unsigned b2:1;
    unsigned b3:1;
    unsigned b4:1;
    unsigned b5:1;
    unsigned b6:1;
    unsigned b7:1;
  };
  uint8_t b;
} bits_t;

// Local Variables:
// mode: C
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:
