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

#include "test/fs_test_utils.h"
#include "core/fs_collector.h"

#include <QDir>
#include <QTest>
#include <QStringList>
#include <QDebug>
#include <QVector>

#include <algorithm>
#include <functional>
#include <ctime>

typedef QVector<size_t> SizeTVector;

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
	const char* kMkFileErrorMess = "Can't create necessary files in TEMP, check permissions";

	const QIODevice::OpenMode kDefaultOpenMode = QIODevice::WriteOnly | QIODevice::Truncate;
	const size_t kDefaultFileSz = 10;
	const size_t kRandDiv = 10;

	const test::range_rand pos_rand_f = test::range_rand(1, kRandDiv); 
}

inline void mkPath (const QString& path)
{
	QVERIFY2 (kTempPath.mkpath (path), kMkPathErrorMess);
}

inline void cleanupFunc (const QString &path)
{
	QVERIFY2 (test::rmPathRecursive(kTempPath.canonicalPath() + "/" + path), kRmPathErrorMess);
}

void createFiles (const QString& path, const SizeTVector& cnt_list)
{
	typedef SizeTVector::const_iterator It;

	for (It it = cnt_list.begin();
		 it != cnt_list.end();
		 ++it) {
		const size_t index = std::distance (cnt_list.begin(), it);
		for (size_t i = 0; i < *it; ++i) {
			const QString fname = kTempPath.canonicalPath()
								  + "/"
								  + path
								  + "/"
								  + QString::number (i)
								  + "."
								  + QString::number (index);

			QFile current_file (fname);
			QVERIFY2 (current_file.open (kDefaultOpenMode), kMkFileErrorMess);
			QVERIFY (current_file.write (QByteArray (kDefaultFileSz, '0')) > 0);
		}
	}
}

void createSubdirs (const QString& path, size_t subdirs_cnt)
{
	for (size_t i = 0; i < subdirs_cnt; ++i) {
		mkPath (path + "/" + QString::number (i));
	}
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
		qsrand (time (NULL));

		qDebug() << "Using" << QDir().toNativeSeparators (kTempPath.path()) << "as TEMP folder";

		std::for_each (kAllPaths.begin(), kAllPaths.end(), &mkPath);

		createFiles (kSingleFilePath, SizeTVector() << 1);

		const size_t nested_dirs_cnt = pos_rand_f ();
		createSubdirs (kNestedCleanPath, nested_dirs_cnt);

		const size_t one_level_dirs_cnt = pos_rand_f ();
		const size_t one_level_files_cnt = pos_rand_f ();
		SizeTVector files_cnt_list (one_level_files_cnt);
		std::generate (files_cnt_list.begin(), files_cnt_list.end(), pos_rand_f);
		createSubdirs (kOneLevelPath, one_level_dirs_cnt);
		createFiles (kOneLevelPath, files_cnt_list);
	}

	void CollectorTest::cleanupTestCase()
	{
		std::for_each (kAllPaths.begin(), kAllPaths.end(), &cleanupFunc);
	}

	void CollectorTest::prepareData() const
	{
		QTest::addColumn<StatDataPtr> ("expected_stat_data");

		QTest::newRow (kCleanPath.toAscii().constData()) << expected_stats_[kCleanPath];
		QTest::newRow (kSingleFilePath.toAscii().constData()) << expected_stats_[kSingleFilePath];
		QTest::newRow (kNestedCleanPath.toAscii().constData()) << expected_stats_[kNestedCleanPath];
		QTest::newRow (kOneLevelPath.toAscii().constData()) << expected_stats_[kOneLevelPath];
	}
}