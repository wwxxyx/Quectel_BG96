//
//  iAP2Stenograph.h
//  ATSMacApp
//
//  Created by Joe Basu on 7/17/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <iAP2MessageKit/iAP2MessageKit.h>
#import <CaptureKit/iAP2MessageEvent.h>
#import <CaptureKit/iAP2LinkPacketEvent.h>
#import <CaptureKit/Rule.h>
#import <CaptureKit/RuleProtocols_Generated.h>
#import <CaptureKit/iAP2Constants.h>
#import <CaptureKit/TimeConversions.h>
#import <CaptureKit/EightPinHardwareEvent.h>
#import <CaptureKit/ProductPlanTypes.h>
#import <CaptureKit/DeviceCaptureConfigurationTypes.h>
#import <CaptureKit/iAP2SessionIDStore.h>
#import <BusSniffer/BusSniffer.h>
#import <CaptureKit/StenographModule.h>
#import <CaptureKit/iAP2LinkLayerSynchronization.h>
#import <CaptureKit/iAP2LinkLayerInitialization.h>
#import <CaptureKit/ATSExternalAccessoryEvent.h>
#import <CaptureKit/iAP2FileTransferDatagramEvent.h>

@class CaptureEvent;
@class iAP2MessageEvent;
@class iAP2AccessoryIdentificationMessageSequence;
@class EightPinUSBBrickDetectEvent;
@class USBNativeHIDReportDescriptorTransferEvent;
@class USBNativeHIDReportTransferEvent;

@interface iAP2Stenograph : StenographModule

@property (nonatomic, readonly, retain) CaptureEvent *currentEvent;
@property (nonatomic, readonly, retain) iAP2MessageEvent *currentiAP2MessageEvent;
@property (nonatomic, readonly, retain) iAP2LinkPacketEvent *currentiAP2LinkPacketEvent;
@property (nonatomic, readonly, retain) EightPinHardwareEvent *currentEightPinHardwareEvent;

/*!
 @property  isAppleDeviceConnected / isAccessoryConnected
 @abstract  Whether or not the Apple device/accessory is currently connected
 */
@property (nonatomic, readonly, assign) BOOL isAppleDeviceConnected;
@property (nonatomic, readonly, assign) BOOL isAccessoryConnected;

/*!
 @property  justGotFirstAppleDeviceConnectedFrame / justGotFirstAccessoryConnectedFrame
 @abstract  If the first Apple device / accessory connection state frame was seen from the R3 box
 */
@property (nonatomic, readonly, assign) BOOL justGotFirstAppleDeviceConnectedFrame;
@property (nonatomic, readonly, assign) BOOL justGotFirstAccessoryConnectedFrame;

/*!
 @property  seenAppleDeviceConnectionState / seenAccessoryConnectionState
 @abstract  Whether or not Apple device / accessory connection state info has been seen
 */
@property (nonatomic, readonly, assign) BOOL seenAppleDeviceConnectionState;
@property (nonatomic, readonly, assign) BOOL seenAccessoryConnectionState;

/*!
 @property  identifyMessageEvent
 @abstract  The last iAP2IdentificationInformationMessageID message seen
 */
@property (nonatomic, readonly, retain) iAP2MessageEvent *identifyMessageEvent;

/*!
 @property  seenBluetoothIdentificationRejected
 @abstract  Whether or not an iAP2IdentificationRejectedMessageID message has been seen rejecting a Bluetooth component
 */
@property (nonatomic, readonly, assign) BOOL seenBluetoothIdentificationRejected;

/*!
 @property  seenSupportedExternalAccessoryIdentificationRejected
 @abstract  Whether or not an iAP2IdentificationRejectedMessageID message has been seen rejecting a supported external accessory protocol
 */
@property (nonatomic, readonly, assign) BOOL seenSupportedExternalAccessoryIdentificationRejected;

/*!
 @property  seeniAP2HIDComponentIdentificationRejected
 @abstract  Whether or not an iAP2IdentificationRejectedMessageID message has been seen rejecting iAP2 HID
 */
@property (nonatomic, readonly, assign) BOOL seeniAP2HIDComponentIdentificationRejected;

/*!
 @property  seenUSBHostHIDComponentIdentificationRejected
 @abstract  Whether or not an iAP2IdentificationRejectedMessageID message has been seen rejecting USB Host HID
 */
@property (nonatomic, readonly, assign) BOOL seenUSBHostHIDComponentIdentificationRejected;

/*!
 @property  seenAttemptediAP2HIDComponentIdentification
 @abstract  Whether or not the accessory has attempted to identify for an iAP2 HID component. This is not reset by re-attempts at identification
 */
@property (nonatomic, readonly, assign) BOOL seenAttemptediAP2HIDComponentIdentification;

/*!
 @property  seenAttemptedUSBHostHIDComponentIdentification
 @abstract  Whether or not the accessory has attempted to identify for an USB Host HID component. This is not reset by re-attempts at identification
 */
