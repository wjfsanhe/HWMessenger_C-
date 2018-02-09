//
// Created by wangjf on 2/7/18.
//

#include "HWControllerThread.h"


namespace android {

HWControllerThread::HWControllerThread(sp<HWMessenger> hwMessenger)
{
}

HWControllerThread::~HWControllerThread()
{
}
void HWControllerThread::onMessageReceived(const sp<AMessage> &msg)
{
     //toDo received command


     switch (msg->what()) {
        case kWhatReadSysfs:
        {
            AString filePath;
            sp<AReplyToken> replyID;
            CHECK(msg->senderAwaitsResponse(&replyID));
            CHECK(msg->findString("path", &filePath));
            //todo read sysfs .retValue = read(filePath);


            /*no block, no response.
            AString retValue;
            sp<AMessage> response = new AMessage();
            int32_t err = 0;
            response->setInt32("err",err);
            response->setString("value",retValue);
            response->postReply(replyID);*/
            break;
        }
        case kWhatWriteSysfs:
        {
            AString filePath;
            AString setValue;
            sp<AReplyToken> replyID;
            CHECK(msg->senderAwaitsResponse(&replyID));
            CHECK(msg->findString("path", &filePath));
            CHECK(msg->findString("setValue", &setValue));
            //todo read sysfs .retValue = write(filePath,value);


            /*no block ,no response.
            AString retValue;
            sp<AMessage> response = new AMessage();
            int32_t err = 0;
            response->setInt32("err",err);
            response->setString("retValue",retValue);
            response->postReply(replyID);*/
            break;
        }
        default:
            TRESPASS();
     }
}
status_t HWControllerThread::start()
{
    mLooper = new ALooper();
    mLooper->setName("HWControllerThread");
    mLooper->registerHandler(this);
    mLooper->start(false,false,ANDROID_PRIORITY_AUDIO);
    return NO_ERROR;
}
status_t HWControllerThread::stop()
{
    if (!mLooper.get()){
        return DEAD_OBJECT;
    }
    mLooper->unregisterHandler(id());
    mLooper->stop();
    mLooper.clear();
    return NO_ERROR;
}
status_t HWControllerThread::readSysfs(const AString &path, AString *value)
{
    status_t err = NO_ERROR;
    sp<AMessage> response;
    sp<AMessage> msg = new AMessage(kWhatReadSysfs, this);
    msg->setString("path", path);
    msg->post();
    /*status_t err = msg->postAndAwaitResponse(&response);
    if (err == OK && response != NULL) {
        CHECK(response->findInt32("err", &err));
        CHECK(response->findString("value", value));
    }*/
    return err;
}
status_t HWControllerThread::writeSysfs(const AString &path, const AString &setValue, AString *retValue)
{
    status_t err = NO_ERROR;
    sp<AMessage> response;
    sp<AMessage> msg = new AMessage(kWhatWriteSysfs, this);
    msg->setString("path", path);
    msg->setString("setValue", setValue);
    msg->post();
    /*status_t err = msg->postAndAwaitResponse(&response);
    if (err == OK && response != NULL) {
        CHECK(response->findInt32("err", &err));
        CHECK(response->findString("retValue", retValue));
    }*/
    return err;
}
}
