PROJECT := cxx_backtrace
CONFIGS := Makefile.config

include $(CONFIGS)

OBJ_DIR := obj/
SRC_DIR := src/
INC_DIR := include/
LIB_DIR := lib/
PREFIX := cxx_backtrace
# BACKTRACE_DIR := libbacktrace/backtrace

LIB := $(LIB_DIR)lib$(PROJECT).so
CUR_DIR := $(shell pwd)

CXX ?=

CFLAGS := -std=c++17
LDFLAGS ?= -L$(BACKTRACE_DIR)/lib64 -Wl,-rpath=$(BACKTRACE_DIR)/lib64
LINK_LIBS ?= -lbacktrace
INCLUDES ?= -I$(INC_DIR) -I$(BACKTRACE_DIR)/include

ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0
else
	CFLAGS += -O3
endif

SRCS := $(notdir $(wildcard $(SRC_DIR)*.cpp $(SRC_DIR)*/*.cpp))
OBJS := $(addprefix $(OBJ_DIR), $(patsubst %.cpp, %.o, $(SRCS)))

all: dirs lib
dirs: $(OBJ_DIR) $(LIB_DIR)
lib: $(LIB)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(LIB): $(OBJS)
	$(CXX) $(LDFLAGS) -fPIC -shared -o $@ $^ $(LINK_LIBS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) $(INCLUDES) $(CFLAGS) -fPIC -c $< -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)/*/%.cpp
	$(CXX) $(INCLUDES) $(CFLAGS) -fPIC -c $< -o $@


.PHONY: clean
clean:
	-rm -rf $(OBJ_DIR) $(LIB_DIR) $(PREFIX)


.PHONY: install
install: all
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	cp -r $(LIB) $(PREFIX)/lib
	cp -r $(INC_DIR)/$(PROJECT).h $(PREFIX)/include