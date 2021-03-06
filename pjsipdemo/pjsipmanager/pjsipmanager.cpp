﻿//
//  pjsipmanager.cpp
//  pjsip demo
//
//  Created by Jimmy on 10/26/18.
//
//
#include "pjsipmanager.h"

#include "pjsuaaccount.h"
#include "pjsuacall.h"
#include "pjvidwidget.h"

#include <mutex>


PjsipManager *PjsipManager::shareInstance() {
    static PjsipManager *instance = nullptr;
    static std::once_flag onceTag;
    std::call_once(onceTag, []() {
        if (!instance) {
            instance = new PjsipManager();
        }
    });
    return instance;
}

PjsipManager::PjsipManager() : QObject(nullptr)

, _inited(false)

, _isTcp(false)

, _account(nullptr)

, _previewVideo(nullptr)

, _mycall(nullptr)

{
	
}

PjsipManager::~PjsipManager() {
    deinit();
}

void PjsipManager::init(bool isTcp) {
    if (_inited) {
        emit log(QString("pjlib already inited"));
        return;
    }
    PjsipEndpoint *ep = new PjsipEndpoint();
    try {
        ep->libCreate();
    } catch (pj::Error &error) {
        emit log(QString("pjlib create failed: %1").arg(QString::fromStdString(error.info())));
        return;
    }

    pj::EpConfig ecfg;
    ecfg.logConfig.level = 5;
    ecfg.uaConfig.maxCalls = 1;
    try {
        ep->libInit(ecfg);
    } catch (pj::Error &error) {
        emit log(QString("pjlib init failed: %1").arg(QString::fromStdString(error.info())));
        ep->libDestroy();
        return;
    }

    try {
        pj::TransportConfig tcfg;
        if (isTcp) {
            _tid = ep->transportCreate(PJSIP_TRANSPORT_TCP, tcfg);
        } else {
            _tid = ep->transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
        }
    } catch (pj::Error &error) {
        emit log(QString("pjlib create transport failed: %1").arg(QString::fromStdString(error.info())));
        ep->libDestroy();
        return;
    }

    try {
        ep->libStart();
    } catch (pj::Error &error) {
        emit log(QString("pjlib start failed: %1").arg(QString::fromStdString(error.info())));
        ep->libDestroy();
        return;
    }

    emit log("pjlib started!");

    _inited = true;
    _isTcp = isTcp;
}

void PjsipManager::deinit() {
    if (!_inited) {
        emit log("pjlib not init yet!");
        return;
    }

    _isTcp = false;

    hangupAllCall();

    if (_account) {
        delete _account;
        _account = nullptr;
    }

    stopPreviewVideo();

    pj::Endpoint::instance().libDestroy();
    delete &(pj::Endpoint::instance());

    _inited = false;
    emit log("pjlib destoryed!");
}

QString PjsipManager::pjLibVersion() {
    pj::Version pv = pj::Endpoint::instance().libVersion();
    return QString::fromStdString(pv.full);
}

