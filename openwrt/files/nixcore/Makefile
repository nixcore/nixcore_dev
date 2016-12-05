#
# Copyright (C) 2008-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# Copied from gpio-button-hotplug

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=nixcore
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/nixcore
  SUBMENU:=Other modules
  TITLE:=Nixcore Support
  FILES:=$(PKG_BUILD_DIR)/nixcore.ko
  AUTOLOAD:=$(call AutoLoad,30,nixcore,1)
  KCONFIG:=
endef

define KernelPackage/nixcore/description
 This is a driver for nixcore
endef

MAKE_OPTS:= \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)"

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		$(MAKE_OPTS) \
		modules
endef

$(eval $(call KernelPackage,nixcore))