@property (nonatomic, readonly, assign) BOOL seenAttemptedUSBHostHIDComponentIdentification;

/*!
 @property  messagesSeenDuringAllCATests
 @abstract  Set when a CA test has just finished. Contains name of test which finished, and an array of all messages seen during the test
 */
@property (nonatomic, readonly, retain) NSMutableArray *messagesSeenDuringAllCATests;

/*!
 @property  accessoryIdentificationSequence / accessoryIdentificationSequenceCA
 @abstract  The accessory identification sequence for this session and for the accessory identification CA
 */
@property (nonatomic, readonly, retain) iAP2AccessoryIdentificationMessageSequence *accessoryIdentificationSequence;
@property (nonatomic, readonly, retain) iAP2AccessoryIdentificationMessageSequence *accessoryIdentificationSequenceCA;

/*!
 @property  lastStartLocationInformationMessageEvent and lastStopLocationInformationMessageEvent
 @abstract  The last start/stop location information messages seen from the Apple Device
 */
@property (nonatomic, readonly, retain) iAP2MessageEvent *lastStartLocationInformationMessageEvent;
@property (nonatomic, readonly, retain) iAP2MessageEvent *lastStopLocationInformationMessageEvent;

/*!
 @property  appleDockConnectoriPadAccCurrentOutOfSpec / appleDockConnectoriPadAccCurrentOutOfSpecJustChanged
 @abstract  If the current provided in recent samples is out of spec, and if it just became out of spec
 */
@property (nonatomic, readonly, assign) BOOL appleDockConnectoriPadAccCurrentOutOfSpec;
@property (nonatomic, readonly, assign) BOOL appleDockConnectoriPadAccCurrentOutOfSpecJustChanged;

/*!
 @property  appleDockConnectoriPodiPhoneAccCurrentOutOfSpec / appleDockConnectoriPodiPhoneAccCurrentOutOfSpecJustChanged
 @abstract  If the current provided in recent samples is too low, and if it just became too low
 */
@property (nonatomic, readonly, assign) BOOL appleDockConnectoriPodiPhoneAccCurrentOutOfSpec;
@property (nonatomic, readonly, assign) BOOL appleDockConnectoriPodiPhoneAccCurrentOutOfSpecJustChanged;

/*!
 @property  appleDockConnectorAccVoltageOutOfSpec / appleDockConnectorAccVoltageOutOfSpecJustChanged
 @abstract  If the Voltage provided in recent samples is not 5 V and if it just became out of spec
 */
@property (nonatomic, readonly, assign) BOOL appleDockConnectorAccVoltageOutOfSpec;
@property (nonatomic, readonly, assign) BOOL appleDockConnectorAccVoltageOutOfSpecJustChanged;

/*!
 @property  nonAppleDockConnectoriPadAccVoltageOutOfSpec / nonAppleDockConnectoriPadAccVoltageOutOfSpecJustChanged
 @abstract  If the Voltage provided in recent samples is not 4.97 V and if it just became out of spec
 */
@property (nonatomic, readonly, assign) BOOL nonAppleDockConnectoriPadAccVoltageOutOfSpec;
@property (nonatomic, readonly, assign) BOOL nonAppleDockConnectoriPadAccVoltageOutOfSpecJustChanged;

/*!
 @property  nonAppleDockConnectoriPhoneiPodAccVoltageOutOfSpec / nonAppleDockConnectoriPhoneiPodAccVoltageOutOfSpecJustChanged
 @abstract  If the Voltage provided in recent samples is not 4.97 V and if it just became out of spec
 */
@property (nonatomic, readonly, assign) BOOL nonAppleDockConnectoriPhoneiPodAccVoltageOutOfSpec;
@property (nonatomic, readonly, assign) BOOL nonAppleDockConnectoriPhoneiPodAccVoltageOutOfSpecJustChanged;

/*!
 @property  accessoryCurrentDrawOverLimit / accessoryCurrentDrawOverLimitJustChanged
 @abstract  If the accessory current draw ever goes over 100mA
 */
@property (nonatomic, readonly, assign) BOOL accessoryCurrentDrawOverLimit;
@property (nonatomic, readonly, assign) BOOL accessoryCurrentDrawOverLimitJustChanged;

/*!
 @property  accessoryCurretDrawAfterSleep
 @abstract  If the accessory current draw goes over 0 mA after device started sleeping
 */
@property (nonatomic, readonly, assign) BOOL accessoryCurretDrawAfterSleep;

/*!
 @property  startOrStopDevicePowerAccessoryTimestamp
 @abstract  Timestamp of the first occurrence/absence of accessory current drawn
 */
@property (nonatomic, readonly, assign) UInt64 startOrStopDevicePowerAccessoryTimestamp;

