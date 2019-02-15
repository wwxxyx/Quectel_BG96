//
//  RuleTests+TokenEmulation.h
//  ATSMacApp
//
//  Created by Joe Basu on 9/13/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleTests.h>

enum {
    iAPAccCapsAnalogLineOut = 1,
    iAPAccCapsAnalogVideoOut = 1 << 2,
    iAPAccCapsUSBAudioOut = 1 << 4,
    iAPAccCapsAssistiveTouch = 1 << 23,
};

enum {
    iAPAccInfoName = 0x01,
    iAPAccInfoFirmwareVersion = 0x04,
    iAPAccInfoHardwareVersion = 0x05,
    iAPAccInfoManufacturer = 0x06,
    iAPAccInfoModelNumber = 0x07,
    iAPAccInfoSerialNumber = 0x08,
    iAPAccInfoIncomingMaxPayloadSize = 0x09,
    iAPAccInfoStatus = 0x0B,
    iAPAccInfoRFCertifications = 0x0C,
};

enum {
    iAPAccInfoRFCertificationClass1 = 1,
    iAPAccInfoRFCertificationClass2 = 1 << 1,
    iAPAccInfoRFCertificationClass4 = 1 << 3,
};

enum {
    iAPPreferenceVideoOutSetting = 0x00,
    iAPPreferenceVideoSignalFormat = 0x02,
    iAPPreferenceLineOutUsage = 0x03,
    iAPPreferenceVideoOutConnection = 0x08,
    iAPPreferenceAssistiveTouch = 0x16,
};

enum {
    iAPPreferenceVideoOutSettingOff = 0x00,
    iAPPreferenceVideoOutSettingOn = 0x01,
};

enum {
    iAPPreferenceVideoOutConnectionNone = 0x00,
    iAPPreferenceVideoOutConnectionComposite = 0x01,
    iAPPreferenceVideoOutConnectionSVideo = 0x02,
    iAPPreferenceVideoOutConnectionComponent = 0x03,
};

enum {
    iAPPreferenceLineOutUsageOff = 0x00,
    iAPPreferenceLineOutUsageOn = 0x01,
};

enum {
    iAPPreferenceVideoSignalFormatNTSC = 0x00,
    iAPPreferenceVideoSignalFormatPAL = 0x01,
};

enum {
    iAPPreferenceAssistiveTouchOff = 0x00,
    iAPPreferenceAssistiveTouchOn = 0x01,
};

enum {
    iAPTokenAccepted = 0x00,
};

@interface RuleTests (TokenEmulation)

- (iAP1PacketEvent *)emulateSetFIDTokenValuesWithTokenDataFragments:(NSData *)firstDataFragment, ...;
- (iAP1PacketEvent *)emulateAckFIDTokenValuesWithTokenAckDataFragments:(NSData *)firstDataFragment, ...;

- (NSData *)identifyTokenDataWithLingoCount:(UInt8)numLingoes lingoIDs:(UInt)firstLingoID, ...;
- (NSData *)accessoryCapsTokenDataWithBitfield:(UInt64)accCapsBitfield;
- (NSData *)accessoryInfoTokenDataVersionWithType:(UInt8)infoType majorVersion:(UInt8)majorVersion minorVersion:(UInt8)minorVersion revisionVersion:(UInt8)revisionVersion;
- (NSData *)accessoryInfoTokenDataStringWithType:(UInt8)infoType string:(NSString *)string;
- (NSData *)accessoryInfoTokenDataForRFCertificationDeclarationBitfield:(UInt32)bitfield;
- (NSData *)iPodPreferenceTokenDataForPrefClass:(UInt8)prefClass settingID:(UInt8)settingID;

- (NSData *)identifyTokenDataAckWithStatusCode:(UInt8)statusCodeExceptFour;
- (NSData *)accessoryCapsTokenDataAckWithStatusCode:(UInt8)statusCode;
- (NSData *)accessoryInfoTokenDataAckWithStatusCode:(UInt8)statusCode forType:(UInt8)infoType;
- (NSData *)iPodPreferenceTokenDataAckWithStatusCode:(UInt8)statusCode forPrefClass:(UInt8)prefClass;

@end
