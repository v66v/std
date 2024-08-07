# Copyright 2024 v66v
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

SRC_DIR ?= src
TYPE ?= release
BUILDDIRBASE := build/
BUILDDIR := $(BUILDDIRBASE)$(TYPE)
DESTDIR ?= $(CURDIR)/$(BUILDDIR)
export DESTDIR

INC_DIRS = $(SRC_DIR)/
INCS := $(addprefix -I,$(INC_DIRS))
SRC_FILES := $(shell find $(SRC_DIR)/ -name "*.c")

HDR_FILES := $(shell find $(SRC_DIR)/ -name "*.h")
HDR_DIR := $(BUILDDIR)/include/std
HDRS := $(HDR_FILES:$(SRC_DIR)/%=$(HDR_DIR)/%)

OBJS_DIR = $(BUILDDIR)/objects
OBJS := $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.lo)
DEPS := $(OBJS:.lo=.d)
-include $(DEPS)

PKGCONFIGS_CFLAGS :=
PKGCONFIGS_LIBS :=

ifneq ($(PKGCONFIGS_CFLAGS),)
	PKGCONFIG_CFLAGS = $(shell pkg-config --cflags $(PKGCONFIGS_CFLAGS))
endif

ifneq ($(PKGCONFIGS_LIBS),)
	PKGCONFIG_LIBS = $(shell pkg-config --libs $(PKGCONFIGS_LIBS))
endif

LIB_NAMES :=
LIBS := $(addprefix -l,$(LIB_NAMES))
LIBS += $(PKGCONFIG_LIBS)

CFLAGS = -MMD -MP -std=gnu11
CFLAGS += $(PKGCONFIG_CFLAGS)

ifeq ($(TYPE), debug)
	CFLAGS += -O2 -DDEBUG -ggdb -Wall -Wextra -Wswitch-enum -Wswitch-default
else
	CFLAGS += -O3
endif

CC := gcc
LIBTOOL := libtool
LIBTOOLFLAGS := --silent
LIBTOOL_TYPE := # -static or -shared
LIBTOOL_CFLAGS := -version-info 1:1:0

LIB_NAME := std

.DEFAULT_GOAL := all
all: build
build: build_info dirs makefile $(BUILDDIR)/lib/lib$(LIB_NAME).la $(HDRS)

check:
	@$(MAKE) -sC tests

$(HDRS):
	@cp $(SRC_DIR)/$(@F) $@

$(BUILDDIR)/lib/lib$(LIB_NAME).la: $(OBJS)
	$(info [CC] Linking object files)
	@$(LIBTOOL) $(LIBTOOLFLAGS) --tag=CC --mode=link $(CC) $(LIBTOOL_TYPE) \
		$(LIBTOOL_CFLAGS) $(LIBS) -o $(OBJS_DIR)/lib$(LIB_NAME).la -rpath $(DESTDIR)/lib $^
	@$(LIBTOOL) $(LIBTOOLFLAGS) --mode=install install -c \
		$(OBJS_DIR)/lib$(LIB_NAME).la $(DESTDIR)/lib/lib$(LIB_NAME).la

$(OBJS_DIR)/%.lo: $(SRC_DIR)/%.c makefile
	$(info [CC] $<)
	@$(LIBTOOL) $(LIBTOOLFLAGS) --tag=CC --mode=compile $(CC) $(CFLAGS) $(INCS) -o $@ -c $<

install: install_info build
ifneq ($(BUILDDIR), $(DESTDIR))
	@cp -r $(BUILDDIR)/lib/ $(DESTDIR)/
	@cp -r $(BUILDDIR)/include/ $(DESTDIR)/
else
	$(info [WRN] BUILDDIR($(BUILDDIR)) == DESTDIR($(DESTDIR)))
endif

dirs: $(BUILDDIR)/lib/ $(DESTDIR)/lib/ $(HDR_DIR)/ $(dir $(OBJS))

.PRECIOUS: $(BUILDDIR)/. $(BUILDDIR)/%/.
$(BUILDDIR)/:
	$(info [INFO] Creating directory [$@])
	@mkdir -p $@

$(DESTDIR)/%/:
	$(info [INFO] Creating directory [$@])
	@mkdir -p $@

$(BUILDDIR)/%/:
	$(info [INFO] Creating directory [$@])
	@mkdir -p $@

.PHONY: clean ccls debug_info build_info install_info
build_info:
ifeq ($(TYPE), debug)
	$(info [INFO] Compiling In Debug Mode [$(BUILDDIR)/lib/$(LIB_NAME)])
else
	$(info [INFO] Building library [$(BUILDDIR)/lib/$(LIB_NAME)])
endif

install_info:
	$(info [INFO] Installing Library [$(DESTDIR)/lib/$(LIB_NAME)])

clean:
	$(info [INFO] Cleaning [$(notdir $(CURDIR))])
	@rm -rf $(BUILDDIRBASE)/
	@$(MAKE) -sC tests clean

ccls:
	$(info [INFO] Creating .ccls file)
	@printf "%s\\n" $(CC) $(subst ' ','\n',$(LIBS)) $(CFLAGS) \
			$(subst ' ','\n',$(INCS)) -I$(subst :, -I,$(C_INCLUDE_PATH)) > .ccls
