# C++ compiler
CXX=clang++

# Includes
IDIR=src
# Source
SDIR=src
# Objects
ODIR=obj

# Compiler flags
CXXFLAGS=-I$(IDIR)

# Dependancies to track
_DEPS = fileHandling.h dbtproj.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# Objects to track
_OBJ = fileHandling.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# Object rule
$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# make rule
dbms: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# Phony rule
.PHONY:
	clean

# Clean rule
clean:
	rm -f $(ODIR)/*.o *~ core