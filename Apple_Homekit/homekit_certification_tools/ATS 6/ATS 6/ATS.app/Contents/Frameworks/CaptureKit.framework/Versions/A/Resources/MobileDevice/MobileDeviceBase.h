/*
 * vim:ts=4:sts=4:expandtab:
 *
 * MobileDeviceBase.h
 * MobileDevice
 *
 * Created by Justin Ko on 4/3/11.
 * Copyright 2011 Apple Inc. All rights reserved.
 */

#ifndef __MOBILEDEVICEBASE_H__
#define __MOBILEDEVICEBASE_H__

#if defined(__WIN32__)
#define AMD_API __cdecl
#define AMD_NOINLINE __declspec(noinline)
#else
#define AMD_API
#define AMD_NOINLINE __attribute__((noinline))
#endif

typedef int AMDError;
typedef struct _AMDevice *AMDeviceRef;
typedef struct _AMDServiceConnection *AMDServiceConnectionRef;
typedef struct _AMDeviceNotificationContext *AMDeviceNotificationRef;
typedef struct _AMDevicePreflightOperation *AMDevicePreflightOperationRef;

#endif /* __MOBILEDEVICEBASE_H__ */
