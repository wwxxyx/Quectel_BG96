/* Copyright 2006-2010, Apple Inc.  All rights reserved. */

/*!
 @header AFCClient.h
 @abstract Client interface for the AFC protocol.
 @discussion
 The Apple File Conduit (AFC) protocol is used to transfer files and data between a client and a server.
 Clients and servers are defined by convention; the protocol doesn't specify which is which,
 but in this implementation the host acts as the client and the device acts as the server.
 Clients connect to servers through stream represented by a file descriptor.

 The AFC client interface provides functions for a client to communicate with an AFC server,
 manage connections, and send and receive AFC protocol messages.
 
 For more information on the AFC protocol see AFCProtocol.h.
 
 <h2>Synchronous functions</h2>
 The synchronous interface is deprecated and is not recommended for new code.
 
 <h2>Asynchronous functions</h2>
 The asyncronous functions create an AFCOperation object which represents one operation on the device. To execute the operation asynchronously, you submit it to a connection, and then get a callback when the operation is complete.  For this to work, you must schedule your AFCConnection object with a CFRunLoop to process events.
 
 You can also synchronously process one operation, whether or not your connection has a run loop, using AFCProcessOpperation().
 
 The result of an operation is either just a status, or if the operation was successful, sometimes a result object.  The type of the result object depends on the operation.
 
 */

#ifndef __AFC_CLIENT_H__
#define __AFC_CLIENT_H__

#include <MobileDevice/AFCTypes.h>
#include <MobileDevice/AFCProtocol.h>
#include <CoreFoundation/CoreFoundation.h>

#if !defined(__WIN32__)
#include <AvailabilityMacros.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if 0 /* Don't deprecate anything yet */
#if defined(__WIN32__)
#pragma warning(push)
#pragma warning(disable : 4995)
#endif /* __WIN32__ */
#endif /* 0 */

#if 0
#pragma mark Errors
#endif

/*!
 @group AFC Error Helpers
 @abstract Functions for converting between domain and protocol errors.
 */
	
/*!
 @function AFCMakeDomainError
 @abstract Make a client domain error code.
 @param proto_err The protocol error to convert to a client domain error.
 @discussion
 Convert an AFC protocol error code into a client domain error code.
 */
#define AFCMakeDomainError(proto_err) ((AFCDomainError)(((proto_err) == 0) ? (0) : ((0x3A << 26) | (0x0001 << 14) | (proto_err))))
	
/*!
 @function AFCMakeProtocolError
 @abstract Make a protocol domain error code.
 @param domain_err The client error to convert to a protocol error.
 @discussion
 Convert an AFC domain error code into a protocol error code.
 This is safe to call on a protocol error code value, which will remain unchanged.
 */
#define AFCMakeProtocolError(domain_err) (((int)(domain_err)) & (0x1FFF))
	
/*!
 @function AFCMakeError
 @abstract Make a client domain error code.
 @param err The protocol error to convert to a client domain error.
 @discussion
 Convert an AFC error code into a client domain error code.
 An alias for AFCMakeDomainError().
 */
#define AFCMakeError(err) (AFCMakeDomainError((int)(err)))

/*!
 @group AFC Errors
 @abstract AFC error numbers.
 @discussion
 The "kAFC...Error" constants are the protocol versions of
 AFC errors.  They should only ever appear inside AFC packets
 on the wire.  Clients should never see them.
 The "kAFCError..." constants are the domain versions of
 AFC errors.  They are the errors that are returned by
 all AFC library functions except for the compatibility synchronous functions.
 They should never appear in a packet.
 All API functions return the "kAFCError..." versions.
 
 */
    
    /*! @defined kAFCErrorUndefined
     @abstract An undefined error occurred */
#define kAFCErrorUndefined		AFCMakeDomainError(kAFCUndefinedError)
    /*! @defined kAFCErrorBadHeader
     @abstract The operation header was invalid */
#define kAFCErrorBadHeader		AFCMakeDomainError(kAFCBadHeaderError)
    /*! @defined kAFCErrorNoResources
     @abstract No resources are available for the requested operation */
#define kAFCErrorNoResources	AFCMakeDomainError(kAFCNoResourcesError)
    /*! @defined kAFCErrorRead
     @abstract A read error occurred */
#define kAFCErrorRead			AFCMakeDomainError(kAFCReadError)
    /*! @defined kAFCErrorWrite
     @abstract A write error occurred*/
#define kAFCErrorWrite			AFCMakeDomainError(kAFCWriteError)
    /*! @defined kAFCErrorUnknownPacket
     @abstract Unknown packet type */
#define kAFCErrorUnknownPacket	AFCMakeDomainError(kAFCUnknownPacketError)
    /*! @defined kAFCErrorInvalidArgument
     @abstract Invalid argument */
#define kAFCErrorInvalidArgument AFCMakeDomainError(kAFCInvalidArgumentError)
    /*! @defined kAFCErrorNotFound
     @abstract The requested object was not found */
#define kAFCErrorNotFound		AFCMakeDomainError(kAFCNotFoundError)
    /*! @defined kAFCErrorIsDirectory
     @abstract The requested object is a directory */
#define kAFCErrorIsDirectory	AFCMakeDomainError(kAFCIsDirectoryError)
    /*! @defined kAFCErrorPermission
     @abstract Permission denied */
#define kAFCErrorPermission		AFCMakeDomainError(kAFCPermissionError)
    /*! @defined kAFCErrorNotConnected
     @abstract  The service is not connected */
#define kAFCErrorNotConnected	AFCMakeDomainError(kAFCNotConnectedError)
    /*! @defined kAFCErrorTimeOut
     @abstract  The requested operation timed out */
#define kAFCErrorTimeOut		AFCMakeDomainError(kAFCTimeOutError)
    /*! @defined kAFCErrorOverrun
     @abstract  More data was received than requested */
#define kAFCErrorOverrun		AFCMakeDomainError(kAFCOverrunError)
    /*! @defined kAFCErrorEOF
     @abstract  End of data */
#define kAFCErrorEOF			AFCMakeDomainError(kAFCEOFError)
    /*! @defined kAFCErrorUnsupported
     @abstract  The requested operation is not supported */
#define kAFCErrorUnsupported	AFCMakeDomainError(kAFCUnsupportedError)
    /*! @defined kAFCErrorFileExists
     @abstract  The requested object already exists */
#define kAFCErrorFileExists		AFCMakeDomainError(kAFCFileExistsError)
    /*! @defined kAFCErrorBusy
     @abstract  The requested object is busy */
#define kAFCErrorBusy			AFCMakeDomainError(kAFCBusyError)
    /*! @defined kAFCErrorNoSpace
     @abstract  No space is available */
#define kAFCErrorNoSpace		AFCMakeDomainError(kAFCNoSpaceError)
    /*! @defined kAFCErrorWouldBlock
     @abstract  The requested operation would block */
#define kAFCErrorWouldBlock		AFCMakeDomainError(kAFCWouldBlockError)
    /*! @defined kAFCErrorInputOutput
     @abstract  An error occurred during I/O processing */
#define kAFCErrorInputOutput	AFCMakeDomainError(kAFCInputOutputError)
    /*! @defined kAFCErrorInterrupted
     @abstract  The requested operation was interrupted */
#define kAFCErrorInterrupted	AFCMakeDomainError(kAFCInterruptedError)
    /*! @defined kAFCErrorInProgress
     @abstract  The requested operation is already in progress */
#define kAFCErrorInProgress		AFCMakeDomainError(kAFCInProgressError)
    /*! @defined kAFCErrorInternal
     @abstract  An internal processing error occurred */
#define kAFCErrorInternal		AFCMakeDomainError(kAFCInternalError)

    
/*!
 @group Description strings
 */
	
/*!
 @function AFCCopyErrorString
 @abstract Return a localized string for an AFC error.  Thread safe.
 */
AFC_EXPORT CFStringRef
AFCCopyErrorString(AFCDomainError _err);

/*!
 @function AFCCopyPacketTypeString
 @abstract Return a localized name for an AFC packet type.
 */
AFC_EXPORT CFStringRef
AFCCopyPacketTypeString(uint64_t packetType);
    
#if 0
#pragma mark -
#pragma mark Synchronous interface
#pragma mark -
#endif
    
/*! @group Synchronous interface
 */

/*!
 @function AFCGetClientVersionString
 @abstract Returns a string describing the client library version.
 @result A pointer to a string describing the library version.  The client should not free or modify this string.
 */
AFC_EXPORT const char *
AFCGetClientVersionString( void );
    

#if 0
#pragma mark Connections (Synchronous, deprecated)
#endif

/*! @group Connections (Synchronous, deprecated)
 @discussion The synchronous functions are not recommended for new code.
 */

    
/*!
@function AFCConnectionOpen
 @abstract Open a connection to an AFC server.  Deprecated.
 @deprecated This function is deprecated; use @link AFCConnectionCreate @/link instead.
 @discussion
 Given a file descriptor which is a connection to an AFC
 server, allocates an AFC connection structure
 and initializes it.
 @param fd   A file descriptor with the open socket to the AFC server for which the connection will be created.
 @param context  A pointer to context data to be saved in the connection.
 @param connOut  A pointer to an AFCConnectionRef which will be filled in with the
	resulting new connection reference on success.
 @result Returns either kAFCSuccess on successful completion, or a protocol error code otherwise.
 */

AFC_EXPORT AFCError AFCConnectionOpen(CFSocketNativeHandle fd, void *context, AFCConnectionRef *connOut) /* DEPRECATED_ATTRIBUTE */;

/*!
@function AFCConnectionClose
 @abstract Close an open connection.
 @deprecated This function is deprecated; use @link AFCConnectionCreate @/link to create a connection object instead.
 @discussion Close an open connection.  The connection structure is freed by this function.
 @param conn The AFCConnectionRef of the connection to close.
 @result kAFCSuccess if the connection was closed, or a protocol error code otherwise.
 */
AFC_EXPORT AFCError AFCConnectionClose(AFCConnectionRef conn) /* DEPRECATED_ATTRIBUTE */;

/*!
    @function AFCConnectionGetFSBlockSize
 @abstract Get the filesystem I/O block size for a connection.
    @discussion Returns the current filesystem block size for the connection.
	The block size is the default read and write size for
    filesystem writes.  This is only a hint for the server, which may choose to use
	a smaller block size, but not a larger one.
    @param conn The connection on which to operate.
    @result The block size in bytes used for filesystem reads and writes on the connection.
 */
AFC_EXPORT size_t
AFCConnectionGetFSBlockSize(AFCConnectionRef conn);

/*!
@function AFCConnectionGetSocketBlockSize
@abstract Get the socket I/O block size for a connection.
 @discussion The block size is the default read and write size for
 packet body data on the socket for the connection.
 This is only a hint for the server, which may choose to use
 a smaller block size, but not a larger one.
 @param conn The connection on which to operate.
 @result The block size in bytes used for reads and writes on the connection socket.
 */
AFC_EXPORT size_t
AFCConnectionGetSocketBlockSize(AFCConnectionRef conn);

/*!
@function AFCConnectionSetFSBlockSize
 @abstract Set the filesystem I/O block size on a connection.
 @discussion  Sets the default filesystem read and write block size for the connection.
 This is only a hint for the server, which may choose to use
 a smaller block size, but not a larger one.
 @param conn The connection on which to operate.
 @param blockSize The new filesystem read and write block size in bytes for the connection.
 @result Returns kAFCSuccess if the block size was set, or a protocol error code otherwise.
 If the block size is out of the range that is acceptable for the server device,
 kAFCInvalidArgumentError will be returned.
 */
AFC_EXPORT AFCError
AFCConnectionSetFSBlockSize(AFCConnectionRef conn, size_t blockSize) /* DEPRECATED_ATTRIBUTE */;

/*!
@function AFCConnectionSetSocketBlockSize
@abstract Set the socket I/O block size on a connection.
 @discussion  Sets the default socket read and write block size for the connection.
 This is only a hint for the server, which may choose to use
 a smaller block size, but not a larger one.
 @param conn The connection on which to operate.
 @param blockSize The new block size in bytes for the connection.
 @result Returns kAFCSuccess if the block size was set, or a protocol error code otherwise.
 If the block size is out of the range that is acceptable for the server device,
 kAFCInvalidArgumentError will be returned.
 */
AFC_EXPORT AFCError
AFCConnectionSetSocketBlockSize(AFCConnectionRef conn, size_t blockSize) /* DEPRECATED_ATTRIBUTE */;

/*!
	@function AFCConnectionGetStatus
	@abstract Get the current status of an AFCConnection.
	@discussion Gets the current status of the AFCConnection.  If the connection
	has been established and has not had a fatal error, returns kAFCSuccess.
	Otherwise, returns the last available fatal error which occurred
	on the connection.
	@param conn The connection on which to operate.
	@result The last fatal error on the connection, or kAFCSuccess.
 */
AFC_EXPORT AFCDomainError
AFCConnectionGetStatus(AFCConnectionRef conn);
	
/*!
 @function AFCConnectionCopyLastErrorInfo
 @abstract Copy the error object from the last error condition that occurred on the connection.  Deprecated.
 @discussion Returns an object representing additional information about the last server error,
 or NULL if no information is available.
 This is only useful when using the synchronous interface;
 if using the asynchronous interface, the error info is in each operation.
 This will be a CFError object on platforms that support it,
 or a dictionary otherwise.
 */
AFC_EXPORT CFTypeRef
AFCConnectionCopyLastErrorInfo(AFCConnectionRef conn);
	
#if 0
#pragma mark Directories (Synchronous, deprecated)
#endif

/*!	@group Directories (Synchronous, deprecated)
 @discussion
 Functions to deal with directories.  Deprecated in favor of the operation-based API.
 */

/*!
    @function AFCDirectoryOpen
 @deprecated This function is deprecated; use @link AFCOperationCreateReadDirectory @/link instead.
    @abstract    Iterates over the entries in a directory on
    the server.  Deprecated.
    @param remotePath   The path of the directory to be opened.
    @param dirOut       A pointer to an AFCDirectoryRef.  On success it will be filled in
	with a reference to the opened directory to be used
    with AFCDirectoryRead() and AFCDirectoryClose().
    @result Returns kAFCSuccess if the directory was successfully opened, or a protocol error code otherwise.
 */
AFC_EXPORT AFCError
AFCDirectoryOpen(AFCConnectionRef conn, const char *remotePath, AFCDirectoryRef *dirOut) /* DEPRECATED_ATTRIBUTE */;

/*!
	@function AFCDirectoryRead
	@abstract Read from a directory opened by AFCDirectoryOpen().  Deprecated.
 @deprecated This function is deprecated; see @link AFCOperationCreateReadDirectory @/link instead.

	@param conn The connection on which to operate.
	@param dir An AFCDirectoryRef returned by AFCDirectoryOpen().
	@param nameOut A pointer to a string pointer which will be filled in on success.
    @result Returns kAFCSuccess if the next directory element was filled in to nameOut,
	or a protocol error code if a failure occurred.  Returns kAFCSuccess and writes NULL to nameOut
	when there are no more entries to read.
 */
AFC_EXPORT AFCError
AFCDirectoryRead(AFCConnectionRef conn, AFCDirectoryRef dir, char **nameOut) /* DEPRECATED_ATTRIBUTE */;

/*!
	@function AFCDirectoryClose
	@abstract Close a directory opened by AFCDirectoryOpen().  Deprecated.
 @deprecated This function is deprecated; see @link AFCOperationCreateReadDirectory @/link instead.
	@param conn The connection on which to operate.
	@param dir An AFCDirectoryRef returned by AFCDirectoryOpen().
	@result Returns kAFCSuccess if the AFCDirectoryRef was valid and was successfully closed,
	or a protocol error code otherwise.
 */
AFC_EXPORT AFCError
AFCDirectoryClose(AFCConnectionRef conn, AFCDirectoryRef dir) /* DEPRECATED_ATTRIBUTE */;

/*!
    @function AFCDirectoryCreate
	@abstract Create a new directory on the server. Deprecated.
 @deprecated This function is deprecated; use @link AFCOperationCreateMakeDirectory @/link instead.
	@param conn The connection on which to operate.
	@param remotePath The path on the remote server naming the new directory to be created.
	@result Returns kAFCSuccess if the directory was created, or a protocol error code otherwise.
 */
AFC_EXPORT AFCError
AFCDirectoryCreate(AFCConnectionRef conn, const char *remotePath) /* DEPRECATED_ATTRIBUTE */;


#if 0
#pragma mark File operations (Synchronous, deprecated)
#endif
	
/*!
 @group File Operations (Synchronous, deprecated)
 */

/*!
	@function AFCDeviceInfoOpen
	@abstract Retrieve information about the device.  Deprecated.
 @deprecated This function is deprecated; use @link AFCOperationCreateGetDeviceInfo @/link instead.
	@param conn The connection on which to operate.
	@param kvOut        A reference to a KeyValue structure
 which can be iterated by AFCKeyValueRead() and must be closed with
 AFCKeyValueClose().
 @result kAFCSuccess or a protocol error code.
 */
AFC_EXPORT AFCError
AFCDeviceInfoOpen(AFCConnectionRef conn, AFCKeyValueRef *kvOut) /* DEPRECATED_ATTRIBUTE */;

/*!
	@function AFCFileInfoOpen
	@abstract Retrieve information about a file.  Deprecated.
 @deprecated This function is deprecated; use @link AFCOperationCreateGetFileInfo @/link instead.
	@param conn The connection on which to operate.
	@param remotePath
	@param kvOut        A reference to a KeyValue structure
    which can be iterated by AFCKeyValueRead() and must be closed with
    AFCKeyValueClose().
 @result kAFCSuccess or a protocol error code.
 */
AFC_EXPORT AFCError
AFCFileInfoOpen(AFCConnectionRef conn, const char *remotePath, AFCKeyValueRef *kvOut) /* DEPRECATED_ATTRIBUTE */;

/*!
    @function AFCRemovePath
    @abstract  Remove a file or directory on the remote system.  Deprecated.
    If the target is a directory, it must be empty or an error will be returned.
 @deprecated This function is deprecated; use @link AFCOperationCreateRemovePath @/link instead.
	@param conn The connection on which to operate.
	@param remotePath  The path of the file or directory to remove.
	@result Returns kAFCSuccess if the file or directory was removed, or a protocol error code otherwise.
 */

AFC_EXPORT AFCError
AFCRemovePath(AFCConnectionRef conn, const char *remotePath) /* DEPRECATED_ATTRIBUTE */;

/*!
    @function AFCRenamePath
    @abstract Rename a file or directory on the remote system.  Deprecated.
 @deprecated This function is deprecated; use @link AFCOperationCreateRenamePath @/link instead.
	@param conn The connection on which to operate.
	@param sourcePath   The existing path to rename.
	@param destPath     The new path name.
	@result Returns kAFCSuccess if the path was renamed, or a protocol error code otherwise.
 */
AFC_EXPORT AFCError
AFCRenamePath(AFCConnectionRef conn, const char *sourcePath, const char *destPath) /* DEPRECATED_ATTRIBUTE */;

/*!
 @function AFCLinkPath
 @abstract Create a hard or symbolic link to a file on the device, if the device supports it.  Deprecated.
 @deprecated This function is deprecated; use @link AFCOperationCreateLinkPath @/link instead.
 @param conn The connection on which to operate.
 @param linkType	 The type of link to create, either kAFCFileLinkHard or kAFCFileLinkSymbolic.
					Not all devices may support both or either types of links.
 @param sourcePath   The existing path that will be the source of the link.
 @param destPath     The destination path that will be created as a linnk to the source.
 @result Returns kAFCSuccess if the link was created, or a protocol error code otherwise.
 */
	
AFC_EXPORT AFCError
AFCLinkPath(AFCConnectionRef conn, uint64_t linkType, const char *sourcePath, const char *destPath) /* DEPRECATED_ATTRIBUTE */;
	
#if 0
#pragma mark Key Value Interface (Synchronous, deprecated)
#endif

	/*! @group Key Value Interface (Synchronous, deprecated)
	 */
	
/*!
	@function AFCKeyValueRead
	@abstract Read the next key/value pair from an AFCKeyValueRef reference.  Deprecated.
 @deprecated This function is deprecated in favor of the operation interface that returns CFDictionary objects.
    @discussion 
	An AFCKeyValueRef is a reference to a key/value object returned by functions such as AFCFileInfoOpen()
	or AFCDeviceInfoOpen().  It represents pairs of key and value strings.
	AFCKeyValueRead allows you to iterate over these objects and read all the key/value pairs.
	Calling AFCKeyValueRead() on an AFCKeyValueRef returns the next key/value pair each time.
	When there are no more key/value pairs, it returns NULL as the key and value pointers.
	The storage for the returned strings is associated with the AFCKeyValueRef and is invalidated
	when the reference is closed.
	@param keyOut	A pointer to a string pointer to the next key that will be filled in on success.
	@param valueOut	A pointer to a string pointer to the next value that will be filled in on success.
    @result Returns kAFCSuccess if the key and value pointers were filled in, or a protocol error code otherwise.
	Returns kAFCSuccess and fills in NULL in keyOut and valueOut when there are no more key/value pairs.
 */
AFC_EXPORT AFCError
AFCKeyValueRead(AFCKeyValueRef kvRef, char **keyOut, char **valueOut) /* DEPRECATED_ATTRIBUTE */;

/*!
	@function AFCKeyValueClose
	@abstract Close a key/value reference.  Deprecated.
 @deprecated This function is deprecated in favor of the operation interface that returns CFDictionary objects.
    @discussion Closes a key/value reference and frees its resources, including all the key/value
	strings.  The reference is invalid after calling this function.
	@param kvRef The reference to the key/value structure to be closed.
	@result Returns kAFCSuccess if the reference was successfully closed, or a protocol error code otherwise.
 */
AFC_EXPORT AFCError
AFCKeyValueClose(AFCKeyValueRef kvRef) /* DEPRECATED_ATTRIBUTE */;

#if 0
#pragma mark FileRef Interface (Synchronous, deprecated)
#endif

/*! @group FileRef Interface (Synchronous, deprecated)
 */

/*!
    @function AFCFileRefOpen
    @abstract Open a path on the remote system with the specified mode.  Deprecated.
 @deprecated This function is deprecated in favor of the @link AFCOperationCreateOpenFile @/link function.
	@discussion
    Paths are UTF-8 strings with '/' as path separator.
    Returns a file reference to the open file on the remote system.
	@param conn The connection on which to operate.
	@param remotePath  The UTF-8 path to be opened.
	@param mode
	@param fileRefOut
	@result kAFCSuccess or a protocol error code.
 */
AFC_EXPORT AFCError
AFCFileRefOpen(AFCConnectionRef conn, const char *remotePath, uint64_t mode,  AFCFileRef *fileRefOut) /* DEPRECATED_ATTRIBUTE */;

/*!
    @function AFCFileRefRead
    @abstract Read from an open file.  Deprecated.
 @deprecated This function is deprecated in favor of the @link AFCFileDescriptorCreateReadOperation @/link function.
	@param conn The connection on which to operate.
    @param ref  The AFCFileRef returned by AFCFileRefOpen() of the file from which to read.
    @param buf  The buffer to which data will be written.  The buffer must be at least
    as large as the read request.
    @param lengthInOut  On input, the length to read.  On output, the length read.
	@result Returns kAFCSuccess if the read was successful, or a protocol error code otherwise.
 */
AFC_EXPORT AFCError
AFCFileRefRead(AFCConnectionRef conn, AFCFileRef ref, void *buf, size_t *lengthInOut) /* DEPRECATED_ATTRIBUTE */;

/*!
    @function AFCFileRefWrite
    @abstract Write to an open file.  Deprecated.
 @deprecated This function is deprecated in favor of the @link AFCFileDescriptorCreateWriteOperation @/link function.
	@param conn The connection on which to operate.
    @param ref  The AFCFileRef returned by AFCFileRefOpen() of the file to which data will be written.
    @param buf  The buffer from which data will be read.
    @param writeLength  The length of data in bytes to write to the file.
	@result Returns kAFCSuccess if the write was successful, or a protocol error code otherwise.
 */
AFC_EXPORT AFCError
AFCFileRefWrite(AFCConnectionRef conn, AFCFileRef ref, const void *buf, size_t writeLength) /* DEPRECATED_ATTRIBUTE */;

/*!
    @function AFCFileRefClose
    @abstract Close an open file.  Deprecated.
 @deprecated This function is deprecated in favor of the @link AFCFileDescriptorCreateCloseOperation @/link function.
	@discussion On a successful return, the file is closed on the server and the file reference is no longer valid.
	@param conn The connection on which to operate.
    @param ref  The AFCFileRef to close.
	@result Returns kAFCSuccess if the file was closed, or a protocol error code otherwise.
 */
AFC_EXPORT AFCError
AFCFileRefClose(AFCConnectionRef conn, AFCFileRef ref) /* DEPRECATED_ATTRIBUTE */;

/*!
    @function AFCFileRefSeek
 @abstract Change the current position of an open file.  Deprecated.
 @deprecated This function is deprecated in favor of the @link AFCFileDescriptorCreateSetPositionOperation @/link function.
	@discussion
	The whence parameter can be one of three values:
	<ul>
		<li>kAFCSeekSet Sets the offset to <i>offset</i> bytes</li>
		<li>kAFCSeekCurrent Sets the offset to the current offset plus <i>offset</i> bytes</li>
		<li>kAFCSeekEnd Sets the offset to the end of the file minus <i>offset</i> bytes</li>
	</ul>
 
	@param conn The connection on which to operate.
    @param ref  The AFCFileRef returned by AFCFileRefOpen() whose position will be changed.
    @param offset   The new offset in bytes for the file reference.
    @param whence   The directive for how to set the offset.
	@result Returns kAFCSuccess if the file position was changed, or a protocol error code otherwise.
 */
AFC_EXPORT AFCError
AFCFileRefSeek(AFCConnectionRef conn, AFCFileRef ref, uint64_t offset, uint64_t whence) /* DEPRECATED_ATTRIBUTE */;

/*!
    @function AFCFileRefTell
 @abstract Return the current position of an open file.  Deprecated.
 @deprecated This function is deprecated in favor of the @link AFCFileDescriptorCreateGetPositionOperation @/link function.
	@param conn The connection on which to operate.
    @param ref  The AFCFileRef returned by AFCFileRefOpen() whose position will be queried.
    @param offsetOut  A pointer to the offset, expressed in bytes, which will be filled in on success.
	@result Returns kAFCSuccess if the file position was returned, or a protocol error code otherwise.
  */
AFC_EXPORT AFCError
AFCFileRefTell(AFCConnectionRef conn, AFCFileRef ref, uint64_t *offsetOut) /* DEPRECATED_ATTRIBUTE */;

/*!
    @function AFCFileRefSetFileSize
 @abstract Set the size of a file.  Deprecated.
 @deprecated This function is deprecated in favor of the @link AFCFileDescriptorCreateSetSizeOperation @/link function.
    @discussion  Sets the size of the file to the requested number of bytes.
	If the file is already larger than this,
    the file will be truncated; if the file is smaller, it will be extended to this size
    if there is room in the filesystem.
	@param conn The connection on which to operate.
	@param ref The AFCFileRef of the file returned by AFCFileRefOpen().
	@param fileSize The new size of the file in bytes.
	@result Returns kAFCSuccess if the size of the file was changed, or a protocol error code otherwise.
 */
AFC_EXPORT AFCError
AFCFileRefSetFileSize(AFCConnectionRef conn, AFCFileRef ref, uint64_t fileSize) /* DEPRECATED_ATTRIBUTE */;

/*!
	@function AFCFileRefLock
	@abstract Locks an open file.  Deprecated.
 @deprecated This function is deprecated in favor of the @link AFCFileDescriptorCreateLockOperation @/link function.
	@param conn The connection on which to operate.
	@param ref The AFCFileRef of the file returned by AFCFileRefOpen().
	@param exclusive Whether to take an exclusive lock or not.
	@discussion
	To lock with a shared lock, call with exclusive=false;
	To get an exclusive lock, call with exclusive=true.
	Returns kAFCWouldBlockError if the file was already locked.
	A shared lock may be upgraded to an exclusive lock by calling AFCFileRefLock() again
	with exclusive=true.
 @result kAFCSuccess or a protocol error code.
 */

AFC_EXPORT AFCError
AFCFileRefLock(AFCConnectionRef conn, AFCFileRef ref, bool exclusive) /* DEPRECATED_ATTRIBUTE */;

/*!
@function AFCFileRefUnlock
	@abstract Unlocks an open file. Deprecated.
 @deprecated This function is deprecated in favor of the @link AFCFileDescriptorCreateUnlockOperation @/link function.
	@param conn The connection on which to operate.
	@param ref The AFCFileRef of the file returned by AFCFileRefOpen().
	@discussion
	Unlocks a file previously locked with AFCFileRefLock().
 @result kAFCSuccess or a protocol error code.
 */

AFC_EXPORT AFCError
AFCFileRefUnlock(AFCConnectionRef conn, AFCFileRef ref) /* DEPRECATED_ATTRIBUTE */;

#if 0
#pragma mark -
#pragma mark Asynchronous interface
#pragma mark -
#endif

	/*!
	 Asynchronous interface
	 All functions in this group return AFCDomainError errors.
	 */
	
#if 0
#pragma mark Connections
#endif
	
/*!
 @group Connections
 */
	
/*!
 @function AFCConnectionGetTypeID
 @abstract Get the Core Foundation type identifier of the AFCConnection opaque type.
 @discussion
 Returns the type identifier for the AFCConnection opaque type.
 @result The Core Foundation type identifier of the AFCConnection opaque type.
 */

AFC_EXPORT CFTypeID AFCConnectionGetTypeID(void);

/*!
 @function AFCConnectionCreate
 @abstract
 Create a connection to an AFC server.
 @discussion
 An AFCConnection object represents a connection between a client and an AFC server on a device.
 @param allocator The CoreFoundation allocator to use when creating the object, or NULL to use the default allocator.
 @param fd
 The file decriptor of the open stream connection to the server.
 @param closeOnInvalidate
 If true, the connection object will close the file descriptor and secure IO context when the connection is invalidated.
 If false, the file descriptor will remain open, and could be used to create another connection object.
 @param callBack
 This function will be called whenever an operation has completed processing, and when the connection is about to be invalidated.
 @param connectionInfo
 A client context pointer which is passed to the callback.
 @result
 An AFCConnection object, which is a Core Foundation object, or NULL if the connection could not be created.
 */

AFC_EXPORT AFCConnectionRef AFCConnectionCreate(CFAllocatorRef allocator, CFSocketNativeHandle fd, Boolean closeOnInvalidate, AFCConnectionCallBack callBack, void *connectionInfo);

