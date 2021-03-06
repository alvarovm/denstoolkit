TOP      = $(shell pwd)

# OS Name (Linux or Darwin)
OSUPPER = $(shell uname -s 2>/dev/null | tr [:lower:] [:upper:])
OSLOWER = $(shell uname -s 2>/dev/null | tr [:upper:] [:lower:])

# Flags to detect 32-bit or 64-bit OS platform
OS_SIZE = $(shell uname -m | sed -e "s/i.86/32/" -e "s/x86_64/64/")
OS_ARCH = $(shell uname -m | sed -e "s/i386/i686/")

# Flags to detect either a Linux system (linux) or Mac OSX (darwin)
DARWIN = $(strip $(findstring DARWIN, $(OSUPPER)))

# Extra user flags
OPTIM_CCFLAGS   ?= -O2 -funroll-loops -falign-loops=8 -ffast-math
WARNING_CCFLAGS ?=  -W -Wall -pedantic -Wno-long-long

# OS-specific build flags
ifneq ($(DARWIN),) 
      GCC       := g++-mp-4.9
      CCFLAGS   := -arch $(OS_ARCH) 
else
      GCC       := g++
  ifeq ($(OS_SIZE),32)
      CCFLAGS   := -m32
  else
      CCFLAGS   := -m64
  endif
endif

# Debug build flags
ifeq ($(DEBUGVERSION),1)
      CCFLAGS   += -DDEBUG=1
else
      CCFLAGS   += -DDEBUG=0
endif

# Use OpenMP
DTKUSEOPENMP := 0
ifneq ($(DTKUSEOPENMP),0)
      CCFLAGS   += -DPARALLELISEDTK=$(DTKUSEOPENMP) -fopenmp
else
      CCFLAGS   += -DPARALLELISEDTK=0
endif

CCFLAGS += -std=c++11

OBJDIR        := ../objs
INCSOLDEFS    := -include soldefines.h
PARTDEPS      := crtflnms.h crtflnms.cpp optflags.h optflags.cpp

# Target rules
all: build

build: dtkplane

$(OBJDIR)/solmemhand.o: ../common/solmemhand.cpp soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) -o $@ -c $<

$(OBJDIR)/solscrutils.o: ../common/solscrutils.cpp soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) -o $@ -c $<

$(OBJDIR)/solstringtools.o: ../common/solstringtools.cpp ../common/solstringtools.h soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) -o $@ -c $<

$(OBJDIR)/solfileutils.o: ../common/solfileutils.cpp soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) -o $@ -c $<

$(OBJDIR)/iofuncts-wfx.o: ../common/iofuncts-wfx.cpp ../common/solmemhand.cpp soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) -o $@ -c $<

$(OBJDIR)/iofuncts-wfn.o: ../common/iofuncts-wfn.cpp ../common/solmemhand.cpp soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) -o $@ -c $<

$(OBJDIR)/solmath.o: ../common/solmath.cpp soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) $(OPTIM_CCFLAGS) -o $@ -c $<

$(OBJDIR)/eig2-4.o: ../common/eig2-4.cpp soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) $(OPTIM_CCFLAGS) -o $@ -c $<

$(OBJDIR)/gausswavefunction.o: ../common/gausswavefunction.cpp soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) $(OPTIM_CCFLAGS) -o $@ -c $<

$(OBJDIR)/solpovtools.o: ../common/solpovtools.cpp soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) -o $@ -c $<

$(OBJDIR)/bondnetwork.o: ../common/bondnetwork.cpp soldefines.h #$(OBJDIR)/solpovtools.o
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) $(OPTIM_CCFLAGS) -o $@ -c $<

$(OBJDIR)/wfgrid2d.o: ../common/wfgrid2d.cpp soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) $(OPTIM_CCFLAGS) -o $@ -c $<

$(OBJDIR)/atomradiicust.o: ../common/atomradiicust.cpp ../common/atomradiicust.h soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) $(OPTIM_CCFLAGS) -o $@ -c $<

$(OBJDIR)/atomcolschjmol.o: ../common/atomcolschjmol.cpp ../common/atomcolschjmol.h soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) $(OPTIM_CCFLAGS) -o $@ -c $<

optflags.o: optflags.cpp soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) -o $@ -c $<

crtflnms.o: crtflnms.cpp soldefines.h
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) -o $@ -c $<

dtkplane.o: dtkplane.cpp soldefines.h $(PARTDEPS)
	$(GCC) $(CCFLAGS) $(WARNING_CCFLAGS) $(INCSOLDEFS) -o $@ -c $<

dtkplane: dtkplane.o optflags.o crtflnms.o $(OBJDIR)/solfileutils.o \
  $(OBJDIR)/iofuncts-wfx.o $(OBJDIR)/iofuncts-wfn.o \
  $(OBJDIR)/gausswavefunction.o $(OBJDIR)/solscrutils.o $(OBJDIR)/solmemhand.o \
  $(OBJDIR)/bondnetwork.o $(OBJDIR)/wfgrid2d.o $(OBJDIR)/solpovtools.o \
  $(OBJDIR)/solmath.o $(OBJDIR)/eig2-4.o $(OBJDIR)/solstringtools.o \
  $(OBJDIR)/atomradiicust.o $(OBJDIR)/atomcolschjmol.o
	$(GCC) $(CCFLAGS) $(OPTIM_CCFLAGS) $(INCSOLDEFS) -o $@ $+
clean:
	rm -f *.o dtkplane

distclean: clean
	rm -f $(OBJDIR)/*.o

runben: build
	./dtkplane  benceno_sto3g.wfx -n 60 -a 1 4 6 -c -P -p u
	./dtkplane  benceno_sto3g.wfx -n 60 -a 1 4 6 -c -P -p U

runbenwl: build
	./dtkplane  benceno_sto3g.wfx -n 300 -a 1 4 6 -P -l

