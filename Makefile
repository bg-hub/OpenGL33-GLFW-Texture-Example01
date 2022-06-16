
CC          = g++
EXTENSION   = cpp
HEADER_EXT  = hpp
GUI         = glfw3
WARNINGS    = -Wall 
COMPILEOPTS = -std=c++11 -O2 
GLES_HEADER = 
GLES_LIBS_TO_USE =  -lGL
AUX_CFLAGS  = 
AUX_LIBS    =
CFLAGS      = $(WARNINGS) $(COMPILEOPTS) `pkg-config $(GUI) --cflags ` $(AUX_CFLAGS)
LDFLAGS     = `pkg-config $(GUI) --libs`  -no-pie
GLES_PATH   = 
GLES_LIBS   = $(GLES_LIBS_TO_USE) $(AUX_LIBS)
NAME        = demo
CONFDIR     = conf
SRCDIR      = src
INCDIR      = inc
OBJDIR      = obj
RESDIR      = res
INCPATH     = -I$(INCDIR) -I$(RESDIR)
SYSINCL     = -isystem 
SOURCES  := $(shell find $(SRCDIR) -name '*.$(EXTENSION)')
OBJ      := $(SOURCES:$(SRCDIR)/%.$(EXTENSION)=$(OBJDIR)/%.o)


compile: $(NAME)


-include $(OBJDIR)/*.d

$(NAME):  $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(GLES_PATH) $(LDFLAGS) $(GLES_LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.$(EXTENSION)
	mkdir -p $(OBJDIR) 
	$(CC)  $(INCPATH) $(CFLAGS)  -c $< -o $@ 
	$(CC) -MM $(INCPATH) $(GLES_HEADER) -MT '$(OBJDIR)/$(basename $(notdir $<)).o' -c $<  > $(OBJDIR)/$(basename $(notdir $<)).d

clean:
	rm  -f $(OBJDIR)/*.o $(OBJDIR)/*.d $(NAME)

.PHONY: compile clean
