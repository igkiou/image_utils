MATLABDIR = /usr/local/matlabR2010a

MATLABARCH = glnxa64
MEXEXT = $(shell $(MATLABDIR)/bin/mexext)
MAPFILE = mexFunction.map

EXRLIBS = -L/usr/lib/ -lIlmImf -lIex
MATLABLIBS = -L$(MATLABDIR)/bin/$(MATLABARCH) -lmx -lmex -lmat
RPATH = -Wl,-rpath-link,$(MATLABDIR)/bin/$(MATLABARCH)
LIBS = $(RPATH) $(MATLABLIBS) $(EXRLIBS) -lm -lpthread 

EXRINCLUDE = -I/usr/include/OpenEXR
MATLABINCLUDE = -I$(MATLABDIR)/extern/include
INCLUDES = $(EXRINCLUDE) $(MATLABINCLUDE) -I../mex_utils

CC = g++
MEXFLAGS = -DUSE_MATLAB_INTERFACE -DMATLAB_MEX_FILE -D_GNU_SOURCE -fexceptions -fno-omit-frame-pointer
GENERALFLAGS = -fPIC -W -Wall -Wextra -g -pedantic -Wcast-align -Wcast-qual \
				-Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 \
				-Winit-self -Wlogical-op -Wmissing-declarations \
				-Wmissing-include-dirs 
				#-Woverloaded-virtual -Wredundant-decls -Wshadow \
				-Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
				-Wstrict-overflow=5 -Wswitch-default -Wundef
OPTIMFLAGS = -march=native -O3 -ffast-math -fopenmp -pthread
REPORTSFLAGS = -Winline -Wimplicit
DEBUGFLAG = -g
ifdef DEBUG_MODE
	CFLAGS = $(DEBUGFLAG) $(MEXFLAGS) $(GENERALFLAGS) 
else
	CFLAGS = $(MEXFLAGS) $(GENERALFLAGS) $(OPTIMFLAGS)
	ifdef PRODUCE_REPORTS
		CFLAGS += $(REPORTSFLAGS) 
	endif
endif 

LDFLAGS = -pthread -shared -Wl,--version-script,$(MATLABDIR)/extern/lib/$(MATLABARCH)/$(MAPFILE) -Wl,--no-undefined
