#include <pjsua2.hpp>
#include <SDL2/SDL.h>
#include <iostream>

using namespace pj;



class PjsuaCall : public Call {
public:
    PjsuaCall(Account &acc, int call_id = PJSUA_INVALID_ID)
    : Call(acc, call_id)
    {
    }


    virtual void onCallState(OnCallStateParam &prm) {
        std::cout << "call state change"<<std::endl;
    }
    virtual void onCallMediaState(OnCallMediaStateParam &prm) {
        std::cout << "call media state change"<<std::endl;
        CallInfo ci = getInfo();

        for (unsigned i = 0; i < ci.media.size(); i++) {
            std::cout << "call media "<< i << " , type " << ci.media[i].type <<std::endl;
            if (ci.media[i].type == PJMEDIA_TYPE_AUDIO && getMedia(i)) {
                // 连接音频
                pj::AudioMedia *aud_med = (pj::AudioMedia *)getMedia(i);
                pj::AudDevManager &mgr = pj::Endpoint::instance().audDevManager();

                aud_med->startTransmit(mgr.getPlaybackDevMedia());
                mgr.getCaptureDevMedia().startTransmit(*aud_med);
            }
            else if (ci.media[i].type == PJMEDIA_TYPE_VIDEO && (ci.media[i].dir & PJMEDIA_DIR_DECODING))
            {
                pjsua_vid_win_info wi;
                pjsua_vid_win_get_info(ci.media[i].videoIncomingWindowId, &wi);

                std::cout << "found vid window, start video...";
            }
        }
    }
    virtual void onInstantMessage(OnInstantMessageParam &prm) {
        std::cout << "call Instant Message"<<std::endl;

    }
};


// 预览视频
bool testVideoPreview(Endpoint &ep) {

	pj_status_t status = pjsua_vid_preview_start(PJMEDIA_VID_DEFAULT_CAPTURE_DEV, NULL);
    if (status != PJ_SUCCESS) {
        std::cout <<"preview local video failed! "<< ep.utilStrError(status)<<std::endl;
        return false;
    }

    return true;
}

// 测试视频通话
bool testMakeCall(Endpoint &ep, TransportId &tid) {
    // create account
    std::string localAddr = ep.transportGetInfo(tid).localName;
    std::string myAccoutID = "zzm<sip:u13681711541@" + localAddr +">";
    std::cout << "myAccoutID:" << myAccoutID;

    AccountConfig acfg;
    acfg.idUri = myAccoutID;
    acfg.videoConfig.defaultCaptureDevice = PJMEDIA_VID_DEFAULT_CAPTURE_DEV;
    acfg.videoConfig.defaultRenderDevice = PJMEDIA_VID_DEFAULT_RENDER_DEV;
    acfg.videoConfig.autoShowIncoming = PJ_TRUE;
    acfg.videoConfig.autoTransmitOutgoing = PJ_TRUE;

    acfg.natConfig.iceEnabled = true;
    acfg.natConfig.turnEnabled = true;
    acfg.natConfig.turnServer = "117.78.36.159:13478";
    acfg.natConfig.turnConnType = PJ_TURN_TP_UDP;

    Account myAcc;
    try {
        myAcc.create(acfg, true);
    } catch (pj::Error &error) {
        std::cout << "create account failed:" << error.info() << std::endl;
        return false;
    }
    /////////////////////

    // make call
    std::string remoteid = "sip:1231@117.78.36.159:7060;transport=tcp";
    std::cout << "remoteid:"<< remoteid << std::endl;


    PjsuaCall cl(myAcc);

    CallOpParam prm(true);
    prm.opt.videoCount = 1;

    try {
        cl.makeCall(remoteid, prm);
    } catch (pj::Error &error) {
        std::cout << "make call failed:" << error.info() << std::endl;
        return false;
    }

    return true;
}


