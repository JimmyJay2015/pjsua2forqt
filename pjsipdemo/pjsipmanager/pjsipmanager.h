//
//  pjsipmanager.h
//  pjsip demo
//
//  Created by Jimmy on 10/26/18.
//
//
#ifndef PJSIPMANAGER_H
#define PJSIPMANAGER_H

#include <pjsua2.hpp>

#include <QObject>
#include <QtCore>


#define LOG(x) (emit PjsipManager::shareInstance()->log(x));


class PjsipEndpoint : public pj::Endpoint {

};


class PjsuaCall;

class PjsipManager : public QObject  {
    Q_OBJECT
public:
    static PjsipManager *shareInstance();
    ~PjsipManager();

    void init(bool isTcp);
    void deinit();

private:
	PjsipManager();

signals:
    void log(QString);

public: // api
    QString pjLibVersion();
    QString codecInfoString();
    QString deviceInfoString();

    QWidget *startPreviewVideo();
    bool stopPreviewVideo();

    bool createMyAccount(QString uid, QString name, QString sipserver, QString turnserver, qint32 turnport);
    
    bool makeCall(QString server, qint32 serverport, QString roomid, QWidget *videoParent);
    bool hangupAllCall();


public slots:
    void onPreviewDestoryed(QObject *obj);
    
private:
    bool _inited;
    bool _isTcp;

    pj::TransportId _tid;
    pj::Account *_account;

    QWidget *_previewVideo;

    PjsuaCall *_mycall;
};

#endif