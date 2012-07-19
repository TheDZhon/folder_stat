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
#include <QProgressBar>

using namespace core;

namespace
{
	const size_t kStatusTruncateSymbols = 40;
}

const char* kAppName = QT_TRANSLATE_NOOP ("gui::MainWindow", "Folder Stat GUI");
const char* kOrgName = QT_TRANSLATE_NOOP ("gui::MainWindow", "PrefixInc");
const char* kCopyright = QT_TRANSLATE_NOOP ("gui::MainWindow", "(c) 2012, Eugene Mamin");

namespace gui
{
	MainWindow::MainWindow (QWidget* parent, Qt::WFlags flags)
		: QMainWindow (parent, flags),
		  ui(),
		  settings_dialog_(),
		  settings_data_ (settings_dialog_.settings()),
		  collector_(),
		  notifier_ (this)
	{
		ui.setupUi (this);

		loadWindowState ();
		connectUi();
		configureUi();

		qRegisterMetaType<core::StatDataPtr> ("core::StatDataPtr");
	}

	MainWindow::~MainWindow()
	{
		notifier_.setParent (0);
	}

	void MainWindow::setVisible (bool visible)
	{
		ui.hideWindowAction->setVisible (visible);
		ui.restoreWindowAction->setVisible (!visible);

		QMainWindow::setVisible (visible);
	}

	void MainWindow::handleCurrentPathChanged (const QString& path)
	{
		ui.scanAction->setEnabled (true);
		ui.refreshAction->setEnabled (false);
	}

	void MainWindow::handleQuitAction()
	{
		settings_dialog_.setSettings (settings_data_);
		settings_dialog_.saveState();
		saveWindowState();
		qApp->quit();
	}

	void MainWindow::handleScanAction()
	{
		processScan (settings_data_.use_cache_);
	}

	void MainWindow::handleRefreshAction()
	{
		processScan (false);
	}

	void MainWindow::handleSettingsAction()
	{
		const int exit_status = settings_dialog_.exec();
		if (exit_status == QDialog::Rejected) { return; }

		settings_data_ = settings_dialog_.settings();
		settings_dialog_.saveState();

		processSettingsData();
	}

	void MainWindow::handleAboutAction()
	{
		QMessageBox::about (this, tr ("About"), QString (tr (kAppName)) + " " + QString (tr (kCopyright)));
	}

	void MainWindow::handleAboutQtAction()
	{
		QMessageBox::aboutQt (this);
	}

	void MainWindow::handleError (const QString& path, const QString& error)
	{
		processFinish (false, error);
	}

	void MainWindow::handleDirectSubfolders (const QString& path, int cnt)
	{
		ui.childrenCntSpinBox->setEnabled (true);
		ui.childrenCntSpinBox->setValue (cnt);
	}

	void MainWindow::handleCurrentScannedDir (const QString& p, const QString& current)
	{
		QString copy_p (current.right (kStatusTruncateSymbols));

		if (p.length() > kStatusTruncateSymbols) {
			copy_p.prepend ("...");
		}

		notifier_.statusMessage (copy_p);
	}

	void MainWindow::handleFinished (const QString& path, const StatDataPtr& ptr)
	{
		ui.tableView->setData (path, ptr);

		processFinish (true, tr("Task finished successfully"));
	}

	void MainWindow::configureUi()
	{
		setWindowTitle (tr (kAppName));

		ui.hideWindowAction->setIcon (style()->standardIcon (QStyle::SP_TitleBarMinButton));
		ui.restoreWindowAction->setIcon (style()->standardIcon (QStyle::SP_TitleBarNormalButton));

		QMenu* tray_menu = new QMenu;
		tray_menu->addAction (ui.hideWindowAction);
		tray_menu->addAction (ui.restoreWindowAction);
		tray_menu->addSeparator();
		tray_menu->addAction (ui.clearCacheAction);
		tray_menu->addAction (ui.cancelCollectAction);
		tray_menu->addSeparator();
		tray_menu->addAction (ui.quitAction);
		notifier_.addTrayMenu (tray_menu);

		ui.treeView->addAction (ui.scanAction);
		ui.treeView->addAction (ui.refreshAction);

		processSettingsData();
	}

	void MainWindow::connectUi() const
	{
		connect (ui.quitAction,
				 SIGNAL (triggered()),
				 SLOT (handleQuitAction()));
		connect (ui.scanAction,
				 SIGNAL (triggered()),
				 SLOT (handleScanAction()));
		connect (ui.refreshAction,
				 SIGNAL (triggered()),
				 SLOT (handleRefreshAction()));
		connect (ui.cancelCollectAction,
				 SIGNAL (triggered()),
				 &collector_,
				 SLOT (cancel()));
		connect (ui.clearCacheAction,
				 SIGNAL (triggered()),
				 &collector_,
				 SLOT (clearCache ()));
		connect (ui.settingsAction,
				 SIGNAL (triggered()),
				 SLOT (handleSettingsAction()));
		connect (ui.aboutAction,
				 SIGNAL (triggered()),
				 SLOT (handleAboutAction()));
		connect (ui.aboutQtAction,
				 SIGNAL (triggered()),
				 SLOT (handleAboutQtAction()));

		connect (&collector_,
				 SIGNAL (error (const QString&, const QString&)),
				 SLOT (handleError (const QString&, const QString&)));
		connect (&collector_,
				 SIGNAL (currentScannedDir (const QString&, const QString&)),
				 SLOT (handleCurrentScannedDir (const QString&, const QString&)));
		connect (&collector_,
				 SIGNAL (directSubfolders (const QString&, int)),
				 SLOT (handleDirectSubfolders (const QString&, int)));
		connect (&collector_,
				 SIGNAL (finished (const QString&, const core::StatDataPtr&)),
				 SLOT (handleFinished (const QString, const core::StatDataPtr&)));

		connect (ui.treeView,
				 SIGNAL (currentPathChanged (const QString&)),
				 SLOT (handleCurrentPathChanged (const QString&)));
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
				notifier_.trayMessage (tr ("The program will keep running in the "
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
		notifier_.setSettingsData (settings_data_);

		ui.hideWindowAction->setEnabled (settings_data_.allow_minimize_to_tray_);
		ui.restoreWindowAction->setEnabled (settings_data_.allow_minimize_to_tray_);

		collector_.setCacheSize (settings_data_.max_cache_items_);
	}

	void MainWindow::processScan (bool use_cache)
	{
		ui.cancelCollectAction->setEnabled (true);
		ui.scanAction->setDisabled (true);
		ui.refreshAction->setDisabled (true);

		const QString& path = ui.treeView->currentFilePath();

		ui.collectedStatGroupBox->setTitle (path);

		notifier_.setBusy (true);

		collector_.collect (path, use_cache);
	}

	void MainWindow::processFinish (bool success, const QString& mess)
	{
		notifier_.setBusy (false);

		const QString& path = ui.treeView->currentFilePath();

		ui.scanAction->setDisabled (success);
		ui.refreshAction->setEnabled (success);
		ui.cancelCollectAction->setDisabled (true);

		if (success) {
			notifier_.statusMessage (mess);
			notifier_.trayMessage (mess);
		} else {
			notifier_.errorMessage (mess);
		}

		if (!success) { clearStats(); }
	}

	void MainWindow::clearStats()
	{
		ui.collectedStatGroupBox->setTitle (QString());
		ui.tableView->clearData();
		ui.childrenCntSpinBox->setDisabled (true);
		ui.childrenCntSpinBox->setValue (0);
	}
}
