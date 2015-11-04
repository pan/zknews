#ifndef WEBWID_H
#define WEBWID_H

#include "systray.h"
#include <QMainWindow>
#include <QStringListModel>
#include <QWebFrame>
#include <QWebPage>
#include <QTimer>

const QUrl BJZK("http://www.bjeea.cn/html/zk/");
const QString FILE_ZKNEWS = "/.zknews.txt";
const QString all_news_selector(".newslist ul");
const QString date_selector(".time");
const QString msg_selector(".tit");
const int NEWS_LIMIT(11);                  // get the lastest 11 pieces of news
const int RELOAD_INTERVAL(15 * 60 * 1000); // 15 minuties
//const int RELOAD_INTERVAL(20 * 1000);

namespace Ui {
class WebWid;
}

class WebWid : public QMainWindow
{
    Q_OBJECT

public:
    explicit WebWid(QWidget *parent = 0);
    ~WebWid();

    void loadnew();

protected:
    void cook(const QStringList &news);
    bool save(const QStringList &news);
    bool loadold();
    int diff(const QStringList &news);

private slots:
    void slotLoadFinished(bool ok);
    void slotRefresh();
    void slotHaveRead();
    void slotActiveWidget(QSystemTrayIcon::ActivationReason);
    void slotActiveWidget();
    void slotHideWin();

private:
    Ui::WebWid *ui;
    QWebPage *page;
    QWebFrame *frame;
    QStringListModel *model;
    QString filename;
    QStringList old;
    QStringList news;
    QTimer *timer;
    Systray *tray;

    QString ul2str(const QWebElement &ul);
    void showMainWin();
};

#endif // WEBWID_H
