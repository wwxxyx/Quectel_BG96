/* Copyright 2006-2010, Apple Inc.  All rights reserved. */

/*!
	@header AFCProtocol.h
	@abstract AFC protocol definitions.
	@discussion
 
 The Apple File Conduit (AFC) protocol is a simple protocol for transferring files and other data between two endpoints.
 
 An endpoint is a computer or other device that wants to transfer data to and from another host or device.
 By convention one endpoint acts as a client, and the other endpoint acts as a server.  Determining which end is the client
 and which is the server is outside the scope of this version of the AFC protocol.
 
 The connection between two endpoints is over a reliable stream transport, such as a TCP stream connection.
 
 Communication is in the form of packets, which have a header and a body.
 There is a standard minimum header, but a header may contain additional packet-specific structured data at the end.
 The packet body contains unstructured data, and may be empty, in which case the header size and packet size are the same.
 
 Packets are sent from the client to the server, and the server sends one reply per client packet.
 
 The standard packet header is comprised of signature, packet length, header length, packet ID, and packet type fields.
 
 The signature field identifies the packet as an AFC packet and determines whether the data is sent in big-endian or little-endian format.
 
 The packet length field indicates the total length of the packet, including the header and data.
 This must always be at least the length of the required header fields.  If the data portion of the packet is empty,
 then the packet length is equal to the header length.
 
 The header length field is the length of the header portion of the packet only. 
 The length of the data portion is the packet length minus the header length.
 
 The packet ID is a serial number identifying this packet within the stream of packets sent in one direction over the connection stream.
 It is intended that the serial number uniquely identify each packet in the connection, although some client implementations
 may not support this field.  When replying to a packet, the server must send its reply with the same packet ID as the request.
 
 The packet type identifies the message sent, which determines what headers and data, if any, are present in the message.
 
 Packet data is sent over the connection in either little-endian or big-endian format;
 each side may send data in whatever format is most convenient, and the other side is responsible for swapping the data if necessary.
 Each packet must use a consistent endianness.
 The host may examine the packet signature to determine whether the packet is sent in big-endian or little-endian format.

 The structure definitions in this document describe the sizes and composition of the fields for each packet type.
 
 */

#ifndef __AFC_PROTOCOL_H__
#define __AFC_PROTOCOL_H__

#include <limits.h>
#include <stdio.h>
#include <time.h>

#ifdef _MSC_VER
/*warning C4200: nonstandard extension used : zero-sized array in ...*/
#pragma warning( push )
#pragma warning( disable : 4200 )
#endif

#define kAFCDefaultPort 1157

#pragma pack(1)

/*!
	@struct AFCPacketHeader
	@abstract An AFC request packet header.
	@field	Signature	This will be set to kAFCPacketSignature or kAFCPacketSwappedSignature.
	@field	PacketLength The length in bytes of the entire packet, including the header and body.
	@field	HeaderLength The length in bytes of only the header of the packet.
	@field	PacketID A serial number used to distinguish sequential packets on the stream.
	@field	PacketType The type of the packet, used to determine how to process the packet.
 */

typedef struct __AFCPacketHeader {
    uint64_t Signature;
    uint64_t PacketLength; // Includes entire header
    uint64_t HeaderLength; // Includes this prefix
    uint64_t PacketID;     // Packet serial number
    uint64_t PacketType;
} AFCPacketHeader;

/*!
 @const kAFCPacketSignature The AFC packet signature in native format for the current host machine.
 */
#define kAFCPacketSignature 0x4141504c36414643ULL          /* AAPL6AFC */
/*!
  @const kAFCPacketSwappedSignature The AFC packet signature in byte-swapped format for the current host machine.
 */
#define kAFCPacketSwappedSignature 0x434641364c504141ULL

/*!
    @enum AFC protocol error codes
    @discussion
 AFC protocol error codes are only used inside AFC request packets.
 They are never returned by client code.
 Client error codes are defined in AFCClient.h.
    @constant kAFCSuccess			Successful result.
	@constant kAFCUndefinedError	An undefined error.  This is used when a more specific error is not available.
	@constant kAFCBadHeaderError	A packet was received with a bad header.
	@constant kAFCNoResourcesError	There was a resource shortage, such as a lack of memory.
	@constant kAFCReadError			A read error occurred.
	@constant kAFCWriteError		A write error occurred.
	@constant kAFCUnknownPacketError An unknown packet type was received.
	@constant kAFCInvalidArgumentError	An invalid argument was supplied.
	@constant kAFCNotFoundError		The resource was not found.
	@constant kAFCIsDirectoryError	An operation was requested that is illegal on a directory.
	@constant kAFCPermissionError	You do not have permission to perform this operation.
	@constant kAFCNotConnectedError	The connection was closed.
	@constant kAFCTimeOutError		The operation timed out.
	@constant kAFCOverrunError		More data was available than was asked for.
	@constant kAFCEOFError			End of file reached.
	@constant kAFCUnsupportedError	The requested operation is not supported.
	@constant kAFCFileExistsError	The specified file exists.
	@constant kAFCBusyError			The requested resource is busy.
	@constant kAFCNoSpaceError		No space is available to complete the requested operation.
	@constant kAFCWouldBlockError	The requested operation would block.
	@constant kAFCInputOutputError	An input/output error occurred.
	@constant kAFCInterruptedError	The requested operation was interrupted.
	@constant kAFCInProgressError    The requested operation is in progress.
	@constant kAFCInternalError		An internal processing error occurred.
 */
