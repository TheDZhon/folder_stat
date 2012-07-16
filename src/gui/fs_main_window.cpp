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
	const char * kAppName = "Folder Stat GUI";
	const char * kOrgName = "PrefixInc";
}

namespace gui
{
	MainWindow::MainWindow (QWidget* parent, Qt::WFlags flags)
		: QMainWindow (parent, flags),
		  ui(),
		  tray_icon_()
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
		saveWindowState();
		qApp->quit();
	}

	void MainWindow::handleRefreshAction()
	{
	}

	void MainWindow::handleSettingsAction()
	{

	}

	void MainWindow::handleAboutAction()
	{

	}

	void MainWindow::handleTrayActivated (QSystemTrayIcon::ActivationReason reason)
	{
		switch (reason) {
			case QSystemTrayIcon::DoubleClick:
				setVisible (!isVisible());
				break;
			case QSystemTrayIcon::MiddleClick:
				tray_icon_.showMessage ("Current status", statusBar()->currentMessage());
				break;
			default:
				;
		}
	}

	void MainWindow::handleError(const QString& path, const QString & error)
	{
		const QString err_mess = "Error processing " + path + ": " + error;

		statusBar()->showMessage(err_mess);
		tray_icon_.showMessage(tr("Error"), err_mess, QSystemTrayIcon::Critical);
	}

	void MainWindow::handleProgress(const QString & path, core::Collector::ProgressUpdate p)
	{
		const QString update_mess = "Processing update: " + path + ": " + QString::number(p);

		statusBar()->showMessage(update_mess);
		tray_icon_.showMessage(tr("Update"), update_mess, QSystemTrayIcon::Information);
	}

	void MainWindow::handleFinished(const QString & path, const StatDataPtr& ptr)
	{
		// build tree
	}

	void MainWindow::configureUi()
	{
		setWindowTitle(tr(kAppName));

		statusBar()->showMessage ("Idle");
		
		tray_icon_.setIcon (windowIcon());
		tray_icon_.setContextMenu (new QMenu);
		tray_icon_.contextMenu()->addAction (ui.hideWindowAction);
		tray_icon_.contextMenu()->addAction (ui.restoreWindowAction);
		tray_icon_.contextMenu()->addSeparator();
		tray_icon_.contextMenu()->addAction (ui.quitAction);

		tray_icon_.show();
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

		connect (&tray_icon_,
				 SIGNAL (activated (QSystemTrayIcon::ActivationReason)),
				 SLOT (handleTrayActivated (QSystemTrayIcon::ActivationReason)));
	}

	void MainWindow::closeEvent (QCloseEvent* ev)
	{
		QMessageBox confirmMsgBox;
		confirmMsgBox.addButton (tr ("Ok"), QMessageBox::AcceptRole);
		const QPushButton* hideButton = confirmMsgBox.addButton (tr ("Hide"), QMessageBox::ActionRole);
		const QPushButton* cancelButton = confirmMsgBox.addButton (tr ("Cancel"), QMessageBox::RejectRole);

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
			tray_icon_.showMessage (tr (kAppName), tr ("The program will keep running in the "
																"system tray. To terminate the program, "
																"choose <b>Exit</b> in the context menu "));
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
}
