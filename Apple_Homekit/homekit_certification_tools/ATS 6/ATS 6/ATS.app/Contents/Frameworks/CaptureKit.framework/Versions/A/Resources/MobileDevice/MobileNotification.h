#ifndef __MOBILENOTIFICATION__
#define __MOBILENOTIFICATION__

/*!
 * @header MobileNotification.h
 *
 * Access to device-side Darwin notification services. 
 */


/* Header Declarations */
#include <MobileDevice/MobileDevice.h>
#include <MobileDevice/AMDServiceConnection.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @defined kAMDNotificationProxyService
 * @abstract Name of the Notification Proxy Service. Use this with AMDeviceStartService().
 */
#define kAMDNotificationProxyService CFSTR("com.apple.mobile.notification_proxy")

/*!
 * @defined kAMDNotificationFacePlant
 * @abstract Notification failure.
 * @discussion If the connection to the device is severed and does a face plant,
 * this notification will be returned to the callback specified
 * in AMDListenForNotifications(). Will not be delivered on clean shutdown.
 */
#define kAMDNotificationFacePlant	CFSTR("AMDNotificationFaceplant")

/*!
 * @typedef AMDNotificationCallback
 * @abstract Function pointer type for listening to notifications from the device
 * @param name Name of the received notification 
 * @param context User provided context
 * @discussion A function of this type registered with AMDListenForNotifications() will 
 * be called when notifications are received form the device.
 *
 * Ownership of the notification string *name* DOES NOT pass to the callback function.
 * Don't free it.
 */
typedef void (AMD_API *AMDNotificationCallback) (
   CFStringRef name,
   void *context);

/*!
 * @typedef AMDNotificationContext
 * @abstract Structure for context with retain and release callbacks
 * @param info A C pointer to a user-specified block of data.
 *
 * The info pointer is passed to the callback function.
 */
typedef struct {
    CFIndex	version;
    void *info;
    const void *(*retain)(const void *info);
    void (*release)(const void *info);
    CFStringRef	(*copyDescription)(const void *info);
} AMDNotificationContext;

/*!
 * @function AMDPostNotification
 * @abstract Inject a notification into the device.
 * @param sock A socket returned with AMDeviceStartService(kAMDNotificationProxyService, ...)
 * @param name Name of the notification to post
 * @param userInfo Arbitrary extra information that will be delivered to the observers. 
 * @result kAMDSuccess or else an AMDError describing the failure. 
 * @discussion Currently Darwin notification centers are used, so userInfo is ignored.
 */
AMDError
AMD_API
AMDPostNotification(
	const AMDNativeSocket sock,
	CFStringRef name,
	CFDictionaryRef userInfo);
	
AMDError
AMD_API
AMDSecurePostNotification(
	AMDServiceConnectionRef servConn,
	CFStringRef name,
	CFDictionaryRef userInfo);

/*!
 * @function AMDObserveNotification
 * @abstract Begin observing notifications from the device.
 * @param sock A socket returned with AMDeviceStartService(kAMDNotificationProxyService, ...)
 * @param name The name of the notification which you would like to be notified about.
 * @result kAMDSuccess or else an AMDError describing the failure. 
 * @discussion Call multiple times to register for more than one notification. If a listening 
 * callback is not first installed with AMDListenForNotifications() notifications may
 * be lost.
 *
 * kAMDNotificationFacePlant may be received by the listening callback if the 
 * notification service shuts down, or the connection is severed.
 */
AMDError
AMD_API
AMDObserveNotification(
	const AMDNativeSocket sock,
	CFStringRef name);

AMDError
AMD_API
AMDSecureObserveNotification(
	AMDServiceConnectionRef servConn,
	CFStringRef name);


/*
 * Use AMDSecureListenForNotificationsWithRetainedContext instead, it is the preferred way to listen for notifications.
 * Returns kAMDInvalidArgumentError if sock is -1 or callback is NULL.
 */
AMDError
AMD_API
AMDListenForNotifications(
	const AMDNativeSocket sock,
	AMDNotificationCallback callback,
	void *context);

/* Use AMDSecureListenForNotificationsWithRetainedContext instead, it is the preferred way to listen for notifications. */
AMDError
AMD_API
AMDSecureListenForNotifications(
	AMDServiceConnectionRef servConn,
	AMDNotificationCallback callback,
	void *context);

/*!
 * @function AMDSecureListenForNotificationsWithRetainedContext
 * @abstract Registers a callback function as a listener for notifications.
 * @param servConn A connection to the device side notification service (kAMDNotificationProxyService)
 * @param callback A callback function
 * @param context A context object, may be NULL.
 * @result kAMDSuccess or an AMDError describing the failure
 *
 * @discussion
 * Installs a notification callback. Once thew callback is installed, call AMDSecureObserveNotification() any
 * number of times to indicate the notifications that you are interested in. The information in 'context'
 * parameter is copied out, the storage for the structure does not need to stay valid after
 * AMDSecureListenForNotificationsWithRetainedContext() returns. If the retain and release function pointers
 * in and the info pointer in the context object are all non-NULL, the context's info pointer will be retained
 * when AMDSecureListenForNotificationsWithRetainedContext() is called, and released when AMDSecureShutdownNotificationProxy()
 * is called.
 *
 * This is the preferred way to listen for notifications.
 *
 * If servConn or callback is NULL, returns kAMDInvalidArgumentError.
 */
AMDError
AMD_API
AMDSecureListenForNotificationsWithRetainedContext(
	AMDServiceConnectionRef servConn,
	AMDNotificationCallback callback,
	AMDNotificationContext *context);

/*!
 * @function AMDShutdownNotificationProxy
 * @abstract Shutdown the notification service
 * @param sock A socket returned with AMDeviceStartService(kAMDNotificationProxyService, ...)
 * @result kAMDSuccess or else an AMDError describing the failure.
 * @discussion This shuts down the device side notification proxy and cleans up resources
 * allocated in AMDListenForNotifications(). On success, the device side service will have
 * shut down and the socket can safely be closed. On clean shutdown kAMDNotificationFacePlant
 * will not be received.
 */
AMDError
AMD_API
AMDShutdownNotificationProxy(
	const AMDNativeSocket sock);

AMDError
AMD_API
AMDSecureShutdownNotificationProxy(
	AMDServiceConnectionRef servConn);
	
#if defined(__cplusplus)
}
#endif

#endif /* __MOBILENOTIFICATION__ */

