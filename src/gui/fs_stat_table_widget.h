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

#ifndef FS_STAT_TABLE_WIDGET_H__
#define FS_STAT_TABLE_WIDGET_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "core/fs_stat_data.h"

#include <QAbstractTableModel>
#include <QTableView>

namespace gui
{
	/**
	 ** Statistics table model.
	 ** Integrates statistics data into table view.
	 ** @sa StatTableWidget
	 **/
	class StatTableModel:
		public QAbstractTableModel
	{
		Q_OBJECT
	public:
		/**
		 ** Default QObject-style constructor.
		 ** @param[in] parent parent object
		 **/
		StatTableModel (QObject* parent = 0);
		/**
		 ** Destructor.
		 **/
		virtual ~StatTableModel();
		/**
		 ** Set new data.
		 ** Table model refreshes all data in the associated view.
		 ** @param[in] data new data
		 **/
		void setData (const core::StatDataPtr& data);
		/**
		 ** Clear data.
		 ** Associated view becomes empty.
		 **/
		void clearData ();
		/**
		 ** Get row count.
		 ** Reimplemented from base.
		 ** @param[in] parent unused parameter
		 ** @return row count
		 **/
		virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;
		/**
		 ** Get column count.
		 ** Reimplemented from base.
		 ** @param[in] parent unused parameter
		 ** @return column count
		 **/
		virtual int columnCount (const QModelIndex& parent = QModelIndex()) const;
		/**
		 ** Get data for given index.
		 ** Reimplemented from base.
		 ** @param[in] index index object
		 ** @param[in] role index display role
		 ** @return data for display in cell
		 **/
		virtual QVariant data (const QModelIndex& index = QModelIndex(), int role = Qt::DisplayRole) const;
		/**
		 ** Get headers data (titles).
		 ** @param[in]
		 **/
		virtual QVariant headerData (int section, Qt::Orientation, int role) const;
	private:
		core::StatData::ExtRecordsMap data_;
	};
	/**
	 ** Statistics table widget.
	 **/
	class StatTableWidget: public QTableView
	{
		Q_OBJECT
	public:
		/**
		 ** Default QObject-style constructor.
		 ** @param[in] parent parent object
		 **/
		StatTableWidget (QWidget* parent);
		/**
		 ** Destructor.
		 **/
		virtual ~StatTableWidget();
	public slots:
		/**
		 ** Set new statistics data for display.
		 ** Translates call to underlying model.
		 ** @param[in] path scanned path
		 ** @param[in] data collected data
		 **/
		inline void setData (const QString& path, const core::StatDataPtr& data) { model_.setData (data); }
		/**
		 ** Clear displaying data.
		 ** Translates call to underlying model.
		 **/
		inline void clearData () { model_.clearData(); }
	private:
		StatTableModel model_;
	};
}

#endif // FS_STAT_TABLE_WIDGET_H__