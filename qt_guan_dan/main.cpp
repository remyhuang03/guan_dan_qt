#include "guan_dan.h"
#include <QtWidgets/QApplication>


int main(int argc, char* argv[])
{

	QApplication a(argc, argv);
	guan_dan w;
	w.show();
	return a.exec();
}
