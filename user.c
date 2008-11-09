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
#include <meters.h>

// Raw pin io's, true if pressed, false if not.
#define hours_switch_raw (!PORTAbits.RA0)
#define mins_switch_raw (!PORTAbits.RA2)
#define secs_switch_raw (!PORTAbits.RA7)

uint8_t silly_hour_display;

static debounce_state_t hours_debounce,mins_debounce,secs_debounce;

void init_user(){
  uint8_t i,j;

  TRISAbits.TRISA0 = 1;
  TRISAbits.TRISA2 = 1;
  TRISAbits.TRISA7 = 1;

  if (hours_switch_raw || mins_switch_raw || secs_switch_raw){
    // Oooh! Metrics/self-test display mode! Don't see that very often...
    inc_metric_meta();

    // High, midrange and low tests.
    //
    // Note that for a bi-polar meter, low is not the same as power off...
    if (hours_switch_raw && !mins_switch_raw && !secs_switch_raw){
      hours_meter = mins_meter = secs_meter = 0;
    } else if (!hours_switch_raw && mins_switch_raw && !secs_switch_raw){
      hours_meter = mins_meter = secs_meter = meter_range / 2;
    } else if (!hours_switch_raw && !mins_switch_raw && secs_switch_raw){
      hours_meter = mins_meter = secs_meter = meter_range - 1;
    }
    else { 
      // Metrics display.
      //
      // Hitting hours goes forward a byte, seconds goes backwards. Index is
      // displayed as the hours, high and low nibbles as minutes and seconds
      // respectively.

      i = 0;
      while (1){
        hours_meter = meter_range - ((meter_range / (sizeof(eeprom_data) - 1)) * i);
        mins_meter = meter_range - ((meter_range / 15) *
                                    (((uint8_t *)(&eeprom_data))[i] / 16));
        secs_meter = meter_range - ((meter_range / 15) *
                                    (((uint8_t *)(&eeprom_data))[i] % 16));

        if (debounce_just_pressed(hours_debounce,hours_switch_raw)){
          inc_metric_sw_hours();
          i++;
          if (i >= sizeof(eeprom_data))
            i = 0;
        }
        if (debounce_just_pressed(secs_debounce,secs_switch_raw)){
          inc_metric_sw_secs();
          i--;
          if (i >= sizeof(eeprom_data))
            i = sizeof(eeprom_data) - 1;
        }

        debounce_add_sample(hours_debounce,hours_switch_raw);
        debounce_add_sample(secs_debounce,secs_switch_raw);

        save_eeprom_if_needed();
        j = 255;
        while (j){
          j--;
          do_meters();
          // slow things down a bit for the debounce code
          delay10tcy(5);
        }
      }
    }
    while (1){
      save_eeprom_if_needed();
      do_meters();
    }
  }
}

void check_for_user_input(){
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
