#include "mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>

#include "pjsipmanager/pjsipmanager.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
    setWindowTitle("pjsip demo");

    setCentralWidget(new QWidget());

    QVBoxLayout *mainLayout = new QVBoxLayout();

    QVBoxLayout *topLayout = new QVBoxLayout();
    topLayout->setMargin(10);

    QHBoxLayout *tfLayout = new QHBoxLayout();
    tfLayout->addWidget(new QLabel("server:"));
    _server = new QLineEdit("ip or domain");
    tfLayout->addWidget(_server);
    tfLayout->addWidget(new QLabel("port"));
    _port = new QLineEdit("");
    tfLayout->addWidget(_port);
    tfLayout->addWidget(new QLabel("protocol"));
    _protocol = new QLineEdit("tcp or udp");
    tfLayout->addWidget(_protocol);
    topLayout->addLayout(tfLayout);


    QHBoxLayout *tf1 = new QHBoxLayout();
    tf1->addWidget(new QLabel("turn server:"));
    _turnServer = new QLineEdit("");
    tf1->addWidget(_turnServer);

    tf1->addWidget(new QLabel("turn server port:"));
    _turnServerPort = new QLineEdit("");
    tf1->addWidget(_turnServerPort);
    topLayout->addLayout(tf1);


    QHBoxLayout *tf2 = new QHBoxLayout();
    tf2->addWidget(new QLabel("user id:"));
    _accountID = new QLineEdit("id");
    tf2->addWidget(_accountID);

    tf2->addWidget(new QLabel("user name:"));
    _accountName = new QLineEdit("name to display");
    tf2->addWidget(_accountName);
    topLayout->addLayout(tf2);

    QHBoxLayout *tsLayout = new QHBoxLayout();

    QPushButton *initSip = new QPushButton("init");
    connect(initSip, &QPushButton::clicked, this, &MainWindow::onInit);
    tsLayout->addWidget(initSip);

    QPushButton *destorySip = new QPushButton("destory");
    connect(destorySip, &QPushButton::clicked, this, &MainWindow::onDestory);
    tsLayout->addWidget(destorySip);

    _previewBtn = new QPushButton("preview local video");
    connect(_previewBtn, &QPushButton::clicked, this, &MainWindow::onPreview);
    tsLayout->addWidget(_previewBtn);


    QPushButton *addCount = new QPushButton("register account");
    connect(addCount, &QPushButton::clicked, this, &MainWindow::onRegister);
    tsLayout->addWidget(addCount, 1, Qt::AlignRight);

    QPushButton *make = new QPushButton("make call");
    connect(make, &QPushButton::clicked, this, &MainWindow::onCall);
    tsLayout->addWidget(make, 1, Qt::AlignRight);

    QPushButton *huangup = new QPushButton("hang up call");
    connect(huangup, &QPushButton::clicked, this, &MainWindow::onHangUp);
    tsLayout->addWidget(huangup, 1, Qt::AlignRight);


    QPushButton *version = new QPushButton("sip version");
    connect(version, &QPushButton::clicked, this, &MainWindow::onVersion);
    tsLayout->addWidget(version, 1, Qt::AlignRight);

    topLayout->addLayout(tsLayout);


    mainLayout->addLayout(topLayout);

    QHBoxLayout *midLayout = new QHBoxLayout();
    _videoWidget = new QWidget();
    _videoWidget->setMinimumSize(QSize(300, 300));
    _videoWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    _videoWidget->setStyleSheet("border:1px solid lightGray;");

    midLayout->addWidget(_videoWidget, 3, Qt::AlignCenter);
    mainLayout->addLayout(midLayout);

    QTextEdit *log = new QTextEdit();
    log->setMaximumHeight(100);
    log->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    connect(this, &MainWindow::postLog, log, &QTextEdit::append);
    mainLayout->addWidget(log);

    centralWidget()->setLayout(mainLayout);

    connect(PjsipManager::shareInstance(), &PjsipManager::log, this, &MainWindow::log);
}
void MainWindow::initTop() {
}
void MainWindow::initMid() {
}
void MainWindow::initBottom() {
}

MainWindow::~MainWindow()
{

}

void MainWindow::log(QString msg) {
    emit postLog(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh::mm::ss zzz")).arg(msg));
}

void MainWindow::onInit() {
    PjsipManager::shareInstance()->init();
}
void MainWindow::onDestory() {
    PjsipManager::shareInstance()->deinit();
}
void MainWindow::onPreview() {
    if (PjsipManager::shareInstance()->stopPreviewVideo()) {
        _previewBtn->setText("preview local video");
    } else {
        QWidget * preview = PjsipManager::shareInstance()->startPreviewVideo();
        if (preview) {
            _previewBtn->setText("stop preview");
            preview->setParent(_videoWidget);
            preview->setGeometry(_videoWidget->rect());
            preview->show();
        } else {
            log("can not preview video");
        }
    }
}

void MainWindow::onVersion() {
    log(QString("pjlib version:%1").arg(PjsipManager::shareInstance()->pjLibVersion()));
}

void MainWindow::onRegister() {
    QString uid = _accountID->text();
    if (uid.isEmpty()) {
        log("input id first");
        _accountID->setFocus();
        return;
    }
    QString name = _accountName->text();
    if (name.isEmpty()) {
        log("input name first");
        _accountName->setFocus();
        return;
    }

    QString turnserver = _turnServer->text();
    qint32 turnserverport = _turnServerPort->text().toInt();

    QString sipserver;
    //QString sipserver = _server->text();
    //qint32 sipport = _port->text().toInt();

    PjsipManager::shareInstance()->createMyAccount(uid, name, sipserver, turnserver, turnserverport);
}

void MainWindow::onCall() {
    QString sipserver = _server->text();
    qint32 sipport = _port->text().toInt();

    PjsipManager::shareInstance()->makeCall(sipserver, sipport);

}
void MainWindow::onHangUp() {
    log("onHangUp");
}