CC := clang
CSCOPE ?= cscope
DEPCC ?= gcc

-include config.mk

SUBDIRS := include src

CSCOPE_FILES := cscope.out cscope.po.out cscope.in.out

TOPDIR := $(PWD)
INCDIR := $(TOPDIR)/include

CPPFLAGS += -I$(INCDIR) -I$(TOPDIR)/CError/include -D_GNU_SOURCE
CFLAGS += -fPIC -std=gnu99 -Werror -Wall -Wextra -Wfatal-errors -Wformat=2 -Winit-self -Wswitch-enum -Wunused-parameter -Wstrict-aliasing=2 -Wstrict-overflow=5 -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wshorten-64-to-32 -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-declarations -Wmissing-noreturn -Wmissing-format-attribute -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wvolatile-register-var -Wpointer-sign

CLEAN_TARGETS := $(SUBDIRS:=/clean)
DEPCLEAN_TARGETS := $(SUBDIRS:=/depclean)
DEPEND_TARGETS := $(SUBDIRS:=/depend)
ALL_TARGETS := $(SUBDIRS:=/all)

.PHONY: all clean depclean cscope pristine cscope-clean depend all-recursive
.DEFAULT_GOAL := all

all: depend
	$(MAKE) -f postdep.mk all-recursive

all-recursive: $(ALL_TARGETS)
depend: $(DEPEND_TARGETS)
clean: $(CLEAN_TARGETS)
depclean: clean $(DEPCLEAN_TARGETS)
pristine: depclean cscope-clean

# Define default hooks so a subdir doesn't need to define them.
$(CLEAN_TARGETS):
$(DEPCLEAN_TARGETS):
$(DEPEND_TARGETS):
$(ALL_TARGETS):

define variableRule
 CURDIR := $$(TOPDIR)/$$$(1)
 include $$(CURDIR)/variables.mk
endef
$(foreach subdir, $(SUBDIRS), $(eval $(call variableRule, $(subdir))))

# This defines the following for every dir in SUBDIRS:
#   Sets CURDIR to the $(TOPDIR)/$(dir)
#   Includes a makefile in $(CURDIR)/Makefile
define subdirRule
 CURDIR := $$(TOPDIR)/$$$(1)
 $$$(1)/all: CURDIR := $$(CURDIR)
 $$$(1)/clean: CURDIR := $$(CURDIR)
 $$$(1)/depclean: CURDIR := $$(CURDIR)
 include $$(CURDIR)/Makefile
endef
# This is what actually does the work.
# The "call" command replaces every $(1) variable reference in subdirRule with $(subdir)
# The "eval" command parses the result of the "call" command as make syntax
$(foreach subdir, $(SUBDIRS), $(eval $(call subdirRule, $(subdir))))
# Reset CURDIR back to what it should be.
CURDIR := $(TOPDIR)

%.o: %.c
	@false

%.d: %.c
	$(DEPCC) -MM $(CPPFLAGS) -MQ $(@:.d=.o) -MQ $@ -MF $*.d $<
