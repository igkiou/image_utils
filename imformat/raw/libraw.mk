LIBRAWDIR = /home/igkiou/cpp/LibRaw-0.17.0-Alpha4/build

LIBRAWLIBS = -L$(LIBRAWDIR)/lib -lraw_r -lraw
LIBS += $(LIBRAWLIBS)

LIBRAWINCLUDE= -I$(LIBRAWDIR)/include/libraw
INCLUDES += $(LIBRAWINCLUDE)
