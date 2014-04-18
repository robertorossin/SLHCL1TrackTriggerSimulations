CXX = g++
LD  = g++
CFLAGS = -Wall -g -std=c++11

INCS = $(ROOTSYS)/include/ $(PWD)/tclap-1.2.1/include/ .

%.o: %.cxx 
	@echo "*"
	@echo "* compile "$@
	@echo "*"
	$(CXX) $(CFLAGS) $(addprefix -I, $(INCS)) -c $< -o $@

AM_ana:main.o rates.o patterngen.o efficiencies.o sector_test.o track_eff.o jobparams.o 
	@echo "Build sectorMaker tool" 
	$(LD) $^ $(shell $(ROOTSYS)/bin/root-config --libs) -o $@

all : AM_ana

clean: 
	\rm *.o	