/*!
 @function AFCConnectionSetCallBack
 @abstract Set the callback to be called when an operation has completed processing on a connection.
 @param conn The connection on which to operate.
 @param callBack
 This function will be called whenever an operation has completed processing, and when the connection is about to be invalidated.
 */
AFC_EXPORT void AFCConnectionSetCallBack(AFCConnectionRef conn, AFCConnectionCallBack callBack);

/*!
 @function AFCConnectionInvalidate
 @abstract
 Invalidate a connection and close the communications channel.
 @discussion
 If closeOnInvalidate was specified when the connection was created,
 then the underlying file descriptor will be closed.
 Unschedules the connection from any run loops it was scheduled on.
 Any pending operations will be cancelled and the connection callback
 will be called for each.
 Thread safe.
 */
AFC_EXPORT void AFCConnectionInvalidate(AFCConnectionRef conn);

/*!
 @function AFCConnectionIsValid
 @abstract
 Returns whether a connection is valid.
 @discussion
 */
AFC_EXPORT Boolean AFCConnectionIsValid(AFCConnectionRef conn);

/*!
 @function AFCConnectionScheduleWithRunLoop
 @abstract
 Add an AFC connection to a run loop so that operations can be processed.
 @discussion
 */

AFC_EXPORT AFCDomainError AFCConnectionScheduleWithRunLoop(AFCConnectionRef conn, CFRunLoopRef runLoop, CFStringRef runLoopMode);

/*!
 @function AFCConnectionUnscheduleFromRunLoop
 @abstract
 Remove an AFC connection from a run loop.
 @discussion
 */

AFC_EXPORT void AFCConnectionUnscheduleFromRunLoop(AFCConnectionRef conn, CFRunLoopRef runLoop, CFStringRef runLoopMode);


/*!
 @function AFCConnectionSubmitOperation
 @abstract
 Asynchronously process an operation.
 @discussion
 Submitting an operation to a connection schedules it for processing on the connection's run loop.
 When the operation is complete, the connection's callback will be called.
 Returns an error if the operation could not be submitted for processing,
 in which case the callback will not be called.
 This function doesn't change the memory ownership of the operation.
 */
AFC_EXPORT AFCDomainError AFCConnectionSubmitOperation(AFCConnectionRef conn, AFCOperationRef op);

/*!
 @function AFCConnectionProcessOperation
 @abstract
 Processes a single operation immediately.
 If the operation was processed successfully, returns kAFCSuccess;
 use AFCOperationGetResultStatus() to get the result of the operation.
 Will call the connection callback on completion, if one was defined.
 @discussion
 May be used if the connection run loop source has not been scheduled on a run loop.
 Intended for processing operations without a run loop.
 */
AFC_EXPORT AFCDomainError AFCConnectionProcessOperation(AFCConnectionRef conn, AFCOperationRef op, CFTimeInterval seconds);

/*!
 @function AFCConnectionProcessOperations
 @abstract
 Continue to process operations until timeout passes.
 @discussion
 Timeout of 0.0 means process until the connection is invalidated.
 Will return kAFCErrorBusy if the connection is already scheduled on a run loop.
 Intended for processing operations without a run loop.
 */
AFC_EXPORT AFCDomainError AFCConnectionProcessOperations(AFCConnectionRef conn, CFTimeInterval timeoutSeconds);


/*!
 @function AFCConnectionGetContext
 @abstract Gets the context argument supplied when the connection was opened
 or last set.
 @param conn The connection on which to operate.
 @result The context pointer.
 */
AFC_EXPORT void *
AFCConnectionGetContext(AFCConnectionRef conn);

/*!
 @function AFCConnectionSetContext
 @abstract
 Sets the context pointer for a connection.
 @param conn The connection on which to operate.
 @param context The context pointer for the connection.
 */
AFC_EXPORT void
AFCConnectionSetContext(AFCConnectionRef conn, void *context);

/*!
 @function AFCConnectionGetSecureContext
 @abstract
 Gets the secure context argument that was last set.
 @param conn The connection on which to operate.
 @result The context pointer.
 */
AFC_EXPORT void *
AFCConnectionGetSecureContext(AFCConnectionRef conn);

/*!
 @function AFCConnectionSetSecureContext
 @abstract
 Assign a secure read or write context to an AFC connection.
 @discussion
 To use AFC over SSL, call this immediately after AFCConnectionCreate().
 If the connection was created with closeOnInvalidate set to true,
 then the connection will also dispose of the secure context on invalidation and ownership of the context should be
 considered to pass to the connection. To remove a secure IO context, set the context to NULL.
 @param conn
 The connection on which to operate.
 @param secure_context
 The secure context. Will be either an SSL * or a SSLContextRef.
 */
AFC_EXPORT void AFCConnectionSetSecureContext(AFCConnectionRef conn, void *secure_context);

/*!
 @function AFCConnectionSetDisposeSecureContextOnInvalidate
 @abstract
 Set whether the connection should dispose the secure context itself,
 or let the client do it.
 Default is for the AFC connection to dispose of the context.
 */
AFC_EXPORT void AFCConnectionSetDisposeSecureContextOnInvalidate(AFCConnectionRef conn, bool dispose);

/*!
 @function AFCConnectionGetIOTimeout
 @abstract Get the I/O timeout on a connection.
 @discussion The IOTimeout on a connection is the timeout for reading
 and writing on the connection, in seconds.  0 means no timeout, and I/O operations will block until completed.
 @param conn The connection on which to operate.
 @result The timeout in seconds for I/O operations on the connection.
 */
AFC_EXPORT time_t
AFCConnectionGetIOTimeout(AFCConnectionRef conn);

/*!
 @function AFCConnectionSetIOTimeout
 @abstract Set the I/O timeout on a connection.
 @discussion The I/O timeout on a connection is the timeout for reading
 and writing on the connection, in seconds.  0 means no timeout, and I/O operations will block until completed.
 @param conn The connection on which to operate.
 @param timeout The timeout in seconds for I/O operations on the connection.
 */
AFC_EXPORT void
AFCConnectionSetIOTimeout(AFCConnectionRef conn, time_t timeout);
	

/*!
 @group Error dictionary keys
 @discussion
 An AFC error dictionary is a dictionary containing detailed information
 about an error, similar to a CFError object.
 */
	
    /*!	@const kAFCErrorLocalizedDescriptionKey
     @abstract 	The localized description of the error. */
AFC_EXPORT const CFStringRef kAFCErrorLocalizedDescriptionKey;
    /*!	@const kAFCErrorLocalizedFailureReasonKey
     @abstract  A description of the underlying failure. */
AFC_EXPORT const CFStringRef kAFCErrorLocalizedFailureReasonKey;
    /*! @const kAFCErrorLocalizedRecoverySuggestionKey
     @abstract  A suggestion for recovery. */
AFC_EXPORT const CFStringRef kAFCErrorLocalizedRecoverySuggestionKey;
    /*! @const kAFCErrorDescriptionKey
     @abstract  A non-localized description of the error. */
AFC_EXPORT const CFStringRef kAFCErrorDescriptionKey;
    /*! @const kAFCErrorDebugDescriptionKey
     @abstract  Additional debugging information about the error. */
AFC_EXPORT const CFStringRef kAFCErrorDebugDescriptionKey;
    /*! @const kAFCErrorUnderlyingErrorKey
     @abstract  The code for the underlying error, usually an errno. */
AFC_EXPORT const CFStringRef kAFCErrorUnderlyingErrorKey;
    /*!	@const kAFCErrorDomainKey
     @abstract  The domain for the error, kAFCErrorDomain. */
AFC_EXPORT const CFStringRef kAFCErrorDomainKey;
    /*! @const kAFCErrorCodeKey
     @abstract 	The error number. */
AFC_EXPORT const CFStringRef kAFCErrorCodeKey;
    /*! @const kAFCErrorExtendedInfoKey
     @abstract 	A dictionary containing additional information. */
AFC_EXPORT const CFStringRef kAFCErrorExtendedInfoKey;
    /*! @const kAFCErrorFileNameKey
     @abstract  The file name for the source file where the error occurred. */
AFC_EXPORT const CFStringRef kAFCErrorFileNameKey;
    /*! @const kAFCErrorFileLineKey
     @abstract 	The line number where the error occurred. */
AFC_EXPORT const CFStringRef kAFCErrorFileLineKey;
    /*! @const kAFCErrorVersionKey
     @abstract  The version of either the client or server that returned the error. */
AFC_EXPORT const CFStringRef kAFCErrorVersionKey;

/*!
 @group Error dictionary values
 @abstract Constants for values found in error dictionaries.
 */

/*!	@const kAFCErrorDomain
 @abstract The error occurred on the client. */
AFC_EXPORT const CFStringRef kAFCErrorDomain;
	
	

#if 0
#pragma mark File Descriptors
#endif
    
/*!
 @group File Descriptors
 */
 

/*!
 @function AFCFileDescriptorGetTypeID
 @abstract Returns the Core Foundation type identifier of the AFCFileDescriptor opaque type.
 @discussion
 Returns the type identifier for the AFCFileDescriptor opaque type.
 @result The Core Foundation type identifier of the AFCFileDescriptor opaque type.
 */
	
AFC_EXPORT CFTypeID AFCFileDescriptorGetTypeID(void);

/*!
 @function AFCFileDescriptorCreateReadOperation
 @abstract Create an operation to read data from an open file.
 @result An operation object representing the read operation.
 @discussion
 The result object for this operation is a CFData object with the read data.
 */
AFC_EXPORT AFCOperationRef AFCFileDescriptorCreateReadOperation(AFCFileDescriptorRef desc, uint8_t *buffer, CFAllocatorRef bufferDeallocator, CFIndex length, void *operationInfo);

/*!
 @function AFCFileDescriptorCreateReadAtPositionOperation
 @abstract Create an operation to read data from an open file at a given position.
 @result An operation object representing the read operation.
 @discussion
 The result object for this operation is a CFData object with the read data.  Does not affect the current file descriptor position.
 */
AFC_EXPORT AFCOperationRef AFCFileDescriptorCreateReadAtPositionOperation(AFCFileDescriptorRef desc, uint8_t *buffer, CFAllocatorRef bufferDeallocator, CFIndex length, UInt64 position, void *operationInfo);

/*!
 @function AFCFileDescriptorCreateWriteOperation
 @abstract Create an operation to write data to an open file.
 @result An operation object representing the write operation.
 @param desc The file descriptor object on which to operate.
 @param data
 @param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
 @discussion
 The result object for this operation is a kCFNull object.
 */
AFC_EXPORT AFCOperationRef AFCFileDescriptorCreateWriteOperation(AFCFileDescriptorRef desc, CFDataRef data, void *operationInfo);

/*!
 @function AFCFileDescriptorCreateWriteAtPositionOperation
 @abstract Create an operation to write data to an open file at a given position.
 @result An operation object representing the write operation.
 @param desc The file descriptor object on which to operate.
 @param data
 @param position The position at which to write the data.
 @param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
 @discussion
 The result object for this operation is a kCFNull object.
 */
AFC_EXPORT AFCOperationRef AFCFileDescriptorCreateWriteAtPositionOperation(AFCFileDescriptorRef desc, CFDataRef data, UInt64 position, void *operationInfo);
    
/*!
 @function AFCFileDescriptorCreateSetPositionOperation
 @abstract Create an operation to set the current position of an open file.
 @param desc The file descriptor object on which to operate.
 @param seek_type
 @param position
 @param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
 @discussion
 The result object for this operation is a kCFNull object.
 */
AFC_EXPORT AFCOperationRef AFCFileDescriptorCreateSetPositionOperation(AFCFileDescriptorRef desc, CFIndex seek_type, SInt64 position, void *operationInfo);

/*!
 @function AFCFileDescriptorCreateGetPositionOperation
 @abstract Create an operation to get the current position of an open file.
 @param desc The file descriptor object on which to operate.
 @param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
 @discussion
 The result object for this operations is a CFNumber object representing the current position.
 */
AFC_EXPORT AFCOperationRef AFCFileDescriptorCreateGetPositionOperation(AFCFileDescriptorRef desc, void *operationInfo);

/*!
 @function AFCFileDescriptorCreateSetSizeOperation
 @abstract Create an operation to set the size of an open file.
 @param desc The file descriptor object on which to operate.
 @param size The new size of the file, in bytes.
 @param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
 @discussion
 The result object for this operation is a kCFNull object.
 */
AFC_EXPORT AFCOperationRef AFCFileDescriptorCreateSetSizeOperation(AFCFileDescriptorRef desc, UInt64 size, void *operationInfo);

/*!
 @function AFCFileDescriptorCreateLockOperation
 @abstract Create an operation to lock an open file.
 @param desc The file descriptor object on which to operate.
 @param exclusive Whether the lock is an exclusive lock or not.
 @param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
 @discussion
 The result object for this operation is a kCFNull object.
 */
AFC_EXPORT AFCOperationRef AFCFileDescriptorCreateLockOperation(AFCFileDescriptorRef desc, Boolean exclusive, void *operationInfo);

/*!
 @function AFCFileDescriptorCreateUnlockOperation
 @abstract Create an operation to unlock an open file.
 @param desc The file descriptor object on which to operate.
 @param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
 @discussion
 The result object for this operation is a kCFNull object.
 */
AFC_EXPORT AFCOperationRef AFCFileDescriptorCreateUnlockOperation(AFCFileDescriptorRef desc, void *operationInfo);

/*!
 @function AFCFileDescriptorCreateCloseOperation
 @abstract Create an operation to close an open file.
 @param desc The file descriptor object on which to operate.
 @param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
 @discussion
 The result object for this operation is a kCFNull object.
 */
AFC_EXPORT AFCOperationRef AFCFileDescriptorCreateCloseOperation(AFCFileDescriptorRef desc, void *operationInfo);

/*!
 @function AFCFileDescriptorCreateSetImmutableHintOperation
 @abstract Create an operation to set the immutability hint on an open file.
 @param desc The file descriptor object on which to operate.
 @param immutable If this flag is true, then the operating system may treat the file from the current position onward as if it is unlikely to change in the future, which may lead it to make optimizations.
 @param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
 @discussion
 The result object for this operation is a kCFNull object.
 */

AFC_EXPORT AFCOperationRef AFCFileDescriptorCreateSetImmutableHintOperation(AFCFileDescriptorRef desc, Boolean immutable, void *operationInfo);

/*!
 @function AFCFileDescriptorInvalidate
 @abstract Invalidate a file descriptor and close the file on the server.
 @param desc The file descriptor object to invalidate.
 @discussion
 Invalidates the file descriptor object locally.
 Before you invalidate the connection locally, you must create, submit and process a close operation on the file descriptor using @link AFCFileDescriptorCreateCloseOperation @/link 
 to close the file descriptor on the server, or you will leak a file descriptor there.
 */
AFC_EXPORT void AFCFileDescriptorInvalidate(AFCFileDescriptorRef desc);

/*!
 @function AFCFileDescriptorIsValid
 @abstract Returns true if the file descriptor object is valid.
 @param desc The file descriptor object on which to operate.
 @result Returns true if the file descriptor object is valid, or false otherwise.
 @discussion
 A valid file descriptor object represents an open file on the device and can be used to create file descriptor request objects.  An invalid file descriptor cannot be used to send requests.
 */
AFC_EXPORT Boolean AFCFileDescriptorIsValid(AFCFileDescriptorRef desc);

#if 0
#pragma mark Operations
#endif
    
/*! @group Operations
 @discussion
 Functions that deal with operation objects.
 */

/*! @function AFCOperationGetTypeID
 @abstract Returns the Core Foundation type identifier of the AFCOperation opaque type.
 @discussion
 Returns the type identifier for the AFCOperation opaque type.
 @result The Core Foundation type identifier of the AFCOperation opaque type.
 
 */
AFC_EXPORT CFTypeID AFCOperationGetTypeID(void);

/*!
@function AFCOperationGetResultObject
@abstract Returns a Core Foundation object representing the result of the operation.
@discussion
 If the operation state is not kAFCOperationStateComplete, then
 this function will return NULL.
If the operation was successful, the result object depends on the operation;
 see the documentation for each operation type for information on the object
 it returns.
 If the operation failed, the result object is a CFDictionary
 with information about the error.  See the documentation for
 the error keys for more information.
*/
AFC_EXPORT CFTypeRef AFCOperationGetResultObject(AFCOperationRef op);

/*!
@function AFCOperationGetState
@abstract Get the current state of the operation.
@discussion
 This function returns the current state of processing of the operation.
 If you have submitted the operation to a connection for processing,
 it is not safe to examine or alter the operation
 until its state is kAFCOperationStateComplete.
 Calling this function is always safe.
*/
AFC_EXPORT AFCOperationState AFCOperationGetState(AFCOperationRef op);

/*!
@function AFCOperationGetContext
@abstract Get the context pointer from the operation.
@discussion
 Returns the context pointer (operationInfo) set when the operation was created.
*/
AFC_EXPORT void *AFCOperationGetContext(AFCOperationRef op);

/*!
 @function AFCOperationSetContext
 @abstract Set the context pointer for the operation.
 @discussion
 Sets the context pointer for the operation.
 */
    AFC_EXPORT void AFCOperationSetContext(AFCOperationRef op, void *context);

/*!
@function AFCOperationGetResultStatus
@abstract Get the status of the completed operation.
@discussion
 Returns the final status of the operation.
 If the operation state is not kAFCOperationStateComplete,
 returns kAFCErrorInProgress.
*/
AFC_EXPORT AFCDomainError AFCOperationGetResultStatus(AFCOperationRef op);

/*!
 @group Packet header dictionary keys
  @discussion
 Keys for the AFC packet header dictionary returned by AFCOperationCreatePacketHeaderDictionary().
 */

/*! @const kAFCHeaderKeySignature
 @abstract The signature from the packet as a CFNumber. */
AFC_EXPORT const CFStringRef kAFCHeaderKeySignature;
	
/*! @const kAFCHeaderKeyHeaderLength
 @abstract Header length as a CFNumber. */
AFC_EXPORT const CFStringRef kAFCHeaderKeyHeaderLength;

/*! @const kAFCHeaderKeyPacketLength
 @abstract Length of entire packet as a CFNumber. */
AFC_EXPORT const CFStringRef kAFCHeaderKeyPacketLength;

/*!	@const kAFCHeaderKeyPacketID
 @abstract Packet ID as a CFNumber. */
AFC_EXPORT const CFStringRef kAFCHeaderKeyPacketID;
	
/*! @const kAFCHeaderKeyPacketType
 @abstract Packet type as a CFNumber. */
AFC_EXPORT const CFStringRef kAFCHeaderKeyPacketType;

/*! @const kAFCHeaderKeyPacketData
 @abstract The entire packet as a CFData. */
AFC_EXPORT const CFStringRef kAFCHeaderKeyPacketData;
    
/*!
@function AFCOperationCreatePacketHeaderDictionary
@abstract Create a CFDictionary representing the packet header for this operation.
@discussion
Creates a CFDictionary that contains the data in the packet header for this operation.
 @result
 A CFDictionary representing the packet header.
*/
AFC_EXPORT CFDictionaryRef AFCOperationCreatePacketHeaderDictionary(AFCOperationRef op);

/*!
 @function AFCOperationCopyPacketData
 @abstract Copy the packet body data from an operation object.
 @result
 A CFData object with a copy of the packet body data.
 */
AFC_EXPORT CFDataRef AFCOperationCopyPacketData(AFCOperationRef op);

/*!
 @group Operation Requests
 @discussion
  Asynchronous interface to AFC requests.
  These functions create an operation object which can be processed synchronously,
  or handed off to the connection for later processing.
 */

/*!
@function AFCOperationCreateGetDeviceInfo
@abstract Create an operation to get information about the device.
 @param allocator The CoreFoundation allocator to use when creating the object, or NULL to use the default allocator.
@param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
@discussion
 The result object for this operation is a CFDictionary on success.
 @result
 Returns an AFCOperation object, or NULL if there were no resources available to create it.
*/
AFC_EXPORT AFCOperationRef
AFCOperationCreateGetDeviceInfo(CFAllocatorRef allocator, void *operationInfo);

/*!
@function AFCOperationCreateGetFileInfo
@abstract Create an operation to get information about a file.
@param allocator The CoreFoundation allocator to use when creating the object, or NULL to use the default allocator.
@param remotePath The path of the file for which to return information.
@param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
@discussion
 The result object for this operation is a CFDictionary on success.
 @result
 Returns an AFCOperation object, or NULL if there were no resources available to create it.
*/
AFC_EXPORT AFCOperationRef
AFCOperationCreateGetFileInfo(CFAllocatorRef allocator, CFStringRef remotePath, void *operationInfo);

/*!
@function AFCOperationCreateGetConnectionInfo
@abstract Create an operation to get information about the device-side connection.
@param allocator The CoreFoundation allocator to use when creating the object, or NULL to use the default allocator.
@param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
@discussion
 The result object for this operation is a CFDictionary .
 @result
 Returns an AFCOperation object, or NULL if there were no resources available to create it.
*/
AFC_EXPORT AFCOperationRef
AFCOperationCreateGetConnectionInfo(CFAllocatorRef allocator, void *operationInfo);

/*!
@function AFCOperationCreateSetConnectionOptions
@abstract Create an operation to set connection options.
@param allocator The CoreFoundation allocator to use when creating the object, or NULL to use the default allocator.
@param options
@param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
@discussion
 The result object for this operation is kCFNull on success.
 @result
 Returns an AFCOperation object, or NULL if there were no resources available to create it.
*/
AFC_EXPORT AFCOperationRef
AFCOperationCreateSetConnectionOptions(CFAllocatorRef allocator, CFDictionaryRef options, void *operationInfo);

/*!
@function AFCOperationCreateRemovePath
@abstract  Create an operation to remove a file or directory on the device.
 @param allocator The CoreFoundation allocator to use when creating the object, or NULL to use the default allocator.
@param remotePath The path of the file or directory to remove.
@param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
@discussion
 The result object for this operation is kCFNull on success.
@result
 Returns an AFCOperation object, or NULL if there were no resources available to create it.
*/
AFC_EXPORT AFCOperationRef
AFCOperationCreateRemovePath(CFAllocatorRef allocator, CFStringRef remotePath, void *operationInfo);

/*!
@function AFCOperationCreateRenamePath
@abstract Create an operation to rename a file or directory on the device.
 @param allocator The CoreFoundation allocator to use when creating the object, or NULL to use the default allocator.
@param sourcePath The existing file or directory path.
@param destPath The new file or directory path.
@param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
@discussion
 The result object for this operation is kCFNull on success.
 @result
 Returns an AFCOperation object, or NULL if there were no resources available to create it.
*/
AFC_EXPORT AFCOperationRef
AFCOperationCreateRenamePath(CFAllocatorRef allocator, CFStringRef sourcePath, CFStringRef destPath, void *operationInfo);

/*!
 @function AFCOperationCreateSetModTime
 @abstract Create an operation to set the modification time of a file.
 @param allocator The CoreFoundation allocator to use when creating the object, or NULL to use the default allocator.
 @param path The path to the file on which to operate.
 @param modTimeNanoseconds The new modification time of the file, expressed in nanoseconds since the epoch.
 @param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
 @discussion
 The result operation for this operation is kCFNull on success.
 @result
 */
AFC_EXPORT AFCOperationRef AFCOperationCreateSetModTime(CFAllocatorRef allocator, CFStringRef path, UInt64 modTimeNanoseconds, void *operationInfo);
	
/*!
@function AFCOperationCreateLinkPath
@abstract Create an operation to create a symbolic or hard link.
 @param allocator The CoreFoundation allocator to use when creating the object, or NULL to use the default allocator.
@param linkType
@param sourcePath
@param destPath
@param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
@discussion
 The result object for this operation is kCFNull on success.
 @result
 Returns an AFCOperation object, or NULL if there were no resources available to create it.
*/
AFC_EXPORT AFCOperationRef
AFCOperationCreateLinkPath(CFAllocatorRef allocator, CFIndex linkType, CFStringRef sourcePath, CFStringRef destPath, void *operationInfo);

/*!
@function AFCOperationCreateReadDirectory
@abstract Create an operation to list the contents of a directory.
 @param allocator The CoreFoundation allocator to use when creating the object, or NULL to use the default allocator.
@param remotePath The path of the directory to list.
 @param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
@discussion
 The result object for this operation is a CFArray of CFString objects.
 @result
 Returns an AFCOperation object, or NULL if there were no resources available to create it.
 */
AFC_EXPORT AFCOperationRef
AFCOperationCreateReadDirectory(CFAllocatorRef allocator, CFStringRef remotePath, void *operationInfo);

/*!
@function AFCOperationCreateMakeDirectory
@abstract Create an operation to make a directory.
 @param allocator The CoreFoundation allocator to use when creating the object, or NULL to use the default allocator.
@param remotePath The path of the directory to create.
 @param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
@discussion
 The result object for this operation is kCFNull on success.
 @result
 Returns an AFCOperation object, or NULL if there were no resources available to create it.
*/
AFC_EXPORT AFCOperationRef
AFCOperationCreateMakeDirectory(CFAllocatorRef allocator, CFStringRef remotePath, void *operationInfo);

/*!
@function AFCOperationCreateOpenFile
@abstract Create an operation to open a file and create an AFCFileDescriptor object.
 @param allocator The CoreFoundation allocator to use when creating the object, or NULL to use the default allocator.
@param remotePath The path of the file to open.
 @param mode
 @param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
@discussion
 The result object for this operation is an AFCFileDescriptor object. This object may be used to create operations on the open file, as seen in the AFCFileDescriptorCreate...Operation functions.
 
 If the AFCFileDescriptor object is released before it is closed, it will generate an implicit close operation and submit it to the connection it was created on.  If the connection is not scheduled on a run loop, there is no way to process that operation, and the file descriptor will be leaked.
 
 To prevent this, always use @link AFCFileDescriptorCreateCloseOperation @/link to create a close operation and process that operation before releasing the file descriptor object.
 
 @result
 Returns an AFCOperation object, or NULL if there were no resources available to create it.
*/
AFC_EXPORT AFCOperationRef
AFCOperationCreateOpenFile(CFAllocatorRef allocator, CFStringRef remotePath, CFIndex mode, void *operationInfo);

/*!
 @function AFCOperationCreateGetFileHash
 @abstract Create an operation to get a cryptographic hash of a file's contents.
 @param allocator The CoreFoundation allocator to use when creating the object, or NULL to use the default allocator.
 @param remotePath The remote file to get a hash range.
 @param operationInfo A context pointer meaningful to the client that is returned by the @link AFCOperationGetContext @/link function.  May be NULL.
 @discussion
 The result object for this operation is a CFData object.
 @result
 Returns an AFCOperation object, or NULL if there were no resources available to create it.
 */
AFC_EXPORT AFCOperationRef
AFCOperationCreateGetFileHash(CFAllocatorRef allocator, CFStringRef remotePath, void *operationInfo);

/*!
 @function AFCOperationCreateGetFileHashWithRange
 @abstract Create an operation to get a cryptographic hash of a file's contents within a given range.
 @param allocator The CoreFoundation allocator to use when creating the object, or NULL to use the default allocator.
 @param remotePath The remote file to get a hash range.
 @param rangeStart The start position of the range to hash.
 @param rangeLength The length of the range to hash.
 @discussion
 The result object for this operation is a CFData object.
 @result
 Returns an AFCOperation object, or NULL if there were no resources available to create it.
 */
AFC_EXPORT AFCOperationRef
AFCOperationCreateGetFileHashWithRange(CFAllocatorRef allocator, CFStringRef remotePath, uint64_t rangeStart, uint64_t rangeLength, void *operationInfo);

/*!
 @function AFCOperationCreateGetSizeOfPathContents
 @param remotePath The path on the device which will have its size computed.
 @param options Options on how to compute the size.
 @discussion If remotePath is a file, then its size will be returned.  If remotePath is a directory, then all regular files in the directory and in any directories below it will
 have their sizes added to the total size returned.
 If Option is set to kAFCGetSizeOfPathContentsAllFilesOption, then the size of any directories themselves and any special files will be included in the size.
 The result object is a CFDictionary with two entries: st_size is the total size in bytes, and st_blocks is the total size in blocks.  Both values are strings.
 @result
 Returns an AFCOperation object, or NULL if there were no resources available to create it.
 */
AFC_EXPORT AFCOperationRef
AFCOperationCreateGetSizeOfPathContents(CFAllocatorRef allocator, CFStringRef remotePath, uint64_t options, void *operationInfo);
    
/*!
 @function AFCOperationCreateRemovePathAndContents
 @param allocator
 @param remotePath The path to remove.
 @param operationInfo
 @discussion Removes remotePath, and if it is a directory, all files and directories underneath it.
 The result object for this operation is kCFNull on success.
 @result
 Returns an AFCOperation object, or NULL if there were no resources available to create it.
 */
AFC_EXPORT AFCOperationRef
AFCOperationCreateRemovePathAndContents(CFAllocatorRef allocator, CFStringRef remotePath, void *operationInfo);
    
#if 0 /* Don't deprecate anything yet */
#if defined(__WIN32__)
#pragma warning(pop)
#pragma deprecated(AFCConnectionOpen, AFCConnectionClose)
#pragma deprecated(AFCConnectionSetFSBlockSize)
#pragma deprecated(AFCConnectionSetSocketBlockSize)
#pragma deprecated(AFCDirectoryOpen)
#pragma deprecated(AFCDirectoryRead)
#pragma deprecated(AFCDirectoryClose)
#pragma deprecated(AFCDirectoryCreate)
#pragma deprecated(AFCDeviceInfoOpen)
#pragma deprecated(AFCFileInfoOpen)
#pragma deprecated(AFCRemovePath)
#pragma deprecated(AFCRenamePath)
#pragma deprecated(AFCLinkPath)
#pragma deprecated(AFCKeyValueRead)
#pragma deprecated(AFCKeyValueClose)
#pragma deprecated(AFCFileRefOpen)
#pragma deprecated(AFCFileRefRead)
#pragma deprecated(AFCFileRefWrite)
#pragma deprecated(AFCFileRefClose)
#pragma deprecated(AFCFileRefSeek)
#pragma deprecated(AFCFileRefTell)
#pragma deprecated(AFCFileRefSetFileSize)
#pragma deprecated(AFCFileRefLock)
#pragma deprecated(AFCFileRefUnlock)
#endif /* __WIN32__ */
#endif /* 0 */
	
#ifdef __cplusplus
}
#endif
	
#endif /* __AFC_CLIENT_H__ */
