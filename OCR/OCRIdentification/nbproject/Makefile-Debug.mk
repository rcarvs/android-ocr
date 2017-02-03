#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/FeatureExtraction.o \
	${OBJECTDIR}/ImageMatrix.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS="/usr/local/cuda/lib64/libOpenCL.so" -I /usr/local/cuda/include/ 

# CC Compiler Flags
CCFLAGS="/usr/local/cuda/lib64/libOpenCL.so" -I /usr/local/cuda/include/ 
CXXFLAGS="/usr/local/cuda/lib64/libOpenCL.so" -I /usr/local/cuda/include/ 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS="/usr/local/cuda/lib64/libOpenCL.so" -I /usr/local/cuda/include/ 

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ocridentification

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ocridentification: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ocridentification ${OBJECTFILES} ${LDLIBSOPTIONS} "/usr/local/cuda/lib64/libOpenCL.so" -I /usr/local/cuda/include/

${OBJECTDIR}/FeatureExtraction.o: FeatureExtraction.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FeatureExtraction.o FeatureExtraction.cpp

${OBJECTDIR}/ImageMatrix.o: ImageMatrix.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ImageMatrix.o ImageMatrix.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
