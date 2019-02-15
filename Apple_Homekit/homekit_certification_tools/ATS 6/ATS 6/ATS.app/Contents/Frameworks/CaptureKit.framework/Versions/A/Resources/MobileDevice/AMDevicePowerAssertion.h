#ifndef __AMDEVICEPOWERASSERTION_H__
#define __AMDEVICEPOWERASSERTION_H__
/*
 * vim:sts=4:sw=4:expandtab:
 *
 *  AMDevicePowerAssertion.h
 *  MobileDevice
 *
 *  Created by Justin Ko on 12/5/11.
 *  Copyright (c) 2011 Apple Inc. All rights reserved.
 */

/*!
 * @header AMDevicePowerAssertion.h
 *
 * Declares interfaces for remotely taking power assertions on an iOS. Defines the AMDevicePowerAssertion class.
 * AMDevicePowerAssertion instances are CF objects.
 */

#include <CoreFoundation/CoreFoundation.h>
#include <MobileDevice/MobileDevice.h>

/* 20 minutes. */
#define AMDPOWERASSERTION_TIMEOUT_MAX ((CFTimeInterval)60 * 20)

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @typedef AMDevicePowerAssertionRef
 * @discussion A pointer to an AMDevicePowerAssertion object.
 */
typedef struct _AMDevicePowerAssertion *AMDevicePowerAssertionRef;

/*!
 * @function AMDevicePowerAssertionGetTypeID
 * @abstract Get the CF type ID for the AMDevicePowerAssertion class
 * @result The CF type ID of the AMDevicePowerAssertion class
 * @discussion Useful for runtime type checking.
 */
CFTypeID
AMD_API
AMDevicePowerAssertionGetTypeID(
    void);

/*!
 * @define kAMDAssertionTypeWirelessSync
 * @abstract Tells the device to stay awake for the purposes of wireless sync.
 * @discussion Tells the device to stay available for wireless sync.
 */
#define kAMDAssertionTypeWirelessSync   CFSTR("AMDPowerAssertionTypeWirelessSync")

/*!
 * @function AMDevicePowerAssertionCreate
 * @abstract Create a remote power assertion against an iOS device.
 * @param targetDevice The device to create a power assertion with (required).
 * @param assertionType The assertion type. Currently only kAMDAssertionTypeWirelessSync (required).
 * @param name A descriptive name for the assertion. This corresponds to the kIOPMAssertionNameKey property (required).
 * @param details The details. Optional extra data beyond the Name. This corresponds to the kIOPMAssertionDetailsKey property.
 * @param timeout Must be greater than zero and less than or equal to AMDPOWERASSERTION_TIMEOUT_MAX.
 * @param error_out If not NULL, on failure this is populated with an AMDError value describing the failure.
 * @result A newly created power assertion object, or NULL on failure.
 *
 * @discussion
 * An AMDevicePowerAssertion object may be used to prevent an iOS device from entering low power states.
 * An AMDevicePowerAssertion requires connectivity to the device. If the device becomes detached, the
 * assertion will be dropped on the device. This call is modeled on the IOPMAssertionCreateWithDescription()
 * call defined in IOPMLib.h, but is more restrictive.
 *
 * The name and details parameters are used for debugging and logging purposes. They should be descriptive
 * strings for humans.
 *
 * When the AMDevicePowerAssertion object is finalized, the power assertion taken on the device is immediately
 * released, even if the timeout period has not yet elapsed. When the timeout period elapses the device
 * will automatically drop the power assertion.
 *
 * To keep a device awake for time periods longer than AMDPOWERASSERTION_TIMEOUT_MAX, take out multiple, successive power assertions.
 */
AMDevicePowerAssertionRef
AMD_API
AMDevicePowerAssertionCreate(
    AMDeviceRef targetDevice,
    CFStringRef assertionType,
    CFStringRef name,
    CFStringRef details,
    CFTimeInterval timeout,
    AMDError *error_out);

#if defined(__cplusplus)
}
#endif

#endif /* __AMDEVICEPOWERASSERTION_H__ */
