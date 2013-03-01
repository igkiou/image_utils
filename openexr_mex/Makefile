# PRODUCE_REPORTS = 1 
# USE_GUIDE = 1
# GENERATE_PROFILE = 1 
# USE_PROFILE = 1
DEBUG_MODE = 1

# USE_GCC = 1
include gcc.mk

all: read write get test

get: exrget.$(MEXEXT)
read: exrread.$(MEXEXT)
write: exrwrite_sub.$(MEXEXT)
test: test_exr.$(MEXEXT)

%.$(MEXEXT): %.o ImfVectorAttribute.o openexr_mex.o
	$(CC) $(LDFLAGS) $(INCLUDES) $(LIBS) $(CFLAGS) -o $@ $^

%.o: %.cpp openexr_mex.h ImfVectorAttribute.h
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -rf *.o *~

distclean:	
	rm -rf *.o *~ *.$(MEXEXT)
