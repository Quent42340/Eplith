#---------------------------------------------------------------------------------
# Name of the shared library
#---------------------------------------------------------------------------------
UL_NAME	:= libep.a

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
INCLUDES:=	source include include/EPlib build

#---------------------------------------------------------------------------------
# Source files
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
export ADIR 	:=	$(CURDIR)

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
	@cp -u include/EPlib.h /usr/include/EPlib.h
	@mkdir -p /usr/include/EPlib
	@cp -u include/EPlib/*.h /usr/include/EPlib/
	@echo installed.

#---------------------------------------------------------------------------------
uninstall:
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
$(OUTPUT): parser.tab.o lex.yy.o $(OFILES)
	@ar rcu $@ parser.tab.o lex.yy.o $(OFILES)
	@echo built ... $(notdir $@)

#---------------------------------------------------------------------------------
lex.yy.o: $(ADIR)/$(SOURCES)/lexer.l
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@flex -o lex.yy.cpp $<
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c lex.yy.cpp -o $@

#---------------------------------------------------------------------------------
parser.tab.o: $(ADIR)/$(SOURCES)/parser.y
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@bison -d -v -o parser.tab.cpp $<
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c parser.tab.cpp -o $@

#---------------------------------------------------------------------------------
%.o: %.cpp
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

