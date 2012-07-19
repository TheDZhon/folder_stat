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

#include "fs_stat_data_test.h"

#include "test/fs_test_utils.h"

#include <QtGlobal>
#include <QTest>
#include <QDir>

#include <ctime>

using namespace core;

namespace
{
	const QFileInfo kRootPath = QDir::rootPath();
	const QString kTestFolderName = QString ("statdatatest");

	const size_t kTestFilesNumber = 100;

	const test::iota_emulator_generator<size_t> iota_gen (0, 1);
}

Q_DECLARE_METATYPE (QFileInfoList)

namespace test
{
	void StatDataTest::testAppendOther()
	{
		typedef StatData::ExtRecordsMap::const_iterator It;

		QFETCH (QFileInfoList, expected_data);

		StatData l;
		StatData r;

		l.collectFilesExts (expected_data);
		r.collectFilesExts (expected_data);

		QCOMPARE (l, r);

		l.appendOther (r);

		const StatData::ExtRecordsMap& l_rec_map = l.extRecords();
		const StatData::ExtRecordsMap& r_rec_map = r.extRecords();
		for (It it = l_rec_map.begin();
			 it != l_rec_map.end();
			 ++it) {
			QCOMPARE (it->count_, r_rec_map[it.key()].count_ * 2);
			QCOMPARE (it->total_size_, r_rec_map[it.key()].total_size_ * 2);
		}

		QCOMPARE (l.all().count_, r.all().count_ * 2);
		QCOMPARE (l.all().total_size_, r.all().total_size_ * 2);
	}

	void StatDataTest::testCollectFilesExts()
	{
		typedef StatData::ExtRecordsMap::const_iterator It;

		QFETCH (QFileInfoList, expected_data);

		StatData stat_obj;

		stat_obj.collectFilesExts (expected_data);
		const StatData::ExtRecordsMap& rec_map = stat_obj.extRecords();

		for (It it = rec_map.begin();
			 it != rec_map.end();
			 ++it) {
			QCOMPARE (it->count_, static_cast<quint64> (it.key().toInt()));
			QCOMPARE (it->total_size_, it->count_ * kDefaultFileSz);
		}
	}

	void StatDataTest::initTestCase()
	{
		qsrand (time (NULL));

		mkPathInTemp (kTestFolderName);

		SizeTVector v (kTestFilesNumber);
		std::generate (v.begin(), v.end(), iota_gen);

		test_files_ = createTestFiles (kTestFolderName, v);
	}

	void StatDataTest::cleanupTestCase()
	{
		rmPathInTempRecursive (kTestFolderName);
	}

	void StatDataTest::prepareFiles() const
	{
		QTest::addColumn<QFileInfoList> ("expected_data");

		QTest::newRow ("empty") << QFileInfoList();
		QTest::newRow ("one") << (QFileInfoList() << test_files_.at (0));
		QTest::newRow ("all") << test_files_;
	}
}