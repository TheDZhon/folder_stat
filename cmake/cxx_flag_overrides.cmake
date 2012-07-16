#--------------------------------------------------------------------------------------------------------------------------------------------
#    Copyright (c) 2011, Eugene Mamin <TheDZhon@gmail.com>
#    All rights reserved.
#
#    Redistribution and use in source and binary forms, with or without
#    modification, are permitted provided that the following conditions are met:
#        * Redistributions of source code must retain the above copyright
#        notice, this list of conditions and the following disclaimer.
#        * Redistributions in binary form must reproduce the above copyright
#        notice, this list of conditions and the following disclaimer in the
#        documentation and/or other materials provided with the distribution.
#        * Neither the name of the <organization> nor the
#        names of its contributors may be used to endorse or promote products
#        derived from this software without specific prior written permission.
#
#    THIS SOFTWARE IS PROVIDED BY <copyright holder> <email> ''AS IS'' AND ANY
#    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#    DISCLAIMED. IN NO EVENT SHALL <copyright holder> <email> BE LIABLE FOR ANY
#    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#---------------------------------------------------------------------------------------------------------------------------------------------

IF (MSVC)
	SET (CMAKE_CXX_FLAGS "/EHa /W3")
	SET (CMAKE_CXX_FLAGS_RELEASE "/Ob2 /O2 /Ot /Oi /Oy /GT /GL /MD /D NDEBUG")
	SET (CMAKE_CXX_FLAGS_DEBUG "/Zi /Od /GT /MDd")
ENDIF (MSVC)
IF (UNIX)
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fno-strict-aliasing")
	SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--as-needed")
	SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--as-needed")
	IF (EXTRA_WARNINGS)
		SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	ENDIF (EXTRA_WARNINGS)
	IF (USE_PIPE)
		SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
	ENDIF (USE_PIPE)
ENDIF (UNIX)
IF (WIN32 AND NOT MSVC)
	SET (CMAKE_CXX_FLAGS "-Wall")
ENDIF (WIN32 AND NOT MSVC)