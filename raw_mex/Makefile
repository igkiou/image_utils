USE_GCC = 1
DEBUG_MODE = 1

CFLAGS =
LDFLAGS =
INCLUDES = -I../include
LIBS =

ifeq ($(USE_GCC), 1)
	include ../gcc.mk
else
	include ../icc.mk
endif

include ../matlab.mk
include ../mex_utils.mk
include libraw.mk

all: read get is

get: rawget.$(MEXEXT)
read: rawread.$(MEXEXT)
is: israw.$(MEXEXT)

%.$(MEXEXT): %.o libraw_ext.o raw_mex.o 
	$(LD) $(LDFLAGS) $(INCLUDES) $(CFLAGS) -o $@ $^ $(LIBS) 

%.o: %.cpp raw_mex.h libraw_ext.h
	$(CC) $(INCLUDES) $(LDFLAGS) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -rf *.o *~

distclean:	
	rm -rf *.o *~ *.$(MEXEXT)
