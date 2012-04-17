#---------------------------------------------------------------------------------
# Name of the shared library
#---------------------------------------------------------------------------------
VERSION	:= 1
SO_NAME	:= libep.so.$(VERSION)
UL_NAME	:= libep.so

#---------------------------------------------------------------------------------
# Compiler executables
#---------------------------------------------------------------------------------
CXX 	:=	g++
CXXFLAGS:=	-g -Wno-write-strings -fpermissive -fPIC

#---------------------------------------------------------------------------------
# Any extra libraries you wish to link with your project
#---------------------------------------------------------------------------------
LIBS	:=	-lfl

#---------------------------------------------------------------------------------
# List of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS :=	

#---------------------------------------------------------------------------------
# Source folders and executable name
#---------------------------------------------------------------------------------
TARGET	:=	$(SO_NAME)
BUILD	:=	build
SOURCES :=	source
INCLUDES:=	source include include/EPlib build

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
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -f $(SOURCES)/*.o
	@rm -fr $(BUILD) $(TARGET)

#---------------------------------------------------------------------------------
install:
	@cp -u $(TARGET) /usr/lib/$(SO_NAME)
	@cp -u -f -s /usr/lib/$(SO_NAME) /usr/lib/$(UL_NAME)
	@cp -u include/EPlib.h /usr/include/EPlib.h
	@mkdir -p /usr/include/EPlib
	@cp -u include/EPlib/*.h /usr/include/EPlib/
	@echo installed.

#---------------------------------------------------------------------------------
uninstall:
	@rm -f /usr/lib/$(SO_NAME)
	@rm -f /usr/lib/$(UL_NAME)
	@rm -f /usr/include/EPlib.h
	@rm -fr /usr/include/EPlib
	@echo uninstalled.

#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
# Makefile targets
#--------------------------------------------------------------------------------
all: $(OUTPUT)

#---------------------------------------------------------------------------------
$(OUTPUT): parser.tab.cpp lex.yy.cpp $(OFILES)
	@$(CXX) $(CXXFLAGS) -shared parser.tab.cpp lex.yy.cpp $(OFILES) $(INCLUDE) $(LIBS) -o $@
	@echo built ... $(notdir $@)

#---------------------------------------------------------------------------------
lex.yy.cpp: $(ADIR)/$(SOURCES)/lexer.l
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@flex -o $@ $<

#---------------------------------------------------------------------------------
parser.tab.cpp: $(ADIR)/$(SOURCES)/parser.y
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@bison -d -v -o $@ $<

#---------------------------------------------------------------------------------
%.o: %.cpp
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

