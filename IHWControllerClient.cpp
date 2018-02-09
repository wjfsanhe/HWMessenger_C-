//
// Created by wangjf on 2/8/18.
//

#include <stdint.h>
#include <sys/types.h>

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


status_t BnHWControllerClient::onTransact(
        uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) {
    switch(code) {
    case WRITE_SYSFS: {
        CHECK_INTERFACE(IHWControllerClient, data, reply);
        String16 path = data.readString16();
        String16 value = data.readString16();
        reply->writeString16(writeSysfs(path, value));
        break;
    }
    case READ_SYSFS: {
        CHECK_INTERFACE(IHWControllerClient, data, reply);
        String16 path = data.readString16();
        reply->writeString16(readSysfs(path));
        break;
    }
    default:
        return BBinder::onTransact(code, data, reply, flags);
    }
    return NO_ERROR;
}
String16 BnHWControllerClient::readSysfs(const String16 &path) {
    //todo readSysfs.
    return String16("NO_OP");

}
String16 BnHWControllerClient::writeSysfs(const String16 &path, const String16 &setValue)
{
    //todo writeSysfs.
    return String16("NO_OP");
}

};

