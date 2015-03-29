CC = g++
LD = g++
GENERALFLAGS = -fPIC -W -Wall -Wextra -g -pedantic -std=c++11 -fdiagnostics-color=always
OPTIMFLAGS = -march=native -Ofast -fopenmp -pthread -pipe -flto -fuse-linker-plugin -fuse-ld=gold
#'-ftree-vectorize', '-mfpmath=sse', '-funsafe-math-optimizations', '-fno-rounding-math', '-fno-signaling-nans', '-fno-math-errno', '-fomit-frame-pointer'
REPORTSFLAGS = -Winline -Wimplicit
DEBUGFLAG = -g
CFLAGS += $(DEBUGFLAG) $(GENERALFLAGS) $(OPTIMFLAGS)
ifeq ($(DEBUG_MODE), 0)
	CFLAGS += -DNDEBUG
endif
ifeq ($(PRODUCE_REPORTS), 1)
	CFLAGS += $(REPORTSFLAGS) 
endif
