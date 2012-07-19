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

#ifndef FS_DIRS_TREE_WIDGET__
#define FS_DIRS_TREE_WIDGET__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "fs_lightweight_icon_provider.h"

#include <QTreeView>
#include <QFileSystemModel>
#include <QMenu>

namespace gui
{
	/**
	 ** Tree widget with filesystem structure.
	 ** Based on QTreeView and QFileSystemModel.
	 ** @sa LightweightIconProvider
	 **/
	class DirsTreeWidget: public QTreeView
	{
		Q_OBJECT
	public:
		/**
		 ** Default QObject-style constructor.
		 ** @param[in] parent parent object
		 **/
		DirsTreeWidget (QWidget* parent = 0);
		/**
		 ** Destructor.
		 **/
		virtual ~DirsTreeWidget();
		/**
		 ** Get current filepath.
		 ** @return filepath
		 **/
		QString currentFilePath() const;
	signals:
		/**
		 ** Report new current path.
		 ** @param[in] path new current path
		 **/
		void currentPathChanged (const QString& path) const;
	protected:
		/**
		 ** Report new current index.
		 ** Reimplemented from base.
		 ** @param[in] current new current index
		 ** @param[in] previous old current index
		 **/
		void currentChanged (const QModelIndex& current, const QModelIndex& previous);
	private slots:
		void handleContextMenu (const QPoint& p) const;
	private:
		Q_DISABLE_COPY (DirsTreeWidget);

		void initModel();
		void initView();
		void initContextMenu();

		QFileSystemModel model_;
	};
}

#endif // FS_DIRS_TREE_WIDGET__