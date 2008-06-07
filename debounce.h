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

#ifndef DEBOUNCE_H
#define DEBOUNCE_H

// Highly flexible debounce routines.
//
// Basic usage:
//
// debounce_state_t state;
//
// if (debounce_just_pressed(state,PORTAbits.RA0))
//    do something
//
// debounce_add_sample(state,PORTAbits.RA0)

typedef 
  struct {
    uint8_t timer;
    int8_t last;
  }
debounce_state_t;

#define debounce_timer_reset (127)

#define debounce_timer_delay (debounce_timer_reset-3)

#define debounce_just_changed(state,value) \
            ((state.timer < debounce_timer_delay) && (state.last != value))

#define debounce_just_pressed(state,value) \
            (value && debounce_just_changed(state,value))

#define debounce_just_released(state,value) \
            (!value && debounce_just_changed(state,value))

#define debounce_held_down(state,value) \
            ((state.timer == 1) && (value == true))

#define debounce_add_sample(state,value) \
            if (state.last != value){ \
              state.timer = debounce_timer_reset; \
              state.last = value; \
            } \
            if (state.timer) \
              state.timer--;

// Local Variables:
// mode: C
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:

#endif
