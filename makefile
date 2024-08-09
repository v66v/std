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
NOTPATH := -not -path "$(SRC_DIR)/extra/*"
SRC_FILES := $(shell find $(SRC_DIR)/ $(NOTPATH) -name "*.c")

HDR_FILES := $(shell find $(SRC_DIR)/ -name "*.h")
HDR_DIR := $(BUILDDIR)/include/std
HDRS := $(HDR_FILES:$(SRC_DIR)/%=$(HDR_DIR)/%)

FONTCONFIG_SRC_FILES := $(shell find $(SRC_DIR)/extra/fontconfig -name "*.c")

OBJS_DIR = $(BUILDDIR)/objects
OBJS := $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.lo)

FONTCONFIG_OBJS := $(FONTCONFIG_SRC_FILES:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.lo)

DEPS := $(OBJS:.lo=.d) $(FONTCONFIG_OBJS:.lo=.d)
-include $(DEPS)

FONTCONFIG_CFLAGS := $(shell pkg-config --cflags fontconfig)

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
TARGET_LIBS := libstd extra/libstd-fontconfig
LIBNS := $(TARGET_LIBS:%=$(BUILDDIR)/lib/%.la)

.DEFAULT_GOAL := all
all: build
build: build_info dirs hdrs $(LIBNS) makefile

check:
	@$(MAKE) -sC tests

hdrs: header_info $(HDRS)
$(HDR_DIR)/%.h:
	@cp $(SRC_DIR)/$*.h $(HDR_DIR)/$*.h

$(BUILDDIR)/lib/libstd.la: $(OBJS)
	$(info [CC] Linking object files [libstd])
	@$(LIBTOOL) $(LIBTOOLFLAGS) --tag=CC --mode=link $(CC) $(LIBTOOL_TYPE) \
		$(LIBTOOL_CFLAGS) $(LIBS) -o $(OBJS_DIR)/libstd.la -rpath $(DESTDIR)/lib $^
	@$(LIBTOOL) $(LIBTOOLFLAGS) --mode=install install -c \
		$(OBJS_DIR)/libstd.la $(DESTDIR)/lib/libstd.la

$(BUILDDIR)/lib/extra/libstd-fontconfig.la: $(FONTCONFIG_OBJS)
	$(info [CC] Linking object files [libstd-fontconfig])
	@$(LIBTOOL) $(LIBTOOLFLAGS) --tag=CC --mode=link $(CC) $(LIBTOOL_TYPE) \
		$(LIBTOOL_CFLAGS) $(FONTCONFIG_CFLAGS) \
		-o $(OBJS_DIR)/libstd-fontconfig.la -rpath $(DESTDIR)/lib/extra $^
	@$(LIBTOOL) $(LIBTOOLFLAGS) --mode=install install -c \
		$(OBJS_DIR)/libstd-fontconfig.la $(DESTDIR)/lib/extra/libstd-fontconfig.la

$(OBJS_DIR)/%.lo: $(SRC_DIR)/%.c makefile
	$(info [CC] $<)
	@$(LIBTOOL) $(LIBTOOLFLAGS) --tag=CC --mode=compile $(CC) $(CFLAGS) $(INCS) -o $@ -c $<

install: $(DESTDIR)/lib/extra/ install_info build
ifneq ($(BUILDDIR), $(DESTDIR))
	@cp -r $(BUILDDIR)/lib/ $(DESTDIR)/
	@cp -r $(BUILDDIR)/include/ $(DESTDIR)/
else
	$(info [WRN] BUILDDIR($(BUILDDIR)) == DESTDIR($(DESTDIR)))
endif

dirs: $(dir $(LIBNS)) $(dir $(OBJS)) $(dir $(HDRS))

.PRECIOUS: $(BUILDDIR)/. $(BUILDDIR)/%/. $(DESTDIR)/. $(DESTDIR)/%/.
$(BUILDDIR)/:
	$(info [INFO] Creating directory [$@])
	@mkdir -p $@

$(DESTDIR)/:
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

header_info:
	$(info [INFO] Copying header files [$(BUILDDIR)/include/])

clean:
	$(info [INFO] Cleaning [$(notdir $(CURDIR))])
	@rm -rf $(BUILDDIRBASE)/
	@$(MAKE) -sC tests clean

ccls:
	$(info [INFO] Creating .ccls file)
	@printf "%s\\n" $(CC) $(subst ' ','\n',$(LIBS)) $(CFLAGS) \
			$(subst ' ','\n',$(INCS)) $(subst ' ','\n',$(FONTCONFIG_CFLAGS)) \
			-I$(subst :, -I,$(C_INCLUDE_PATH)) > .ccls
