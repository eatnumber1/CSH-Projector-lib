include Makefile

define postdepRule
 CURDIR := $$(TOPDIR)/$$$(1)
 include $$(CURDIR)/postdep.mk
endef
$(foreach subdir, $(SUBDIRS), $(eval $(call postdepRule, $(subdir))))

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

%.d: %.c
	@false
