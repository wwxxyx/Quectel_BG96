//
//  IDBUtils.h
//  IDBusKit
//
//  Created by Cody Brimhall on 4/19/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

/*!
 @header     IDBUtils
 @discussion This header file defines functions that can be used to work with IDBPacket instances and packet info structs.
 */

#import <IDBusKit/IDBPacket.h>
#import <IDBusKit/IDBTypes.h>

#pragma mark IDBPacket Utils

/*!
 Parses a packet for module state info. The packet to be parsed must be a IDBCommandState packet.
 
 Commands: 0x70, 0x73.
 
 @param packet The packet to parse for state info.
 @param info A pointer to the state info struct that should be updated with values from the parsed packet.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketGetStateInfo(IDBPacket *packet, IDBStateInfo *info);

/*!
 Parses a Get ID command for the host ID.
 
 Command: 0x75.
 
 @param packet The packet to parse for host ID.
 @param hostID 16-bit host identifier.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketGetIDCommand(IDBPacket *packet, UInt16 *hostID);

/*!
 Parses a packet for digital ID info. The packet to be parsed must be a IDBCommandID packet.
 
 Command 0x75.
 
 @param packet The packet to parse for ID info.
 @param info A pointer to the ID info struct that should be updated with values from the parsed packet.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketGetIDInfo(IDBPacket *packet, IDBIDInfo *info);

/*!
 Parses a packet for interface device info. The packet to be parsed must be a IDBCommandInterfaceDeviceInfo packet.
 
 Command 0x77.
 
 @param packet The packet to parse for interface device info.
 @param info A pointer to the interface device info struct that should be updated with values from the parsed packet.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketGetInterfaceDeviceInfo(IDBPacket *packet, IDBInterfaceDeviceInfo *info);

/*!
 Parses a packet for module serial number info. The packet to be parsed must be a IDBCommandInterfaceModuleSerialNumber packet.
 
 Command: 0x79.
 
 @param packet The packet to parse for module serial number info.
 @param info A pointer to the module serial number info struct that should be updated with values from the parsed packet.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketGetInterfaceModuleSerialNumberInfo(IDBPacket *packet, IDBInterfaceModuleSerialNumberInfo *info);

/*!
 Parses a packet for accessory serial number info. The packet to be parsed must be a IDBCommandAccessorySerialNumber packet.
 
 Command 0x7B.
 
 @param packet The packet to parse for accessory serial number info.
 @param info A pointer to the accessory serial number info struct that should be updated with values from the parsed packet.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketGetAccessorySerialNumberInfo(IDBPacket *packet, IDBAccessorySerialNumberInfo *info);

/*!
 Parses a ServiceModeRequest packet.
 
 Command: 0x7E.
 
 @param packet The packet to parse.
 @param caps Caps byte.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketSystemIDServiceModeRequestFields(IDBPacket *packet, UInt8 *caps);

/*!
 Parses a GetESNRequest packet.
 
 Command: 0x7D.
 
 @param packet The packet to parse.
 @param keyIndex Key Index.
 @param nonce 8 byte nonce challenge.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketSystemIDGetESNRequestFields(IDBPacket *packet, UInt8 *keyIndex, NSData **nonce);

/*!
 Parses a GetESNResponse packet.
 
 Command 0x7C.
 
 @param packet The packet to parse.
 @param keyIndex Key Index.
 @param ESN 8 byte ESN.
 @param DES 8 byte 3DES.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketSystemIDGetESNResponseFields(IDBPacket *packet, UInt8 *keyIndex, NSData **ESN, NSData **DES);

/*!
 Parses a program OTP packet.
 
 Command 0xE0.
 
 @param packet The packet to parse.
 @param VID Vendor ID.
 @param PID Product ID.
 @param ADR addr field (6 bits).
 @param DAT data byte.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketProgramOTPFields(IDBPacket *packet, UInt8 *VID, UInt8 *PID, UInt8 *ADR, UInt8 *DAT);

/*!
 Parses a program OTP Status packet.
 
 Command 0xE2.
 
 @param packet The packet to parse.
 @param VID Vendor ID.
 @param PID Product ID.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketProgramOTPStatusFields(IDBPacket *packet, UInt8 *VID, UInt8 *PID);

/*!
 Parses a program OTP Status Response packet.
 
 Command: 0xE3.
 
 @param packet The packet to parse.
 @param ADR addr field (6 bits).
 @param DAT data byte.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketProgramOTPStatusResponseFields(IDBPacket *packet, UInt8 *ADR, BOOL *SV, BOOL *PF, BOOL *AL, BOOL *PA);

/*!
 Parses a SelfTest Control Response packet.
 
 Command: 0xE5.
 
 @param packet The packet to parse.
 @param errCode The errorCode for the response.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketSelfTestControlResponseFields(IDBPacket *packet, IDBErrorCode *errCode);

/*!
 Calculate the CRC8 of the following bytes, using the generator 0x8C.
 
 Command 0x8C.
 
 @param packet The packet to parse.
 @param bytes The bytes to calculate the CRC8 of.
 @param lenght Length of the bytes passed.
 @return Returns the CRC8.
 */
