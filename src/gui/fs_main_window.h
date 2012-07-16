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

#ifndef FS_MAIN_WINDOW_H__
#define FS_MAIN_WINDOW_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "ui_fs_main_window.h"

#include "core/fs_collector.h"

#include <QtGui/QMainWindow>
#include <QSystemTrayIcon>

namespace gui
{
	class MainWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		MainWindow (QWidget* parent = 0, Qt::WFlags flags = 0);
		virtual ~MainWindow();

		virtual void setVisible(bool);
	private slots:
		void handleQuitAction ();
		void handleRefreshAction ();
		void handleSettingsAction ();
		void handleAboutAction ();

		void handleTrayActivated(QSystemTrayIcon::ActivationReason);

		void handleError (const QString& path, const QString & error);
		void handleProgress (const QString & path, core::Collector::ProgressUpdate);
		void handleFinished (const QString & path, const core::StatDataPtr & data);
	private:
		Q_DISABLE_COPY (MainWindow);

		void configureUi ();
		void connectUi () const;

		virtual void closeEvent (QCloseEvent*);

		void loadWindowState ();
		void saveWindowState () const;

		Ui::FolderStatWindowClass ui;

		QSystemTrayIcon tray_icon_;
	};
}

#endif // FS_MAIN_WINDOW_H__

