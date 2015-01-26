CC = gcc 
CXX = g++ -std=c++11 
AS  = as
LDFLAGS = -lgmp -lgmpxx -fopenmp -lGL -lglut -lGLU

BIN = NOARCH.x

#
# include list for the Columbia code
#
INCLIST = 

CFLAGS= 
CXXFLAGS= 
ASFLAGS= 
DFLAGS += 

all:$(BIN)

.SUFFIXES:
.SUFFIXES:  .o .C .S .c

CSRC :=$(wildcard *.c)
CCSRC :=$(wildcard *.C)
SSRC :=$(wildcard *.S)

COBJ=$(CSRC:.c=.o)
CCOBJ=$(CCSRC:.C=.o)
SOBJ=$(SSRC:.S=.o)

OBJS_SRC = $(SOBJ) $(CCOBJ) $(COBJ)
OBJS := $(notdir $(OBJS_SRC))

all : $(BIN)
	make -C AI-hs

$(BIN):  $(OBJS) $(LIBLIST)
	@echo OBJS = $(OBJS)
	$(CXX) $(OBJS) $(LIBLIST) $(LDFLAGS) -o $(BIN)

.c.o:
	$(CC) -o $@ $(CFLAGS) $(DFLAGS) -c $(INCLIST) $<
.C.o:
	$(CXX) -o $@ $(CXXFLAGS) $(DFLAGS) -c $(INCLIST) $<
.S.o:
	$(AS) -o $@ $(ASFLAGS) -c $(INCLIST) $<

cps:
	$(MAKE) -C $(BUILDDIR)

clean:
	rm -f *.o $(BIN) || :
	make -C AI-hs clean
