#ifndef _PJSIPBOT_CLASSES_H
#define _PJSIPBOT_CLASSES_H

#include <pjsua2.hpp>

class MyCall : public pj::Call
{
public:
    MyCall(pj::Account& acc, int callId = PJSUA_INVALID_ID);

    virtual void onCallState(pj::OnCallStateParam& param); /*отработка сброса*/
    virtual void onCallMediaState(pj::OnCallMediaStateParam& param); /*подключение к аппаратуре*/
};

class MyAccount : public pj::Account
{
public:
    virtual void onRegState(pj::OnRegStateParam& param);
    virtual void onIncomingCall(pj::OnIncomingCallParam& param); /*поднять трубку и обработать*/
};

#endif