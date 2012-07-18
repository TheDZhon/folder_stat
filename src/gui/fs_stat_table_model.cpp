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

#include "fs_stat_table_model.h"

#include <algorithm>

namespace
{
	enum Columns {
		kExtension,
		kCount,
		kTotalSize,
		kAvgSize,

		kColumnsCnt
	};

	const char* kColumnnames [] = {
		"Extension",
		"Count",
		"Total size",
		"Average size"
	};

	const double kOrderLimit = 1024.;

	const char* kSizesSuffixes [] = {
		"B",
		"KB",
		"MB",
		"GB",
		"TB",
		"EB"
	};
}

QString humanReadableSize (double sz)
{
	size_t ind (0);
	while (sz > kOrderLimit) {
		sz /= kOrderLimit;
		++ind;
	}
	//return QString::number(sz, 'f', 2).append("append(kSizesSuffixes[ind]);
	return QString ("%1 %2").arg (sz, 0, 'f', 2).arg (kSizesSuffixes[ind]);
}

namespace gui
{
	StatTableModel::StatTableModel (QObject* parent) :
		QAbstractTableModel (parent),
		path_(),
		data_()
	{

	}

	StatTableModel::~StatTableModel() {}

	void StatTableModel::clear()
	{
		beginResetModel();

		data_.clear();
		path_.clear();

		endResetModel();
	}

	int StatTableModel::rowCount (const QModelIndex& parent) const
	{
		return data_.size();
	}

	int StatTableModel::columnCount (const QModelIndex& parent) const
	{
		return kColumnsCnt;
	}

	QVariant StatTableModel::data (const QModelIndex& index, int role) const
	{
		typedef core::StatData::ExtRecordsMap::const_iterator It;

		if (role == Qt::DisplayRole) {
			const size_t row = index.row();

			Q_ASSERT (row < static_cast<size_t> (data_.size()));
			Q_ASSERT (row >= 0);

			It it = data_.begin();
			std::advance (it, row);

			const size_t col = index.column();

			switch (col) {
				case kExtension: {
					const QString & ext = it.key();
					return ext.isEmpty() ? "<None>" : ext;
				}
				case kCount: return it->count_;
				case kTotalSize: return humanReadableSize (it->total_size_);
				case kAvgSize: return humanReadableSize (it->total_size_ / static_cast<double> (it->count_));
			}
		}

		return QVariant();
	}

	QVariant StatTableModel::headerData (int section, Qt::Orientation o, int role) const
	{
		if (o == Qt::Horizontal) {
			if (role == Qt::DisplayRole) {
				return tr (kColumnnames[section]);
			}
		}

		return QVariant();
	}

	void StatTableModel::handleNewData (const QString& path, const core::StatDataPtr& stat_data)
	{
		beginResetModel();

		path_ = path;
		data_ = stat_data->extRecords();
		data_["<All>"] = stat_data->all();

		endResetModel();
	}
}