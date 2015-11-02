#include "systray.h"
#include <QIcon>

Systray::Systray(QObject *parent) : QObject(parent)
{
    icon = new QSystemTrayIcon(QIcon(":/img/icon.xpm"), parent);
}

void Systray::show() {
    icon->show();
}

