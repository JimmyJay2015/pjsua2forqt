//
//  pjsuacall.cpp
//  pjsip demo
//
//  Created by Jimmy on 10/26/18.
//
//
#include "pjsuacall.h"

#include <pjsua2.hpp>
#include <pjsua.h>

#include "pjsipmanager.h"
#include "pjvidwidget.h"

#include <QTimer.h>
#include <QLayout.h>


PjsuaCall::PjsuaCall(pj::Account &acc, QWidget *videoParent, int call_id)
    : pj::Call(acc, call_id)
    , _videoParent(videoParent)
    , _videoWidget(nullptr)
{
    // videoWindowReady
    connect(this, &PjsuaCall::videoWindowReady, this, &PjsuaCall::onVideoWindowReady);

    connect(this, &PjsuaCall::callEnd, this, &PjsuaCall::onCallEnd);
}

PjsuaCall::~PjsuaCall() {
}

void PjsuaCall::setVideoParent(QWidget *videoParent) {
    _videoParent = videoParent;
}

void PjsuaCall::_releaseVideoWidget() {
    if (_videoWidget) {
        _videoWidget->setParent(nullptr);
        _videoWidget->deleteLater();
        _videoWidget = 0;
    }
}
void PjsuaCall::onCallEnd() {
    _releaseVideoWidget();
}
void PjsuaCall::onVideoWindowReady(void *hwnd) {
    _releaseVideoWidget();

    PjvidWidget *widget = new PjvidWidget(hwnd);
    _videoWidget = widget;
    widget->init();
    if (_videoParent) {
        _videoParent->layout()->addWidget(_videoWidget);
        //widget->setParent(_videoParent);
        //widget->move(0, 0);
        //widget->resize(_videoParent->size());
    }
    widget->show();
}


void PjsuaCall::onCallState(pj::OnCallStateParam &prm) {
    pj::CallInfo ci = getInfo();

    LOG(QString("call state change. state:%1(%2), last state:%3, role:%4").arg(ci.state).arg(QString::fromStdString(ci.stateText)).arg(ci.lastStatusCode).arg(ci.role));

    if (ci.state == PJSIP_INV_STATE_CALLING) {
        emit calling();
    } else if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
        if (ci.lastStatusCode == PJSIP_SC_OK) {
            emit callEnd();
        } else if (ci.lastStatusCode == PJSIP_SC_DECLINE) {
            emit callReject();
        } else if (ci.lastStatusCode == PJSIP_SC_SERVICE_UNAVAILABLE || ci.lastStatusCode == PJSIP_SC_SERVER_TIMEOUT) {
            emit callFailed();
        } else if (ci.lastStatusCode == PJSIP_SC_REQUEST_TIMEOUT) {
            emit callTimeout();
        } else if (ci.lastStatusCode == PJSIP_SC_REQUEST_TERMINATED) {
            emit callCancel();
        } else if (ci.lastStatusCode == PJSIP_SC_BUSY_HERE) {
            emit callBusy();
        }

        LOG(QString("call dump: \n%1").arg(QString::fromStdString(dump(true, ">>>"))));
    } else if (ci.state == PJSIP_INV_STATE_CONFIRMED) {
        emit callAccept();
    }

}

void PjsuaCall::onCallMediaState(pj::OnCallMediaStateParam &prm) {
    LOG("call media state change");
    pj::CallInfo ci = getInfo();

    for (unsigned i = 0; i < ci.media.size(); i++) {
        LOG(QString("call media %1, type %2").arg(i).arg(ci.media[i].type));

        if (ci.media[i].type == PJMEDIA_TYPE_AUDIO && getMedia(i)) {
            // 连接音频
            pj::AudioMedia *aud_med = (pj::AudioMedia *)getMedia(i);
            pj::AudDevManager& mgr = pj::Endpoint::instance().audDevManager();

            aud_med->startTransmit(mgr.getPlaybackDevMedia());
            mgr.getCaptureDevMedia().startTransmit(*aud_med);
        } else if (ci.media[i].type == PJMEDIA_TYPE_VIDEO && (ci.media[i].dir & PJMEDIA_DIR_DECODING)) {
            pjsua_vid_win_info wi;
            pjsua_vid_win_get_info(ci.media[i].videoIncomingWindowId, &wi);

            LOG(("found vid window, start video..."));
            emit videoWindowReady(wi.hwnd.info.win.hwnd);
        }
    }

}

void PjsuaCall::onInstantMessage(pj::OnInstantMessageParam &prm) {

    LOG(QString("call receive message :%1").arg(QString::fromStdString(prm.msgBody)));

}