UInt8 IDBCalculateCRC8(const UInt8 *bytes, NSUInteger length);

/*!
 Extracts field values from a GetAccessoryInfo packet (0x80).
 
 Command: 0x80.
 
 @param packet The packet to parse for accessory serial number info.
 @param str A pointer to the str enum describing what the contained string is.
 @param off A pointer to a UInt16 where the offset will be written to.
 @param len A pointer to a UInt16 where the length of the portion returned will be written to.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketGetAccessoryInfoFields(IDBPacket *packet, IDBAccessoryInfoString *str, UInt16 *off, UInt16 *len);

/*!
 Extracts field values from an AccessoryInfoVersion packet (0x83).
 
 Command 0x83.
 
 @param packet The packet to read from.
 @param fwmaj FW major version.
 @param fwmin FW min version.
 @param fwrev FW revision number.
 @param hwmaj Hardware major version.
 @param hwmin Hardware minor version.
 @param hwrev Hadware revision number.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketAccessoryInfoVersionFields(IDBPacket *packet,
                                         UInt8 *fwmaj, UInt8 *fwmin, UInt8 *fwrev,
                                         UInt8 *hwmaj, UInt8 *hwmin, UInt8 *hwrev);

/*!
 Extracts fields from a SystemInfo command.
 
 Command: 0x84.
 
 @param packet The packet to read from.
 @param str String type.
 @param len Length of string contained.
 @param data UTF8 data string (info string).
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketSystemInfoFields(IDBPacket *packet, IDBSystemInfoString *str, UInt16 *len, NSString **data);

/*!
 Extracts fields from a LastError command.
 
 Command: 0x9B.
 
 @param packet The packet to read from.
 @param errCode The error code.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketLastErrorFields(IDBPacket *packet, IDBErrorCode *errCode);

/*!
 Extracts fields from a Bulk Data Identify response.
 
 Command: 0xB1.
 
 @param packet The packet to read from.
 @param version The bulk data version.
 @param numEndpoints The number of supported endpoints.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketBulkDataIdentifyResponseFields(IDBPacket *packet,
                                             UInt8 *version,
                                             UInt8 *numEndpoints);

/*!
 Extracts fields from a Bulk Data Endpoint Info command.
 
 Command: 0xB2.
 
 @param packet The packet to read from.
 @param endpoint Endpoint field.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketBulkDataEndpointInfoFields(IDBPacket *packet,
                                         UInt8 *endpoint);

/*!
 Extracts fields from a Bulk Data Endpoint Info response.
 
 Command: 0xB3.
 
 @param packet The packet to read from.
 @param endpoint Endpoint field.
 @param function The endpoint function ID.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketBulkDataEndpointInfoResponseFields(IDBPacket *packet,
                                                 UInt8 *endpoint,
                                                 UInt16 *function);

/*!
 Extracts fields from a Bulk Data Read command.
 
 Command: 0xBB.
 
 @param packet The packet to read from.
 @param sequence Sequence of the packet.
 @param responseLength The length of the expected response.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketBulkDataReadFields(IDBPacket *packet,
                                 UInt8 *sequence,
                                 UInt8 *responseLength);

/*!
 Extracts fields from a Bulk Data Read response.
 
 Command: 0xBC.
 
 @param packet The packet to read from.
 @param sequence Sequence of the packet.
 @param length The length of the payload.
 @param command Command field.
 @param endpoint Endpoint field.
 @param payload The bulk data payload.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketBulkDataReadResponseFields(IDBPacket *packet,
                                         UInt8 *sequence,
                                         UInt8 *length,
                                         UInt8 *command,
                                         UInt8 *endpoint,
                                         NSData **payload);

/*!
 Extracts fields from a Bulk Data command.
 
 Command: 0xBD.
 
 @param packet The packet to read from.
 @param sequence Sequence of the packet.
 @param length Packet length field.
 @param responseLength The length of the expected response.
 @param command Command field.
 @param endpoint Endpoint field.
 @param payload The bulk data payload.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketBulkDataFields(IDBPacket *packet,
                             UInt8 *sequence,
                             UInt8 *length,
                             UInt8 *responseLength,
                             UInt8 *command,
                             UInt8 *endpoint,
                             NSData **payload);

/*!
 Extracts fields from a Bulk Data response.
 
 Command: 0xBE.
 
 @param packet The packet to read from.
 @param sequence Sequence of the packet.
 @param length Packet length field.
 @param command Command field.
 @param endpoint Endpoint field.
 @param payload The bulk data response payload.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketBulkDataResponseFields(IDBPacket *packet,
                                     UInt8 *sequence,
                                     UInt8 *length,
                                     UInt8 *command,
                                     UInt8 *endpoint,
                                     NSData **payload);

/*!
 Extracts fields from a Bulk Data Continue response.
 
 Command: 0xBE.
 
 @param packet The packet to read from.
 @param sequence Sequence of the packet.
 @param responseLength Packet length field.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketBulkDataContinueFields(IDBPacket *packet,
                                     UInt8 *sequence,
                                     UInt8 *responseLength);

/*!
 Extracts fields from a AuthenticationInfoResponse packet.
 
 Command: 0x91.
 
 @param packet The packet to parse.
 @param d1 Whether or not ID bus authenticates the d1 transport.
 @param d2 Whether or not ID bus authenticates the d2 transport.
 @param verMaj Auth major version.
 @param verMin Auth minor version.
 @param vid VID of the auth chip.
 @param pid PID of the auth chip.
 @param eswRev Revision of the embedded software.
 @param certLen Length of the certificate.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketAuthenticationInfoResponseFields(IDBPacket *packet,
                                               BOOL *d1, BOOL *d2,
                                               UInt8 *verMaj, UInt8 *verMin,
                                               UInt8 *vid, UInt8 *pid,
                                               UInt8 *eswRev,
                                               UInt16  *certLen);

/*!
 Extracts fields from a GetAuthenticationCertificate packet.
 
 Command: 0x92.
 
 @param packet The packet to parse.
 @param offset Offset in the cert being requested.
 @param len Length of cert data being requested.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketGetAuthenticationCertificateFields(IDBPacket *packet,
                                                 UInt16 *offset, UInt16 *length);

/*!
 Extracts fields from a AuthenticationCertificate packet.
 
 Command: 0x93.
 
 @param packet The packet to parse.
 @param data The requested bytes of the certificate.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketAuthenticationCertificateFields(IDBPacket *packet,
                                              NSData **data);

/*!
 Extracts fields from a StartAuthenticationResponse packet.
 
 Command: 0x95.
 
 @param packet The packet to parse.
 @param c Whether or not a valid challenge was received.
 @param r Whether or not response is ready.
 @param time Time until response is ready.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketStartAuthenticationResponseFields(IDBPacket *packet,
                                                BOOL *c,
                                                BOOL *r,
                                                UInt8 *time);

/*!
 Extracts fields from a CheckAuthenticationResponse packet.
 
 Command: 0x97.
 
 @param packet The packet to parse.
 @param c Whether or not a valid challenge was received.
 @param r Whether or not response is ready.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketCheckAuthenticationResponseFields(IDBPacket *packet,
                                                BOOL *c,
                                                BOOL *r);

/*!
 Extracts fields from a GetAuthenticationSignature packet.
 
 Command: 0x98.
 
 @param packet The packet to parse.
 @param offset Offset to return.
 @param length Length to return of signature.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketGetAuthenticationSignatureFields(IDBPacket *packet,
                                               UInt16 *offset,
                                               UInt16 *length);

/*!
 Extracts fields from a GetAuthenticationSignature packet.
 
 Command: 0x99.
 
 @param packet The packet to parse.
 @param signature.
 @return YES if the packet was successfully parsed; NO otherwise.
 */
