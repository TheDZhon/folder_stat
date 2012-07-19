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

#include <QTest>
#include <QStringList>
#include <QDebug>

#include <algorithm>
#include <functional>
#include <ctime>

using namespace core;

namespace
{
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

	const size_t kRandDiv = 10;
	const test::range_rand_generator pos_rand_f = test::range_rand_generator (1, kRandDiv);
}

namespace test
{
	void CollectorTest::testCollect()
	{
		QFETCH (QString, path);
		QFETCH (StatDataPtr, expected_stat_data);

		QVERIFY (!expected_stat_data.isNull());

		Collector collector;

		connect (&collector, 
			SIGNAL (finished (const QString&, const core::StatDataPtr&)),
			SLOT (handleScanFinished(const QString&, const core::StatDataPtr&)));

		collector.collect(QDir::tempPath() + "/" + path, false);
		QVERIFY(waitForSignal(this, SIGNAL (scanFinished()), 10000));

		QVERIFY (!current_stat_.isNull());

		QCOMPARE (*current_stat_, *expected_stat_data);
	}

	void CollectorTest::testCancel()
	{
		Collector collector;

		collector.collect(QDir::rootPath(), false);
		collector.cancel();

		QVERIFY(waitForSignal(&collector, SIGNAL (error(const QString&, const QString&)), 10000));
	}

	void CollectorTest::initTestCase()
	{
		cleanupTestCase(); // remove old directories

		qsrand (time (NULL));

		//

		std::for_each (kAllPaths.begin(), kAllPaths.end(), &mkPathInTemp);
		const QFileInfoList& single_file_info_list = createTestFiles (kSingleFilePath, SizeTVector() << 1);

		const size_t nested_clean_dirs_cnt = pos_rand_f ();
		const QFileInfoList& nested_clean_dirs = createTestSubdirs (kNestedCleanPath, nested_clean_dirs_cnt);

		const size_t one_level_dirs_cnt = pos_rand_f ();
		const size_t one_level_files_cnt = pos_rand_f ();
		SizeTVector one_level_files_cnt_list (one_level_files_cnt);
		std::generate (one_level_files_cnt_list.begin(), one_level_files_cnt_list.end(), pos_rand_f);
		const QFileInfoList& one_level_dirs = createTestSubdirs (kOneLevelPath, one_level_dirs_cnt);
		const QFileInfoList& one_level_files = createTestFiles (kOneLevelPath, one_level_files_cnt_list);

		const size_t common_dirs_cnt = pos_rand_f ();
		const size_t common_files_cnt_1 = pos_rand_f ();
		const size_t common_files_cnt_2 = pos_rand_f ();
		
		SizeTVector common_files_cnt_list_1 (common_files_cnt_1);
		SizeTVector common_files_cnt_list_2 (common_files_cnt_2);
		
		std::generate (common_files_cnt_list_1.begin(), common_files_cnt_list_1.end(), pos_rand_f);
		std::generate (common_files_cnt_list_2.begin(), common_files_cnt_list_2.end(), pos_rand_f);

		const QFileInfoList & common_level_dirs_1 = createTestSubdirs(kCommonPath, common_dirs_cnt);
		const QFileInfoList & common_level_files_1 = createTestFiles(kCommonPath, common_files_cnt_list_1);
		const QFileInfoList & common_level_files_2 = createTestFiles(kCommonPath + "/0", common_files_cnt_list_2);

		//

		expected_stats_[kCleanPath] = StatDataPtr (new StatData);

		StatDataPtr single_stat_data_ptr (new StatData);
		single_stat_data_ptr->collectFilesExts(single_file_info_list);
		expected_stats_[kSingleFilePath] = StatDataPtr (single_stat_data_ptr);

		StatDataPtr nested_clean_stat (new StatData);
		expected_stats_[kNestedCleanPath] = nested_clean_stat;

		StatDataPtr one_level_stat (new StatData);
		one_level_stat->collectFilesExts(one_level_files);
		expected_stats_[kOneLevelPath] = one_level_stat;

		StatDataPtr common_stat (new StatData);
		common_stat->collectFilesExts(common_level_files_1);
		common_stat->collectFilesExts(common_level_files_2);
		expected_stats_[kCommonPath] = common_stat;
	}

	void CollectorTest::cleanupTestCase()
	{
		std::for_each (kAllPaths.begin(), kAllPaths.end(), &rmPathInTempRecursive);
	}

	void CollectorTest::handleScanFinished(const QString & path, const core::StatDataPtr & ptr)
	{
		current_stat_ = ptr;

		emit scanFinished();
	}

	void CollectorTest::prepareData() const
	{
		QTest::addColumn<QString>("path");
		QTest::addColumn<StatDataPtr> ("expected_stat_data");

		QTest::newRow (kCleanPath.toAscii().constData()) << kCleanPath << expected_stats_[kCleanPath];
		QTest::newRow (kSingleFilePath.toAscii().constData()) << kSingleFilePath << expected_stats_[kSingleFilePath];
		QTest::newRow (kNestedCleanPath.toAscii().constData()) << kNestedCleanPath << expected_stats_[kNestedCleanPath];
		QTest::newRow (kOneLevelPath.toAscii().constData()) << kOneLevelPath << expected_stats_[kOneLevelPath];
		QTest::newRow (kCommonPath.toAscii().constData()) << kCommonPath << expected_stats_[kCommonPath];
	}
}