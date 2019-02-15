// Automatically generated file from TKMessageGenerator
#import <Foundation/Foundation.h>
@import TrafficKit.APHTTPBPlistContainer;
@import TrafficKit.APHTTPRequest;
@import TrafficKit.APHTTPResponse;


#pragma mark - Enums -

typedef NS_ENUM(NSInteger, CPStreamID) {
    CPStreamIDInvalid = 0, /* Invalid */
    CPStreamIDMainAudio = 100, /* Main Audio */
    CPStreamIDAltAudio = 101, /* Alternate Audio */
    CPStreamIDMainHighAudio = 102, /* Main High Audio */
    CPStreamIDScreen = 110, /* Screen */
};

typedef NS_OPTIONS(NSUInteger, CPAudioFormatType) {
    CPAudioFormatTypePCM8000Hz16BitMono = ((NSUInteger)1<<2), /* PCM, 8000 Hz, 16-Bit, Mono */
    CPAudioFormatTypePCM8000Hz16BitStereo = ((NSUInteger)1<<3), /* PCM, 8000 Hz, 16-Bit, Stereo */
    CPAudioFormatTypePCM16000Hz16BitMono = ((NSUInteger)1<<4), /* PCM, 16000 Hz, 16-Bit, Mono */
    CPAudioFormatTypePCM16000Hz16BitStereo = ((NSUInteger)1<<5), /* PCM, 16000 Hz, 16-Bit, Stereo */
    CPAudioFormatTypePCM24000Hz16BitMono = ((NSUInteger)1<<6), /* PCM, 24000 Hz, 16-Bit, Mono */
    CPAudioFormatTypePCM24000Hz16BitStereo = ((NSUInteger)1<<7), /* PCM, 24000 Hz, 16-Bit, Stereo */
    CPAudioFormatTypePCM32000Hz16BitMono = ((NSUInteger)1<<8), /* PCM, 32000 Hz, 16-Bit, Mono */
    CPAudioFormatTypePCM32000Hz16BitStereo = ((NSUInteger)1<<9), /* PCM, 32000 Hz, 16-Bit, Stereo */
    CPAudioFormatTypePCM44100Hz16BitMono = ((NSUInteger)1<<10), /* PCM, 44100 Hz, 16-Bit, Mono */
    CPAudioFormatTypePCM44100Hz16BitStereo = ((NSUInteger)1<<11), /* PCM, 44100 Hz, 16-Bit, Stereo */
    CPAudioFormatTypeReservedAudioFormatBit12 = ((NSUInteger)1<<12), /* Reserved */
    CPAudioFormatTypeReservedAudioFormatBit13 = ((NSUInteger)1<<13), /* Reserved */
    CPAudioFormatTypePCM48000Hz16BitMono = ((NSUInteger)1<<14), /* PCM, 48000 Hz, 16-Bit, Mono */
    CPAudioFormatTypePCM48000Hz16BitStereo = ((NSUInteger)1<<15), /* PCM, 48000 Hz, 16-Bit, Stereo */
    CPAudioFormatTypeReservedAudioFormatBit16 = ((NSUInteger)1<<16), /* Reserved */
    CPAudioFormatTypeReservedAudioFormatBit17 = ((NSUInteger)1<<17), /* Reserved */
    CPAudioFormatTypeReservedAudioFormatBit18 = ((NSUInteger)1<<18), /* Reserved */
    CPAudioFormatTypeReservedAudioFormatBit19 = ((NSUInteger)1<<19), /* Reserved */
    CPAudioFormatTypeReservedAudioFormatBit20 = ((NSUInteger)1<<20), /* Reserved */
    CPAudioFormatTypeReservedAudioFormatBit21 = ((NSUInteger)1<<21), /* Reserved */
    CPAudioFormatTypeAACLC44100HzStereo = ((NSUInteger)1<<22), /* AAC-LC, 44100 Hz, Stereo */
    CPAudioFormatTypeAACLC48000HzStereo = ((NSUInteger)1<<23), /* AAC-LC, 48000 Hz, Stereo */
    CPAudioFormatTypeAACELD44100HzStereo = ((NSUInteger)1<<24), /* AAC-ELD, 44100 Hz, Stereo */
    CPAudioFormatTypeAACELD48000HzStereo = ((NSUInteger)1<<25), /* AAC-ELD, 48000 Hz, Stereo */
    CPAudioFormatTypeAACELD16000HzMono = ((NSUInteger)1<<26), /* AAC-ELD, 16000 Hz, Mono */
    CPAudioFormatTypeAACELD24000HzMono = ((NSUInteger)1<<27), /* AAC-ELD, 24000 Hz, Mono */
    CPAudioFormatTypeOPUS16000HzMono = ((NSUInteger)1<<28), /* OPUS, 16000 Hz, Mono */
    CPAudioFormatTypeOPUS24000HzMono = ((NSUInteger)1<<29), /* OPUS, 24000 Hz, Mono */
    CPAudioFormatTypeOPUS48000HzMono = ((NSUInteger)1<<30), /* OPUS, 48000 Hz, Mono */
    CPAudioFormatTypeAACELD44100HzMono = ((NSUInteger)1<<31), /* AAC-ELD, 44100 Hz, Mono */
    CPAudioFormatTypeAACELD48000HzMono = ((NSUInteger)1<<32), /* AAC-ELD, 48000 Hz, Mono */
};

typedef NS_OPTIONS(NSUInteger, CPDisplayFeatures) {
    CPDisplayFeaturesKnobInteraction = ((NSUInteger)1<<1), /* Knobs Interaction */
    CPDisplayFeaturesLowFidelityTouch = ((NSUInteger)1<<2), /* Low-Fidelity Touch */
    CPDisplayFeaturesHighFidelityTouch = ((NSUInteger)1<<3), /* High-Fidelity Touch */
    CPDisplayFeaturesTouchpad = ((NSUInteger)1<<4), /* Touchpad */
};

typedef NS_ENUM(NSInteger, CPPrimaryInputDevice) {
    CPPrimaryInputDeviceTouchscreen = 1, /* Touchscreen */
    CPPrimaryInputDeviceTouchpad = 2, /* Touchpad */
    CPPrimaryInputDeviceKnob = 3, /* Knob */
};

typedef NS_ENUM(NSInteger, CPResourceOwner) {
    CPResourceOwnerNone = 0, /* None */
    CPResourceOwnerController = 1, /* Controller */
    CPResourceOwnerAccessory = 2, /* Accessory */
};

typedef NS_ENUM(NSInteger, CPResourceID) {
    CPResourceIDMainScreen = 1, /* Main Screen */
    CPResourceIDMainAudio = 2, /* Main Audio */
};

typedef NS_ENUM(NSInteger, CPResourceConstraint) {
    CPResourceConstraintAnytime = 100, /* Anytime */
    CPResourceConstraintUserInitiated = 500, /* User-Initiated */
    CPResourceConstraintNever = 1000, /* Never */
};

typedef NS_ENUM(NSInteger, CPResourceOwnershipTransferType) {
    CPResourceOwnershipTransferTypeTake = 1, /* Take */
    CPResourceOwnershipTransferTypeUntake = 2, /* Untake */
    CPResourceOwnershipTransferTypeBorrow = 3, /* Borrow */
    CPResourceOwnershipTransferTypeUnborrow = 4, /* Unborrow */
};

typedef NS_ENUM(NSInteger, CPResourceTransferPriority) {
    CPResourceTransferPriorityNiceToHave = 100, /* Nice-to-Have */
    CPResourceTransferPriorityUserInitiated = 500, /* User-Initiated */
};

typedef NS_ENUM(NSInteger, CPAppState) {
    CPAppStateSpeech = 1, /* Speech Operation */
    CPAppStatePhoneCall = 2, /* Phone Call */
    CPAppStateTurnByTurn = 3, /* Turn-by-Turn Navigation */
};

typedef NS_ENUM(NSInteger, CPSpeechMode) {
    CPSpeechModeNone = -1, /* None */
    CPSpeechModeSpeaking = 1, /* Speaking */
    CPSpeechModeRecognizingSpeech = 2, /* Recognizing Speech */
};

typedef NS_ENUM(NSInteger, CPEncryptionType) {
    CPEncryptionTypeInvalid = 0x00, /* Invalid */
    CPEncryptionTypeUnencrypted = 0x01, /* Unencrypted */
    CPEncryptionTypeMFiSAP = 0x10, /* MFi SAP */
};

typedef NS_OPTIONS(NSUInteger, CPHapticFeedbackType) {
    CPHapticFeedbackTypeDefault = ((NSUInteger)1<<1), /* Default */
};

typedef NS_ENUM(NSInteger, CPHIDInputModes) {
    CPHIDInputModesDefault = 0, /* Default */
    CPHIDInputModesCharacter = 1, /* Character */
    CPHIDInputModesScrolling = 2, /* Scrolling */
    CPHIDInputModesScrollingWithCharacters = 3, /* Scrolling with Characters */
    CPHIDInputModesDialPad = 4, /* Dial Pad */
};

typedef NS_ENUM(NSInteger, CPSiriActions) {
    CPSiriActionsPrewarm = 1, /* Prewarm */
    CPSiriActionsButtonDown = 2, /* ButtonDown */
    CPSiriActionsButtonUp = 3, /* ButtonUp */
};


#pragma mark - Containers -

@interface CPVocoderInformationKeysContainer : APHTTPBPlistContainer

/*!
 @property sampleRate
 @abstract 
 */
@property (nonatomic, retain) NSNumber *sampleRate;

@end

@interface CPStreamDescriptorRequestContainer : APHTTPBPlistContainer

/*!
 @property type
 @abstract (null)
 */
