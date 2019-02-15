/*
 *  vim:ts=4:sts=4:expandtab:
 *
 *  MobileDeviceError.h
 *  MobileDevice
 *
 *  Created by Justin Ko on 4/3/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 */

#ifndef __MOBILEDEVICEERRORS_H__
#define __MOBILEDEVICEERRORS_H__

#include <CoreFoundation/CoreFoundation.h>
#include <MobileDevice/MobileDeviceBase.h>

/*!
 * @header MobileDeviceError.h
 *
 * Error codes returned my the MobileDevice.framework.
 */

#define AMDMakeError(err_num) ((0x3A << 26) | (err_num))

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * Error codes returned by MobileDevice.framework to its clients.
 * - AMDErrorString() needs to be kept in sync with this list.
 */
enum {

    kAMDSuccess = 0,
    kAMDUndefinedError = AMDMakeError(1),
    kAMDBadHeaderError = AMDMakeError(2),
    kAMDNoResourcesError = AMDMakeError(3),
    kAMDReadError = AMDMakeError(4),
    kAMDWriteError = AMDMakeError(5),
    kAMDUnknownPacketError = AMDMakeError(6),
    kAMDInvalidArgumentError = AMDMakeError(7),
    kAMDNotFoundError = AMDMakeError(8),
    kAMDIsDirectoryError = AMDMakeError(9),
    kAMDPermissionError = AMDMakeError(10),
    kAMDNotConnectedError = AMDMakeError(11),
    kAMDTimeOutError = AMDMakeError(12),
    kAMDOverrunError = AMDMakeError(13),
    kAMDEOFError = AMDMakeError(14),
    kAMDUnsupportedError = AMDMakeError(15),
    kAMDFileExistsError = AMDMakeError(16),
    kAMDBusyError = AMDMakeError(17),
    kAMDCryptoError = AMDMakeError(18),
    kAMDTooBigError = AMDMakeError(133),

    /*
     * Error codes received from Lockdownd over the connection. MobileDevice itself
     * should only use these to relay information about Lockdownd errors to clients.
     *
     * There should be an analogous AMDError for each kLDErrorXXX declared at the top
     * of MobileDevice.c.
     */
    kAMDInvalidResponseError = AMDMakeError(19),
    kAMDMissingKeyError = AMDMakeError(20),
    kAMDMissingValueError = AMDMakeError(21),
    kAMDGetProhibitedError = AMDMakeError(22),
    kAMDSetProhibitedError = AMDMakeError(23),
    kAMDRemoveProhibitedError = AMDMakeError(24),
    kAMDImmutableValueError = AMDMakeError(25),
    kAMDPasswordProtectedError = AMDMakeError(26),
    kAMDUserDeniedPairingError = AMDMakeError(149),
    kAMDPairingDialogResponsePendingError = AMDMakeError(150),
    kAMDMissingHostIDError = AMDMakeError(27),
    kAMDInvalidHostIDError = AMDMakeError(28),
    kAMDSessionActiveError = AMDMakeError(29),
    kAMDSessionInactiveError = AMDMakeError(30),
    kAMDMissingSessionIDError = AMDMakeError(31),
    kAMDInvalidSessionIDError = AMDMakeError(32),
    kAMDMissingServiceError = AMDMakeError(33),
    kAMDInvalidServiceError = AMDMakeError(34),
    kAMDServiceLimitError = AMDMakeError(91),
    kAMDCheckinSetupFailedError = AMDMakeError(94),
    kAMDInvalidCheckinError = AMDMakeError(35),
    kAMDCheckinTimeoutError = AMDMakeError(36),
    kAMDCheckinConnectionFailedError = AMDMakeError(95),
    kAMDCheckinReceiveFailedError = AMDMakeError(96),
    kAMDCheckinResponseFailedError = AMDMakeError(97),
    kAMDCheckinOutOfMemoryError = AMDMakeError(105),
    kAMDCheckinSendFailedError = AMDMakeError(98),
    kAMDMissingPairRecordError = AMDMakeError(37),
    kAMDInvalidPairRecordError = AMDMakeError(92),
    kAMDSavePairRecordFailedError = AMDMakeError(104),
    kAMDInvalidActivationRecordError = AMDMakeError(38),
    kAMDMissingActivationRecordError = AMDMakeError(39),
    kAMDServiceProhibitedError = AMDMakeError(93),
    kAMDEscrowLockedError = AMDMakeError(129),
    kAMDPairingProhibitedError = AMDMakeError(130),
    kAMDProhibitedBySupervision = AMDMakeError(131),
    kAMDFMiPProtectedError = AMDMakeError(153),
    kAMDMCProtected = AMDMakeError(154),
    kAMDMCChallengeRequired = AMDMakeError(155),
   /* End of Lockdown errors. */


    kAMDWrongDroidError = AMDMakeError(40),

    kAMDSUVerificationError = AMDMakeError(41),
    kAMDSUPatchError = AMDMakeError(42),
    kAMDSUFirmwareError = AMDMakeError(43),

    kAMDProvisioningProfileNotValid = AMDMakeError(44),

    kAMDSendMessageError = AMDMakeError(45),
    kAMDReceiveMessageError = AMDMakeError(46),
    kAMDMissingOptionsError = AMDMakeError(47),
    kAMDMissingImageTypeError = AMDMakeError(48),
    kAMDDigestFailedError = AMDMakeError(49),
    kAMDStartServiceError = AMDMakeError(50),
    kAMDInvalidDiskImageError = AMDMakeError(51),
    kAMDMissingDigestError = AMDMakeError(52),
    kAMDMuxError = AMDMakeError(53),


    /*
     * Errors defined by either the MobileInstallationProxy or MobileInstallation.framework.
     */
    kAMDApplicationAlreadyInstalledError = AMDMakeError(54), //kErrorApplicationAlreadyInstalled
    kAMDApplicationMoveFailedError = AMDMakeError(55), //kErrorApplicationMoveFailed
    kAMDApplicationSINFCaptureFailedError = AMDMakeError(56), //kErrorApplicationSINFCaptureFailed
    kAMDApplicationSandboxFailedError = AMDMakeError(57), //OBSOLETE
    kAMDApplicationVerificationFailedError = AMDMakeError(58), //kErrorApplicationVerificationFailed
    kAMDArchiveDestructionFailedError = AMDMakeError(59), //OBSOLETE
    kAMDBundleVerificationFailedError = AMDMakeError(60), //OBSOLETE
    kAMDCarrierBundleCopyFailedError = AMDMakeError(61), //kErrorCarrierBundleCopyFailed
    kAMDCarrierBundleDirectoryCreationFailedError = AMDMakeError(62), //kErrorCarrierBundleDirectoryCreationFailed
    kAMDCarrierBundleMissingSupportedSIMsError = AMDMakeError(63), //kErrorCarrierBundleMissingSupportedSIMs
    kAMDCommCenterNotificationFailedError = AMDMakeError(64), //kErrorCommCenterNotificationFailed
    kAMDContainerCreationFailedError = AMDMakeError(65), //kErrorContainerCreationFailed
    kAMDContainerP0wnFailedError = AMDMakeError(66), //kErrorContainerP0wnFailed
    kAMDContainerRemovalFailedError = AMDMakeError(67), //OBSOLETE
    kAMDEmbeddedProfileInstallFailedError = AMDMakeError(68), //OBSOLETE
    kAMDErrorError = AMDMakeError(69), //Huh?
    kAMDExecutableTwiddleFailedError = AMDMakeError(70), //kErrorExecutableTwiddleFailed
    kAMDExistenceCheckFailedError = AMDMakeError(71), //OBSOLETE
    kAMDInstallMapUpdateFailedError = AMDMakeError(72), //kErrorInstallMapUpdateFailed
    kAMDManifestCaptureFailedError = AMDMakeError(73), //kErrorManifestCaptureFailed
    kAMDMapGenerationFailedError = AMDMakeError(74), //OBSOLETE
    kAMDMissingBundleExecutableError = AMDMakeError(75), //kErrorMissingBundleExecutable
    kAMDMissingBundleIdentifierError = AMDMakeError(76), //kErrorMissingBundleIdentifier
    kAMDMissingBundlePathError = AMDMakeError(77), //kErrorMissingBundlePath
    kAMDMissingContainerError = AMDMakeError(78), //kErrorMissingContainer
    kAMDNotificationFailedError = AMDMakeError(79), //OBSOLETE
    kAMDPackageExtractionFailedError = AMDMakeError(80), //kErrorPackageExtractionFailed
    kAMDPackageInspectionFailedError = AMDMakeError(81), //kErrorPackageInspectionFailed
    kAMDPackageMoveFailedError = AMDMakeError(82), //OBSOLETE
    kAMDPathConversionFailedError = AMDMakeError(83), //kErrorPathConversionFailed
    kAMDRestoreContainerFailedError = AMDMakeError(84), //OBSOLETE
    kAMDSeatbeltProfileRemovalFailedError = AMDMakeError(85), //OBSOLETE
    kAMDStageCreationFailedError = AMDMakeError(86), //kErrorStageCreationFailed
    kAMDSymlinkFailedError = AMDMakeError(87), //kErrorSymlinkFailed
    kAMDiTunesArtworkCaptureFailedError = AMDMakeError(88), //kErroriTunesArtworkCaptureFailed
    kAMDiTunesMetadataCaptureFailedError = AMDMakeError(89), //kErroriTunesMetadataCaptureFailed
    kAMDAlreadyArchivedError = AMDMakeError(90), //kErrorAlreadyArchived
    kAMDUnknownCommandError = AMDMakeError(102), //kErrorUnknownCommand
    kAMDAPIInternalError = AMDMakeError(103), //kErrorAPIInternalError
    kAMDDeviceOSVersionTooLow = AMDMakeError(126), //kErrorDeviceOSVersionTooLow
    kAMDDeviceFamilyNotSupported = AMDMakeError(128), //kErrorDeviceNotSupported
    kAMDPackagePatchFailedError = AMDMakeError(134), //kErrorPackagePatchFailed
    kAMDIncorrectArchitectureError = AMDMakeError(135), //kErrorIncorrectArchitecture
    kAMDPluginCopyFailedError = AMDMakeError(136), //kErrorPluginCopyFailed
    kAMDBreadcrumbFailedError = AMDMakeError(137), //kErrorBreadcrumbFailed
    kAMDBreadcrumbUnlockError = AMDMakeError(138), //kErrorBreadcrumbUnlockFailed
    kAMDGeoJSONCaptureFailedError = AMDMakeError(139), //kErrorGeoJSONCaptureFailed
    kAMDNewsstandArtworkCaptureFailedError = AMDMakeError(140), //kErrorNewsstandArtworkCaptureFailed
    kAMDMissingCommandError = AMDMakeError(141), //kErrorMissingCommand
    kAMDNotEntitledError = AMDMakeError(142), //kErrorNotEntitled
    kAMDMissingPackagePathError = AMDMakeError(143), //kErrorMissingPackagePath
    kAMDMissingContainerPathError = AMDMakeError(144), //kErrorMissingContainerPath
    kAMDMissingApplicationIdentifierError = AMDMakeError(145), //kErrorMissingApplicationIdentifier
    kAMDMissingAttributeValueError = AMDMakeError(146), //kErrorMissingAttributeValue
    kAMDLookupFailedError = AMDMakeError(147), //kErrorLookupFailed
    kAMDDictCreationFailedError = AMDMakeError(148), //kErrorDictCreationFailed
    kAMDInstallProhibitedError = AMDMakeError(151), //kErrorInstallProhibited
    kAMDUninstallProhibitedError = AMDMakeError(152), //kErrorInstallProhibited
    kAMDMissingBundleVersionError = AMDMakeError(156), //kErrorMissingBundleVersion

    kAMDAppBlacklistedError = AMDMakeError(157), //kErrorAppBlacklisted
    kAMDAppexBundleIDNotPrefixedError = AMDMakeError(158), //kErrorAppexBundleIDNotPrefixed
    kAMDAppexBundleIllegalXPCServiceDictError = AMDMakeError(159), //kErrorAppexBundleIllegalXPCServiceDict
    kAMDAppexBundleMissingNSExtensionDictError = AMDMakeError(160), //kErrorAppexBundleMissingNSExtensionDict
    kAMDAppexBundleIllegalPackageTypeValueError = AMDMakeError(161), //kErrorAppexBundleIllegalPackageTypeValue
    kAMDAppexBundleMissingClassOrStoryboardError = AMDMakeError(162), //kErrorAppexBundleMissingClassOrStoryboard
    kAMDAppexBundleIllegalExtensionContextClassError = AMDMakeError(163), //kErrorAppexBundleIllegalExtensionContextClass
    kAMDAppexBundleIllegalExtensionContextHostClassError = AMDMakeError(164), //kErrorAppexBundleIllegalExtensionContextHostClass
    kAMDAppexBundleIllegalExtensionViewControllerHostClassError = AMDMakeError(165), //kErrorAppexBundleIllegalExtensionViewControllerHostClass
    kAMDAppexBundleMissingExtensionPointIdentifierStringError = AMDMakeError(166), //kErrorAppexBundleMissingExtensionPointIdentifierString
    kAMDAppexBundleIllegalExtensionPointIdentifierStringError = AMDMakeError(167), //kErrorAppexBundleIllegalExtensionPointIdentifierString
    kAMDAppexBundleIllegalExtensionAttributesDictError = AMDMakeError(168), //kErrorAppexBundleIllegalExtensionAttributesDict
    kAMDAppexBundleIllegalExtensionPointNameStringError = AMDMakeError(169), //kErrorAppexBundleIllegalExtensionPointNameString
    kAMDAppexBundleIllegalExtensionPointVersionStringError = AMDMakeError(170), //kErrorAppexBundleIllegalExtensionPointNameString
    kAMDMissingBundleNameStringError = AMDMakeError(171), //kErrorMissingBundleNameString
    kAMDMissingBundleDisplayNameStringError = AMDMakeError(172), //kErrorMissingBundleDisplayNameString
    kAMDIllegalBundleShortVersionStringError = AMDMakeError(173), //kErrorIllegalBundleShortVersionString
    kAMDIllegalXPCServiceRunLoopTypeError = AMDMakeError(174), //kErrorIllegalXPCServiceRunLoopType
    kAMDIllegalXPCServiceServiceTypeError = AMDMakeError(175), //kErrorIllegalXPCServiceServiceType
    kAMDDuplicateIdentifierError = AMDMakeError(176), //kErrorDuplicateIdentifier
    kAMDAppexBundleUnknownExtensionPointIdentifierStringError = AMDMakeError(177), //kErrorAppexBundleUnknownExtensionPointIdentifier
    kAMDMultipleFileProviderAppexBundlesError = AMDMakeError(178), //kErrorMultipleFileProviderAppexBundles
    
    /* End MobileInstallation Errors. */

    /* Mux errors. */
    kAMDMuxCreateListenerError = AMDMakeError(99),
    kAMDMuxGetListenerError = AMDMakeError(100),
    kAMDMuxConnectError = AMDMakeError(101),
    kAMDDeviceTooNewError = AMDMakeError(106),
    kAMDDeviceRefNoGood = AMDMakeError(107),
    kAMDDeviceDisconnectedError = AMDMakeError(132),

    /* Use this for the error conversion functions, not kAMDUndefinedError. */
    kAMDCannotTranslateError = AMDMakeError(108),

    /* Errors from the Mobile Image Mounter */
    kAMDMobileImageMounterMissingImageSignature = AMDMakeError(109),
    kAMDMobileImageMounterResponseCreationFailed = AMDMakeError(110),
    kAMDMobileImageMounterMissingImageType = AMDMakeError(111),
    kAMDMobileImageMounterMissingImagePath = AMDMakeError(112),
    kAMDMobileImageMounterImageMapLoadFailed = AMDMakeError(113),
    kAMDMobileImageMounterAlreadyMounted = AMDMakeError(114),
    kAMDMobileImageMounterImageMoveFailed = AMDMakeError(115),
    kAMDMobileImageMounterMountPathMissing = AMDMakeError(116),
    kAMDMobileImageMounterMountPathNotEmpty = AMDMakeError(117),
    kAMDMobileImageMounterImageMountFailed = AMDMakeError(118),
    kAMDMobileImageMounterTrustCacheLoadFailed = AMDMakeError(119),
    kAMDMobileImageMounterDigestFailed = AMDMakeError(120),
    kAMDMobileImageMounterDigestCreationFailed = AMDMakeError(121),
    kAMDMobileImageMounterImageVerificationFailed = AMDMakeError(122),
    kAMDMobileImageMounterImageInfoCreationFailed = AMDMakeError(123),
    kAMDMobileImageMounterImageMapStoreFailed = AMDMakeError(124),
    /* End of Mobile Image Mounter errors. */

    /* Bonjour errors. */
    kAMDBonjourSetupError = AMDMakeError(125),
    kAMDNoWifiSyncSupportError = AMDMakeError(127),

    /*
     * This should always be the last error. When adding new error code, steal whatever
     * value is currently being used by kAMDMaximumError, and then bump the value of
     * kAMDMaximumError by one.
     */
    kAMDMaximumError = AMDMakeError(179),
};

/*!
 * @function AMDCopyErrorText
 * @abstract Provides a presentable error text for the passed-in AMDError
 * @param error The AMDError for which to return a description.
 * @result A CFStringRef describing the error. Or NULL.
 * @discussion
 *
 * If the error does not appear to come from the MobileDevice system or any of its
 * subsystems, this function may return NULL.
 *
 * If no description is known, will just return the error in string form
 * (for example, kAMDSuccess would be a CFStringRef containing "kAMDSuccess").
 * Will be a sentence.
 */
CFStringRef
AMD_API
AMDCopyErrorText(
    AMDError error);

#if defined(__cplusplus)
}
#endif

#endif /* __MOBILEDEVICEERRORS_H__ */

