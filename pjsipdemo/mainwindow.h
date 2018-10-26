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

public:
    void log(QString msg);

signals:
    void postLog(QString log);

public slots:
    void onInit();
    void onDestory();
    void onCall();
    void onHangUp();

private:
    QWidget *_videoWidget;
};

#endif // TEST4_H
