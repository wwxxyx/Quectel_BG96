/* This file has been auto-generated. Do not edit. To make changes, edit rules.xml. */

@class Rule; // must be externally defined

@protocol WiFiNetworkLoginSharingRules
@required
- (void)runRequestWiFiConnectionInfoRetriedRule:(Rule *)rule;
- (void)runWiFiNetworkLoginSharingRequiredCommandsNotSeenCARule:(Rule *)rule;
@end

@protocol AccessoryIdentificationiAP2Rules
@required
- (void)runAccessoryIdentificationRequiredMessagesNotSeenCAiAP2Rule:(Rule *)rule;
- (void)runDuplicateTransportComponentIdentifieriAP2Rule:(Rule *)rule;
- (void)runIdentificationInformationBeforeStartIdentificationiAP2Rule:(Rule *)rule;
- (void)runIdentificationInformationIncorrectSourceForDeclaredReceivedMessageiAP2Rule:(Rule *)rule;
- (void)runIdentificationInformationIncorrectSourceForDeclaredSentMessageiAP2Rule:(Rule *)rule;
- (void)runIdentificationInformationNonUniqueSupportedMessagesListiAP2Rule:(Rule *)rule;
- (void)runIdentificationInformationUnknownSentOrReceivedMessageiAP2Rule:(Rule *)rule;
- (void)runIdentificationMessageSentAfterIdentificationAcceptediAP2Rule:(Rule *)rule;
- (void)runNoTimelyResponseToStartIdentificationOrIdentificationRejectedMessageiAP2Rule:(Rule *)rule;
- (void)runNoTransportSupportsiAP2ConnectionParameteriAP2Rule:(Rule *)rule;
- (void)runUSBHostHIDComponentUSBHostTransportComponentIdentifierInvalidiAP2Rule:(Rule *)rule;
- (void)runUnsupportedMessageSentiAP2Rule:(Rule *)rule;
- (void)runUnsupportedMessagesDeclaredCAiAP2Rule:(Rule *)rule;
@end

@protocol HIDiAP2Rules
@required
- (void)runAccessoryHIDReportPaddingBitSetToOneiAP2Rule:(Rule *)rule;
- (void)runAccessoryIdentifiediAP2HIDMessagesWithoutiAP2HIDComponentiAP2Rule:(Rule *)rule;
- (void)runAssistiveTouchHIDComponentCountInvalidiAP2Rule:(Rule *)rule;
- (void)runAssistiveTouchRequiredHIDUsagePageMissingNativeUSBiAP2Rule:(Rule *)rule;
- (void)runAssistiveTouchRequiredHIDUsagePageMissingiAP2Rule:(Rule *)rule;
- (void)runDuplicateHIDComponentIdentifieriAP2Rule:(Rule *)rule;
- (void)runGameControllerAccessoryIdentificationExceededNumberOfNFFComponentsiAP2Rule:(Rule *)rule;
- (void)runGameControllerDPadOpposingCardinalDirectionsHIDSeeniAP2Rule:(Rule *)rule;
- (void)runGameControllerDPadOpposingCardinalDirectionsNativeHIDSeeniAP2Rule:(Rule *)rule;
- (void)runGameControllerDigitalSwitchHIDInvalidiAP2Rule:(Rule *)rule;
- (void)runGameControllerDigitalSwitchNativeHIDInvalidiAP2Rule:(Rule *)rule;
- (void)runGameControllerInvalidLEDArrayFieldiAP2Rule:(Rule *)rule;
- (void)runGameControllerInvalidNativeLEDArrayFieldiAP2Rule:(Rule *)rule;
- (void)runGameControllerJoystickAxesHIDFieldSizeNotEqualiAP2Rule:(Rule *)rule;
- (void)runGameControllerJoystickAxesNativeHIDFieldSizeNotEqualiAP2Rule:(Rule *)rule;
- (void)runGameControllerJoystickHIDInvalidiAP2Rule:(Rule *)rule;
- (void)runGameControllerJoystickNativeHIDInvalidiAP2Rule:(Rule *)rule;
- (void)runGameControllerLEDHIDInvalidiAP2Rule:(Rule *)rule;
- (void)runGameControllerLEDNativeHIDInvalidiAP2Rule:(Rule *)rule;
- (void)runGameControllerMultipleFormFittingControllerComponentImplementationiAP2Rule:(Rule *)rule;
- (void)runGameControllerPressureSensitiveSwitchHIDInvalidiAP2Rule:(Rule *)rule;
- (void)runGameControllerPressureSensitiveSwitchNativeHIDInvalidiAP2Rule:(Rule *)rule;
- (void)runGameControllerProhibitedUsagesSeeniAP2Rule:(Rule *)rule;
- (void)runGameControllerRequiredHIDUsagesNotDefinediAP2Rule:(Rule *)rule;
- (void)runGameControllerRequiredNativeHIDUsagesNotDefinediAP2Rule:(Rule *)rule;
- (void)runGameControllerSwitchesHIDNotSeenCAiAP2Rule:(Rule *)rule;
- (void)runGameControllerWithNonControllerComponentiAP2Rule:(Rule *)rule;
- (void)runHIDFunctionClaimWithoutUSBHostHIDComponentCAiAP2Rule:(Rule *)rule;
- (void)runHIDFunctionClaimWithoutiAP2HIDComponentCAiAP2Rule:(Rule *)rule;
- (void)runHIDNativeUSBHIDReportPaddingBitSetToOneiAP2Rule:(Rule *)rule;
- (void)runHIDOveriAP2MessagesNotSeenCAiAP2Rule:(Rule *)rule;
- (void)runIAP2HIDComponentWithoutHIDFunctionClaimCAiAP2Rule:(Rule *)rule;
- (void)runKeyboardRequiredHIDUsagePageMissingNativeUSBiAP2Rule:(Rule *)rule;
- (void)runKeyboardRequiredHIDUsagePageMissingiAP2Rule:(Rule *)rule;
- (void)runKeyboardWithConsumerPageInvalidHIDUsagesiAP2Rule:(Rule *)rule;
- (void)runMediaPlaybackRemoteInvalidHIDUsagesNativeUSBiAP2Rule:(Rule *)rule;
- (void)runMediaPlaybackRemoteInvalidHIDUsagesiAP2Rule:(Rule *)rule;
- (void)runMediaPlaybackRemoteRequiredHIDUsagePageMissingNativeUSBiAP2Rule:(Rule *)rule;
- (void)runMediaPlaybackRemoteRequiredHIDUsagePageMissingiAP2Rule:(Rule *)rule;
- (void)runNoHIDComponentDuringIdentificationCAiAP2Rule:(Rule *)rule;
- (void)runStartHIDHIDComponentIdentifierInvalidiAP2Rule:(Rule *)rule;
- (void)runStopHIDHIDComponentIdentifierInvalidiAP2Rule:(Rule *)rule;
- (void)runSwitchControlHIDButtonCountInvalidiAP2Rule:(Rule *)rule;
- (void)runSwitchControlHIDButtonCountNotRecommendediAP2Rule:(Rule *)rule;
- (void)runSwitchControlHIDComponentCountInvalidiAP2Rule:(Rule *)rule;
- (void)runSwitchControlNativeHIDButtonCountInvalidiAP2Rule:(Rule *)rule;
- (void)runSwitchControlNativeHIDButtonCountNotRecommendediAP2Rule:(Rule *)rule;
- (void)runUSBHostHIDComponentDuplicateInterfaceNumberiAP2Rule:(Rule *)rule;
- (void)runUSBHostHIDComponentUndefinedInterfaceiAP2Rule:(Rule *)rule;
- (void)runUSBHostHIDComponentWithoutHIDFunctionClaimCAiAP2Rule:(Rule *)rule;
- (void)runUsageNotSeenForSupportedHIDUsagesCAiAP2Rule:(Rule *)rule;
- (void)runUsageNotSeenForSupportedHIDUsagesNativeUSBCAiAP2Rule:(Rule *)rule;
@end

