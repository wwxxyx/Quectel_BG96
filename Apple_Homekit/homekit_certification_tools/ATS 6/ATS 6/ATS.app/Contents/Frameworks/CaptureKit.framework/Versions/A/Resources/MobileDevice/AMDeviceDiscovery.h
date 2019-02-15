#ifndef __AMDEVICEDISCOVERY_H__
#define __AMDEVICEDISCOVERY_H__
/*
 *  vim:ts=4:sts=4:expandtab:
 *
 *  AMDeviceDiscovery.h
 *  MobileDevice
 *
 *  Created by Justin Ko on 12/1/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 */

/*!
 * @header AMDeviceDiscovery.h
 *
 * Declares interfaces discovering iOS devices and getting notifications as they come and go.
 */
#include <CoreFoundation/CoreFoundation.h>
#include <MobileDevice/MobileDeviceBase.h>

#define kAMDNotificationOptionSearchForPairedDevices  CFSTR("NotificationOptionSearchForPairedDevices")

/*
 * The interface connection type.  Pass ONE and ONLY ONE of these to AMDeviceNotificationSubscribe(WithOptions).  Not a bitfield (unfortunately).
 */
typedef enum {
    kAMDDeviceInterfaceConnectionTypeInvalid = -1,
    kAMDeviceInterfaceConnectionTypeAny = 0x00,
    kAMDeviceInterfaceConnectionTypeDirect = 0x01,  // e.g. USB, Firewire, Bluetooth pairing
    kAMDeviceInterfaceConnectionTypeIndirect = 0x02, // e.g. Ethernet, 802.11g or other network
    
    //This can be returned from AMDeviceGetInterfaceType, but should not be passed to AMDeviceNotificationSubscribe(WithOptions).
    //AMDeviceNotificationSubscribe only wants to know if we are subscribing for USB devices, WiFi devices, or both (Any).
    kAMDeviceInterfaceConnectionTypeProxied = 0x03, // e.g. Connection to this device is proxied through a paired companion device
} AMDeviceInterfaceConnectionType;

/*
 * Various interface connection speeds
 * in kilobits per second.
 */
enum {
    kAMDeviceSpeedAny = 0,
    kAMDeviceSpeedUSBLowSpeed = (3 * 512),
    kAMDeviceSpeedUSBFullSpeed = (12 * 1024),
    kAMDeviceSpeedUSBHighSpeed = (480 * 1024),
    kAMDeviceSpeedFirewire400 = (400 * 1024),
    kAMDeviceSpeedFirewire800 = (800 * 1024),
    kAMDeviceSpeed10baseT = (10 * 1024),
    kAMDeviceSpeed100baseT = (100 * 1024),
    kAMDeviceSpeedGigabit = (1024 * 1024),
    kAMDeviceSpeed80211b = (11 * 1024),
    kAMDeviceSpeed80211g = (54 * 1024),
    kAMDeviceSpeed80211n = (540 * 1024),
    kAMDeviceSpeedBluetooth1 = (1 * 1024),
    kAMDeviceSpeedBluetooth2 = (21 * 1024)
};

/*
 * Device Action
 */
typedef enum {
    kAMDeviceAttached = 1,
    kAMDeviceDetached = 2,
    kAMDeviceNotificationStopped = 3,
    kAMDevicePaired = 4
} AMDeviceAction;


/*
 * Notification info.
 */
typedef struct _AMDeviceNotificationInfo {
    AMDeviceRef     Device;
    AMDeviceAction  Action;
    AMDeviceNotificationRef NotificationRef;
} AMDeviceNotificationInfo;

/*
 * @typedef AMDeviceNotificationCallback
 * @discussion
 * Notification callback. Ownership of the notification info struct *info* DOES NOT
 * pass to the callback function.
 *
 * The Action field of the 'info' parameter describes the notification being sent.
 *
 * - kAMDeviceAttached: A device has attached. The device reference belongs to the
 *   client. It must be explicitly released, or else it will leak.
 * - kAMDeviceDetached: A device has detached. The device object delivered will be
 *   the same as the one delivered in the kAMDeviceAttached notification. This
 *   device reference does not need to be released.
 * - kAMDeviceNotificationStopped: This notification is delivered in response to
 *   1. A call to AMDeviceNotificationUnsubscribe().
 *   2. An error occurred on one of the underlying notification systems
 *     (i.e. usbmuxd or mDNSResponder crashed or stopped responding).
 *     Unsubcribing and resubscribing may recover the notification system.
 */
typedef void (AMD_API *AMDeviceNotificationCallback)(AMDeviceNotificationInfo *info, void *context);

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @function AMDCreateDeviceList
 * @abstract Get a list of currently attached devices.
 * @result An array of AMDeviceRefs on success or NULL on failure.
 *
 * @discussion
 * Synchronously queries for and returns a list of the currently connected devices as
 * an array of AMDeviceRefs. Devices may be disconnected at any time at which will
 * cause the corresponding AMDeviceRef to become invalid. If no devices are attached,
 * returned array will have zero length. Ownership follows the Create Rule.
 *
 * AMDCopyArrayOfDevicesMatchingQuery() is similar.
 *
 * To deal with devices dynamically coming and going, use AMDeviceNotificationSubscribe() instead.
 */
CFArrayRef
AMD_API
AMDCreateDeviceList(
    void);

/*!
 * @function AMDeviceNotificationSubscribe
 * @abstract Subscribe for device notifications
 * @param callback A callback to which notifications are delivered
 * @param minimumInterfaceSpeed Minimum interface speed
 * @param connectionType The types of connections the caller is interested in:
 *              kAMDeviceInterfaceConnectionTypeAny, kAMDeviceInterfaceConnectionTypeDirect, or kAMDeviceInterfaceConnectionTypeIndirect
 * @param context Context for the user-provided callback.
 * @param refOut On success, a AMDeviceNotificationRef which is passed to AMDeviceNotificationSubscribe(). Must not be NULL.
 *
 * @discussion
 * Begins watching for MobileDevice notifications. For clients of MobileDevice.framework
 * and MobileDevice.dll, this function registers to do work on the current run loop and
 * calls the callback function on the current CFRunLoop.
 *
 * In iTunesMobileDevice.dll, this functions spawns a secondary work thread. The callback
 * function will be called on that newly created thread.
 *
 * If you don't need notifications and just want a list of devices, see AMDCreateDeviceList().
 *
 * Pass refOut to AMDeviceNotificationUnsubscribe() to free resources uses by this function.
 *
 * This is equivalent to calling AMDeviceNotificationSubscribeWithOptions with options set to NULL.
 */
AMDError
AMD_API
AMDeviceNotificationSubscribe(
    AMDeviceNotificationCallback callback,
    UInt32 minimumInterfaceSpeed,
    AMDeviceInterfaceConnectionType connectionType,
    void *context,
    AMDeviceNotificationRef *refOut);
    
/*!
 * @function AMDeviceNotificationSubscribeWithOptions
 * @abstract Subscribe for device notifications
 * @param callback A callback to which notifications are delivered
 * @param minimumInterfaceSpeed Minimum interface speed
 * @param connectionType The types of connections the caller is interested in:
 *              kAMDeviceInterfaceConnectionTypeAny, kAMDeviceInterfaceConnectionTypeDirect, or kAMDeviceInterfaceConnectionTypeIndirect
 * @param context Context for the user-provided callback.
 * @param refOut On success, a AMDeviceNotificationRef which is passed to AMDeviceNotificationSubscribe(). Must not be NULL.
 * @param options See discussion for more options
 *
 * @discussion
 * Begins watching for MobileDevice notifications. For clients of MobileDevice.framework
 * and MobileDevice.dll, this function registers to do work on the current run loop and
 * calls the callback function on the current CFRunLoop.
 *
 * In iTunesMobileDevice.dll, this functions spawns a secondary work thread. The callback
 * function will be called on that newly created thread.
 *
 * If you don't need notifications and just want a list of devices, see AMDCreateDeviceList().
 *
 * Pass refOut to AMDeviceNotificationUnsubscribe() to free resources uses by this function.
 *
 * Supported Options:
 * - kAMDNotificationOptionSearchForPairedDevices: A true CFBooleanRef value for this key means we will also present AMDevice refs
 *                                                 for devices paired to connected companion devices.
 *                                                 A false or absent key will not present any paired devices.
 */
AMDError
AMD_API
AMDeviceNotificationSubscribeWithOptions(
    AMDeviceNotificationCallback callback,
    UInt32 minimumInterfaceSpeed,
    AMDeviceInterfaceConnectionType connectionType,
    void *context,
    AMDeviceNotificationRef *refOut,
    CFDictionaryRef options);

