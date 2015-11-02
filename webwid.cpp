#include "webwid.h"
#include "ui_webwid.h"
#include <QWebElement>
#include <QWebElementCollection>
#include <QFile>
#include <QDir>
#include <QTextStream>

WebWid::WebWid(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WebWid)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    tray = new Systray(this);
    page = new QWebPage();
    frame = page->mainFrame();

    connect(frame, SIGNAL(loadFinished(bool)), this, SLOT(slotLoadFinished(bool)));
    connect(ui->refreshButton, SIGNAL(clicked()), this, SLOT(slotRefresh()));
    connect(timer, SIGNAL(timeout()), this, SLOT(slotRefresh()));
    connect(ui->OKButton, SIGNAL(clicked()), this, SLOT(slotHaveRead()));
    connect(tray->icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(slotActiveWidget(QSystemTrayIcon::ActivationReason)));
    connect(tray->icon, SIGNAL(messageClicked()), this, SLOT(slotActiveWidget()));
    connect(ui->systrayButton, SIGNAL(clicked()), this, SLOT(slotHideWin()));

    loadold();
    loadnew();
    tray->show();
}

void WebWid::loadnew()
{
    qDebug() << BJZK;
    frame->load(BJZK);
    timer->start(RELOAD_INTERVAL);
}

void WebWid::slotRefresh() {
    page->triggerAction(QWebPage::ReloadAndBypassCache);
    timer->start(RELOAD_INTERVAL);
}

void WebWid::slotHaveRead() {
    old = news;
    if (save(news)) {
        ui->statusBar->showMessage("OK, I've read all the news. - "
                                   + QTime::currentTime().toString());
    }
    else
        ui->statusBar->showMessage("Saving file failed.");
    slotHideWin();
}

void WebWid::slotHideWin() {
    this->hide();
}

void WebWid::slotActiveWidget(QSystemTrayIcon::ActivationReason reason) {
    if (reason != QSystemTrayIcon::Context) {
        showMainWin();
    }
}

void WebWid::slotActiveWidget() {
    showMainWin();
}

void WebWid::showMainWin() {
    this->show();
    this->setWindowState(Qt::WindowActive);
}

WebWid::~WebWid()
{
    delete page;
    delete ui;
}

/*
<ul>
<li class="time">2014-12-24</li>
<li class="tit">
<p>
    <a title="北京市高等教育自学考试2015年工作日程表" href="/html/zk/xxfb/2014/1224/56069.html">北京市高等教育自学考试2015年工作日程表</a>
</p>
</li>
</ul>
*/
QString WebWid::ul2str(const QWebElement &ul) {
    QString date = ul.findFirst(date_selector).toPlainText();
    QString message = ul.findFirst(msg_selector).toPlainText();
    return (date + " " + message).trimmed();
}

void WebWid::slotLoadFinished(bool ok)
{
    if (ok) {
        QWebElementCollection notice = frame->findAllElements(all_news_selector);
        if (notice.count() < 1) {
            ui->statusBar->showMessage("Something may be wrong. \
            No message got from the web page.");
            return;
        }
        int c=0;
        news.clear();
        foreach(QWebElement msg, notice) {
            news << ul2str(msg);
            if(++c == NEWS_LIMIT)
                break;
        }
        model = new QStringListModel(news);
        ui->listView->setModel(model);
        ui->listView->show();
        cook(news);
    }
    else
        ui->statusBar->showMessage("Cannot download the news from Internet.");
}

void WebWid::cook(const QStringList &news) {
    if (old == news) {
        QString last_update = QTime::currentTime().toString();
        ui->statusBar->showMessage("No new message. - " + last_update);
    }
    else {
        int n = diff(news);
        QString status = QString::number(n);
        if (n>1)
            status += " new messages. - ";
        else
            status += " new message. - ";
        status += QTime::currentTime().toString();
        ui->statusBar->showMessage(status);
        if(!this->isVisible() || !this->isActiveWindow()) {
            QString msg;
            if(n>1)
                msg = QString::number(n) + " new messages.";
            else
                msg = QString::number(n) + " new message.";
            tray->icon->showMessage(QTime::currentTime().toString(), msg);
        }
    }
}

bool WebWid::loadold() {
    filename = QDir::homePath() + FILE_ZKNEWS;
    QFile file(filename);
    if(file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        QString msg;
        while(in.readLineInto(&msg)) {
            old << msg;
        }
        file.close();
        return true;
    }
    return false;
}

bool WebWid::save(const QStringList &news) {
    QFile file(filename);
    if(file.open(QFile::WriteOnly | QFile::Truncate) ) {
        QTextStream out(&file);
        foreach (QString line, news) {
            out << line << endl;
        }
        qDebug() << "written to " << filename;
        file.close();
        return true;
    }
    return false;
}

int WebWid::diff(const QStringList &news) {
    int count = 0;
    foreach (QString msg, news) {
        if(!old.contains(msg))
            count++;
    }
    return count;
}