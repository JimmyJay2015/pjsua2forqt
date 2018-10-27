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

#include <QObject>
#include <QtCore>


class PjsuaCall : public pj::Call {
public:
    PjsuaCall(pj::Account &acc, int call_id = PJSUA_INVALID_ID);
    ~PjsuaCall();


    virtual void onCallState(pj::OnCallStateParam &prm);
    virtual void onCallMediaState(pj::OnCallMediaStateParam &prm);
    virtual void onInstantMessage(pj::OnInstantMessageParam &prm);

};

#endif