enum {
    kAFCSuccess = 0,
    kAFCUndefinedError =		1,
    kAFCBadHeaderError =		2,
    kAFCNoResourcesError =		3,
    kAFCReadError =				4,
    kAFCWriteError =			5,
    kAFCUnknownPacketError =	6,
    kAFCInvalidArgumentError =	7,
    kAFCNotFoundError =			8,
    kAFCIsDirectoryError =		9,
    kAFCPermissionError =		10,
    kAFCNotConnectedError =		11,
    kAFCTimeOutError =			12,
    kAFCOverrunError =			13,
    kAFCEOFError =				14,
    kAFCUnsupportedError =		15,
    kAFCFileExistsError =		16,
    kAFCBusyError =				17,
    kAFCNoSpaceError =			18,
    kAFCWouldBlockError =		19,
    kAFCInputOutputError =		20,
    kAFCInterruptedError =		21,
    kAFCInProgressError =       22,
	kAFCInternalError =			23
};

/*!
 @group Packet types
 */

/*!
 @const kAFCPacketTypeStatus The packet type number for an AFC status request packet.
 */
#define kAFCPacketTypeStatus 0x1

/*!
  @struct AFCPacketHeaderStatus
 @field Header The standard AFC packet header.
 @field Status An AFC status code representing the status of the requested operation.
 @discussion
 A status packet is used to report back the status of a previous request.
 It may be used after any failed request to report back an error status instead
 of the expected reply packet.
 */
typedef struct __AFCPacketHeaderStatus {
	AFCPacketHeader Header;
	uint64_t	Status;
} AFCPacketHeaderStatus;

/*!
 @const kAFCPacketTypeData The packet type number for an AFC data packet.
 */
#define kAFCPacketTypeData 0x2

/*!
 @struct AFCPacketHeaderData
 @field Header The standard AFC packet header.
 @discussion
 A data packet is used as a response to packets that return data, such as a file read.
 If the request is successful, then a data packet will be returned in response with the
 data in the data portion of the packet; if the request fails,
 a status packet with the error code will be returned instead.
 */
typedef struct __AFCPacketHeaderData {
	AFCPacketHeader	Header;
} AFCPacketHeaderData;

/*!
 @const kAFCPacketTypeReadDirectory The packet type number for an AFC read directory packet.
 */
#define kAFCPacketTypeReadDirectory 0x3

/*!
 @struct AFCPacketHeaderReadDirectory
 @field Header The standard AFC packet header.
 @field Path A NUL-terminated POSIX path string in UTF-8 format.
 @discussion
 On success, the read directory request returns a data packet with a series of NUL-terminated
 UTF-8 strings; each string is the name of a directory entry from the directory named in the request.
*/
typedef struct __AFCPacketHeaderReadDirectory {
	AFCPacketHeader	Header;
	uint8_t			Path[];
} AFCPacketHeaderReadDirectory;

/*!
 @const kAFCPacketTypeReadFile The packet type number for an AFC read file packet.
 */
#define kAFCPacketTypeReadFile 0x4

/*!
 @struct AFCPacketHeaderReadFile
 @discussion Reads data from a file.  The data will be returned in a data packet.
 @field Header The standard AFC packet header.
 @field Offset Offset in bytes from start of file to begin reading.
 @field Length Length in bytes of data to read from file.
 @field Path A NUL-terminated POSIX path string in UTF-8 format.
 */
typedef struct __AFCPacketHeaderReadFile {
    AFCPacketHeader  Header;
    uint64_t        Offset;
    uint64_t        Length;
    uint8_t         Path[];
} AFCPacketHeaderReadFile;

/*!
 @const kAFCPacketTypeWriteFile The packet type number for an AFC write file packet.
 */
#define kAFCPacketTypeWriteFile 0x5

/*!
 @struct AFCPacketHeaderWriteFile
 @discussion Write an entire file.  Any existing file will be replaced.
 The data to be written is contained in the data portion of the packet.
 @field Header The standard AFC packet header.
 @field Path A NUL-terminated POSIX path string in UTF-8 format.
 */
typedef struct __AFCPacketHeaderWriteFile {
    AFCPacketHeader Header;
    uint8_t         Path[];
} AFCPacketHeaderWriteFile;

/*!
 @const kAFCPacketTypeWritePart The packet type number for an AFC partial file write packet.
 */
#define kAFCPacketTypeWritePart 0x6

