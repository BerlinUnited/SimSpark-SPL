SUBDIRS = doc utility simulator

pkginclude_HEADERS = \
config.h

doc:	
	cd doc && $(MAKE) all doxygen-doc

release:	update distcheck add_rel commit upload tag 

quick-release:	update add_rel commit upload tag commit

tag: do_tag inc_rel

do_tag:
	tag=`echo @PACKAGE@-@VERSION@ | sed s/\\\\./_/g`; \
	echo "tagging release branch with $$tag"; \
	echo `cvs tag -b $$tag`;	

add_rel:
	if test "$$USERNAME" = ""; then \
	    USERNAME="$$USER"; fi; \
	if test "$$USEREMAIL" = ""; then \
	    USEREMAIL="sserver-admin@lists.sf.net"; fi; \
	DATE=`date +"%Y-%m-%d"`; \
	LOGHEADER=`echo -e "$$DATE\t$$USERNAME\t<$$USEREMAIL>\n"`; \
	echo -e "$$LOGHEADER\n\n\t* ./configure.in\n\tReleased @PACKAGE@-@VERSION@\n" \
	| cat - @srcdir@/ChangeLog > ChangeLog.tmp
	mv ChangeLog.tmp @srcdir@/ChangeLog

inc_rel:
	$(AWK) -f @srcdir@/rel.awk @srcdir@/configure.in > configure.tmp;
	mv configure.tmp @srcdir@/configure.in
	$(MAKE) commit

upload:	@PACKAGE@-@VERSION@.tar.gz
	ncftpput -v upload.sourceforge.net /incoming @PACKAGE@-@VERSION@.tar.gz

@PACKAGE@-@VERSION@.tar.gz: dist

update:
	cvs -z9 update

commit:
	$(AWK) '/^[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]/ { count++ } { if ( count == 1 ) print $0 }' @srcdir@/ChangeLog > message.tmp
	cvs -z9 commit -F message.tmp

addlog:
	if test "$$USERNAME" = ""; then \
	    USERNAME="$$USER"; \
	fi; \
	if test "$$USEREMAIL" = ""; then \
	    USEREMAIL="sserver-admin@lists.sf.net"; \
	fi; \
	cvs -z9 diff -u --brief 2>&1 > @srcdir@/addlog.tmp; \
	if test -s @srcdir@/addlog.tmp; then \
		DATE=`date +"%Y-%m-%d"`; \
		LOGHEADER=`echo -e "$$DATE\t$$USERNAME\t<$$USEREMAIL>\n"`; \
		cat @srcdir@/addlog.tmp | $(AWK) \
		-v LOGHEADER="$$LOGHEADER" 'BEGIN { print LOGHEADER "\n" } \
		/^Index/ { print "\t* ./" $$2 } \
		/^cvs server: .* is a new entry/ { print "\t* ./" $$3 }	\
		END { print "" }' | cat - @srcdir@/ChangeLog > ChangeLog.tmp; \
		mv ChangeLog.tmp @srcdir@/ChangeLog; \
		if test -n "$(CVSEDITOR)"; then $(CVSEDITOR) ChangeLog; \
		elif test -n "$(VISUAL)"; then $(VISUAL) ChangeLog; \
		elif test -n "$(EDITOR)"; then $(EDITOR) ChangeLog; \
		else \
			EDITOR=`which vi`; \
			if test -n "$$EDITOR" && test -x "$$EDITOR"; then \
				$$EDITOR ChangeLog; \
			else \
				echo ""; \
				echo "Cannot find an editor."; \
				echo "Please edit ChangeLog manually\n"; \
			fi; \
		fi; \
	fi



