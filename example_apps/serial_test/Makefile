#=========================================================
# Makefile of project "serial_test"
#==========================================================

# Point this to your cross compiler for a given architecture
# I created a link to a buildroot install
C_COMPILE = ../../buildroot/output/host/usr/bin/mipsel-linux-

# Point this to your uClib build for the cross target.  I created
# a link to a openwrt build for a system
RFS_ROOT = ../../openwrt/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2

# Setup output parameters
PATH_TARGET = 
PATH_OBJ    = 
CC = $(C_COMPILE)gcc
CXX = $(C_COMPILE)g++
#STRIP = $(C_COMPILE)strip
NAME = serial_nix
EXEC = $(NAME).bin

RFS_PATH = ./

CFLAGS = -Wall -Wno-unused-function -Wno-unused-variable  -Wno-unused-but-set-variable -Wno-pointer-to-int-cast

DFLAGS = -DDEBUG

# Need to include headers for libraries
INCLUDE  = -I . 
INCLUDE += -I $(RFS_ROOT)/usr/include/

# Need to link in libs for libconfuse
LIBS = -L $(RFS_ROOT)/usr/lib/

# Use libconfuse
LDFLAGS = -lrt -pthread -lz
LDFLAGS += -lconfuse

# List all the files
DEPEND_FILES  =
DEPEND_FILES += $(PATH_OBJ)serial_test.o
#DEPEND_FILES += $(PATH_OBJ)subfolder/example.o

DEBUG	= -g -Wall

.PHONY : all
all: $(EXEC)

$(EXEC):  $(DEPEND_FILES)
	$(CC) $(DFLAG) -s -o $(PATH_TARGET)$@ $(DEPEND_FILES) $(LIBS) $(LDFLAGS)
						
#./subfolder/%.o:%.c
#	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@	$<
	
$(PATH_OBJ)%.o:%.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DFLAGS) -c -o $@	$<

$(PATH_OBJ)%.o:%.cpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(INCLUDE) $(DFLAGS) -c -o $@	$<	

clean:
	rm -f $(PATH_OBJ)*.o
	rm -f $(PATH_TARGET)$(EXEC) 
