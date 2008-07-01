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
#include <time.h>
#include <metrics.h>
#include <meters.h>
#include <user.h>
#include <ds3231.h>

// This is all done a little strangely...
//
// First of all the pwm range is set to 240 rather than 256. This is so the
// time values can be easilly multipled to get pwm values. 240/60=4 240/24=10
//
// Having established that each meter then needs to be incremented a given
// ratio with respect to the other, rather than the more simple on roll over.
// So we increment two sets of variables, the pwm value, as well as the
// ratio incrementor.

// Number of hours, mins, secs
#define num_time_hours (24)
#define num_time_mins (60)
#define num_time_secs (60)

// 8,000,000/4/256/256/4 = 7.629 
#define num_time_tics (8)

// Ratio counters.
uint8_t ratio_count_mins = 1,ratio_count_secs = 1,ratio_count_tics = 1;

void init_time(){
  init_ds3231();
  load_time_from_rtc();
}

// Sheesh, no osctune?
__sfr __at (0xf9b) OSCTUNE;

// Further more, looks like we need a shadow register for it too...
// OSCTUNE++; does nothing, same with OSCTUNE += 2; but OSCTUNE = 10
// does something. Weird.
uint8_t osctune_shadow = 0;

void do_time_int(){
  // Tics aren't used anywhere else, so as an optimization, we can increment
  // their ratio directly and not bother keeping track of their real value.
  ratio_count_tics--;
  if (!ratio_count_tics){
    ratio_count_tics = num_time_tics + 1;

    // Once a second...
    if (!(secs_meter % 4))
      inc_metric_running_time();

    secs_meter--;
    if (!secs_meter){
      secs_meter = meter_range + 1;

      // One minute has passed. This the the correct place to put this sort of
      // code, when the pwm for the meter overflows, not the ratio counter.

      // Try to tune the oscillator slightly to follow the rtc perfectly.
      //
      // This will correct for some really major errors actually, OSCTUNE can
      // shift the oscillator frequency quite a bit, in the order of multiple
      // seconds off (%10) for setting it to 10

      // Try to resync a bit... The current timer1 code, even though it's being
      // fed from the clean 32khz output, seems to be a little slow.
      
      read_time_from_ds3231();

      // If we're on track, seconds will == 0 too. Otherwise push or pull
      // time_tics a bit to adjust the time. 
      if (ds3231_time.secs){
        if (ds3231_time.secs > num_time_secs / 2){
          // running fast
          osctune_shadow--;
          OSCTUNE = osctune_shadow;
        } else {
          // running slow
          osctune_shadow++;
          OSCTUNE = osctune_shadow;
        }
      }
    }

    ratio_count_secs--;
    if (!ratio_count_secs){
      ratio_count_secs = num_time_mins + 1;

      inc_mins();
    }
  }
}

void inc_mins(){
  mins_meter--;
  if (!mins_meter)
    mins_meter = meter_range + 1;
  ratio_count_mins--;
  if (!ratio_count_mins){
    ratio_count_mins = num_time_hours + 1;

    inc_hours();
  }
}

void inc_hours(){
  hours_meter--;
  if (!hours_meter){
    hours_meter = meter_range + 1;

    // Saving once an hour won't exceed the eeprom's write endurance.
    save_metrics();
  }

  // No further ratio for hours.
}

void clear_secs(){
  // Undo the effects of the ratio increments.
  mins_meter += (((meter_range + 1) - secs_meter) / num_time_secs);

  // Reset to zero.
  ratio_count_secs = num_time_secs + 1;
  secs_meter = meter_range + 1;
}

void save_time_to_rtc(){
  ds3231_time.hours = (meter_range - hours_meter) / 4;
  ds3231_time.mins = (meter_range - mins_meter) / 4;
  ds3231_time.secs = (meter_range - secs_meter) / 4;

  write_time_to_ds3231();
}

void load_time_from_rtc(){
  read_time_from_ds3231();

  secs_meter = meter_range - (ds3231_time.secs * 4);
  mins_meter = meter_range - (ds3231_time.mins * 4);
  hours_meter = meter_range - (ds3231_time.hours * 4);

  // FIXME: need to also muck with the ratio's here as well.
}

// Local Variables:
// mode: C
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:
