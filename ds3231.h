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

#ifndef DS3231_H
#define DS3231_H

void init_ds3231();

typedef struct {
  uint8_t secs;
  uint8_t mins;
  uint8_t hours;
} ds3231_time_t;

extern ds3231_time_t ds3231_time;

// Read time from the ds3231
//
// A rather annoying thing about this is it turns out the ds3231 seems to have
// jitter in the seconds. So there is *no* way to align up to an exact seconds
// boundry, even by waiting for the rollover to happen. Just don't bother
// trying.
void read_time_from_ds3231();

// Write time to the ds3231.
void write_time_to_ds3231();

uint8_t from_bcd(uint8_t d);
uint8_t to_bcd(uint8_t d);

// Local Variables:
// mode: C
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:

#endif
