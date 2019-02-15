/* Copyright 2006-2009, Apple Inc.  All rights reserved. */

#ifndef __AFC_RAW_H__
#define __AFC_RAW_H__

/*!
	@header AFCRaw.h
	@discussion Functions that operate at a low level on an AFC connection.
	Mainly useful for implementing higher-level API.
	Can be used to roll your own AFC packet types.
 */

#include <MobileDevice/AFCTypes.h>
#include <MobileDevice/AFCProtocol.h>

#include <CoreFoundation/CoreFoundation.h>

#ifdef __cplusplus
extern "C" {
#endif
		
/*!
@function AFCValidateHeader
 @discussion
 Validate a packet header by checking the signature, packet length
 and body length.  Validates only the core fields in the packet header.
 Swaps wrong-endian values in the core header (but not the signature); if you use
 extended parameters in the header, you must swap them yourself.
 */
extern AFCDomainError AFCValidateHeader(AFCPacketHeader *hdr);

/*!
 @function AFCSwapHeader
 @discussion
 Swaps known non-core header fields.  If the packet type is not known,
 returns kAFCDomainErrorUnknownPacket.
*/	 
extern AFCDomainError AFCSwapHeader(AFCPacketHeader *hdr);
	
/*!
    @function AFCReadPacketHeader
    @discussion
 Read a full packet header.  If the caller supplies a static buffer in staticBuf,
 and the packet header fits, it will be copied there and allocedHdrOut will be set to NULL;
 otherwise a new buffer will be allocated and returned.
 The caller must free the header buffer when finished with it.
 Note that an extra NUL byte is added to the end of the header to defend against
 non-NUL terminated strings in packets that use them.
 */
extern AFCDomainError AFCReadPacketHeader(AFCConnectionRef conn, AFCPacketHeader *staticBuf, size_t bufSize, AFCPacketHeader **allocedHdrOut);

/*!
	@function AFCReadPacketBody
	@discussion Read the entire body of a packet.
	The caller must supply the packet header so the body length can be known.
	The caller must free the body buffer when finished with it.
 */
extern AFCDomainError AFCReadPacketBody(AFCConnectionRef conn, const AFCPacketHeader *hdr, uint8_t **bodyOut, size_t *bodyLengthOut);

/*!
	@function AFCReadPacket
	@discussion Read an entire packet, both header and body.
	The caller must free the header and body buffers when finished with them.
 */
extern AFCDomainError AFCReadPacket(AFCConnectionRef conn, AFCPacketHeader **hdrOut, uint8_t **bodyOut, size_t *bodyLengthOut);

/*!
	@function AFCReadData
	@discussion
 Read some amount of data from a connection into the caller's buffer.
 */
extern AFCDomainError AFCReadData(AFCConnectionRef conn, void *buffer, size_t readLength);


/*!
	@function AFCDiscardData
	@discussion Discard data from a connection.
	@param length The length of the data to discard.
 */
extern AFCDomainError
AFCDiscardData(AFCConnectionRef conn, size_t length);

/*!
	@function AFCDiscardBodyData
	@discussion Discard the body of an AFC packet, if any.
 */
extern AFCDomainError
AFCDiscardBodyData(AFCConnectionRef conn, const AFCPacketHeader *hdr);


/*!
	@function AFCProcessServerPacket
	@discussion
 Process the packet.
 It is passed the entire header, but the body has not been read from the stream.
 The processor is responsible for reading the body.
 */
extern AFCDomainError AFCProcessServerPacket(AFCConnectionRef conn, const AFCPacketHeader *hdr);

/*!
	@function AFCErrnoToAFCDomainError
	@discussion
 Convert an errno to an AFC domain error.  If it doesn't convert,
 it will return defaultError.
 It will always return defaultError if errno is less than zero.
 */
extern AFCDomainError AFCErrnoToAFCDomainError(int sys_errno, AFCDomainError defaultError);


/*!
 @function AFCDomainErrorFromCFError
 @discussion
 Given a CFError object, returns an appropriate AFC domain error to represent the error,
 or defaultError if it cannot be determined.
 */
extern AFCDomainError AFCDomainErrorFromCFError(CFErrorRef error, AFCDomainError defaultError);

/*! @functiongroup Sending packets */

/*!
	@function AFCSendStatus
	@discussion Send a status reply to a previous message,
 whose header you must pass in.
 */
extern AFCDomainError AFCSendStatus(AFCConnectionRef conn, const AFCPacketHeader *origHdr, AFCDomainError result);

    /*!
     @function AFCSendStatusExtended
     @discussion Send a status reply to a previous message,
     whose header you must pass in.
     */
    
extern AFCDomainError AFCSendStatusExtended(AFCConnectionRef conn, const AFCPacketHeader *origHdr, AFCDomainError result, CFDictionaryRef info);

/*!
	@function AFCHeaderInit
	@discussion Initialize a packet header.
	Use right before sending the packet.
 */
    extern void
    AFCHeaderInit(AFCPacketHeader *hdr, AFCPacketType packetType,
                  size_t headerLength, size_t bodyLength,
                  const AFCPacketHeader *origHdr);
    
/*!
	@function AFCSendHeader
	@discussion
	Send a header of any size.
	You must have called AFCHeaderInit on the packet header.
 */
extern AFCDomainError
AFCSendHeader(AFCConnectionRef conn, const AFCPacketHeader *hdr);

/*!
	@function AFCSendData
	@discussion Send arbitrary data on a connection.
 */
extern AFCDomainError
AFCSendData(AFCConnectionRef conn, const void *data, size_t dataLength);

/*!
	@function AFCFlushData
	@discussion Write any data buffered on a connection.  Use after you are done sending all the data on the connection.
 */
extern AFCDomainError
AFCFlushData(AFCConnectionRef conn);

/*!
	@function AFCSendPacket
	@discussion Send an entire packet.
	You must have called AFCHeaderInit on the packet header.
 */
extern AFCDomainError
AFCSendPacket(AFCConnectionRef conn, const AFCPacketHeader *hdr, const void *body, size_t bodyLength);

/*!
	@function AFCConnectionSetFatalError
	@discussion
 A fatal error means that further communication
 on the connection is impossible and the connection
 should be closed.
 */
extern AFCDomainError
AFCConnectionSetFatalError(AFCConnectionRef conn,
                           AFCDomainError error);

extern AFCDomainError
AFCSendDataPacket(AFCConnectionRef conn, const AFCPacketHeader *origHdr, const uint8_t *data, size_t data_len);


#ifdef __cplusplus
}
#endif

#endif /* __AFC_RAW_H__ */