/*!
 @struct AFCPacketHeaderWritePart
 @discussion Write part of an existing file.
 The data to be written is in the data portion of the packet.
 @field Header The standard AFC packet header.
 @field Offset The offset in bytes from the beginning of an existing file to begin writing.
 @field Path A NUL-terminated POSIX path string in UTF-8 format.
 */

typedef struct __AFCPacketHeaderWritePart {
    AFCPacketHeader Header;
    uint64_t        Offset;
    uint8_t         Path[];
} AFCPacketHeaderWritePart;

/*!
 @const kAFCPacketTypeTruncFile The packet type number for an AFC file truncation packet.
 */
#define kAFCPacketTypeTruncFile 0x7

/*!
 @struct AFCPacketHeaderTruncFile
 @field Header The standard AFC packet header.
 @field Length The length in bytes of the new file size.
 @field Path A NUL-terminated POSIX path string in UTF-8 format.
 @discussion
 On success, the file will be truncated to the length specified, or will be extended with zeros
 if the existing file was smaller than the requested length.
 */
typedef struct __AFCPacketHeaderTruncFile {
    AFCPacketHeader Header;
    uint64_t        Length;
    uint8_t         Path[];
} AFCPacketHeaderTruncFile;

/*!
 @const kAFCPacketTypeRemovePath The packet type number for an AFC remove path packet.
 */
#define kAFCPacketTypeRemovePath 0x8

/*!
 @struct AFCPacketHeaderRemovePath
 @field Header The standard AFC packet header.
 @field Path A NUL-terminated POSIX path string in UTF-8 format.
 @discussion
 If the requested path is a file, it will be removed.  If the requested path
 is a directory, then if the directory is empty, it will be removed.
 If the directory is not empty, an error will be returned.
 */
typedef struct __AFCPacketHeaderRemovePath {
    AFCPacketHeader Header;
    uint8_t         Path[];
} AFCPacketHeaderRemovePath;

/*!
 @const kAFCPacketTypeMakeDirectory The packet type number for an AFC make directory packet.
 */
#define kAFCPacketTypeMakeDirectory 0x9

/*!
 @struct AFCPacketHeaderMakeDirectory
 @field Header The standard AFC packet header.
 @field Path A NUL-terminated POSIX path string in UTF-8 format.
 */
typedef struct __AFCPacketHeaderMakeDirectory {
    AFCPacketHeader Header;
    uint8_t         Path[];
} AFCPacketHeaderMakeDirectory;

/*!
 @const kAFCPacketTypeGetFileInfo The packet type number for an AFC get file info packet.
 */
#define kAFCPacketTypeGetFileInfo 0xA

/*!
 @struct AFCPacketHeaderGetFileInfo
 @field Header The standard AFC packet header.
 @field Path A NUL-terminated POSIX path string in UTF-8 format.
 @discussion
 On success, a data packet is returned with a series of key/value pairs.
 Each key and value is a NUL-terminated UTF-8 string; the pairs are concatenated
 in the data packet.  Numeric values are presented as decimal strings.
 
 <ul>
 <li><b>st_size</b>	Size of the file in bytes
 <li><b>st_blocks</b>	Number of blocks allocated for file
 <li><b>st_nlink</b>	Number of hard links to the file
 <li><b>st_ifmt</b>	Format information about the type of the file.  Value contains one or more of the following entries separated by commas:
 </ul>
 <ul>
 <li><b>S_IFREG</b>	Regular file
 <li><b>S_IFDIR</b>	Directory
 <li><b>S_IFBLK</b>	Block device
 <li><b>S_IFCHR</b>	Character device
 <li><b>S_IFIFO</b>	Fifo
 <li><b>S_IFLNK</b>	Symbolic link
 <li><b>S_IFSOCK</b>	Socket
 </ul>
 
 Client implementations should be tolerant of missing or new key values.
 */
typedef struct __AFCPacketHeaderGetFileInfo {
	AFCPacketHeader	Header;
	uint8_t			Path[];
} AFCPacketHeaderGetFileInfo;

/*!
 @const kAFCPacketTypeGetDeviceInfo The packet type number for an AFC get device info packet.
 */
#define kAFCPacketTypeGetDeviceInfo 0xB

/*!
 @typedef AFCPacketHeaderGetDeviceInfo
 @discussion
 On success, a data packet is returned with a series of key/value pairs.
 Each key and value is a NUL-terminated UTF-8 string; the pairs are concatenated
 in the data packet.
 
 Possible key values are:
 
 <ul>
 <li><b>Model</b> Model string describing the device.
 <li><b>FSTotalBytes</b>	Capacity of the device filesystem in bytes.
 <li><b>FSFreeBytes</b>	Free bytes in the device filesystem.
 <li><b>FSBlockSize</b> Device filesystem block size. 
 </ul>
 
 Client implementations should be tolerant of missing or new key values.
 */
typedef struct __AFCPacketHeaderGetDeviceInfo {
    AFCPacketHeader	Header;
} AFCPacketHeaderGetDeviceInfo;

/*!
 @const kAFCPacketTypeWriteFileAtomic The packet type number for an AFC atomic file write packet.
 */
#define kAFCPacketTypeWriteFileAtomic 0xC

/*!
 @struct AFCPacketHeaderWriteFileAtomic
 @discussion 
 Writes the entire file atomically.  Any existing file will be replaced safely.
 If the write fails, the existing file will not be disturbed.
 @field Header The standard AFC packet header.
 @field Path A NUL-terminated POSIX path string in UTF-8 format.
 */
typedef struct __AFCPacketHeaderWriteFileAtomic {
    AFCPacketHeader Header;
    uint8_t         Path[];
} AFCPacketHeaderWriteFileAtomic;

/*!
 @const kAFCPacketTypeFileRefOpen The packet type number for an AFC file ref open packet.
 */
#define kAFCPacketTypeFileRefOpen 0xD

/*!
 @struct AFCPacketHeaderFileRefOpen
 @field Header The standard AFC packet header.
 @field Mode The mode for the opened file.  See @link kAFCOpenReadOnly Open mode values for AFCPacketHeaderFileRefOpen @/link.
 @field Path A NUL-terminated POSIX path string in UTF-8 format.
 @discussion
 A file reference exists on the server and contains state including the current position in the file.
 File references exist until closed by the FileRefClose request.  They are closed when the
 connection terminates.
 
 Upon success, the reply to this packet is a AFCPacketHeaderFileRefOpenResult packet.
*/ 
typedef struct __AFCPacketHeaderFileRefOpen {
    AFCPacketHeader Header;
    uint64_t        Mode;
    uint8_t         Path[];
} AFCPacketHeaderFileRefOpen;

/*!
 @enum Open mode values for AFCPacketHeaderFileRefOpen
 @discussion
 @const kAFCOpenReadOnly	The file is opened for reading only.  If the file does not exist, it is an error.
 @const kAFCOpenReadWrite	The file is opened for reading and writing.  If the file does not exist it is created.
 @const kAFCOpenWriteTruncate	The file is opened for writing only, and all data in the file is removed if it exists.  If the file does not exist it is created.
 @const kAFCOpenWriteTruncateImmutable	The file is opened for writing only, and all data in the file is removed if it exists.  If the file does not exist it is created.
				The filesystem is hinted that the file is unlikely to change.
				
				This constant has been introduced with iOS 5.0 and is not backwards compatible;
				clients requiring support of older servers must handle the case of receiving
				@link kAFCInvalidArgumentError @/link when using this open mode,
				and should manually fall back to using @link kAFCOpenWriteTruncate @/link instead.
 @const kAFCOpenReadWriteTruncate The file is opened for reading and writing, and all data in the file is removed if it exists.  If the file does not exist it is created.
 @const kAFCOpenWriteAppend	The file is opened for writing only, and the initial write position is set at the end of the file.  If the file does not exist it is created.
 @const kAFCOpenReadWriteAppend The file is opened for reading and writing, and the initial write position is set at the end of the file.  If the file does not exist it is created.

 @availability
 @link kAFCOpenWriteTruncateImmutable @/link has been introduced with iOS 5.0 and is not backwards compatible.  See discussion above for more details.
 */
enum {
    kAFCOpenReadOnly = 1,
    kAFCOpenReadWrite = 2,
    kAFCOpenWriteTruncate = 3,
    kAFCOpenWriteTruncateImmutable = 7,
    kAFCOpenReadWriteTruncate = 4,
    kAFCOpenWriteAppend = 5,
    kAFCOpenReadWriteAppend = 6
};

/*!
 @const kAFCPacketTypeFileRefOpenResult The packet type number for an AFC file ref open result packet.
 */
#define kAFCPacketTypeFileRefOpenResult 0xE

/*!
 @struct AFCPacketHeaderFileRefOpenResult
 @field Header The standard AFC packet header.
 @field FileRef The newly created file reference number.
 */
typedef struct __AFCPacketHeaderFileRefOpenResult {
    AFCPacketHeader Header;
    uint64_t        FileRef;
} AFCPacketHeaderFileRefOpenResult;

/*!
 @const kAFCPacketTypeFileRefRead The packet type number for an AFC file ref read packet.
 */
#define kAFCPacketTypeFileRefRead 0xF

/*!
 @struct AFCPacketHeaderFileRefRead
 @field Header The standard AFC packet header.
 @field FileRef The file reference on which to operate.
 @field Length The length of data requested to read from the file.
 @discussion
 Reads the requested amount of data from the specified file at the current offset.
 On success, the reply is a data packet.  The reply may contain less data than was requested.
 */
typedef struct __AFCPacketHeaderFileRefRead {
    AFCPacketHeader Header;
    uint64_t        FileRef;
    uint64_t        Length;
} AFCPacketHeaderFileRefRead;

/*!
 @const kAFCPacketTypeFileRefWrite The packet type number for an AFC file ref write packet.
 */
#define kAFCPacketTypeFileRefWrite 0x10

/*!
 @struct AFCPacketHeaderFileRefWrite
 @field Header The standard AFC packet header.
 @field FileRef The file reference on which to operate.
 @discussion
 Writes data to the specified open file at the current offset.
 The data to write is contained in the data portion of the packet.
 */
typedef struct __AFCPacketHeaderFileRefWrite {
    AFCPacketHeader Header;
    uint64_t      FileRef;
} AFCPacketHeaderFileRefWrite;

/*!
 @const kAFCPacketTypeFileRefSeek The packet type number for an AFC file ref seek packet.
 */
#define kAFCPacketTypeFileRefSeek 0x11

/*!
 @struct AFCPacketHeaderFileRefSeek
 @field Header The standard AFC packet header.
 @field FileRef The file reference on which to operate.
 @field Whence How to apply the offset.  See @link kAFCSeekSet Whence values for AFCPacketHeaderFileRefSeek @/link.
 @field Offset The offset in bytes to apply.
 */
typedef struct __AFCPacketHeaderFileRefSeek {
    AFCPacketHeader Header;
    uint64_t      FileRef;
    uint64_t        Whence;
    uint64_t        Offset;
} AFCPacketHeaderFileRefSeek;

/*!
 @enum Whence values for AFCPacketHeaderFileRefSeek
 @discussion
 @const kAFCSeekSet	The offset is measured from the beginning of the file.
 @const kAFCSeekCurrent The offset is measured from the current position of the file.
 @const kAFCSeekEnd The offset is measured backwards from the end of the file.
 */
enum {
    kAFCSeekSet         = 0,
    kAFCSeekCurrent     = 1,
    kAFCSeekEnd         = 2
};

/*!
 @const kAFCPacketTypeFileRefTell The packet type number for an AFC file ref tell packet.
 */
#define kAFCPacketTypeFileRefTell 0x12

/*!
 @struct AFCPacketHeaderFileRefTell
 @field Header The standard AFC packet header.
 @field FileRef The file reference on which to operate.
 @discussion
 On success, the reply is an AFCPacketHeaderFileRefTellResult packet containing
 the current offset of the specified file reference.
 */
typedef struct __AFCPacketHeaderFileRefTell {
    AFCPacketHeader Header;
    uint64_t      FileRef;
} AFCPacketHeaderFileRefTell;

/*!
 @const kAFCPacketTypeFileRefTellResult The packet type number for an AFC file ref tell result packet.
 */
#define kAFCPacketTypeFileRefTellResult 0x13

/*!
 @struct AFCPacketHeaderFileRefTellResult
 @field Header The standard AFC packet header.
 @field Offset The current offset of the requested file reference.
 */
typedef struct __AFCPacketHeaderFileRefTellResult {
    AFCPacketHeader Header;
    uint64_t        Offset;
} AFCPacketHeaderFileRefTellResult;

/*!
 @const kAFCPacketTypeFileRefClose The packet type number for an AFC file ref close packet.
 */
#define kAFCPacketTypeFileRefClose 0x14

/*!
 @struct AFCPacketHeaderFileRefClose
 @field Header The standard AFC packet header.
 @field FileRef The file reference on which to operate.
 @discussion
 On success, the specified file reference is closed and its resources are freed.
 Any pending writes to the file are flushed to the storage medium before
 the status reply is sent.
 */
typedef struct __AFCPacketHeaderFileRefClose {
    AFCPacketHeader Header;
    uint64_t      FileRef;
} AFCPacketHeaderFileRefClose;

/*!
 @const kAFCPacketTypeFileRefSetFileSize The packet type number for an AFC file ref set file size packet.
 */
#define kAFCPacketTypeFileRefSetFileSize 0x15

/*!
 @struct AFCPacketHeaderFileRefSetFileSize
 @field Header The standard AFC packet header.
 @field FileRef The file reference on which to operate.
 @field FileSize The new size of the file in bytes.
 @discussion
 Sets the size of the opened file to the requested value.  If the file is larger than the requested size,
 it is truncated; if it is smaller, it is extended with zeroes.
 */
typedef struct __AFCPacketHeaderFileRefSetFileSize {
    AFCPacketHeader Header;
    uint64_t      FileRef;
    uint64_t        FileSize;
} AFCPacketHeaderFileRefSetFileSize;

/*!
 @const kAFCPacketTypeGetConnectionInfo The packet type number for an AFC get connection info packet.
 */
#define kAFCPacketTypeGetConnectionInfo 0x16

/*!
 @typedef AFCPacketHeaderGetConnectionInfo
 @discussion
 
 */
typedef struct __AFCPacketHeaderGetConnectionInfo {
    AFCPacketHeader	Header;
} AFCPacketHeaderGetConnectionInfo;

/*!
 @const kAFCPacketTypeSetConnectionOptions The packet type number for an AFC set connection options packet.
 */
#define kAFCPacketTypeSetConnectionOptions 0x17

/*!
 @typedef AFCPacketHeaderSetConnectionOptions
 @discussion
 */
