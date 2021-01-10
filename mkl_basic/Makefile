MAIN=$(notdir $(CURDIR))

export MKL_ROOT='C:\Program Files (x86)\Intel\oneAPI\mkl\2021.1.1'

MKL_OPTS+=-lmkl_core -lmkl_intel_ilp64 -lmkl_blas95_ilp64
MKL_OPTS+=-fopenmp -lpthread -lm

CC_OPTS+=$(MKL_OPTS)

include ../Makefile 