BOOL IDBPacketAuthenticationSignatureFields(IDBPacket *packet,
                                            NSData **signature);

#pragma mark - ID Bus Packet Info Utils

/*!
 Populates a digital ID info struct with values derived from a functional variant ID value.
 
 @param info A pointer to the ID info struct to be populated with data.
 @param functionalVariantID The functional variant ID from which to derive ID values.
 @return A pointer to the populated ID info struct.
 */
IDBIDInfo *IDBPacketGetIDInfoFromFunctionalVariantID(IDBIDInfo *info, IDBFunctionalVariantID functionalVariantID);

/*!
 Populates a module serial number info struct with the bytes in the serial number string.
 
 @param info A pointer to the module serial number info struct to be populated with data.
 @param serialNumber The serial number string from which to copy bytes to the info struct.
 @return A pointer to the populated module serial number info struct.
 */
IDBInterfaceModuleSerialNumberInfo *IDBPacketGetInterfaceModuleSerialNumberInfoFromString(IDBInterfaceModuleSerialNumberInfo *info, NSString *serialNumber);

/*!
 Populates an accessory serial number info struct with the bytes in the serial number string.
 
 @param info A pointer to the accessory serial number info struct to be populated with data.
 @param serialNumber The serial number string from which to copy bytes to the info struct.
 @return A pointer to the populated accessory serial number info struct.
 */
IDBAccessorySerialNumberInfo *IDBPacketGetAccessorySerialNumberInfoFromString(IDBAccessorySerialNumberInfo *info, NSString *serialNumber);

/*!
 Creates and returns a serial number string based on the contents of the module serial number info struct.
 
 @param info A pointer to the module serial number info struct to use for generating the serial number string.
 @return The serial number string.
 */
NSString *IDBStringFromInterfaceModuleSerialNumberInfo(IDBInterfaceModuleSerialNumberInfo *info);

/*!
 Creates and returns a serial number string based on the contents of the accessory serial number info struct.
 
 @param info A pointer to the accessory serial number info struct to use for generating the serial number string.
 @return The serial number string.
 */
NSString *IDBStringFromAccessorySerialNumberInfo(IDBAccessorySerialNumberInfo *info);

#pragma mark - Other Utilities

/*!
 Returns a display-friendly hed string representing the given data buffer.
 
 @param data The data buffer to represent as a hex string.
 @return The display-friendly hex string.
 */
NSString *IDBHexStringFromData(NSData *data);
