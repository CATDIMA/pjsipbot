#include <iostream>
#include <memory>
#include "inherited_classes.h"
#include "auxiliary_functions.h"

MyCall::MyCall(pj::Account& acc, int callId) : pj::Call(acc, callId)
{ }

void MyCall::onCallState(pj::OnCallStateParam& param)
{
    pj::CallInfo ci = getInfo();

    std::cout << "CALL: " << ci.remoteUri << " " << ci.stateText << std::endl;

    if(ci.state == PJSIP_INV_STATE_DISCONNECTED)
    {
        delete this;
    }
}

void MyCall::onCallMediaState(pj::OnCallMediaStateParam& param)
{
    pj::CallInfo ci = getInfo();

    for(size_t i = 0; i < ci.media.size(); i++)
    {
        if(ci.media.at(i).type == PJMEDIA_TYPE_AUDIO)   
        {
            try
            {
                pj::AudioMedia audioMedia = getAudioMedia(i);
                pj::AudDevManager& manager = pj::Endpoint::instance().audDevManager();

                audioMedia.startTransmit(manager.getPlaybackDevMedia());
                manager.getCaptureDevMedia().startTransmit(audioMedia);
            }
            catch(pj::Error& e)
            {
                std::cerr << e.info() << '\n';
            }
            
        }
    }
}

void MyAccount::onRegState(pj::OnRegStateParam& param)
{
    pj::AccountInfo accInfo = getInfo();
    if(accInfo.regIsActive)
        std::cout << "Registered\n";
    else
        std::cout << "Unregistered\n";

    std::cout << "Code: " << param.code << std::endl;
}

void MyAccount::onIncomingCall(pj::OnIncomingCallParam& param)
{
    std::unique_ptr<pj::Call> call(new MyCall(*this, param.callId));

    /*pick up*/
    pj::CallOpParam op;
    op.statusCode = PJSIP_SC_OK;
    call->answer(op);

    /*start recording*/
    std::unique_ptr<pj::AudioMediaRecorder> recorder(new pj::AudioMediaRecorder());
    pj::AudioMedia& mic_media = pj::Endpoint::instance().audDevManager().getCaptureDevMedia();
    try
    {
        /*get a random name for a file*/
        recorder->createRecorder(getUniqueFilename() + ".wav");
        mic_media.startTransmit(*recorder);
    }
    catch(pj::Error& e)
    {
        std::cout << e.info() << std::endl;
    }

    /*play greeting*/
    pj::AudioMediaPlayer greetingsPlayer;
    pj::AudioMedia& speaker_media = pj::Endpoint::instance().audDevManager().getPlaybackDevMedia();
    try
    {
        greetingsPlayer.createPlayer("res/greetings.wav", PJMEDIA_FILE_NO_LOOP);
        greetingsPlayer.startTransmit(speaker_media);
    }
    catch(pj::Error& e)
    {
        std::cout << e.info() << std::endl;
    }

    /*wait for 5 secs*/
    pj_thread_sleep(5000);

    /*play epilogue*/
    pj::AudioMediaPlayer epiloguePlayer;
    try
    {
        epiloguePlayer.createPlayer("res/epilogue.wav", PJMEDIA_FILE_NO_LOOP);
        epiloguePlayer.startTransmit(speaker_media);
    }
    catch(pj::Error& e)
    {
        std::cout << e.info() << std::endl;
    }
    
    /*hang up*/
    call->hangup(op);
}