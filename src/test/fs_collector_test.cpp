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

#include "fs_collector_test.h"

#include "core/fs_collector.h"

#include <QDir>
#include <QTest>
#include <QStringList>
#include <QDebug>

#include <algorithm>

using namespace core;

namespace
{
	const QDir kTempPath = QDir::temp();

	const QString kCleanPath = "clean";
	const QString kSingleFilePath = "single";
	const QString kNestedCleanPath = "nested_clean";
	const QString kOneLevelPath = "one_level";
	const QString kCommonPath = "common";

	const QStringList kAllPaths = QStringList()
								  << kCleanPath
								  << kSingleFilePath
								  << kNestedCleanPath
								  << kOneLevelPath
								  << kCommonPath;

	const char* kMkPathErrorMess = "Can't create necessary folders in TEMP, check permissions";
	const char* kRmPathErrorMess = "Can't remove necessary folders in TEMP, check permissions";
}

inline void mkPathFunc (const QString& path)
{
	QVERIFY2 (kTempPath.mkpath (path), kMkPathErrorMess);
}
inline void rmPathFunc (const QString& path)
{
	QVERIFY2 (kTempPath.rmpath (path), kRmPathErrorMess);
}

namespace test
{
	void CollectorTest::testCollect()
	{

	}

	void CollectorTest::testPause()
	{

	}

	void CollectorTest::testCancel()
	{

	}

	void CollectorTest::initTestCase()
	{
		qDebug() << "Using" << QDir().toNativeSeparators(kTempPath.path()) << "as TEMP folder";

		std::for_each (kAllPaths.begin(), kAllPaths.end(), &mkPathFunc);
	}

	void CollectorTest::cleanupTestCase()
	{
		std::for_each (kAllPaths.begin(), kAllPaths.end(), &rmPathFunc);
	}

	void CollectorTest::prepareData() const
	{
		QTest::addColumn<QString> ("path");
		QTest::addColumn<StatDataPtr> ("stat_data");

		QTest::newRow ("all_empty") << "" << StatDataPtr();

		StatDataPtr cleanDataPtr;
		QTest::newRow ("clean_folder") << kCleanPath << cleanDataPtr;
	}
}