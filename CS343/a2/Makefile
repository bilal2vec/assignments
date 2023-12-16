TYPE:=int

CXX = u++					# compiler
CXXFLAGS = -g -Wall -Wextra -MMD -O2 -DTYPE="${TYPE}"
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

.SUFFIXES : .cfa				# CFA default rules
.cfa.o :
	${CXX} ${CXXFLAGS} -c $<

OBJECTS1 = q1.o # object files forming 1st executable with prefix "q1"
EXEC1 = binsertsort

OBJECTS2 = q2.o # object files forming 2nd executable with prefix "q2"
EXEC2 = hotpotato

OBJECTS = ${OBJECTS1} ${OBJECTS2}		# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC1} ${EXEC2}			# all executables

.PHONY : all clean
.ONESHELL :

all : ${EXECS}					# build all executables

#############################################################

-include BinsertImpl

ifeq (${IMPLTYPE},${TYPE})			# same implementation type as last time ?
${EXEC1} : ${OBJECTS1}
	${CXX} $^ -o $@
else						# implementation type has changed => rebuilt
.PHONY : ${EXEC1}
${EXEC1} :
	rm -f BinsertImpl
	touch q1binsertsort.h
	sleep 1
	${MAKE} ${EXEC1} TYPE="${TYPE}"
endif

BinsertImpl :
	echo "IMPLTYPE=${TYPE}" > BinsertImpl
	sleep 1

${EXEC2} : ${OBJECTS2}				# link step 2nd executable
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o BinsertImpl ${EXECS}