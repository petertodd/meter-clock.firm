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

#ifndef METRICS_H
#define METRICS_H

#include <eeprom.h>

#define get_metric_running_time() eeprom_data[EEPROM_ADDR_METRIC_RUNNING_TIME]
#define get_metric_sw_hours() eeprom_data[EEPROM_ADDR_METRIC_SW_HOURS]
#define get_metric_sw_mins() eeprom_data[EEPROM_ADDR_METRIC_SW_MINS]
#define get_metric_sw_secs() eeprom_data[EEPROM_ADDR_METRIC_SW_SECS]
#define get_metric_max_temp() eeprom_data[EEPROM_ADDR_METRIC_MAX_TEMP]
#define get_metric_min_temp() eeprom_data[EEPROM_ADDR_METRIC_MIN_TEMP]
#define get_metric_meta() eeprom_data[EEPROM_ADDR_METRIC_META]

#define inc_metric_running_time() \
  get_metric_running_time()++;
// Running time doesn't save as it happens too often.

#define inc_metric_meta() \
  save_metrics(); \
  get_metric_meta()++;

#define inc_metric_sw_hours() \
  save_metrics(); \
  get_metric_sw_hours()++;

#define inc_metric_sw_mins() \
  save_metrics(); \
  get_metric_sw_mins()++;

#define inc_metric_sw_secs() \
  save_metrics(); \
  get_metric_sw_secs()++;

#define save_metrics() trigger_save_eeprom()

// Note the int8_t conversions, the temperature is stored as a signed value in
// the obvious way.
#define new_temp_sample(s) \
  if ((int8_t)s > (int8_t)get_metric_max_temp()){ \
    get_metric_max_temp() = s; \
    save_metrics(); \
  } \
  if ((int8_t)s < (int8_t)get_metric_min_temp()){ \
    get_metric_min_temp() = s; \
    save_metrics(); \
  }


// Local Variables:
// mode: C
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:

#endif
