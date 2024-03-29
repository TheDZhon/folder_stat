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

#include "fs_settings_dialog.h"

#include <QSettings>

extern const char* kAppName;
extern const char* kOrgName;

namespace gui
{
	SettingsDialog::SettingsDialog (QWidget* parent)
		: QDialog (parent),
		  ui (),
		  settings_data_()
	{
		ui.setupUi (this);

		loadState();
		initSettings();
		connectUi();
	}

	SettingsDialog::~SettingsDialog()
	{
		saveState();
	}

	void SettingsDialog::loadState()
	{
		QSettings sets (kOrgName, kAppName);

		sets.beginGroup ("settings");

		ui.exitConfirmationCheckBox->setChecked (sets.value ("exit_confirm", true).toBool());
		ui.trayIconCheckBox->setChecked (sets.value ("tray_icon", true).toBool());
		ui.allowMinimizeToTrayCheckBox->setChecked (sets.value ("allow_min_to_tray", true).toBool());
		ui.showNotificationsCheckBox->setChecked (sets.value ("tray_notifications", true).toBool());
		ui.notificationTimeoutSpinBox->setValue (sets.value ("tray_notify_timeout", 1000).toInt());
		ui.useCacheCheckBox->setChecked (sets.value ("use_cache", true).toBool());
		ui.cacheMaxItemsSpinBox->setValue (sets.value ("max_cache_items", 10000).toInt());
	}

	void SettingsDialog::saveState() const
	{
		QSettings sets (kOrgName, kAppName);

		sets.beginGroup ("settings");

		sets.setValue ("exit_confirm", settings_data_.exit_confirmation_);
		sets.setValue ("tray_icon", settings_data_.show_tray_icon_);
		sets.setValue ("allow_min_to_tray", settings_data_.allow_minimize_to_tray_);
		sets.setValue ("tray_notifications", settings_data_.show_tray_notifications_);
		sets.setValue ("tray_notify_timeout", static_cast<int> (settings_data_.notification_timeout_));
		sets.setValue ("use_cache", settings_data_.use_cache_);
		sets.setValue ("max_cache_items", static_cast<int> (settings_data_.max_cache_items_));
	}

	void SettingsDialog::handleGeneral()
	{
		settings_data_.exit_confirmation_ = ui.exitConfirmationCheckBox->isChecked ();
	}

	void SettingsDialog::handleTray()
	{
		settings_data_.show_tray_icon_ = ui.trayIconCheckBox->isChecked ();
		settings_data_.allow_minimize_to_tray_ = ui.allowMinimizeToTrayCheckBox->isChecked ();
		settings_data_.show_tray_notifications_ = ui.showNotificationsCheckBox->isChecked ();
		settings_data_.notification_timeout_ = ui.notificationTimeoutSpinBox->value ();
	}

	void SettingsDialog::handleCache()
	{
		settings_data_.use_cache_ = ui.useCacheCheckBox->isChecked();
		ui.cacheMaxItemsSpinBox->setEnabled (settings_data_.use_cache_);
		settings_data_.max_cache_items_ = ui.cacheMaxItemsSpinBox->value ();
	}

	void SettingsDialog::connectUi()
	{
		connect (ui.exitConfirmationCheckBox,
				 SIGNAL (clicked (bool)),
				 SLOT (handleGeneral()));

		connect (ui.trayIconCheckBox,
				 SIGNAL (clicked (bool)),
				 SLOT (handleTray()));
		connect (ui.allowMinimizeToTrayCheckBox,
				 SIGNAL (clicked (bool)),
				 SLOT (handleTray()));
		connect (ui.showNotificationsCheckBox,
				 SIGNAL (clicked (bool)),
				 SLOT (handleTray()));
		connect (ui.notificationTimeoutSpinBox,
				 SIGNAL (valueChanged (int)),
				 SLOT (handleTray()));

		connect (ui.useCacheCheckBox,
				 SIGNAL (toggled (bool)),
				 SLOT (handleCache()));
		connect (ui.cacheMaxItemsSpinBox,
				 SIGNAL (valueChanged (int)),
				 SLOT (handleCache()));
	}

	void SettingsDialog::initSettings()
	{
		handleGeneral();
		handleTray();
		handleCache();
	}
}