typedef struct __AFCPacketHeaderSetConnectionOptions {
    AFCPacketHeader	Header;
} AFCPacketHeaderSetConnectionOptions;

/*!
 @const kAFCPacketTypeRenamePath The packet type number for an AFC rename path packet.
 */
#define kAFCPacketTypeRenamePath 0x18

/*!
 @struct AFCPacketHeaderRenamePath
 @field Header The standard AFC packet header.
 @field SourcePath The NUL-terminated UTF-8 POSIX path of the file or directory to rename.
 @discussion
 Renames the specified path to a new path name.  The target path is a NUL-terminated UTF-8 POSIX path
 which follows immediately after the NUL of the SourcePath field in the header.
 */
typedef struct __AFCPacketHeaderRenamePath {
    AFCPacketHeader Header;
    uint8_t         SourcePath[];
    // followed by  DestPath[];
} AFCPacketHeaderRenamePath;

/*!
 @struct AFCPacketHeaderSetBlockSize
 @field Header The standard AFC packet header.
 @field BlockSize The size in bytes of the I/O block to use.
 @discussion
 This packet structure is used to set both the filesystem block I/O size and
 the communications socket I/O size.
*/
typedef struct __AFCPacketHeaderSetBlockSize {
    AFCPacketHeader Header;
    uint64_t        BlockSize;
} AFCPacketHeaderSetBlockSize;

/*!
 @const kAFCPacketTypeSetFSBlockSize The packet type number for an AFC set filesystem block size packet.
 */
#define kAFCPacketTypeSetFSBlockSize     0x19

/*!
 @typedef AFCPacketHeaderSetFSBlockSize
 @discussion
 Sets the block size used for file I/O operations on the server.
 The server may use read and write sizes smaller than this size.
 */
typedef AFCPacketHeaderSetBlockSize AFCPacketHeaderSetFSBlockSize;

/*!
 @const kAFCPacketTypeSetSocketBlockSize The packet type number for an AFC set socket block size packet.
 */
#define kAFCPacketTypeSetSocketBlockSize 0x1A

/*!
 @typedef AFCPacketHeaderSetSocketBlockSize
 @discussion
 Sets the block size used for socket I/O operations on the server when communicating with the client.
 The server may use read and write sizes smaller than this size.
 */
typedef AFCPacketHeaderSetBlockSize AFCPacketHeaderSetSocketBlockSize;

/*!
 @enum Lock operation values for AFCPacketHeaderFileRefLock
 @discussion
 Multiple options may be ORed together.  It is recommended to always use the kAFCFileRefLockNonBlocking option.
 @const kAFCFileRefLockShared	Multiple shared locks may be taken on a file.
 @const kAFCFileRefLockExclusive	Only one exclusive lock may be taken.  A shared lock may be upgraded to exclusive.
 @const kAFCFileRefLockNonBlocking	If this is present, the operation will return with an error if the lock cannot be taken.
 @const kAFCFileRefLockUnlock	Unlocks a previously locked file.
 */
enum {
// Windows has no notion of shared file locks
    kAFCFileRefLockShared         = 1,
    kAFCFileRefLockExclusive      = 2,
    kAFCFileRefLockNonBlocking    = 4,
	kAFCFileRefLockUnlock         = 8
};

/*!
 @const kAFCPacketTypeFileRefLock The packet type number for an AFC file ref lock packet.
 */
#define kAFCPacketTypeFileRefLock 0x1B

/*!
 @struct AFCPacketHeaderFileRefLock
 @field Header The standard AFC packet header.
 @field FileRef The file reference on which to operate.
 @field LockOperation The lock operation to perform.  See @link kAFCFileRefLockShared Lock operation values for AFCPacketHeaderFileRefLock @/link.
 */
typedef struct __AFCPacketHeaderFileRefLock {
	AFCPacketHeader Header;
	uint64_t		FileRef;
	uint64_t		LockOperation;
} AFCPacketHeaderFileRefLock;

/*!
 @enum Link types for AFCPacketHeaderMakeLink
 @discussion
 @const kAFCFileLinkHard  Create a hard link, which shares the underlying resources with the target object.
 @const kAFCFileLinkSymbolic  Create a symbolic link, which is a reference to the path of the target object.  Not supported on all platforms.
 */
enum {
	kAFCFileLinkHard			= 1,
	kAFCFileLinkSymbolic		= 2
};

/*!
 @const kAFCPacketTypeMakeLink The packet type number for an AFC make link packet.
 */
#define kAFCPacketTypeMakeLink 0x1C

/*!
 @struct AFCPacketHeaderMakeLink
 @field Header The standard AFC packet header.
 @field LinkType The type of link to create.  See @link kAFCFileLinkHard Link types for AFCPacketHeaderMakeLink @/link.
 @field SourcePath The NUL-terminated UTF-8 POSIX path of the source file for the link.
 @discussion
 Creates a link to another file.  The type of link is specified by the LinkType field; either a hard link or a symbolic link
 can be specified.  The destination file path is a NUL-terminated UTF-8 POSIX path immediately following the NUL of the source path.
 */
