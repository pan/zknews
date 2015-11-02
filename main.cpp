#include "webwid.h"
#include <QApplication>
#include <QWebSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WebWid *w = new WebWid;
    w->setWindowFlags(w->windowFlags() ^ Qt::WindowMaximizeButtonHint);
    w->show();

    int ret = a.exec();
    delete w;
    QWebSettings::clearMemoryCaches();
    return ret;
}
