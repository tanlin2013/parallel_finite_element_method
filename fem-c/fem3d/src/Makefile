CC 	    = gcc
CCFLAGS     = -O3
LDFLAGS     = -lm

LD          = $(CC)

.SUFFIXES:
.SUFFIXES: .o .c

.c.o:
	$(CC) -c $< $(CCFLAGS) $(LDFLAGS)

TARGET = ../run/sol

all: $(TARGET)

OBJS = \
	solver_CG.o \
	solve11.o \
	test1.o \
	util.o \
	input_cntl.o \
	input_grid.o \
	mat_con0.o \
	mat_con1.o \
	mat_ass_main.o \
	mat_ass_bc.o \
	output_ucd.o \
	allocate.o

$(TARGET): $(OBJS)
	$(LD) $(CCFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	/bin/rm -f *.o $(TARGET) *~ *.mod