@protocol ExternalAccessoryProtocoliAP2Rules
@required
- (void)runEAProtocolParameterSeenDuringIdentificationWithoutEAMessagesDeclarediAP2Rule:(Rule *)rule;
- (void)runEAProtocolParameterSeenDuringIdentificationWithoutEASessionDuringLinkSynchronizationiAP2Rule:(Rule *)rule;
- (void)runEASessionDuringLinkSynchronizationWithoutiAP2EASessionProtocolParameterDuringIdentificationiAP2Rule:(Rule *)rule;
- (void)runExternalAccessoryProtocolIdentifierParametersNotUniqueiAP2Rule:(Rule *)rule;
- (void)runExternalAccessoryProtocolNameParametersNotUniqueiAP2Rule:(Rule *)rule;
- (void)runExternalAccessoryProtocolNativeTransportComponentIdentifierInvalidiAP2Rule:(Rule *)rule;
- (void)runExternalAccessorySessionMessagesNotSeenCAiAP2Rule:(Rule *)rule;
- (void)runNoExternalAccessoryProtocolParameterWhileIdentifyingCAiAP2Rule:(Rule *)rule;
- (void)runStartEAMessageSeenWithoutEADatagramCAiAP2Rule:(Rule *)rule;
- (void)runUSBInterfaceDescriptorsIncorrectForEANativeTransportiAP2Rule:(Rule *)rule;
@end

@protocol IPodOutRules
@required
- (void)runEnterExitExtendedInterfaceModeInsteadOfSetUIModeRule:(Rule *)rule;
- (void)runIPodOutButtonStatusSpacingGreaterThan100msRule:(Rule *)rule;
- (void)runIPodOutRequiredCommandsNotSeenCARule:(Rule *)rule;
- (void)runIPodOutSimpleRemoteCommandWithoutiPodOutModeRule:(Rule *)rule;
- (void)runIdenticalRotationInputStatusSpacingLessThan30msRule:(Rule *)rule;
- (void)runIdenticaliPodOutButtonStatusSpacingLessThan30msRule:(Rule *)rule;
- (void)runLineOutPrefNotSetDuringIDPSForiPodOutRule:(Rule *)rule;
- (void)runRestrictedCommandWhileIniPodOutModeRule:(Rule *)rule;
- (void)runRestrictedContextButtonStatusWhileIniPodOutModeRule:(Rule *)rule;
- (void)runRotationInputStatusSpacingGreaterThan100msRule:(Rule *)rule;
- (void)runScreenInfoTokenDeprecatedRule:(Rule *)rule;
- (void)runTestPatternProhibitedInProductionFirmwareRule:(Rule *)rule;
- (void)runVideoMonitorAspectRatioNotFullscreenWhenEnteringiPodOutModeRule:(Rule *)rule;
- (void)runVideoOutConnectionPrefNotSetDuringIDPSForiPodOutRule:(Rule *)rule;
- (void)runVideoOutNotTurnedOnDuringIDPSForiPodOutRule:(Rule *)rule;
- (void)runVideoSignalFormatNotNTSCWhenEnteringiPodOutModeRule:(Rule *)rule;
- (void)runVideoSignalFormatPrefNotSetDuringIDPSForiPodOutRule:(Rule *)rule;
@end

@protocol StorageLingoAccessoryRules
@required
- (void)runFileWriteBeforeOpenRule:(Rule *)rule;
- (void)runSecondOpeniPodFeatureFileWithoutCloseiPodFileRule:(Rule *)rule;
- (void)runStorageRequiredCommandsNotSeenCARule:(Rule *)rule;
- (void)runWriteBeforeOpeniPodFeatureFileACKRule:(Rule *)rule;
- (void)runWriteToWrongFileHandleRule:(Rule *)rule;
@end

@protocol VoiceOveriAP2Rules
@required
- (void)runVoiceOverMessagesNotSeenCAiAP2Rule:(Rule *)rule;
- (void)runVoiceOverOptionalMessagesNotSeenCAiAP2Rule:(Rule *)rule;
@end

@protocol USBChargingPowerSourceRules
@required
- (void)runSetAvailableCurrentNotSentAfterAuthRule:(Rule *)rule;
- (void)runSetAvailableCurrentValueIncorrectForSupportedAppleDevicesCARule:(Rule *)rule;
- (void)runUSBChargingPowerSourceType500mADeprecated30PinRule:(Rule *)rule;
- (void)runUSBDMinusDrivenWithoutUSBVbus30PinRule:(Rule *)rule;
- (void)runUSBDMinusResistorIncorrectFor1A30PinRule:(Rule *)rule;
- (void)runUSBDMinusResistorIncorrectFor2Point1A30PinRule:(Rule *)rule;
- (void)runUSBDMinusResistorIncorrectFor500mA30PinRule:(Rule *)rule;
- (void)runUSBDPlusDrivenWithoutUSBVbus30PinRule:(Rule *)rule;
- (void)runUSBDPlusResistorIncorrectFor1A30PinRule:(Rule *)rule;
- (void)runUSBDPlusResistorIncorrectFor2Point1A30PinRule:(Rule *)rule;
- (void)runUSBDPlusResistorIncorrectFor500mA30PinRule:(Rule *)rule;
- (void)runUSBVbusVoltageAtAcceptableLevelDuringLoadTest30PinRule:(Rule *)rule;
- (void)runUSBVbusVoltageReturnedToAcceptableLevel30PinRule:(Rule *)rule;
- (void)runUSBVbusVoltageTooHigh30PinRule:(Rule *)rule;
- (void)runUSBVbusVoltageTooLow30PinRule:(Rule *)rule;
- (void)runUSBVbusVoltageTooLowDuringLoadTest30PinRule:(Rule *)rule;
- (void)runUSBVbusVoltageTooLowExtended30PinRule:(Rule *)rule;
@end