QString PjsipManager::codecInfoString() {
    QString result;
    result.append("********** video codec info ************\n");

    pjsua_codec_info videoCodecs[100] = {0};
    unsigned videoCount = 100;
    pj_status_t ret = pjsua_vid_enum_codecs(videoCodecs, &videoCount);
    if (ret != PJ_SUCCESS) {
        result.append(QString("enum video codecs failed: %1\n").arg(ret));
    } else {
        result.append(QString("enum %1 video codecs\n").arg(videoCount));

        for (unsigned i = 0; i < videoCount; i++) {
            pjsua_codec_info ci = videoCodecs[i];
            std::string idString(ci.codec_id.ptr, ci.codec_id.slen);
            result.append(QString(">>>id: %1\n").arg(QString::fromStdString(idString)));
            result.append(QString("priority: %1\n").arg(ci.priority));
            std::string decString(ci.desc.ptr, ci.desc.slen);
            result.append(QString("description: %1\n").arg(QString::fromStdString(decString)));
            result.append(">>>>>>>>>>>>>\n");
        }
    }

    result.append("********** audio codec info ************\n");

    pjsua_codec_info audioCodecs[100] = { 0 };
    unsigned audioCount = 100;
    ret = pjsua_enum_codecs(audioCodecs, &audioCount);
    if (ret != PJ_SUCCESS) {
        result.append(QString("enum audio codecs failed: %1\n").arg(ret));
    } else {
        result.append(QString("enum %1 audio codecs\n").arg(audioCount));

        for (unsigned i = 0; i < audioCount; i++) {
            pjsua_codec_info ci = audioCodecs[i];
            std::string idString(ci.codec_id.ptr, ci.codec_id.slen);
            result.append(QString(">>>id: %1\n").arg(QString::fromStdString(idString)));
            result.append(QString("priority: %1\n").arg(ci.priority));
            std::string decString(ci.desc.ptr, ci.desc.slen);
            result.append(QString("description: %1\n").arg(QString::fromStdString(decString)));
            result.append(">>>>>>>>>>>>>\n");
        }
    }

    result.append("*********************************\n");
    return result;
}

QString PjsipManager::deviceInfoString() {

    pjmedia_vid_dev_refresh();

    QString result;
    result.append("********** preview info ************\n");


    pjmedia_vid_dev_info viddevs[100] = {0};
    unsigned devcount = 100;

    pj_status_t ret = pjsua_vid_enum_devs(viddevs, &devcount);

    if (ret != PJ_SUCCESS) {
        result.append(QString("enum video device failed:%1\n").arg(ret));
    } else {
        result.append(QString("there %1 video device\n").arg(devcount));

        for (unsigned i = 0; i < devcount; i++)
        {
            pjmedia_vid_dev_info info = viddevs[i];
            
            result.append(QString(">>>id: %1\n").arg(info.id));
            
            std::string nameString(info.driver);
            result.append(QString("name: %1\n").arg(QString::fromStdString(nameString)));

            result.append(QString("caps: %1\n").arg(info.caps));

            result.append(QString("fmt_cnt: %1\n").arg(info.fmt_cnt));
            
            result.append(">>>>>>>>>>>>>\n");
        }
    }


    result.append("*********************************\n");
    return result;
}

bool PjsipManager::stopPreviewVideo() {
    if (_previewVideo) {
        _previewVideo->setParent(nullptr);
        _previewVideo->deleteLater();
        _previewVideo = nullptr;

        pjsua_vid_preview_stop(PJMEDIA_VID_DEFAULT_CAPTURE_DEV);
        return true;
    }
    return false;
}

QWidget *PjsipManager::startPreviewVideo() {
    if (!_inited) {
        emit log("pjlib not init!");
        return nullptr;
    }


    stopPreviewVideo();

    pjsua_vid_win_id wid;
    pjsua_vid_win_info winfo;
    pj_status_t status;

    // 创建预览界面，预览默认的摄像头
    status = pjsua_vid_preview_start(PJMEDIA_VID_DEFAULT_CAPTURE_DEV, NULL);
    if (status != PJ_SUCCESS) {
        LOG(QString("preview local video failed! %1").arg(status));
        return nullptr;
    }

    // 获取默认摄像头的 预览 winid
    wid = pjsua_vid_preview_get_win(PJMEDIA_VID_DEFAULT_CAPTURE_DEV);
    // 获取 winid 的 win info
    pjsua_vid_win_get_info(wid, &winfo);

    // win info 中取出 hwnd，设为 QWidget 的子窗口
#if defined(Q_OS_WIN)
    PjvidWidget *previewVideo = new PjvidWidget(winfo.hwnd.info.win.hwnd);
#elif defined(Q_OS_MAC)
    PjvidWidget *previewVideo = new PjvidWidget(winfo.hwnd.info.cocoa.window);
#elif defined(Q_OS_LINUX)
    PjvidWidget *previewVideo = new PjvidWidget(winfo.hwnd.info.x11.window);
#endif
    previewVideo->hide();
    previewVideo->init();
    _previewVideo = previewVideo;

    connect(_previewVideo, &QWidget::destroyed, this, &PjsipManager::onPreviewDestoryed);

    return _previewVideo;
}
void PjsipManager::onPreviewDestoryed(QObject *obj) {
    if (_previewVideo == obj) {
        _previewVideo = 0;
    }
}

