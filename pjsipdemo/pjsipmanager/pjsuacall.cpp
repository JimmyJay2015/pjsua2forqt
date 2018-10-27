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


PjsuaCall::PjsuaCall(pj::Account &acc, int call_id) 
    : pj::Call(acc, call_id)
{
}

PjsuaCall::~PjsuaCall() {
}

void PjsuaCall::onCallState(pj::OnCallStateParam &prm) {
    pj::CallInfo ci = getInfo();

    LOG(QString("call state change. state:%1(%2), last state:%3, role:%4").arg(ci.state).arg(QString::fromStdString(ci.stateText)).arg(ci.lastStatusCode).arg(ci.role));



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



        }
    }

}

void PjsuaCall::onInstantMessage(pj::OnInstantMessageParam &prm) {

    LOG(QString("call receive message :%1").arg(QString::fromStdString(prm.msgBody)));

}







