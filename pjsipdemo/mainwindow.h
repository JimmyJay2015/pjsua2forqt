#ifndef TEST4_H
#define TEST4_H

#include <QtWidgets/QMainWindow>

#include <QtCore>
#include <QtWidgets>


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
    void initTop();
    void initMid();
    void initBottom();

signals:
    void postLog(QString log);

public slots:
    void log(QString msg);

    void onVersion();

    void onInit();
    void onDestory();
    void onPreview();
    void onRegister();
    void onCall();
    void onHangUp();

private:
    QLineEdit *_server;
    QLineEdit *_port;
    QLineEdit *_protocol;

    QLineEdit *_turnServer;
    QLineEdit *_turnServerPort;

    QLineEdit *_accountID;
    QLineEdit *_accountName;

    QWidget *_videoWidget;
};

#endif // TEST4_H
