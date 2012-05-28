# PRODUCE_REPORTS = 1 
# USE_GUIDE = 1
# GENERATE_PROFILE = 1 
# USE_PROFILE = 1
DEBUG_MODE = 1

# USE_GCC = 1
include gcc.mk

all: read write get set

get: exrget.$(MEXEXT)
read: exrread.$(MEXEXT)
set: exrset.$(MEXEXT)
write: exrwrite_sub.$(MEXEXT)

## test mex executable 
exrset.$(MEXEXT): exrset.cpp ImfExtraAttributes.cpp openexr_mex.h ImfExtraAttributes.h
	$(CC) $(LDFLAGS) $(INCLUDES) $(LIBS) $(CFLAGS) -o $@ exrset.cpp ImfExtraAttributes.cpp  

%.$(MEXEXT): %.cpp openexr_mex.h
	$(CC) $(LDFLAGS) $(INCLUDES) $(LIBS) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o *~
	rm -rf $(MEXDIR)/*.o $(MEXDIR)/*~
	rm -rf $(SRCDIR)/*.o $(SRCDIR)/*~
	rm -rf $(LIBDIR)/*.o $(LIBDIR)/*~

distclean:	
	rm -rf *.o *~
	rm -rf $(MEXDIR)/*.o $(MEXDIR)/*.$(MEXEXT) $(MEXDIR)/*~
	rm -rf $(SRCDIR)/*.o $(SRCDIR)/*~
	rm -rf $(LIBDIR)/*.o $(LIBDIR)/*~ $(LIBDIR)/*.so*
