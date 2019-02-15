/*
 *  BSUSBTypes.h
 *  BusSniffer
 *
 *  Created by Joe Basu on 2/24/09.
 *  Copyright © 2009 Apple Inc. All Rights Reserved.
 *
 */

#import <MacTypes.h>

enum {
    BSUSBStateHostConnected     = 0x00000001,
    BSUSBStateHostDisconnected  = 0x00000002,
    BSUSBStateReset             = 0x00000004,
    BSUSBStateSuspended         = 0x00000008,
    BSUSBStateResuming          = 0x00000010,
    BSUSBStateUnknownSpeed      = 0x00000020,
    BSUSBStateLowSpeed          = 0x00000040,
    BSUSBStateFullSpeed         = 0x00000080,
    BSUSBStateHighSpeed         = 0x00000100,
    BSUSBStateTimerIncrement    = 0x00000200
};

typedef NS_ENUM(UInt8, BSUSBSpeed) {
    BSUSBSpeedLow       = 0x00,
    BSUSBSpeedFull      = 0x01,
    BSUSBSpeedHigh      = 0x02,
    BSUSBSpeedUnknown   = 0xFF,
};

enum {
    BSUSBPacketOUT          = 0xE1,
    BSUSBPacketIN           = 0x69,
    BSUSBPacketSOF          = 0xA5,
    BSUSBPacketSETUP        = 0x2D,
    BSUSBPacketDATA0        = 0xC3,
    BSUSBPacketDATA1        = 0x4B,
    BSUSBPacketDATA2        = 0x87,
    BSUSBPacketMDATA        = 0x0F,
    BSUSBPacketACK          = 0xD2,
    BSUSBPacketNAK          = 0x5A,
    BSUSBPacketSTALL        = 0x1E,
    BSUSBPacketNYET         = 0x96,
    BSUSBPacketPREOrERR     = 0x3C,
    BSUSBPacketPRE          = 0x3C,
    BSUSBPacketERR          = 0x3C,
    BSUSBPacketSPLIT        = 0x78,
    BSUSBPacketPING         = 0xB4,
    BSUSBPacketEXT          = 0xF0,
    BSUSBPacketUnknown      = 0xFF,
};

typedef UInt8 BSUSBPacketID;

enum {
    BSUSBPacketStatusOK                 = 0x0,
    BSUSBPacketStatusInvalidPacketID    = 0x1,
    BSUSBPacketStatusTooManyBytes       = 0x2,
    BSUSBPacketStatusTooFewBytes        = 0x3,
    BSUSBPacketStatusInvalidCRC         = 0x4,
    BSUSBPacketStatusEmpty              = 0x5,
    BSUSBPacketStatusUnknownError       = 0xFF
};

typedef UInt8 BSUSBPacketStatus;

enum {
    BSUSBTransactionSETUP   = 0x0,
    BSUSBTransactionIN      = 0x1,
    BSUSBTransactionOUT     = 0x2,
    BSUSBTransactionPING    = 0x3,
    BSUSBTransactionUnknown = 0xFF,
};

typedef UInt8 BSUSBTransactionType;

enum {
    BSUSBTransactionStatusACK                               = 0x0,
    BSUSBTransactionStatusNAK                               = 0x1,
    BSUSBTransactionStatusSTALL                             = 0x2,
    BSUSBTransactionStatusNYET                              = 0x3,
    BSUSBTransactionStatusIsochronousInNoData               = 0x4,
    BSUSBTransactionStatusIsochronousInWithData             = 0x5,
    BSUSBTransactionStatusIsochronousOut                    = 0x5,
    BSUSBTransactionStatusInvalidEndpoint                   = 0x10,
    BSUSBTransactionStatusSetupOnNonControlPipe             = 0x11,
    BSUSBTransactionStatusPingOnNonPingEndpoint             = 0x12,
    BSUSBTransactionStatusInvalidPacketForNewTransaction    = 0x13,
    BSUSBTransactionStatusPacketAfterSETUPNotDATA0          = 0x14,
    BSUSBTransactionStatusNoACKPacketSeenAfterSetupDATA0    = 0x15,
    BSUSBTransactionStatusInvalidResponseToIN               = 0x16,
    BSUSBTransactionStatusNoACKPacketSeenAfterInData        = 0x17,
    BSUSBTransactionStatusNoDataPacketAfterOUT              = 0x18,
    BSUSBTransactionStatusInvalidResponseToOutData          = 0x19,
    BSUSBTransactionStatusInvalidResponseToPing             = 0x1A,
    BSUSBTransactionStatusUnknownError                      = 0xFF
};

