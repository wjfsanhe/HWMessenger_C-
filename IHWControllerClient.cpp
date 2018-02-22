//
// Created by wangjf on 2/8/18.
//
#define LOG_TAG "HWMessenger"
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>

#include <binder/Parcel.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <cutils/log.h>

#include "IHWControllerClient.h"

namespace android {

class BpHWControllerClient : public BpInterface<IHWControllerClient> {
public:
    BpHWControllerClient(const sp<IBinder>& impl) :
            BpInterface<IHWControllerClient>(impl) { }

    virtual String16 readSysfs(const String16 &path) {
        Parcel data, reply;
        data.writeInterfaceToken(IHWControllerClient::getInterfaceDescriptor());
        data.writeString16(path);
        remote()->transact(BnHWControllerClient::READ_SYSFS, data, &reply);
        return reply.readString16();
    }
    virtual String16 writeSysfs(const String16 &path, const String16 &setValue) {
        Parcel data, reply;
        data.writeInterfaceToken(IHWControllerClient::getInterfaceDescriptor());
        data.writeString16(path);
        data.writeString16(setValue);
        remote()->transact(BnHWControllerClient::WRITE_SYSFS, data, &reply);
        return reply.readString16();
    }
};

IMPLEMENT_META_INTERFACE(HWControllerClient, "com.qiyi.hwmessenger.IHWControllerClient");

//according to AIDL implementation.
status_t BnHWControllerClient::onTransact(
        uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) {
    switch(code) {
    case WRITE_SYSFS: {
        CHECK_INTERFACE(IHWControllerClient, data, reply);
        String16 path = data.readString16();
        String16 value = data.readString16();
        reply->writeNoException();
        reply->writeString16(writeSysfs(path, value));
        break;
    }
    case READ_SYSFS: {
        CHECK_INTERFACE(IHWControllerClient, data, reply);
        String16 path = data.readString16();
        reply->writeNoException();
        reply->writeString16(readSysfs(path));
        break;
    }
    default:
        return BBinder::onTransact(code, data, reply, flags);
    }
    return NO_ERROR;
}
static inline const char* getChars(const String16 str) {
    return String8(str).string();
}
String16 BnHWControllerClient::readSysfs(const String16 &path) {
    //todo readSysfs.
    ALOGI("readSys  be called [%s]",getChars(path));
    int fd = open(getChars(path), O_RDONLY);
    if(fd < 0) {
        ALOGI("readSys error: fd <0");
        return String16("NO_OP");
    }

    char buffer[1024];
    int len = read(fd, buffer,sizeof(buffer) - 1);
    buffer[len] = 0;
    ALOGI("readSys return : %s ", buffer);
    close(fd);
    return String16(buffer);

}
String16 BnHWControllerClient::writeSysfs(const String16 &path, const String16 &setValue)
{
    //todo writeSysfs.
    ALOGI("writeSys  be called [%s -- %s]",getChars(path),getChars(setValue));
    int fd = open(getChars(path), O_RDWR);
    if(fd < 0) {
        ALOGI("writeSys error: fd <0");
        return String16("NO_OP");
    }

    const char* strVal = getChars(setValue);
    size_t len = write(fd, strVal, strlen(strVal));
    if (len != strlen(strVal)) {
        ALOGI("writeSys error: io write imcomplete.");
        return String16("NO_OP");
    }
    return readSysfs(path);
}

};

