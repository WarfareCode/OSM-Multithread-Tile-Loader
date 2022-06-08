#include <QApplication>
#include "widgets/mainwidget.h"

int main(int argc, char *argv[])
{
    //http://tools.geofabrik.de/
    QApplication a(argc, argv);
    MainWidget widget(Q_NULLPTR);
    widget.show();
    return a.exec();
}

