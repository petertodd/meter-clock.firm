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

#include <common.h>
#include <i2c.h>

void init_i2c(){
  LATBbits.LATB2 = 0;
  LATBbits.LATB3 = 0;
  i2c_stop();
}

void i2c_reset(){
  i2c_sda = 1;
  while (!i2c_sda_in){
    i2c_scl = 0;
    i2c_delay();
    i2c_scl = 1;
    i2c_delay();
  }
}

void i2c_start(){
  i2c_sda = 1;
  i2c_delay();
  i2c_scl = 1;
  i2c_delay();
  i2c_sda = 0;
  i2c_delay();
  i2c_scl = 0;
  i2c_delay();
}

void i2c_stop(){
  i2c_sda = 0; 
  i2c_delay();
  i2c_scl = 1;
  i2c_delay();
  i2c_sda = 1;
  i2c_delay();
}

bool i2c_write(uint8_t d){
  uint8_t i;
  bool b;

  for(i = 8; i != 0; i--) {
    if(d & b(10000000))
      i2c_sda = 1;
    else
      i2c_sda = 0;

    i2c_delay();
    i2c_scl = 1;
    i2c_delay();
    i2c_scl = 0;

    d <<= 1;
  }

  i2c_delay();
  i2c_sda = 1;
  i2c_delay();
  i2c_scl = 1;
  i2c_delay();

  b = i2c_sda_in; // possible ACK bit
  i2c_scl = 0;
  i2c_delay();
  return b;
}

uint8_t i2c_read(bool ack){
  uint8_t i, d = 0;
  i2c_sda = 1; 
  for(i = 8; i != 0; i--) {
    i2c_delay();
    d <<= 1;
    do {
      i2c_scl = 1;
    }
    while(i2c_scl_in == 0); // wait for any i2c_scl clock stretching to complete
    i2c_delay();
    if (i2c_sda_in)
      d |= 1;
    i2c_scl = 0;
  } 

  if (ack)
    i2c_sda = 0;
  else
    i2c_sda = 1;

  i2c_delay();
  i2c_scl = 1;
  i2c_delay();             // send (N)ACK bit
  i2c_scl = 0;
  i2c_delay();
  i2c_sda = 1;

  return d;
}


// Local Variables:
// mode: C
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:
