#include "mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>

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
    tfLayout->addWidget(new QLineEdit("ip or domain"));
    tfLayout->addWidget(new QLabel("port"));
    tfLayout->addWidget(new QLineEdit(""));
    tfLayout->addWidget(new QLabel("sc"));
    tfLayout->addWidget(new QLineEdit("tcp or udp"));
    topLayout->addLayout(tfLayout);

    QHBoxLayout *tsLayout = new QHBoxLayout();

    QPushButton *initSip = new QPushButton("init");
    connect(initSip, &QPushButton::clicked, this, &MainWindow::onInit);
    tsLayout->addWidget(initSip);

    QPushButton *destorySip = new QPushButton("destory");
    connect(destorySip, &QPushButton::clicked, this, &MainWindow::onDestory);
    tsLayout->addWidget(destorySip);

    QPushButton *make = new QPushButton("make call");
    connect(make, &QPushButton::clicked, this, &MainWindow::onCall);
    tsLayout->addWidget(make, 0, Qt::AlignRight);

    QPushButton *huangup = new QPushButton("hang up call");
    connect(huangup, &QPushButton::clicked, this, &MainWindow::onHangUp);
    tsLayout->addWidget(huangup, 0, Qt::AlignRight);

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
}

MainWindow::~MainWindow()
{

}

void MainWindow::log(QString msg) {
    emit postLog(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh::mm::ss zzz")).arg(msg));
}

void MainWindow::onInit() {
    qDebug() << "onInit";
    log("onInit");
}
void MainWindow::onDestory() {
    qDebug() << "onDestory";
    log("onDestory");
}
void MainWindow::onCall() {
    qDebug() << "onCall";
    log("onCall");
}
void MainWindow::onHangUp() {
    qDebug() << "onHangUp";
    log("onHangUp");
}