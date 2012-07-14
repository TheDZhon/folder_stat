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

#include "fs_cacher_test.h"

#include "core/fs_cacher.h"

#include <QTest>

using namespace core;

namespace test
{
	void CacherTest::testGetAndStore()
	{
		QFETCH (QString, path);
		QFETCH (StatDataPtr, stat_data);

		cacher_.store (path, stat_data);
		const StatDataPtr & sd_from_cache = cacher_.get (path);

		QCOMPARE (sd_from_cache, stat_data);
	}

	void CacherTest::testInvalidate()
	{
		QFETCH (QString, path);
		QFETCH (StatDataPtr, stat_data);

		cacher_.store(path, stat_data);
		cacher_.invalidate(path);
		const StatDataPtr & sd_from_cache = cacher_.get (path);

		QVERIFY (sd_from_cache.isNull());
	}

	void CacherTest::testClear()
	{
		QFETCH (QString, path);
		QFETCH (StatDataPtr, stat_data);

		cacher_.store(path, stat_data);
		cacher_.clear();
		const StatDataPtr & sd_from_cache = cacher_.get(path);

		QVERIFY (sd_from_cache.isNull());
	}

	void CacherTest::prepareData() const
	{
		QTest::addColumn<QString> ("path");
		QTest::addColumn<StatDataPtr> ("stat_data");

		QTest::newRow ("all_empty") << "" << StatDataPtr();
		QTest::newRow ("path_filled") << "C:\\" << StatDataPtr();
		QTest::newRow ("stat_filled") << "" << StatDataPtr (new StatData);
		QTest::newRow ("all_filled") << "C:\\" << StatDataPtr (new StatData);
	}
}