typedef UInt8 BSUSBTransactionStatus;

enum {
    BSUSBTransferStatusOK                       = 0x0,
    BSUSBTransferStatusUnexpectedTransaction    = 0x10,
    BSUSBTransferStatusInvalidPacketSize        = 0x11,
    BSUSBTransferStatusInvalidSETUPTransaction  = 0x12,
    BSUSBTransferStatusUnknownError             = 0xFF
};

typedef UInt8 BSUSBTransferStatus;

typedef struct {
    UInt8 bmRequestType;
    UInt8 bRequest;
    UInt16 wValue;
    UInt16 wIndex;
    UInt16 wLength;
} BSUSBDeviceRequestRecord;

enum {
    BSUSBDeviceRequestDirectionShift    = 7,
    BSUSBDeviceRequestDirectionMask     = 1,
    
    BSUSBDeviceRequestTypeShift         = 5,
    BSUSBDeviceRequestTypeMask          = 3,
    
    BSUSBDeviceRequestRecipientMask     = 0x1F,
};

enum {
    BSUSBDeviceRequestRecipientDevice       = 0,
    BSUSBDeviceRequestRecipientInterface    = 1,
    BSUSBDeviceRequestRecipientEndpoint     = 2,
    BSUSBDeviceRequestRecipientOther        = 3,
};

typedef UInt8 BSUSBDeviceRequestRecipient;

enum {
    BSUSBDeviceRequestTypeStandard  = 0,
    BSUSBDeviceRequestTypeClass     = 1,
    BSUSBDeviceRequestTypeVendor    = 2
};

typedef UInt8 BSUSBDeviceRequestType;

enum {
    BSUSBDeviceRequestDirectionOut      = 0,
    BSUSBDeviceRequestDirectionIn       = 1,
    BSUSBDeviceRequestDirectionUnknown  = 0xFF,
};

typedef UInt8 BSUSBDeviceRequestDirection;

enum {
    BSUSBDeviceRequestGetStatus             = 0,
    BSUSBDeviceRequestClearFeature          = 1,
    BSUSBDeviceRequestSetFeature            = 3,
    BSUSBDeviceRequestSetAddress            = 5,
    BSUSBDeviceRequestGetDescriptor         = 6,
    BSUSBDeviceRequestSetDescriptor         = 7,
    BSUSBDeviceRequestGetConfiguration      = 8,
    BSUSBDeviceRequestSetConfiguration      = 9,
    BSUSBDeviceRequestGetInterface          = 10,
    BSUSBDeviceRequestSetInterface          = 11,
    BSUSBDeviceRequestSynchFrame            = 12
};

typedef UInt8 BSUSBDeviceRequestRequest;

enum {
    BSUSBDescriptorDevice                   = 1,
    BSUSBDescriptorConfiguration            = 2,
    BSUSBDescriptorString                   = 3,
    BSUSBDescriptorInterface                = 4,
    BSUSBDescriptorEndpoint                 = 5,
    BSUSBDescriptorDeviceQualifier          = 6,
    BSUSBDescriptorOtherSpeedConfiguration  = 7,
    BSUSBDescriptorInterfacePower           = 8,
    BSUSBDescriptorOnTheGo                  = 9,
    BSUSBDescriptorDebug                    = 10,
    BSUSBDescriptorInterfaceAssociation     = 11,
    BSUSBDescriptorCSInterface              = 0x24,
    BSUSBDescriptorUnknown                  = 0xFF,
};

typedef UInt8 BSUSBDescriptorType;

enum {
    BSUSBDescriptorSubtypeHeaderFunctionalDescriptor                = 0x00,
    BSUSBDescriptorSubtypeUnionFunctionalDescriptor                 = 0x06,
    BSUSBDescriptorSubtypeEthernetNetworkingFunctionalDescriptor    = 0x0F,
    BSUSBDescriptorSubtypeNCMFunctionalDescriptor                   = 0x1A,
};

