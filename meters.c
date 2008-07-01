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
#include <meters.h>

uint8_t hours_meter,mins_meter,secs_meter;

void init_meters(){
  TRISBbits.TRISB0 = 0;
  TRISBbits.TRISB1 = 0;
  TRISBbits.TRISB4 = 0;
}

uint8_t hours_meter_pwm,mins_meter_pwm,secs_meter_pwm;
void do_meters(){

#if 1
  hours_meter_pwm--;
  if (!hours_meter_pwm)
    hours_meter_pwm = meter_range;
  mins_meter_pwm--;
  if (!mins_meter_pwm)
    mins_meter_pwm = meter_range;
  secs_meter_pwm--;
  if (!secs_meter_pwm)
    secs_meter_pwm = meter_range;
#else
  // The above is fairly inefficient, but the below is broken. Notice how movwf
  // puts everything into hours meter every time? Even when that's fixed it
  // still doesn't work, and just forces all meters off to one side.
  __asm
    banksel _hours_meter_pwm
    movlw meter_range

    decfsz  _hours_meter_pwm, f
    goto  $+2
    movwf _hours_meter_pwm

    decfsz  _mins_meter_pwm, f
    goto  $+2
    movwf _hours_meter_pwm

    decfsz  _secs_meter_pwm, f
    goto  $+2
    movwf _hours_meter_pwm
  __endasm;
#endif

  if (hours_meter > hours_meter_pwm){
    LATBbits.LATB4 = 0;
  } else {
    LATBbits.LATB4 = 1;
  }
  if (mins_meter > mins_meter_pwm){
    LATBbits.LATB1 = 0;
  } else {
    LATBbits.LATB1 = 1;
  }
  if (secs_meter > secs_meter_pwm){
    LATBbits.LATB0 = 0;
  } else {
    LATBbits.LATB0 = 1;
  }
}

// Local Variables:
// mode: C
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:
