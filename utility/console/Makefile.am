noinst_LTLIBRARIES = libconsole.la

AM_CPPFLAGS = -I$(top_srcdir)/utility/mathstuff/ \
-I$(top_srcdir)/utility/forwarder/ \
-I$(top_srcdir)/utility/classserver 
# the inputserver needs to be used for an interactive console only
# -I$(top_srcdir)/utility/inputserver

noinst_PROGRAMS = example
example_SOURCES = example.cc
example_LDADD   = libconsole.la \
$(top_srcdir)/utility/forwarder/libforwarder.la \
$(top_srcdir)/utility/classserver/libclassserver.la -ldl

libconsole_la_SOURCES = \
console.cc console.h \
consolealiases.cc consolealiases.h \
consolebaseview.h \
consoleparser.cc consoleparser.h \
consolesettings.cc consolesettings.h \
scanner.cc scanner.h \
commands/commandserver.cc commands/commandserver.h \
commands/concommand.cc commands/concommand.h \
commands/conquit.cc commands/conquit.h \
commands/signature/commandsignature.cc commands/signature/commandsignature.h \
commands/signature/commandsignatures.cc commands/signature/commandsignatures.h \
commands/signature/comsigany.cc commands/signature/comsigany.h \
commands/signature/comsigbase.cc commands/signature/comsigbase.h \
commands/signature/comsigbool.cc commands/signature/comsigbool.h \
commands/signature/comsigcommand.cc commands/signature/comsigcommand.h \
commands/signature/comsigelliptic.cc commands/signature/comsigelliptic.h \
commands/signature/comsigfloat.cc commands/signature/comsigfloat.h \
commands/signature/comsigint.cc commands/signature/comsigint.h \
commands/signature/comsiglabel.cc commands/signature/comsiglabel.h \
commands/signature/comsigstring.cc commands/signature/comsigstring.h \
commands/signature/comsigvariable.cc commands/signature/comsigvariable.h \
commands/signature/comsigvector.cc commands/signature/comsigvector.h \
variables/convar.cc variables/convar.h \
variables/convarattributes.cc variables/convarattributes.h \
variables/convarbool.cc variables/convarbool.h \
variables/convarcallback.h \
variables/convarcallbackconsole.cc variables/convarcallbackconsole.h \
variables/convarcharstring.cc variables/convarcharstring.h \
variables/convarcommand.cc variables/convarcommand.h \
variables/convarfloat.cc variables/convarfloat.h \
variables/convarint.cc variables/convarint.h \
variables/convarlabel.cc variables/convarlabel.h \
variables/convarrefbool.cc variables/convarrefbool.h \
variables/convarrefcharstring.cc variables/convarrefcharstring.h \
variables/convarreffloat.cc variables/convarreffloat.h \
variables/convarrefint.cc variables/convarrefint.h \
variables/convarrefstring.cc variables/convarrefstring.h \
variables/convarrefvector.cc variables/convarrefvector.h \
variables/convarseparator.cc variables/convarseparator.h \
variables/convarserver.cc variables/convarserver.h \
variables/convarstate.cc variables/convarstate.h \
variables/convarstring.cc variables/convarstring.h \
variables/convarundefined.cc variables/convarundefined.h \
variables/convarvariable.cc variables/convarvariable.h \
variables/convarvector.cc variables/convarvector.h

# consolebindings.cc consolebindings.h \
# consolehistory.cc consolehistory.h \
# consolebinding.cc consolebinding.h \
# consoleinputline.cc consoleinputline.h \
# consoleinputlinebindings.cc consoleinputlinebindings.h \
