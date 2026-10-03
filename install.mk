DESTDIR ?=
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

RES_PERMISSIONS := 755

.PHONY: install

install: release
	@echo "INSTALL\t$(DESTDIR)$(BINDIR)/$(TARGET)"
	$(Q)install -d $(DESTDIR)/$(BINDIR)/
	$(Q)install $(BUILD_DIR)/$(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET) -m $(RES_PERMISSIONS)

uninstall:
	@echo "RM\t$(DESTDIR)$(BINDIR)/$(TARGET)"
	$(Q)rm -f $(DESTDIR)$(BINDIR)/$(TARGET)
