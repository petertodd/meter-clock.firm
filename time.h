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

#ifndef TIME_H
#define TIME_H

void init_time();

// Do required time tasks for the interrupt.
void do_time_int();

// Do required time tasks to update to the RTC.
void do_time_update();

// Save time to the rtc.
void save_time_to_rtc();

// Load time from the rtc. 
void load_time_from_rtc();

#define inc_one_hour() \
  inc_hours(); \
  inc_hours(); \
  inc_hours(); \
  inc_hours(); \
  inc_hours(); \
  inc_hours(); \
  inc_hours(); \
  inc_hours(); \
  inc_hours(); \
  inc_hours();

#define inc_one_minute() \
  inc_mins(); \
  inc_mins(); \
  inc_mins(); \
  inc_mins();

void clear_secs();

// Don't use these directly, use above functions instead.
void inc_hours();
void inc_mins();
void inc_secs();

// Local Variables:
// mode: C
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:

#endif
