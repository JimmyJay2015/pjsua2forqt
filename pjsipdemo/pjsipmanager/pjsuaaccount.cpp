//
//  pjsuaaccount.cpp
//  pjsip demo
//
//  Created by Jimmy on 10/26/18.
//
//
#include "pjsuaaccount.h"

#include "pjsipmanager.h"


PjsuaAccount::PjsuaAccount() {
}
PjsuaAccount::~PjsuaAccount() {
    LOG("release account");
}


void PjsuaAccount::onRegState(pj::OnRegStateParam &prm) {
    pj::AccountInfo ai = getInfo();
    if (ai.regIsActive) {
        LOG((QString("account reg succeed:%1->%2(%3)").arg(QString::fromStdString(ai.uri)).arg(prm.code).arg(QString::fromStdString(ai.regStatusText))));
    } else {
        LOG((QString("account reg failed:%1->%2(%3)").arg(QString::fromStdString(ai.uri)).arg(prm.code).arg(QString::fromStdString(ai.regStatusText))));
    }
}

void PjsuaAccount::onIncomingCall(pj::OnIncomingCallParam &prm) {
    pj::AccountInfo ai = getInfo();
    LOG((QString("in coming call:%2->%1").arg(QString::fromStdString(ai.uri)).arg(prm.callId)));

    // option
    //Call *call = new MyCall(*this, iprm.callId);
    //CallOpParam op;
    //op.statusCode = PJSIP_SC_DECLINE;
    //call->hungup(op);
    //delete Call;
}












