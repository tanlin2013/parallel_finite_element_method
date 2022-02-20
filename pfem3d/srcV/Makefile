include Makefile.in

CFLAGSL  = -I/work/gt00/z30088/ppohVIS/include
LDFLAGSL = -L/work/gt00/z30088/ppohVIS/lib
LIBSL    = -lppohvispfem3d

.SUFFIXES:
.SUFFIXES: .o .c

.c.o:
	$(CC) -c $(CFLAGS) $(CFLAGSL) $< -o $@

TARGET = ../runV/solv

OBJS = \
	test1.o \
	pfem_init.o \
	input_cntl.o \
	input_grid.o \
	define_file_name.o \
	mat_con0.o \
	mat_con1.o \
	mat_ass_main.o \
	mat_ass_bc.o \
	solve11.o \
	solver_CG.o \
	solver_SR.o \
	output_ucd.o \
	pfem_finalize.o \
	allocate.o \
	util.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(CFLAGS) $(CFLAGSL) $(OBJS) $(LDFLAGSL) $(LIBS) $(LIBSL) 
clean:
	rm -f *.o *.mod $(TARGET) *~

distclean:
	rm -f *.o *.mod $(TARGET)
