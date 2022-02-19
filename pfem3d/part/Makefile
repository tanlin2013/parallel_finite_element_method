F77	= mpiifort
F90	= mpiifort
FLINKER   = $(F77)
F90LINKER = $(F90)
FLIB_PATH =
INC_DIR   =  
OPTFLAGS = -align array64byte -O3 -axCORE-AVX512
FFLAGS = $(OPTFLAGS)
FLIBS  = -lmetis
#
#TARGET = hpcmw-solver-scalar-test
TARGET = ../mesh/part
default: $(TARGET)
OBJS =\
geofem_util.o partitioner.o input_grid.o main.o \
calc_edgcut.o cre_local_data.o define_file_name.o \
interface_nodes.o metis.o\
neib_pe.o paraset.o proc_local.o local_data.o\
double_numbering.o output_ucd.o util.o

$(TARGET):  $(OBJS)
	$(F90LINKER) $(OPTFLAGS) -o $(TARGET) $(OBJS) $(FLIBS) 
clean:
	/bin/rm -f *.o $(TARGET) *~ *.mod
.f.o:
	$(F90) $(FFLAGS) $(INC_DIR) -c  $*.f
.SUFFIXES: .f
