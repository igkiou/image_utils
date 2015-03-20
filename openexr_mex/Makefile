USE_GCC = 1
DEBUG_MODE = 1

CFLAGS =
LDFLAGS =
INCLUDES = -I../include -I/home/igkiou/MATLAB/mex_utils -I/home/igkiou/MATLAB/openexr -I/home/igkiou/MATLAB/openexr/OpenEXR/config -I/home/igkiou/MATLAB/openexr/IlmBase/config -I/home/igkiou/MATLAB/openexr/IlmBase/include/OpenEXR/
LIBS = -L/home/igkiou/MATLAB/openexr/OpenEXR/IlmImf/ -L/home/igkiou/MATLAB/openexr/IlmBase/lib -lIlmImf-2_2 -lHalf -lIex-2_2 -lIexMath-2_2 -lIlmThread-2_2 -lImath-2_2

ifeq ($(USE_GCC), 1)
	include ../gcc.mk
else
	include ../icc.mk
endif

include ../matlab.mk

#all: read write get is test
all: read write get is test

get: exrget.$(MEXEXT)
read: exrread.$(MEXEXT)
write: exrwrite.$(MEXEXT)
is: isexr.$(MEXEXT)
test: test_exr.$(MEXEXT)

%.$(MEXEXT): %.o openexr_mex.o
	$(LD) $(LDFLAGS) $(INCLUDES) $(CFLAGS) -o $@ $^ $(LIBS) 

%.o: %.cpp openexr_mex.h
	$(CC) $(INCLUDES) $(LDFLAGS) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -rf *.o *~

distclean:	
	rm -rf *.o *~ *.$(MEXEXT)
