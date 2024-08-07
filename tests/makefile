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

SRC_DIR ?= .
TYPE ?= release
BUILDDIRBASE := build/
BUILDDIR := $(BUILDDIRBASE)$(TYPE)
DESTDIR ?= $(dir $(CURDIR))$(BUILDDIR)

INC_DIRS = $(SRC_DIR)/ ../
INCS := $(addprefix -I,$(INC_DIRS))
SRC_FILES := $(shell find $(SRC_DIR)/ -name "*.c")

OBJS_DIR = $(BUILDDIR)/objects
OBJS := $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.o)
.SECONDARY: $(OBJS)

DEPS := $(OBJS:.o=.d)
-include $(DEPS)

PKGCONFIGS_CFLAGS :=
PKGCONFIGS_LIBS :=

ifneq ($(PKGCONFIGS_CFLAGS),)
	PKGCONFIG_CFLAGS = $(shell pkg-config --cflags $(PKGCONFIGS_CFLAGS))
endif

ifneq ($(PKGCONFIGS_LIBS),)
	PKGCONFIG_LIBS = $(shell pkg-config --libs $(PKGCONFIGS_LIBS))
endif

LIB_NAMES := cmocka
LIBS := $(addprefix -l,$(LIB_NAMES))
LIBS += $(PKGCONFIG_LIBS)

CFLAGS = -MMD -MP -flto -O2 -std=gnu11 -fsanitize=address
CFLAGS += $(PKGCONFIG_CFLAGS)

TEST_LIB := $(DESTDIR)/lib/libstd.la

ifeq ($(TYPE), debug)
	CFLAGS += -DDEBUG -ggdb -Wall -Wextra -Wswitch-enum -Wswitch-default
endif

CC = gcc
LIBTOOL := libtool
LIBTOOLFLAGS := --silent

TEST_FILES := std-str std-lib std-file
BIN_NAME := main

.DEFAULT_GOAL := check
check: build $(TEST_FILES)

$(TEST_FILES):
	./$(BUILDDIR)/bin/$@

build: build_info $(TEST_LIB) dirs makefile $(addprefix $(BUILDDIR)/bin/,$(TEST_FILES))

.NOTPARALLEL:
%.la:
	$(info [INFO] Bootstraping library [$(*:$(dir $(CURDIR))%=%)])
	@$(MAKE) -C ..

$(BUILDDIR)/bin/%: $(OBJS_DIR)/%.o
	$(info [CC] Linking object files)
	@$(LIBTOOL) $(LIBTOOLFLAGS) --tag=CC --mode=link $(CC) -o $(OBJS_DIR)/$* \
		$^ $(TEST_LIB) $(LIBS)
	@mv $(OBJS_DIR)/$* $(BUILDDIR)/bin/$*

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c makefile
	$(info [CC] $<)
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@

dirs: $(BUILDDIR)/bin/ $(dir $(OBJS))

.PRECIOUS: $(BUILDDIR)/. $(BUILDDIR)/%/.
$(BUILDDIR)/:
	$(info [INFO] Creating directory [$@])
	@mkdir -p $@

$(BUILDDIR)/%/:
	$(info [INFO] Creating directory [$@])
	@mkdir -p $@

.PHONY: clean ccls debug_info build_info install_info
build_info:
ifeq ($(TYPE), debug)
	$(info [INFO] Compiling In Debug Mode [Test Suite])
else
	$(info [INFO] Building Executable [Test Suite])
endif

clean:
	$(info [INFO] Cleaning [$(notdir $(CURDIR))])
	@rm -rf $(BUILDDIRBASE)/

ccls:
	$(info [INFO] Creating .ccls file)
	@printf "%s\\n" $(CC) $(subst ' ','\n',$(LIBS)) $(CFLAGS) \
			$(subst ' ','\n',$(INCS)) -I$(subst :, -I,$(C_INCLUDE_PATH)) > .ccls
