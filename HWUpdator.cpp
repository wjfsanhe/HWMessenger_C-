//
// Created by wangjf on 2/8/18.
//
#define LOG_TAG "HWMessenger"
#include "HWMessenger.h"
#include "HWUpdator.h"



namespace android {

HWUpdator::HWUpdator(sp<HWMessenger> hwMessenger)
          : mHWMessenger(hwMessenger)
{
}

HWUpdator::~HWUpdator()
{
    if (!mLooper.get()){
        return ;
    }
    mLooper->unregisterHandler(id());
    mLooper->stop();
    mLooper.clear();
}
void HWUpdator::onFirstRef()
{
    mLooper = new ALooper();
    mLooper->setName("HWUpdator");
    mLooper->registerHandler(this);
    mLooper->start(false,false,ANDROID_PRIORITY_AUDIO);
    ALOGD("init HWUpdator OK");
}
void HWUpdator::update(AString deviceName, int32_t keyCode, int32_t keyValue, int32_t keyFlags)
{
    sp<AMessage> msg = new AMessage(kWhatUpdate, this);
    msg->setString("deviceName", deviceName);
    msg->setInt32("keyCode", keyCode);
    msg->setInt32("keyValue", keyValue);
    msg->setInt32("keyFlags", keyFlags);
    msg->post();
}
void HWUpdator::onMessageReceived(const sp<AMessage> &msg)
{

    switch (msg->what()) {
        case kWhatUpdate:
        {
            int32_t keyCode, keyValue, keyFlags;
            AString deviceName;
            CHECK(msg->findString("deviceName", &deviceName));
            CHECK(msg->findInt32("keyCode", &keyCode));
            CHECK(msg->findInt32("keyValue", &keyValue));
            CHECK(msg->findInt32("keyFlags", &keyFlags));
            mHWMessenger->updateKey(deviceName, keyCode, keyValue, keyFlags);
            break;
        }
        default:
            TRESPASS();
    }
}
}
