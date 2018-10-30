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
    centralWidget()->setLayout(new QVBoxLayout());
    

    initTop();
    initMid();
    initBottom();


    connect(PjsipManager::shareInstance(), &PjsipManager::log, this, &MainWindow::log);

    _server->setText("117.78.36.159");
    _port->setText("7060");

    _turnServer->setText("117.78.36.159");
    _turnServerPort->setText("13478");

}
void MainWindow::initTop() {
    QVBoxLayout *topLayout = new QVBoxLayout();
    topLayout->setMargin(10);

    QHBoxLayout *tfLayout = new QHBoxLayout();
    tfLayout->addWidget(new QLabel("server:"));
    _server = new QLineEdit("ip or domain");
    tfLayout->addWidget(_server);
    tfLayout->addWidget(new QLabel("port"));
    _port = new QLineEdit("");
    tfLayout->addWidget(_port);

    _protocol = new QButtonGroup();
    QRadioButton *tcp = new QRadioButton("tcp");
    QRadioButton *udp = new QRadioButton("udp");
    tcp->setChecked(true);
    _protocol->addButton(tcp, 10086);
    _protocol->addButton(udp, 10087);
    tfLayout->addWidget(tcp);
    tfLayout->addWidget(udp);
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
    tf2->addWidget(new QLabel("room id:"));
    _roomID = new QLineEdit("");
    tf2->addWidget(_roomID);
    topLayout->addLayout(tf2);

    QHBoxLayout *tsLayout = new QHBoxLayout();

    QPushButton *initSip = new QPushButton("init");
    connect(initSip, &QPushButton::clicked, this, &MainWindow::onInit);
    tsLayout->addWidget(initSip);

    QPushButton *destorySip = new QPushButton("destory");
    connect(destorySip, &QPushButton::clicked, this, &MainWindow::onDestory);
    tsLayout->addWidget(destorySip);

    QPushButton *showCodec = new QPushButton("codec");
    connect(showCodec, &QPushButton::clicked, this, &MainWindow::onCodec);
    tsLayout->addWidget(showCodec);

    _previewBtn = new QPushButton("preview local video");
    _previewBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(_previewBtn, &QPushButton::clicked, this, &MainWindow::onPreview);
    tsLayout->addWidget(_previewBtn);

    tsLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));


    QPushButton *addCount = new QPushButton("register account");
    connect(addCount, &QPushButton::clicked, this, &MainWindow::onRegister);
    tsLayout->addWidget(addCount);

    QPushButton *make = new QPushButton("make call");
    connect(make, &QPushButton::clicked, this, &MainWindow::onCall);
    tsLayout->addWidget(make);

    QPushButton *huangup = new QPushButton("hang up call");
    connect(huangup, &QPushButton::clicked, this, &MainWindow::onHangUp);
    tsLayout->addWidget(huangup);


    QPushButton *version = new QPushButton("sip version");
    connect(version, &QPushButton::clicked, this, &MainWindow::onVersion);
    tsLayout->addWidget(version);

    topLayout->addLayout(tsLayout);

    ((QBoxLayout*)centralWidget()->layout())->addLayout(topLayout);
}
void MainWindow::initMid() {
    QHBoxLayout *midLayout = new QHBoxLayout();
    _previewVideoWidget = new QWidget();
    _previewVideoWidget->setMinimumSize(QSize(300, 300));
    _previewVideoWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    _previewVideoWidget->setStyleSheet("border:1px solid lightGray;");
    _previewVideoWidget->setLayout(new QHBoxLayout());
    midLayout->addWidget(_previewVideoWidget);

    _sipVideoWidget = new QWidget();
    _sipVideoWidget->setMinimumSize(QSize(300, 300));
    _sipVideoWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    _sipVideoWidget->setStyleSheet("border:1px solid lightGray;");
    _sipVideoWidget->setLayout(new QHBoxLayout());
    midLayout->addWidget(_sipVideoWidget);

    ((QBoxLayout*)centralWidget()->layout())->addLayout(midLayout);
}
void MainWindow::initBottom() {
    QTextEdit *log = new QTextEdit();
    log->setMaximumHeight(150);
    log->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    connect(this, &MainWindow::postLog, log, &QTextEdit::append);

    ((QBoxLayout*)centralWidget()->layout())->addWidget(log);
}

MainWindow::~MainWindow()
{

}

void MainWindow::log(QString msg) {
    emit postLog(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh::mm::ss zzz")).arg(msg));
}

void MainWindow::onInit() {
    bool isTcp = (_protocol->checkedId() == 10086);
    PjsipManager::shareInstance()->init(isTcp);
}
void MainWindow::onDestory() {
    PjsipManager::shareInstance()->deinit();
}
void MainWindow::onCodec() {
    log(PjsipManager::shareInstance()->codecInfoString());
}
void MainWindow::onPreview() {
    if (PjsipManager::shareInstance()->stopPreviewVideo()) {
        _previewBtn->setText("preview local video");
    } else {
        QWidget * preview = PjsipManager::shareInstance()->startPreviewVideo();
        if (preview) {
            _previewBtn->setText("stop preview");
            _previewVideoWidget->layout()->addWidget(preview);
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

    QString roomid = _roomID->text();

    PjsipManager::shareInstance()->makeCall(sipserver, sipport, roomid, _sipVideoWidget);

}
void MainWindow::onHangUp() {
    PjsipManager::shareInstance()->hangupAllCall();


}