typedef UInt8 BSUSBDescriptorSubtype;

enum {
    BSUSBInterfaceClassAudio                    = 0x01,
    BSUSBInterfaceClassCommunicationControl     = 0x02,
    BSUSBInterfaceClassHID                      = 0x03,
    BSUSBInterfaceClassPhysical                 = 0x05,
    BSUSBInterfaceClassImage					= 0x06,
    BSUSBInterfaceClassPrinter					= 0x07,
    BSUSBInterfaceClassMassStorage				= 0x08,
    BSUSBDeviceClassHub                         = 0x09,
    BSUSBInterfaceClassCommunicationData		= 0x0A,
    BSUSBInterfaceClassSmartCard				= 0x0B,
    BSUSBInterfaceClassContentSecurity			= 0x0D,
    BSUSBInterfaceClassVideo					= 0x0E,
    BSUSBInterfaceClassPersonalHealthcare		= 0x0F,
    BSUSBInterfaceClassDiagnosticDevice         = 0xDC,
    BSUSBInterfaceClassWirelessController		= 0xE0,
    BSUSBInterfaceClassMiscellaneous			= 0xEF,
    BSUSBInterfaceClassApplicationSpecific		= 0xFE,
    BSUSBInterfaceClassVendorSpecific           = 0xFF,
};

typedef UInt8 BSUSBInterfaceClass;

typedef UInt8 BSUSBInterfaceSubClass; // this misuse of camel case is not my doing, it's in the USB spec

enum {
    BSUSBEndpointControl        = 0,
    BSUSBEndpointIsochronous    = 1,
    BSUSBEndpointBulk           = 2,
    BSUSBEndpointInterrupt      = 3,
    BSUSBEndpointUnknown        = 0xFF
};

typedef UInt8 BSUSBEndpointType;

#define BSUSBTransferControl BSUSBEndpointControl
#define BSUSBTransferIsochronous BSUSBEndpointIsochronous
#define BSUSBTransferBulk BSUSBEndpointBulk
#define BSUSBTransferInterrupt BSUSBEndpointInterrupt
#define BSUSBTransferUnknown BSUSBEndpointUnknown

#define BSUSBTransferType BSUSBEndpointType

#define BSUSBUnknownDeviceAddress 0xFF
#define BSUSBUnknownEndpointNumber 0xFF

enum {
    BSUSBEndpointDirectionOut       = 0,
    BSUSBEndpointDirectionIn        = 1,
    BSUSBEndpointDirectionNone      = 2,
    BSUSBEndpointDirectionUnknown   = 0xFF,
};

#define BSUSBEndpointDirectionBidirectional BSUSBEndpointDirectionNone

typedef UInt8 BSUSBEndpointDirection;

#define BSUSBTransferDirection BSUSBEndpointDirection

#define BSUSBTransferDirectionOut BSUSBEndpointDirectionOut
#define BSUSBTransferDirectionIn BSUSBEndpointDirectionIn
#define BSUSBTransferDirectionNone BSUSBEndpointDirectionNone
#define BSUSBTransferDirectionUnknown BSUSBEndpointDirectionUnknown
#define BSUSBTransferDirectionBidirectional BSUSBTransferDirectionBidirectional

// originally ripped off from IOKit, but some names have been changed
enum {
    BSUSBbEndpointNumberMask                            = 0x0F,
    BSUSBbEndpointDirectionBit                          = 7,
    BSUSBbEndpointDirectionMask                         = ( 1 << BSUSBbEndpointDirectionBit ),
    BSUSBEndpointbmAttributesTransferTypeMask           = 0x03,
    BSUSBEndpointbmAttributesSynchronizationTypeMask    = 0x0C,
    BSUSBEndpointbmAttributesSynchronizationTypeShift   = 2,
    BSUSBEndpointbmAttributesUsageTypeMask              = 0x30,
    BSUSBEndpointbmAttributesUsageTypeShift             = 4,
    BSUSBEndpointwMaxPacketSizeMaxPacketSizeMask        = 0x3FF,
    
};