@property (nonatomic, assign) CPStreamID type;
@property (nonatomic, assign) BOOL hasType;

/*!
 @property audioFormat
 @abstract 
 */
@property (nonatomic, assign) CPAudioFormatType audioFormat;
@property (nonatomic, assign) BOOL hasAudioFormat;

/*!
 @property audioLatencyMs
 @abstract 
 */
@property (nonatomic, retain) NSNumber *audioLatencyMs;

/*!
 @property audioLoopback
 @abstract 
 */
@property (nonatomic, assign) BOOL audioLoopback;
@property (nonatomic, assign) BOOL hasAudioLoopback;

/*!
 @property audioType
 @abstract 
 */
@property (nonatomic, retain) NSString *audioType;

/*!
 @property controlPort
 @abstract 
 */
@property (nonatomic, retain) NSNumber *controlPort;

/*!
 @property input
 @abstract 
 */
@property (nonatomic, assign) BOOL input;
@property (nonatomic, assign) BOOL hasInput;

/*!
 @property dataPort
 @abstract 
 */
@property (nonatomic, retain) NSNumber *dataPort;

/*!
 @property spf
 @abstract 
 */
@property (nonatomic, retain) NSNumber *spf;

/*!
 @property streamConnectionID
 @abstract 
 */
@property (nonatomic, retain) NSNumber *streamConnectionID;

/*!
 @property vocoderInfo
 @abstract 
 */
@property (nonatomic, retain) CPVocoderInformationKeysContainer *vocoderInfo;

/*!
 @property latencyMs
 @abstract 
 */
@property (nonatomic, retain) NSNumber *latencyMs;

@end

@interface CPStreamDescriptorResponseContainer : APHTTPBPlistContainer

/*!
 @property type
 @abstract (null)
 */
@property (nonatomic, assign) CPStreamID type;
@property (nonatomic, assign) BOOL hasType;

/*!
 @property controlPort
 @abstract 
 */
@property (nonatomic, retain) NSNumber *controlPort;

/*!
 @property dataPort
 @abstract 
 */
@property (nonatomic, retain) NSNumber *dataPort;

/*!
 @property sampleTime
 @abstract 
 */
@property (nonatomic, retain) NSNumber *sampleTime;

/*!
 @property streamConnectionID
 @abstract (null)
 */
@property (nonatomic, retain) NSNumber *streamConnectionID;

/*!
 @property timestamp
 @abstract 
 */
@property (nonatomic, retain) NSNumber *timestamp;

/*!
 @property timestampRawNs
 @abstract Unadjusted wall clock timestamp.
 */
@property (nonatomic, retain) NSNumber *timestampRawNs;

@end

@interface CPTeardownStreamDescriptorRequestContainer : APHTTPBPlistContainer

/*!
 @property type
 @abstract (null)
 */
@property (nonatomic, assign) CPStreamID type;
@property (nonatomic, assign) BOOL hasType;

@end

@interface CPStreamFeedbackContainer : APHTTPBPlistContainer

/*!
 @property type
 @abstract 
 */
@property (nonatomic, assign) CPStreamID type;
@property (nonatomic, assign) BOOL hasType;

/*!
 @property sr
 @abstract 
 */
@property (nonatomic, retain) NSNumber *sr;

/*!
 @property sampleTime
 @abstract 
 */
@property (nonatomic, retain) NSNumber *sampleTime;

/*!
 @property streamConnectionID
 @abstract (null)
 */
@property (nonatomic, retain) NSNumber *streamConnectionID;

/*!
 @property timestamp
 @abstract 
 */
@property (nonatomic, retain) NSNumber *timestamp;

/*!
 @property timestampRawNs
 @abstract Unadjusted wall clock timestamp.
 */
@property (nonatomic, retain) NSNumber *timestampRawNs;

@end

@interface CPHIDDeviceContainer : APHTTPBPlistContainer

/*!
 @property displayUUID
 @abstract 
 */
@property (nonatomic, retain) NSString *displayUUID;

/*!
 @property hidCountryCode
 @abstract 
 */
@property (nonatomic, retain) NSNumber *hidCountryCode;

/*!
 @property hidDescriptor
 @abstract 
 */
@property (nonatomic, retain) NSData *hidDescriptor;
@property (nonatomic, retain) id hidDescriptorRepresentation;

/*!
 @property hidProductID
 @abstract 
 */
@property (nonatomic, retain) NSNumber *hidProductID;

/*!
 @property hidVendorID
 @abstract 
 */
@property (nonatomic, retain) NSNumber *hidVendorID;

/*!
 @property name
 @abstract 
 */
@property (nonatomic, retain) NSString *name;

/*!
 @property uuid
 @abstract 
 */
@property (nonatomic, retain) NSString *uuid;

@end

@interface CPIconKeysContainer : APHTTPBPlistContainer

/*!
 @property imageData
 @abstract 
 */
@property (nonatomic, retain) NSData *imageData;
@property (nonatomic, retain) id imageDataRepresentation;

/*!
 @property heightPixels
 @abstract 
 */
@property (nonatomic, retain) NSNumber *heightPixels;

/*!
 @property widthPixels
 @abstract 
 */
@property (nonatomic, retain) NSNumber *widthPixels;

/*!
 @property prerendered
 @abstract 
 */
@property (nonatomic, assign) BOOL prerendered;
@property (nonatomic, assign) BOOL hasPrerendered;

@end

@interface CPDisplayKeysContainer : APHTTPBPlistContainer

/*!
 @property edid
 @abstract 
 */
@property (nonatomic, retain) NSData *edid;
@property (nonatomic, retain) id edidRepresentation;

/*!
 @property features
 @abstract 
 */
@property (nonatomic, assign) CPDisplayFeatures features;
@property (nonatomic, assign) BOOL hasFeatures;

/*!
 @property maxFPS
 @abstract 
 */
@property (nonatomic, retain) NSNumber *maxFPS;

/*!
 @property heightPixels
 @abstract 
 */
@property (nonatomic, retain) NSNumber *heightPixels;

/*!
 @property widthPixels
 @abstract 
 */
@property (nonatomic, retain) NSNumber *widthPixels;

/*!
 @property heightPhysical
 @abstract 
 */
@property (nonatomic, retain) NSNumber *heightPhysical;

/*!
 @property widthPhysical
 @abstract 
 */
@property (nonatomic, retain) NSNumber *widthPhysical;

/*!
 @property uuid
 @abstract 
 */
@property (nonatomic, retain) NSString *uuid;

/*!
 @property primaryInputDevice
 @abstract (null)
 */
@property (nonatomic, assign) CPPrimaryInputDevice primaryInputDevice;
@property (nonatomic, assign) BOOL hasPrimaryInputDevice;

@end

@interface CPAudioFormatContainer : APHTTPBPlistContainer

/*!
 @property type
 @abstract 
 */
@property (nonatomic, assign) CPStreamID type;
@property (nonatomic, assign) BOOL hasType;

/*!
 @property audioInputFormats
 @abstract 
 */
@property (nonatomic, assign) CPAudioFormatType audioInputFormats;
@property (nonatomic, assign) BOOL hasAudioInputFormats;

/*!
 @property audioOutputFormats
 @abstract 
 */
@property (nonatomic, assign) CPAudioFormatType audioOutputFormats;
@property (nonatomic, assign) BOOL hasAudioOutputFormats;

/*!
 @property audioType
 @abstract 
 */
@property (nonatomic, retain) NSString *audioType;

@end

@interface CPAudioLatencyContainer : APHTTPBPlistContainer

/*!
 @property type
 @abstract 
 */
@property (nonatomic, assign) CPStreamID type;
@property (nonatomic, assign) BOOL hasType;

/*!
 @property audioType
 @abstract 
 */
@property (nonatomic, retain) NSString *audioType;

/*!
 @property sr
 @abstract 
 */
@property (nonatomic, retain) NSNumber *sr;

/*!
 @property ss
 @abstract 
 */
@property (nonatomic, retain) NSNumber *ss;

/*!
 @property ch
 @abstract 
 */
@property (nonatomic, retain) NSNumber *ch;

/*!
 @property inputLatencyMicros
 @abstract 
 */
@property (nonatomic, retain) NSNumber *inputLatencyMicros;

/*!
 @property outputLatencyMicros
 @abstract 
 */
@property (nonatomic, retain) NSNumber *outputLatencyMicros;

@end

@interface CPAppStateRequestInfoContainer : APHTTPBPlistContainer

/*!
 @property appStateID
 @abstract (null)
 */
@property (nonatomic, assign) CPAppState appStateID;
@property (nonatomic, assign) BOOL hasAppStateID;

/*!
 @property speechMode
 @abstract (null)
 */
@property (nonatomic, assign) CPSpeechMode speechMode;
@property (nonatomic, assign) BOOL hasSpeechMode;

/*!
 @property state
 @abstract (null)
 */
@property (nonatomic, assign) BOOL state;
@property (nonatomic, assign) BOOL hasState;

@end

@interface CPResourceRequestInfoContainer : APHTTPBPlistContainer

/*!
 @property resourceID
 @abstract (null)
 */
@property (nonatomic, assign) CPResourceID resourceID;
@property (nonatomic, assign) BOOL hasResourceID;

/*!
 @property transferType
 @abstract (null)
 */
@property (nonatomic, assign) CPResourceOwnershipTransferType transferType;
@property (nonatomic, assign) BOOL hasTransferType;

/*!
 @property transferPriority
 @abstract (null)
 */
@property (nonatomic, assign) CPResourceTransferPriority transferPriority;
@property (nonatomic, assign) BOOL hasTransferPriority;

/*!
 @property takeConstraint
 @abstract (null)
 */
@property (nonatomic, assign) CPResourceConstraint takeConstraint;
@property (nonatomic, assign) BOOL hasTakeConstraint;

/*!
 @property borrowConstraint
 @abstract (null)
 */
@property (nonatomic, assign) CPResourceConstraint borrowConstraint;
@property (nonatomic, assign) BOOL hasBorrowConstraint;

/*!
 @property unborrowConstraint
 @abstract (null)
 */
@property (nonatomic, assign) CPResourceConstraint unborrowConstraint;
@property (nonatomic, assign) BOOL hasUnborrowConstraint;

@end

@interface CPAppStateInfoContainer : APHTTPBPlistContainer

/*!
 @property appStateID
 @abstract (null)
 */
@property (nonatomic, assign) CPAppState appStateID;
@property (nonatomic, assign) BOOL hasAppStateID;

/*!
 @property entity
 @abstract (null)
 */
@property (nonatomic, assign) CPResourceOwner entity;
@property (nonatomic, assign) BOOL hasEntity;

/*!
 @property speechMode
 @abstract (null)
 */
@property (nonatomic, assign) CPSpeechMode speechMode;
@property (nonatomic, assign) BOOL hasSpeechMode;

@end

@interface CPResourceInfoContainer : APHTTPBPlistContainer

/*!
 @property resourceID
 @abstract (null)
 */
@property (nonatomic, assign) CPResourceID resourceID;
@property (nonatomic, assign) BOOL hasResourceID;

/*!
 @property entity
 @abstract (null)
 */
@property (nonatomic, assign) CPResourceOwner entity;
@property (nonatomic, assign) BOOL hasEntity;

@end

@interface CPDuckAudioRequestParamsContainer : APHTTPBPlistContainer

/*!
 @property durationMs
 @abstract 
 */
@property (nonatomic, retain) NSNumber *durationMs;

/*!
 @property volume
 @abstract 
 */
@property (nonatomic, retain) NSString *volume;

@end

@interface CPUnduckAudioRequestParamsContainer : APHTTPBPlistContainer

/*!
 @property durationMs
 @abstract 
 */
@property (nonatomic, retain) NSNumber *durationMs;

@end

@interface CPDisableBluetoothRequestParamsContainer : APHTTPBPlistContainer

/*!
 @property deviceID
 @abstract 
 */
@property (nonatomic, retain) NSString *deviceID;

@end

@interface CPChangeModesRequestParamsContainer : APHTTPBPlistContainer

/*!
 @property appStates
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPAppStateRequestInfoContainer *> *appStates;

/*!
 @property resources
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPResourceRequestInfoContainer *> *resources;

/*!
 @property reasonStr
 @abstract 
 */
@property (nonatomic, retain) NSString *reasonStr;

@end

@interface CPChangeModesResponseParamsContainer : APHTTPBPlistContainer

/*!
 @property appStates
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPAppStateInfoContainer *> *appStates;

/*!
 @property resources
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPResourceInfoContainer *> *resources;

@end

@interface CPModesChangedRequestParamsContainer : APHTTPBPlistContainer

/*!
 @property appStates
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPAppStateInfoContainer *> *appStates;

/*!
 @property resources
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPResourceInfoContainer *> *resources;

@end

@interface CPRequestUIRequestParamsContainer : APHTTPBPlistContainer

/*!
 @property url
 @abstract 
 */
@property (nonatomic, retain) NSString *url;

@end

@interface CPHIDSetInputModeRequestParamsContainer : APHTTPBPlistContainer

/*!
 @property hidInputMode
 @abstract (null)
 */
@property (nonatomic, assign) CPHIDInputModes hidInputMode;
@property (nonatomic, assign) BOOL hasHidInputMode;

/*!
 @property uuid
 @abstract 
 */
@property (nonatomic, retain) NSString *uuid;

@end

@interface CPRequestSiriRequestParamsContainer : APHTTPBPlistContainer

/*!
 @property siriAction
 @abstract (null)
 */
@property (nonatomic, assign) CPSiriActions siriAction;
@property (nonatomic, assign) BOOL hasSiriAction;

@end

@interface CPSetNightModeRequestParamsContainer : APHTTPBPlistContainer

/*!
 @property nightMode
 @abstract 
 */
@property (nonatomic, assign) BOOL nightMode;
@property (nonatomic, assign) BOOL hasNightMode;

@end

@interface CPSetLimitedUIRequestParamsContainer : APHTTPBPlistContainer

/*!
 @property limitedUI
 @abstract 
 */
@property (nonatomic, assign) BOOL limitedUI;
@property (nonatomic, assign) BOOL hasLimitedUI;

@end

@interface CPElectronicTollCollectionKeysContainer : APHTTPBPlistContainer

/*!
 @property active
 @abstract 
 */
@property (nonatomic, assign) BOOL active;
@property (nonatomic, assign) BOOL hasActive;

@end

@interface CPNavigationAidedDrivingKeysContainer : APHTTPBPlistContainer

/*!
 @property active
 @abstract 
 */
@property (nonatomic, assign) BOOL active;
@property (nonatomic, assign) BOOL hasActive;

@end

@interface CPTouchpadSettingsKeysContainer : APHTTPBPlistContainer

/*!
 @property touchpadSensitivity
 @abstract 
 */
@property (nonatomic, retain) NSNumber *touchpadSensitivity;

/*!
 @property supportedHapticFeedbackTypes
 @abstract 
 */
@property (nonatomic, assign) CPHapticFeedbackType supportedHapticFeedbackTypes;
@property (nonatomic, assign) BOOL hasSupportedHapticFeedbackTypes;

@end

@interface CPUserPreferencesKeysContainer : APHTTPBPlistContainer

/*!
 @property touchpadSettings
 @abstract 
 */
@property (nonatomic, retain) CPTouchpadSettingsKeysContainer *touchpadSettings;

@end

@interface CPVehicleInformationKeysContainer : APHTTPBPlistContainer

/*!
 @property ElectronicTollCollection
 @abstract 
 */
@property (nonatomic, retain) CPElectronicTollCollectionKeysContainer *ElectronicTollCollection;

/*!
 @property NavigationAidedDriving
 @abstract 
 */
@property (nonatomic, retain) CPNavigationAidedDrivingKeysContainer *NavigationAidedDriving;

/*!
 @property userPreferences
 @abstract 
 */
@property (nonatomic, retain) CPUserPreferencesKeysContainer *userPreferences;

@end

@interface CPVehicleInformationParamsContainer : APHTTPBPlistContainer

/*!
 @property vehicleInformation
 @abstract 
 */
@property (nonatomic, retain) CPVehicleInformationKeysContainer *vehicleInformation;

@end


#pragma mark - Messages -

@interface CPInfoRequest : APHTTPBPlistContainer

/*!
 @property qualifier
 @abstract 
 */
@property (nonatomic, retain) NSArray<NSString *> *qualifier;

@end

@interface CPInfoResponse : APHTTPBPlistContainer

/*!
 @property audioFormats
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPAudioFormatContainer *> *audioFormats;

/*!
 @property audioLatencies
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPAudioLatencyContainer *> *audioLatencies;

/*!
 @property bluetoothIDs
 @abstract 
 */
@property (nonatomic, retain) NSArray<NSString *> *bluetoothIDs;

/*!
 @property deviceID
 @abstract 
 */
@property (nonatomic, retain) NSString *deviceID;

/*!
 @property displays
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPDisplayKeysContainer *> *displays;

/*!
 @property extendedFeatures
 @abstract 
 */
@property (nonatomic, retain) NSArray<NSString *> *extendedFeatures;

/*!
 @property features
 @abstract 
 */
@property (nonatomic, assign) NSNumber *features;
@property (nonatomic, assign) BOOL hasFeatures;

/*!
 @property firmwareRevision
 @abstract 
 */
@property (nonatomic, retain) NSString *firmwareRevision;

/*!
 @property hardwareRevision
 @abstract 
 */
@property (nonatomic, retain) NSString *hardwareRevision;

/*!
 @property hidDevices
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPHIDDeviceContainer *> *hidDevices;

/*!
 @property hidLanguages
 @abstract 
 */
@property (nonatomic, retain) NSArray<NSString *> *hidLanguages;

/*!
 @property keepAliveLowPower
 @abstract 
 */
@property (nonatomic, assign) BOOL keepAliveLowPower;
@property (nonatomic, assign) BOOL hasKeepAliveLowPower;

/*!
 @property keepAliveSendStatsAsBody
 @abstract 
 */
@property (nonatomic, assign) BOOL keepAliveSendStatsAsBody;
@property (nonatomic, assign) BOOL hasKeepAliveSendStatsAsBody;

/*!
 @property limitedUI
 @abstract 
 */
@property (nonatomic, assign) BOOL limitedUI;
@property (nonatomic, assign) BOOL hasLimitedUI;

/*!
 @property limitedUIElements
 @abstract 
 */
@property (nonatomic, retain) NSArray<NSString *> *limitedUIElements;

/*!
 @property macAddress
 @abstract 
 */
@property (nonatomic, retain) NSString *macAddress;

/*!
 @property manufacturer
 @abstract 
 */
@property (nonatomic, retain) NSString *manufacturer;

/*!
 @property model
 @abstract 
 */
@property (nonatomic, retain) NSString *model;

/*!
 @property modes
 @abstract 
 */
@property (nonatomic, retain) CPChangeModesRequestParamsContainer *modes;

/*!
 @property name
 @abstract 
 */
@property (nonatomic, retain) NSString *name;

/*!
 @property nightMode
 @abstract 
 */
@property (nonatomic, assign) BOOL nightMode;
@property (nonatomic, assign) BOOL hasNightMode;

/*!
 @property oemIconVisible
 @abstract 
 */
@property (nonatomic, assign) BOOL oemIconVisible;
@property (nonatomic, assign) BOOL hasOemIconVisible;

/*!
 @property oemIcons
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPIconKeysContainer *> *oemIcons;

/*!
 @property oemIcon
 @abstract 
 */
@property (nonatomic, retain) NSData *oemIcon;
@property (nonatomic, retain) id oemIconRepresentation;

/*!
 @property oemIconLabel
 @abstract 
 */
@property (nonatomic, retain) NSString *oemIconLabel;

/*!
 @property OSInfo
 @abstract 
 */
@property (nonatomic, retain) NSString *OSInfo;

/*!
 @property pi
 @abstract 
 */
@property (nonatomic, retain) NSString *pi;

/*!
 @property protocolVersion
 @abstract 
 */
@property (nonatomic, retain) NSString *protocolVersion;

/*!
 @property rightHandDrive
 @abstract 
 */
@property (nonatomic, assign) BOOL rightHandDrive;
@property (nonatomic, assign) BOOL hasRightHandDrive;

/*!
 @property sourceVersion
 @abstract 
 */
@property (nonatomic, retain) NSString *sourceVersion;

/*!
 @property statusFlags
 @abstract 
 */
@property (nonatomic, assign) NSNumber *statusFlags;
@property (nonatomic, assign) BOOL hasStatusFlags;

/*!
 @property vehicleInformation
 @abstract 
 */
@property (nonatomic, retain) CPVehicleInformationKeysContainer *vehicleInformation;

@end

@interface CPSetupRequest : APHTTPBPlistContainer

/*!
 @property deviceID
 @abstract 
 */
@property (nonatomic, retain) NSString *deviceID;

/*!
 @property eiv
 @abstract 
 */
@property (nonatomic, retain) NSData *eiv;
@property (nonatomic, retain) id eivRepresentation;

/*!
 @property ekey
 @abstract 
 */
@property (nonatomic, retain) NSData *ekey;
@property (nonatomic, retain) id ekeyRepresentation;

/*!
 @property et
 @abstract 
 */
@property (nonatomic, assign) CPEncryptionType et;
@property (nonatomic, assign) BOOL hasEt;

/*!
 @property macAddress
 @abstract 
 */
@property (nonatomic, retain) NSString *macAddress;

/*!
 @property model
 @abstract 
 */
@property (nonatomic, retain) NSString *model;

/*!
 @property name
 @abstract 
 */
@property (nonatomic, retain) NSString *name;

/*!
 @property osBuildVersion
 @abstract 
 */
@property (nonatomic, retain) NSString *osBuildVersion;

/*!
 @property sessionUUID
 @abstract 
 */
@property (nonatomic, retain) NSString *sessionUUID;

/*!
 @property sourceVersion
 @abstract 
 */
@property (nonatomic, retain) NSString *sourceVersion;

/*!
 @property streams
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPStreamDescriptorRequestContainer *> *streams;

/*!
 @property timingPort
 @abstract 
 */
@property (nonatomic, retain) NSNumber *timingPort;

@end

@interface CPSetupResponse : APHTTPBPlistContainer

/*!
 @property eventPort
 @abstract 
 */
@property (nonatomic, retain) NSNumber *eventPort;

/*!
 @property keepAlivePort
 @abstract 
 */
@property (nonatomic, retain) NSNumber *keepAlivePort;

/*!
 @property streams
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPStreamDescriptorResponseContainer *> *streams;

/*!
 @property timingPort
 @abstract 
 */
@property (nonatomic, retain) NSNumber *timingPort;

@end

@interface CPFlushRequest : APHTTPBPlistContainer

@end

@interface CPFlushResponse : APHTTPBPlistContainer

@end

@interface CPOptionsRequest : APHTTPBPlistContainer

@end

@interface CPOptionsResponse : APHTTPBPlistContainer

@end

@interface CPRecordRequest : APHTTPBPlistContainer

@end

@interface CPRecordResponse : APHTTPBPlistContainer

@end

@interface CPTeardownRequest : APHTTPBPlistContainer

/*!
 @property streams
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPTeardownStreamDescriptorRequestContainer *> *streams;

@end

@interface CPTeardownResponse : APHTTPBPlistContainer

@end

@interface CPConnectCarPlayControlRequestRequest : APHTTPBPlistContainer

@end

@interface CPConnectCarPlayControlRequestResponse : APHTTPBPlistContainer

@end

@interface CPAuthSetupRequest : APHTTPBPlistContainer

@end

@interface CPAuthSetupResponse : APHTTPBPlistContainer

@end

@interface CPFeedbackRequest : APHTTPBPlistContainer

@end

@interface CPFeedbackResponse : APHTTPBPlistContainer

/*!
 @property streams
 @abstract 
 */
@property (nonatomic, retain) NSArray<CPStreamFeedbackContainer *> *streams;

@end

@interface CPLogsRequest : APHTTPBPlistContainer

@end

@interface CPLogsResponse : APHTTPBPlistContainer

@end

@interface CPPairSetupRequest : APHTTPBPlistContainer

@end

@interface CPPairSetupResponse : APHTTPBPlistContainer

@end

@interface CPPairVerifyRequest : APHTTPBPlistContainer

@end

@interface CPPairVerifyResponse : APHTTPBPlistContainer

@end

@interface CPCommandRequest : APHTTPBPlistContainer

/*!
 @property type
 @abstract (null)
 */
@property (nonatomic, retain) NSString *type;

@end

@interface CPDuckAudioRequest : CPCommandRequest

/*!
 @property params
 @abstract (null)
 */
@property (nonatomic, retain) CPDuckAudioRequestParamsContainer *params;

@end

@interface CPUnduckAudioRequest : CPCommandRequest

/*!
 @property params
 @abstract (null)
 */
@property (nonatomic, retain) CPUnduckAudioRequestParamsContainer *params;

@end

@interface CPDisableBluetoothRequest : CPCommandRequest

/*!
 @property params
 @abstract (null)
 */
@property (nonatomic, retain) CPDisableBluetoothRequestParamsContainer *params;

@end

@interface CPChangeModesRequest : CPCommandRequest

/*!
 @property params
 @abstract (null)
 */
@property (nonatomic, retain) CPChangeModesRequestParamsContainer *params;

@end

@interface CPModesChangedRequest : CPCommandRequest

/*!
 @property params
 @abstract (null)
 */
@property (nonatomic, retain) CPModesChangedRequestParamsContainer *params;

@end

@interface CPForceKeyFrameRequest : CPCommandRequest

@end

@interface CPHIDSendReportRequest : CPCommandRequest

/*!
 @property hidReport
 @abstract 
 */
@property (nonatomic, retain) NSData *hidReport;
@property (nonatomic, retain) id hidReportRepresentation;

/*!
 @property timestamp
 @abstract 
 */
@property (nonatomic, retain) NSNumber *timestamp;

/*!
 @property uuid
 @abstract 
 */
@property (nonatomic, retain) NSString *uuid;

@end

@interface CPHIDSetInputModeRequest : CPCommandRequest

/*!
 @property params
 @abstract (null)
 */
@property (nonatomic, retain) CPHIDSetInputModeRequestParamsContainer *params;

@end

@interface CPRequestSiriRequest : CPCommandRequest

/*!
 @property params
 @abstract (null)
 */
@property (nonatomic, retain) CPRequestSiriRequestParamsContainer *params;

@end

@interface CPRequestUIRequest : CPCommandRequest

/*!
 @property params
 @abstract (null)
 */
@property (nonatomic, retain) CPRequestUIRequestParamsContainer *params;

@end

@interface CPSetNightModeRequest : CPCommandRequest

/*!
 @property params
 @abstract (null)
 */
@property (nonatomic, retain) CPSetNightModeRequestParamsContainer *params;

@end

@interface CPUpdateVehicleInformationRequest : CPCommandRequest

/*!
 @property params
 @abstract (null)
 */
@property (nonatomic, retain) CPVehicleInformationParamsContainer *params;

@end

@interface CPSetLimitedUIRequest : CPCommandRequest

/*!
 @property params
 @abstract (null)
 */
@property (nonatomic, retain) CPSetLimitedUIRequestParamsContainer *params;

@end

@interface CPFlushAudioRequest : CPCommandRequest

@end

@interface CPPerformHapticFeedbackRequest : CPCommandRequest

/*!
 @property hapticFeedbackType
 @abstract 
 */
@property (nonatomic, assign) CPHapticFeedbackType hapticFeedbackType;
@property (nonatomic, assign) BOOL hasHapticFeedbackType;

/*!
 @property uuid
 @abstract 
 */
@property (nonatomic, retain) NSString *uuid;

@end

@interface CPCommandResponse : APHTTPBPlistContainer

@end

@interface CPChangeModesResponse : CPCommandResponse

/*!
 @property status
 @abstract 
 */
@property (nonatomic, retain) NSNumber *status;

/*!
 @property params
 @abstract 
 */
@property (nonatomic, retain) CPChangeModesResponseParamsContainer *params;

@end


#pragma mark - Generator -
@interface APHTTPBPlistContainer (TKMessageGeneration)

+ (id)requestMessageContainerForDictionary:(NSDictionary *)dictionary requesterIsAppleDevice:(BOOL)requesterIsAppleDevice request:(APHTTPRequest *)request errorDescription:(NSString **)errorDescription dataSource:(id<APHTTPBPlistDataSource>)dataSource;
+ (id)responseMessageContainerForDictionary:(NSDictionary *)dictionary requestMessageContainer:(id)requestMessageContainer response:(APHTTPResponse *)response errorDescription:(NSString **)errorDescription dataSource:(id<APHTTPBPlistDataSource>)dataSource;

@end
