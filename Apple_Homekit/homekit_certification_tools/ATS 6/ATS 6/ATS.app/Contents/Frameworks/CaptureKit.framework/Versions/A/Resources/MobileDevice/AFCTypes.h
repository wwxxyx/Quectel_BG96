/* Copyright 2006-2009, Apple Inc.  All rights reserved. */

/*!
@header AFCTypes.h
 Types used by the AFC protocol structures and the AFC client library.
 These types are not used to define any protocol structures.
 */

#ifndef __AFC_TYPES_H__
#define __AFC_TYPES_H__

#include <stdint.h>
#include <stdbool.h>

#if defined(__GNUC__)

#ifndef INLINE
#define INLINE inline
#endif
#define AFC_EXPORT __attribute__((visibility("default"))) extern
#define AFC_FORMAT(args...) __attribute__((format(printf, ## args)))


#else /* __GNUC__ */

#ifndef INLINE
#define INLINE __inline
#endif
#if defined(BUILDING_MOBILEDEVICE)
#define AFC_EXPORT __declspec(dllexport) extern
#else
#define AFC_EXPORT __declspec(dllimport) extern
#endif /* BUILDING_MOBILEDEVICE */
#define AFC_FORMAT(...)

#endif /* __GNUC__ */

#include <CoreFoundation/CFBase.h>
#include <AvailabilityMacros.h>

/*!
 @typedef AFCError Error code returned from synchronous functions.  Values are identical to AFCStatus values.  Deprecated.
 */
typedef int AFCError /* DEPRECATED_ATTRIBUTE */;

/*!
 @typedef AFCDomainError Error code returned from asynchronous functions.
 */
typedef int AFCDomainError;

/*!
 @typedef AFCStatus Result returned by the Status packet.  Values are in AFCProtocol.h.
 */
typedef uint64_t AFCStatus;

/*!
 @typedef AFCPacketType
 */
typedef uint64_t AFCPacketType;

/*!
 @typedef AFCFileRef
 A reference to an open file on the server.
 Deprecated from use by clients of the AFC API.
 Use AFCFileDescriptorRef instead.
 */
typedef uint64_t AFCFileRef;
/*!
 @const kAFCInvalidFileRef An invalid file reference value that may be used to initialize variables and determine if they represent a real reference.
 */
#define kAFCInvalidFileRef 0

/*!
 @typedef AFCConnectionRef
 A reference to an open connection with an AFC server.
 An AFCConnection is a CoreFoundation object and may be used with CFRetain(), CFRelease(), etc.
 */
typedef struct __AFCConnection *AFCConnectionRef;
/*!
 @const kAFCInvalidConnectionRef An invalid connection reference value that may be used to initialize variables and determine if they represent a real  reference.
 */
#define kAFCInvalidConnectionRef 0

/*!
 @typedef AFCDirectoryRef
 A reference to directory information from the server returned by AFCDirectoryOpen().
 This type is deprecated.
 */
typedef struct __AFCDirectory *AFCDirectoryRef;
/*!
 @const kAFCInvalidDirectoryRef An invalid directory reference value that may be used to initialize variables and determine if they represent a real reference.
 */
#define kAFCInvalidDirectoryRef 0

/*!
 @typedef AFCKeyValueRef
 A reference to key/value pairs of information returned by multiple functions.
 This type is deprecated.
 */
typedef struct __AFCKeyValue *AFCKeyValueRef;
/*!
 @const kAFCInvalidKeyValueRef An invalid key/value reference value that may be used to initialize variables and determine if they represent a real file reference.
 */
#define kAFCInvalidKeyValueRef 0

/*!
 @typedef AFCLockRef A reference to an AFC lock object.  This is not a Core Foundation type.
 */
typedef struct __AFCLock *AFCLockRef;
/*!
 @const kAFCInvalidLockRef An invalid lock reference value that may be used to initialize variables and determine if they represent a real file reference.
 */
#define kAFCInvalidLockRef 0

/*!
 @typedef AFCOperationRef A reference to an AFC operation.
 */

typedef struct __AFCOperation *AFCOperationRef;
#define kAFCInvalidOperationRef 0

/* Values for AFCConnectionCallBackType */
enum {
    kAFCConnectionNoCallBack = 0,
    kAFCConnectionOperationCompleteCallBack = 1,
    kAFCConnectionInvalidatedCallBack = 2
};

typedef CFOptionFlags AFCConnectionCallBackType;

typedef void (*AFCConnectionCallBack)(AFCConnectionRef conn, AFCConnectionCallBackType callbackType, CFTypeRef object);

/*
 File descriptor objects are Core Foundation objects that represent open files on the server.
 The file descriptor functions create operation objects that can be processed by the connection,
 which will call the connection's callback when they are complete.
 After creating an operation, you must submit it to a connection for processing.
 */

typedef struct __AFCFileDescriptor *AFCFileDescriptorRef;

/*!
 @enum Operation states returned by AFCOperationGetState().
 @constant kAFCOperationStateInvalid No operation should have this state.
 @constant kAFCOperationStateInitialized Fields are initialized.  This is the state of a newly created operation.
 @constant kAFCOperationStateSendPending Waiting to send in the send queue.
 @constant kAFCOperationStateSent Sent to server, but not in the receive queue.
 @constant kAFCOperationStateReplyPending Waiting to receive reply in the receive queue.
 @constant kAFCOperationStateComplete Processing is finished for this operation; either a reply was received or there was an error during processing.
 @constant kAFCOperationStateMax The maximum number of states.
*/

enum __AFCOperationState {
    kAFCOperationStateInvalid = 0,
    kAFCOperationStateInitialized,
    kAFCOperationStateSendPending,
    kAFCOperationStateSent,       
    kAFCOperationStateReplyPending,
    kAFCOperationStateComplete,
    kAFCOperationStateMax
};

typedef enum __AFCOperationState AFCOperationState;

typedef struct _AFCPerFileContext AFCPerFileContext;

typedef struct _AFCServerContext AFCServerContext;

/*!
 @constant kAFCMaxFSBlockSize The maximum filesystem read or write size.  Larger reads will only return this much data, and larger writes will fail.
 */

#define kAFCMaxFSBlockSize (4096 * 1024)

#endif /* __AFC_TYPES_H__ */
