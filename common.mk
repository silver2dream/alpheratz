#.PHONY:all clean

# GCC is responsible for compiling C language, while g++ is used to compile C++. 
# The -g option is used to display debugging information.
ifeq ($(TARGET),debug)
	CC = g++ -std=c++11 -g -Wall -Wextra
	VERSION = debug
else
	CC = g++ -std=c++11 -Wall -Wextra
	VERSION = release
endif

# The wildcard is a feature in computing that allows users to use special characters to represent one or more characters in a file name, path, or search pattern.
SRCS_CXX = $(wildcard *.cxx)

OBJS_CXX = $(SRCS_CXX:.cxx=.o)

OBJS = $(OBJS_CXX) 

DEPS_CXX = $(SRCS_CXX:.cxx=.d)

DEPS = $(DEPS_CXX) 

# In the context of a Makefile,
# using ":=" ensures that the variable is assigned immediately during Makefile processing,
# whereas using "=" assigns the variable at runtime. 
LIB := $(addprefix $(BUILD_ROOT)/,$(LIB))

LINK_OBJ_DIR = $(BUILD_ROOT)/app/link_obj
DEP_DIR = $(BUILD_ROOT)/app/dep

$(shell mkdir -p $(LINK_OBJ_DIR))
$(shell mkdir -p $(DEP_DIR))

OBJS:= $(addprefix $(LINK_OBJ_DIR)/,$(OBJS))
DEPS:= $(addprefix $(DEP_DIR)/,$(DEPS))

LINK_OBJ = $(wildcard $(LINK_OBJ_DIR)/*.o)
LINK_OBJ += $(OBJS)

all:$(DEPS) $(OBJS) $(LIB)

ifneq ("$(wildcard $(DEPS))","")
include $(DEPS)
endif

$(LIB):$(LINK_OBJ)
	@echo "------------------------build $(VERSION) mode--------------------------------"
	
	ar rcs $@ $^

$(LINK_OBJ_DIR)/%.o:%.cxx
	$(CC) -I$(INCLUDE_PATH) -o $@ -c $(filter %.cxx,$^)

$(DEP_DIR)/%.d:%.cxx
	echo -n $(LINK_OBJ_DIR)/ > $@
	$(CC) -I$(INCLUDE_PATH)  -MM $^ >> $@
