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

#ifndef FS_NOTIFIER_H__
#define FS_NOTIFIER_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "fs_settings_dialog.h"

#include <QProgressBar>
#include <QMainWindow>
#include <QStatusBar>
#include <QSystemTrayIcon>

namespace gui
{
	/**
	 ** Notifier class.
	 ** Responsible for all notifications from MainWindow.
	 ** @sa MainWindow
	 **/
	class Notifier: public QObject
	{
		Q_OBJECT
	public:
		/**
		 ** Default QObject-style constructor.
		 ** @param[in] parent parent QMainWindow instance.
		 **/
		Notifier (QMainWindow* parent);
		/**
		 ** Destructor.
		 **/
		~Notifier();
		/**
		 ** Set settings for notifications.
		 ** @param[in] settings_data settings
		 **/
		void setSettingsData (const SettingsData& settings_data);
		/**
		 ** Show status message (in status bar at this moment).
		 ** @param[in] mess message
		 **/
		void statusMessage (const QString& mess);
		/**
		 ** Show message in system tray.
		 ** @param[in] mess message
		 **/
		void trayMessage (const QString& mess);
		/**
		 ** Show error message.
		 ** @param[in] err_mess error message
		 **/
		void errorMessage (const QString& err_mess);
		/**
		 ** Add menu to tray icon.
		 ** Takes ownership of menu object.
		 ** @param[in] menu menu object
		 **/
		void addTrayMenu (QMenu* menu);
		/**
		 ** Enable/disable busy indication.
		 ** @param[in] on enable if true, disable if false
		 **/
		void setBusy (bool on);
	private slots:
		void init ();
		void handleTrayActivated (QSystemTrayIcon::ActivationReason);
	private:
		SettingsData settings_data_;

		QStatusBar* status_bar_;
		QSystemTrayIcon tray_icon_;
		QProgressBar progressbar_;
	};
}

#endif // FS_NOTIFIER_H__