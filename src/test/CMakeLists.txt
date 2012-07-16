#--------------------------------------------------------------------------------------------------------------------------------------------
#    Copyright (c) 2012, Eugene Mamin <TheDZhon@gmail.com>
#    All rights reserved.
#
#    Redistribution and use in source and binary forms, with or without
#    modification, are permitted provided that the following conditions are met:
#        * Redistributions of source code must retain the above copyright
#        notice, this list of conditions and the following disclaimer.
#        * Redistributions in binary form must reproduce the above copyright
#        notice, this list of conditions and the following disclaimer in the
#        documentation and/or other materials provided with the distribution.
#        * Neither the name of the Prefix Increment nor the
#        names of its contributors may be used to endorse or promote products
#        derived from this software without specific prior written permission.
#
#    THIS SOFTWARE IS PROVIDED BY Eugene Mamin <TheDZhon@gmail.com> ''AS IS'' AND ANY
#    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#    DISCLAIMED. IN NO EVENT SHALL Eugene Mamin <TheDZhon@gmail.com> BE LIABLE FOR ANY
#    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#---------------------------------------------------------------------------------------------------------------------------------------------

# CMake file for tests

# --- Definition ---

CMAKE_MINIMUM_REQUIRED (VERSION 2.8 FATAL_ERROR)
PROJECT (folder_stat_test)

GET_FILENAME_COMPONENT (SRC_DIR ${PROJECT_SOURCE_DIR} PATH)
GET_FILENAME_COMPONENT (TOP_DIR ${SRC_DIR} PATH)

SET (BIN_DIR ${TOP_DIR}/bin)
SET (TMP_DIR ${TOP_DIR}/tmp)

# --- Qt ---

SET (CMAKE_AUTOMOC TRUE)
SET (QT_USE_QTTEST TRUE)

FIND_PACKAGE (Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

ADD_DEFINITIONS (${QT_DEFINITIONS})
INCLUDE_DIRECTORIES (${QT_INCLUDE_DIRECTORIES}
	${CMAKE_BINARY_DIR}
	${CMAKE_CURRENT_BINARY_DIR})
	
# --- Paths ---

FILE (GLOB CORE_INCLUDES ${SRC_DIR}/core/*.h)
FILE (GLOB CORE_SOURCES ${SRC_DIR}/core/*.cpp)

FILE (GLOB TEST_INCLUDES ${SRC_DIR}/test/*.h)
FILE (GLOB TEST_SOURCES ${SRC_DIR}/test/*.cpp)

SET (ALL_CORE_FILES ${CORE_INCLUDES} ${CORE_SOURCES})
SET (ALL_TEST_FILES ${TEST_INCLUDES} ${TEST_SOURCES})

SOURCE_GROUP ("headers\\test" FILES ${TEST_INCLUDES})
SOURCE_GROUP ("headers\\core" FILES ${CORE_INCLUDES})
SOURCE_GROUP ("sources\\test" FILES ${TEST_SOURCES})
SOURCE_GROUP ("sources\\core" FILES ${CORE_SOURCES})

INCLUDE_DIRECTORIES (${SRC_DIR})

# --- Targets ---

ADD_EXECUTABLE (${PROJECT_NAME}
	${ALL_TEST_FILES}
	${ALL_CORE_FILES})
	
SET_TARGET_PROPERTIES (${PROJECT_NAME}
	PROPERTIES
		OUTPUT_NAME ${PROJECT_NAME}
		DEBUG_OUTPUT_NAME ${PROJECT_NAME}_d
		RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR})
		
TARGET_LINK_LIBRARIES (${PROJECT_NAME} 
	${QT_LIBRARIES})
	
ADD_TEST (NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})