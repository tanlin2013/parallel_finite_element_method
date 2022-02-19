CC     = mpiicc
LIB_DIR=
INC_DIR=  
OPTFLAGS= -O3 -axCORE-AVX512 -align
LIBS = 
LFLAGS= 
#
TARGET = ../run/sol
default: $(TARGET)
OBJS =\
	test1.o\
	pfem_init.o\
	input_cntl.o\
	input_grid.o\
	define_file_name.o\
	mat_con0.o\
	mat_con1.o\
	mat_ass_main.o\
	mat_ass_bc.o\
	solve11.o\
	solver_CG.o\
	solver_SR.o\
	output_ucd.o\
	pfem_finalize.o\
	allocate.o\
	util.o

$(TARGET):  $(OBJS)
	$(CC) $(OPTFLAGS) -o $@ $(OBJS) $(LFLAGS)
.c.o:
	$(CC) $(OPTFLAGS) -c  $*.c
clean:
	/bin/rm -f *.o $(TARGET) *~ *.mod
