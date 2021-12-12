###########################	
#	Author:		Žan Šadl-Ferš		#
#	Version:		1.0-stable				#
#	Date:			2021						#
#	Copyright:	MIT							#
###########################

#####################
#	Setup process	#
#####################

.PHONY: compile recompile generate_assembly doc archive help clean

#	Variables reserved for the compiling program.
MAIN_PROGRAM := main.c
OUTPUT_EXECUTABLE := cmpfiles

#	Directories used during the compiling process.
SOURCES_DIRECTORY := implementation
HEADERS_DIRECTORY := interface
OBJECTS_DIRECTORY := object_files
EXECUTABLE_DIRECTORY := executable

#	Install filepaths. Should be set by the user! If you install the software without settings this properly, you may encounter trouble.
#INSTALL_DIRECTORY := /usr/local

#	Files needed.
SOURCE_FILES := $(wildcard $(SOURCES_DIRECTORY)/*.c)
OBJECT_FILES := $(addprefix $(OBJECTS_DIRECTORY)/, $(notdir $(SOURCE_FILES:.c=.o)))
ASSEMBLY_FILES := $(addprefix $(EXECUTABLE_DIRECTORY)/, $(notdir $(SOURCE_FILES:.c=.s)))

#	Default compiler flags (currently optimized for gcc). For different builds, the flags and compiler should be set by the developer or a skilled user.
CC := gcc
LDFLAGS := -I$(HEADERS_DIRECTORY)
OPTIMIZATION_FLAGS := -O2
ARCHITECTURE_FLAGS := -march=native -mtune=native
SECURITY_FLAGS := -fstack-protector-all -D_FORTIFY_SOURCE=2 -Werror=format-security
CFLAGS := $(OPTIMIZATION_FLAGS) $(ARCHITECTURE_FLAGS) $(SECURITY_FLAGS) -Wall -std=c99



#####################
#	Build options	#
#####################

#	Create a build of the program.
compile: $(MAIN_PROGRAM) $(OBJECT_FILES) $(EXECUTABLE_DIRECTORY)
	$(CC) $(MAIN_PROGRAM) $(OBJECT_FILES) -o $(EXECUTABLE_DIRECTORY)/$(OUTPUT_EXECUTABLE) $(LDFLAGS) $(CFLAGS)
	$(EXECUTABLE_DIRECTORY)/$(OUTPUT_EXECUTABLE) -h > $(EXECUTABLE_DIRECTORY)/$(OUTPUT_EXECUTABLE)_help.txt

#	Recompile the program build.
recompile: clean compile

#	Generate the assembly file of the main program and (if they aren't) of the source files.
generate_assembly: $(MAIN_PROGRAM) $(ASSEMBLY_FILES) $(EXECUTABLE_DIRECTORY)
	$(CC) -S -fverbose-asm $(MAIN_PROGRAM) -o $(EXECUTABLE_DIRECTORY)/$(OUTPUT_EXECUTABLE).s $(LDFLAGS) $(CFLAGS)
	
#$(INSTALL_DIRECTORY):
#	@mkdir -p $@

#	Create the directory that will house the compiled executable, if it doesn't exist.
$(EXECUTABLE_DIRECTORY):
	@mkdir -p $@

#############################################
#	Installing and un-installing options	#
#############################################

#install: compile
#	@mkdir -p $(INSTALL_DIRECTORY)
#	
#uninstall:

#####################################
#	Object/Assembly file compiling process	#
#####################################

#	Object file compilation.
$(OBJECTS_DIRECTORY)/%.o: $(SOURCES_DIRECTORY)/%.c | $(OBJECTS_DIRECTORY)
	$(CC) $(LDFLAGS) $(CFLAGS) -c $< -o $@ 

#	Generate assembly files from the source files.
$(EXECUTABLE_DIRECTORY)/%.s: $(SOURCES_DIRECTORY)/%.c | $(EXECUTABLE_DIRECTORY)
	$(CC) -S -fverbose-asm $< -o $@ $(LDFLAGS) $(CFLAGS)

#	Create the directory that will house the compiled object files, if it doesn't exist.
$(OBJECTS_DIRECTORY):
	@mkdir -p $@



#############################
#	Alternative options		#
#############################

#	Generates the documentation (in HTML) of this program and its source code. So far, only Doxygen is supported.
doc:
	@mkdir -p documentation
	@doxygen Doxyfile
	
#	Creates a tar archive, that is compressed using the bzip2 compressor tool. Best used for source code storage, or for distribution.
archive:
	@tar --create --sparse --bzip2 --file=CompareFiles.tar.bz2 *
	

#	Shows the documentation of this programs makefile.
help:
	@echo ''
	@echo 'This makefile is tested and supported by the C compiler GCC 9.3.0 .'
	@echo ''
	@echo 'Usage:'
	@echo '    make <target> <optional flags>'
	@echo ''
	@echo 'Targets:'
	@echo '    compile              Create a build of the program. Default target.'
	@echo '    recompile            Recompile the program build.'
	@echo '    generate_assembly    Generate assembly files from the source files.'
	@echo '    help                 Shows the documentation of this programs makefile.'
	@echo '    clean                Delete all compiled object files and executables.'
	@echo '    doc                  Generate the documentation of this program.'
	@echo '    archive              Generate a tar.bz2 archive of the source code'
	@echo ''
	@echo 'Optional flags:'
	@echo '    CC                   Used C compiler for this build (by default: $(CC)).'
	@echo '    LDFLAGS              Linker flags (by default: $(LDFLAGS)).'
	@echo '    EXECUTABLE_DIRECTORY The directory in which the compiled binary is (by default: $(EXECUTABLE_DIRECTORY)).'
	@echo '    OPTIMIZATION_FLAGS   Optimization flags (by default: $(OPTIMIZATION_FLAGS)).'
	@echo '    ARCHITECTURE_FLAGS   CPU architecture flags (by default: $(ARCHITECTURE_FLAGS)).'
	@echo '    SECURITY_FLAGS       Enhanced security flags (by default: $(SECURITY_FLAGS)).'
	@echo ''
	@echo 'Set C compiler flags by default:'
	@echo '    $(LDFLAGS) $(CFLAGS)'

#	Delete all compiled object files and executables.
clean:
	-rm -f $(OBJECT_FILES) $(EXECUTABLE_DIRECTORY)/$(OUTPUT_EXECUTABLE)
