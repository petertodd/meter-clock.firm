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

#include <common.h>
#include <eeprom.h>

uint32_t eeprom_data[num_eeprom_items];

// 16 so a pk2 -readdata shows each value on it's own line
#define spacing (16)

void do_eeprom_read(){
  EECON1bits.EEPGD = 0;
  EECON1bits.CFGS = 0;
  EECON1bits.RD = 1;
}

void init_eeprom(){
  for (EEADR = 0; EEADR < sizeof(eeprom_data); EEADR++){
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
    ((uint8_t *)(&eeprom_data))[EEADR] = EEDATA;
  }
}

uint8_t incremental_save_counter = sizeof(eeprom_data);

void save_eeprom_if_needed(){
  if ((incremental_save_counter < sizeof(eeprom_data))
      && !EECON1bits.WR){ // Is previous write complete?
    EEADR = incremental_save_counter;

    EEDATA = ((uint8_t *)(&eeprom_data))[EEADR];

    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;

    // critical bit here
    _asm 
      movlw	0x55
      movwf	_EECON2
      movlw	0xAA
      movwf	_EECON2
      bsf	_EECON1,1 
    _endasm;

    incremental_save_counter++;
  }
}

void trigger_save_eeprom(){
  incremental_save_counter = 0;
}

// Local Variables:
// mode: C
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:
