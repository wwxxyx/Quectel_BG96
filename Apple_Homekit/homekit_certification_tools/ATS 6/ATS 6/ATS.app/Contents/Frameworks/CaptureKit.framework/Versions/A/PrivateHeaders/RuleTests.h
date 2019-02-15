//
//  RuleTests.h
//  ATS
//
//  Created by Joe Basu on 8/3/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <LingoKit/LingoKit.h>
#import <ATSBoxSupport/ATSBoxSupport.h>
#import <CaptureKit/RuleTestsDelegate.h>
#import <CaptureKit/DeviceCaptureConfigurationTypes.h>
#import <CaptureKit/RTConstants.h>

#define RT_RUNTIME_WARNING(message)                                                                                        \
    {                                                                                                                      \
        static BOOL __gaveWarning = NO;                                                                                    \
        if (!__gaveWarning) {                                                                                              \
            __gaveWarning = YES;                                                                                           \
            NSLog(@"Warning: %@:%d: %@", [[NSString stringWithUTF8String:__FILE__] lastPathComponent], __LINE__, message); \
        }                                                                                                                  \
    }

/*!
 @macro     RT_EMULATE_AND_ASSERT
 @abstract  Emulate an event and assert a series of rules thrown on it
 */
#define RT_EMULATE_AND_ASSERT_PRIV(event, rules, ...) [self assertRuleErrorsThrownWithEvent:event codes:rules, ##__VA_ARGS__]
#define RT_EMULATE_AND_ASSERT(event, ...) RT_EMULATE_AND_ASSERT_PRIV(event, ##__VA_ARGS__, 0xFFFF)

/*!
 @macro         RT_SHIM_TEST_WITH_SUFFIX_IAP2_TO_NONE / RT_SHIM_TEST_WITH_SUFFIX_NONE_TO_IAP2
 @abstract      Implements a shim method to call 1 rule test with the given suffix from another with a different suffix
 @discussion
 <rdar://problem/14530560> 8Pin rule tests are not being run when they should
 <rdar://problem/14759364> Fix RuleTests to better account for multiple transport configurations and rules which are both cross-transport and potentially cross-protocol
 Temporary fix to get rule tests running where they need to be, by adding
 tests with the correctly-suffixed name to call the actual implementation
 */
#define RT_SHIM_TEST_WITH_SUFFIX_IAP2_TO_NONE(commonTestPrefix) \
    -(void)test##commonTestPrefix##Rule                         \
    {                                                           \
        [self test##commonTestPrefix##iAP2Rule];                \
    }
#define RT_SHIM_TEST_WITH_SUFFIX_NONE_TO_IAP2(commonTestPrefix) \
    -(void)test##commonTestPrefix##iAP2Rule                     \
    {                                                           \
        [self test##commonTestPrefix##Rule];                    \
    }

@class iAP1PacketEvent;
@class ATFRetHardwareInfoEvent;
@class ATFEvent;
@class AccessoryConnectEvent, AppleDeviceConnectEvent, AppleDeviceDisconnectEvent;
@class ATSRuleEngine;
@class EmulatedATFR3CaptureDevice;
@class EmulatedATSBoxCaptureDevice;
@class EmulatedBeagleUSBCaptureDevice;
@class EmulatedComprobeCaptureDevice;
@class CaptureEvent;

#define kEndOfEmulatediAPPayload 0x5500

enum {
    RuleTestCasePositive, // Asserts that an error should be raised
    RuleTestCaseNegative  // Asserts that no errors should be raised
};

enum {
    RuleTestConnectUART,
    //    RuleTestConnectUSB, //FIXME: <rdar://problem/8830547> Revise the hardware config iteration for unit tests
    RuleTestConnectCount
};

enum {
    RuleTestDisconnectDropVbusFirst,
    RuleTestDisconnectDropResistorFirst,
    RuleTestDisconnectDropPin20First,
    RuleTestDisconnectDropFirewireFirst,
    RuleTestDisconnectCount
};

typedef UInt8 RuleTestCaseType;

extern NSString *const USBVbusKey;
extern NSString *const USBDPlusKey;
extern NSString *const USBDMinusKey;

@interface RuleTests : NSObject {
    NSMutableArray *_unassertedRuleErrors;
    BOOL _parserErrorsSeen;

    UInt8 _currentRuleTestConnectMode;
    UInt8 _currentRuleTestDisconnectMode;

    UInt16 _accTransactionIDCounter;
    UInt16 _iPodTransactionIDCounter;

    BOOL _useEnterExtendedInterfaceModeInsteadOfSetUIMode;
    BOOL _positiveTestCaseSeen;
    BOOL _negativeTestCaseSeen;

    BOOL _shouldEmulateEANativeTransportDescriptors;
    BOOL _shouldEmulateUSBHostHIDDescriptors;

    BOOL _shouldEmulateiAP2LinkInitialization;

    UInt8 _currentAppleDevicePacketSequenceNumber;
    UInt8 _currentAccessoryPacketSequenceNumber;
}

@property (nonatomic, assign) id<RuleTestsDelegate> delegate;
@property (nonatomic, assign) id<RuleTestsDocumentDelegate> documentDelegate;

@property (nonatomic) DeviceTransportType defaultiAPTransport;
@property (nonatomic, retain) ATSRuleEngine *atsBoxRuleEngine;
@property (nonatomic, retain) ATSRuleEngine *atsR3BoxRuleEngine;
@property (nonatomic, retain) ATSRuleEngine *beagleUSBRuleEngine;
@property (nonatomic, retain) ATSRuleEngine *bluetoothRuleEngine;
@property (nonatomic, retain) EmulatedATFR3CaptureDevice *emulatedATFR3CaptureDevice;
@property (nonatomic, retain) EmulatedATSBoxCaptureDevice *emulatedATSBoxCaptureDevice;
@property (nonatomic, retain) EmulatedBeagleUSBCaptureDevice *emulatedBeagleUSBCaptureDevice;
@property (nonatomic, retain) EmulatedComprobeCaptureDevice *emulatedComprobeCaptureDevice;
@property (nonatomic, copy) NSString *currentRuleName;

@property (nonatomic) BOOL useEnterExtendedInterfaceModeInsteadOfSetUIMode;
@property (nonatomic) BOOL shouldEmulateAccessoryUSBBulkPipeEnumeration;
@property (nonatomic) BOOL shouldEmulateEANativeTransportDescriptors;
@property (nonatomic) BOOL shouldEmulateUSBHostHIDDescriptors;
@property (nonatomic) BOOL shouldEmulateiAP2LinkInitialization;
@property (nonatomic) BOOL shouldEmulateCarPlayDescriptors;

@property (nonatomic) UInt8 currentAppleDevicePacketSequenceNumber;
@property (nonatomic) UInt8 currentAccessoryPacketSequenceNumber;

/*!
 @method    recordParserError
 @abstract  Records that a parser error occurred, when there is no error object
 */
- (void)recordParserError;

- (void)recordNewRuleErrors:(NSArray *)ruleErrors;
- (void)runTestForRuleName:(NSString *)ruleName iterateHardwareConfigs:(BOOL)iterateHardwareConfigs;
- (void)runTestForRuleName:(NSString *)ruleName;

- (void)insertMessage:(NSString *)message;

/*!
 @method    emulateAccessorySerialData:
 @abstract  Emulates serial data from accessory
 */
- (NSArray *)emulateAccessorySerialData:(NSData *)data;

/*!
 @method    emulateAppleDeviceSerialData:
 @abstract  Emulates serial data from Apple device
 */
- (NSArray *)emulateAppleDeviceSerialData:(NSData *)data;

/*!
 @method    emulateReconnectProcedure
 @abstract  Emulates the reconnect procedure
 */
- (void)emulateReconnectProcedure;

/*!
 @method    emulateReconnectProcedureWithSyncByte:
 @abstract  Emulates the reconnect procedure
 @param     iPodDetect
            If iPod Detect should be triggered
 @param     accPower
            If AccPower should be on
 @param     If 80ms time should be waited
            wait80msTime
 @param     sendsSyncByte
            Whether or not an initial sync byte should be emulated
 */
- (void)emulateReconnectProcedureWithiPodDetect:(BOOL)iPodDetect accDetect:(BOOL)accDetect accPower:(BOOL)accPower wait80msTime:(BOOL)wait80msTime sendsSyncByte:(BOOL)sendsSyncByte;

/*!
 @method    emulateUSBEnumeration
 @abstract  Emulates USB enumeration including resets and bulk pipe initialization
 */
- (void)emulateUSBEnumeration;

/*!
 @method    certificateDataWithClass:
 @abstract  Loads certificate data for the given class
 @param     certClass
            The certificate class (must be valid)
 @return    Certificate data blob, or assertion and nil if loading failed
 */
- (NSData *)certificateDataWithClass:(UInt8)certClass;

// emulateAccDetect YES means accessory is present (pin 20 is grounded)
- (ATFRetHardwareInfoEvent *)emulate30PinAccDetect:(BOOL)accDetect;
// emulateiPodDetect YES means iPod is present (pin 10 is grounded)
- (ATFRetHardwareInfoEvent *)emulate30PiniPodDetect:(BOOL)iPodDetect;
// emulateAccPower YES means iPod is providing Accessory Power (pin 13 is driven by iPod)
- (ATFRetHardwareInfoEvent *)emulate30PinAccPower:(BOOL)accPower;
// emulates an ATF RetHardwareInfo for peak current over the last 1 second.  Provide a floating point number representing the number of milliamps in the range [0.0, 118.0].
- (ATFRetHardwareInfoEvent *)emulate30PinAccPeakCurrent:(double)peakCurrentMilliAmps;
// emulates an ATF RetHardwareInfo for average current over the last 1 second.  Provide a floating point number representing the number of milliamps in the range [0.0, 118.0].
- (ATFRetHardwareInfoEvent *)emulate30PinAccAverageCurrent:(double)averageCurrentMilliAmps;
// emulates an ATF RetHardwareInfo for firewire voltage.  Provide a floating point number representing the number of volts in the range [0.0, 29.94].
- (ATFRetHardwareInfoEvent *)emulate30PinAccFirewireVoltage:(double)volts;
// emulates an ATF RetHardwareInfo for id resistor.  Provide a floating point number representing the number of kΩ.
- (ATFRetHardwareInfoEvent *)emulate30PinAccIDResistor:(double)kilohms;
// emulates an ATF RetHardwareInfo for USB Vbus voltage.  Provide a floating point number representing the number of volts.
- (ATFRetHardwareInfoEvent *)emulate30PinAccUSBVbus:(double)volts;
// emulates an ATF RetHardwareInfo for USB D+ voltage. Provide a floating point number representing the number of volts.
- (ATFRetHardwareInfoEvent *)emulate30PinAccUSBDPlus:(double)volts;
// emulates an ATF RetHardwareInfo for USB D- voltage. Provide a floating point number representing the number of volts.
- (ATFRetHardwareInfoEvent *)emulate30PinAccUSBDMinus:(double)volts;
// emulates an ATF RetHardwareInfo for Tx marking-state voltage.  Provide a floating point number representing the number of volts in the range [0.0, 6.60].
- (ATFRetHardwareInfoEvent *)emulate30PinAccTx:(double)volts;
// emulates an ATF RetHardwareInfo for reserved pin 14
- (ATFRetHardwareInfoEvent *)emulate30PinReservedPin14:(UInt8)pin14;
// emulates an ATF RetHardwareInfo for reserved pin 17
- (ATFRetHardwareInfoEvent *)emulate30PinReservedPin17:(UInt8)pin17;
// emulates an ATF RetHardwareInfo for Audio Return
- (ATFRetHardwareInfoEvent *)emulate30PinAudioReturnState:(UInt8)state;
// emulates an ATF RetHardwareInfo for USB Vbus with 500 mA load
- (ATFRetHardwareInfoEvent *)emulate30PinAccUSBVbusWith500mALoad:(double)volts;
// emulates an ATF RetHardwareInfo for USB Vbus with 1 A load
- (ATFRetHardwareInfoEvent *)emulate30PinAccUSBVbusWith1ALoad:(double)volts;

// Emulates AccPower going low then high, and sends the required re-connection emulations
- (void)emulate30PinAccPowerBounce;

// emulates a USB Charging Power Source 500 mA with the given USB Vbus value
- (NSDictionary *)emulate30PinAccUSBChargingPowerSource500mAWithVbus:(double)volts;
// emulates a USB Charging Power Source 1 A with the given USB Vbus value
- (NSDictionary *)emulate30PinAccUSBChargingPowerSource1AWithVbus:(double)volts;
// emulates a USB Charging Power Source 2.1 A with the given USB Vbus value
- (NSDictionary *)emulate30PinAccUSBChargingPowerSource2Point1AWithVbus:(double)volts;
// emulates a non USB Charging Power Source
- (NSDictionary *)emulate30PinAccUSBChargingPowerSourceNoneWithVbus:(double)volts;
// emulates a valid USB Charging Power Source 500 mA
- (NSDictionary *)emulate30PinAccUSBChargingPowerSource500mA;
// emulates a valid USB Charging Power Source 1 A
- (NSDictionary *)emulate30PinAccUSBChargingPowerSource1A;
// emulates a valid USB Charging Power Source 2.1 A
- (NSDictionary *)emulate30PinAccUSBChargingPowerSource2Point1A;
// emulates a valid non USB Charging Power Source
- (NSDictionary *)emulate30PinAccUSBChargingPowerSourceNone;

- (ATFEvent *)emulateSerialDataWithBaud:(ATFSource)source byte:(char)byte baudRate:(UInt32)baudRate;

// returns mutliple packets if returned (used for returning sync bytes)
- (NSArray *)emulateiAPPacketFromSourceExpectingMulti:(LKTSource)source
                                              lingoID:(UInt8)lingoID
                                            commandID:(UInt16)commandID
                                         payloadBytes:(UInt)firstByte, ...;
- (NSArray *)emulateiAPPacketFromSourceExpectingMulti:(LKTSource)source
                                              lingoID:(UInt8)lingoID
                                            commandID:(UInt16)commandID
                                          payloadData:(NSData *)payloadData;

/*!
 @method    emulateiAPPacketFromSourceExpectingMulti:lingoID:commandID:payloadData:dropTrailingBytes:
 @abstract  Emulate an iAP packet, and DROP the last numDroppedBytes bytes. This will create a broken packet.
 */
- (NSArray *)emulateiAPPacketFromSourceExpectingMulti:(LKTSource)source
                                              lingoID:(UInt8)lingoID
                                            commandID:(UInt16)commandID
                                          payloadData:(NSData *)payloadData
                                    dropTrailingBytes:(int)numDroppedBytes;

- (void)allowCommandsWithReservedErrors:(BOOL)enabled;

- (void)assertRuleErrorThrownWithCode:(NSInteger)code event:(CaptureEvent *)event;
- (void)assertRuleErrorThrownWithCode:(NSInteger)code events:(CaptureEvent *)event, ... NS_REQUIRES_NIL_TERMINATION;
- (void)assertRuleErrorThrownWithCode:(NSInteger)code eventsArray:(NSArray *)events;

/*!
 @method    assertRuleErrorsThrownWithEvent:codes:
 @abstract  Asserts a series of rule codes are thrown given a single event. The list must be terminated with an 0xFFFF
 @param     event
            The event which triggered the throw(s)
 @param     code
            A list of rule ids terminated in 0xFFFF
 */
- (void)assertRuleErrorsThrownWithEvent:(CaptureEvent *)event codes:(NSInteger)code, ...;

/*!
 @method        assertParserErrorSeen
 @abstract      Asserts that a parser error was seen
 @discussion    This can be used to say "Ok, that error was expected" so that the rule test does not fail
 */
- (void)assertParserErrorSeen;

- (void)verifyNoUnassertedRuleErrors;

- (void)defineRuleTestCaseWithType:(RuleTestCaseType)type description:(NSString *)description;
- (void)defineRuleTestCaseWithType:(RuleTestCaseType)type description:(NSString *)description withAccDetect:(BOOL)accDetect;
- (void)defineRuleTestCaseWithType:(RuleTestCaseType)type description:(NSString *)description sendSyncByte:(BOOL)sendSyncByte;
- (void)defineRuleTestCaseWithType:(RuleTestCaseType)type description:(NSString *)description wait80msTime:(BOOL)wait80msTime sendSyncByte:(BOOL)sendSyncByte;
- (void)defineRuleTestCaseWithType:(RuleTestCaseType)type description:(NSString *)description withAccDetect:(BOOL)accDetect accPower:(BOOL)accPower wait80msTime:(BOOL)wait80msTime sendSyncByte:(BOOL)sendSyncByte;
- (void)defineRuleTestCaseWithType:(RuleTestCaseType)type description:(NSString *)description withAccDetect:(BOOL)accDetect accPower:(BOOL)accPower wait80msTime:(BOOL)wait80msTime sendSyncByte:(BOOL)sendSyncByte simulateCertificationAssistantRunning:(BOOL)simCA withClaims:(NSArray *)claims;
- (void)defineRuleTestCaseWithType:(RuleTestCaseType)type description:(NSString *)description withiPodDetect:(BOOL)ipodDetect withAccDetect:(BOOL)accDetect accPower:(BOOL)accPower wait80msTime:(BOOL)wait80msTime sendSyncByte:(BOOL)sendSyncByte simulateCertificationAssistantRunning:(BOOL)simCA withClaims:(NSArray *)claims;

- (void)emulReportGUIMessage:(NSString *)format, ...;
- (void)emulAddTimeInMillis:(UInt32)durationInMilliseconds;

/*!
 @method    emulAddTimeInMillis:display:
 @abstract  Adds the number of milliseconds and optionally inserts a method in the trace notifying of the time change
 */
- (void)emulAddTimeInMillis:(UInt32)durationInMilliseconds display:(BOOL)display;

- (iAP1PacketEvent *)emulAccIdentify:(unsigned char)lingo;
- (iAP1PacketEvent *)emulAccessoryNoAuthIDL;
- (iAP1PacketEvent *)emulAccIdentifyDeviceLingoes:(UInt32)lingoes options:(UInt32)options deviceID:(UInt32)device_id;
- (iAP1PacketEvent *)emulAccIdentifyDeviceLingoes:(UInt32)lingoes options:(UInt32)options deviceID:(UInt32)device_id withiPodACK:(BOOL)iPodACK;
- (void)emulIDLAuthV2ForLingoes:(UInt32)lingoes;
- (void)emulStartIDPSAuthV2ForLingoes:(UInt32)lingoes withAccTransactionID:(UInt8 *)pAccTransactionID andiPodTransactionID:(UInt8 *)piPodTransactionID;
- (void)emulStartIDPSWithAccTransactionID:(UInt8 *)pAccTransactionID;
- (void)emulUnsupportedStartIDPSWithAccTransactionID:(UInt8)pAccTransactionID followedByNoAuthIDL:(BOOL)sendIDL;
- (void)emulUnsupportedStartIDPSWithAccTransactionID:(UInt8)pAccTransactionID;
- (void)emulAccMinimalValidSetFIDTokenValuesWithTransID:(UInt16)transID;
- (void)emulAccMinimalValidSetFIDTokenValuesWithTransID:(UInt16)transID andDeviceOptions:(UInt32)deviceOptions;
- (void)emulAccMinimalValidSetFIDTokenValuesWithTransID:(UInt16)transID andLingoMask:(UInt32)lingoes;
- (void)emulAccMinimalValidSetFIDTokenValuesWithTransID:(UInt16)transID andLingoMask:(UInt32)lingoes andDeviceOptions:(UInt32)deviceOptions andAccCaps:(UInt64)accCaps;
- (void)emulAccSDKSetFIDTokenValuesWithAccTransactionID:(uint8_t *)pAccTransactionID andiPodTransactionID:(uint8_t *)piPodTransactionID;
- (void)emulConcludeIDPSAfterSetFIDTokenValuesWithAccTransactionID:(UInt8 *)pAccTransactionID andiPodTransactionID:(UInt8 *)piPodTransactionID;
- (NSArray *)emulSendCertificateWithClass:(UInt8) class maxSectionSize:(UInt32)maxSize emulateACKs:(BOOL)emulateACKs withTransID:(UInt16)transID;
- (NSArray *)emulSendCertificateWithClass:(UInt8) class maxSectionSize:(UInt32)maxSize emulateACKs:(BOOL)emulateACKs;
- (void)emulRequestLingoProtocolVersion:(UInt8)lingo versionMajor:(UInt8)majorVersion minor:(UInt8)minorVersion;
- (void)emulRequestLingoProtocolVersion:(UInt8)lingo transIDCounter:(UInt8 *)accTransID versionMajor:(UInt8)majorVersion minor:(UInt8)minorVersion;

- (void)emulateAuthV2WithiPodTransactionID:(UInt16)iPodTransactionID;
- (void)emulRequestTransportMaxPayloadSizeWithAccTransactionID:(UInt8)accTransID;

- (void)emulGetiPodOptionsForLingo:(UInt8)lingo options:(UInt64)options accTransID:(UInt8)accTransID;

- (void)emulStartIDPSAuthV2;
- (void)emulStartIDPSAuthV2AndEnteriPodOutMode;
- (iAP1PacketEvent *)emulateAccSyncByte;
- (iAP1PacketEvent *)accCommand:(UInt32)lingoAndCommandID bytes:(UInt)firstByte, ...;
- (iAP1PacketEvent *)iPodCommand:(UInt32)lingoAndCommandID bytes:(UInt)firstByte, ...;
- (iAP1PacketEvent *)accCommand:(UInt32)lingoAndCommandID;
- (iAP1PacketEvent *)iPodCommand:(UInt32)lingoAndCommandID;
- (iAP1PacketEvent *)emulateMinimalValidSetFIDTokenValuesWithTransID:(UInt16)transID;
- (iAP1PacketEvent *)emulateMinimalValidRetFIDTokenValuesWithTransID:(UInt16)transID;
- (iAP1PacketEvent *)emulateSetFIDTokenValuesWithTransID:(UInt16)transID andLingoMask:(UInt32)lingoes andDeviceOptions:(UInt32)deviceOptions andAccCaps:(UInt64)accCaps;
- (iAP1PacketEvent *)emulateSetFIDTokenValuesWithTransID:(UInt16)transID andLingoMask:(UInt32)lingoes andDeviceOptions:(UInt32)deviceOptions andAccCaps:(UInt64)accCaps andRFCertifcation:(UInt32)rfCertMask;
- (iAP1PacketEvent *)emulateRetFIDTokenValuesWithTransID:(UInt16)transID andLingoMask:(UInt32)lingoes andDeviceOptions:(UInt32)deviceOptions andAccCaps:(UInt64)accCaps;

// Same as accCommand: or iPodCommand:, but auto-detects source using LingoKit
- (iAP1PacketEvent *)emulateCommand:(UInt32)lingoAndCommandID bytes:(UInt)firstByte, ...;
- (iAP1PacketEvent *)emulateCommand:(UInt32)lingoAndCommandID;
- (iAP1PacketEvent *)emulateCommand:(UInt32)lingoAndCommandID payloadData:(NSData *)data;

- (void)simulateCertificationAssistantRunning;
- (void)forceClaim:(NSString *)claimName;

/*!
 @method    forceAppleDeviceCompatibility:
 @abstract  Emulates CA compatibility with the apple device
 */
- (void)forceAppleDeviceCompatibility:(AppleDeviceType)type;

/*!
 @method    forceTestMode:
 @abstract  Emulates CA "Test Mode" ProductPlan.
 */
- (void)forceTestMode;

/*!
 @method    forceLightningModuleFunctionalVariant:
 @abstract  Emulates CA ProductPlan support for a Lightning module functional variant.
 */
- (void)forceLightningModuleFunctionalVariant:(LightningModuleFunctionalVariant)variant;

/*!
 @method    forceFeature:
 @abstract  Emulates CA ProductPlan support for feature
 */
- (void)forceFeature:(FeatureType)type;

/*!
 @method    forceTransport:
 @abstract  Forces a transport type
 */
- (void)forceTransport:(TransportType)transport;

/*!
 @method    forceUSBVbusPowerSourceType:
 @abstract  Forces a USB Vbus power source type
 */
- (void)forceUSBVbusPowerSourceType:(USBVbusPowerSourceType)type;

/*!
 @method -emulateAccessoryATSR3BoxSerialData:
 @abstract Emulates an ATS Lightning Box sniffing raw bytes originating from the accessory
 @param serialData The accessory serial data
 @return An array of iAP2MessageEvents that have been constructed by the ATS processing code
 */
- (NSArray *)emulateAccessoryATSR3BoxSerialData:(NSData *)serialData;
/*!
 @method -emulateAppleDeviceATSR3BoxSerialData:
 @abstract Emulates an ATS Lightning Box sniffing raw bytes originating from the Apple device
 @param serialData The Apple device serial data
 @return An array of iAP2MessageEvents that have been constructed by the ATS processing code
 */
- (NSArray *)emulateAppleDeviceATSR3BoxSerialData:(NSData *)serialData;

@end