/*!
 @property  accessoryCurrentDrawOver5mA / accessoryCurrentDrawOver5mAJustChanged
 @abstract  If the accessory current draw ever goes over 5 mA
 */
@property (nonatomic, readonly, assign) BOOL accessoryCurrentDrawOver5mA;
@property (nonatomic, readonly, assign) BOOL accessoryCurrentDrawOver5mAJustChanged;

/*!
 @property  accessoryCurrentDrawOverDeclaredMaximum / accessoryCurrentDrawOverDeclaredMaximumJustChanged
 @abstract  If the accessory current draw ever goes over what it declared during Identification
 */
@property (nonatomic, readonly, assign) BOOL accessoryCurrentDrawOverDeclaredMaximum;
@property (nonatomic, readonly, assign) BOOL accessoryCurrentDrawOverDeclaredMaximumJustChanged;

/*!
 @property  lastUSBBrickDetectEvent
 @abstract  The last USB brick detect event seen
 */
@property (nonatomic, readonly, retain) EightPinUSBBrickDetectEvent *lastUSBBrickDetectEvent;

/*!
 @property  lastAccessoryAuthenticationCertificateMessage
 @abstract  The last iAP2AuthenticationCertificateMessage message
 */
@property (nonatomic, readonly, retain) iAP2MessageEvent *lastAccessoryAuthenticationCertificateMessage;

/*!
 @property   currentUSBNativeHIDReportDescriptorTransferEvent
 @abstract   Set when a USBNativeHIDReportDescriptorTransferEvent event is being processed.
 */
@property (nonatomic, readonly, retain) USBNativeHIDReportDescriptorTransferEvent *currentUSBNativeHIDReportDescriptorTransferEvent;

/*!
 @property  currentUSBNativeHIDReportTransferEvent
 @abstract  Set when a USBNativeHIDReportTransferEvent event is being processed.
 */
@property (nonatomic, readonly, retain) USBNativeHIDReportTransferEvent *currentUSBNativeHIDReportTransferEvent;

/*!
 @property  inputHIDReportUsagesNotSeenDuringAllCATests
 @abstract  The collection of usages corresponding to the HID report fields for which we expect to see
            values during the CA tests.
 */
@property (nonatomic, readonly, retain) NSMutableDictionary *inputHIDReportUsagesNotSeenDuringAllCATests;

//An array containing all invalid link packets identified during test execution.
@property (nonatomic, readonly, retain) NSMutableArray *invalidLinkPackets;

/*!
    The list of game controller switch usages maximum and minimum values seen during all the CA tests.
 */
@property (nonatomic, readonly, retain) NSMutableDictionary *gameControllerSwitchesUsagesValuesNotSeenSeenDuringAllCATests;

@property (nonatomic, readonly, retain) NSDictionary *appleApps;

/*!
 @method    -iap2HIDReportDescriptorForComponentIdentifier:
 @abstract  Returns the BSUSBHIDReportDescriptor corresponding to the componentIdentifier.
 @param     componentIdentifier
            The componentIdentifier sent in the corresponding StartHID message.
 @discussion Stenographer tracks HID report descriptors sent in StartHID messages and keys them by
             component identifiers. This method returns the StartHID's corresponding HID report descriptpor.
 */
- (BSUSBHIDReportDescriptor *)iap2HIDReportDescriptorForComponentIdentifier:(UInt16)componentIdentifer;

- (void)processEvent:(CaptureEvent *)event;

/*
 @method    accessoryDeclaredSupportForAtLeastOneMessageIDs:
 @abstract  Check whether messages send by accessory or receive from device include at least one of the required message IDs
 @param     messagesIDs
            
 @return    YES if include otherwise NO
 */
- (BOOL)accessoryDeclaredSupportForAtLeastOneMessageIDs:(NSArray *)messageIDs;

/*!
 @method    sawiAP2MessageDuringFinishedCATest:
 @abstract  Determines whether or not an iAPMessage with this ID was seen during the just finished test
 @see       currentCATestJustFinishedName
 @return    Whether or not this message was seen during the CA Test which just finished
 */
- (BOOL)sawiAP2MessageDuringFinishedCATest:(UInt16)messageId;

/*!
 @method    allMessagesSeenDuringAllCATests
 @abstract  Returns all messages seen during the CA run
 */
- (NSArray *)messagesSeenDuringAllCATestsWithID:(UInt16)messageID;

/*!
 @method    nameAndIDForMessageID:
 @abstract  Converts a message ID into a user presentable string with the ID
 @param     messageID 
            The message ID to convert.
 */
- (NSString *)nameAndIDForMessageID:(UInt16)messageId;

/*!
 @method    nameForMessageID:
 @abstract  Converts a message ID into a user presentable string
 @return    User presentable name for message
 */
- (NSString *)nameForMessageID:(UInt16)messageId;

/*!
 @method    currentCATestTypes
 @abstract  Accessor for a list of the currently active running CA types. See SelfCertTestType
 @return    NSArray of test names
 */
