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

#ifndef FS_SETTINGS_DIALOG_H__
#define FS_SETTINGS_DIALOG_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "ui_fs_settings_dialog.h"

#include <QDialog>

namespace gui
{
	/**
	 ** Settings POD structure.
	 ** Represents all options available in settings dialog.
	 ** @sa SettingsDialog
	 **/
	struct SettingsData {
		bool exit_confirmation_;       /**< Show exit confirmation on main window closing */

		bool show_tray_icon_;          /**< Show icon in system tray */
		bool allow_minimize_to_tray_;  /**< Allow main window minimization to system tray */
		bool show_tray_notifications_; /**< Show system tray notifications */
		size_t notification_timeout_;  /**< Timeout for notification in system tray */

		bool use_cache_;               /**< Use cache for collecting statistics */
		size_t max_cache_items_;       /**< Max items in cache (cache capacity) */
	};
	/**
	 ** Settings dialog widget.
	 ** UI for settings modifications.
	 ** @sa SettingsData
	 **/
	class SettingsDialog : public QDialog
	{
		Q_OBJECT
	public:
		/**
		 ** Default QObject-style constructor.
		 ** @param[in] parent parent object
		 **/
		SettingsDialog (QWidget* parent = 0);
		/**
		 ** Destructor.
		 **/
		virtual ~SettingsDialog();
		/**
		 ** Set settings data structure.
		 ** @param[in] settings_data settings data
		 **/
		void setSettings (const SettingsData& settings_data) { settings_data_ = settings_data; }
		/**
		 ** Get settings data structure.
		 ** @return current settings
		 **/
		inline SettingsData settings () const { return settings_data_; }
		/**
		 ** Load settings data from system storage (platform-dependent).
		 **/
		void loadState ();
		/**
		 ** Save settings data in system storage (platform-dependent).
		 **/
		void saveState () const;
	private slots:
		void handleGeneral ();
		void handleTray ();
		void handleCache ();
	private:
		Q_DISABLE_COPY (SettingsDialog);

		void connectUi ();
		void initSettings ();

		Ui::SettingsDialog ui;
		SettingsData settings_data_;
	};
}

#endif // FS_SETTINGS_DIALOG_H__