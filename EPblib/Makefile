#---------------------------------------------------------------------------------
# Name of the shared library
#---------------------------------------------------------------------------------
UL_NAME	:= libepb.a

#---------------------------------------------------------------------------------
# Compiler executables
#---------------------------------------------------------------------------------
CXX 	:=	g++
CXXFLAGS:=	-g -Wno-write-strings -fpermissive

#---------------------------------------------------------------------------------
# Source folders and executable name
#---------------------------------------------------------------------------------
TARGET	:=	$(UL_NAME)
BUILD	:=	build
SOURCES :=	source
INCLUDES:=	source include include/EPblib build ../EPlib/include ../EPlib/include/EPlib

#---------------------------------------------------------------------------------
# Source files
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

export CPPFILES :=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(CURDIR)/$(dir)/*.cpp)))

export OFILES	:=	$(CPPFILES:.cpp=.o)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))

#---------------------------------------------------------------------------------
.PHONY: $(BUILD) clean install uninstall
#------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET)

#---------------------------------------------------------------------------------
install:
	@cp -u $(TARGET) /usr/lib/$(UL_NAME)
	@cp -u include/EPblib.h /usr/include/EPblib.h
	@mkdir -p /usr/include/EPblib
	@cp -u include/EPblib/*.h /usr/include/EPblib/
	@echo installed.

#---------------------------------------------------------------------------------
uninstall:
	@rm -f /usr/lib/$(UL_NAME)
	@rm -f /usr/include/EPblib.h
	@rm -fr /usr/include/EPblib
	@echo uninstalled.

#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
# Makefile targets
#--------------------------------------------------------------------------------
all: $(OUTPUT)

#---------------------------------------------------------------------------------
$(OUTPUT): $(OFILES)
	@ar rcu $@ $(OFILES)
	@echo built ... $(notdir $@)

#---------------------------------------------------------------------------------
%.o: %.cpp
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

