//
//  Stenographer.h
//  ATS
//
//  Created by Marin Balde on 7/13/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CaptureKit/TimeConversions.h>
#import <CaptureKit/ProductPlanTypes.h>
#import <CaptureKit/StenographModule.h>

@class Rule;
@class RuleError;
@class Event;
@class iAP1PacketEvent;
@class ATFRetHardwareInfoEvent;
@class USBResetEvent;
@class LKTToken;
@class LKTCommand;
@class LKTCommandDefinition;
@class CertificateCollection;
@class FIDTokenCollection;
@class MultipleSectionTransactionCollection;
@class TransactionIDContext;
@class AccessoryConnectEvent;
@class AppleDeviceConnectEvent;
@class LingoProtocolVersionCollection;
@class AccessoryDataTransferContext;
@class USBHIDReportCollection;
@class EightPinUSBBrickDetectEvent;
@class ATSEightPinStenograph;
@class EightPinHardwareEvent;

// timestamp tolerance
extern const UInt64 tolerance;
// voltage tolerance
extern const double voltageTolerance;

enum DBCategoryTypes {
    CT_FirstValidCategory = 0,
    CT_TopLevel = 0,
    CT_Playlist = 1,
    CT_Artist = 2,
    CT_Album = 3,
    CT_Genre = 4,
    CT_Track = 5,
    CT_Composer = 6,
    CT_Audiobook = 7,
    CT_Podcast = 8,
    CT_NestedList = 9,
    CT_GeniusPlaylist = 10,
    CT_iTunesU = 11,
    CT_LastValidCategory = 11,
};

enum {
    USBChargingPowerSourceTypeInvalid,
    USBChargingPowerSourceTypeNone,
    USBChargingPowerSourceType500mA,
    USBChargingPowerSourceType1A,
    USBChargingPowerSourceType2Point1A,
    USBChargingPowerSourceType2Point4A,
};
typedef UInt8 USBChargingPowerSourceType;

@interface iAP1Stenograph : StenographModule {
    iAP1PacketEvent *_currentPacket;
    iAP1PacketEvent *_currentAccessorySyncBytePacket;
    ATFRetHardwareInfoEvent *_currentHardwareEvent;
    AccessoryConnectEvent *_currentAccessoryConnectEvent;
    AppleDeviceConnectEvent *_currentAppleDeviceConnectEvent;
    USBResetEvent *_currentUSBResetEvent;
    BOOL _identificationSessionHasBeenReset;
    BOOL _iAPStackHasBeenReset;

    BOOL _syncByteSeen;
    BOOL _syncByteSeenJustChangedToYes;
    BOOL _inIDPS;
    BOOL _identifyTokenSeen;
    BOOL _idlSeen;
    BOOL _identifySeen;
    BOOL _startIDPSSeen;
    BOOL _startIDPSSeenJustChangedToSeen;
    BOOL _startIDPSSeenWithoutEndIDPSOrIdentifyOrIDL;
    BOOL _startIDPSSeenWithoutEndIDPSOrIdentifyOrIDLJustChangedToNotSeen;
    UInt16 _startIDPSTransactionID;
    UInt16 _endIDPSTransactionID;
    BOOL _setFIDTokenValuesSeenInIDPS;
    BOOL _retFIDTokenValuesACKsSeenInIDPS;
    BOOL _retFIDTokenValuesACKsPending;
    BOOL _startIDPSACKSeenAfterStartIDPS;
    BOOL _accessoryCapsTokenACKedOK;
    BOOL _accessoryCapsTokenSeen;
    BOOL _accessoryCapsTokenSeenInLastFIDToken;
    BOOL _accCapsIncludeiPhoneSDK;
    BOOL _bundleSeedIDTokenSeen;
    BOOL _identifyTokenACKedOK;
    BOOL _accInfoModelNumberTokenSeen;
    BOOL _accInfoAccNameTokenSeen;
    BOOL _accInfoAccNameTokenACKedOK;
    BOOL _accInfoManufacturerTokenSeen;
    UInt32 _idpsLingoMask;
    BOOL _otaDeclarationSeen;
    UInt32 _otaDeclarationMask;
    BOOL _videoSignalFormatPreferenceSeenInLastFIDToken;
    BOOL _accInfoModelNumberTokenACKedOK;
    BOOL _eaProtocolTokenSeen;
    BOOL _microphoneCapsSeen;
    BOOL _displayPortPreferenceOn;
    BOOL _videoOutTurnedOnInIDPS;
    BOOL _videoSignalFormatSetInIDPS;
    BOOL _videoOutPreferenceSetInIDPS;
    BOOL _bundleSeedIDTokenACKedOK;
    BOOL _microphoneLingoIdentified;
    BOOL _eaProtocolTokenACKedOK;
    BOOL _idpsCompletedSuccessfully;
    BOOL _authRequestedInFIDToken;
    UInt32 _identifiedLingoesMask;
    BOOL _idlACKSeen;
    BOOL _idlACKSuccessful;
    UInt8 _authMajorVersion;
    BOOL _openiPodFeatureFileSent;
    BOOL _openiPodFeatureFileSentButNotACKed;
    BOOL _shouldAuthenticate;
    BOOL _shouldAuthenticateJustChangedToYes;
    UInt32 _idlTotalCount;
    BOOL _waitingForIDLACK;
    BOOL _waitingForIDPSStatus;
    BOOL _waitingForIDLACKJustChanged;
    BOOL _waitingForIDPSStatusJustChanged;
    BOOL _accCapsIncludeAsyncProcessLargeData;
    BOOL _startIDPSNACKedWithBadParameter;
    BOOL _microphoneCapsTokenACKedOK;
    BOOL _accInfoFirmwareVersionTokenSeen;
    BOOL _enterExtendedInterfaceModeRequestJustSeen;
    BOOL _exitExtendedInterfaceModeRequestJustSeen;
    BOOL _idpsStatusSeen;

    BOOL _accessoryDetect;
    BOOL _accessoryDetectStateKnown;
    BOOL _iPodDetect;
    BOOL _iPodDetectStateKnown;

    BOOL _videoOutSettingSet;
    BOOL _videoSignalFormatSet;
    BOOL _videoSignalFormatChecked;
    UInt8 _videoSignalFormatSetting;
    BOOL _videoConnectionTypeSet;
    BOOL _videoConnectionTypeSetInIDPS;
    BOOL _videoConnectionTypeChecked;
    BOOL _videoAspectRatioSet;
    UInt8 _videoAspectRatioSetting;
    BOOL _videoAspectRatioChecked;
    BOOL _anyiPodPreferenceTokenVideoOutPreferencesSeen;
    BOOL _anyiPodPreferenceTokenVideoOutPreferencesInFIDToken;

    BOOL _listOfIdentifiedLingoesJustSent;
    UInt32 _lingoesJustIdentified;

    BOOL _authCompletedSuccessfully;
    BOOL _provisionalAuthenticationPeriod; // authGracePeriod

    BOOL _playControlFFWing;
    iAP1PacketEvent *_startFFPacket;
    BOOL _playControlREWing;
    iAP1PacketEvent *_startREWPacket;

    BOOL _sawRUISetPlayStatusNotification;
    BOOL _ruiSetPlayStatusNotificationShortFormat; // If the 1 byte format was used
    UInt32 _latestRUIPlayStatusNotificationMask;
    UInt32 _cumulativeRUIPlayStatusNotificationMask;

    BOOL _sawDRSetRemoteEventNotification;
    UInt32 _drRemoteEventNotificationMask;

    iAP1PacketEvent *_lastContextButtonStatusPacket;
    iAP1PacketEvent *_lastVideoButtonStatusPacket;
    iAP1PacketEvent *_secondToLastContextButtonStatusPacket;
    iAP1PacketEvent *_secondToLastVideoButtonStatusPacket;
    iAP1PacketEvent *_lastRotationInputStatusPacket;
    iAP1PacketEvent *_secondToLastRotationInputStatusPacket;
    iAP1PacketEvent *_lastCameraButtonStatusPacket;
    iAP1PacketEvent *_secondToLastCameraButtonStatusPacket;
    iAP1PacketEvent *_lastPacketToChangeSupportedEventNotificationMask;
    iAP1PacketEvent *_lastRequestApplicationLaunchCommandAckPacket;
    iAP1PacketEvent *_lastGetiPodOptionsForLingoPacket;
    iAP1PacketEvent *_lastAccCapsPacket;
    iAP1PacketEvent *_lastTooManyIdentifysOrIDLPacket;
    iAP1PacketEvent *_lastIDPSStatusPacket;
    iAP1PacketEvent *_lastIDLACKPacket;
    iAP1PacketEvent *_outstandingGetiPodOptionsForLingoGeneralPacket;
    UInt32 _tooManyIdentifysOrIDLCount;

    iAP1PacketEvent *_lastSetUIModePacket;
    iAP1PacketEvent *_lastEnterExtendedInterfaceModePacket;
    iAP1PacketEvent *_lastExitExtendedInterfaceModePacket;

    iAP1PacketEvent *_locationSysControlPacket;
    BOOL _locationSysControlReceived;
    BOOL _locationSysControlAckReceived;

    UInt64 _supportedEventNotificationMask;
    BOOL _accInfoAccStatusTokenSeen;
    BOOL _accInfoAccHardwareVersionTokenSeen;
    BOOL _accInfoManufacturerTokenACKedOK;
    BOOL _accInfoHardwareVersionTokenACKedOK;
    BOOL _iPodOptionForVoiceOverSeen;
    BOOL _iPodOptionForAVSyncSeen;
    BOOL _iPodOptionForAnalogUSBAudioRoutingSeen;
    BOOL _iPodOptionForAssistiveTouchSeen;
    BOOL _getiPodOptionsForLingoGeneralSeen;
    BOOL _getiPodOptionsForLingoExtendedInterfaceSeen;
    BOOL _getiPodOptionsForLingoDigitalAudioSeen;
    BOOL _accCapsIncludeAnalogUSBAudioRouting;
    BOOL _digitalAudioLingoInIdentifyToken;
    BOOL _retiPodOptionsForLingoGeneralSeen;
    BOOL _iPodOptionForCommunicationWithiOSAppsBitEnabled;
    BOOL _iPodOptionForCommunicationWithiOSAppsSeen;
    BOOL _iPodOptionForRequestApplicationLaunchSeen;
    BOOL _accCapsIncludeVideoOut;
    BOOL _accCapsIncludeLineOut;
    BOOL _iPodPreferenceTokenLineOutPreferencesSeen;
    BOOL _getiPodOptionsForLingoWasNacked;
    BOOL _getiPodOptionsForLingoGeneralWasNacked;
    BOOL _lastRequestApplicationLaunchCommandWasNacked;
    BOOL _retAccessoryStatusNotificationSeen;
    BOOL _iPodOutUnusable;
    BOOL _iniPodOutMode;
    BOOL _inExtendedInterfaceMode;
    BOOL _inExtendedInterfaceModeJustChangedToYes;
    BOOL _inExtendedInterfaceModeJustChangedToNo;
    BOOL _justSawFirstExtendedInterfaceCommand;
    BOOL _seenExtendedInterfaceCommands;
    BOOL _accCapsIncludeVolumeCheck;
    BOOL _accCapsIncludeVoiceOver;
    BOOL _accCapsIncludeAssistiveTouch;
    BOOL _outstandingiPodOptionsRequestedForiPodOut;
    BOOL _accCapsIncludeUSBAudio;
    BOOL _iPodConnectedSeen;
    ATFRetHardwareInfoEvent *_lastAccDetectEvent;
    ATFRetHardwareInfoEvent *_lastAccPowerEvent;
    ATFRetHardwareInfoEvent *_lastiPodDetectGroundedEvent;
    ATFRetHardwareInfoEvent *_lastFirewireVoltageEvent;
    ATFRetHardwareInfoEvent *_secondToLastFirewireVoltageEvent;
    ATFRetHardwareInfoEvent *_lastAccTxVoltageEvent;
    ATFRetHardwareInfoEvent *_secondToLastAccTxVoltageEvent;
    ATFRetHardwareInfoEvent *_lastIDResistorEvent;
    ATFRetHardwareInfoEvent *_secondToLastIDResistorEvent;
    ATFRetHardwareInfoEvent *_lastUSBVBusVoltageEvent;
    ATFRetHardwareInfoEvent *_secondToLastUSBVBusVoltageEvent;
    ATFRetHardwareInfoEvent *_lastUSBDPlusVoltageEvent;
    ATFRetHardwareInfoEvent *_lastUSBDMinusVoltageEvent;
    ATFRetHardwareInfoEvent *_lastReservedPin14StateEvent;
    ATFRetHardwareInfoEvent *_lastReservedPin17StateEvent;
    iAP1PacketEvent *_lastValidGetCurrentPlayingTrackChapterInfoPacket;    // this can be set to nil by the end of track play or the end of extended interface mode
    iAP1PacketEvent *_lastValidReturnCurrentPlayingTrackChapterInfoPacket; // this can be set to nil by the end of track play or the end of extended interface mode
    BOOL _allowedToSendSetEventNotification;
    UInt32 _setAccessoryStatusNotificationsMask;
    iAP1PacketEvent *_lastRetAccessoryStatusNotificationPacket;
    BOOL _accPowerHighSeen; //Do not include in resetAccessoryState
    BOOL _accConnectedSeen;
    BOOL _vBusVoltageSeenTooLow;
    BOOL _vBusVoltageSeenTooLowJustChanged;
    BOOL _vBusVoltageSeenTooHigh;
    BOOL _vBusVoltageSeenTooHighJustChanged;
    BOOL _vBusVoltageJustChangedToAcceptable;
    double _lastDPlusVBusRatio;
    double _lastDMinusVBusRatio;
    iAP1PacketEvent *_lastSetAvailableCurrentPacket;
    BOOL _returnTransportMaxPayloadSizeSeen;
    UInt16 _maxPacketPayloadSize;
    BOOL _sportsiPodCapsReceived;
    iAP1PacketEvent *_sportsiPodCapsPacket;
    BOOL _locationSysCapsReceived;
    BOOL _locationLocAsstCapsReceived;
    BOOL _locationGPSCapsReceived;
    BOOL _nmeaSentenceTypeSentTooFrequently;
    iAP1PacketEvent *_locationSysCapsPacket;
    iAP1PacketEvent *_locationGPSCapsPacket;
    iAP1PacketEvent *_locationLocAsstCapsPacket;
    NSMutableDictionary *_nmeaSentenceTypes;                          // Dictionary of NMEA types to transaction context
    MultipleSectionTransactionCollection *_nmeaSentenceMSTCollection; // collection of multiple section transactions
                                                                      // for Location Lingo NMEA sentence packets

    NSMutableDictionary *_iPodPrefSettingTokens; // taken from iPodPreferenceTokens

    //SDK
    NSMutableArray *_odsfpMap; //maps the OpenDataSessionForProtocol session ID to a
                               //transaction ID so we can know whether a particular ODSFP
                               //has been DevACK'ed

    NSMutableArray *_iPodDataTransferMap; //maps the iPodDataTransfer session ID to a
                                          //transaction ID so we can know whether a particular iPodDataTransfer
                                          //has been DevACK'ed

    UInt16 _setAccessoryDataTransactionID;
    NSMutableArray *_setAccessoryDataAckedSectionIndices;

    MultipleSectionTransactionCollection *_voiceOverEventMSTCollection; // collection of multiple section transactions
                                                                        // for VoiceOver event packets

    NSMutableArray *_openFileHandles; // store file handles returned by RetiPodFileHandle

    CertificateCollection *_certificateCollection; // processes and stores certificate information

    BOOL _lineOutUsageSet;
    BOOL _lineOutUsageSetInIDPS;
    BOOL _displayRemoteLingoIdentifiedInFIDToken;
    BOOL _indicatedVolumeInfo;
    BOOL _indicatedVolumeInfoJustChangedToNo;
    BOOL _indicatedDisplayRemoteLingo;
    BOOL _indicatedDisplayRemoteLingoJustChangedToNo;

    FIDTokenCollection *_fidTokenCollection;
    AccessoryDataTransferContext *_accessoryDataTransferContext;
    USBHIDReportCollection *_usbHIDReportCollection;

    // Extended Interface
    BOOL _extendedInterfaceArtworkFormatsRequested;

    int _categoryCounts[CT_LastValidCategory + 1];     // -1 means that the count was not obtained, positive number is record count returned by GetNumberCategorizedDBRecords.
    int _categoryGetNumberCategorizedDBRecordsPending; // -1 means that L04_GetNumberCategorizedDBRecords is NOT pending.

    TransactionIDContext *_accTransactionIDContext;  // Holds bitmap of accessory transaction IDs used in non-response commands
    TransactionIDContext *_ipodTransactionIDContext; // Holds bitmap of iPod transaction IDs used in request commands
    BOOL _accTransactionIDDuplicatedJustSeen;
    BOOL _accTransactionIDUnrecognizedJustSeen;

    BOOL _pin20Grounded;
    BOOL _pin20JustGrounded;
    BOOL _identifyTokenWasFirstInIDPS;
    NSUInteger _numberOfSetFIDTokenValuesSeenSinceStartIDPS;
    BOOL _accInfoFirmwareVersionTokenACKedOK;

