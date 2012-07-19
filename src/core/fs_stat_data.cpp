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

#include "fs_stat_data.h"

namespace
{
	const quint64 kULLZero = Q_UINT64_C (0);
}

namespace core
{
	StatData::ExtensionRecord::ExtensionRecord () :
		count_ (kULLZero),
		total_size_ (kULLZero)
	{}

	void StatData::appendOther (const StatData& other)
	{
		const ExtRecordsMap& otherFilesMap = other.extRecords();
		typedef ExtRecordsMap::const_iterator It;

		for (It it = otherFilesMap.begin();
			 it != otherFilesMap.end();
			 ++it) {

			const QString& ext = it.key();
			ExtensionRecord& record = ext_records_[ext];

			record.count_ += it->count_;
			record.total_size_ += it->total_size_;
		}

		all_.count_ += other.all_.count_;
		all_.total_size_ += other.all_.total_size_;
	}

	void StatData::collectFilesExts (const QFileInfoList& l)
	{
		typedef QFileInfoList::const_iterator It;

		for (It it = l.begin(); it != l.end();
			 ++it)
		{
			Q_ASSERT (it->isFile());
			incExtCnt (it->suffix(), it->size());	
		}
	}

	void StatData::incExtCnt (const QString& ext, quint64 sz)
	{
		ExtensionRecord& record = ext_records_[ext];
		++record.count_;
		record.total_size_ += sz;

		++all_.count_;
		all_.total_size_ += sz;
	}

	bool StatData::operator== (const StatData & other) const {
		return (ext_records_ == other.ext_records_)
			&& (all_ == other.all_);
	}

	bool operator== (const StatData::ExtensionRecord& l, const StatData::ExtensionRecord& r)
	{
		return (l.count_ == r.count_) && (l.total_size_ == r.total_size_);
	}
}