/////////////////////////
bool PjsipManager::createMyAccount(QString uid, QString name, QString sipserver, QString turnserver, qint32 turnport) {
    if (!_inited) {
        emit log("pjlib not init!");
        return false;
    }

    pj::TransportInfo ti = pj::Endpoint::instance().transportGetInfo(_tid);
    QString localAddress = QString::fromStdString(pj::Endpoint::instance().transportGetInfo(_tid).localName);
    QString acountID = QString("%1<sip:%2@%3>").arg(name).arg(uid).arg(localAddress);

    pj::AccountConfig acfg;
    acfg.idUri = acountID.toStdString();
    acfg.videoConfig.defaultCaptureDevice = PJMEDIA_VID_DEFAULT_CAPTURE_DEV;
    acfg.videoConfig.defaultRenderDevice = PJMEDIA_VID_DEFAULT_RENDER_DEV;
    acfg.videoConfig.autoShowIncoming = PJ_TRUE;
    acfg.videoConfig.autoTransmitOutgoing = PJ_TRUE;

    if (!sipserver.isEmpty()) {
        acfg.regConfig.registrarUri = QString("sip:%1").arg(sipserver).toStdString();
    }

    // 启用 turn server
    if (!turnserver.isEmpty() && turnport > 0) {
        acfg.natConfig.iceEnabled = true;
        acfg.natConfig.turnEnabled = true;
        acfg.natConfig.turnServer = QString("%1:%2").arg(turnserver).arg(turnport).toStdString();
        acfg.natConfig.turnConnType = PJ_TURN_TP_UDP;
    }

    _account = new PjsuaAccount();
    try {
        _account->create(acfg, true);
    } catch (pj::Error &error) {
        emit log(QString("create account failed: %1").arg(QString::fromStdString(error.info())));
        return false;
    }

    emit log(QString("Account registed:%1").arg(acountID));
    return true;
}

bool PjsipManager::makeCall(QString server, qint32 serverport, QString roomid, QWidget *videoParent) {
    if (!_inited) {
        emit log("pjlib not init!");
        return false;
    }

    if (_mycall) {
        emit log("you are in call!");
        return false;
    }

    if (roomid.isEmpty()) {
        roomid.setNum(rand());
    }
    QString remoteid = QString("sip:%1@%2:%3%4").arg(roomid).arg(server).arg(serverport).arg(_isTcp ? ";transport=tcp" : "");
        emit log(QString("call: %1").arg(remoteid));

    _mycall = new PjsuaCall(*_account, videoParent);

    pj::CallOpParam prm(true);
    prm.opt.videoCount = 1;

    try {
        _mycall->makeCall(remoteid.toStdString(), prm);
    } catch (pj::Error &error) {
        emit log(QString("make call failed: %1").arg(QString::fromStdString(error.info())));
        return false;
    }


    return true;
}

bool PjsipManager::hangupAllCall() {
    if (!_inited) {
        emit log("pjlib not init!");
        return false;
    }

    if (_mycall) {
        pj::CallOpParam op;
        op.statusCode = PJSIP_SC_DECLINE;
        try {
            _mycall->hangup(op);
        } catch (pj::Error &error) {
            emit log(QString("hung up call failed: %1").arg(QString::fromStdString(error.info())));
            return false;
        }
        _mycall = nullptr;
    }
    return true;
}