@protocol NowPlayingiAP2Rules
@required
- (void)runNowPlaying3rdPartyAppNotSeenCAiAP2Rule:(Rule *)rule;
- (void)runNowPlayingAppleAppsNotSeenCAiAP2Rule:(Rule *)rule;
- (void)runNowPlayingMessagesNotSeenCAiAP2Rule:(Rule *)rule;
@end

@protocol AuthenticationRules
@required
- (void)runAuthenticationV2BadIDLDeviceIDRule:(Rule *)rule;
- (void)runAuthenticationV2Timeout150sRule:(Rule *)rule;
- (void)runDeviceIDNotZeroOrTwoHundredRule:(Rule *)rule;
- (void)runDidNotWaitAfterAuthenticationFailedRule:(Rule *)rule;
- (void)runRetAccessoryAuthInfoNotV2Rule:(Rule *)rule;
@end

@protocol MisciAP2Rules
@required
- (void)runFileTransferSessionDuringLinkSynchronizationWithoutiAP2FileTransferSessionMessagesDuringIdentificationiAP2Rule:(Rule *)rule;
- (void)runFileTransferWithoutFileTransferSessioniAP2Rule:(Rule *)rule;
- (void)runMeasured55Or5ABaudRateOutOfRange8PiniAP2Rule:(Rule *)rule;
- (void)runStartFileTransferMessageSeenWithoutFileTransferDatagramCAiAP2Rule:(Rule *)rule;
@end

@protocol USBHostModeRules
@required
- (void)runDeprecatedRoleSwitchSeenRule:(Rule *)rule;
- (void)runDigitalAudioLingoInUSBHostMode30PinRule:(Rule *)rule;
- (void)runNoNotifyUSBModeDevACKWithin500msRule:(Rule *)rule;
- (void)runUSBAudioAccCapWhenNotInUSBDeviceMode30PinRule:(Rule *)rule;
- (void)runUSBHostModeInterruptInEndpointOptionalRule:(Rule *)rule;
@end

@protocol HardwareCommonRules
@required
- (void)runAccTxVoltageLogicHighTooHigh30PinRule:(Rule *)rule;
- (void)runAccessoryInvalidConnectionLightningTo30PinAdapter8PinRule:(Rule *)rule;
- (void)runAccessoryInvalidConnectionOrientation8PinRule:(Rule *)rule;
- (void)runBaud55Not19200Or5760030PinRule:(Rule *)rule;
- (void)runBaudFFNot19200Or5760030PinRule:(Rule *)rule;
- (void)runDeprecatedFireWirePowerPresent30PinRule:(Rule *)rule;
- (void)runDevicePowerVoltageTooLowExtended8PinRule:(Rule *)rule;
- (void)runDevicePowerVoltageTooLowLoadTest8PinRule:(Rule *)rule;
- (void)runLightningModuleConfigurationDeprecated8PinRule:(Rule *)rule;
- (void)runMeasuredFFBaudRateOutOfRange8PinRule:(Rule *)rule;
- (void)runResistorsIncorrectForClaimedCurrentCA8PinRule:(Rule *)rule;
@end

@protocol VideoOutAccessoryRules
@required
- (void)runNoSetOrCheckiPodPreferencesVideoBeforeVideoOutEnabledRule:(Rule *)rule;
- (void)runVideoOutRequiredCommandsNotSeenCARule:(Rule *)rule;
@end

@protocol USBHIDReportRules
@required
- (void)runAccessoryHIDReportNotDevACKedRule:(Rule *)rule;
- (void)runClosedDescriptorInUnregisterDescriptorRule:(Rule *)rule;
- (void)runIPodHIDReportBeforeRegisterDescriptorACKedRule:(Rule *)rule;
- (void)runRegisterDescriptorHasAppleVIDRule:(Rule *)rule;
- (void)runUnknownDescriptorInUnregisterDescriptorRule:(Rule *)rule;
- (void)runUnknownDescriptorIniPodHIDReportRule:(Rule *)rule;
@end

@protocol AuthenticationiAP2Rules
@required
- (void)runAccessoryAuthenticationCertificateSerialUnparsableiAP2Rule:(Rule *)rule;
- (void)runAuthenticationCertificateTimeoutiAP2Rule:(Rule *)rule;
- (void)runAuthenticationRequiredMessagesNotSeenCAiAP2Rule:(Rule *)rule;
- (void)runAuthenticationResponseTimeoutiAP2Rule:(Rule *)rule;
@end

@protocol MediaLibraryiAP2Rules
@required
- (void)runMediaLibraryInformationMessagesNotSeenCAiAP2Rule:(Rule *)rule;
- (void)runMediaLibraryPlaybackOptionalMessagesNotSeenCAiAP2Rule:(Rule *)rule;
- (void)runMediaLibraryPlaybackUsedWithoutMediaLibraryUpdatesCAiAP2Rule:(Rule *)rule;
- (void)runMediaLibraryPlaybackWithoutMediaLibraryUpdatesInDeclarationiAP2Rule:(Rule *)rule;
- (void)runMediaLibraryUpdatesMessagesNotSeenCAiAP2Rule:(Rule *)rule;
- (void)runMediaLibraryUpdatesUsedWithoutMediaLibraryInformationCAiAP2Rule:(Rule *)rule;
- (void)runMediaLibraryUpdatesWithoutMediaLibraryInformationInDeclarationiAP2Rule:(Rule *)rule;
- (void)runStartMediaLibraryUpdatesWithoutCorrectLastKnownRevisioniAP2Rule:(Rule *)rule;
- (void)runStartSecondMediaLibraryUpdatesWithoutLastKnownRevisioniAP2Rule:(Rule *)rule;
@end

@protocol AccessoryIdentifyResistorRules
@required
- (void)runCommandsFromDeviceWithResistor360k30PinRule:(Rule *)rule;
- (void)runNoTimelyiAPButiAPResistorPresented30PinRule:(Rule *)rule;
- (void)runResistorIDNotValid30PinRule:(Rule *)rule;
- (void)runResistorPresentButNot191k30PinRule:(Rule *)rule;
- (void)runResistorPresentButNot255kOrNone30PinRule:(Rule *)rule;
- (void)runResistorPresentButNot28k30PinRule:(Rule *)rule;
- (void)runResistorPresentButNot549k30PinRule:(Rule *)rule;
@end

@protocol USBDeviceModeAudioiAP2Rules
@required
- (void)runUSBDeviceModeAudioMessagesNotSeenCAiAP2Rule:(Rule *)rule;
@end

@protocol AppLaunchiAP2Rules
@required
- (void)runAppLaunchRequiredMessagesNotSeenCAiAP2Rule:(Rule *)rule;
@end

@protocol GymEquipmentRules
@required
- (void)runNoGetiPodCapsForGymEquipmentRule:(Rule *)rule;
- (void)runNoRetAccessoryCapsForGymEquipmentRule:(Rule *)rule;
- (void)runNoRetiPodCapsForGymEquipmentRule:(Rule *)rule;
- (void)runSportsLingoRequiredCommandsNotSeenCARule:(Rule *)rule;
- (void)runUserDataCommandSentButUserDataNotSupportedRule:(Rule *)rule;
- (void)runWorkoutFeatureDeprecatedRule:(Rule *)rule;
@end

@protocol BluetoothiAP2Rules
@required
- (void)runBluetoothComponentInformationComponentIdentifierInvalidiAP2Rule:(Rule *)rule;
- (void)runBluetoothComponentInformationMissingComponentiAP2Rule:(Rule *)rule;
- (void)runBluetoothComponentInformationNoNewInformationiAP2Rule:(Rule *)rule;
- (void)runBluetoothRequiredMessagesNotSeenCAiAP2Rule:(Rule *)rule;
- (void)runNoBluetoothComponentAfterIdentificationiAP2Rule:(Rule *)rule;
- (void)runNoBluetoothIdentificationCAiAP2Rule:(Rule *)rule;
- (void)runStartBluetoothConnectionUpdatesBluetoothTransportComponentIdentifierInvalidiAP2Rule:(Rule *)rule;
@end

@protocol RFTunerRules
@required
- (void)runHDRadioCommandWithoutHDCapableBitRule:(Rule *)rule;
- (void)runNoResponseToGetHDDataRule:(Rule *)rule;
- (void)runNoResponseToGetHDDataNotifyMaskRule:(Rule *)rule;
- (void)runNoResponseToGetHDDataReadyStatusRule:(Rule *)rule;
- (void)runNoResponseToGetHDProgramServiceRule:(Rule *)rule;
- (void)runNoResponseToGetHDProgramServiceCountRule:(Rule *)rule;
- (void)runRFTunerRequestTimedOutRule:(Rule *)rule;
- (void)runResponseToGetTunerFreqGreaterThan200msRule:(Rule *)rule;
- (void)runResponseToSetTunerFreqGreaterThan200msRule:(Rule *)rule;
@end

@protocol LocationRules
@required
- (void)runAsyncAccessoryDataLocationTypeNotSupportedByAccRule:(Rule *)rule;
- (void)runAsyncAccessoryDataSentWithoutWaitingForiPodACKRule:(Rule *)rule;
- (void)runCurrentGPSTimeSentTooFrequentlyRule:(Rule *)rule;
- (void)runDevACKCommandOKReturnedForOtherThanLastSectionRule:(Rule *)rule;
- (void)runDevACKSectionReceivedReturnedForLastSectionRule:(Rule *)rule;
- (void)runDevACKWithSectionReceivedOKStatusDuplicateSectionIndexRule:(Rule *)rule;
- (void)runLocationLingoRequiredCommandsNotSeenCARule:(Rule *)rule;
- (void)runMultisectionAsyncAccessoryDataLocationDataTypeMismatchRule:(Rule *)rule;
- (void)runMultisectionAsyncAccessoryDataLocationTypeMismatchRule:(Rule *)rule;
- (void)runMultisectionAsyncAccessoryDataWithDataSectionIndexOutOfRangeRule:(Rule *)rule;
- (void)runMultisectionAsyncAccessoryDataWithEmptyPayloadRule:(Rule *)rule;
- (void)runMultisectionAsyncAccessoryDataWithMaximumDataSectionIndexMismatchRule:(Rule *)rule;
- (void)runMultisectionAsyncAccessoryDataWithNonSequentialDataSectionIndexRule:(Rule *)rule;
- (void)runMultisectionSetAccessoryDataValidButEmptyNotACKedRule:(Rule *)rule;
- (void)runMustSetSatEphRequiredBitRule:(Rule *)rule;
- (void)runNoRetAccessoryCapsForGetAccessoryCapsSystemCapsRule:(Rule *)rule;
- (void)runNoRetAccessoryCapsReturnedForSupportedLocationTypeRule:(Rule *)rule;
- (void)runNoRetAccessoryControlReturnedForSupportedControlTypeRule:(Rule *)rule;
- (void)runNoTimelyResponseToGetAccessoryCapsRule:(Rule *)rule;
- (void)runNoTimelyResponseToGetAccessoryControlRule:(Rule *)rule;
- (void)runNoTimelyResponseToGetAccessoryDataRule:(Rule *)rule;
- (void)runNoTimelyResponseToSetAccessoryControlRule:(Rule *)rule;
- (void)runNoTimelyResponseToSetAccessoryDataRule:(Rule *)rule;
- (void)runRetAccessoryCapsLocAsstDataBits1And2DoNotMatchRule:(Rule *)rule;
- (void)runRetAccessoryCapsLocationTypeMismatchRule:(Rule *)rule;
- (void)runRetAccessoryCapsSentWithInvalidLocCapsMaskRule:(Rule *)rule;
- (void)runRetAccessoryControlControlTypeMismatchRule:(Rule *)rule;
- (void)runRetAccessoryDataLocationDataTypeMismatchRule:(Rule *)rule;
- (void)runRetAccessoryDataLocationTypeMismatchRule:(Rule *)rule;
- (void)runRetAccessoryDataSentWithoutWaitingForiPodACKRule:(Rule *)rule;
- (void)runSatelliteEphemerisRequestedTooFrequentlyRule:(Rule *)rule;
- (void)runUnexpectedAsyncAccessoryDataNMEASentenceRule:(Rule *)rule;
- (void)runUnexpectedAsyncAccessoryDataNMEASentenceAfterAccessoryAckRule:(Rule *)rule;
@end

@protocol LocationiAP2Rules
@required
- (void)runLocationMessagesNotSeenCAiAP2Rule:(Rule *)rule;
- (void)runUnwarrantedLocationInformationSentiAP2Rule:(Rule *)rule;
@end

@protocol IDPSRules
@required
- (void)runAccCapsVolumeInfoButNoDisplayRemoteLingoIdentifyRule:(Rule *)rule;
- (void)runAccessoryCapsTokeniPodPreferenceTokenLineOutMismatchRule:(Rule *)rule;
- (void)runAccessoryCapsTokeniPodPreferenceTokenVideoOutConnectionMismatchRule:(Rule *)rule;
- (void)runAccessoryCapsTokeniPodPreferenceTokenVideoOutMismatchRule:(Rule *)rule;
- (void)runAccessoryCapsTokeniPodPreferenceTokenVideoOutMonitorAspectRatioMismatchRule:(Rule *)rule;
- (void)runAccessoryCapsTokeniPodPreferenceTokenVideoOutSignalFormatMismatchRule:(Rule *)rule;
- (void)runAnalogUSBAudioRoutingAccCapWithAVSynchronizationSupportedRule:(Rule *)rule;
- (void)runAnalogUSBAudioRoutingAccCapWithoutAnalogUSBAudioRoutingSupportRule:(Rule *)rule;
- (void)runAnalogUSBAudioRoutingAccCapWithoutIdentifyForDigitalAudioLingoRule:(Rule *)rule;
- (void)runAnalogUSBAudioRoutingAccCapWithoutUSBHostDeviceRule:(Rule *)rule;
- (void)runCapsTokenWithoutLingoIdentifiedRule:(Rule *)rule;
- (void)runDuplicateTransactionIDDetectedRule:(Rule *)rule;
- (void)runEndIDPSRequestingAuthSentWithoutAnySetFIDTokenValuesRule:(Rule *)rule;
- (void)runEndIDPSSentWhileAckFIDTokenValuesPendingRule:(Rule *)rule;
- (void)runFirstCommandNotStartIDPSRule:(Rule *)rule;
- (void)runFirstTokenNotIdentifyTokenRule:(Rule *)rule;
- (void)runGeneralLingoNotInIdentifyTokenRule:(Rule *)rule;
- (void)runGetiPodOptionsForLingoDigitalAudioBeforeAnalogUSBAudioRoutingAccCapRule:(Rule *)rule;
- (void)runGetiPodOptionsForLingoGeneralBeforeAnalogUSBAudioRoutingAccCapRule:(Rule *)rule;
- (void)runIDPSCommandSentBeforeStartIDPSACKedRule:(Rule *)rule;
- (void)runIDPSCommandSentWithoutStartIDPSRule:(Rule *)rule;
- (void)runIDPSStartedButNotCompletedWithin3SecondsRule:(Rule *)rule;
- (void)runIdentificationCommandSentTooSoonAfterFirstStartIDPSRule:(Rule *)rule;
- (void)runIdentifyingForAccessoryPowerLingoIsDeprecatedRule:(Rule *)rule;
- (void)runIdentifyingForMicrophoneLingoIsDeprecatedRule:(Rule *)rule;
- (void)runLegacyIdentifyAfterIDPSRule:(Rule *)rule;
- (void)runLineOutCapsWithoutLineOutPreferencesRule:(Rule *)rule;
- (void)runLineOutDisabledAfterLineOutPreferencesSetRule:(Rule *)rule;
- (void)runMaxConnectionsResponseIgnoredRule:(Rule *)rule;
- (void)runMicrophoneCapsTokenDeprecatedRule:(Rule *)rule;
- (void)runNoIAPAfterStartIDPSNACKRule:(Rule *)rule;
- (void)runNoTimelyIDLAfterStartIDPSNACKRule:(Rule *)rule;
- (void)runNoTimelyResponseToRetiPodAuthenticationInfoRule:(Rule *)rule;
- (void)runProhibitedCommandSentDuringIDPSRule:(Rule *)rule;
- (void)runRequiredAccessoryCapsTokenNotACKedRule:(Rule *)rule;
- (void)runRequiredAccessoryCapsTokenNotSentRule:(Rule *)rule;
- (void)runRequiredAccessoryInfoTokenWithFirmwareVersionNotACKedRule:(Rule *)rule;
- (void)runRequiredAccessoryInfoTokenWithFirmwareVersionNotSentRule:(Rule *)rule;
- (void)runRequiredAccessoryInfoTokenWithHardwareVersionNotACKedRule:(Rule *)rule;
- (void)runRequiredAccessoryInfoTokenWithHardwareVersionNotSentRule:(Rule *)rule;
- (void)runRequiredAccessoryInfoTokenWithManufacturerNotACKedRule:(Rule *)rule;
- (void)runRequiredAccessoryInfoTokenWithManufacturerNotSentRule:(Rule *)rule;
- (void)runRequiredAccessoryInfoTokenWithModelNumberNotACKedRule:(Rule *)rule;
- (void)runRequiredAccessoryInfoTokenWithModelNumberNotSentRule:(Rule *)rule;
- (void)runRequiredAccessoryInfoTokenWithNameNotACKedRule:(Rule *)rule;
- (void)runRequiredAccessoryInfoTokenWithNameNotSentRule:(Rule *)rule;
- (void)runRequiredBundleSeedIDPrefTokenNotACKedRule:(Rule *)rule;
- (void)runRequiredBundleSeedIDPrefTokenNotSentRule:(Rule *)rule;
- (void)runRequiredEAProtocolTokenNotACKedRule:(Rule *)rule;
- (void)runRequiredEAProtocolTokenNotSentRule:(Rule *)rule;
- (void)runRequiredIdentifyTokenNotACKedRule:(Rule *)rule;
- (void)runRequiredMicrophoneCapsTokenNotACKedRule:(Rule *)rule;
- (void)runRequiredMicrophoneCapsTokenNotSentRule:(Rule *)rule;
- (void)runRetriedUnsupportedTokenRule:(Rule *)rule;
- (void)runSetFIDTokenValuesSentWhileAckFIDTokenValuesPendingRule:(Rule *)rule;
- (void)runSetFIDTokenValuesWithoutGetiPodOptionsForLingoRule:(Rule *)rule;
- (void)runSetVideoDelayWithoutRetiPodOptionsForLingoRule:(Rule *)rule;
- (void)runStartIDPSRetriedTooQuicklyRule:(Rule *)rule;
- (void)runStartIDPSSentAfterBadParameterNACKRule:(Rule *)rule;
- (void)runStartIDPSSentAfterRejectionRule:(Rule *)rule;
- (void)runUnrecognizedTransactionIDInResponseRule:(Rule *)rule;
- (void)runVideoOutCapsWithoutVideoOutPreferencesRule:(Rule *)rule;
- (void)runVideoOutDisabledAfterVideoOutPreferencesSetRule:(Rule *)rule;
- (void)runVolumePollingOrInquiryWithoutAccessoryCapsTokenChecksVolumeRule:(Rule *)rule;
@end

@protocol HiddenSharedRules
@required
- (void)runDeprecated191kResistor30PinRule:(Rule *)rule;
- (void)runDeprecated1MResistor30PinRule:(Rule *)rule;
- (void)runDeprecated360kResistor30PinRule:(Rule *)rule;
- (void)runFirstPacket20msDelayViolatedRule:(Rule *)rule;
- (void)runIAPSentWith255kResistor30PinRule:(Rule *)rule;
- (void)runIPodAttachWhenAccessoryAttached30PinRule:(Rule *)rule;
- (void)runSecondByteNotSyncByteRule:(Rule *)rule;
- (void)runSyncByteSentOverUSBRule:(Rule *)rule;
- (void)runTrafficSentBeforeAccessoryPowerTurnedOn30PinRule:(Rule *)rule;
- (void)runTrafficSentButPin20NotGrounded30PinRule:(Rule *)rule;
- (void)runTrafficSentWithin80msOfAccessoryPower30PinRule:(Rule *)rule;
- (void)runTrafficSentWithoutUsesiAPClaimRule:(Rule *)rule;
- (void)runUnexpectedAccResistor30PinRule:(Rule *)rule;
- (void)runVbusDrivenWithoutUSBChargingPowerSourceClaim30PinRule:(Rule *)rule;
@end

@protocol AssistiveTouchRules
@required
- (void)runAssistiveTouchRequiredCommandsNotSeenCARule:(Rule *)rule;
- (void)runAssistiveTouchWithoutAccessoryCapsTokenRule:(Rule *)rule;
- (void)runAssistiveTouchWithoutLingoOptionsRule:(Rule *)rule;
@end

@protocol VoiceOverRules
@required
- (void)runInvalidScaleDisplayFactorRule:(Rule *)rule;
- (void)runVoiceOverEventLastSectionsDifferRule:(Rule *)rule;
- (void)runVoiceOverEventMultipleSectionsOutOfOrderRule:(Rule *)rule;
- (void)runVoiceOverEventWithMixedEventTypesRule:(Rule *)rule;
- (void)runVoiceOverRequiredCommandsNotSeenCARule:(Rule *)rule;
@end

@protocol CommunicationwithiOSApplicationsRules
@required
- (void)runAccessoryDataTransferAfterSessionClosedRule:(Rule *)rule;
- (void)runAccessoryDataTransferAfterSessionClosedAndACKedRule:(Rule *)rule;
- (void)runAccessoryDataTransferBeforeOpenDataSessionForProtocolACKedRule:(Rule *)rule;
- (void)runCloseDataSessionNotDevACKed30PinRule:(Rule *)rule;
- (void)runCommWithAppsClaimedWithoutValidAccessoryCapsTokenRule:(Rule *)rule;
- (void)runCommWithAppsCommandSentWithoutCommWithAppsAccessoryCapsTokenACKedRule:(Rule *)rule;
- (void)runCommunicationWithiOSApplicationsRequiredCommandsNotSeenCARule:(Rule *)rule;
- (void)runDidNotWaitForSessionSpaceAvailableAfterSessionWriteFailureRule:(Rule *)rule;
- (void)runEAProtocolTokenOrBundleSeedIDPrefTokenWhenCommunicationWithiOSUnusableRule:(Rule *)rule;
- (void)runFlowControlNotificationIgnoredRule:(Rule *)rule;
- (void)runIncorrectBadParameterSentForiPodDataTransferRule:(Rule *)rule;
- (void)runNextAccessoryDataTransferTooSoonRule:(Rule *)rule;
- (void)runNoRequestTransportMaxPayloadSizeBeforeAccessoryDataTransferRule:(Rule *)rule;
- (void)runOpenDataSessionNotDevACKedRule:(Rule *)rule;
- (void)runTooManyRetriesOfAccessoryDataTransferRule:(Rule *)rule;
- (void)runUnknownSessionIDInAccessoryDataTransferRule:(Rule *)rule;
@end

@protocol AssistiveTouchiAP2Rules
@required
- (void)runAssistiveTouchRequiredMessagesNotSeenCAiAP2Rule:(Rule *)rule;
@end

@protocol WiFiiAP2Rules
@required
- (void)runWiFiMessagesNotSeenCAiAP2Rule:(Rule *)rule;
@end

@protocol AccessoryEqualizerRules
@required
- (void)runNoResponseToGetCurrentEQIndexRule:(Rule *)rule;
- (void)runNoResponseToGetEQIndexNameRule:(Rule *)rule;
- (void)runNoResponseToGetEQSettingCountRule:(Rule *)rule;
@end

@protocol SimpleRemoteRules
@required
- (void)runFirstCameraButtonStatusIsZeroRule:(Rule *)rule;
- (void)runIdenticalContextButtonStatusSpacingLessThan30msRule:(Rule *)rule;
- (void)runIdenticalVideoButtonStatusSpacingLessThan30msRule:(Rule *)rule;
- (void)runMissingContextButtonStatusRule:(Rule *)rule;
- (void)runMissingVideoButtonStatusRule:(Rule *)rule;
- (void)runRadioButtonStatusSpacingGreaterThan100msRule:(Rule *)rule;
- (void)runRadioButtonStatusSpacingLessThan30msRule:(Rule *)rule;
- (void)runRestrictedContextButtonStatusFromiOSDeviceRule:(Rule *)rule;
- (void)runSimpleRemoteRequiredCommandsNotSeenCARule:(Rule *)rule;
- (void)runWarnZeroCameraButtonStatusMissingRule:(Rule *)rule;
- (void)runZeroCameraButtonStatusMissingRule:(Rule *)rule;
@end

@protocol UsesiAPRules
@required
- (void)runAccessoryInfoContainsAppleTrademarkRule:(Rule *)rule;
- (void)runAccessorySentDeprecatedCommandRule:(Rule *)rule;
- (void)runAccessoryStatusNotificationCommandWithoutAccessoryInfoTokenRule:(Rule *)rule;
- (void)runAccessoryStatusNotificationForSameStatusTypeSentTooFrequentlyRule:(Rule *)rule;
- (void)runAccessoryStatusNotificationMissingRule:(Rule *)rule;
- (void)runAccessoryStatusNotificationWithUnrecognizedStatusTypeRule:(Rule *)rule;
- (void)runAccessoryStatusNotificationWithoutRetAccessoryStatusNotificationRule:(Rule *)rule;
- (void)runAlarmDeprecatedRule:(Rule *)rule;
- (void)runAuthCertClassDoesNotMatchIdentifiedLingoesRule:(Rule *)rule;
- (void)runAuthV2GetSignatureTimeoutRule:(Rule *)rule;
- (void)runBadSignatureReturnedRule:(Rule *)rule;
- (void)runCancelCommandBeforeAccessoryCapsTokenRule:(Rule *)rule;
- (void)runCancelCommandForUnsupportedCommandIDRule:(Rule *)rule;
- (void)runCommWithAppsAccessoryCapsTokenWithoutCommWithAppsClaimRule:(Rule *)rule;
- (void)runCommandFromUnidentifiedLingoRule:(Rule *)rule;
- (void)runCommandRequiresAuthenticationV2Rule:(Rule *)rule;
- (void)runCommandSentAfterIdentificationFailedRule:(Rule *)rule;
- (void)runCommandSentBeforeIdentificationCompletedRule:(Rule *)rule;
- (void)runDeprecatedBitsSetRule:(Rule *)rule;
- (void)runDidNotRegisterForBothNowPlayingApplicationNotificationsRule:(Rule *)rule;
- (void)runDisplayRemoteRequiredCommandsNotSeenCARule:(Rule *)rule;
- (void)runExceededMaxPacketPayloadSizeRule:(Rule *)rule;
- (void)runGetOrSetiPodPreferencesVideoOutRequiresAuthRule:(Rule *)rule;
- (void)runGetWithoutAttemptedNotificationRegistrationRule:(Rule *)rule;
- (void)runGetiPodOptionsWithoutTryingGetiPodOptionsForLingoRule:(Rule *)rule;
- (void)runIAPSentAfterUnsupportedAccessoryRule:(Rule *)rule;
- (void)runIAPSentDuringFlowControlWaitTimeRule:(Rule *)rule;
- (void)runIPodAuthBeforeDeviceAuthRule:(Rule *)rule;
- (void)runIPodOutWithoutClaimRule:(Rule *)rule;
- (void)runIdentificationRule:(Rule *)rule;
- (void)runInsufficientRetryingOfIDLRule:(Rule *)rule;
- (void)runLegacyIdentifyForDigitalAudioRule:(Rule *)rule;
- (void)runLineOutPreferencesSeenWithoutLineOutAccessoryClaimRule:(Rule *)rule;
- (void)runLocationLingoIdentificationWithoutLocationClaimRule:(Rule *)rule;
- (void)runMissingRequestTransportMaxPayloadSizeDuringIDPSRule:(Rule *)rule;
- (void)runNoIdentificationAfterPowerCycle30PinRule:(Rule *)rule;
- (void)runNoNewIdentificationBetweenIDLAndACKRule:(Rule *)rule;
- (void)runNoResponseToGetAccessoryAuthenticationInfoRule:(Rule *)rule;
- (void)runNoResponseToGetAccessoryInfoRule:(Rule *)rule;
- (void)runPendingACKRule:(Rule *)rule;
- (void)runPower100mAExceeded30PinRule:(Rule *)rule;
- (void)runRFTunerIdentificationWithoutClaimRule:(Rule *)rule;
- (void)runRFTunerWithoutClaimRule:(Rule *)rule;
- (void)runRedundantGetAfterNotificationRule:(Rule *)rule;
- (void)runReferToRetiPodOptionsForLingoGeneralBit27NotUSBHostModeBit0Rule:(Rule *)rule;
- (void)runRemoteUIIdentificationWithoutRemoteUIClaimRule:(Rule *)rule;
- (void)runRemoteUIModeCommandsRequireLingo4Rule:(Rule *)rule;
- (void)runRepeatRetAccessoryAuthenticationInfoSentBeforeACKRule:(Rule *)rule;
- (void)runRepeatedGetSentTooQuicklyRule:(Rule *)rule;
- (void)runRequestApplicationLaunchRetriedAfterNACKRule:(Rule *)rule;
- (void)runRequestApplicationLaunchUnsupportedRule:(Rule *)rule;
- (void)runRequestIdentifyNotAnsweredRule:(Rule *)rule;
- (void)runRequestLingoProtocolVersionWithoutTryingGetiPodOptionsForLingoRule:(Rule *)rule;
- (void)runRetAccessoryAuthenticationInfoCertClassUnparsableRule:(Rule *)rule;
- (void)runRetAccessoryAuthenticationInfoCertSerialUnparsableRule:(Rule *)rule;
- (void)runRetAccessoryAuthenticationInfoMaxSectionIndexMismatchRule:(Rule *)rule;
- (void)runRetAccessoryAuthenticationInfoOutOfRangeSectionIndexRule:(Rule *)rule;
- (void)runRetAccessoryAuthenticationInfoWithEmptyCertDataPayloadRule:(Rule *)rule;
- (void)runRetAccessoryInfoTypeTwoDeprecatedRule:(Rule *)rule;
- (void)runRetAccessoryStatusNotificationMissingAfterSetAccessoryStatusNotificationRule:(Rule *)rule;
- (void)runRetAccessoryStatusNotificationWithUnrecognizedBitsRule:(Rule *)rule;
- (void)runSetEventNotificationBeforeRetSupportedEventNotificationRule:(Rule *)rule;
- (void)runSetEventNotificationWithUnrecognizedBitsRule:(Rule *)rule;
- (void)runSetUIModeSentWheniPodOutUnusableRule:(Rule *)rule;
- (void)runSimpleRemoteCommandSentWithoutSimpleRemoteClaimRule:(Rule *)rule;
- (void)runSimpleRemoteIdentifiedWithoutSimpleRemoteClaimRule:(Rule *)rule;
- (void)runSportsLingoIdentificationWithoutGymEquipmentClaimRule:(Rule *)rule;
- (void)runStorageLingoIdentificationWithoutStorageLingoAccessoryClaimRule:(Rule *)rule;
- (void)runUSBHostModeIdentificationWithoutClaimRule:(Rule *)rule;
- (void)runUSBHostModeWithoutClaimRule:(Rule *)rule;
- (void)runVideoOutPreferencesSeenWithoutVideoOutAccessoryClaimRule:(Rule *)rule;
- (void)runVideoOutTurnedOnWithoutVideoOutPreferencesRule:(Rule *)rule;
- (void)runVoiceOverCommandBeforeAccessoryCapsTokenRule:(Rule *)rule;
- (void)runVoiceOverCommandBeforeRetiPodOptionsForLingoRule:(Rule *)rule;
- (void)runVoiceOverWithoutClaimRule:(Rule *)rule;
- (void)runWarnAboutRequestiPodModelNumRule:(Rule *)rule;
- (void)runWarnAboutRequestiPodSoftwareVersionRule:(Rule *)rule;
@end

@protocol RemoteUIHeadUnitRules
@required
- (void)runExtendedInterfaceModeWithoutRegistrationForDatabaseChangeNotificationRule:(Rule *)rule;
- (void)runExtendedInterfaceRequiredCommandsNotSeenCARule:(Rule *)rule;
- (void)runGetLyricsRepeatFrequencyTooFastRule:(Rule *)rule;
- (void)runGetNumPlayingTracksNotSentBeforeGetPBTrackInfoRule:(Rule *)rule;
- (void)runGetPlayStatusBeforePlayControlACKRule:(Rule *)rule;
- (void)runIncorrectSelectDBRecordFieldLengthRule:(Rule *)rule;
- (void)runLingo4DBSelectionRule:(Rule *)rule;
- (void)runLingo4OnlyInExtendedInterfaceModeRule:(Rule *)rule;
- (void)runNoGetArtworkFormatsRule:(Rule *)rule;
- (void)runNoGetCurrentPlayingTrackChapterInfoRule:(Rule *)rule;
- (void)runNoRequestLingoProtocolVersionForLingo4Rule:(Rule *)rule;
- (void)runPlayControlCommandCodeDeprecatedRule:(Rule *)rule;
- (void)runPlayControlCommandNotEndFFREWAfterStartFFRule:(Rule *)rule;
- (void)runPlayControlCommandNotEndFFREWAfterStartREWRule:(Rule *)rule;
- (void)runPlayControlResumeiPodWithoutThirdPartyApplicationCommunicationRule:(Rule *)rule;
- (void)runRecordIndexOutOfBoundsRule:(Rule *)rule;
- (void)runRemoteUIEntryTooSoonAfterExitRule:(Rule *)rule;
- (void)runRemoteUIExitTooSoonAfterEntryRule:(Rule *)rule;
- (void)runSetCurrentPlayingTrackChapterBeforeReturnCurrentPlayingTrackChapterInfoRule:(Rule *)rule;
- (void)runSetCurrentPlayingTrackChapterOutOfBoundsRule:(Rule *)rule;
- (void)runSetDisplayImageTooLateRule:(Rule *)rule;
- (void)runSetPlayStatusChangeNotificationOneByteFormSentWithoutVerifyingFourByteFormUnsupportedRule:(Rule *)rule;
- (void)runUnselectingTracksNotAllowedRule:(Rule *)rule;
@end

@protocol LineOutAccessoryRules
@required
- (void)runAnalogAudioOutRequiredCommandsNotSeenCARule:(Rule *)rule;
- (void)runNoExplicitSetiPodPreferencesAudioRule:(Rule *)rule;
@end

@protocol PoweriAP2Rules
@required
- (void)runDevicePoweredAccessoryFeatureNotSelectedCAiAP2Rule:(Rule *)rule;
- (void)runDevicePoweredAccessoryPowerMessagesNotSeenCAiAP2Rule:(Rule *)rule;
- (void)runDevicePoweredAccessoryUpdateMessagesNotDeclarediAP2Rule:(Rule *)rule;
- (void)runNonDevicePoweredAccessorySentAccessoryPowerModeiAP2Rule:(Rule *)rule;
- (void)runPowerSourceAccessoryInitialPowerSourceUpdateNotSeeniAP2Rule:(Rule *)rule;
- (void)runPowerSourceAccessoryInitialStartPowerUpdatesNotSeeniAP2Rule:(Rule *)rule;
- (void)runPowerSourceAccessoryRequiredMessagesNotDeclarediAP2Rule:(Rule *)rule;
- (void)runPowerSourceUpdateInsufficientPowerSourceUsageSeenForClaimsCAiAP2Rule:(Rule *)rule;
- (void)runSerialNoTimelyExitFromIntermittentHighPowerModeiAP2Rule:(Rule *)rule;
@end

@protocol LinkLayerRules
@required
- (void)runAccessorySentInvalidLinkPacketCAiAP2Rule:(Rule *)rule;
- (void)runLinkDetectSequenceSentTooEarlyiAP2Rule:(Rule *)rule;
- (void)runLinkDetectSequenceSentTooLateiAP2Rule:(Rule *)rule;
- (void)runLinkPacketACKAfterCumulativeAcknowledgmentTimeoutiAP2Rule:(Rule *)rule;
- (void)runLinkPacketACKNumberInvalidiAP2Rule:(Rule *)rule;
- (void)runLinkPacketEAKNumberInvalidiAP2Rule:(Rule *)rule;
- (void)runLinkPacketEAKSentWithoutPacketsOutOfOrderiAP2Rule:(Rule *)rule;
- (void)runLinkPacketLinkSynchronizationCumulativeACKTimeoutNotNegotiatediAP2Rule:(Rule *)rule;
- (void)runLinkPacketLinkSynchronizationLinkVersionNotNegotiatediAP2Rule:(Rule *)rule;
- (void)runLinkPacketLinkSynchronizationMaxCumulativeACKNotNegotiatediAP2Rule:(Rule *)rule;
- (void)runLinkPacketLinkSynchronizationMaxNumOfRetransmissionsNotNegotiatediAP2Rule:(Rule *)rule;
- (void)runLinkPacketLinkSynchronizationNegotiationNotConcludediAP2Rule:(Rule *)rule;
- (void)runLinkPacketLinkSynchronizationNumberOfRetriesExceedediAP2Rule:(Rule *)rule;
- (void)runLinkPacketLinkSynchronizationReactivationBefore80MsiAP2Rule:(Rule *)rule;
- (void)runLinkPacketLinkSynchronizationRetransmissionTimeoutNotNegotiatediAP2Rule:(Rule *)rule;
- (void)runLinkPacketLinkSynchronizationSessionNotNegotiatediAP2Rule:(Rule *)rule;
- (void)runLinkPacketLinkSynchronizationStartSYNPacketInConjunctionWithOtherControlBitiAP2Rule:(Rule *)rule;
- (void)runLinkPacketMaxNumberOfRetransmissionsExceedediAP2Rule:(Rule *)rule;
- (void)runLinkPacketMaxPacketLengthExceedediAP2Rule:(Rule *)rule;
- (void)runLinkPacketNoSYNWithinTimeoutAfterRSTiAP2Rule:(Rule *)rule;
- (void)runLinkPacketNumberOfOutstandingPacketsExceedediAP2Rule:(Rule *)rule;
- (void)runLinkPacketRetransmittedBeforeRetransmissionTimeoutiAP2Rule:(Rule *)rule;
- (void)runLinkPacketSentToUnsupportedDeviceiAP2Rule:(Rule *)rule;
- (void)runLinkPacketSequenceNumberDuplicatediAP2Rule:(Rule *)rule;
- (void)runLinkPacketiAP1CommunicationNotInitiatediAP2Rule:(Rule *)rule;
- (void)runLinkPacketiAP2DetectSequenceNotSentiAP2Rule:(Rule *)rule;
@end

@protocol DeviceNotificationsRules
@required
- (void)runDeviceNotificationsOptionalMessagesNotSeenCAiAP2Rule:(Rule *)rule;
@end

@protocol MessageBasediAP2Rules
@required
- (void)runAppleDeviceMessageContainsUnknownParametersiAP2Rule:(Rule *)rule;
- (void)runIdentificationInformationFirmwareVersionEmptyiAP2Rule:(Rule *)rule;
- (void)runIdentificationInformationHardwareVersionEmptyiAP2Rule:(Rule *)rule;
- (void)runIdentificationInformationManufacturerEmptyiAP2Rule:(Rule *)rule;
- (void)runIdentificationInformationModelIdentifierEmptyiAP2Rule:(Rule *)rule;
- (void)runIdentificationInformationNameEmptyiAP2Rule:(Rule *)rule;
- (void)runIdentificationInformationSerialNumberEmptyiAP2Rule:(Rule *)rule;
- (void)runMessageContainsUnknownParametersiAP2Rule:(Rule *)rule;
@end

