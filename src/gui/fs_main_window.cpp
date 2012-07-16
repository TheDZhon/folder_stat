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

#include "fs_main_window.h"

#include <QSettings>
#include <QMessageBox>
#include <QCloseEvent>
#include <QPushButton>

using namespace core;

namespace
{
	const char* kAppName = "Folder Stat GUI";
	const char* kOrgName = "PrefixInc";
	const char* kCopyright = "(c) 2012, Eugene Mamin";
}

namespace gui
{
	MainWindow::MainWindow (QWidget* parent, Qt::WFlags flags)
		: QMainWindow (parent, flags),
		  ui(),
		  tray_icon_(),
		  settings_dialog_(),
		  settings_data_ (settings_dialog_.settings())
	{
		ui.setupUi (this);

		loadWindowState ();
		connectUi();
		configureUi();
	}

	MainWindow::~MainWindow()
	{}

	void MainWindow::setVisible (bool visible)
	{
		ui.hideWindowAction->setVisible (visible);
		ui.restoreWindowAction->setVisible (!visible);
		QMainWindow::setVisible (visible);
	}

	void MainWindow::handleQuitAction()
	{
		settings_dialog_.setSettings (settings_data_);
		saveWindowState();
		close ();
	}

	void MainWindow::handleRefreshAction()
	{
	}

	void MainWindow::handleSettingsAction()
	{
		const int exit_status = settings_dialog_.exec();
		if (exit_status == QDialog::Rejected) { return; }

		settings_data_ = settings_dialog_.settings();

		processSettingsData();
	}

	void MainWindow::handleAboutAction()
	{
		QMessageBox::about (this, "About", QString (kAppName) + " " + QString (kCopyright));
	}

	void MainWindow::handleAboutQtAction()
	{
		QMessageBox::aboutQt (this);
	}

	void MainWindow::handleTrayActivated (QSystemTrayIcon::ActivationReason reason)
	{
		switch (reason) {
			case QSystemTrayIcon::DoubleClick:
				if (settings_data_.allow_minimize_to_tray_) {
					setVisible (!isVisible());
				};
				break;
			case QSystemTrayIcon::MiddleClick:
				tray_icon_.showMessage ("Current status", statusBar()->currentMessage());
				break;
			default:
				;
		}
	}

	void MainWindow::handleError (const QString& path, const QString& error)
	{
		const QString err_mess = "Error processing " + path + ": " + error;

		statusBar()->showMessage (err_mess);
		if (settings_data_.show_notifications_) {
			tray_icon_.showMessage (tr ("Error"), err_mess, QSystemTrayIcon::Critical);
		}
	}

	void MainWindow::handleProgress (const QString& path, core::Collector::ProgressUpdate p)
	{
		const QString update_mess = "Processing update: " + path + ": " + QString::number (p);

		statusBar()->showMessage (update_mess);
		if (settings_data_.show_notifications_) {
			tray_icon_.showMessage (tr ("Update"), update_mess, QSystemTrayIcon::Information);
		}
	}

	void MainWindow::handleFinished (const QString& path, const StatDataPtr& ptr)
	{
		// build tree
	}

	void MainWindow::configureUi()
	{
		setWindowTitle (tr (kAppName));

		statusBar()->showMessage ("Idle");

		ui.hideWindowAction->setIcon (style()->standardIcon (QStyle::SP_TitleBarMinButton));
		ui.restoreWindowAction->setIcon (style()->standardIcon (QStyle::SP_TitleBarNormalButton));

		tray_icon_.setIcon (windowIcon());

		tray_icon_.setContextMenu (new QMenu);
		QMenu* m = tray_icon_.contextMenu();
		m->addAction (ui.hideWindowAction);
		m->addAction (ui.restoreWindowAction);
		m->addSeparator();
		m->addAction (ui.quitAction);

		processSettingsData();
	}

	void MainWindow::connectUi() const
	{
		connect (ui.quitAction,
				 SIGNAL (triggered()),
				 SLOT (handleQuitAction()));
		connect (ui.refreshAction,
				 SIGNAL (triggered()),
				 SLOT (handleRefreshAction()));
		connect (ui.settingsAction,
				 SIGNAL (triggered()),
				 SLOT (handleSettingsAction()));
		connect (ui.aboutAction,
				 SIGNAL (triggered()),
				 SLOT (handleAboutAction()));
		connect (ui.aboutQtAction,
				 SIGNAL (triggered()),
				 SLOT (handleAboutQtAction()));

		connect (&tray_icon_,
				 SIGNAL (activated (QSystemTrayIcon::ActivationReason)),
				 SLOT (handleTrayActivated (QSystemTrayIcon::ActivationReason)));
	}

	void MainWindow::closeEvent (QCloseEvent* ev)
	{
		if (!settings_data_.exit_confirmation_) { handleQuitAction(); return; }

		QMessageBox confirmMsgBox;
		confirmMsgBox.addButton (tr ("Quit"), QMessageBox::AcceptRole);
		const QPushButton* hideButton = settings_data_.allow_minimize_to_tray_ ?
										confirmMsgBox.addButton (tr ("Hide"), QMessageBox::ActionRole)
										: 0;
		const QPushButton* cancelButton =
			confirmMsgBox.addButton (tr ("Cancel"), QMessageBox::RejectRole);

		confirmMsgBox.setWindowTitle (tr ("Quit confirmation"));
		confirmMsgBox.setText (tr ("Are you sure you want to exit")
							   + " "
							   + tr (kAppName)
							   + "?");
		confirmMsgBox.setIcon (QMessageBox::Question);
		confirmMsgBox.exec();

		if (confirmMsgBox.clickedButton() == cancelButton) {
			ev->ignore();
			return;
		}

		if (confirmMsgBox.clickedButton() == hideButton) {
			if (settings_data_.show_notifications_) {
				tray_icon_.showMessage (tr (kAppName), tr ("The program will keep running in the "
														   "system tray. To terminate the program, "
														   "choose \"Quit\" in the context menu "));
			}
			hide();
			ev->ignore();
			return;
		}

		handleQuitAction();
	}

	void MainWindow::loadWindowState()
	{
		QSettings sets (kOrgName, kAppName);

		sets.beginGroup ("gui");
		restoreGeometry (sets.value ("geometry").toByteArray());
		restoreState (sets.value ("state").toByteArray());
	}

	void MainWindow::saveWindowState() const
	{
		QSettings sets (kOrgName, kAppName);

		sets.beginGroup ("gui");
		sets.setValue ("geometry", saveGeometry());
		sets.setValue ("state", saveState());
	}

	void MainWindow::processSettingsData()
	{
		tray_icon_.setVisible (settings_data_.tray_icon_);
		ui.hideWindowAction->setEnabled (settings_data_.allow_minimize_to_tray_);
		ui.restoreWindowAction->setEnabled (settings_data_.allow_minimize_to_tray_);
	}
}