typedef struct __AFCPacketHeaderMakeLink {
    AFCPacketHeader Header;
    uint64_t		LinkType;
    uint8_t         SourcePath[];
    // followed by  DestPath[];
} AFCPacketHeaderMakeLink;

/*!
 @const kAFCPacketTypeGetFileHash The packet type number for an AFC get file hash packet.
 */
#define kAFCPacketTypeGetFileHash 0x1D

/*!
 @struct AFCPacketHeaderGetFileHash
 @field Header The standard AFC packet header.
 @field Path The NUL-terminated UTF-8 POSIX path of the file whose hash will be computed.
 @discussion
 Computes the SHA-1 hash of the specified file and returns it in a data packet.
 */
typedef struct __AFCPacketHeaderGetFileHash {
	AFCPacketHeader Header;
	uint8_t			Path[];
} AFCPacketHeaderGetFileHash;

/*!
 @const kAFCPacketTypeSetModTime The packet type number for an AFC packet to set the modification time on a path.
 */
#define kAFCPacketTypeSetModTime 0x1E

/*!
 @struct AFCPacketHeaderSetModTime
 @field Header The standard AFC packet header.
 @field ModTimeNanoseconds The file modification time to set on the file, in nanoseconds from the reference time,
 @field TargetPath The file path on which to operate.
 which is the value of time in seconds since 0 hours, 0 minutes, 0 seconds, January 1, 1970,
 Coordinated Universal Time, without including leap seconds.
 */
typedef struct __AFCPacketHeaderSetModTime {
    AFCPacketHeader Header;
	uint64_t		ModTimeNanoseconds;
	uint8_t			TargetPath[];
} AFCPacketHeaderSetModTime;


/*!
 @const kAFCPacketTypeGetFileHashWithRange The packet type number for an AFC get file hash packet.
 */
#define kAFCPacketTypeGetFileHashWithRange 0x1F

/*!
 @struct AFCPacketHeaderGetFileHashWithRange
 @field Header The standard AFC packet header.
 @field Path The NUL-terminated UTF-8 POSIX path of the file whose hash will be computed.
 @field RangeStart The start of the range to hash.
 @field RangeLength The length of the range to hash.
 @discussion
 Computes the SHA-1 hash of the specified file and returns it in a data packet.
 */
typedef struct __AFCPacketHeaderGetFileHashWithRange {
	AFCPacketHeader Header;
	uint64_t		RangeStart;
	uint64_t		RangeLength;
	uint8_t			Path[];
} AFCPacketHeaderGetFileHashWithRange;

/*!
 @const kAFCPacketTypeFileRefSetImmutableHint  The packet type number for an AFC set file ref immutable hint packet.
 */
#define kAFCPacketTypeFileRefSetImmutableHint 0x20

/*!
 @struct AFCPacketHeaderFileRefSetImmutableHint
 @field Header The standard AFC packet header.
 @field FileRef The file ref to operate on.
 @field Immutable Whether to hint that the file is immutable or not.
 */

typedef struct __AFCPacketHeaderFileRefSetImmutableHint {
    AFCPacketHeader Header;
    uint64_t        FileRef;
    uint64_t        Immutable;
} AFCPacketHeaderFileRefSetImmutableHint;

/*!
 */
#define kAFCPacketTypeGetSizeOfPathContents 0x21

/*!
 */

/* Option: get size of all files, not just regular files. */
enum {
  kAFCGetSizeOfPathContentsAllFilesOption = 1
};

/*!
@struct AFCPacketHeaderGetSizeOfPathContents
@field Header The standard AFC packet header.
@field Options Options on how to compute the size.
@field Path The path on the device which will have its size computed.
 @discussion If Path is a file, then its size will be returned.  If Path is a directory, then all regular files in the directory and in any directories below it will
 have their sizes added to the total size returned.
 If Option is set to kAFCGetSizeOfPathContentsAllFilesOption, then the size of any directories themselves and any special files will be included in the size.
 */

typedef struct __AFCPacketHeaderGetSizeOfPathContents {
	AFCPacketHeader Header;
	uint64_t		Options;
	uint8_t			Path[];
} AFCPacketHeaderGetSizeOfPathContents;

/*!
 */
#define kAFCPacketTypeRemovePathAndContents 0x22

/*!
 @struct AFCPacketHeaderRemovePathAndContents
 @field Header The standard AFC packet header.
 @field Path The path to remove.
 @discussion Removes Path, and if it is a directory, all files and directories underneath it.
 */

typedef struct __AFCPacketHeaderRemovePathAndContents {
	AFCPacketHeader Header;
	uint8_t			Path[];
} AFCPacketHeaderRemovePathAndContents;


/*
 * Directory enumerators are not supported. As of Innsbruck (iOS 7.0)
 * all support is removed. This remains commented out so that archaeologists
 * in a distant future will know what happened to AFCPacket numbers
 * 0x23 through 0x26. The #defines are retained so that __gAFCPacketTypeStrings
 * and other things that depend on the numbers aren't disturbed...
 */

