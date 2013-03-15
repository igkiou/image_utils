# PRODUCE_REPORTS = 1 
# USE_GUIDE = 1
# GENERATE_PROFILE = 1 
# USE_PROFILE = 1
DEBUG_MODE = 1

# USE_GCC = 1
include gcc.mk

all: read write get is test

get: pfmget.$(MEXEXT)
read: pfmread.$(MEXEXT)
write: pfmwrite.$(MEXEXT)
is: ispfm.$(MEXEXT)
test: test_pfm.$(MEXEXT)

%.$(MEXEXT): %.o pfm_mex.o
	$(CC) $(LDFLAGS) $(INCLUDES) $(LIBS) $(CFLAGS) -o $@ $^

%.o: %.cpp pfm_mex.h
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -rf *.o *~

distclean:	
	rm -rf *.o *~ *.$(MEXEXT)
