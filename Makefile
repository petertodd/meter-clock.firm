# ### BOILERPLATE ###
# Meter Clock Firmware
# Copyright (C) 2007 Peter Todd <pete@petertodd.org>
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
# ### BOILERPLATE ###

# Makefile for 18F1320 compilation w/ SDCC.

INCLUDE=-I. -I/usr/share/sdcc/include/pic16
LDFLAGS=-w -I/usr/share/sdcc/lib/pic16 -c -s /usr/share/gputils/lkr/18f1320.lkr 

# The following libraries are very important! They contain the code that jumps
# to the initial main() function for one thing.
LIBS=crt0i.o pic18f1320.lib libc18f.lib libsdcc.lib
PROCESSOR=pic18f1320
PROCESSOR_FLAGS=-mpic16 -p$(PROCESSOR)
SDCC=sdcc
GPASM=gpasm
CFLAGS=-V -S --optimize-goto --optimize-cmp --optimize-df --obanksel=2
GPLINK=gplink

OUTPUT_HEX=meter-clock.hex

SRCS=debounce.c ds3231.c eeprom.c i2c.c main.c meters.o metrics.c time.c user.c
OBJS=${SRCS:%.c=%.o}

all: $(OUTPUT_HEX) 

%.o : %.c 
	$(SDCC) $(INCLUDE) $(PROCESSOR_FLAGS) $(CFLAGS) $<  
	$(GPASM) -c $*.asm  

$(OUTPUT_HEX): $(OBJS)
	$(GPLINK) $(LDFLAGS) -o $(OUTPUT_HEX) $(OBJS) $(LIBS)

burn: all init_eeprom.dat
	srec_cat $(OUTPUT_HEX) -Intel init_eeprom.dat -Intel -ignore-checksums -Output -Intel > combined.hex
	pk2 -progress -write combined.hex

depend: ${SRCS}
	makedepend -f.depend -D$(PROCESSOR) ${INCLUDE} ${SRCS}

include .depend

clean:
	rm -f *.cof *.asm *.cod *.lst *.hex *.o *~

realclean: clean
	rm -f .depend
