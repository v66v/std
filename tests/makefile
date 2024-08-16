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
DESTDIR ?= $(dir $(CURDIR))$(BUILDDIR)

INC_DIRS = $(CURDIR)/$(SRC_DIR)/ $(DESTDIR)/include/
INCS := $(addprefix -I,$(INC_DIRS))
NOTPATH := -not -path "$(SRC_DIR)/extra/*"
ifdef EXTRAS
export EXTRAS
	SRC_FILES := $(shell find $(SRC_DIR)/ -name "*.c")
else
	SRC_FILES := $(shell find $(SRC_DIR)/ $(NOTPATH) -name "*.c")
endif

OBJS_DIR = $(BUILDDIR)/objects
OBJS := $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.o)
.SECONDARY: $(OBJS)

DEPS := $(OBJS:.o=.d)
-include $(DEPS)

LIB_NAMES := cmocka
LIBS := $(addprefix -l,$(LIB_NAMES))

CFLAGS = -MMD -MP -flto -O2 -std=gnu11 -fsanitize=address

TEST_LIB := $(DESTDIR)/lib/libstd.la
TEST_LIB_FONTCONFIG := $(DESTDIR)/lib/extra/libstd-fontconfig.la

TEST_LIBS := $(TEST_LIB)
ifdef EXTRAS
	TEST_LIBS += $(TEST_LIB_FONTCONFIG)
endif

ifeq ($(TYPE), debug)
	CFLAGS += -DDEBUG -ggdb -Wall -Wextra -Wswitch-enum -Wswitch-default
endif

PKGCONFIGS_CFLAGS :=
PKGCONFIGS_LIBS :=

TEST_FILES := std-str std-lib std-file

LOCAL := T
ifdef EXTRAS
ifeq ($(LOCAL),T)
	TEST_FILES += extra/fontconfig/std-fontconfig
endif
endif

ifneq ($(PKGCONFIGS_CFLAGS),)
	PKGCONFIG_CFLAGS = $(shell pkg-config --cflags $(PKGCONFIGS_CFLAGS))
endif
CFLAGS += $(PKGCONFIG_CFLAGS)

ifneq ($(PKGCONFIGS_LIBS),)
	PKGCONFIG_LIBS = $(shell pkg-config --libs $(PKGCONFIGS_LIBS))
endif
LIBS += $(PKGCONFIG_LIBS)

CC = gcc
LIBTOOL := libtool
LIBTOOLFLAGS := --silent

BIN_TEST_FILES := $(addprefix $(BUILDDIR)/bin/,$(TEST_FILES))

.DEFAULT_GOAL := check
check: build
	$(foreach file, $(TEST_FILES), ./$(BUILDDIR)/bin/$(file);)

LIBS: $(TEST_LIBS)
	$(info [INFO] Bootstraping library [$(TEST_LIB:$(dir $(CURDIR))%.la=%)])
	@$(MAKE) -C ..

build: build_info dirs makefile $(BIN_TEST_FILES)

$(BUILDDIR)/bin/%: $(OBJS_DIR)/%.o LIBS
	$(info [CC] Linking object files [$(BUILDDIR)/bin/$*])
	@$(LIBTOOL) $(LIBTOOLFLAGS) --tag=CC --mode=link $(CC) -o $(OBJS_DIR)/$* \
		$< $(TEST_LIB) $(LIBS)
	@mv $(OBJS_DIR)/$* $(BUILDDIR)/bin/$*

$(BUILDDIR)/bin/extra/fontconfig/%: $(OBJS_DIR)/extra/fontconfig/%.o LIBS
	$(info [CC] Linking object files [$(BUILDDIR)/bin/$*])
	$(LIBTOOL) $(LIBTOOLFLAGS) --tag=CC --mode=link $(CC) -o $(OBJS_DIR)/$* \
		$< $(TEST_LIB) $(TEST_LIB_FONTCONFIG) $(LIBS) $(shell pkg-config --libs fontconfig)
	@mv $(OBJS_DIR)/$* $(BUILDDIR)/bin/extra/fontconfig/$*

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c LIBS makefile
	$(info [CC] $<)
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@

dirs: $(BUILDDIR)/bin/ $(dir $(OBJS)) $(dir $(BIN_TEST_FILES))

.PRECIOUS: $(BUILDDIR)/. $(BUILDDIR)/%/.
$(BUILDDIR)/:
	$(info [INFO] Creating directory [$@])
	@mkdir -p $@

$(BUILDDIR)/%/:
	$(info [INFO] Creating directory [$@])
	@mkdir -p $@

.PHONY: clean ccls build_info

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
