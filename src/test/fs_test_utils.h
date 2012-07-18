//------------------------------------------------------------------------------------------------------------------------------------------
//    Copyright (c) 2012, Eugene Mamin <TheDZhon@gmail.com>
//    All rights reserved.
//
//    Redistribution and use in source and binary forms, with or without
//    modification, are permitted provided that the following conditions are met:
//        * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//        * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//        * Neither the name of the Prefix Increment nor the
//        names of its contributors may be used to endorse or promote products
//        derived from this software without specific prior written permission.
//
//    THIS SOFTWARE IS PROVIDED BY Eugene Mamin <TheDZhon@gmail.com> ''AS IS'' AND ANY
//    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//    DISCLAIMED. IN NO EVENT SHALL Eugene Mamin <TheDZhon@gmail.com> BE LIABLE FOR ANY
//    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------------------------------------------------------------------

#ifndef FS_TEST_UTILS_H__
#define FS_TEST_UTILS_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <QtGlobal>
#include <QFileInfo>
#include <QDir>
#include <QVector>
#include <QTest>

#include <algorithm>

typedef QVector<size_t> SizeTVector;

namespace test
{
	static const size_t kDefaultFileSz = 10;

	void rmPathInTempRecursive (const QString& path);
	void mkPathInTemp (const QString& path);

	QFileInfoList createTestFiles (const QString& path, const SizeTVector& cnt_list);
	QFileInfoList createTestSubdirs (const QString& path, size_t subdirs_cnt);

	struct range_rand_generator {
		range_rand_generator (size_t min, size_t max) :
			min_ (min),
			max_ (max)
		{}

		inline size_t operator () () const {
			return (qrand() % (max_ - min_)) + min_;
		}

		size_t min_;
		size_t max_;
	};

	template<typename T>
	struct iota_emulator_generator {
		iota_emulator_generator (T fst, T inc): 
			fst_(fst),
			inc_(inc) {}

		inline T operator () () {
			T old = fst_;
			fst_ += inc_;
			return old;
		}		
		
		T fst_;
		T inc_;
	};
}

#endif // FS_TEST_UTILS_H__

