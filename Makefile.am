include $(top_srcdir)/Makefile.tool.am

EXTRA_DIST = docs/vr-manual.xml
pkginclude_HEADERS = verrou.h

#----------------------------------------------------------------------------
# verrou-<platform>
#----------------------------------------------------------------------------

noinst_PROGRAMS  = verrou-@VGCONF_ARCH_PRI@-@VGCONF_OS@
if VGCONF_HAVE_PLATFORM_SEC
noinst_PROGRAMS += verrou-@VGCONF_ARCH_SEC@-@VGCONF_OS@
endif

VERROU_SOURCES_COMMON = vr_main.c vr_fpOps.cxx

verrou_@VGCONF_ARCH_PRI@_@VGCONF_OS@_SOURCES      = \
	$(VERROU_SOURCES_COMMON)
verrou_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CPPFLAGS     = \
	$(AM_CPPFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
verrou_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CFLAGS       = \
	$(AM_CFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
# Disable exceptions and RTTI to avoid problems with `__gxx_personality_v0'
verrou_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CXXFLAGS     = \
	$(AM_CFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)     \
	-fno-exceptions -fno-rtti
verrou_@VGCONF_ARCH_PRI@_@VGCONF_OS@_DEPENDENCIES = \
	$(TOOL_DEPENDENCIES_@VGCONF_PLATFORM_PRI_CAPS@)
verrou_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDADD        = \
	$(TOOL_LDADD_@VGCONF_PLATFORM_PRI_CAPS@)    \
	-lm -lc
verrou_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDFLAGS      = \
	$(TOOL_LDFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
verrou_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LINK = \
	$(top_builddir)/coregrind/link_tool_exe_@VGCONF_OS@ \
	@VALT_LOAD_ADDRESS_PRI@ \
	$(LINK) \
	$(verrou_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CFLAGS) \
	$(verrou_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDFLAGS)

if VGCONF_HAVE_PLATFORM_SEC
verrou_@VGCONF_ARCH_SEC@_@VGCONF_OS@_SOURCES      = \
	$(VERROU_SOURCES_COMMON)
verrou_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CPPFLAGS     = \
	$(AM_CPPFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
verrou_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CFLAGS       = \
	$(AM_CFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
verrou_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CXXFLAGS     = \
	$(AM_CFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)     \
	-fno-exceptions -fno-rtti
verrou_@VGCONF_ARCH_SEC@_@VGCONF_OS@_DEPENDENCIES = \
	$(TOOL_DEPENDENCIES_@VGCONF_PLATFORM_SEC_CAPS@)
verrou_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDADD        = \
	$(TOOL_LDADD_@VGCONF_PLATFORM_SEC_CAPS@)    \
	-lm -lc
verrou_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDFLAGS      = \
	$(TOOL_LDFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
verrou_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LINK = \
	$(top_builddir)/coregrind/link_tool_exe_@VGCONF_OS@ \
	@VALT_LOAD_ADDRESS_SEC@ \
	$(LINK) \
	$(verrou_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CFLAGS) \
	$(verrou_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDFLAGS)
endif