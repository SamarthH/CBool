IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) -Wall 
EXTRAFLAGS= -g

ODIR=obj
LDIR =../lib

LIBS= -fopenmp -lgsl -lgslcblas -lm

_DEPS = AsynchronousUpdate.h constants.h final_list.h inputMethod.h parseMatrix.h requirements.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = client.o AsynchronousUpdate.o final_list.o inputMethod.o parseMatrix.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -L$(LDIR) -c -o $@ $< $(CFLAGS) $(EXTRAFLAGS) $(LIBS)

boolean_updates: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRAFLAGS) $(LIBS)

	mv boolean_updates ../

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ 

nest:
	set OMP_DYNAMIC=true
	set OMP_NESTED=true 
	