- (NSArray *)currentCATestTypes;

/*!
 Analyzes if the list optional message IDs were seen during the just-finished test. If any were missing, it returns an array containing the user presentable name of each missing message
 @param     optionalMessageIDNumbers
            Array of optional message ids
 @return    List of user presentable name of each missing message
 */
- (NSArray *)missingiAP2OptionalMessagesDuringFinishedCATest:(NSArray *)optionalMessageIDNumbers;

/*!
 Analyzes if the list message IDs were seen during the just-finished test. If any were missing, it returns an array containing the user presentable name of each missing message
 @param     messages
            Array of optional message ids
 @return    List of user presentable name of each missing message
 */
- (NSArray *)missingiAP2MessagesDuringFinishedCATest:(NSArray *)messages;

/*!
 @method        sawAnyiAP2MessagesDuringFinishedCATest:count:
 @abstract      Returns whether or not ANY of these messages were seen during the just-finished test
 @discussion    IMPORTANT: This differs from sawiAP2MessagesDuringFinishedCATest:count:missing: in that it's an OR instead of an AND
 @param         messages
                Array of message ids
 @param         count
                How many message ids are in the messages array
 @return        Whether or not any of the passed in messages were seen
 */
- (BOOL)sawAnyiAP2MessagesDuringFinishedCATest:(const UInt16[])messages count:(unsigned int)count;

/*!
 @method    declaredBluetoothComponents
 @abstract  Parses identifyMessageEvent and returns an array of I2MParameters which are the bluetooth components
 @return    Array of declared bluetooth components
 */
- (NSArray *)declaredBluetoothComponents;

/*!
 @method    supportedExternalAccessoryProtocols
 @abstract  Parses identifyMessageEvent and returns an array of I2MParameters which are the supported external accessory protocols
 @return    Array of supported external accessory protocols
 */
- (NSArray *)supportedExternalAccessoryProtocols;

/*!
 @method    identifiediAP2HIDComponents
 @abstract  Parses identifyMessageEvent and returns an array of I2MParameters which are the identified iAP2 HID components
 @return    Array of identified iAP2 HID components
 */
- (NSArray *)identifiediAP2HIDComponents;

/*!
 @method    firstiAP2HIDComponentForComponentIdentifier
 @abstract  Returns the first iAP2HIDComponent parameter from IdentificationInformation that matches the
            given componentIdentifier.
 @param     componentIdentifier
            The componentIdentifier the matching parameter will have.
 */
- (I2MGroupParameter *)firstiAP2HIDComponentForComponentIdentifier:(UInt16)componentIdentifier;

/*!
 @method    firstiAP2HIDComponentForComponentIdentifier:hidComponentFunction:
 @abstract  Returns the first iAP2HIDComponent parameter from IdentificationInformation that matches the
            given componentIdentifier and has the given HID component function.
 @param     componentIdentifier
            The componentIdentifier the matching parameter will have.
 @param     hidComponentFunction
            The hidComponentFunction the matching parameter will have.
 */
- (I2MGroupParameter *)firstiAP2HIDComponentForComponentIdentifier:(UInt16)componentIdentifier hidComponentFunction:(iAP2HIDComponentFunction)hidComponentFunction;

/*!
 @method    firstUSBHostHIDComponentForInterfaceNumber
 @abstract  Returns the first usbHostHIDComponent parameter from IdentificationInformation that matches the
            given interfaceNumber.
 @param     interfaceNumber
            The interfaceNumber the matching parameter will have.
 */
- (I2MGroupParameter *)firstUSBHostHIDComponentForInterfaceNumber:(UInt16)interfaceNumber;

/*!
 @method    firstUSBHostHIDComponentForInterfaceNumber:hidComponentFunction:
 @abstract  Returns the first usbHostHIDComponent parameter from IdentificationInformation that matches the
            given interfaceNumber and has the given HID component function.
 @param     interfaceNumber
            The interfaceNumber the matching parameter will have.
 @param     hidComponentFunction
            The hidComponentFunction the matching parameter will have.
 */
- (I2MGroupParameter *)firstUSBHostHIDComponentForInterfaceNumber:(UInt16)interfaceNumber hidComponentFunction:(iAP2HIDComponentFunction)hidComponentFunction;

/*!
 @method    identifiedUSBHostHIDComponents
 @abstract  Parses identifyMessageEvent and returns an array of I2MParameters which are the identified USB Host HID components
 @return    Array of identified USB Host HID components
 */
- (NSArray *)identifiedUSBHostHIDComponents;

/*!
 @method    identifiedUSBNativeHIDInterfaces
 @abstract  Parses identifiedUSBHostHIDComponents and returns an array of BSUSBInterfaceDescriptor which are the identified USB native HID interfaces
 @return    Array of identified USB native HID interfaces
 */
- (NSArray *)identifiedUSBNativeHIDInterfaces;

/*!
 @method    identifiedPowerProvidingCapability
 @abstract  Which power-providing capability the accessory identified for.
 */
- (iAP2PowerProvidingCapability)identifiedPowerProvidingCapability;

/*!
 @method    didIdentifyForMaximumCurrentDrawFromDevice
 @abstract  Whether or not the accessory included MaximumCurrentDrawFromDevice in the most recent identification information
 */
- (BOOL)didIdentifyForMaximumCurrentDrawFromDevice;

/*!
 @method        identifiedMaximumCurrentDrawFromDevice
 @abstract      What the accessory said their maximum current draw was during identification, in mA
 @discussion    Caller should check -didIdentifyForMaximumCurrentDrawFromDevice == YES before calling this
 @return        Current in mA
 */
- (UInt16)identifiedMaximumCurrentDrawFromDevice;

/*!
 @property  startIdentificationSeen
 @abstract  Indicates whether or not a StartIdentification message has been received from the Apple device
 */
@property (nonatomic, readonly, assign) BOOL startIdentificationSeen;

/*!
 @property  declaredMessagesSentByAccessory
 @abstract  An array of NSNumbers of the message IDs of the messages the accessory indicates it is capable of sending
 */
@property (nonatomic, readonly, retain) NSArray *declaredMessagesSentByAccessory;

/*!
 @property  declaredMessagesReceivedFromDevice
 @abstract  An array of NSNumbers of the message IDs of the messages the accessory indicates it is capable of receiving
 */
@property (nonatomic, readonly, retain) NSArray *declaredMessagesReceivedFromDevice;

/*!
    An array that contains the File Transfer Start Message IDs with File Transfer session which have been sent during the CA
 */
@property (nonatomic, readonly, retain) NSMutableArray *fileTransferStartMessagesSeenDuringCA;

/*!
 @method    recentHardwareEventsOfClass:
 @abstract  Returns array of recent hardware events with the given class. See kNumRecentHardwareEvents for max size
 */
- (NSArray *)recentHardwareEventsOfClass:(Class)hardwareClass;

/*!
 @property accessoryPowerMode
 @abstract Returns the powerMode as returned by the Apple device in an PowerUpdate message or iAP2UndefinedAccessoryPowerMode if not yet set, or intermittent high power if a trigger for that is active
 */
@property (nonatomic, readonly, assign) iAP2AccessoryPowerModes accessoryPowerMode;

/*!
 @property previousAccessoryPowerMode
 @abstract Returns the previous value of the accessoryPowerMode property
 */
@property (nonatomic, readonly, assign) iAP2AccessoryPowerModes previousAccessoryPowerMode;

/*!
 @property accessoryPowerModeJustChanged
 @abstract Set when accessoryPowerMode was just set
 */
@property (nonatomic, readonly, assign) BOOL accessoryPowerModeJustChanged;

/*!
 @property locationInformationUpdatesActive
 @abstract Set if Location updates are active
 */
@property (nonatomic, readonly, assign) BOOL locationInformationUpdatesActive;

/*!
    Set when a file transfer datagram was seen during the CA.
 */
@property (nonatomic, readonly, assign) BOOL fileTransferDatagramSeen;

/*!
    Set when an external accessory datagram was seen during the CA.
 */
@property (nonatomic, readonly, assign) BOOL externalAccessoryDatagramSeen;

/*!
 @method    accessoryAuthenticationCertificateSerial
 @abstract  Extracts accessory authentication certificate serial from lastAccessoryAuthenticationCertificateMessage
 */
- (NSString *)accessoryAuthenticationCertificateSerial;

/*!
 @method    accessoryAuthenticationCertificateClass
 @abstract  Extracts accessory authentication certificate class from lastAccessoryAuthenticationCertificateMessage
 @return    Certificate class, or -1 on error
 */
- (SInt16)accessoryAuthenticationCertificateClass;

/*!
 @property sessionIDStore
 @abstract Session information obtained from the most recent link synchronization
 */
@property (nonatomic, readonly, retain) iAP2SessionIDStore *sessionIDStore;

/*!
 @property openEASessionIdentifiers
 @abstract An array of NSNumbers represent open EA session identifiers
 */
@property (nonatomic, readonly, retain) NSMutableSet *openEASessionIdentifiers;

/*!
 @property currentConfigurationDescriptor
 @abstract The current USB configuration descriptor
 */
@property (nonatomic, readonly, retain) BSUSBConfigurationDescriptor *currentConfigurationDescriptor;

/*!
 @property stringDescriptorStringsByIndex
 @abstract A map of USB string descriptor strings by their index
 */
@property (nonatomic, readonly, retain) NSMutableDictionary *stringDescriptorStringsByIndex;

/*!
 @property stringDescriptorIndexesRequiredForEANativeTransportProtocols
 @abstract A list of string descriptor indexes required for EA protocols
 */
@property (nonatomic, readonly, retain) NSMutableSet *stringDescriptorIndexesRequiredForEANativeTransportProtocols;

/*!
 @property  appleDevicePackets
 @abstract  The collection of all apple device packets sent during the test.
 */
@property (nonatomic, readonly, retain) NSMutableArray *appleDevicePackets;

/*!
 @property  accessoryPackets
 @abstract  The collection of all accessory packets sent during the test.
 */
@property (nonatomic, readonly, retain) NSMutableArray *accessoryPackets;

/*!
 @property  accessoryPacketEvents
 @abstract  The collection of all accessory packet events sent during the test. If there is a rollover then this collection is reset.
 */
@property (nonatomic, readonly, retain) NSMutableOrderedSet *accessoryEventsSinceLastRollOver;

/*!
 The last accessory packet that acked an apple device packet
 */
@property (nonatomic, readonly, retain) BSiAP2LinkPacket *lastAccessoryPacketThatAckedAppleDevicePacket;

/*!
 @property lastPacketSequenceNumberAckedByAppleDevice
 @abstract The sequence number of the last packet acked by the apple device.
 */
@property (nonatomic, readonly, assign) NSInteger lastPacketSequenceNumberAckedByAppleDevice;

/*!
    The state of the link synchronization operation
 */
@property (nonatomic, readonly, retain) iAP2LinkLayerSynchronization *iAP2LinkLayerSynchronization;

/*!
    The state of the link initialization operation
 */
@property (nonatomic, readonly, retain) iAP2LinkLayerInitialization *iAP2LinkLayerInitialization;

/*!
 A map from media library identifiers to mutable arrays of StartMediaLibraryUpdates events that don't include a last known library revision.
 */
@property (nonatomic, readonly, retain) NSMutableDictionary<NSString *, NSMutableArray<iAP2MessageEvent *> *> *startMediaLibraryUpdatesWithNoLastKnownRevision;

/*!
 @method   currentConfigurationDescriptorReadyForEAValidation
 @abstract Returns whether or not all of the string descriptors required for EA descriptors have been fetched and the configuration descriptor is ready to be validated
 @return   YES if all the required string descriptors have been fetched, NO otherwise
 */
- (BOOL)currentConfigurationDescriptorReadyForEAValidation;

/*!
 @method   validateCurrentCurrentConfigurationDescriptorForEANativeTransportProtocolStringIndex:
 @abstract Validate the current configuration descriptor for EA native transport, given the string descriptor index of the EA protocol index being validated
 @return   nil if there are no validation errors, otherwise a string indicating the first validation error found
 */
- (NSString *)validateCurrentCurrentConfigurationDescriptorForEANativeTransportProtocolStringIndex:(UInt8)stringDescriptorIndex;

/*!
 @method    permittedConsumerPageUsagesForHIDFunction
 @abstract  Returns a set of usage ID's which are permitted by the given hid component function.
 @param     hidComponentFunction
            The HID component function to get the set of permitted usages for.
 */
- (NSSet *)permittedConsumerPageUsagesForHIDFunction:(iAP2HIDComponentFunction)hidComponentFunction;

/*!
    Returns an array of usage ID's which are prohibited by the given hid component function and the given HID report.
    @param     fields The report fields
    @param     hidComponentFunction The HID component function to get the set of permitted usages for.
 */
- (NSArray *)invalidUsagesSeenForReportFields:(NSArray *)fields hidComponentFunction:(iAP2HIDComponentFunction)hidComponentFunction;

/*!
 @method    prohibitedUsageStringsForUsagePage:permittedUsages:hidReportDescriptorItems:
 @abstract  Returns an array of formatted strings containing the ID and name for each usage under the given usage page
            which is not in the permittedUsages set.
 @param     usagePage
            The usage page for the permitted usages.
 @param     permittedUsages
            The set of usages that are permitted to be set in the HID report descriptor.
 @param     hidReportDescriptorItems
            The HID report descriptor items.
 */
- (NSArray *)prohibitedUsageStringsForUsagePage:(UInt32)usagePage permittedUsages:(NSSet *)permittedUsages hidReportDescriptorItems:(NSArray *)hidReportDescriptorItems;

/*!
 @method        recentRevisionsForMediaLibrary
 @abstract      Returns an array of recent media library revisions which are suitable for inclusion in StartMediaLibraryUpdates for the given media library.
 @discussion    This will return nil if there are no known revisions. The "recent" timespan is arbitrary and is subject to change.
 */
- (NSArray *)recentRevisionsForMediaLibrary:(NSString *)mediaLibraryUniqueId;

/*!
 @method        enumerateUnknownParameterIDsFromParameters:results:
 @abstract      Enumerates the I2MParameter instances in "params" and for any unknown parameter seen places the path to the unknown ID in results
 */
+ (void)enumerateUnknownParameterIDsFromParameters:(NSArray *)params results:(NSMutableArray **)results;

/*!
 @method    -identifiedUSBNativeHIDInterfaceWithInterfaceNumber:
 @abstract  Returns the HID interface that has a corresponding USBHostHIDComponent with the given interface number.
 @param     interfaceNumber
            The interface number.
 */
- (BSUSBInterfaceDescriptor *)identifiedUSBNativeHIDInterfaceWithInterfaceNumber:(UInt8)interfaceNumber;

/*!
 @method    -usbNativeHIDReportDescriptorTransferEventForInterfaceNumber:
 @abstract  Returns the HID report descriptor event associated with the given interface number.
 @param     interfaceNumber
            The interface number.
 */
- (USBNativeHIDReportDescriptorTransferEvent *)usbNativeHIDReportDescriptorTransferEventForInterfaceNumber:(UInt8)interfaceNumber;

/*!
 @method    -usbNativeHIDReportDescriptorTransferEventForInterruptEndpointNumber:
 @abstract  Finds the HID interface that was identified for (has corresponding USBHostHIDComponent) which has the given endpoint
            and direction and returns the associated HID report descriptor event.
 @param     endpoint
            The endpoint number the Interrupt trasfer was sent over.
 @param     direction
            The endpoint direction the Interrupt transfer was sent over.
 */
- (USBNativeHIDReportDescriptorTransferEvent *)usbNativeHIDReportDescriptorTransferEventForInterruptEndpointNumber:(UInt8)endpoint endpointDirection:(BSUSBEndpointDirection)direction;

/*!
 @method    -populateMissingInputHIDReportUsagesWithReportDescriptor:componentIdentifier:
 @abstract  Populates the list of HID input reports usages for which values have not
            been seen during CA tests using the given HID report descriptor.
 @param     reportDescriptor
            The HID report descriptor to examine in order to know what fields 
            to expect values for.
 @param     componentIdentifier
            The identifier for the HID component associated with the report 
            descriptor.
 */
- (void)populateMissingInputHIDReportUsagesWithReportDescriptor:(BSUSBHIDReportDescriptor *)reportDescriptor componentIdentifier:(UInt16)componentIdentifier;

/*!
 @method    -updateMissingInputHIDReportFieldValues:componentIdentifier:
 @abstract  Updates the list of HID report fields for which values have not been 
            seen during CA tests, by removing any of the fields for which the 
            given collection contains a non-zero value.
 @param     latestFieldValues
            The HID report field values to use for updating the missing field
            list.
 @param     componentIdentifier
            The identifier for the HID component associated with the report 
            field values.
 */
- (void)updateMissingInputHIDReportUsages:(NSArray *)latestFieldValues componentIdentifier:(UInt16)componentIdentifier;

/*!
 @method    -stringsForMissingInputHIDReportFieldValues
 @abstract  Returns a collection of strings describing the HID report fields for which values were not observed in AccessoryHIDReport
            messages during CA tests.
 */
- (NSArray *)stringsForMissingInputHIDReportFieldValues;

/*!
 @method    -getUSBHostHIDComponentIdentifier:forHIDInterfaceWithIdentifier:
 @abstract  Looks up the identifier for the HID component associated with the 
            HID interface with the given identifier.
 @param     componentIdentifier
            If a component identifier is found, on exit this will point to its 
            value.
 @param     interfaceIdentifier
            The identifier for the HID interface associated with the HID 
            component whose identifier is being looked for.
 @return    YES if a component identifier was found; NO otherwise.
 */
- (BOOL)getUSBHostHIDComponentIdentifier:(UInt16 *)componentIdentifier forHIDInterfaceWithIdentifier:(UInt16)interfaceIdentifier;

/*!
 @method    isHIDComponentFunctionGamepad:
 @abstract  Returns whether the given hidComponentFunction is a GameController.
 */
- (BOOL)isHIDComponentFunctionGamepad:(iAP2HIDComponentFunction)hidComponentFunction;

/*!
 @method    permittedHIDReportFieldUsagesForGamepad:
 @abstract  Returns an array of permitted BSUSBHIDReportFieldUsages for the passed hidComponentFunction
 */
- (NSArray *)permittedHIDReportFieldUsagesForGamepad:(iAP2HIDComponentFunction)hidComponentFunction;

/*!
 @method    isHIDComponentFunctionGamepadFormFitting:
 @abstract  Returns whether the given hidComponentFunction is a GameController form fitting.
 */
- (BOOL)isHIDComponentFunctionGamepadFormFitting:(iAP2HIDComponentFunction)hidComponentFunction;

/*!
 @method    gameControllerLEDArrayUsages:
 @abstract  Returns the set of all HID GameController LED usages (BSUSBHIDReportFieldUsage).
 */
- (NSSet *)gameControllerLEDArrayUsages;

