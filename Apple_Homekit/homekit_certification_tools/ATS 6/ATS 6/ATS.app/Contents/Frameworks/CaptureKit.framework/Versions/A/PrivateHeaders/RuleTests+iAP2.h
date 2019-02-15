//
//  RuleTests+iAP2.h
//  ATSMacApp
//
//  Created by Joe Basu on 7/18/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleTests.h>
#import <iAP2MessageKit/iAP2MessageKit.h>
#import <ATSR3BoxSupport/ATSR3BoxSupport.h>
#import <CaptureKit/iAP2MessageEvent.h>
#import <CaptureKit/iAP2Constants.h>
#import <CaptureKit/AccessoryConnectEvent.h>
#import <CaptureKit/AccessoryDisconnectEvent.h>
#import <CaptureKit/AppleDeviceConnectEvent.h>
#import <CaptureKit/AppleDeviceDisconnectEvent.h>
#import <CaptureKit/iAP2LinkPacketEvent.h>
#import <IDBusKit/IDBusKit.h>
#import <BusSniffer/BusSniffer.h>

@class CATestStartedEvent;
@class CATestFinishedEvent;
@class EightPinAccVoltageEvent;
@class EightPinAccCurrentEvent;
@class EightPinDeviceVoltageEvent;
@class EightPinDeviceCurrentEvent;
@class EightPinUSBBrickDetectEvent;
@class EightPinAccOrientationStateEvent;
@class EightPinSerialBaudRateEvent;
@class VBusLoadTestEvent;
@class ATSExternalAccessoryEvent;
@class iAP2FileTransferDatagramEvent;
@class IDBusPacketEvent;

#define kRTParamSelector_int8 int8
#define kRTParamSelector_int16 int16
#define kRTParamSelector_int32 int32
#define kRTParamSelector_int64 int64
#define kRTParamSelector_uint8 uint8
#define kRTParamSelector_uint16 uint16
#define kRTParamSelector_uint32 uint32
#define kRTParamSelector_uint64 uint64
#define kRTParamSelector_bool bool
#define kRTParamSelector_utf8 string
#define kRTParamSelector_blob data

/*!
 @const      iap2LinkSynchronizationPayloadLinkVersionParameter
 @abstract   The key for the link version parameter of iAP2 link synchronization payload.
 */
extern NSString *const LinkPacketSynchronizationPayloadLinkVersionParameterKey;

/*!
 @const      iap2LinkSynchronizationPayloadMaxNumberOfOutstandingPacketsParameter
 @abstract   The key for the max number of outstanding packets parameter of iAP2 link synchronization payload.
 */
extern NSString *const LinkPacketSynchronizationPayloadMaxNumberOfOutstandingPacketsParameterKey;

/*!
 @const      iap2LinkSynchronizationPayloadMaxPacketLengthParameter
 @abstract   The key for the maxpacket length parameter of iAP2 link synchronization payload.
 */
extern NSString *const LinkPacketSynchronizationPayloadMaxPacketLengthParameterKey;

/*!
 @const      iap2LinkSynchronizationPayloadRetransmissionTimeoutParameter
 @abstract   The key for the retransmission timeout parameter of iAP2 link synchronization payload.
 */
extern NSString *const LinkPacketSynchronizationPayloadRetransmissionTimeoutParameterKey;

/*!
 @const      iap2LinkSynchronizationPayloadCumulativeAcknowledgementTimeoutParameter
 @abstract   The key for the cumulative acknowledgement timeout parameter of iAP2 link synchronization payload.
 */
extern NSString *const LinkPacketSynchronizationPayloadCumulativeAcknowledgementTimeoutParameterKey;

/*!
 @const      iap2LinkSynchronizationPayloadMaxNumberOfRetransmissionsParameter
 @abstract   The key for the max number of retransmissions parameter of iAP2 link synchronization payload.
 */
extern NSString *const LinkPacketSynchronizationPayloadMaxNumberOfRetransmissionsParameterKey;

/*!
 @const      iap2LinkSynchronizationPayloadMaxNumberOfCumulativeAcknowledgementsParameter
 @abstract   The key for the max number of cumultaive acknowledgements parameter of iAP2 link synchronization payload.
 */
extern NSString *const LinkPacketSynchronizationPayloadMaxNumberOfCumulativeAcknowledgementsParameterKey;

/*!
  @const      LinkPacketSynchronizationPayloadSessionDefinitionParameterKey
  @abstract   The key for the session definition parameter of iAP2 link synchronization payload.
  */
extern NSString *const LinkPacketSynchronizationPayloadSessionDefinitionParameterKey;

/*!
 @macro         RT_I2MMessage
 @abstract      Constructs an I2MMessage object
 @discussion    Convenience method for creating an I2MMessage instance.
                Usage: RT_I2MMessage(messageId, parameter1, parameter2, ...)
                Example: RT_I2MMessage(iAP2AssistiveTouchInformationMessageID,
                                       [I2MParameter boolParameterWithID:0 bool:YES name:@"isEnabled"])
 */
#define RT_PRIV_I2MMessage(messageId, parameter, ...) [self messageWithID:messageId parameters:parameter, ##__VA_ARGS__]
#define RT_I2MMessage(messageId, ...) RT_PRIV_I2MMessage(messageId, ##__VA_ARGS__, nil)

/*!
 @macro         RT_I2MGroupParameter
 @abstract      Constructs an I2MGroupParameter object
 @discussion    Convenience method for creating an I2MGroupParameter instance.
                Usage: RT_I2MGroupParameter(paramId, name, subparams, ...)
 */
#define RT_I2MGroupParameter(paramId, paramName, subparams, ...) [I2MGroupParameter groupParameterWithID:paramId subparameters:[NSArray arrayWithObjects:subparams, ##__VA_ARGS__, nil] name:paramName]

/*!
 @macro         RT_I2MAutoGroupParameter
 @abstract      Constructs an I2MGroupParameter using just the parameter name
 @discussion    Usage: RT_I2MAutoGroupParameter(BluetoothComponentInformation_BluetoothComponentStatus, subparams, ...)
 */
#define RT_I2MAutoGroupParameter(pname, subparams, ...) RT_I2MGroupParameter(iAP2##pname##ParameterID, iAP2##pname##ParameterName, subparams, __VA_ARGS__)

/*!
 @macro         RT_I2MParameter
 @abstract      Constructs an I2MParameter object
 @discussion    Convenience method for creating an I2MParameter object
                Usage: RT_I2MParameter(uint16, PowerSourceUpdate_AvailableCurrentForDevice, 1)
 */
#define RT_I2MParameter(type, pname, value) [I2MParameter type##ParameterWithID:iAP2##pname##ParameterID kRTParamSelector_##type:value name:iAP2##pname##ParameterName]

@interface RuleTests (iAP2)

/*!
 @method    messageWithID:
 @abstract  Creates an I2MMessage instance and smartly inserts the correct name based off iAP2Constants.h
 @param     ID
            The message ID
 @return    I2MMessage instance
 */
- (I2MMessage *)messageWithID:(UInt16)ID;

/*!
 @method    messageWithID:parameters:
 @abstract  Creates an I2MMessage instance and smartly inserts the correct name based off iAP2Constants.h, and populates with list of parameters
 @param     ID
            The message ID
 @param     parameter
            A nil terminated list of parameters
 @return    I2MMessage instance
 */
- (I2MMessage *)messageWithID:(UInt16)ID parameters:(I2MParameter *)parameter, ... NS_REQUIRES_NIL_TERMINATION;

/*!
 @method -emulateiAP2LinkSynchronizationControlFileTransferEASessions
 @abstract Emulates a link packet with a vanilla link synchronization payload supporting Control, File Transfer, and EA sessions
 */
- (void)emulateiAP2LinkSynchronizationControlFileTransferEASessions;

/*!
    Emulates a link packet with a vanilla link synchronization payload supporting Control and EA sessions
 */
- (void)emulateiAP2LinkSynchronizationControlEASessions;

/*!
 @method -emulateiAP2LinkSynchronizationControlSessionOnly
 @abstract Emulates a link packet with a vanilla link synchronization payload supporting a Control session only
 */
- (void)emulateiAP2LinkSynchronizationControlSessionOnly;

/*!
 Emulates all the necessary packets to create an iAP2 link with the given payload data.
 
 @param     linkSynchronizationPayloadData  the synchronizatiom payload data
 @return    The list of events sent to emulate an iAP2 link synchonrization
 */
- (NSArray *)emulateiAP2LinkSynchronizationWithData:(NSData *)linkSynchronizationPayloadData;

/*!
 @method    -emulateAppleDeviceBadACKSequence
 @abstract  Emulates a bad ACK sequence sent by the apple device
 @return    The list of events sent to emulate a bad ACK sequence
 */
- (iAP2LinkPacketEvent *)emulateAppleDeviceBadACKSequence;

/*!
 @method    -emulateAppleDeviceRequestIdentifySequence
 @abstract  Emulates a request identify sequence sent by the apple device
 @return    The list of events sent to emulate a request identify  sequence
 */
- (iAP2LinkPacketEvent *)emulateAppleDeviceRequestIdentifySequence;

/*!
 @method    -emulateAccessoryUnsupportedSequence
 @abstract  Emulates a lack of compatibility sequence sent by the accessory
 @return    The list of events sent to emulate a lack of compatibility sequence 
 */
- (iAP2LinkPacketEvent *)emulateAccessoryStartIDPSSequence;

/*!
@method    -emulateAccessoryUnsupportedSequence
@abstract  Emulates an iAP1 Start IDPS sequence sent by the accessory
@return    The list of events sent to emulate an iAP1 Start IDPS sequence
*/
- (iAP2LinkPacketEvent *)emulateAccessoryUnsupportedSequence;

/*!
 @method    -emulateAccessoryiAP2DetectSequence
 @abstract  Emulates an iAP2 detect sequence sent by the accessory
 @return    The list of events sent to emulate an iAP2 Detect sequence
 */
- (iAP2LinkPacketEvent *)emulateAccessoryiAP2DetectSequence;

/*!
 @method    -emulateAppleDeviceiAP2DetectSequence
 @abstract  Emulates an iAP2 detect sequence sent by the apple device
 @return    The list of events sent to emulate an iAP2 Detect sequence
 */
- (iAP2LinkPacketEvent *)emulateAppleDeviceiAP2DetectSequence;

/*!
 @method   -emulateDeviceSleepLinkPacket
 @abstract Emulates a sleep packet sent by the device
 @return   The list of events sent to emulate a sleep packet
 */
- (iAP2LinkPacketEvent *)emulateDeviceSleepLinkPacket;

/*!
 @method    iAP2LinkPacketSynchronizationPayloadWithParameters:
 @abstract  Creates a synchronization payload data with the parameters in the dictionary. The parameters that are not specified by the dicionary are set to the default value
 @param     parameters
            a dictionary that contains the values of the parameters
 @return    A data that contains the link synchronization payload
 */
- (NSData *)iAP2LinkPacketSynchronizationPayloadWithParameters:(NSDictionary *)parameters;

/*!
 @method    emulateAppleDeviceiAP2LinkPacketWithControlByte:packetSequenceNumber:packetAcknowledgementNumber:sessionIdentifier:payloadData:
 @abstract  Emulates an apple device link packet with the specified link version control byte, packet sequence number, packet acknowledgement number, session identifier and payload data.
 @param     controlByte
            the link packet control byte
 @param     packetSequenceNumber
            the link packet sequence number
 @param     packetAcknowledgementNumber
            the link packet acknowledgement number
 @param     sessionIdentifier
            the link packet session identifeier
 @param     payloadData
            the link packet payload
 @return    The list of events sent to emulate a iAP2 Link packet
 */
- (iAP2LinkPacketEvent *)emulateAppleDeviceiAP2LinkPacketWithControlByte:(UInt8)controlByte packetSequenceNumber:(UInt8)packetSequenceNumber packetAcknowledgementNumber:(UInt8)packetAcknowledgementNumber sessionIdentifier:(UInt8)sessionIdentifier payloadData:(NSData *)payloadData;

/*!
 @method    emulateAccessoryiAP2LinkPacketWithControlByte:packetSequenceNumber:packetAcknowledgementNumber:sessionIdentifier:payloadData:
 @abstract  Emulates an accessory link packet with the specified link version control byte, packet sequence number, packet acknowledgement number, session identifier and payload data.
 @param     controlByte
            the link packet control byte
 @param     packetSequenceNumber
            the link packet sequence number
 @param     packetAcknowledgementNumber
            the link packet acknowledgement number
 @param     sessionIdentifier
            the link packet session identifeier
 @param     payloadData
            the link packet payload
 @return    The list of events sent to emulate a iAP2 Link packet
 */
- (iAP2LinkPacketEvent *)emulateAccessoryiAP2LinkPacketWithControlByte:(UInt8)controlByte packetSequenceNumber:(UInt8)packetSequenceNumber packetAcknowledgementNumber:(UInt8)packetAcknowledgementNumber sessionIdentifier:(UInt8)sessionIdentifier payloadData:(NSData *)payloadData;

/*!
 @method    emulateAppleDeviceiAP2ControlLinkPacketWithControlByte:payloadData:
 @abstract  Emulates an apple device link packet with the specified link version control byte and payload data.
 @param     controlByte
 the link packet control byte
 @param     payloadData
 the link packet payload
 @return    The list of events sent to emulate a iAP2 Link packet
 */
- (iAP2LinkPacketEvent *)emulateAppleDeviceiAP2ControlLinkPacketWithControlByte:(UInt8)controlByte payloadData:(NSData *)payloadData;

/*!
 @method    emulateAccessoryiAP2ControlLinkPacketWithControlByte:payloadData:
 @abstract  Emulates an accessory link packet with the specified link version control byte and payload data.
 @param     controlByte
 the link packet control byte
 @param     payloadData
 the link packet payload
 @return    The list of events sent to emulate a iAP2 Link packet
 */
- (iAP2LinkPacketEvent *)emulateAccessoryiAP2ControlLinkPacketWithControlByte:(UInt8)controlByte payloadData:(NSData *)payloadData;

/*!
 @method -emulateiAP2LinkSynchronizationControlFileTransferEASessions
 @abstract Emulates a link packet with a vanilla link synchronization payload supporting Control and File Transfer sessions
 */
- (void)emulateiAP2LinkSynchronizationControlFileTransferSessions;

/*!
 Creates a session payload for a message sent by an accessory.
 
 @param     message A constructed message object.
 @return    A NSData object constructed to represent the session payload.
 */
- (NSData *)accessoryLinkPacketPayloadForiAP2Message:(I2MMessage *)message;

/*!
 Creates a session payload for a message sent by an apple device.
 
 @param     message A constructed message object.
 @return    A NSData object constructed to represent the session payload.
 */
- (NSData *)appleDeviceLinkPacketPayloadForiAP2Message:(I2MMessage *)message;

/*!
 @method -emulateAccessoryiAP2Message:
 @abstract Emulates the accessory sending over the wire an iAP2 message represented by the constructed I2MMessage object
 @param message A constructed message object
 @return An iAP2MessageEvent object constructed by the ATS app reading the raw bytes originating from the emulated messages and then parsing them
 */
- (iAP2MessageEvent *)emulateAccessoryiAP2Message:(I2MMessage *)message;

/*!
 Emulates the accessory sending over Bluetooth an iAP2 message represented by the constructed I2MMessage object and emulates a comprobe frame loss at the specified position
 
 @param message             A constructed message object
 @param frameLostPosition   The position of the frame that should be skipped
 @return An array of events generated by the ATS app reading the raw bytes originating from the emulated messages and then parsing them
 */
- (NSArray *)emulateAccessoryiAP2Message:(I2MMessage *)message skipComprobeFrameAtPosition:(UInt8)frameLostPosition;

/*!
 @method -emulateAccessoryiAP2MessageData:
 @abstract Emulates the accessory sending over the wire an iAP2 message represented by the constructed iAP2 message data
 @param messageData The raw data representing an iAP2 message.
 @return An iAP2MessageEvent object constructed by the ATS app reading the raw bytes originating from the emulated messages and then parsing them
 */
- (iAP2MessageEvent *)emulateAccessoryiAP2MessageData:(NSData *)messageData;

/*!
 @method -emulateAppleDeviceiAP2Message:
 @abstract Emulates the Apple device sending over the wire an iAP2 message represented by the constructed I2MMessage object
 @param message A constructed message object
 @return An iAP2MessageEvent object constructed by the ATS app reading the raw bytes originating from the emulated messages and then parsing them
 */
- (iAP2MessageEvent *)emulateAppleDeviceiAP2Message:(I2MMessage *)message;

/*!
 @method -emulateAppleDeviceiAP2Message:dropTrailingBytes:
 @abstract Emulates the Apple device sending over the wire an iAP2 message represented by the constructed I2MMessage object
 @param message A constructed message object
 @param numDroppedBytes How many link packet bytes to drop
 @return An iAP2MessageEvent object constructed by the ATS app reading the raw bytes originating from the emulated messages and then parsing them
 */
- (iAP2MessageEvent *)emulateAppleDeviceiAP2Message:(I2MMessage *)message dropTrailingBytes:(int)numDroppedBytes;

/*!
 @method -emulateIdentificationInformationWithName:modelIdentifier:manufacturer:serialNumber:firmwareVersion:hardwareVersion:messagesSentByAccessory:messagesReceivedFromDevice:powerProvidingCapability:maximumCurrentDrawnFromDevice:currentLanguage:supportedLanguage:
 @abstract Emulates a basic IdentificationInformation with a minimal set of parameters
 @param name                          The accessory name
 @param modelIdentifier               The accessory's model identifier
 @param serialNumber                  The accessory's serial number
 @param firmwareVersion               The accessory's firmware version
 @param hardwareVersion               The accessory's hardware version
 @param messagesSentByAccessory       An array of NSNumbers containing the IDs of messages sent by the accessory
 @param messagesReceivedFromDevice    An array of NSNumbers containing the IDs of messages received from the device
 @param powerProvidingCapability      An enum type represent what kind of power source the accesssory provides
 @param maximumCurrentDrawnFromDevice The maximum current the device draws
 @param currentLanguage               The accessory's current language
 @return The emulated iAP2MessageEvent
 */
- (iAP2MessageEvent *)emulateIdentificationInformationWithName:(NSString *)name
                                               modelIdentifier:(NSString *)modelIdentifier
                                                  manufacturer:(NSString *)manufacturer
                                                  serialNumber:(NSString *)serialNumber
                                               firmwareVersion:(NSString *)firmwareVersion
                                               hardwareVersion:(NSString *)hardwareVersion
                                       messagesSentByAccessory:(NSArray *)messagesSentByAccessory
                                    messagesReceivedFromDevice:(NSArray *)messagesReceivedFromDevice
                                               powerProvidingCapability:(UInt8)powerProvidingCapability
                                 maximumCurrentDrawnFromDevice:(UInt16)maximumCurrentDrawnFromDevice
                                               currentLanguage:(NSString *)currentLanguage;

/*!
 @method emulateIdentificationInformationWithName:modelIdentifier:manufacturer:serialNumber:firmwareVersion:hardwareVersion:messagesSentByAccessory:messagesReceivedFromDevice:powerProvidingCapability:maximumCurrentDrawnFromDevice:currentLanguage:supportedLanguage:additionalParameters:
 @abstract  Same as above method, but provides the ability to add additional parameters to the identification message
 @params -- See documentation for method without additionalParameters
 @param     params        Additional parameters which may be passed
 @return    The emulated iAP2MessageEvent
 */
- (iAP2MessageEvent *)emulateIdentificationInformationWithName:(NSString *)name
                                               modelIdentifier:(NSString *)modelIdentifier
                                                  manufacturer:(NSString *)manufacturer
                                                  serialNumber:(NSString *)serialNumber
                                               firmwareVersion:(NSString *)firmwareVersion
                                               hardwareVersion:(NSString *)hardwareVersion
                                       messagesSentByAccessory:(NSArray *)messagesSentByAccessory
                                    messagesReceivedFromDevice:(NSArray *)messagesReceivedFromDevice
                                      powerProvidingCapability:(UInt8)powerProvidingCapability
                                 maximumCurrentDrawnFromDevice:(UInt16)maximumCurrentDrawnFromDevice
                                               currentLanguage:(NSString *)currentLanguage
                                          additionalParameters:(I2MParameter *)params, ... NS_REQUIRES_NIL_TERMINATION;

/*!
 @method -emulateIdentificationInformationWithMessagesSentByAccessory:messagesReceivedFromDevice:
 @abstract Emulates an IdentificationInformation using the given lists of message IDs sent and received by the accessory
 @param messagesSentByAccessory An array of NSNumbers containing the IDs of messages sent by the accessory
 @param messagesReceivedFromDevice An array of NSNumbers containing the IDs of messages received from the device
 @return The emulated iAP2MessageEvent
 */
- (iAP2MessageEvent *)emulateIdentificationInformationWithMessagesSentByAccessory:(NSArray *)messagesSentByAccessory
                                                       messagesReceivedFromDevice:(NSArray *)messagesReceivedFromDevice;

/*!
 @method    emulateCATestStarted:
 @abstract  Emulates a CATestStartedEvent with the given test name
 @param     testType
            The test to emulate
 @return    ATSAppEvent which was dispatched
 */
- (CATestStartedEvent *)emulateCATestStarted:(SelfCertTestType)testType;

/*!
 Emulates a CATestStartedEvent with the given tests names
 @param     testTypes
 The collection of test types to be emulated
 @return    ATSAppEvent which was dispatched
 */
- (CATestStartedEvent *)emulateCATestStartedWithTestTypes:(NSArray *)testTypes;

/*!
 @method    emulateCATestFinished:
 @abstract  Emulates a CATestFinishedEvent with the given test name
 @param     testType
            The test to emulate
 @return    ATSAppEvent which was dispatched
 */
- (CATestFinishedEvent *)emulateCATestFinished:(SelfCertTestType)testType;

/*!
 Emulates a CATestFinishedEvent with the given tests names
 @param     testTypes
 The collection of test types to be emulated
 @return    ATSAppEvent which was dispatched
 */
- (CATestFinishedEvent *)emulateCATestFinishedWithTestTypes:(NSArray *)testTypes;

/*!
 @method    emulateVbusLoadTestEvent:
 @abstract  Emulates a USB Vbus load test start/finish event
 @param     state
            State of the load test, either start or finish
 @return    ATSAppEvent which was dispatched
 */
- (VBusLoadTestEvent *)emulateVbusLoadTestEvent:(BOOL)state;

/*!
 @method    emulateAccVoltage:
 @abstract  Emulates an acc voltage event
 @param     voltageInMillivolts
            Voltage in millivolt
 @param     currentInMilliamps
            Current in milliamps
 */
- (EightPinAccVoltageEvent *)emulateAccVoltage:(double)voltageInMillivolts current:(double)currentInMilliamps;

/*!
 @method    emulateAccCurrent:
 @abstract  Emulates an acc current event, which is the current flowing to the accessory
 @param     currentInMilliamps
            Current in milliamps
 @param     voltageInMillivolts
            Voltage in millivolt
 */
- (EightPinAccCurrentEvent *)emulateAccCurrent:(double)currentInMilliamps voltage:(double)voltageInMillivolts;

/*!
 @method    emulateDeviceVoltage:
 @abstract  Emulates a device voltage event
 @param     voltageInMillivolts
            Voltage in millivolt
 @param     currentInMilliamps
            Current in milliamps
 */
- (EightPinDeviceVoltageEvent *)emulateDeviceVoltage:(double)voltageInMillivolts current:(double)currentInMilliamps;

/*!
 @method    emulateDeviceCurrent:
 @abstract  Emulates a device current event, which is the current flowing to the Apple device
 @param     currentInMilliamps
            Current in milliamps
 @param     voltageInMillivolts
            Voltage in millivolt
 */
- (EightPinDeviceCurrentEvent *)emulateDeviceCurrent:(double)currentInMilliamps voltage:(double)voltageInMillivolts;

/*!
 @method    emulateUSBR1Resistor:R2:R3:R4:
 @abstract  Emulates USB pull up/down resistors brick detect
 @param     r1
            D+ pull up (kΩ)
 @param     r2
            D+ pull down (kΩ)
 @param     r3
            D- pull up (kΩ)
 @param     r4
            D- pull down (kΩ)
 */
- (EightPinUSBBrickDetectEvent *)emulateUSBR1Resistor:(double)r1 R2:(double)r2 R3:(double)r3 R4:(double)r4;

/*!
 @method    emulateUSBDataLinesFloating
 @abstract  Emulates floating USB resistors
 */
- (EightPinUSBBrickDetectEvent *)emulateUSBDataLinesFloating;

/*!
 @method    emulateUSBDataLinesTied
 @abstract  Emulates china brick config with D+ and D- tied together
 */
- (EightPinUSBBrickDetectEvent *)emulateUSBDataLinesTied;

/*!
 @method    emulateAccessoryConnected
 @abstract  Emulates an accessory connect event
 */
- (AccessoryConnectEvent *)emulateAccessoryConnected;

/*!
 @method    emulateAccessoryDisconnected
 @abstract  Emulates an accessory disconnect event
 */
- (AccessoryDisconnectEvent *)emulateAccessoryDisconnected;

/*!
 @method    emulateAppleDeviceConnected
 @abstract  Emulates an Apple device connect event
 */
- (AppleDeviceConnectEvent *)emulateAppleDeviceConnected;

/*!
 @method    emulateAppleDeviceDisconnected
 @abstract  Emulates an Apple device disconnect event
 */
- (AppleDeviceDisconnectEvent *)emulateAppleDeviceDisconnected;

/*!
 @method    emulateAccessoryOrientationWithState:
 @abstract  Emulates accessory orientation report
 @param     orientation
 Orientation state
 @param     command
 The command which the frame should be created with
 */
- (EightPinAccOrientationStateEvent *)emulateAccessoryOrientationWithState:(ATFR3AccessoryOrientation)state;

/*!
 @method    emulateSuccessfuliAP2Authentication
 @abstract  Emulates an accessory successfully authenticating via iAP2
 */
- (void)emulateSuccessfuliAP2Authentication;

/*!
 @method    emulateSuccessfuliAP2Authentication
 @abstract  Emulates an accessory successfully identifying via iAP2
 */
- (void)emulateSuccessfuliAP2Identification;

/*!
 @method    emulateAccessoryEASessionDatagramWithSessionIdentifier:transferIdentifier:data:
 @abstract  Emulates an EA session datagram
 @param     sessionIdentifier The link layer session identifier
 @param     transferIdentifer The EA session/transfer identifier
 @param     data The data to send in the payload
 */
- (ATSExternalAccessoryEvent *)emulateAccessoryEASessionDatagramWithSessionIdentifier:(UInt8)sessionIdentifier
                                                                     transferIdentifier:(UInt16)transferIdentifier
                                                                                   data:(NSData *)data;

/*!
 @method    emulateAppleDeviceEASessionDatagramWithSessionIdentifier:transferIdentifier:data:
 @abstract  Emulates an EA session datagram
 @param     sessionIdentifier The link layer session identifier
 @param     transferIdentifer The EA session/transfer identifier
 @param     data The data to send in the payload
 */
- (ATSExternalAccessoryEvent *)emulateAppleDeviceEASessionDatagramWithSessionIdentifier:(UInt8)sessionIdentifier
                                                                       transferIdentifier:(UInt16)transferIdentifier
                                                                                     data:(NSData *)data;

/*!
    Emulates an File Transfer session datagram
    @param     sessionIdentifier The link layer session identifier
    @param     transferIdentifer The File Transfer session/transfer identifier
    @param     data The data to send in the payload
 */
- (iAP2FileTransferDatagramEvent *)emulateAccessoryFileTransferSessionDatagramWithSessionIdentifier:(UInt8)sessionIdentifier
                                                                                 transferIdentifier:(UInt16)transferIdentifier
                                                                                               data:(NSData *)data;

/*!
 @method    emulateSerialTimingStatisticsWithBaudFF:baud55:sourceIsAccessory:
 @abstract  Emulate either a Serial UART1 Timing Statistics or Serial UART2 Timing Statistics
 @param     baudFF
            Baud rate for FF
 @param     baud55
            Baud rate for 55 or 5A
 @param     sourceIsAccessory
            Whether or not the data is from an accessory
 @returns   An array of EightPinSerialBaudRateEvents
 */
- (NSArray *)emulateSerialTimingStatisticsWithBaudFF:(UInt32)baudFF baud55:(UInt32)baud55 sourceIsAccessory:(BOOL)sourceIsAccessory;

/*!
 @method    emulateIDBusRawBreak
 @abstract  Emulate an ID Bus break.
 @returns   An array of IDBusPacketEvents.
 */
- (NSArray *)emulateIDBusRawBreak;

/*!
 @method    emulateIDBusRawBytes:
 @abstract  Emulate an ID Bus byte stream.
 @param     bytesData
            The NSData object containing the bytes in the stream.
 @returns   An array of IDBusPacketEvents
 */
- (NSArray *)emulateIDBusRawBytes:(NSData *)bytesData;

/*!
 @method    emulateIDBusCommand:payload:
 @abstract  Emulates an IDBusCommand with the command prefix and auto-generated CRC
 @param     command
            The command to prepend to the bytes
 @param     payload
            NSData payload
 */
- (IDBusPacketEvent *)emulateIDBusCommand:(IDBCommand)command payload:(NSData *)payload;

/*!
 @method    convertMessageIDArrayToData:messageIDs
 @abstract  convert array of messageIDs to NSData
 @param     An Array of messageIDs
 @returns   NSData object of the messageIDs
 */
- (NSData *)convertMessageIDArrayToData:(NSArray *)messageIDs;

- (NSData *)linkPacketDataForControlByte:(BSiAP2LinkPacketType)controlByte
                    packetSequenceNumber:(UInt8)packetSequenceNumber
             packetAcknowledgementNumber:(UInt8)packetAcknowledgementNumber
                       sessionIdentifier:(UInt8)sessionIdentifier
                             payloadData:(NSData *)payloadData;

- (iAP2LinkPacketEvent *)emulateAccessoryiAP2LinkPacket:(NSData *)linkPacketData;
@end