#define kAFCPacketTypeDirectoryEnumeratorRefOpen 0x23

/*!
 @struct AFCPacketHeaderDirectoryEnumeratorOpen
 @field Header The standard AFC packet header.
 @field Path String containing path to directory to enumerate.
 @discussion Creates an enumerator that will return information
    about the contents of the directory at Path.
 
typedef struct __AFCPacketHeaderDirectoryEnumeratorRefOpen {
    AFCPacketHeader Header;
    uint8_t         Path[];
} AFCPacketHeaderDirectoryEnumeratorRefOpen;
*/

/*!
 @const kAFCPacketTypeDirectoryEnumeratorRefOpenResult The packet type number for an AFC Directory Enumerator ref open result packet.
 */
#define kAFCPacketTypeDirectoryEnumeratorRefOpenResult 0x24 

/*!
 @struct AFCPacketTypeDirectoryEnumeratorRefOpenResult
 @field Header The standard AFC packet header.
 @field DirectoryEnumeratorRefIndex Index of the newly created directory enumerator reference.
 
typedef struct __AFCPacketHeaderDirectoryEnumeratorRefOpenResult {
    AFCPacketHeader Header;
    uint64_t        DirectoryEnumeratorRefIndex;
} AFCPacketHeaderDirectoryEnumeratorRefOpenResult;
*/

/*!
 @const kAFCPacketTypeDirectoryEnumeratorRefRead The packet type number for an AFC Directory Enumerator ref read packet.
 */
#define kAFCPacketTypeDirectoryEnumeratorRefRead 0x25

/*!
 @struct AFCPacketHeaderDirectoryEnumeratorRefRead
 @field Header The standard AFC packet header.
 @field DirectoryEnumeratorRefIndex Index of the Directory Enumerator reference on which to operate.
 @discussion
 Returns the next batch of directory information from the enumerator as a CFDictionary.
 
typedef struct __AFCPacketHeaderDirectoryEnumeratorRefRead {
    AFCPacketHeader Header;
    uint64_t        DirectoryEnumeratorRefIndex;
} AFCPacketHeaderDirectoryEnumeratorRefRead;
 */

/*!
 @const kAFCPacketTypeDirectoryEnumeratorRefClose The packet type number for an AFC Directory Enumerator ref close packet.
 */
#define kAFCPacketTypeDirectoryEnumeratorRefClose 0x26

/*!
 @struct AFCPacketHeaderDirectoryEnumeratorRefClose
 @field Header The standard AFC packet header.
 @field DirectoryEnumeratorRefIndex Index of the Directory Enumerator reference on which to operate.
 @discussion
 Closes the directory enumerator ref and reclaims its resources.

typedef struct __AFCPacketHeaderDirectoryEnumeratorRefClose {
    AFCPacketHeader Header;
    uint64_t        DirectoryEnumeratorRefIndex;
} AFCPacketHeaderDirectoryEnumeratorRefClose;
*/


/*!
 @const kAFCPacketTypeFileRefReadWithOffset The packet type number for an AFC file ref read with offset packet.
 */
#define kAFCPacketTypeFileRefReadWithOffset 0x27

/*!
 @struct AFCPacketHeaderFileRefReadWithOffset
 @field Header The standard AFC packet header.
 @field FileRef The file reference on which to operate.
 @field Length The length of data requested to read from the file.
 @field Offset The offset in bytes from the beginning of the file from which to start the read.
 @discussion
 Reads the requested amount of data from the specified file at the specified offset.  Does not affect the current offset of the file descriptor.
 On success, the reply is a data packet.  The reply may contain less data than was requested.
 */
typedef struct __AFCPacketHeaderFileRefReadWithOffset {
    AFCPacketHeader Header;
    uint64_t        FileRef;
    uint64_t        Length;
    uint64_t        Offset;
} AFCPacketHeaderFileRefReadWithOffset;

/*!
 @const kAFCPacketTypeFileRefWrite The packet type number for an AFC file ref write packet.
 */
#define kAFCPacketTypeFileRefWriteWithOffset 0x28

/*!
 @struct AFCPacketHeaderFileRefWriteWithOffset
 @field Header The standard AFC packet header.
 @field FileRef The file reference on which to operate.
 @field Offset The offset from the beginning of the file in bytes.
 @discussion
 Writes data to the specified open file at the specified offset.
 The data to write is contained in the data portion of the packet.
 Does not change the current offset of the file descriptor.
 */
typedef struct __AFCPacketHeaderFileRefWriteWithOffset {
    AFCPacketHeader Header;
    uint64_t      FileRef;
    uint64_t      Offset;
} AFCPacketHeaderFileRefWriteWithOffset;


#define kAFCPacketTypeMaximum kAFCPacketTypeFileRefWriteWithOffset

#pragma pack()

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif /* ! __AFC_PROTOCOL_H__ */
