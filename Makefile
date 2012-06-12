
# Generic makefile to build/install a static library (zlib example)
ARCH      = linux
CFLAGS    = -O3 -Wall

# List of source code directories
SOURCES   = src

TESTSOURCES = test

#test utils
# List of header files to install
INCLUDES  = src/hexdump.h
# Library to create
LIBNAME   = libhexdump.a
SHORTLIBNAME = hexdump

############################################################

BUILD_DIR   = build/$(ARCH)

# Full path to the built library
TARGET      = $(BUILD_DIR)/$(LIBNAME)

TESTTARGET  = $(BUILD_DIR)/test/testapp

prefix = .
exec_prefix = prefix
libdir = $(prefix)/lib/$(ARCH)
includedir = $(prefix)/include
INSTALL_PROGRAM = install -D
INSTALL_DATA = $(INSTALL_PROGRAM) -m 644

CFILES    := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
OBJECTS   := $(addprefix $(BUILD_DIR)/,$(CFILES:.c=.o))
TESTCFILES := $(foreach dir,$(TESTSOURCES),$(wildcard $(dir)/*.c))
TESTOBJECTS := $(addprefix $(BUILD_DIR)/,$(TESTCFILES:.c=.o))
DEPENDS   := $(OBJECTS:.o=.d)

.PHONY: all installdirs install uninstall clean

# Default
default: $(DEPENDS) $(TARGET)

test: default install $(TESTTARGET)
	$(TESTTARGET)

# Build the dependency files
# (GNU Make Manual 4.14 Generating Prerequisites Automatically)
$(BUILD_DIR)/%.d: $(BUILD_DIR)
	@echo "build dep for $*.c as $@"
	@$(CC) -M $(CFLAGS) $*.c > $@.tmp
	@sed s~.*:~"$(BUILD_DIR)/$*.o $@:"~ $@.tmp > $@
	@rm $@.tmp

# Link all changed object files into static library
$(TARGET): $(OBJECTS)
	$(AR) -rc $(TARGET) $?

$(TESTTARGET): $(TESTOBJECTS)
	$(CC) -o $(TESTTARGET) $? -l$(SHORTLIBNAME) -lcunit -L$(libdir)

# Compile C source to object code
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(includedir) -c $< -o $@

# Create the necessary directory tree for the build
$(BUILD_DIR):
	@for p in $(SOURCES); do mkdir -p $(BUILD_DIR)/$$p; done
	@for p in $(TESTSOURCES); do mkdir -p $(BUILD_DIR)/$$p; done

# Create the necessary directory tree for installation
installdirs:
	@mkdir -p $(libdir)
	@mkdir -p $(includedir)

# Install the library and headers
install: default installdirs
	$(INSTALL_DATA) $(TARGET) $(libdir)
	for p in $(INCLUDES); do $(INSTALL_DATA) $$p $(includedir); done

# Remove the library and headers
uninstall:
	rm -f $(libdir)/$(LIBNAME)
	for p in $(notdir $(INCLUDES)); do rm -f $(includedir)/$$p; done

# Remove all build files
clean:
	rm -fr build lib $(includedir) src/*~ test/*~

# Pull in the dependencies if they exist
# http://scottmcpeak.com/autodepend/autodepend.html
-include $(DEPENDS)