    BOOL _lineOutPreferencesSeenInFIDToken;
    BOOL _locationLingoIdentifiedInFIDToken;
    BOOL _iPodOutLingoIdentifiedInFIDToken;

    BOOL _sportsLingoGetiPodCapsSent;
    BOOL _extendedInterfaceLingoProtocolVersionChecked;
    BOOL _iPodSetToUSBHostModeWithiAP;

    LingoProtocolVersionCollection *_protocolVersionCollection;
    NSString *_iPodSoftwareVersion;

    BOOL _hdCapableBitSetInRetTunerCaps;
    UInt32 _rfTunerCaps;

    BOOL _lineOutPreferencesCheckedWithGetiPodPrefs;
    BOOL _lineOutPreferencesPossiblyUnsupported;
    BOOL _lineOutPreferencesUnsupported;

    BOOL _locationLingoFailedAttemptedIdentify;

    NSString *_nowPlayingAppFocusChangeAppID;

    BOOL _iPodAckNonSuccessForSetPlayStatusChangeNotificationFourByteFormSeen;
    BOOL _getSupportedEventNotificationSeen;
    BOOL _setEventNotificationForDatabaseChangedSeen;
}

// readonly properties
@property (nonatomic, readonly, retain) iAP1PacketEvent *currentPacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *currentAccessorySyncBytePacket;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *currentHardwareEvent;
@property (nonatomic, readonly, retain) EightPinHardwareEvent *currentEightPinHardwareEvent;
@property (nonatomic, readonly, retain) AccessoryConnectEvent *currentAccessoryConnectEvent;
@property (nonatomic, readonly, retain) AppleDeviceConnectEvent *currentAppleDeviceConnectEvent;
@property (nonatomic, readonly, retain) USBResetEvent *currentUSBResetEvent;
@property (nonatomic, readonly, assign) BOOL identificationSessionHasBeenReset;
@property (nonatomic, readonly, assign) BOOL iAPStackHasBeenReset;
@property (nonatomic, readonly, assign) BOOL syncByteSeen;
@property (nonatomic, readonly, assign) BOOL syncByteSeenJustChangedToYes;
@property (nonatomic, readonly, assign) BOOL inIDPS;
@property (nonatomic, readonly, assign) BOOL identifyTokenSeen;
@property (nonatomic, readonly, assign) BOOL idlSeen;
@property (nonatomic, readonly, assign) BOOL identifySeen;
@property (nonatomic, readonly, assign) BOOL startIDPSSeen;
@property (nonatomic, readonly, assign) BOOL startIDPSSeenJustChangedToSeen;
@property (nonatomic, readonly, assign) BOOL startIDPSSeenWithoutEndIDPSOrIdentifyOrIDL;
@property (nonatomic, readonly, assign) BOOL startIDPSSeenWithoutEndIDPSOrIdentifyOrIDLJustChangedToNotSeen;
@property (nonatomic, readonly, assign) BOOL setFIDTokenValuesSeenInIDPS;
@property (nonatomic, readonly, assign) BOOL retFIDTokenValuesACKsSeenInIDPS;
@property (nonatomic, readonly, assign) BOOL retFIDTokenValuesACKsPending;
@property (nonatomic, readonly, assign) BOOL startIDPSACKSeenAfterStartIDPS;
@property (nonatomic, readonly, assign) BOOL accessoryCapsTokenSeen;
@property (nonatomic, readonly, assign) BOOL accessoryCapsTokenSeenInLastFIDToken;
@property (nonatomic, readonly, assign) BOOL accCapsIncludeiPhoneSDK;
@property (nonatomic, readonly, assign) BOOL eaProtocolTokenSeen;
@property (nonatomic, readonly, assign) BOOL microphoneCapsSeen;
@property (nonatomic, readonly, assign) BOOL videoOutTurnedOnInIDPS;
@property (nonatomic, readonly, assign) BOOL videoSignalFormatSetInIDPS;
@property (nonatomic, readonly, assign) BOOL videoOutPreferenceSetInIDPS;
@property (nonatomic, readonly, assign) BOOL displayPortPreferenceOn;
@property (nonatomic, readonly, assign) BOOL bundleSeedIDTokenSeen;
@property (nonatomic, readonly, assign) BOOL identifyTokenACKedOK;
@property (nonatomic, readonly, assign) BOOL accInfoModelNumberTokenSeen;
@property (nonatomic, readonly, assign) BOOL videoSignalFormatPreferenceSeenInLastFIDToken;
@property (nonatomic, readonly, assign) BOOL accInfoModelNumberTokenACKedOK;
@property (nonatomic, readonly, assign) BOOL videoOutSettingSet;
@property (nonatomic, readonly, assign) BOOL videoSignalFormatSet;
@property (nonatomic, readonly, assign) BOOL videoSignalFormatChecked;
@property (nonatomic, readonly, assign) UInt8 videoSignalFormatSetting;
@property (nonatomic, readonly, assign) BOOL videoConnectionTypeSet;
@property (nonatomic, readonly, assign) BOOL videoConnectionTypeSetInIDPS;
@property (nonatomic, readonly, assign) BOOL videoConnectionTypeChecked;
@property (nonatomic, readonly, assign) BOOL videoAspectRatioSet;
@property (nonatomic, readonly, assign) UInt8 videoAspectRatioSetting;
@property (nonatomic, readonly, assign) BOOL videoAspectRatioChecked;
@property (nonatomic, readonly, assign) UInt32 identifiedLingoesMask;
@property (nonatomic, readonly, assign) BOOL idlACKSeen;
@property (nonatomic, readonly, assign) BOOL idlACKSuccessful;
@property (nonatomic, readonly, assign) UInt8 authMajorVersion;
@property (nonatomic, readonly, retain) iAP1PacketEvent *secondToLastContextButtonStatusPacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *secondToLastVideoButtonStatusPacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *secondToLastRotationInputStatusPacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastPacketToChangeSupportedEventNotificationMask;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastRequestApplicationLaunchCommandAckPacket;
@property (nonatomic, readonly, assign) UInt64 supportedEventNotificationMask;
@property (nonatomic, readonly, assign) BOOL accInfoAccStatusTokenSeen;
@property (nonatomic, readonly, assign) BOOL accInfoAccHardwareVersionTokenSeen;
@property (nonatomic, readonly, assign) BOOL accInfoAccNameTokenSeen;
@property (nonatomic, readonly, assign) BOOL accInfoAccNameTokenACKedOK;
@property (nonatomic, readonly, assign) BOOL accInfoManufacturerTokenSeen;
@property (nonatomic, readonly, assign) BOOL accInfoManufacturerTokenACKedOK;
@property (nonatomic, readonly, assign) BOOL accInfoHardwareVersionTokenACKedOK;
@property (nonatomic, readonly, assign) BOOL accInfoFirmwareVersionTokenACKedOK;
@property (nonatomic, readonly, assign) BOOL retiPodOptionsForLingoGeneralSeen;
@property (nonatomic, readonly, assign) BOOL iPodOptionForCommunicationWithiOSAppsBitEnabled;
@property (nonatomic, readonly, assign) BOOL iPodOptionForCommunicationWithiOSAppsSeen;
@property (nonatomic, readonly, assign) BOOL iPodOptionForVoiceOverSeen;
@property (nonatomic, readonly, assign) BOOL iPodOptionForAVSyncSeen;
@property (nonatomic, readonly, assign) BOOL iPodOptionForAnalogUSBAudioRoutingSeen;
@property (nonatomic, readonly, assign) BOOL iPodOptionForAssistiveTouchSeen;
@property (nonatomic, readonly, assign) BOOL getiPodOptionsForLingoGeneralSeen;
@property (nonatomic, readonly, assign) BOOL getiPodOptionsForLingoExtendedInterfaceSeen;
@property (nonatomic, readonly, assign) BOOL getiPodOptionsForLingoDigitalAudioSeen;
@property (nonatomic, readonly, assign) BOOL accCapsIncludeAnalogUSBAudioRouting;
@property (nonatomic, readonly, assign) BOOL digitalAudioLingoInIdentifyToken;
@property (nonatomic, readonly, assign) BOOL iPodOptionForRequestApplicationLaunchSeen;
@property (nonatomic, readonly, assign) BOOL accCapsIncludeVideoOut;
@property (nonatomic, readonly, assign) BOOL accCapsIncludeLineOut;
@property (nonatomic, readonly, assign) BOOL iPodPreferenceTokenLineOutPreferencesSeen;
@property (nonatomic, readonly, assign) BOOL getiPodOptionsForLingoWasNacked;
@property (nonatomic, readonly, assign) BOOL getiPodOptionsForLingoGeneralWasNacked;
@property (nonatomic, readonly, retain) iAP1PacketEvent *outstandingGetiPodOptionsForLingoGeneralPacket;
@property (nonatomic, readonly, assign) BOOL lastRequestApplicationLaunchCommandWasNacked;
@property (nonatomic, readonly, assign) UInt8 lastRequestApplicationLaunchCommandAckStatus;
@property (nonatomic, readonly, assign) BOOL iPodConnectedSeen;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *lastAccDetectEvent;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *lastAccPowerEvent;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *lastiPodDetectGroundedEvent;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *lastFirewireVoltageEvent;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *secondToLastFirewireVoltageEvent;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *lastAccTxVoltageEvent;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *secondToLastAccTxVoltageEvent;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *lastIDResistorEvent;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *secondToLastIDResistorEvent;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *lastUSBVBusVoltageEvent;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *secondToLastUSBVBusVoltageEvent;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *lastUSBDPlusVoltageEvent;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *lastUSBDMinusVoltageEvent;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *lastReservedPin14StateEvent;
@property (nonatomic, readonly, retain) ATFRetHardwareInfoEvent *lastReservedPin17StateEvent;
@property (nonatomic, readonly, assign) BOOL allowedToSendSetEventNotification;
@property (nonatomic, readonly, assign) UInt32 setAccessoryStatusNotificationsMask;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastRetAccessoryStatusNotificationPacket;
@property (nonatomic, readonly, assign) BOOL accessoryCapsTokenACKedOK;
@property (nonatomic, readonly, assign) UInt32 idpsLingoMask;
@property (nonatomic, readonly, assign) BOOL otaDeclarationSeen;
@property (nonatomic, readonly, assign) UInt32 otaDeclarationMask;
@property (nonatomic, readonly, assign) BOOL bundleSeedIDTokenACKedOK;
@property (nonatomic, readonly, assign) BOOL retAccessoryStatusNotificationSeen;
@property (nonatomic, readonly, assign) BOOL accCapsIncludeVoiceOver;
@property (nonatomic, readonly, assign) BOOL accCapsIncludeAssistiveTouch;
@property (nonatomic, readonly, assign) BOOL iPodOutUnusable;
@property (nonatomic, readonly, assign) BOOL iniPodOutMode;
@property (nonatomic, readonly, assign) BOOL iniPodOutActionSafeMode;
@property (nonatomic, readonly, assign) BOOL inExtendedInterfaceMode;
@property (nonatomic, readonly, assign) BOOL inExtendedInterfaceModeJustChangedToYes;
@property (nonatomic, readonly, assign) BOOL inExtendedInterfaceModeJustChangedToNo;
@property (nonatomic, readonly, assign) BOOL justSawFirstExtendedInterfaceCommand;
@property (nonatomic, readonly, assign) BOOL seenExtendedInterfaceCommands;
@property (nonatomic, readonly, assign) BOOL accCapsIncludeVolumeCheck;
@property (nonatomic, readonly, assign) BOOL outstandingiPodOptionsRequestedForiPodOut;
@property (nonatomic, readonly, assign) BOOL accCapsIncludeUSBAudio;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastGetiPodOptionsForLingoPacket;
@property (nonatomic, readonly, assign) BOOL microphoneLingoIdentified;
@property (nonatomic, readonly, assign) BOOL eaProtocolTokenACKedOK;
@property (nonatomic, readonly, assign) BOOL idpsCompletedSuccessfully;
@property (nonatomic, readonly, assign) BOOL authRequestedInFIDToken;
@property (nonatomic, readonly, assign) BOOL returnTransportMaxPayloadSizeSeen;
@property (nonatomic, readonly, assign) UInt16 maxPacketPayloadSize;
@property (nonatomic, readonly, assign) BOOL sportsiPodCapsReceived;
@property (nonatomic, readonly, retain) iAP1PacketEvent *sportsiPodCapsPacket;
@property (nonatomic, readonly, assign) BOOL locationSysCapsReceived;
@property (nonatomic, readonly, assign) BOOL locationLocAsstCapsReceived;
@property (nonatomic, readonly, assign) BOOL locationGPSCapsReceived;
@property (nonatomic, readonly, assign) BOOL nmeaSentenceTypeSentTooFrequently;
@property (nonatomic, readonly, assign) BOOL anyiPodPreferenceTokenVideoOutPreferencesSeen;
@property (nonatomic, readonly, assign) BOOL anyiPodPreferenceTokenVideoOutPreferencesInFIDToken;
@property (nonatomic, readonly, assign) BOOL playControlFFWing;
@property (nonatomic, readonly, assign) BOOL playControlREWing;
@property (nonatomic, readonly, retain) iAP1PacketEvent *locationSysCapsPacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *locationGPSCapsPacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *locationLocAsstCapsPacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastAccCapsPacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *startFFPacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *startREWPacket;
@property (nonatomic, readonly, assign) BOOL sawRUISetPlayStatusNotification;
@property (nonatomic, readonly, assign) BOOL sawPlayStatusChangeNotificationNumberOfTracks;
@property (nonatomic, readonly, assign) BOOL ruiSetPlayStatusNotificationShortFormat;
@property (nonatomic, readonly, assign) UInt32 latestRUIPlayStatusNotificationMask;
@property (nonatomic, readonly, assign) UInt32 cumulativeRUIPlayStatusNotificationMask;
@property (nonatomic, readonly, assign) BOOL sawDRSetRemoteEventNotification;
@property (nonatomic, readonly, assign) UInt32 drRemoteEventNotificationMask;
@property (nonatomic, readonly, assign) BOOL lineOutUsageSet;
@property (nonatomic, readonly, assign) BOOL lineOutUsageSetInIDPS;
@property (nonatomic, readonly, assign) BOOL displayRemoteLingoIdentifiedInFIDToken;
@property (nonatomic, readonly, assign) BOOL indicatedVolumeInfo;
@property (nonatomic, readonly, assign) BOOL indicatedVolumeInfoJustChangedToNo;
@property (nonatomic, readonly, assign) BOOL indicatedDisplayRemoteLingo;
@property (nonatomic, readonly, assign) BOOL indicatedDisplayRemoteLingoJustChangedToNo;
@property (nonatomic, readonly, assign) BOOL listOfIdentifiedLingoesJustSent;
@property (nonatomic, readonly, assign) UInt32 lingoesJustIdentified;
@property (nonatomic, readonly, assign) BOOL authCompletedSuccessfully;
@property (nonatomic, readonly, assign) BOOL provisionalAuthenticationPeriod;
@property (nonatomic, readonly, assign) BOOL locationSysControlReceived;
@property (nonatomic, readonly, assign) BOOL locationSysControlAckReceived;
@property (nonatomic, readonly, retain) iAP1PacketEvent *locationSysControlPacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastTooManyIdentifysOrIDLPacket;
@property (nonatomic, readonly, assign) UInt32 tooManyIdentifysOrIDLCount;
@property (nonatomic, readonly, assign) BOOL extendedInterfaceArtworkFormatsRequested;
@property (nonatomic, readonly, assign) UInt16 setAccessoryDataTransactionID;
@property (nonatomic, readonly, assign) BOOL openiPodFeatureFileSent;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastSetUIModePacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastEnterExtendedInterfaceModePacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastExitExtendedInterfaceModePacket;
@property (nonatomic, readonly, assign) BOOL openiPodFeatureFileSentButNotACKed;
@property (nonatomic, readonly, assign) BOOL shouldAuthenticate;
@property (nonatomic, readonly, assign) BOOL shouldAuthenticateJustChangedToYes;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastValidGetCurrentPlayingTrackChapterInfoPacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastValidReturnCurrentPlayingTrackChapterInfoPacket;
@property (nonatomic, readonly, assign) BOOL accTransactionIDDuplicatedJustSeen;
@property (nonatomic, readonly, assign) BOOL accTransactionIDUnrecognizedJustSeen;
@property (nonatomic, readonly, assign) UInt32 idlTotalCount;
@property (nonatomic, readonly, assign) BOOL waitingForIDLACK;
@property (nonatomic, readonly, assign) BOOL waitingForIDPSStatus;
@property (nonatomic, readonly, assign) BOOL waitingForIDLACKJustChanged;
@property (nonatomic, readonly, assign) BOOL waitingForIDPSStatusJustChanged;
@property (nonatomic, readonly, assign) BOOL accPowerHighSeen;
@property (nonatomic, readonly, assign) BOOL accConnectedSeen;
@property (nonatomic, readonly, assign) BOOL vBusVoltageSeenTooLow;
@property (nonatomic, readonly, assign) BOOL vBusVoltageSeenTooLowJustChanged;
@property (nonatomic, readonly, assign) BOOL vBusVoltageSeenTooHigh;
@property (nonatomic, readonly, assign) BOOL vBusVoltageSeenTooHighJustChanged;
@property (nonatomic, readonly, assign) BOOL vBusVoltageJustChangedToAcceptable;
@property (nonatomic, readonly, assign) double lastDPlusVBusRatio;
@property (nonatomic, readonly, assign) double lastDMinusVBusRatio;
@property (nonatomic, readonly, assign) BOOL pin20Grounded;
@property (nonatomic, readonly, assign) BOOL pin20JustGrounded;
@property (nonatomic, readonly, assign) BOOL identifyTokenWasFirstInIDPS;
@property (nonatomic, readonly, assign) NSUInteger numberOfSetFIDTokenValuesSeenSinceStartIDPS;
@property (nonatomic, readonly, assign) BOOL sportsLingoGetiPodCapsSent;
@property (nonatomic, readonly, assign) BOOL lineOutPreferencesSeenInFIDToken;
@property (nonatomic, readonly, assign) BOOL locationLingoIdentifiedInFIDToken;
@property (nonatomic, readonly, assign) BOOL extendedInterfaceLingoProtocolVersionChecked;
@property (nonatomic, readonly, retain) LingoProtocolVersionCollection *protocolVersionCollection;
@property (nonatomic, readonly, retain) NSString *iPodSoftwareVersion;
@property (nonatomic, readonly, assign) BOOL iPodOutLingoIdentifiedInFIDToken;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastCameraButtonStatusPacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *secondToLastCameraButtonStatusPacket;
@property (nonatomic, readonly, assign) BOOL accCapsIncludeAsyncProcessLargeData;
@property (nonatomic, readonly, assign) BOOL startIDPSNACKedWithBadParameter;
@property (nonatomic, readonly, assign) BOOL iPodSetToUSBHostModeWithiAP;
@property (nonatomic, readonly, assign) BOOL microphoneCapsTokenACKedOK;
@property (nonatomic, readonly, assign) BOOL accInfoFirmwareVersionTokenSeen;
@property (nonatomic, readonly, assign) BOOL hdCapableBitSetInRetTunerCaps;
@property (nonatomic, readonly, assign) UInt32 rfTunerCaps;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastSetAvailableCurrentPacket;
@property (nonatomic, readonly, assign) BOOL enterExtendedInterfaceModeRequestJustSeen;
@property (nonatomic, readonly, assign) BOOL exitExtendedInterfaceModeRequestJustSeen;
@property (nonatomic, readonly, assign) BOOL idpsStatusSeen;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastIDPSStatusPacket;
@property (nonatomic, readonly, retain) iAP1PacketEvent *lastIDLACKPacket;
@property (nonatomic, readonly, assign) BOOL lineOutPreferencesCheckedWithGetiPodPrefs;
@property (nonatomic, readonly, assign) BOOL lineOutPreferencesPossiblyUnsupported;
@property (nonatomic, readonly, assign) BOOL lineOutPreferencesUnsupported;
@property (nonatomic, readonly, assign) BOOL locationLingoFailedAttemptedIdentify;
@property (nonatomic, readonly, retain) NSString *nowPlayingAppFocusChangeAppID;
@property (nonatomic, readonly, assign) BOOL iPodAckNonSuccessForSetPlayStatusChangeNotificationFourByteFormSeen;
@property (nonatomic, readonly, assign) BOOL getSupportedEventNotificationSeen;
@property (nonatomic, readonly, assign) BOOL setEventNotificationForDatabaseChangedSeen;

// properties for CA tests
@property (nonatomic, readonly, assign) BOOL assistiveTouchEnabled;

/*!
 @property  lastUSBBrickDetectEvent
 @abstract  The last USB brick detect event seen
 */
@property (nonatomic, readonly, retain) EightPinUSBBrickDetectEvent *lastUSBBrickDetectEvent;

/*!
 @property  eightPinDevicePowerStenograph
 @abstract  8-pin Device Power Stenograph
 */
@property (nonatomic, readonly, assign) ATSEightPinStenograph *eightPinStenograph;

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
 @property  accessorySportsLingoCommandSeen
 @abstract  If an accessory-originated Sports lingo command has been seen since identification
 */
@property (nonatomic, readonly, assign) BOOL accessorySportsLingoCommandSeen;

/*!
 @property  commWithAppsDataTransferCommandSeen
 @abstract  If an AccessoryDataTransfer or DeviceDataTransfer command has been seen since identification
 */
@property (nonatomic, readonly, assign) BOOL commWithAppsDataTransferCommandSeen;

/*!
 @property  iPodOutButtonStatusCommandSeen
 @abstract  If a Simple Remote iPodOutButtonStatus command has been seen since identification
 */
@property (nonatomic, readonly, assign) BOOL iPodOutButtonStatusCommandSeen;

/*!
 @property  accessoryLocationLingoCommandSeen
 @abstract  If an accessory Location lingo command has been seen since identification
 */
@property (nonatomic, readonly, assign) BOOL accessoryLocationLingoCommandSeen;

/*!
 @property  setiPodPreferencesVideoOutFeatureSeen
 @abstract  If SetiPodPreferences with a video out related class has been seen since identification
 */
@property (nonatomic, readonly, assign) BOOL setiPodPreferencesVideoOutFeatureSeen;

/*!
 @property  accessorySimpleRemoteLingoCommandSeen
 @abstract  If an accessory Simple Remote lingo command has been seen since identification
 */
@property (nonatomic, readonly, assign) BOOL accessorySimpleRemoteLingoCommandSeen;

/*!
 @property  accessoryExtendedInterfaceLingoCommandSeen
 @abstract  If an accessory Extended Interface lingo command has been seen since identification
 */
@property (nonatomic, readonly, assign) BOOL accessoryExtendedInterfaceLingoCommandSeen;

/*!
 @property  requestWiFiConnectionInfoCommandSeen
 @abstract  If RequestWiFiConnectionInfo has been seen since the last identification
 */
@property (nonatomic, readonly, assign) BOOL requestWiFiConnectionInfoCommandSeen;

/*!
 @property  wiFiConnectionInfoCommandSeen
 @abstract  If WiFiConnectionInfo has been seen since the last identification
 */
@property (nonatomic, readonly, assign) BOOL wiFiConnectionInfoCommandSeen;

/*!
 @property  accessoryDisplayRemoteCommandSeen
 @abstract  If an accessory command in the DisplayRemote lingo has been seen since the last identification
 */
@property (nonatomic, readonly, assign) BOOL accessoryDisplayRemoteCommandSeen;

/*!
 @property  accessoryStorageCommandSeen
 @abstract  If an accessory command in the Storage lingo has been seen since the last identification
 */
@property (nonatomic, readonly, assign) BOOL accessoryStorageCommandSeen;

/*!
 @property  voiceOverEventCommandSeen
 @abstract  If a VoiceOverEvent has been seen since the last identification
 */
@property (nonatomic, readonly, assign) BOOL voiceOverEventCommandSeen;

/*!
    If a GetNumPlayingTracks has been seen since the last identification
 */
@property (nonatomic, readonly, assign) BOOL getNumPlayingTracksSeen;

- (void)processEvent:(CaptureEvent *)event;

- (NSString *)lingoNameForLingoID:(UInt8)lingoID;
- (NSString *)commandNameForLingoID:(UInt8)lingoID commandID:(UInt16)commandID;
- (LKTCommandDefinition *)commandDefinitionForLingoID:(UInt8)lingoID commandID:(UInt16)commandID;

- (UInt32)buttonStatesForCommand:(LKTCommand *)command;

- (BOOL)previousSettingConflictsWithiPodPref:(UInt8)prefSetting forClass:(UInt8)prefClass;
- (LKTToken *)previousIDPSPrefSettingTokenForClass:(UInt8)prefClass;
- (BOOL)isVideoOutClass:(UInt8)prefClass;
- (BOOL)isVoiceOverCommandID:(UInt16)cmdID;
- (BOOL)accSupportsLocType:(UInt8)locType;
- (BOOL)accSupportsLocDataType:(UInt8)dataType forLocType:(UInt8)locType;
- (BOOL)openDataSessionForProtocolSeen;
- (BOOL)odsfpSessionSeen:(UInt16)sessionID;
- (BOOL)odsfpSessionOpen:(UInt16)sessionID;

/*!
 @method    hasOpenDataSession
 @abstract  Checks if there's any active data session
 @return    YES if a data session was found to be open, NO otherwise
 */
- (BOOL)hasOpenDataSession;

- (BOOL)odsfpSessionDevACKed:(UInt16)sessionID;
- (BOOL)odsfpSessionAccessoryACKedClosed:(UInt16)sessionID;
- (UInt8)devACKStatusForODSFPSession:(UInt16)sessionID;
- (BOOL)iPodDataTransferSeen;
- (BOOL)iPodDataTransferTransactionIDSeen:(UInt16)transactionID;
- (UInt16)iPodDataTransferSessionIDForTransactionID:(UInt16)transactionID;
- (NSArray *)failedAccessoryDataTransferPacketsForSessionID:(UInt16)sessionID;
- (BOOL)isCommunicationWithiOSApplicationCommand:(UInt16)commandID;
- (NSUInteger)countTokensSentAfterNoRetryAndResetCount;
- (UInt8)descriptorIndexStateForIndex:(UInt8)index;
- (void)addVoiceOverEventTransactionPacket:(iAP1PacketEvent *)accessiblityEventPacket;
- (BOOL)isLastVoiceOverEventSectionConsistentForTransactionID:(UInt16)transactionID;
- (BOOL)isLastVoiceOverEventSectionInOrderForTransactionID:(UInt16)transactionID;
- (void)processNMEADataPacket:(iAP1PacketEvent *)packet;
- (BOOL)isLingoIdentified:(UInt8)lingo;
- (NSUInteger)countOfSetAccessoryDataAcksForSectionIndex:(UInt16)sectionIndex;
- (int)categoryCountForCategory:(UInt8)category;
- (BOOL)isFileHandleCurrentlyOpen:(NSNumber *)fileHandle;
- (BOOL)isAccResponseCommand:(LKTCommand *)command;
- (BOOL)isiPodRequestCommand:(LKTCommand *)command;
- (BOOL)isAccMultisectionCommand:(LKTCommand *)command;
- (BOOL)isIdentifyCommand:(LKTCommand *)command;
- (USBChargingPowerSourceType)accUSBChargingPowerSourceType;
- (BOOL)isAccUSBChargingPowerSourceType:(USBChargingPowerSourceType)type;
- (BOOL)isValidAccDPlusForUSBChargingPowerSourceType:(USBChargingPowerSourceType)type;
- (BOOL)isValidAccDMinusForUSBChargingPowerSourceType:(USBChargingPowerSourceType)type;
- (double)accMinVBusVoltageThreshold;
- (int)certificateClass;
- (NSString *)certificateSerial;
- (BOOL)accJustSawAccPowerHigh;
- (BOOL)isAccDrivingUSBVbus;
- (BOOL)isAccDrivingUSBDPlus;
- (BOOL)isAccDrivingUSBDMinus;
- (BOOL)canCancelCommandWithLingoID:(UInt8)lingoID commandID:(UInt16)commandID;
- (UInt16)currentPacketPayloadSize;
- (BOOL)isNowPlayingAppAppleOwned;

@end
