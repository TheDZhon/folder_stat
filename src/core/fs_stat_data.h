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

#ifndef FS_STAT_DATA_H__
#define FS_STAT_DATA_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <QtGlobal>
#include <QSharedPointer>
#include <QMetaType>
#include <QFileInfo>
#include <QHash>

namespace core
{
	class StatData;
	typedef QSharedPointer<StatData> StatDataPtr;

	class StatData
	{
	public:
		struct ExtensionRecord {
			ExtensionRecord();

			quint64 count_;
			quint64 total_size_;
		};

		typedef QHash<QString, ExtensionRecord> ExtRecordsMap;

		StatData(): subdirs_(), ext_records_(), all_() {}
		~StatData() {}

		void appendOther (const StatData&);

		inline void setSubdirs (const QFileInfoList & subdirs) { subdirs_ = subdirs; }
		inline QFileInfoList subdirs () const { return subdirs_; }

		void collectFilesExts (const QFileInfoList&);
		inline ExtRecordsMap extRecords () const { return ext_records_; }

		inline ExtensionRecord all () const { return all_; }

		bool operator==(const StatData & other) const;
	private:
		Q_DISABLE_COPY (StatData);

		void incExtCnt (const QString & ext, quint64 sz);

		QFileInfoList subdirs_;
		QHash<QString, ExtensionRecord> ext_records_;

		ExtensionRecord all_;
	};

	bool operator==(const StatData::ExtensionRecord&, const StatData::ExtensionRecord&);	
}

Q_DECLARE_METATYPE (core::StatDataPtr);
Q_DECLARE_METATYPE (QFileInfoList);
Q_DECLARE_METATYPE (core::StatData::ExtRecordsMap);

#endif // FS_STAT_DATA_H__

