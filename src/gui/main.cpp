#include "folder_stat_window.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FolderStatWindow w;
	w.show();
	return a.exec();
}