bool testDevices(Endpoint &ep) {
    std::cout << "********* video device info ************" << std::endl;;

    pjmedia_vid_dev_info viddevs[100] = {0};
    unsigned devcount = 100;

    pj_status_t ret = pjsua_vid_enum_devs(viddevs, &devcount);
    if (ret != PJ_SUCCESS) {
        std::cout << "enum video device failed: " << ep.utilStrError(ret) << std::endl;
    } else {
        std::cout << "video device count: " << devcount << std::endl;

        for (unsigned i = 0; i < devcount; i++) {
            pjmedia_vid_dev_info info = viddevs[i];
            
            std::cout << "video index: " << i << std::endl;
            std::cout << "video id: " << info.id << std::endl;
            std::cout << "video name: " << info.driver << std::endl;
            std::cout << "video caps: " << info.caps << std::endl;
            std::cout << "video fmt_cnt: " << info.fmt_cnt << std::endl;
            std::cout << "----------------" << std::endl;
        }
    }

    std::cout << "********** video codec info ************" << std::endl;

    pjsua_codec_info videoCodecs[100] = {0};
    unsigned videoCount = 100;
    ret = pjsua_vid_enum_codecs(videoCodecs, &videoCount);
    if (ret != PJ_SUCCESS) {
        std::cout << "enum video codecs failed: " << ep.utilStrError(ret) << std::endl;
    } else {
        std::cout << "video codecs count: " << videoCount << std::endl;

        for (unsigned i = 0; i < videoCount; i++) {
            pjsua_codec_info ci = videoCodecs[i];
            std::string idString(ci.codec_id.ptr, ci.codec_id.slen);
            std::string decString(ci.desc.ptr, ci.desc.slen);

            std::cout << "video codec index: " << i << std::endl;
            std::cout << "video codec id: " << idString << std::endl;
            std::cout << "video codec description: " << decString << std::endl;
            std::cout << "----------------" << std::endl;
        }
    }

    std::cout << "********** audio device info ************" << std::endl;
    pjmedia_aud_dev_info audioDevices[100] = {0};
    unsigned audioDeviceCount = 100;
    ret = pjsua_enum_aud_devs(audioDevices, &audioDeviceCount);
    if (ret != PJ_SUCCESS) {
        std::cout << "enum audio device failed: " << ep.utilStrError(ret) << std::endl;
    } else {
        std::cout << "audio device count: " << audioDeviceCount << std::endl;

        for (unsigned i = 0; i < audioDeviceCount; i++) {
            pjmedia_aud_dev_info ai = audioDevices[i];

            std::string nameString(ai.name);
            std::string driverString(ai.driver);

            std::cout << "video device index: " << i << std::endl;
            std::cout << "video device name: " << nameString << std::endl;
            std::cout << "video device driver: " << driverString << std::endl;
            std::cout << "----------------" << std::endl;
        }
    }

    std::cout << "********** audio codec info ************" << std::endl;

    pjsua_codec_info audioCodecs[100] = { 0 };
    unsigned audioCount = 100;
    ret = pjsua_enum_codecs(audioCodecs, &audioCount);
    if (ret != PJ_SUCCESS) {
        std::cout << "enum audio codecs failed: " << ep.utilStrError(ret) << std::endl;
    } else {
        std::cout << "audio codecs count: " << audioCount << std::endl;

        for (unsigned i = 0; i < audioCount; i++) {
            pjsua_codec_info ci = audioCodecs[i];
            std::string idString(ci.codec_id.ptr, ci.codec_id.slen);
            std::string decString(ci.desc.ptr, ci.desc.slen);


            std::cout << "audio codec index: " << i << std::endl;
            std::cout << "audio codec id: " << idString << std::endl;
            std::cout << "audio codec description: " << decString << std::endl;
            std::cout << "----------------" << std::endl;
        }
    }

    std::cout << "*********************************" << std::endl;

    return true;
}


int main() {
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    Endpoint ep;

    ep.libCreate();

    // 初始化 Endpoint
    EpConfig ep_conf;
    ep.libInit(ep_conf);

    // 初始化 sip  Transport，简单处理 错误
    TransportId tid;
    TransportConfig tcfg;
    tcfg.port = 5060;
    try
    {
        // 使用 udp
        //ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
        tid = ep.transportCreate(PJSIP_TRANSPORT_TCP, tcfg);
    }
    catch (Error &error)
    {
        std::cout << error.info() << std::endl;
        return 1;
    }

    // 启动 pjsua lib 库( 轮询 pjsip 的工作线程等)
    ep.libStart();
    std::cout << "***  pjsua2 started ***" << std::endl;
    std::cout << "veision: " << pj::Endpoint::instance().libVersion().full << std::endl;

    bool ret = true;

    //// 预览测试 10 秒
    //ret = testVideoPreview(ep);
    //pj_thread_sleep(1000 * 10);
    ////////////////////////////////////////////////////////////////////////


    // 设备测试 
    ret = testDevices(ep);
    ////////////////////////////////////////////////////////////////////////



    //// 通话测试 30 分钟
    //ret = testMakeCall(ep, tid);
    //pj_thread_sleep(1000 * 60 * 10 * 3);
    //////////////////////////////////////////////////////////////////////////

    if (ret) {
        std::cout << "***  pjsua2 succeed ***" << std::endl;
    } else {
        std::cout << "***  pjsua2 failed ***" << std::endl;
    }        
    return 0;
}