/*
 @method    unseenUsagesForRequiredUsages:hidReportType:inHIDReportDescriptor:
 @abstract  Returns the set of all HID GameController LED usages (BSUSBHIDReportFieldUsage) that were
            not set in the given HID report descriptor for the given HID report type.
 */
- (NSSet *)unseenUsagesForRequiredUsages:(NSSet *)usages hidReportType:(BSUSBHIDReportType)hidReportType inHIDReportDescriptor:(BSUSBHIDReportDescriptor *)hidReportDescriptor;

/*!
 @method    sentenceTypeForNMEASentence:
 @abstract  Parse the sentence type out of an NMEA sentence
 @param     sentence    The NMEA sentence
 @return    Type as a string, such as @"GPGGA", or nil if type is not parsable
 */
- (NSString *)sentenceTypeForNMEASentence:(NSString *)sentence;

/*!
 @method        recentLocationInformationMessagesBySentenceType
 @abstract      Return an dictionary of LocationInformation messages keyed by their sentence types
 @discussion    { "SentenceTypeA": an iAP2MessageEvent LocationInformation which contains a sentence of this type,
                  "SentenceTypeB": an iAP2MessageEvent LocationInformation which contains a sentence of this type }
 */
- (NSDictionary *)recentLocationInformationMessagesBySentenceType;

/*!
 @method    minimumIntervalForNMEASentenceOfType:
 @abstract  Determine minimum interval for the NMEA sentence of the passed type
 @return    Minimum interval in nanoseconds, or 0 if unknown sentence type
 */
- (UInt64)minimumIntervalForNMEASentenceOfType:(NSString *)sentenceType;

/*!
 @method    getSwitchControlComponentButtonCountFromUSBHostHIDComponent:
 @abstract  Returns the quantity of button in a USB Host Switch Control Component
 @param     usbHostHIDComponentParameter    The USB host component parameter
 @return    the button count in a Switch Control Component
 */
- (NSInteger)getSwitchControlComponentButtonCountFromUSBHostHIDComponent:(I2MGroupParameter *)usbHostHIDComponentParameter;

/*!
 @method    getSwitchControlComponentButtonCountFromiAP2HIDComponent:
 @abstract  Returns the quantity of button in a iAP2 Switch Control Component
 @param     iAP2HIDComponentParameter    The iAP2 component parameter
 @return    the button count in a Switch Control Component
 */
- (NSInteger)getSwitchControlComponentButtonCountFromiAP2HIDComponent:(I2MGroupParameter *)iAP2HIDComponentParameter;

/*!
 @method    requiredGameControllerControlsForHIDComponentFunction:
 @abstract  Returns the set of game controller controls that are required for the specifief function
 @param     hidComponentFunction    The HID component function
 @return    the set of required game controller controls
 */
- (NSSet *)requiredGameControllerControlsForHIDComponentFunction:(iAP2HIDComponentFunction)hidComponentFunction;

/*!
 @method    gameControllerPressureSensitiveSwitches
 @abstract  Returns the set of all game controller pressure sensistive switches
 @return    the set of game controller pressure sensistive switches
 */
- (NSSet *)gameControllerPressureSensitiveSwitches;

/*!
 @method    gameControllerJoystickControls
 @abstract  Returns the set of all game controller joystick controls
 @return    the set of game controller joystick controls
 */
- (NSSet *)gameControllerJoystickControls;

/*!
 @method    isOneBitValue:
 @abstract  Verify if the field is defined as a 1-bit value
 @param     reportFieldStructure    The structure of the report field
 @return    YES if the field is 1-bit value, NO otherwise
 */
- (BOOL)isOneBitValue:(BSUSBHIDReportFieldStructure *)reportFieldStructure;

/*!
 @method    isGameControllerJoystickReportStructureProperlyDefined:
 @abstract  Verify if the field is defined correctly to a joystick control
 @param     reportFieldStructure    The structure of the report field
 @return    YES if the field is correct, NO otherwise
 */
- (BOOL)isGameControllerJoystickReportStructureCorrectlyDefined:(BSUSBHIDReportFieldStructure *)reportFieldStructure;

/*!
 @method    isGameControllerPressureSensitiveControlCorrectlyDefined:
 @abstract  Verify if the field is defined correctly to a pressure sensitive control
 @param     reportFieldStructure    The structure of the report field
 @return    YES if the field is correct, NO otherwise
 */
- (BOOL)isGameControllerPressureSensitiveControlCorrectlyDefined:(BSUSBHIDReportFieldStructure *)reportFieldStructure;

/*!
 @method    areOpposingCardinalDirectionButtonsSet:
 @abstract  Verify if the report returns opposing cardinal direction buttons with non-zero value
 @param     hidReportFields    The report array of fields
 @return    YES if opposing button are set, NO otherwise
 */
- (BOOL)areOpposingCardinalDirectionButtonsSet:(NSArray *)hidReportFields;

@end
