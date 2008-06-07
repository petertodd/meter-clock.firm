// ### BOILERPLATE ###
// Alternate Pace Firmware 
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

#ifndef EEPROM_H
#define EEPROM_H

// Robust eeprom reading and writing. Internally does error correction and what
// not.

// Address list here. Addresses should be sequential, even though these are 4
// byte values.

// internal metrics
#define EEPROM_ADDR_METRIC_RUNNING_TIME (0)
#define EEPROM_ADDR_METRIC_SW_HOURS (1)
#define EEPROM_ADDR_METRIC_SW_MINS (2)
#define EEPROM_ADDR_METRIC_SW_SECS (3)
#define EEPROM_ADDR_METRIC_MAX_TEMP (4)
#define EEPROM_ADDR_METRIC_MIN_TEMP (5)
#define EEPROM_ADDR_METRIC_META (6)

#define EEPROM_ADDR_MODE (7)
#define EEPROM_ADDR_SILLY_HOUR_DISPLAY (8)

#define num_eeprom_items (9)

extern uint32_t eeprom_data[num_eeprom_items];

// Reads a uint32 from the eeprom at addr
#define eeprom_read_uint32(a) eeprom_data[a]

// Robustly writes uint32 d to the eeprom at addr
#define eeprom_write_uint32(d,a) eeprom_data[a] = d

void init_eeprom();

// Call during int repeatedly.
void save_eeprom_if_needed();

// Trigger an eeprom save eeprom needs saving.
void trigger_save_eeprom();

// Local Variables:
// mode: C
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:

#endif
