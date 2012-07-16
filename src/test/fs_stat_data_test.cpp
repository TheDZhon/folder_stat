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

#include <QtGlobal>
#include <QTest>
#include <QDir>

#include <ctime>

using namespace core;

namespace
{
	const QFileInfo kRootPath = QDir::rootPath();
	const size_t kRandDiv = 100;
}

namespace test
{
	void StatDataTest::testSetAndGetSubdirs_data() const
	{
		QTest::addColumn<QFileInfoList> ("dirs");

		QTest::newRow ("empty") << QFileInfoList();
		QTest::newRow ("one") << (QFileInfoList() << QFileInfo ("."));
		QTest::newRow ("common") << (QFileInfoList()
									 << QFileInfo (".")
									 << QFileInfo ("..")
									 << kRootPath);
	}

	void StatDataTest::testSetAndGetSubdirs()
	{
		QFETCH (QFileInfoList, dirs);

		StatData stat_data;

		stat_data.setSubdirs (dirs);
		const QFileInfoList& dirs_from_sdata = stat_data.subdirs();

		QCOMPARE (dirs_from_sdata, dirs);
	}

	void StatDataTest::testIncExtCnt_data() const
	{
		QTest::addColumn<StatData::ExtRecordsMap> ("expected_map");

		StatData::ExtRecordsMap one_item_map;
		StatData::ExtensionRecord one_item;
		one_item.count_ = 10;
		one_item_map.insert ("exe", one_item);

		StatData::ExtRecordsMap common_map;
		const size_t items_cnt = qrand () / kRandDiv;
		for (size_t i = 0; i < items_cnt; ++i) {
			StatData::ExtensionRecord current_item;
			current_item.count_ = qrand() / kRandDiv + 1;
			common_map.insert (QString::number (i), current_item);
		}

		QTest::newRow ("empty") << StatData::ExtRecordsMap();
		QTest::newRow ("one_item") << one_item_map;
		QTest::newRow ("common") << common_map;
	}

	void StatDataTest::testIncExtCnt()
	{
		typedef StatData::ExtRecordsMap::const_iterator It;

		QFETCH (StatData::ExtRecordsMap, expected_map);

		StatData stat_data;

		for (It it = expected_map.constBegin();
			 it != expected_map.constEnd();
			 ++it) {
			for (size_t i = 0; i < it->count_; ++i) {
				stat_data.incExtCnt (it.key(), 0);
			}
		}

		QCOMPARE (stat_data.extRecords(), expected_map);
	}

	void StatDataTest::initTestCase() const
	{
		qsrand (time (NULL));
	}
}