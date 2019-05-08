# library name
lib.name = mpe

# input source file (class name == source file basename)
class.sources = Source/mpe.cpp

# all extra files to be included in binary distribution of the library
datafiles = bin/mpe-help.pd bin/mpeTest.pd


# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
PDLIBBUILDER_DIR=pd-lib-builder/
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder
