#ifndef __AMDEVICEPREFLIGHTOPERATION_H__
#define __AMDEVICEPREFLIGHTOPERATION_H__
/*
 *  vim:ts=4:sts=4:expandtab:
 *
 *  AMDevicePreflightOperation.h
 *  MobileDevice
 *
 *  Created by Justin Ko on 7/20/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 */

/*!
 * @header AMDevicePreflightOperation.h
 *
 * Declares interfaces for the mux to preflight iOS devices before presenting them as muxed devices.
 * Defines the AMDevicePreflightOperation class. AMDevicePreflightOperation instances are CF objects.
 */
#include <CoreFoundation/CoreFoundation.h>
#include <MobileDevice/MobileDevice.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @function AMDevicePreflightOperationGetTypeID
 * @abstract Get the CF type ID for the AMDevicePreflightOperation class
 * @result The CF type ID of the AMDevicePreflightOperation class
 * @discussion Useful for runtime type checking.
 */
CFTypeID
AMD_API
AMDevicePreflightOperationGetTypeID(
    void);

/*
 * @typedef AMDevicePreflightOperationCallback
 * @param pairingResult kAMDSuccess on success, or an error on failure.
 * @param serviceConnection A service connection to the heartbeat service. The object is create with kAMDOptionCloseOnInvalidate enabled.
 * @param context The pointer that was passed to AMDevicePreflightOperationCreate().
 *
 * @discussion
 * Clients provide a function of this type to know when the preflight operation has completed.
 */
typedef void (AMD_API *AMDevicePreflightOperationCallback)(AMDError pairingResult, AMDServiceConnectionRef serviceConnection, void *context);

/*!
 * @function AMDevicePreflightOperationCreate
 * @abstract On behalf of the mux, perform any necessary preflighting.
 * @param preflightCallback A callback to deliver notification
 * @param callbackContext A context pointer passed to the callback
 * @param deviceProperties A dictionary of properties for the device.
 * @param options See discussion for supported options. May be NULL.
 * @result An initialized AMDeviceWakeupOperation object, or NULL on failure.
 *
 * @discussion
 * A preflight operation is used to preflight an iOS device before the mux presents it.
 * On success, the callback will be called once. Once the callback is called, the
 * preflight operation should be invalidated with AMDevicePreflightOperationInvalidate()
 * and released.
 *
 * The operation may be cancelled at any time by calling AMDeviceWakeupOperationInvalidate().
 *
 * Ownership follows the 'Create rule'.
 */
AMDevicePreflightOperationRef
AMD_API
AMDevicePreflightOperationCreate(
    AMDevicePreflightOperationCallback preflightCallback,
    void * callbackContext,
    CFDictionaryRef deviceProperties,
    CFDictionaryRef options);

/*!
 * @function AMDevicePreflightOperationGetRunLoopSource
 * @abstract Get a runloopsource for this object.
 * @param pre The operation to get a runloop source for.
 * @result On success, a CFRunLoopSource; on failure, NULL.
 *
 * @discussion
 * Add the runloop source to a runloop to begin doing work. The callback is called on the runloop
 * that this source is added to.
 *
 * Ownership follows the Get rule.
 */
CFRunLoopSourceRef
AMD_API
AMDevicePreflightOperationGetRunLoopSource(
    AMDevicePreflightOperationRef pre);

/*!
 * @function AMDevicePreflightOperationInvalidate
 * @abstract Halts any work being done by the preflight operation operation
 * @param pre The preflight operation to invalidate.
 *
 * @discussion
 * Halts any work being done by the preflight operation and prevents the callback from firing in the future.
 */
void
AMD_API
AMDevicePreflightOperationInvalidate(
    AMDevicePreflightOperationRef pre);

#if defined(__cplusplus)
}
#endif

#endif /* __AMDEVICEPREFLIGHTOPERATION_H__ */
