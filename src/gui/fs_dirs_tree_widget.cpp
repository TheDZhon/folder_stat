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

#include "fs_dirs_tree_widget.h"

#include "fs_lightweight_icon_provider.h"

namespace
{
	enum { kTreeViewSizeColumn = 1,
		   kTreeViewTypeColumn = 2,
		   kTreeViewDateColumn = 3
		 };
}

namespace gui
{
	DirsTreeWidget::DirsTreeWidget (QWidget* parent /* = 0 */) :
		QTreeView (parent),
		model_()
	{
		initModel();
		initView();
		initContextMenu();
	}

	DirsTreeWidget::~DirsTreeWidget () {}

	QString DirsTreeWidget::currentFilePath() const
	{
		if (currentIndex().isValid()) {
			return model_.filePath (currentIndex());
		}
		return QString();
	}

	void DirsTreeWidget::currentChanged (const QModelIndex& current, const QModelIndex& previous)
	{
		QTreeView::currentChanged (current, previous);
		if (current.isValid()) {
			const QString& path = model_.filePath (current);
			emit currentPathChanged (path);
		}
	}

	void DirsTreeWidget::handleContextMenu (const QPoint& p) const
	{
		const QModelIndex& ind = indexAt (p);
		if (ind.isValid()) {
			QMenu contextMenu;
			contextMenu.addActions (actions());

			contextMenu.exec (mapToGlobal (p));
		}
	}

	void DirsTreeWidget::initModel()
	{
		model_.setIconProvider (new LightweightIconProvider);
		model_.setRootPath (QDir::currentPath());
		model_.setFilter (QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

		model_.removeColumn (kTreeViewSizeColumn);
		model_.removeColumn (kTreeViewTypeColumn);
		model_.removeColumn (kTreeViewDateColumn);
	}

	void DirsTreeWidget::initView()
	{
		setModel (&model_);
		setUniformRowHeights (true);
		setContextMenuPolicy (Qt::CustomContextMenu);

		hideColumn (kTreeViewSizeColumn);
		hideColumn (kTreeViewTypeColumn);
		hideColumn (kTreeViewDateColumn);
	}

	void DirsTreeWidget::initContextMenu()
	{
		connect (this,
				 SIGNAL (customContextMenuRequested (const QPoint&)),
				 SLOT (handleContextMenu (const QPoint&)));
	}
}
