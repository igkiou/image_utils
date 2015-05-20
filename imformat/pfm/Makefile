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
include pfm.mk

all: read write get is test

get: pfmget.$(MEXEXT)
read: pfmread.$(MEXEXT)
write: pfmwrite.$(MEXEXT)
is: ispfm.$(MEXEXT)
test: test_pfm.$(MEXEXT)

%.$(MEXEXT): %.o pfm.o
	$(LD) $(LDFLAGS) $(INCLUDES) $(CFLAGS) -o $@ $^ $(LIBS) 

%.o: %.cpp pfm.h
	$(CC) $(INCLUDES) $(LDFLAGS) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -rf *.o *~

distclean:	
	rm -rf *.o *~ *.$(MEXEXT)
