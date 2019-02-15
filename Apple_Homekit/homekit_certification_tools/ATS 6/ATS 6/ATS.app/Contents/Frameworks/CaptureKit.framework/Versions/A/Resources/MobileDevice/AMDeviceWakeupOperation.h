#ifndef __AMDEVICEWAKEUPOPERATION_H__
#define __AMDEVICEWAKEUPOPERATION_H__
/*
 *  vim:ts=4:sts=4:expandtab:
 *
 *  AMDeviceWakeupOperation.h
 *  MobileDevice
 *
 *  Created by Justin Ko on 4/3/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 */

/*!
 * @header AMDeviceWakeupOperation.h
 *
 * Declares interfaces for waking iOS devices. Defines the AMDeviceWakeupOperation class.
 * AMDeviceWakeupOperation instances are CF objects.
 */
#include <CoreFoundation/CoreFoundation.h>
#include <MobileDevice/MobileDevice.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @function AMDeviceCreateWakeupToken
 * @abstract Create an opaque CF object used to identify a sleeping device.
 * @param device The device to create a wakeup token for
 * @param options See discussion for supported options
 * @param token_out On successful return, a new CFPropertyList object that can be used to refer to this device while it is asleep.
 * @result kAMDSuccess, or a non-zero value on failure.
 *
 * @discussion
 * Creates a wakeup token for the specified device. The token created is suitable
 * for use with AMDeviceWakeupDeviceWithToken().
 *
 * No options are currently supported.
 *
 * Ownership of the returned token follows the 'Create Rule'.
 */
AMDError
AMD_API
AMDeviceCreateWakeupToken(
    AMDeviceRef device,
    CFDictionaryRef options,
    CFPropertyListRef *token_out);

/*!
 * @function AMDeviceWakeupUsingToken
 * @abstract Attempt to wake up a device using a wake token.
 * @param token The wake token to use
 * @param timeout See discussion for interpretation of timeout value.
 * @param options See discussion for supported options. May be NULL.
 * @result kAMDSuccess or an error.
 *
 * @discussion
 * This is a simplified interface for waking a device.
 *
 * If timeout is 0, starts a background wake attempt and returns immediately. The
 * result indicates only whether or not the wake attempt was successfully started.
 *
 * If timeout is -1, this function will block for up to 10 seconds. If timeout
 * is > 0, will block for up to 'timeout' interval or 10 seconds, whichever is shorter.
 * In both these cases, result is kAMDTimeOutError if the operation times out without
 * waking the device, kAMDNotFoundError if an asynchronous error occurs, or kAMDSuccess
 * if the device appears on the network (i.e. the wake attempt succeeded). These values
 * are based on the possible AMDeviceWakeupOperationResult values. If the device
 * is already awake, this should return kAMDSuccess quickly.
 *
 * No options are currently supported.
 */
AMDError
AMD_API
AMDeviceWakeupUsingToken(
    CFPropertyListRef token,
    CFTimeInterval timeout,
    CFDictionaryRef options);

/*!
 * @typedef AMDeviceWakeupOperationRef
 * @discussion A pointer to an AMDeviceWakeupOperation object.
 */
typedef struct _AMDeviceWakeupOperation *AMDeviceWakeupOperationRef;

/*!
 * @function AMDeviceWakeupOperationGetTypeID
 * @abstract Get the CF type ID for the AMDeviceWakeupOperation class
 * @result The CF type ID of the AMDeviceWakeupOperation class
 * @discussion Useful for runtime type checking.
 */
CFTypeID
AMD_API
AMDeviceWakeupOperationGetTypeID(
    void);

/*
 * Result of the wakeup operation.
 *
 * RESULT_DEVICE_AWAKE: Device is awake and advertising its presence on the network.
 * RESULT_OPERATION_EXPIRED: Wakeup operation expired.
 * RESULT_ASYNC_ERROR: Some asynchronous error occurred.
 */
typedef enum _AMDeviceWakeupOperationResult {
    RESULT_DEVICE_AWAKE,
    RESULT_OPERATION_EXPIRED,
    RESULT_ASYNC_ERROR,
} AMDeviceWakeupOperationResult;

