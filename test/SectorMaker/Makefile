CXX = g++
LD  = g++
CFLAGS = -Wall -g 

INCS = $(ROOTSYS)/include/ $(PWD)/tclap-1.2.1/include/ .

%.o: %.cxx 
	@echo "*"
	@echo "* compile "$@
	@echo "*"
	$(CXX) $(CFLAGS) $(addprefix -I, $(INCS)) -c $< -o $@

sectorMaker:main.o rates.o sector.o efficiencies.o sector_test.o jobparams.o 
	@echo "Build sectorMaker tool" 
	$(LD) $^ $(shell $(ROOTSYS)/bin/root-config --libs) -o $@

all : sectorMaker

clean: 
	\rm *.o	