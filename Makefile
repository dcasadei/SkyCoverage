# Diego Casadei (http://cern.ch/casadei/)
# `SkyCoverage' version 20051216
#
#  See D. Casadei,
# `Sky coverage of orbital detectors.  Semi-analytical approach',
#  STILL UNPUBLISHED!.
#
# This was developed on Linux with gcc compiler.
#
CXX = g++
CXXFLAGS = -Wall -O2 # -DDEBUG

LD = $(CXX)
LDFLAGS =

EXE = SkyCoverage
LIB = ExposureFraction.C ChangeRefSys.C ExposureTime.C
SRC = SkyCoverage.C $(LIB)
OBJ = $(SRC:.C=.o)
TGZ = SkyCoverage20051216.tgz
INC = $(LIB:.C=.h)
ADD = Makefile README

#--------------------------

help:
	@echo " "
	@echo "  This is \`SkyCoverage' version 20051216,"
	@echo "  by Diego Casadei (http://cern.ch/casadei/)."
	@echo " "
	@echo "  For a full explanation, please refer to:"
	@echo "   D. Casadei,"
	@echo "   \`Sky coverage of orbital detectors.  Semi-analytical approach',"
	@echo "   arXiv: astro-ph/0511674."
	@echo " "
	@echo "  make all           # produces $(EXE)"
	@echo "  make clean         # deletes object files"
	@echo "  make cleanall      # deletes objects and executable"
	@echo "  make dist          # produces the $(TGZ) archive"
	@echo " "


# build an exe from object files and libraries
all : $(OBJ)
	${LD} $(LDFLAGS)  $(OBJ) -o $(EXE)

# compile a .C
%.o : %.C ;
	$(CXX) $(CXXFLAGS) -c $<

dist:
	tar cvzf $(TGZ) $(SRC) $(INC) $(ADD)

clean:
	rm -f $(OBJ)

cleanall: clean
	rm -f $(EXE)
