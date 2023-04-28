#include "guan_dan.h"
#include <QtWidgets/QApplication>


int main(int argc, char* argv[])
{
	//高分屏适配
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QApplication a(argc, argv);
	guan_dan w;
	w.show();
	return a.exec();
}
