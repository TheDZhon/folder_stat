#ifndef FOLDER_STAT_WINDOW_H
#define FOLDER_STAT_WINDOW_H

#include <QtGui/QMainWindow>
#include "ui_folder_stat_window.h"

class FolderStatWindow : public QMainWindow
{
	Q_OBJECT

public:
	FolderStatWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FolderStatWindow();

private:
	Ui::FolderStatWindowClass ui;
};

#endif // FOLDER_STAT_WINDOW_H
