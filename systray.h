#ifndef SYSTRAY_H
#define SYSTRAY_H

#include <QObject>
#include <QSystemTrayIcon>

namespace Ui {
class Systray;
}

class Systray : public QObject
{
    Q_OBJECT
public:
    QSystemTrayIcon *icon;
    explicit Systray(QObject *parent = 0);
    void show();

signals:

public slots:

};

#endif // SYSTRAY_H