/*!
 * @function AMDeviceNotificationUnsubscribe
 * @abstract Release the resources associated with an AMDeviceNotificationRef
 * @param ref A notification reference created by AMDeviceNotificationSubscribe().
 * @result kAMDSuccess on success, or else an AMDError describing the failure.
 * @discussion Frees the resources associated with a notification ref and stops
 * any further notifications from being delivered by it.
 *
 * A single kAMDeviceNotificationStopped will be delivered in response to this call.
 */
AMDError
AMD_API
AMDeviceNotificationUnsubscribe(
    AMDeviceNotificationRef ref);

/* Keys and values for use with AMDCopyArrayOfDevicesMatchingQuery(). */
#define kAMDMatchingModeKey                     CFSTR("MatchingMode")
#define kAMDMatchingModeMatchAnyValue           CFSTR("MatchAny")
#define kAMDMatchingModeMatchAllValue           CFSTR("MatchAll")
#define kAMDMatchingModeMatchWildcardValue      CFSTR("MatchWildcard")

#define kAMDMatchingCriteriaUDIDKey             CFSTR("MatchUDID")

#define kAMDMatchingCriteriaConnectionTypeKey   CFSTR("MatchConnectionType")
#define kAMDCriteriaUSBKey                      CFSTR("MatchConnectionTypeUSB")
#define kAMDCriteriaNetworkKey                  CFSTR("MatchConnectionTypeNetwork")
#define kAMDCriteriaPairedDeviceKey             CFSTR("MatchConnectionTypePairedDevice")

/*!
 * @function AMDCopyArrayOfDevicesMatchingQuery
 * @abstract Get a list of attached devices matching certain criteria.
 * @param note A notification reference created with AMDeviceNotificationSubscribe(). May be NULL.
 * @param query A set of options describing the devices of interest. See discussion for query options.
 * @param out_array A pointer to an array. On success, will contain a array.
 * @result kAMDSuccess on success, or else an AMDError describing the failure.
 *
 * @discussion
 * This functions returns a subset (, possibly equal to the set,) of all known, attached devices. It
 * evaluates each device using the criteria specified in the 'query' dictionary. All matching devices
 * are added to the 'out_array'.
 *
 * On success, 'out_array' is a new CFArray holding a variable number of AMDeviceRefs that the caller
 * is responsible for freeing. If no devices match the given criteria, the returned array will have
 * zero length.
 *
 * If note is not-NULL, the AMDeviceRefs contained in the array will be the exact same set that were
 * delivered via callbacks to AMDeviceNotificationCallback. Besides being inserted into the returned array, they
 * are not retained or released as part of the operation of this function. If a returned device is
 * to be safely used outside of the lifetime of the returned array, the device must be retained (and
 * released) by the caller.
 *
 * If note is NULL, the AMDeviceRefs contained in the array will be newly created.
 *
 * This call returns immediately and makes no guarantess about how long the returned
 * device refs will be valid for.
 *
 * Matching Modes:
 *  kAMDMatchingModeKey - This key determines how the matching works. (Required)
 *  kAMDMatchingModeMatchAnyValue - If a device matches ANY of the criteria it will be part of the returned array.
 *  kAMDMatchingModeMatchAllValue - Only if a device matches ALL of the criteria will it be part of the returned array.
 *  kAMDMatchingModeMatchWildcardValue - Ignore all criteria, just return all devices.
 *
 * Matching Criteria:
 *  kAMDMatchingCriteriaUDIDKey - Value is an array of CFStrings of device UDIDs, as returned
 *      by AMDeviceCopyDeviceIdentifier(). Case IN-sensitive.
 *
 *  kAMDMatchingCriteriaConnectionTypeKey - Value must be either kAMDCriteriaUSBKey or kAMDCriteriaNetworkKey.
 *
 * See AMDCreateDeviceList() for a way to synchronously query for all attached devices.
 */
AMDError
AMD_API
AMDCopyArrayOfDevicesMatchingQuery(
    AMDeviceNotificationRef note,
    CFDictionaryRef query,
    CFArrayRef *out_array);

#if defined(__cplusplus)
}
#endif

#endif /* __AMDEVICEDISCOVERY_H__ */