/*
 * @typedef AMDeviceWakeupOperationCallback
 * @param wakeop The wake operation that is delivering status
 * @param wakeResult The result of the wake operation
 * @param context The context pointer passed to AMDeviceWakeupOperationCreateWithToken().
 * @result Currently ignored.
 *
 * @discussion
 * This is the type of the callback that is provided to AMDeviceWakeupOperationCreateWithToken().
 * The wakeup operation may be invalidated and released from within the callback. Only one callback
 * will ever be delivered for each wake operation.
 */
typedef int (AMD_API *AMDeviceWakeupOperationCallback)(AMDeviceWakeupOperationRef wakeop, AMDeviceWakeupOperationResult wakeResult, void *context);

/*!
 * @function AMDeviceWakeupOperationCreateWithToken
 * @abstract
 * @param wakeCallback A callback to deliver notification
 * @param callbackContext A context pointer passed to the callback
 * @param token A wakeup token created with AMDeviceCreateWakeupToken
 * @param options See discussion for supported options. May be NULL.
 * @result An initialized AMDeviceWakeupOperation object, or NULL on failure.
 *
 * @discussion
 * A wake operation is used to attempt to wake a sleeping iOS device asynchronously.
 *
 * A wake operation will send wake packets at the device for up for 10 seconds, after which
 * it will give up. If the device is on the network and responsive, it will wake up and advertise
 * its presence on the network within a few (2 ~ 3) seconds of receiving a wake packet. Delivery
 * of wake packets isn't guaranteed.
 *
 * If the device is awoken and it begins advertising its presence on the network, the callback
 * will be called with a wakeResult of RESULT_DEVICE_AWAKE. In this case, expect a callback
 * from AMDeviceNotificationSubscribe() shortly.
 *
 * If the wake operation runs until completion, but does not detect the device on the network,
 * the callback will be called with a wakeResult of RESULT_OPERATION_EXPIRED. Expiration does
 * not necessarily mean that the device will not eventually awake. The device may just responding
 * to the wake request slowly.
 *
 * If an asynchronous error is encountered while running the wake operation, wakeResult will be
 * RESULT_ASYNC_ERROR.
 *
 * The callback will only be called once. Once the callback is called, the wakeup operation should be
 * invalidated with AMDeviceWakeupOperationInvalidate() and released.
 *
 * The operation may be cancelled by calling AMDeviceWakeupOperationInvalidate().
 *
 * Ownership follows the 'Create rule'.
 */
AMDeviceWakeupOperationRef
AMD_API
AMDeviceWakeupOperationCreateWithToken(
    AMDeviceWakeupOperationCallback wakeCallback,
    void * callbackContext,
    CFPropertyListRef token,
    CFDictionaryRef options);

/*!
 * @function AMDeviceWakeupOperationSchedule
 * @param wakeop The operation to schedule
 * @param runloop The runloop to run on
 * @param mode The mode to run in
 * @result On success returns TRUE, otherwise returns FALSE.
 *
 * @discussion
 * Begins processing an AMDeviceWakeupOperation. Use AMDeviceWakeupOperationInvalidate() to cancel the operation.
 *
 * AMDeviceWakeupOperationSchedule() takes a retain on the wakeop. You are free to release your reference to
 * the wakeop object after scheduling it.
 */
bool
AMD_API
AMDeviceWakeupOperationSchedule(
    AMDeviceWakeupOperationRef wakeop,
    CFRunLoopRef runloop,
    CFStringRef mode);

/*!
 * @function AMDeviceWakeupOperationInvalidate
 * @abstract Halts any work being done by the wake operation
 * @param wakeop The wake operation to invalidate
 *
 * @discussion
 * Halts any work being done by the wake operation and removes all runloop sources that
 * have been added to the scheduled runloop. After invalidating, the only thing
 * left to do is to release the wake operation object.
 *
 * This is safe to call many times.
 */
void
AMD_API
AMDeviceWakeupOperationInvalidate(
    AMDeviceWakeupOperationRef wakeop);

#if defined(__cplusplus)
}
#endif

#endif /* __AMDEVICEWAKEUPOPERATION_H__ */
