//
// Created by wangjf on 2/8/18.
//

#ifndef HWMESSENGER_IHWCONTROLLERCLIENT_H
#define HWMESSENGER_IHWCONTROLLERCLIENT_H

#include <stdint.h>
#include <sys/types.h>
#include <binder/IInterface.h>

namespace android {

/*
 * This class defines the Binder IPC interface for accessing various
 * InputFlinger features.
 */
class IHWControllerClient : public IInterface {
public:
    DECLARE_META_INTERFACE(HWControllerClient);
};


/**
 * Binder implementation.
 */
class BnHWControllerClient : public BnInterface<IHWControllerClient>
{
public:
    enum {
        WRITE_SYSFS = IBinder::FIRST_CALL_TRANSACTION,
        READ_SYSFS = IBinder::FIRST_CALL_TRANSACTION + 1,
    };

    virtual status_t onTransact(uint32_t code, const Parcel& data,
            Parcel* reply, uint32_t flags = 0);

protected:
    //we support multi client.
    virtual String16 readSysfs(const String16 &path);
    virtual String16 writeSysfs(const String16 &path, const String16 &setValue);

};

} // namespace android

#endif //HWMESSENGER_IHWCONTROLLERCLIENT_H
