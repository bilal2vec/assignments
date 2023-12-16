OUTPUT = OUTPUT
GVERSION = -12
CXX = u++					# uC++ compiler
#CXX = /u/cs343/cfa-cc/bin/cfa			# CFA compiler
CXXFLAGS = -g -Wall -Wextra -MMD -Wno-implicit-fallthrough -D${OUTPUT} # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}# makefile name

.SUFFIXES : .cfa				# CFA default rules
.cfa.o :
	${CXX} ${CXXFLAGS} -c $<

OBJECTS01 = q1exception.o			# optional build of given program
EXEC01 = exception				# given executable name

OBJECTS1 = q1returnglobal.o			# 1st executable object files
EXEC1 = returnglobal				# 1st executable name

OBJECTS2 = q1returntype.o			# 2nd executable object files
EXEC2 = returntype				# 2nd executable name

OBJECTS3 = q1returntypec.o			# 3rd executable object files
EXEC3 = returntypec				# 3rd executable name

OBJECTS02 = q2throwcatch.o			# optional build of given program
EXEC02 = throwcatch				# given executable name

OBJECTS4 = q2longjmp.o				# 4th executable object files
EXEC4 = longjmp					# 4th executable name

OBJECTS5 = q3floatconstant.o # object files forming 5th executable with prefix "q3"
OBJECTS6 = q3.o 
EXEC5 = floatconstant					# 5th executable name

OBJECTS = ${OBJECTS1} ${OBJECTS2} ${OBJECTS3} ${OBJECTS4} ${OBJECTS5}
DEPENDS = ${OBJECTS:.o=.d}
EXECS = ${EXEC1} ${EXEC2} ${EXEC3} ${EXEC4} ${EXEC5}

#############################################################

.PHONY : all clean

all : ${EXECS}					# build all executables

${EXEC01} : ${OBJECTS01}			# optional build of given program
	g++${GVERSION} ${CXXFLAGS} $^ -o $@

q1%.o : q1%.cc					# change compiler 1st executable, ADJUST SUFFIX (for .C/.cpp)
	g++${GVERSION} ${CXXFLAGS} -std=c++17 -c $< -o $@

${EXEC1} : ${OBJECTS1}				# compile and link 1st executable
	g++${GVERSION} ${CXXFLAGS} $^ -o $@

${EXEC2} : ${OBJECTS2}				# compile and link 2nd executable
	g++${GVERSION} ${CXXFLAGS} $^ -o $@

q1%.o : q1%.c					# change compiler 2nd executable
	gcc${GVERSION} ${CXXFLAGS} -c $< -o $@

${EXEC02} : ${OBJECTS02}			# optional build of given program
	g++${GVERSION} ${CXXFLAGS} $^ -o $@

${EXEC3} : ${OBJECTS3}				# compile and link 3rd executable
	g++${GVERSION} ${CXXFLAGS} $^ -o $@

q2%.o : q2%.cc					# change compiler 4th executable, ADJUST SUFFIX (for .C/.cpp)
	g++${GVERSION} ${CXXFLAGS} -c $< -o $@

${EXEC4} : ${OBJECTS4}				# compile and link 4th executable
	g++${GVERSION} ${CXXFLAGS} $^ -o $@


q3%.o : q3%.cc					# change compiler 4th executable, ADJUST SUFFIX (for .C/.cpp)
	${CXX} ${CXXFLAGS} -c $< -o $@

${EXEC5} : ${OBJECTS5} ${OBJECTS6}				# compile and link 5th executable
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXEC01} ${EXEC02} ${EXECS}
