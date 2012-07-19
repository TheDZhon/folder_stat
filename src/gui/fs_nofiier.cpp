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

#include "fs_nofiier.h"

#include <QTimer>

namespace
{
	const char* kStatusMessage = QT_TRANSLATE_NOOP ("gui::Notifier", "Info");
	const char* kErrorMessage = QT_TRANSLATE_NOOP ("gui::Notifier", "Error");
	const char* kBusyMessage = QT_TRANSLATE_NOOP ("gui::Notifier", "Task started, stay tuned ;)");
}

namespace gui
{
	Notifier::Notifier (QMainWindow* parent) :
		QObject (parent),
		status_bar_ (0),
		tray_icon_(),
		progressbar_()
	{
		Q_ASSERT (parent);

		connect (&tray_icon_,
				 SIGNAL (activated (QSystemTrayIcon::ActivationReason)),
				 SLOT (handleTrayActivated (QSystemTrayIcon::ActivationReason)));

		QTimer::singleShot (0, this, SLOT (init()));
	}

	Notifier::~Notifier()
	{
		progressbar_.setParent (0);
	}

	void Notifier::setSettingsData (const SettingsData& settings_data)
	{
		settings_data_ = settings_data;
		tray_icon_.setVisible(settings_data_.show_tray_icon_);
	}

	void Notifier::statusMessage (const QString& mess)
	{
		status_bar_->showMessage (tr (kStatusMessage) + ": " + mess);
	}

	void Notifier::trayMessage (const QString& mess)
	{
		if (settings_data_.show_tray_icon_ && settings_data_.show_tray_notifications_) {
			tray_icon_.showMessage (tr (kStatusMessage), mess, QSystemTrayIcon::Information, settings_data_.notification_timeout_);
		}
	}

	void Notifier::errorMessage (const QString& err_mess)
	{
		status_bar_->showMessage (tr (kErrorMessage) + ": " + err_mess);
		if (settings_data_.show_tray_icon_ && settings_data_.show_tray_notifications_) {
			tray_icon_.showMessage (tr (kErrorMessage), err_mess, QSystemTrayIcon::Critical, settings_data_.notification_timeout_);
		}
	}

	void Notifier::addTrayMenu (QMenu* menu)
	{
		Q_ASSERT (menu);

		tray_icon_.setContextMenu (menu);
	}

	void Notifier::setBusy (bool on)
	{
		progressbar_.setVisible (on);
		if (on) { trayMessage (tr (kBusyMessage)); }
	}

	void Notifier::init ()
	{
		QMainWindow* window = qobject_cast<QMainWindow*> (parent());
		tray_icon_.setIcon (window->windowIcon());
		progressbar_.setRange (0, 0);

		status_bar_ = window->statusBar();
		status_bar_->addPermanentWidget (&progressbar_);

		progressbar_.hide();
	}

	void Notifier::handleTrayActivated (QSystemTrayIcon::ActivationReason reason)
	{
		switch (reason) {
			case QSystemTrayIcon::DoubleClick:
				if (settings_data_.allow_minimize_to_tray_) {
					QMainWindow* window = qobject_cast<QMainWindow*> (parent());
					if (window) { window->setVisible (!window->isVisible()); }
				};
				break;
			case QSystemTrayIcon::MiddleClick:
				trayMessage (status_bar_->currentMessage());
				break;
			default:
				;
		}
	}
}