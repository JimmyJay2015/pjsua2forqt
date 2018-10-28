//
//  pjsuacall.h
//  pjsip demo
//
//  Created by Jimmy on 10/26/18.
//
//
#ifndef PJSUACALL_H
#define PJSUACALL_H

#include <pjsua2.hpp>

#include <QtCore>
#include <QWidget>


class PjsuaCall : public QObject , public pj::Call {
    Q_OBJECT
public:
    PjsuaCall(pj::Account &acc, QWidget *videoParent, int call_id = PJSUA_INVALID_ID);
    ~PjsuaCall();

    virtual void onCallState(pj::OnCallStateParam &prm);
    virtual void onCallMediaState(pj::OnCallMediaStateParam &prm);
    virtual void onInstantMessage(pj::OnInstantMessageParam &prm);

    void setVideoParent(QWidget *videoParent);

signals:
    void calling();
    void callEnd();
    void callReject();
    void callFailed();
    void callTimeout();
    void callCancel();
    void callBusy();
    void stopRing();
    void callAccept();

    void videoWindowReady(void *hwnd);

public slots:
    void onVideoWindowReady(void *hwnd);
    void onCallEnd();

private:
    void _releaseVideoWidget();

private:
    QWidget *_videoParent;

    QWidget *_videoWidget;
};

#endif