//
//  pjsuaaccount.h
//  pjsip demo
//
//  Created by Jimmy on 10/26/18.
//
//
#ifndef PJSUAACCOUNT_H
#define PJSUAACCOUNT_H

#include <pjsua2.hpp>

#include <QObject>
#include <QtCore>


class PjsuaAccount : public pj::Account {
public:
    PjsuaAccount();
    ~PjsuaAccount();


    virtual void onRegState(pj::OnRegStateParam &prm);
    virtual void onIncomingCall(pj::OnIncomingCallParam &prm);

private:


};



#endif