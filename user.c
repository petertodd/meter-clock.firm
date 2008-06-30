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
#include <user.h>
#include <time.h>
#include <eeprom.h>
#include <debounce.h>
#include <metrics.h>
#include <ds3231.h>

// Raw pin io's, true if pressed, false if not.
#define hours_switch_raw (!PORTAbits.RA0)
#define mins_switch_raw (!PORTAbits.RA2)
#define secs_switch_raw (!PORTAbits.RA7)

uint8_t silly_hour_display;

void init_user(){
  uint8_t i;

  TRISAbits.TRISA0 = 1;
  TRISAbits.TRISA2 = 1;
  TRISAbits.TRISA7 = 1;

  if (hours_switch_raw && mins_switch_raw && secs_switch_raw){
    // Oooh! Metrics display mode! Don't see that very often...
    inc_metric_meta();
    save_metrics();

    // Basically just cycle through the bytes in the eeprom and display them on
    // screen forever.

    i = 0;
    while (1){
      // FIXME, not display, meters.
#if 0
      display_digits(
                     i / 16,i % 16,
                     CHAR_BLANK,CHAR_BLANK,
                     (((uint8_t *)(&eeprom_data))[i] / 16),
                      ((uint8_t *)(&eeprom_data))[i] % 16);
      delay100ktcy(4);
#endif

      i++;
      if (i >= sizeof(eeprom_data))
        i = 0;
    }
  }

  silly_hour_display = eeprom_read_uint32(EEPROM_ADDR_SILLY_HOUR_DISPLAY);
}

void check_for_user_input(){
  static debounce_state_t hours_debounce,mins_debounce,secs_debounce;

  if (debounce_just_pressed(hours_debounce,hours_switch_raw)){
    inc_one_hour();
    inc_metric_sw_hours();
    save_time_to_rtc();
  } //else if (debounce_held_down(hours_debounce,hours_switch_raw)) {
    //silly_hour_display = !silly_hour_display;
    //eeprom_write_uint32(silly_hour_display,EEPROM_ADDR_SILLY_HOUR_DISPLAY);
    //save_eeprom();
  //}

  if (debounce_just_pressed(mins_debounce,mins_switch_raw)){
    inc_one_minute();
    inc_metric_sw_mins();
    save_time_to_rtc();
  }

  if (debounce_just_pressed(secs_debounce,secs_switch_raw)){
    clear_secs();
    inc_metric_sw_secs();
    save_time_to_rtc();
  }

  debounce_add_sample(hours_debounce,hours_switch_raw);
  debounce_add_sample(mins_debounce,mins_switch_raw);
  debounce_add_sample(secs_debounce,secs_switch_raw);
}

// Local Variables:
// mode: C
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:
