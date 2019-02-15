/* This file has been auto-generated. Do not edit. To make changes, edit rules.xml. */

@protocol WiFiNetworkLoginSharingRuleTests
@required
- (void)testRequestWiFiConnectionInfoRetriedRule;
- (void)testWiFiNetworkLoginSharingRequiredCommandsNotSeenCARule;
@end

@protocol AccessoryIdentificationiAP2RuleTests
@required
- (void)testAccessoryIdentificationRequiredMessagesNotSeenCAiAP2Rule;
- (void)testDuplicateTransportComponentIdentifieriAP2Rule;
- (void)testIdentificationInformationBeforeStartIdentificationiAP2Rule;
- (void)testIdentificationInformationIncorrectSourceForDeclaredReceivedMessageiAP2Rule;
- (void)testIdentificationInformationIncorrectSourceForDeclaredSentMessageiAP2Rule;
- (void)testIdentificationInformationNonUniqueSupportedMessagesListiAP2Rule;
- (void)testIdentificationInformationUnknownSentOrReceivedMessageiAP2Rule;
- (void)testIdentificationMessageSentAfterIdentificationAcceptediAP2Rule;
- (void)testNoTimelyResponseToStartIdentificationOrIdentificationRejectedMessageiAP2Rule;
- (void)testNoTransportSupportsiAP2ConnectionParameteriAP2Rule;
- (void)testUSBHostHIDComponentUSBHostTransportComponentIdentifierInvalidiAP2Rule;
- (void)testUnsupportedMessageSentiAP2Rule;
- (void)testUnsupportedMessagesDeclaredCAiAP2Rule;
@end

@protocol HIDiAP2RuleTests
@required
- (void)testAccessoryHIDReportPaddingBitSetToOneiAP2Rule;
- (void)testAccessoryIdentifiediAP2HIDMessagesWithoutiAP2HIDComponentiAP2Rule;
- (void)testAssistiveTouchHIDComponentCountInvalidiAP2Rule;
- (void)testAssistiveTouchRequiredHIDUsagePageMissingNativeUSBiAP2Rule;
- (void)testAssistiveTouchRequiredHIDUsagePageMissingiAP2Rule;
- (void)testDuplicateHIDComponentIdentifieriAP2Rule;
- (void)testGameControllerAccessoryIdentificationExceededNumberOfNFFComponentsiAP2Rule;
- (void)testGameControllerDPadOpposingCardinalDirectionsHIDSeeniAP2Rule;
- (void)testGameControllerDPadOpposingCardinalDirectionsNativeHIDSeeniAP2Rule;
- (void)testGameControllerDigitalSwitchHIDInvalidiAP2Rule;
- (void)testGameControllerDigitalSwitchNativeHIDInvalidiAP2Rule;
- (void)testGameControllerInvalidLEDArrayFieldiAP2Rule;
- (void)testGameControllerInvalidNativeLEDArrayFieldiAP2Rule;
- (void)testGameControllerJoystickAxesHIDFieldSizeNotEqualiAP2Rule;
- (void)testGameControllerJoystickAxesNativeHIDFieldSizeNotEqualiAP2Rule;
- (void)testGameControllerJoystickHIDInvalidiAP2Rule;
- (void)testGameControllerJoystickNativeHIDInvalidiAP2Rule;
- (void)testGameControllerLEDHIDInvalidiAP2Rule;
- (void)testGameControllerLEDNativeHIDInvalidiAP2Rule;
- (void)testGameControllerMultipleFormFittingControllerComponentImplementationiAP2Rule;
- (void)testGameControllerPressureSensitiveSwitchHIDInvalidiAP2Rule;
- (void)testGameControllerPressureSensitiveSwitchNativeHIDInvalidiAP2Rule;
- (void)testGameControllerProhibitedUsagesSeeniAP2Rule;
- (void)testGameControllerRequiredHIDUsagesNotDefinediAP2Rule;
- (void)testGameControllerRequiredNativeHIDUsagesNotDefinediAP2Rule;
- (void)testGameControllerSwitchesHIDNotSeenCAiAP2Rule;
- (void)testGameControllerWithNonControllerComponentiAP2Rule;
- (void)testHIDFunctionClaimWithoutUSBHostHIDComponentCAiAP2Rule;
- (void)testHIDFunctionClaimWithoutiAP2HIDComponentCAiAP2Rule;
- (void)testHIDNativeUSBHIDReportPaddingBitSetToOneiAP2Rule;
- (void)testHIDOveriAP2MessagesNotSeenCAiAP2Rule;
- (void)testIAP2HIDComponentWithoutHIDFunctionClaimCAiAP2Rule;
- (void)testKeyboardRequiredHIDUsagePageMissingNativeUSBiAP2Rule;
- (void)testKeyboardRequiredHIDUsagePageMissingiAP2Rule;
- (void)testKeyboardWithConsumerPageInvalidHIDUsagesiAP2Rule;
- (void)testMediaPlaybackRemoteInvalidHIDUsagesNativeUSBiAP2Rule;
- (void)testMediaPlaybackRemoteInvalidHIDUsagesiAP2Rule;
- (void)testMediaPlaybackRemoteRequiredHIDUsagePageMissingNativeUSBiAP2Rule;
- (void)testMediaPlaybackRemoteRequiredHIDUsagePageMissingiAP2Rule;
- (void)testNoHIDComponentDuringIdentificationCAiAP2Rule;
- (void)testStartHIDHIDComponentIdentifierInvalidiAP2Rule;
- (void)testStopHIDHIDComponentIdentifierInvalidiAP2Rule;
- (void)testSwitchControlHIDButtonCountInvalidiAP2Rule;
- (void)testSwitchControlHIDButtonCountNotRecommendediAP2Rule;
- (void)testSwitchControlHIDComponentCountInvalidiAP2Rule;
- (void)testSwitchControlNativeHIDButtonCountInvalidiAP2Rule;
- (void)testSwitchControlNativeHIDButtonCountNotRecommendediAP2Rule;
- (void)testUSBHostHIDComponentDuplicateInterfaceNumberiAP2Rule;
- (void)testUSBHostHIDComponentUndefinedInterfaceiAP2Rule;
- (void)testUSBHostHIDComponentWithoutHIDFunctionClaimCAiAP2Rule;
- (void)testUsageNotSeenForSupportedHIDUsagesCAiAP2Rule;
- (void)testUsageNotSeenForSupportedHIDUsagesNativeUSBCAiAP2Rule;
@end

@protocol ExternalAccessoryProtocoliAP2RuleTests
@required
- (void)testEAProtocolParameterSeenDuringIdentificationWithoutEAMessagesDeclarediAP2Rule;
- (void)testEAProtocolParameterSeenDuringIdentificationWithoutEASessionDuringLinkSynchronizationiAP2Rule;
- (void)testEASessionDuringLinkSynchronizationWithoutiAP2EASessionProtocolParameterDuringIdentificationiAP2Rule;
- (void)testExternalAccessoryProtocolIdentifierParametersNotUniqueiAP2Rule;
- (void)testExternalAccessoryProtocolNameParametersNotUniqueiAP2Rule;
- (void)testExternalAccessoryProtocolNativeTransportComponentIdentifierInvalidiAP2Rule;
- (void)testExternalAccessorySessionMessagesNotSeenCAiAP2Rule;
- (void)testNoExternalAccessoryProtocolParameterWhileIdentifyingCAiAP2Rule;
- (void)testStartEAMessageSeenWithoutEADatagramCAiAP2Rule;
- (void)testUSBInterfaceDescriptorsIncorrectForEANativeTransportiAP2Rule;
@end

@protocol IPodOutRuleTests
@required
- (void)testEnterExitExtendedInterfaceModeInsteadOfSetUIModeRule;
- (void)testIPodOutButtonStatusSpacingGreaterThan100msRule;
- (void)testIPodOutRequiredCommandsNotSeenCARule;
- (void)testIPodOutSimpleRemoteCommandWithoutiPodOutModeRule;
- (void)testIdenticalRotationInputStatusSpacingLessThan30msRule;
- (void)testIdenticaliPodOutButtonStatusSpacingLessThan30msRule;
- (void)testLineOutPrefNotSetDuringIDPSForiPodOutRule;
- (void)testRestrictedCommandWhileIniPodOutModeRule;
- (void)testRestrictedContextButtonStatusWhileIniPodOutModeRule;
- (void)testRotationInputStatusSpacingGreaterThan100msRule;
- (void)testScreenInfoTokenDeprecatedRule;
- (void)testTestPatternProhibitedInProductionFirmwareRule;
- (void)testVideoMonitorAspectRatioNotFullscreenWhenEnteringiPodOutModeRule;
- (void)testVideoOutConnectionPrefNotSetDuringIDPSForiPodOutRule;
- (void)testVideoOutNotTurnedOnDuringIDPSForiPodOutRule;
- (void)testVideoSignalFormatNotNTSCWhenEnteringiPodOutModeRule;
- (void)testVideoSignalFormatPrefNotSetDuringIDPSForiPodOutRule;
@end

@protocol StorageLingoAccessoryRuleTests
@required
- (void)testFileWriteBeforeOpenRule;
- (void)testSecondOpeniPodFeatureFileWithoutCloseiPodFileRule;
- (void)testStorageRequiredCommandsNotSeenCARule;
- (void)testWriteBeforeOpeniPodFeatureFileACKRule;
- (void)testWriteToWrongFileHandleRule;
@end

@protocol VoiceOveriAP2RuleTests
@required
- (void)testVoiceOverMessagesNotSeenCAiAP2Rule;
- (void)testVoiceOverOptionalMessagesNotSeenCAiAP2Rule;
@end

@protocol USBChargingPowerSourceRuleTests
@required
- (void)testSetAvailableCurrentNotSentAfterAuthRule;
- (void)testSetAvailableCurrentValueIncorrectForSupportedAppleDevicesCARule;
- (void)testUSBChargingPowerSourceType500mADeprecated30PinRule;
- (void)testUSBDMinusDrivenWithoutUSBVbus30PinRule;
- (void)testUSBDMinusResistorIncorrectFor1A30PinRule;
- (void)testUSBDMinusResistorIncorrectFor2Point1A30PinRule;
- (void)testUSBDMinusResistorIncorrectFor500mA30PinRule;
- (void)testUSBDPlusDrivenWithoutUSBVbus30PinRule;
- (void)testUSBDPlusResistorIncorrectFor1A30PinRule;
- (void)testUSBDPlusResistorIncorrectFor2Point1A30PinRule;
- (void)testUSBDPlusResistorIncorrectFor500mA30PinRule;
- (void)testUSBVbusVoltageAtAcceptableLevelDuringLoadTest30PinRule;
- (void)testUSBVbusVoltageReturnedToAcceptableLevel30PinRule;
- (void)testUSBVbusVoltageTooHigh30PinRule;
- (void)testUSBVbusVoltageTooLow30PinRule;
- (void)testUSBVbusVoltageTooLowDuringLoadTest30PinRule;
- (void)testUSBVbusVoltageTooLowExtended30PinRule;
@end

@protocol NowPlayingiAP2RuleTests
@required
- (void)testNowPlaying3rdPartyAppNotSeenCAiAP2Rule;
- (void)testNowPlayingAppleAppsNotSeenCAiAP2Rule;
- (void)testNowPlayingMessagesNotSeenCAiAP2Rule;
@end

@protocol AuthenticationRuleTests
@required
- (void)testAuthenticationV2BadIDLDeviceIDRule;
- (void)testAuthenticationV2Timeout150sRule;
- (void)testDeviceIDNotZeroOrTwoHundredRule;
- (void)testDidNotWaitAfterAuthenticationFailedRule;
- (void)testRetAccessoryAuthInfoNotV2Rule;
@end

@protocol MisciAP2RuleTests
@required
- (void)testFileTransferSessionDuringLinkSynchronizationWithoutiAP2FileTransferSessionMessagesDuringIdentificationiAP2Rule;
- (void)testFileTransferWithoutFileTransferSessioniAP2Rule;
- (void)testMeasured55Or5ABaudRateOutOfRange8PiniAP2Rule;
- (void)testStartFileTransferMessageSeenWithoutFileTransferDatagramCAiAP2Rule;
@end

@protocol USBHostModeRuleTests
@required
- (void)testDeprecatedRoleSwitchSeenRule;
- (void)testDigitalAudioLingoInUSBHostMode30PinRule;
- (void)testNoNotifyUSBModeDevACKWithin500msRule;
- (void)testUSBAudioAccCapWhenNotInUSBDeviceMode30PinRule;
- (void)testUSBHostModeInterruptInEndpointOptionalRule;
@end

@protocol HardwareCommonRuleTests
@required
- (void)testAccTxVoltageLogicHighTooHigh30PinRule;
- (void)testAccessoryInvalidConnectionLightningTo30PinAdapter8PinRule;
- (void)testAccessoryInvalidConnectionOrientation8PinRule;
- (void)testBaud55Not19200Or5760030PinRule;
- (void)testBaudFFNot19200Or5760030PinRule;
- (void)testDeprecatedFireWirePowerPresent30PinRule;
- (void)testDevicePowerVoltageTooLowExtended8PinRule;
- (void)testDevicePowerVoltageTooLowLoadTest8PinRule;
- (void)testLightningModuleConfigurationDeprecated8PinRule;
- (void)testMeasuredFFBaudRateOutOfRange8PinRule;
- (void)testResistorsIncorrectForClaimedCurrentCA8PinRule;
@end

@protocol VideoOutAccessoryRuleTests
@required
- (void)testNoSetOrCheckiPodPreferencesVideoBeforeVideoOutEnabledRule;
- (void)testVideoOutRequiredCommandsNotSeenCARule;
@end

@protocol USBHIDReportRuleTests
@required
- (void)testAccessoryHIDReportNotDevACKedRule;
- (void)testClosedDescriptorInUnregisterDescriptorRule;
- (void)testIPodHIDReportBeforeRegisterDescriptorACKedRule;
- (void)testRegisterDescriptorHasAppleVIDRule;
- (void)testUnknownDescriptorInUnregisterDescriptorRule;
- (void)testUnknownDescriptorIniPodHIDReportRule;
@end

@protocol AuthenticationiAP2RuleTests
@required
- (void)testAccessoryAuthenticationCertificateSerialUnparsableiAP2Rule;
- (void)testAuthenticationCertificateTimeoutiAP2Rule;
- (void)testAuthenticationRequiredMessagesNotSeenCAiAP2Rule;
- (void)testAuthenticationResponseTimeoutiAP2Rule;
@end

@protocol MediaLibraryiAP2RuleTests
@required
- (void)testMediaLibraryInformationMessagesNotSeenCAiAP2Rule;
- (void)testMediaLibraryPlaybackOptionalMessagesNotSeenCAiAP2Rule;
- (void)testMediaLibraryPlaybackUsedWithoutMediaLibraryUpdatesCAiAP2Rule;
- (void)testMediaLibraryPlaybackWithoutMediaLibraryUpdatesInDeclarationiAP2Rule;
- (void)testMediaLibraryUpdatesMessagesNotSeenCAiAP2Rule;
- (void)testMediaLibraryUpdatesUsedWithoutMediaLibraryInformationCAiAP2Rule;
- (void)testMediaLibraryUpdatesWithoutMediaLibraryInformationInDeclarationiAP2Rule;
- (void)testStartMediaLibraryUpdatesWithoutCorrectLastKnownRevisioniAP2Rule;
- (void)testStartSecondMediaLibraryUpdatesWithoutLastKnownRevisioniAP2Rule;
@end

@protocol AccessoryIdentifyResistorRuleTests
@required
- (void)testCommandsFromDeviceWithResistor360k30PinRule;
- (void)testNoTimelyiAPButiAPResistorPresented30PinRule;
- (void)testResistorIDNotValid30PinRule;
- (void)testResistorPresentButNot191k30PinRule;
- (void)testResistorPresentButNot255kOrNone30PinRule;
- (void)testResistorPresentButNot28k30PinRule;
- (void)testResistorPresentButNot549k30PinRule;
@end

@protocol USBDeviceModeAudioiAP2RuleTests
@required
- (void)testUSBDeviceModeAudioMessagesNotSeenCAiAP2Rule;
@end

@protocol AppLaunchiAP2RuleTests
@required
- (void)testAppLaunchRequiredMessagesNotSeenCAiAP2Rule;
@end

@protocol GymEquipmentRuleTests
@required
- (void)testNoGetiPodCapsForGymEquipmentRule;
- (void)testNoRetAccessoryCapsForGymEquipmentRule;
- (void)testNoRetiPodCapsForGymEquipmentRule;
- (void)testSportsLingoRequiredCommandsNotSeenCARule;
- (void)testUserDataCommandSentButUserDataNotSupportedRule;
- (void)testWorkoutFeatureDeprecatedRule;
@end

@protocol BluetoothiAP2RuleTests
@required
- (void)testBluetoothComponentInformationComponentIdentifierInvalidiAP2Rule;
- (void)testBluetoothComponentInformationMissingComponentiAP2Rule;
- (void)testBluetoothComponentInformationNoNewInformationiAP2Rule;
- (void)testBluetoothRequiredMessagesNotSeenCAiAP2Rule;
- (void)testNoBluetoothComponentAfterIdentificationiAP2Rule;
- (void)testNoBluetoothIdentificationCAiAP2Rule;
- (void)testStartBluetoothConnectionUpdatesBluetoothTransportComponentIdentifierInvalidiAP2Rule;
@end

@protocol RFTunerRuleTests
@required
- (void)testHDRadioCommandWithoutHDCapableBitRule;
- (void)testNoResponseToGetHDDataRule;
- (void)testNoResponseToGetHDDataNotifyMaskRule;
- (void)testNoResponseToGetHDDataReadyStatusRule;
- (void)testNoResponseToGetHDProgramServiceRule;
- (void)testNoResponseToGetHDProgramServiceCountRule;
- (void)testRFTunerRequestTimedOutRule;
- (void)testResponseToGetTunerFreqGreaterThan200msRule;
- (void)testResponseToSetTunerFreqGreaterThan200msRule;
@end

@protocol LocationRuleTests
@required
- (void)testAsyncAccessoryDataLocationTypeNotSupportedByAccRule;
- (void)testAsyncAccessoryDataSentWithoutWaitingForiPodACKRule;
- (void)testCurrentGPSTimeSentTooFrequentlyRule;
- (void)testDevACKCommandOKReturnedForOtherThanLastSectionRule;
- (void)testDevACKSectionReceivedReturnedForLastSectionRule;
- (void)testDevACKWithSectionReceivedOKStatusDuplicateSectionIndexRule;
- (void)testLocationLingoRequiredCommandsNotSeenCARule;
- (void)testMultisectionAsyncAccessoryDataLocationDataTypeMismatchRule;
- (void)testMultisectionAsyncAccessoryDataLocationTypeMismatchRule;
- (void)testMultisectionAsyncAccessoryDataWithDataSectionIndexOutOfRangeRule;
- (void)testMultisectionAsyncAccessoryDataWithEmptyPayloadRule;
- (void)testMultisectionAsyncAccessoryDataWithMaximumDataSectionIndexMismatchRule;
- (void)testMultisectionAsyncAccessoryDataWithNonSequentialDataSectionIndexRule;
- (void)testMultisectionSetAccessoryDataValidButEmptyNotACKedRule;
- (void)testMustSetSatEphRequiredBitRule;
- (void)testNoRetAccessoryCapsForGetAccessoryCapsSystemCapsRule;
- (void)testNoRetAccessoryCapsReturnedForSupportedLocationTypeRule;
- (void)testNoRetAccessoryControlReturnedForSupportedControlTypeRule;
- (void)testNoTimelyResponseToGetAccessoryCapsRule;
- (void)testNoTimelyResponseToGetAccessoryControlRule;
- (void)testNoTimelyResponseToGetAccessoryDataRule;
- (void)testNoTimelyResponseToSetAccessoryControlRule;
- (void)testNoTimelyResponseToSetAccessoryDataRule;
- (void)testRetAccessoryCapsLocAsstDataBits1And2DoNotMatchRule;
- (void)testRetAccessoryCapsLocationTypeMismatchRule;
- (void)testRetAccessoryCapsSentWithInvalidLocCapsMaskRule;
- (void)testRetAccessoryControlControlTypeMismatchRule;
- (void)testRetAccessoryDataLocationDataTypeMismatchRule;
- (void)testRetAccessoryDataLocationTypeMismatchRule;
- (void)testRetAccessoryDataSentWithoutWaitingForiPodACKRule;
- (void)testSatelliteEphemerisRequestedTooFrequentlyRule;
- (void)testUnexpectedAsyncAccessoryDataNMEASentenceRule;
- (void)testUnexpectedAsyncAccessoryDataNMEASentenceAfterAccessoryAckRule;
@end

@protocol LocationiAP2RuleTests
@required
- (void)testLocationMessagesNotSeenCAiAP2Rule;
- (void)testUnwarrantedLocationInformationSentiAP2Rule;
@end

@protocol IDPSRuleTests
@required
- (void)testAccCapsVolumeInfoButNoDisplayRemoteLingoIdentifyRule;
- (void)testAccessoryCapsTokeniPodPreferenceTokenLineOutMismatchRule;
- (void)testAccessoryCapsTokeniPodPreferenceTokenVideoOutConnectionMismatchRule;
- (void)testAccessoryCapsTokeniPodPreferenceTokenVideoOutMismatchRule;
- (void)testAccessoryCapsTokeniPodPreferenceTokenVideoOutMonitorAspectRatioMismatchRule;
- (void)testAccessoryCapsTokeniPodPreferenceTokenVideoOutSignalFormatMismatchRule;
- (void)testAnalogUSBAudioRoutingAccCapWithAVSynchronizationSupportedRule;
- (void)testAnalogUSBAudioRoutingAccCapWithoutAnalogUSBAudioRoutingSupportRule;
- (void)testAnalogUSBAudioRoutingAccCapWithoutIdentifyForDigitalAudioLingoRule;
- (void)testAnalogUSBAudioRoutingAccCapWithoutUSBHostDeviceRule;
- (void)testCapsTokenWithoutLingoIdentifiedRule;
- (void)testDuplicateTransactionIDDetectedRule;
- (void)testEndIDPSRequestingAuthSentWithoutAnySetFIDTokenValuesRule;
- (void)testEndIDPSSentWhileAckFIDTokenValuesPendingRule;
- (void)testFirstCommandNotStartIDPSRule;
- (void)testFirstTokenNotIdentifyTokenRule;
- (void)testGeneralLingoNotInIdentifyTokenRule;
- (void)testGetiPodOptionsForLingoDigitalAudioBeforeAnalogUSBAudioRoutingAccCapRule;
- (void)testGetiPodOptionsForLingoGeneralBeforeAnalogUSBAudioRoutingAccCapRule;
- (void)testIDPSCommandSentBeforeStartIDPSACKedRule;
- (void)testIDPSCommandSentWithoutStartIDPSRule;
- (void)testIDPSStartedButNotCompletedWithin3SecondsRule;
- (void)testIdentificationCommandSentTooSoonAfterFirstStartIDPSRule;
- (void)testIdentifyingForAccessoryPowerLingoIsDeprecatedRule;
- (void)testIdentifyingForMicrophoneLingoIsDeprecatedRule;
- (void)testLegacyIdentifyAfterIDPSRule;
- (void)testLineOutCapsWithoutLineOutPreferencesRule;
- (void)testLineOutDisabledAfterLineOutPreferencesSetRule;
- (void)testMaxConnectionsResponseIgnoredRule;
- (void)testMicrophoneCapsTokenDeprecatedRule;
- (void)testNoIAPAfterStartIDPSNACKRule;
- (void)testNoTimelyIDLAfterStartIDPSNACKRule;
- (void)testNoTimelyResponseToRetiPodAuthenticationInfoRule;
- (void)testProhibitedCommandSentDuringIDPSRule;
- (void)testRequiredAccessoryCapsTokenNotACKedRule;
- (void)testRequiredAccessoryCapsTokenNotSentRule;
- (void)testRequiredAccessoryInfoTokenWithFirmwareVersionNotACKedRule;
- (void)testRequiredAccessoryInfoTokenWithFirmwareVersionNotSentRule;
- (void)testRequiredAccessoryInfoTokenWithHardwareVersionNotACKedRule;
- (void)testRequiredAccessoryInfoTokenWithHardwareVersionNotSentRule;
- (void)testRequiredAccessoryInfoTokenWithManufacturerNotACKedRule;
- (void)testRequiredAccessoryInfoTokenWithManufacturerNotSentRule;
- (void)testRequiredAccessoryInfoTokenWithModelNumberNotACKedRule;
- (void)testRequiredAccessoryInfoTokenWithModelNumberNotSentRule;
- (void)testRequiredAccessoryInfoTokenWithNameNotACKedRule;
- (void)testRequiredAccessoryInfoTokenWithNameNotSentRule;
- (void)testRequiredBundleSeedIDPrefTokenNotACKedRule;
- (void)testRequiredBundleSeedIDPrefTokenNotSentRule;
- (void)testRequiredEAProtocolTokenNotACKedRule;
- (void)testRequiredEAProtocolTokenNotSentRule;
- (void)testRequiredIdentifyTokenNotACKedRule;
- (void)testRequiredMicrophoneCapsTokenNotACKedRule;
- (void)testRequiredMicrophoneCapsTokenNotSentRule;
- (void)testRetriedUnsupportedTokenRule;
- (void)testSetFIDTokenValuesSentWhileAckFIDTokenValuesPendingRule;
- (void)testSetFIDTokenValuesWithoutGetiPodOptionsForLingoRule;
- (void)testSetVideoDelayWithoutRetiPodOptionsForLingoRule;
- (void)testStartIDPSRetriedTooQuicklyRule;
- (void)testStartIDPSSentAfterBadParameterNACKRule;
- (void)testStartIDPSSentAfterRejectionRule;
- (void)testUnrecognizedTransactionIDInResponseRule;
- (void)testVideoOutCapsWithoutVideoOutPreferencesRule;
- (void)testVideoOutDisabledAfterVideoOutPreferencesSetRule;
- (void)testVolumePollingOrInquiryWithoutAccessoryCapsTokenChecksVolumeRule;
@end

@protocol HiddenSharedRuleTests
@required
- (void)testDeprecated191kResistor30PinRule;
- (void)testDeprecated1MResistor30PinRule;
- (void)testDeprecated360kResistor30PinRule;
- (void)testFirstPacket20msDelayViolatedRule;
- (void)testIAPSentWith255kResistor30PinRule;
- (void)testIPodAttachWhenAccessoryAttached30PinRule;
- (void)testSecondByteNotSyncByteRule;
- (void)testSyncByteSentOverUSBRule;
- (void)testTrafficSentBeforeAccessoryPowerTurnedOn30PinRule;
- (void)testTrafficSentButPin20NotGrounded30PinRule;
- (void)testTrafficSentWithin80msOfAccessoryPower30PinRule;
- (void)testTrafficSentWithoutUsesiAPClaimRule;
- (void)testUnexpectedAccResistor30PinRule;
- (void)testVbusDrivenWithoutUSBChargingPowerSourceClaim30PinRule;
@end

@protocol AssistiveTouchRuleTests
@required
- (void)testAssistiveTouchRequiredCommandsNotSeenCARule;
- (void)testAssistiveTouchWithoutAccessoryCapsTokenRule;
- (void)testAssistiveTouchWithoutLingoOptionsRule;
@end

@protocol VoiceOverRuleTests
@required
- (void)testInvalidScaleDisplayFactorRule;
- (void)testVoiceOverEventLastSectionsDifferRule;
- (void)testVoiceOverEventMultipleSectionsOutOfOrderRule;
- (void)testVoiceOverEventWithMixedEventTypesRule;
- (void)testVoiceOverRequiredCommandsNotSeenCARule;
@end

@protocol CommunicationwithiOSApplicationsRuleTests
@required
- (void)testAccessoryDataTransferAfterSessionClosedRule;
- (void)testAccessoryDataTransferAfterSessionClosedAndACKedRule;
- (void)testAccessoryDataTransferBeforeOpenDataSessionForProtocolACKedRule;
- (void)testCloseDataSessionNotDevACKed30PinRule;
- (void)testCommWithAppsClaimedWithoutValidAccessoryCapsTokenRule;
- (void)testCommWithAppsCommandSentWithoutCommWithAppsAccessoryCapsTokenACKedRule;
- (void)testCommunicationWithiOSApplicationsRequiredCommandsNotSeenCARule;
- (void)testDidNotWaitForSessionSpaceAvailableAfterSessionWriteFailureRule;
- (void)testEAProtocolTokenOrBundleSeedIDPrefTokenWhenCommunicationWithiOSUnusableRule;
- (void)testFlowControlNotificationIgnoredRule;
- (void)testIncorrectBadParameterSentForiPodDataTransferRule;
- (void)testNextAccessoryDataTransferTooSoonRule;
- (void)testNoRequestTransportMaxPayloadSizeBeforeAccessoryDataTransferRule;
- (void)testOpenDataSessionNotDevACKedRule;
- (void)testTooManyRetriesOfAccessoryDataTransferRule;
- (void)testUnknownSessionIDInAccessoryDataTransferRule;
@end

@protocol AssistiveTouchiAP2RuleTests
@required
- (void)testAssistiveTouchRequiredMessagesNotSeenCAiAP2Rule;
@end

@protocol WiFiiAP2RuleTests
@required
- (void)testWiFiMessagesNotSeenCAiAP2Rule;
@end

@protocol AccessoryEqualizerRuleTests
@required
- (void)testNoResponseToGetCurrentEQIndexRule;
- (void)testNoResponseToGetEQIndexNameRule;
- (void)testNoResponseToGetEQSettingCountRule;
@end

@protocol SimpleRemoteRuleTests
@required
- (void)testFirstCameraButtonStatusIsZeroRule;
- (void)testIdenticalContextButtonStatusSpacingLessThan30msRule;
- (void)testIdenticalVideoButtonStatusSpacingLessThan30msRule;
- (void)testMissingContextButtonStatusRule;
- (void)testMissingVideoButtonStatusRule;
- (void)testRadioButtonStatusSpacingGreaterThan100msRule;
- (void)testRadioButtonStatusSpacingLessThan30msRule;
- (void)testRestrictedContextButtonStatusFromiOSDeviceRule;
- (void)testSimpleRemoteRequiredCommandsNotSeenCARule;
- (void)testWarnZeroCameraButtonStatusMissingRule;
- (void)testZeroCameraButtonStatusMissingRule;
@end

@protocol UsesiAPRuleTests
@required
- (void)testAccessoryInfoContainsAppleTrademarkRule;
- (void)testAccessorySentDeprecatedCommandRule;
- (void)testAccessoryStatusNotificationCommandWithoutAccessoryInfoTokenRule;
- (void)testAccessoryStatusNotificationForSameStatusTypeSentTooFrequentlyRule;
- (void)testAccessoryStatusNotificationMissingRule;
- (void)testAccessoryStatusNotificationWithUnrecognizedStatusTypeRule;
- (void)testAccessoryStatusNotificationWithoutRetAccessoryStatusNotificationRule;
- (void)testAlarmDeprecatedRule;
- (void)testAuthCertClassDoesNotMatchIdentifiedLingoesRule;
- (void)testAuthV2GetSignatureTimeoutRule;
- (void)testBadSignatureReturnedRule;
- (void)testCancelCommandBeforeAccessoryCapsTokenRule;
- (void)testCancelCommandForUnsupportedCommandIDRule;
- (void)testCommWithAppsAccessoryCapsTokenWithoutCommWithAppsClaimRule;
- (void)testCommandFromUnidentifiedLingoRule;
- (void)testCommandRequiresAuthenticationV2Rule;
- (void)testCommandSentAfterIdentificationFailedRule;
- (void)testCommandSentBeforeIdentificationCompletedRule;
- (void)testDeprecatedBitsSetRule;
- (void)testDidNotRegisterForBothNowPlayingApplicationNotificationsRule;
- (void)testDisplayRemoteRequiredCommandsNotSeenCARule;
- (void)testExceededMaxPacketPayloadSizeRule;
- (void)testGetOrSetiPodPreferencesVideoOutRequiresAuthRule;
- (void)testGetWithoutAttemptedNotificationRegistrationRule;
- (void)testGetiPodOptionsWithoutTryingGetiPodOptionsForLingoRule;
- (void)testIAPSentAfterUnsupportedAccessoryRule;
- (void)testIAPSentDuringFlowControlWaitTimeRule;
- (void)testIPodAuthBeforeDeviceAuthRule;
- (void)testIPodOutWithoutClaimRule;
- (void)testIdentificationRule;
- (void)testInsufficientRetryingOfIDLRule;
- (void)testLegacyIdentifyForDigitalAudioRule;
- (void)testLineOutPreferencesSeenWithoutLineOutAccessoryClaimRule;
- (void)testLocationLingoIdentificationWithoutLocationClaimRule;
- (void)testMissingRequestTransportMaxPayloadSizeDuringIDPSRule;
- (void)testNoIdentificationAfterPowerCycle30PinRule;
- (void)testNoNewIdentificationBetweenIDLAndACKRule;
- (void)testNoResponseToGetAccessoryAuthenticationInfoRule;
- (void)testNoResponseToGetAccessoryInfoRule;
- (void)testPendingACKRule;
- (void)testPower100mAExceeded30PinRule;
- (void)testRFTunerIdentificationWithoutClaimRule;
- (void)testRFTunerWithoutClaimRule;
- (void)testRedundantGetAfterNotificationRule;
- (void)testReferToRetiPodOptionsForLingoGeneralBit27NotUSBHostModeBit0Rule;
- (void)testRemoteUIIdentificationWithoutRemoteUIClaimRule;
- (void)testRemoteUIModeCommandsRequireLingo4Rule;
- (void)testRepeatRetAccessoryAuthenticationInfoSentBeforeACKRule;
- (void)testRepeatedGetSentTooQuicklyRule;
- (void)testRequestApplicationLaunchRetriedAfterNACKRule;
- (void)testRequestApplicationLaunchUnsupportedRule;
- (void)testRequestIdentifyNotAnsweredRule;
- (void)testRequestLingoProtocolVersionWithoutTryingGetiPodOptionsForLingoRule;
- (void)testRetAccessoryAuthenticationInfoCertClassUnparsableRule;
- (void)testRetAccessoryAuthenticationInfoCertSerialUnparsableRule;
- (void)testRetAccessoryAuthenticationInfoMaxSectionIndexMismatchRule;
- (void)testRetAccessoryAuthenticationInfoOutOfRangeSectionIndexRule;
- (void)testRetAccessoryAuthenticationInfoWithEmptyCertDataPayloadRule;
- (void)testRetAccessoryInfoTypeTwoDeprecatedRule;
- (void)testRetAccessoryStatusNotificationMissingAfterSetAccessoryStatusNotificationRule;
- (void)testRetAccessoryStatusNotificationWithUnrecognizedBitsRule;
- (void)testSetEventNotificationBeforeRetSupportedEventNotificationRule;
- (void)testSetEventNotificationWithUnrecognizedBitsRule;
- (void)testSetUIModeSentWheniPodOutUnusableRule;
- (void)testSimpleRemoteCommandSentWithoutSimpleRemoteClaimRule;
- (void)testSimpleRemoteIdentifiedWithoutSimpleRemoteClaimRule;
- (void)testSportsLingoIdentificationWithoutGymEquipmentClaimRule;
- (void)testStorageLingoIdentificationWithoutStorageLingoAccessoryClaimRule;
- (void)testUSBHostModeIdentificationWithoutClaimRule;
- (void)testUSBHostModeWithoutClaimRule;
- (void)testVideoOutPreferencesSeenWithoutVideoOutAccessoryClaimRule;
- (void)testVideoOutTurnedOnWithoutVideoOutPreferencesRule;
- (void)testVoiceOverCommandBeforeAccessoryCapsTokenRule;
- (void)testVoiceOverCommandBeforeRetiPodOptionsForLingoRule;
- (void)testVoiceOverWithoutClaimRule;
- (void)testWarnAboutRequestiPodModelNumRule;
- (void)testWarnAboutRequestiPodSoftwareVersionRule;
@end

@protocol RemoteUIHeadUnitRuleTests
@required
- (void)testExtendedInterfaceModeWithoutRegistrationForDatabaseChangeNotificationRule;
- (void)testExtendedInterfaceRequiredCommandsNotSeenCARule;
- (void)testGetLyricsRepeatFrequencyTooFastRule;
- (void)testGetNumPlayingTracksNotSentBeforeGetPBTrackInfoRule;
- (void)testGetPlayStatusBeforePlayControlACKRule;
- (void)testIncorrectSelectDBRecordFieldLengthRule;
- (void)testLingo4DBSelectionRule;
- (void)testLingo4OnlyInExtendedInterfaceModeRule;
- (void)testNoGetArtworkFormatsRule;
- (void)testNoGetCurrentPlayingTrackChapterInfoRule;
- (void)testNoRequestLingoProtocolVersionForLingo4Rule;
- (void)testPlayControlCommandCodeDeprecatedRule;
- (void)testPlayControlCommandNotEndFFREWAfterStartFFRule;
- (void)testPlayControlCommandNotEndFFREWAfterStartREWRule;
- (void)testPlayControlResumeiPodWithoutThirdPartyApplicationCommunicationRule;
- (void)testRecordIndexOutOfBoundsRule;
- (void)testRemoteUIEntryTooSoonAfterExitRule;
- (void)testRemoteUIExitTooSoonAfterEntryRule;
- (void)testSetCurrentPlayingTrackChapterBeforeReturnCurrentPlayingTrackChapterInfoRule;
- (void)testSetCurrentPlayingTrackChapterOutOfBoundsRule;
- (void)testSetDisplayImageTooLateRule;
- (void)testSetPlayStatusChangeNotificationOneByteFormSentWithoutVerifyingFourByteFormUnsupportedRule;
- (void)testUnselectingTracksNotAllowedRule;
@end

@protocol LineOutAccessoryRuleTests
@required
- (void)testAnalogAudioOutRequiredCommandsNotSeenCARule;
- (void)testNoExplicitSetiPodPreferencesAudioRule;
@end

@protocol PoweriAP2RuleTests
@required
- (void)testDevicePoweredAccessoryFeatureNotSelectedCAiAP2Rule;
- (void)testDevicePoweredAccessoryPowerMessagesNotSeenCAiAP2Rule;
- (void)testDevicePoweredAccessoryUpdateMessagesNotDeclarediAP2Rule;
- (void)testNonDevicePoweredAccessorySentAccessoryPowerModeiAP2Rule;
- (void)testPowerSourceAccessoryInitialPowerSourceUpdateNotSeeniAP2Rule;
- (void)testPowerSourceAccessoryInitialStartPowerUpdatesNotSeeniAP2Rule;
- (void)testPowerSourceAccessoryRequiredMessagesNotDeclarediAP2Rule;
- (void)testPowerSourceUpdateInsufficientPowerSourceUsageSeenForClaimsCAiAP2Rule;
- (void)testSerialNoTimelyExitFromIntermittentHighPowerModeiAP2Rule;
@end

@protocol LinkLayerRuleTests
@required
- (void)testAccessorySentInvalidLinkPacketCAiAP2Rule;
- (void)testLinkDetectSequenceSentTooEarlyiAP2Rule;
- (void)testLinkDetectSequenceSentTooLateiAP2Rule;
- (void)testLinkPacketACKAfterCumulativeAcknowledgmentTimeoutiAP2Rule;
- (void)testLinkPacketACKNumberInvalidiAP2Rule;
- (void)testLinkPacketEAKNumberInvalidiAP2Rule;
- (void)testLinkPacketEAKSentWithoutPacketsOutOfOrderiAP2Rule;
- (void)testLinkPacketLinkSynchronizationCumulativeACKTimeoutNotNegotiatediAP2Rule;
- (void)testLinkPacketLinkSynchronizationLinkVersionNotNegotiatediAP2Rule;
- (void)testLinkPacketLinkSynchronizationMaxCumulativeACKNotNegotiatediAP2Rule;
- (void)testLinkPacketLinkSynchronizationMaxNumOfRetransmissionsNotNegotiatediAP2Rule;
- (void)testLinkPacketLinkSynchronizationNegotiationNotConcludediAP2Rule;
- (void)testLinkPacketLinkSynchronizationNumberOfRetriesExceedediAP2Rule;
- (void)testLinkPacketLinkSynchronizationReactivationBefore80MsiAP2Rule;
- (void)testLinkPacketLinkSynchronizationRetransmissionTimeoutNotNegotiatediAP2Rule;
- (void)testLinkPacketLinkSynchronizationSessionNotNegotiatediAP2Rule;
- (void)testLinkPacketLinkSynchronizationStartSYNPacketInConjunctionWithOtherControlBitiAP2Rule;
- (void)testLinkPacketMaxNumberOfRetransmissionsExceedediAP2Rule;
- (void)testLinkPacketMaxPacketLengthExceedediAP2Rule;
- (void)testLinkPacketNoSYNWithinTimeoutAfterRSTiAP2Rule;
- (void)testLinkPacketNumberOfOutstandingPacketsExceedediAP2Rule;
- (void)testLinkPacketRetransmittedBeforeRetransmissionTimeoutiAP2Rule;
- (void)testLinkPacketSentToUnsupportedDeviceiAP2Rule;
- (void)testLinkPacketSequenceNumberDuplicatediAP2Rule;
- (void)testLinkPacketiAP1CommunicationNotInitiatediAP2Rule;
- (void)testLinkPacketiAP2DetectSequenceNotSentiAP2Rule;
@end

@protocol DeviceNotificationsRuleTests
@required
- (void)testDeviceNotificationsOptionalMessagesNotSeenCAiAP2Rule;
@end

@protocol MessageBasediAP2RuleTests
@required
- (void)testAppleDeviceMessageContainsUnknownParametersiAP2Rule;
- (void)testIdentificationInformationFirmwareVersionEmptyiAP2Rule;
- (void)testIdentificationInformationHardwareVersionEmptyiAP2Rule;
- (void)testIdentificationInformationManufacturerEmptyiAP2Rule;
- (void)testIdentificationInformationModelIdentifierEmptyiAP2Rule;
- (void)testIdentificationInformationNameEmptyiAP2Rule;
- (void)testIdentificationInformationSerialNumberEmptyiAP2Rule;
- (void)testMessageContainsUnknownParametersiAP2Rule;
@end

