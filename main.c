// ###BOILERPLATE###
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
// ###BOILERPLATE###

#include <common.h>
#include <signal.h>

#include <meters.h>
#include <time.h>
#include <user.h>
#include <eeprom.h>

code char at __CONFIG1H config1h = 0xFF & _OSC_INT_Port_on_RA6_Port_on_RA7_1H; 
code char at __CONFIG2L config2l = 0xFF & _PUT_ON_2L & _BODEN_ON_2L & _BODENV_4_5V_2L;
code char at __CONFIG2H config2h = 0xFF & _WDT_DISABLED_CONTROLLED_2H;
code char at __CONFIG3H config3h = 0xFF & _MCLRE_MCLR_enabled_RA5_input_dis_3H;
code char at __CONFIG4L config4l = 0xFF & _LVP_OFF_4L & _BACKBUG_OFF_4L & _STVR_OFF_4L;
code char at __CONFIG5L config5l = 0xFF & _CP_0_OFF_5L & _CP_1_OFF_5L;
code char at __CONFIG5H config5h = 0xFF & _CPD_OFF_5H & _CPB_OFF_5H;
code char at __CONFIG6L config6l = 0xFF & _WRT_0_OFF_6L & _WRT_1_OFF_6L;
code char at __CONFIG6H config6h = 0xFF & _WRTD_OFF_6H & _WRTB_OFF_6H & _WRTC_OFF_6H;
code char at __CONFIG7L config7l = 0xFF & _EBTR_0_OFF_7L & _EBTR_1_OFF_7L;
code char at __CONFIG7H config7h = 0xFF & _EBTRB_OFF_7H;

// If stack isn't set, delay() calls do really weird things.
//
// Base set at 0x80 as aparently there is a "access bank" from 0x00 to 0x80
//
// Unsure as to what the meaning of all that is exactly...
#pragma stack 0x80 0x20

uint8_t non_pwm_tasks_divider;

// Interupts, AKA signals.
DEF_INTHIGH(high_int)
	
DEF_HANDLER(SIG_TMR0,_tmr0_handler)

END_DEF

SIGHANDLER(_tmr0_handler)

{
  non_pwm_tasks_divider--;
  if (!non_pwm_tasks_divider){
    check_for_user_input();
    do_time_int();
  }

  do_meters();

  save_eeprom_if_needed();

  // Re-enable ourselves.
  INTCONbits.T0IF = 0;
}

void main(){
  // Set oscillator to 8mhz.
  OSCCON = b(01110000);
  
  // Ports
  TRISA = 0x00;
  TRISB = 0x00;

  // turn ADC ports off
  ADCON1 = 0x7F;

  // Modules
  init_eeprom();
  init_time();
  init_meters();
  init_user();

  // Setup TMR0. 8mhz/4/256/240 = 32.5hz pwm frequency. 
  INTCONbits.T0IF = 0;
  INTCONbits.T0IE = 1;
  INTCONbits.GIE = 1;
  T0CON = b(11001000); 

  // Done setting up, loop until the end of time.
  while (1){
    // Insert seti@home implementation here.
  }
}

// Local Variables:
// mode: C
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:
