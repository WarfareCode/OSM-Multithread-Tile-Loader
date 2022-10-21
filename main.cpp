#include <QApplication>
#include "widgets/mainwidget.h"

int main(int argc, char *argv[])
{
    //http://tools.geofabrik.de/
    QApplication app(argc, argv);
    MainWidget widget;
    widget.show();
    return app.exec();
}

