#---------------------------------------------------------------------------------
# Name of the executable in /usr/bin when installed
#---------------------------------------------------------------------------------
EX_NAME	:=	eplith
LK_NAME :=	ep

#---------------------------------------------------------------------------------
# Compiler executables
#---------------------------------------------------------------------------------
CXX		:=	g++
CXXFLAGS:=	-g -Wno-write-strings -fpermissive -fPIC

#---------------------------------------------------------------------------------
# Any extra libraries you wish to link with your project
#---------------------------------------------------------------------------------
LIBS	:=	-lep

#---------------------------------------------------------------------------------
# List of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	

#---------------------------------------------------------------------------------
# Source folders and executable name
#---------------------------------------------------------------------------------
TARGET	:=  $(shell basename $(CURDIR))
BUILD	:=	build
SOURCES	:=	source
INCLUDES:=	source EPlib/include EPlib/include/EPlib

#---------------------------------------------------------------------------------
# Source files
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
export ADIR 	:=	$(CURDIR)

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

export CPPFILES :=	$(foreach dir,$(SOURCES),$(wildcard $(CURDIR)/$(dir)/*.cpp))

export OFILES	:=	$(CPPFILES:.cpp=.o)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))

export LIBPATHS :=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

#---------------------------------------------------------------------------------
.PHONY: $(BUILD) clean install uninstall
#------------------------------------------------------------------------------
$(BUILD):
	@echo Compiling EPlib...
	@make --no-print-directory -C EPlib
	@sudo make --no-print-directory -C EPlib install
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@make --no-print-directory -C EPlib clean
	@rm -f $(SOURCES)/*.o
	@rm -fr $(BUILD) $(TARGET)

#---------------------------------------------------------------------------------
install:
	@make --no-print-directory -C EPlib install
	@cp -u $(TARGET) /usr/bin/$(EX_NAME)
	@cp -s /usr/bin/$(EX_NAME) /usr/bin/$(LK_NAME)
	@echo installed.

#---------------------------------------------------------------------------------
uninstall:
	@make --no-print-directory -C EPlib uninstall
	@rm -f /usr/bin/$(EX_NAME)
	@rm -f /usr/bin/$(LK_NAME)
	@echo uninstalled.

#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
# Makefile targets
#---------------------------------------------------------------------------------
all: $(OUTPUT)

#---------------------------------------------------------------------------------
$(OUTPUT): $(OFILES)
	@echo built ... $(notdir $@)
	@$(CXX) $(CXXFLAGS) $(OFILES) $(LIBS) -o $@

#---------------------------------------------------------------------------------
%.o: %.cpp
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

