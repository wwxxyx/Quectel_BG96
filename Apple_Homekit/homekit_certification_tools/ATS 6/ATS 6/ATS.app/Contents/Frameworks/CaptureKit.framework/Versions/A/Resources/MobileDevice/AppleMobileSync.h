//
//  AppleMobileSync.h
//  AppleMobileSync
//
//  Created by Keebler elves and mirth makers on 11/01/06.
//  Copyright 2006 Apple Computer, Inc. All rights reserved.
//

#ifndef APPLE_MOBILE_SYNC_H
#define	APPLE_MOBILE_SYNC_H

#include <CoreFoundation/CoreFoundation.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define kAMSBookmarksDataClassName CFSTR("com.apple.Bookmarks")
#define kAMSCalendarsDataClassName CFSTR("com.apple.Calendars")
#define kAMSContactsDataClassName CFSTR("com.apple.Contacts")
#define kAMSMailAccountsDataClassName CFSTR("com.apple.MailAccounts")
#define kAMSNotesDataClassName CFSTR("com.apple.Notes")

#define kAMSSyncAvailabilityChangedNotification CFSTR("com.apple.syncservices.ISyncAvailabilityChangedNotification")
    
#define kAMSSyncPlanChangedNotification CFSTR("com.apple.syncservices.ISyncPlanChangedNotification")
    // Keys in userInfo
    #define kAMSSyncPlanStatus CFSTR("ISyncPlanStatus")
        // Plan status values
        #define kAMSSyncPlanStatusFinished CFSTR("Finished")
    #define kAMSSyncPlanEntityNames CFSTR("ISyncPlanEntityNames")    
        // Entity names
        #define kAMSEntityNameCalendar CFSTR("com.apple.calendars.Calendar")
        #define kAMSEntityNameGroup CFSTR("com.apple.contacts.Group")
        #define kAMSEntityNameMailAccount CFSTR("com.apple.mail.Account")
        #define kAMSEntityNameNoteAccount CFSTR("com.apple.notes.Account")

typedef enum AMSStatus
{
    // XXX: Define status codes
    AMSStatus_OK = 0,
    AMSStatus_Unsupported = 1,
    AMSStatus_ComponentsMissing = 2,
    AMSStatus_GenericError /* placeholder */
} AMSStatus;

#pragma mark -
#pragma mark General methods


/* Need to call this first before calling anything else. */
extern AMSStatus AMSInitialize (CFStringRef path);
extern AMSStatus AMSCleanup (void);

extern AMSStatus AMSIsSyncServicesEnabled (Boolean *enabled);
extern AMSStatus AMSEnableSyncServices (Boolean enable);

/* Get supported data class names (since they can be different on Tiger and Leopard - i.e. only Leopard
   has the data class for Notes syncing. Data class names are of the form "com.apple.Contacts". The
   caller is responsible for releasing the dataClassNames array.
*/
extern AMSStatus AMSGetSupportedDataClassNames (CFArrayRef /* CFStringRef data class names */ *dataClassNames);

/* Gets the last date the device synced the specified data class. If the device has never synced the
   data class, or the sync history has been reset, this returns NULL. If a non-NULL date is returned,
   it is the caller's responsibility to free it.
*/
extern AMSStatus AMSGetLastSyncDateForDataClass (CFStringRef targetIdentifier, CFStringRef dataClassName, CFDateRef *date);

#if defined(__WIN32__)
/* Gets DataClass Application Provider info.
 
Returned dictionary maps DataClass name to a dictionary of potential application providers. Entries for all supported dataclasses are always returned.  The values are dictionaries, keyed off of client identifiers, and have two keys:
kAMSDataClassDataSourceIsDesignated (CFNumberRef)
kAMSDataClassAvailableDataSourceDisplayName (CFStringRef)
 
By way of example:
com.apple.Contacts ->
  com.apple.Outlook ->
    kAMSDataClassDataSourceIsDesignated -> 1
    kAMSDataClassAvailableDataSourceDisplayName -> Outlook
    kAMSDataClassDataSourceHasContainers -> 0
  com.apple.WAB ->
    kAMSDataClassDataSourceIsDesignated -> 0
    kAMSDataClassAvailableDataSourceDisplayName -> Address Book
    kAMSDataClassDataSourceHasContainers -> 1

The caller is responsible for releasing the returned appProviderInfo

 */
#define kAMSDataClassDataSourceIsDesignated CFSTR("kAMSDataClassDataSourceIsDesignated")
#define kAMSDataClassAvailableDataSourceDisplayName CFSTR("kAMSDataClassAvailableDataSourceDisplayName")
#define kAMSDataClassDataSourceHasContainers CFSTR("kAMSDataClassDataSourceHasContainers")
    
extern AMSStatus AMSGetApplicationProviderInfo (CFDictionaryRef /* CFDictionaryRef */ *appProviderInfo);

extern AMSStatus AMSSetDesignatedProviderForDataClassName (CFStringRef appProviderIdentifier, CFStringRef dataClassName);

/*
 On Windows, there is much less support for applications doing trickle syncs.  This call is intended to let iTunes force
 data provider applications to sync, thus updating the collections info.
 
 If the designated app provider for this dataclass supports collections, the Sync Server will kick off an asynchronous
 sync plan with the sync client acting as the data class provider.

 Note that the collection gathering is done asynchronously, so when this method returns iTunes should not assume the
 collection info has been updated, this call just starts a chain of events that culminates with an eventual call to 
 AMSNoteSyncPlanEndedCallback.
 
 As a temporary measure, while the app provider syncs, iTunes should use the value of AMSGetCollectionsForDataClassName()
 to populate the UI. When iTunes receives the AMSNoteSyncPlanEndedCallback, it can call AMSGetCollectionsForDataClassName()
 again, to get the most recent data.  
 
 As a rule of thumb, don't call this method unless you intend to update the UI.  For example: good times to call this would be
 when iTunes launches, and when iTunes regains focus while the collections tab is showing.
 */    
extern AMSStatus AMSRefreshCollectionsForDataClassName (CFStringRef dataClassName);
    
/* This will retrieve the username from the AOSUtils if it exists.  *username can be NULL on success. */
extern AMSStatus AMSGetAOSUsername (CFStringRef *username);

#endif
    
#define kAMSDataClassName          CFSTR("kAMSDataClassName")
#define kAMSDataClassEnabled       CFSTR("kAMSDataClassEnabled")
#define kAMSDataClassReset         CFSTR("kAMSDataClassReset")

/* Gets DataClass info.

Returned dictionary maps DataClass name to properties. Entries for all supported dataclasses are always returned.
Property dictionary contains keys for:
        kAMSDataClassName (CFStringRef)
        kAMSEnabled (CFBooleanRef)
        kAMSReset (CFBooleanRef)
The caller is responsible for releasing the dataClasses array.
*/
extern AMSStatus AMSGetDataClassInfoForTarget (CFStringRef targetIdentifier,  CFArrayRef /* CFDictionary info */ *dataClasses);

/* Enable/disable/reset/etc. the DataClass names for syncing. The caller does not have to pass in entries
   for all data classes, any data classes that aren't in the array are considered unchanged. */
extern AMSStatus AMSSetDataClassInfoForTarget (CFStringRef targetIdentifier, CFArrayRef /* CFDictionary info */ dataClasses);

/* Registers Sync Services client and sets the display name. */
extern AMSStatus AMSRegisterClientWithTargetIdentifierAndDisplayName (CFStringRef targetIdentifier, CFStringRef displayName);

/* Unregister the client with Sync Services. */
extern AMSStatus AMSUnregisterTarget (CFStringRef targetIdentifier);

/* Gets the display name for the Sync Services client. 
   The caller is responsible for releasing the returned clientIdentifier and displayName. */
extern AMSStatus AMSGetClientIdentifierAndDisplayNameForTarget (CFStringRef targetIdentifier, CFStringRef *clientIdentifier, CFStringRef *displayName);

/* Sync Progress Callback status */
#define kAMSProgressVersionMismatchComputerGreater CFSTR("ProgressVersionMismatchComputerGreater")
#define kAMSProgressVersionMismatchDeviceGreater CFSTR("ProgressVersionMismatchDeviceGreater")
#define kAMSProgressSyncStarting CFSTR("ProgressSyncStarting")
#define kAMSProgressSyncCompleted CFSTR("ProgressSyncCompleted")
#define kAMSProgressSyncPlanFinished CFSTR("ProgressSyncPlanFinished")
#define kAMSProgressSyncPlanCancelled CFSTR("ProgressSyncPlanCancelled")
#define kAMSProgressSyncPlanPending CFSTR("ProgressSyncPlanPending")
#define kAMSProgressSyncPlanCreated CFSTR("ProgressSyncPlanCreated")
#define kAMSProgressSyncPlanPushing CFSTR("ProgressSyncPlanPushing")
#define kAMSProgressSyncPlanMingling CFSTR("ProgressSyncPlanMingling")
#define kAMSProgressSyncPlanPulling CFSTR("ProgressSyncPlanPulling")

/* Error codes for errorCode value in SyncProgressCallback */
#define kMobileSyncNoError                           (0)
#define kMobileSyncGenericError                      (-1)
#define kMobileSyncSessionFailure                    (-2)
#define kMobileSyncDeviceOutOfDate                   (-3)
#define kMobileSyncNegotiationFailed                 (-4)
#define kMobileSyncDeviceRefusedToSyncDataclass      (-5)
#define kMobileSyncMingleFailure                     (-6)
#define kMobileSyncPushFailure                       (-7)
#define kMobileSyncRemapFailure                      (-8)
#define kMobileSyncDeviceCancelledSync               (-9)
#define kMobileSyncDeviceDisconnected                (-10)
#define kMobileSyncErrorCodeFailedToConnectToDevice  (-11)
#define kMobileSyncErrorCodeFailedToAcceptFromDevice (-12)
#define kMobileSyncCommitFailure                     (-13)
#define kMobileSyncDeviceProtocolOutOfDate           (-14)
#define kMobileSyncHostProtocolOutOfDate             (-15)
#define kMobileSyncAlreadyRunning                    (-16)
#define kMobileSyncSyncServerFailed                  (-17)
#define kMobileSyncSyncBlocked                       (-18)
#define kMobileSyncBackupSessionFailure              (-19)
#define kMobileSyncBackupVersionError                (-20)
#define kMobileSyncBackupCancelled                   (-21) /* Not really an error, but cleaner to define here. */
#define kMobileSyncRestoreCancelled                  (-21) /* Synonym for restore/migrate. */
#define kMobileSyncCRCDisabled                       (-22)
#define kMobileSyncCRCNotPaired                      (-23)
#define kMobileSyncCRCCouldntConnect                 (-24)
#define kMobileSyncRestoreRefused                    (-25)
#define kMobileSyncRestoreTimeout                    (-26)
#define kMobileSyncBackupRefused                     (-27)
#define kMobileSyncBackupNoDestination               (-28)
#define kMobileSyncUnknownMessage                    (-29)
#define kMobileSyncBackupCouldNotSave                (-30)
#define kMobileSyncBackupInvalidPassword             (-31)
#define kMobileSyncBackupDoesntExist                 (-32)
#define kMobileSyncRestorePartiallyFailed            (-33)
#define kMobileSyncDataClassDisabled                 (-34)
#define kMobileSyncDeviceLocked                      (-35)
#define kMobileSyncNoSpace                           (-36)
#define kMobileSyncRestoreFindMyiPhoneEnabled        (-37)
#define kMobileSyncBackupDisabledDuringSetup         (-38)
#define kMobileSyncNotAllowedByMDM                   (-39)

/* Sync Progress Callback */
typedef void (*AMSSyncProgressCallback) (CFStringRef targetIdentifier, CFStringRef clientIdentifier, CFStringRef status, CFStringRef dataClassName, int errorCode, void *context);

/* Begins a sync with the specified device. This call is syncronous and returns when the sync completes or fails. 
   Progress Callbacks are called during the sync in the same thread calling this function. Earlier versions of sync
   supported syncing on the network, and syncing with devices found directly from USB Mux. The shipping product will
   only sync devices found via Lockdown, and should specify kAMSLockdownDeviceIdentifier for the deviceIdentifier. To
   sync a network device, specify kAMSNetworkDeviceIdentifier. To sync a device found directly from USB Mux, specify
   the device_id as indicated from the USB Mux listener callback (create a string from the int value). */
#define kAMSLockdownDeviceIdentifier   -1
#define kAMSNetworkDeviceIdentifier    0
/* Syncs all enabled data classes. */
extern AMSStatus AMSBeginSync (CFStringRef deviceIdentifier, CFStringRef targetIdentifier, AMSSyncProgressCallback callback, void *context);
/* More specific variant of the above. Only syncs indicated data classes, with the option to reset them as well. 
   After the sync, the reset indication is cleared, regardless of success. */
extern AMSStatus AMSBeginSyncForDataClasses (CFStringRef deviceIdentifier, CFStringRef targetIdentifier, CFArrayRef /* CFStringRef data class names */ dataClassNames, Boolean reset, AMSSyncProgressCallback callback, void *context);

/* Clears all data on the device for the specified data classes */
extern AMSStatus AMSClearDataClasses (CFStringRef deviceIdentifier, CFStringRef targetIdentifier, CFArrayRef dataClassNames, AMSSyncProgressCallback callback, void *context);

    
/* Cancels any existing syncs containing the specified device */
extern AMSStatus AMSCancelSync (CFStringRef targetIdentifier);

/* Reset sync data: Remove all records from the truth, and set all clients to refresh all sync states. */
extern AMSStatus AMSResetSyncData ();

#if defined(__WIN32__)
/* Run Sync Diagnostics for all enabled data classes. The caller must specify the path where the results of the diagnostics need to be stored*/
extern AMSStatus AMSRunSyncDiagnostics (CFStringRef deviceIdentifier, CFStringRef targetIdentifiers, CFStringRef resultsPath);

/* Cancels any existing syncs containing the specified device. Currently unimplemented. */
extern AMSStatus AMSCancelSyncDiagnostics (CFStringRef targetIdentifier);
    
    
/* The user can choose to map their default Outlook calendar (called "Calendar"), to a specific calendar on MobileMe or their iPhone ("Home" or "Work").
   A NULL setting indicates unset.
   Using a name that is an invalid calendar will effectively work like the value being unset.
   Setting the name to NULL will make it unset.
     
   Note: These APIs should only be called in combination with an initial or replace (reset) sync of
   the iPhone or MobileMe. That initial sync or reset must occur after the setter function is called.
   The getter may be called at any time. */
    
extern AMSStatus AMSSetDefaultOutlookCalendarMapping(CFStringRef calendarName);
extern AMSStatus AMSGetDefaultOutlookCalendarMapping(CFStringRef *calendarName);
    
#endif
    
#pragma mark -
#pragma mark Backup

// lockdown keys used to indicate whether a device will be encrypting backups or not
#define kBackupLockdownDomain       CFSTR("com.apple.mobile.backup")
    #define kBackupsWillEncryptKey      CFSTR("WillEncrypt")  // kCFBooleanRef
    #define kCloudBackupEnabledKey      CFSTR("CloudBackupEnabled")  // kCFBooleanRef

#define kAMSBackupPathKey CFSTR("Backup Path")
#define kAMSLastBackupDateKey CFSTR("Last Backup Date")
/* kAMSBackupTargetIdentifierKey refers to the identifier of the device used
   to make the backup which is now in the restore source list. This is just
   used for tracking purposes to identify where a backup originally came from. 
   This was an unfortunate choice for the name, and should have been something
   like kAMSBackupOriginalSourceIdentifierKey. Multiple restore sources can
   have the same value for kAMSBackupTargetIdentifierKey (see below). */
#define kAMSBackupTargetIdentifierKey CFSTR("Target Identifier")
/* kAMSBackupSourceIdentifierKey refers to the value to be used for the 
   sourceTargetIdentifier parameter to AMSRestore, it unambiguously identifies
   the backup to be used. This will be the same value as kAMSBackupTargetIdentifierKey
   when a backup is first done, but if the application wants to preserve a backup,
   it can copy the directory the backup lives in and change the directory name of 
   the copy to whatever it wants, which will be reflected as the kAMSBackupSourceIdentifierKey
   in the list of sources returned from AMSGetSourcesForRestore. */
#define kAMSBackupSourceIdentifierKey CFSTR("Source Identifier")
#define kAMSBackupTargetTypeKey CFSTR("Target Type")
    #define kAMSBackupTargetTypeDevice CFSTR("Device")
#define kAMSBackupProductVersionKey CFSTR("Product Version")
#define kAMSBackupProductTypeKey CFSTR("Product Type")
#define kAMSBackupDisplayNameKey CFSTR("Display Name")
#define kAMSBackupLastBackupSuccessKey CFSTR("Backup Success")
#define kAMSBackupIsEncryptedKey CFSTR("Is Encrypted")
#define kAMSBackupContainsAppDataKey CFSTR("Contains Application Data")
/* kAMSRestoreCompatibilityKey refers to the compatibility of a backup for restores. Not all the data in a backup may
   be compatible with the target device, and wont be restored. */
#define kAMSRestoreCompatibilityKey CFSTR("Restore Compatibility")
    #define kAMSRestoreNotCompatible CFSTR("Not Compatible")
    #define kAMSRestoreFullyCompatible CFSTR("Fully Compatible")
    #define kAMSRestoreAppDataOnlyCompatible CFSTR("Application Data Only Compatible")

/* Get list of available sources for restore. Each entry is a dictionary with whatever was
   passed into the deviceInfo parameter to AMSBackup as well as the date of the last backup, 
   which is a CFDateRef with the key kAMSLastBackupDateKey, and the device's unique identifier
   which is a CFStringRef with the key kAMSBackupTargetIdentifierKey, and is passed into 
   AMSBackup/AMSRestore/etc. as the targetIdentifier argument

   The caller is responsible for releasing the sourceInfos array.
*/
extern AMSStatus AMSGetSourcesForRestore (CFArrayRef *sourceInfos);

/* Same as AMSGetSourcesForRestore, except a query dictionary is passed for finer grained control of what 
    backups will match. Currently, two keys are supported:
     kAMSBackupProductVersionKey - Versions are forwards compatible, but not backwards compatible (1.1.2 will
        return all backups with versions 1.0-1.1.2, but not 1.1.3 or higher)
     kAMSBackupProductTypeKey - Matches the product type (iPod or iPhone). You can also specify a version for
        even finer control (iPhone1,1)
    If a value is nil it will match any backup.
*/
extern AMSStatus AMSCopySourcesForRestoreCompatibleWith (CFDictionaryRef query, CFArrayRef *sourceInfos);

/* Callback to report status while doing backup/restore. Percent does double duty reporting status,
   (percent complete), and any errors (which will be a negative number). For percent, 0 is starting
   and 100 is complete, with value in between during the operation. */
typedef void (*AMSBackupRestoreProgressCallback) (CFStringRef targetIdentifier, int percent, void *context);

// AMSBackupWithOptions options dictionary keys
#define kAMSBackupForceFullBackup       CFSTR("ForceFullBackup")

/* Does a backup of the device's non-sync user data. */
extern AMSStatus AMSBackupWithOptions (CFStringRef deviceIdentifier, CFStringRef targetIdentifier, CFDictionaryRef deviceInfo, CFDictionaryRef options, AMSBackupRestoreProgressCallback callback, void *context);
    
// AMSRestoreWithApplications options dictionary keys
#define kAMSRestorePreserveSettings     CFSTR("RestorePreserveSettings") // Defaults to NO
#define kAMSRestorePreserveCameraRoll   CFSTR("RestorePreserveCameraRoll") // Defaults to NO
#define kAMSRestoreShouldReboot         CFSTR("RestoreShouldReboot") // Defaults to YES
#define kAMSRestoreDontCopyBackup       CFSTR("RestoreDontCopyBackup")
#define kAMSBackupPassword              CFSTR("Password") 
#define kAMSRestoreSystemFiles          CFSTR("RestoreSystemFiles") // Defaults to YES
#define kAMSPerformSplitRestore         CFSTR("ShouldPerformSplitRestore") // Defaults to NO
#define kAMSRemoveItemsNotRestored      CFSTR("RemoveItemsNotRestored") // Defaults to YES
#define kAMSBackupRootDirectory         CFSTR("RootDirectory")


/* Does a restore of the device's non-sync user data. Restores data to the device specified with targetIdentifier
   from data previously backed up from the device specified by sourceTargetIdentifier. A migrate is done by specifying
   a different target id for the source of the data to be restored. To restore a device from that device's last backup,
   use the same value for targetIdentifier and sourceTargetIdentifier. Yeah I know "sourceTargetIdentifier" is an oxymoron. 
   Use appIdentifiers to filter the 3rd party applications restored to the device (only on devices that suppport 3rd party applications):
    If appIdentifiers is NULL, all 3rd party applications will be restored to the device
    If appIdentifiers is an empty array, no 3rd party applications will be restored.
    If appIdentifiers contains application identifiers, only applications matching those identifiers will be restored */
extern AMSStatus AMSRestoreWithApplications (CFStringRef deviceIdentifier, CFStringRef targetIdentifier, CFStringRef sourceTargetIdentifier, CFArrayRef appIdentifiers, CFDictionaryRef restoreOptions, AMSBackupRestoreProgressCallback callback, void *context);

// Returns the keys and values from the Info.plist. Also sets kAMSBackupIsEncryptedKey to the encryption state of the backup.
extern AMSStatus AMSGetBackupInfo (CFStringRef targetIdentifier, CFDictionaryRef *info, int *error);

// Set, change, or clear the password for a backup.
// To enable encryption on an unencrypted backup, pass nil for oldPassword and the new password for newPassword.
// To remove encryption on an encrypted backup, pass the old password for oldPassword and nil for newPassword. 
//  Encryption will only be removed if the old password is valid.
// WARNING: In both of the above cases, the inner manifest will be cleared, forcing a fresh backup. 
//  Do not call this function in either of these cases if you're not planning on running a backup immediately afterward.
//
// If you just want to change the password for an already encrypted backup the inner manifest will be preserved.
// Note that since all encryption is done on the device, the device must be connected in order to change the backup password.
extern AMSStatus AMSChangeBackupPassword (CFStringRef targetIdentifier, CFStringRef oldPassword, CFStringRef newPassword, int *error);

/* Cancels any existing backup/restore containing the specified device */
extern AMSStatus AMSCancelBackupRestore (CFStringRef targetIdentifier);
    
/* Toggles cloud backup for the primary AppleAccount on the device */
extern AMSStatus AMSEnableCloudBackup(CFStringRef targetIdentifier, CFNumberRef enable, int *error);

/* Returns a dictionary mapping application ids to the keys from their Info.plist */
extern AMSStatus AMSCopyApplicationListFromBackup (CFStringRef targetIdentifier, CFDictionaryRef *appInfo);
    
/* Sets a keychain password for a backup */
extern AMSStatus AMSSetBackupPasswordInKeychain (CFStringRef targetIdentifier, CFStringRef password, Boolean *success);
    
/* Retrieves the keychain password for a backup */
extern AMSStatus AMSGetBackupPasswordFromKeychainForTarget(CFStringRef targetIdentifier, CFStringRef *password);
    
/* Move a backup to the side to archive it. Giving the directory and display name a suffix of the current data. */
extern AMSStatus AMSArchiveBackup(CFStringRef targetIdentifier);

#pragma mark -
#pragma mark Crash Logs

/* This progress callback is made while copying or submitting crash logs. The progress dictionary contains the keys 
   defined below. */
typedef void (*AMSCrashReportCopyProgressCallback) (CFStringRef targetIdentifier, CFDictionaryRef progress, void *context);

/* Crash report copy progress callback dictionary keys */
#define kAMSCrashLogCopyState               CFSTR("CrashLogCopyState") // NSNumber * (AMSCrashReportCopyState)
#define kAMSCrashLogCopyStateString         CFSTR("CrashLogCopyStateString")
#define kAMSTotalCrashLogsKey               CFSTR("TotalCrashLogs")
#define kAMSCurrentCrashLogNumberKey        CFSTR("CurrentCrashLogNumber")
#define kAMSTotalCrashLogsToSubmitKey       CFSTR("TotalCrashLogsToSubmit")
#define kAMSSubmittedCrashLogNumberKey      CFSTR("SubmittedCrashLogs")
#define kAMSCrashLogPath                    CFSTR("CrashLogPath")

typedef enum {
    AMSCrashCopyStateDisconnected = 1,
    AMSCrashCopyStateConnecting,
    AMSCrashCopyStateConnected,
    AMSCrashCopyStateRequestingSysInfo,
    AMSCrashCopyStateCopying,
    AMSCrashCopyStateSubmitting
} AMSCrashReportCopyState;

/* Connect to a crash report copy target. If the device is not paired, this returns a kMobileSyncCRCNotPaired error */
extern AMSStatus AMSConnectToCrashReportCopyTarget (CFStringRef targetIdentifier, int *errorCode);
/* Disconnect from the crash report copy target. */
extern AMSStatus AMSDisconnectFromCrashReportCopyTarget (CFStringRef targetIdentifier, int *errorCode);

/* Get and set the crash report copy submission preferences. Crash report copy doesn't currently enforce these on
   its own. It is up to the caller to decide whether crash reports should be copied and submitted.
   shouldPrompt is the "don't ask me again" setting.
   shouldSubmit is the "submit crash reports to Apple" setting 
   These preferences are set per device. */
AMSStatus AMSGetCrashReportCopyPreferencesForTarget (CFStringRef targetIdentifier, Boolean *shouldPrompt, Boolean *shouldSubmit, int *errorCode);
extern AMSStatus AMSSetCrashReportCopyPreferencesForTarget (CFStringRef targetIdentifier, Boolean shouldPrompt, Boolean shouldSubmit, int *errorCode);

/* Asks the device how many crash reports it has available to be copied. If crashReports is set to 0 there is 
   no need to call AMSCopyCrashReportsFromTarget, but you probably still want to call AMSGetNumberOfCrashReportsToSubmit. 
   On an error, crashReports is set to -1 and errorCode is set. */
extern AMSStatus AMSGetNumberOfCrashReportsToCopy (CFStringRef targetIdentifier, int *crashReports, int *errorCode);
/* Copies crash logs from the device to the desktop. Note that this does not submit the crash reports to Apple.
   It is ok to call this without first having called AMSGetNumberOfCrashReportsToCopy. */
extern AMSStatus AMSCopyCrashReportsFromTarget (CFStringRef targetIdentifier, AMSCrashReportCopyProgressCallback callback, int *errorCode, void *context);

/* Determines the number of crash reports to submit based on the number of unsubmitted crash reports on the desktop for
   the given target identifier */
extern AMSStatus AMSGetNumberOfCrashReportsToSubmit (CFStringRef targetIdentifier, int *crashReports, int *errorCode);
/* Submits the crash reports from the host to Apple. If the "do not submit" preference has been set, errorCode is set to
    kMobileSyncCRCDisabled. It is ok to call this without first having called AMSGetNumberOfCrashReportsToSubmit. */
extern AMSStatus AMSSubmitCrashReportsFromTarget (CFStringRef targetIdentifier, AMSCrashReportCopyProgressCallback callback, int *errorCode, void *context);

/* Submits a single restore log from a given location. The file is not deleted after it is submitted. 
   Any valid crashreporter keys passed in the options dictionary will be added to the problem report dictionary 
   that is submitted to the server.
   Use kAMSRestoreLogsTargetName as the target name for setting the submission preferences. */
#define kAMSRestoreLogsTargetName   CFSTR("RestoreLogs")
extern AMSStatus AMSSubmitRestoreLogFromPath (CFStringRef path, CFDictionaryRef options, Boolean *success, int *errorCode);

/* Cancels any existing crash report copy or submission for the specified device */
extern AMSStatus AMSCancelCrashReportCopy (CFStringRef targetIdentifier, int *errorCode);

/* Returns the local path where crash reports are stored for a given target. Since the crash reports are stored
   at a path using the device's name, not just the target identifier, the device needs to be plugged in to get
   this path. 
   It is possible that you could call this back and get a path that does not exist. The returned path is the 
   directory that would be created if any crash logs are copied off the device. */
AMSStatus AMSCopyCrashReportPath (CFStringRef targetIdentifier, CFStringRef *crashReportPath, int *errorCode);

#pragma mark -
#pragma mark All Collections


#define kAMSCollectionName        CFSTR("AMSCollectionName")
#define kAMSCollectionDisplayName CFSTR("AMSCollectionDisplayName")
#define kAMSCollectionReadOnly    CFSTR("AMSCollectionReadOnly")
#define kAMSCollectionFiltered    CFSTR("AMSCollectionFiltered")

/* Gets Collection info (i.e. Calendars, Address Book Groups, Mail Account).

   Returned dictionary maps Collection name to properties. Property dictionary contains keys for:
        kAMSCollectionName (CFStringRef)
        kAMSCollectionDisplayName (CFStringRef)
        kAMSCollectionReadOnly (CFBooleanRef)
        kAMSCollectionFiltered (CFBooleanRef)
   Not all collections support all properties, so some properties might not be present.

   If kAMSCollectionFiltered is not set for any collections, means filtering is turned off, as
   at least one collection must be filtered if filtering is on.
 
   Note that if no collections are present, collections will be NULL

   The caller is responsible for releasing the collections array.

*/
extern AMSStatus AMSGetCollectionsForDataClassName (CFStringRef targetIdentifier, CFStringRef dataClassName, CFArrayRef /* CFDictionary collection info */ *collections);

/* Sets collections to be filtered. Passing in NULL or an empty array turns filtering off for collections in the data class. */
extern AMSStatus AMSSetFilteredCollectionNamesForDataClassName (CFStringRef targetIdentifier, CFStringRef dataClassName, CFArrayRef /* CFStringRef Collection name */ collectionNames);

#pragma mark -
#pragma mark Calendars


/* Gets Calendar name used for new events/tasks. 
   The caller is responsible for releasing the calendarName. 
   If there are no calendars in the sync server this call returns "Home" */
extern AMSStatus AMSGetNewRecordCalendarName (CFStringRef targetIdentifier, CFStringRef *calendarName);

/* Sets Calendar name to put new events/tasks into. */
extern AMSStatus AMSSetNewRecordCalendarName (CFStringRef targetIdentifier, CFStringRef calendarName);

#define kAMSDayLimitUnlimited (0)

/* Gets the day limit for Events filtering. Returns kAMSDayLimitUnlimited if no day limit. */
extern AMSStatus AMSGetCalendarDayLimit (CFStringRef targetIdentifier, int *dayLimit);

/* Sets day limit for Events filtering. All events which end (recurrences included) before 
   the current date minus the number of days will be filtered off the device. If there is
   no day limit, returns kAMSDayLimitUnlimited. */
extern AMSStatus AMSSetCalendarDayLimit (CFStringRef targetIdentifier, int dayLimit);

#pragma mark -
#pragma mark Contacts

/* Gets Group name used for new Contacts. 
   The caller is responsible for releasing the groupName. 
   If there are no Groups in the sync server this call returns "Home" */
extern AMSStatus AMSGetNewRecordGroupName (CFStringRef targetIdentifier, CFStringRef *groupName);

/* Sets Group name to put new Contacts into. */
extern AMSStatus AMSSetNewRecordGroupName (CFStringRef targetIdentifier, CFStringRef groupName);

// Use general collection methods for Contacts to get/set filtered groups

#pragma mark -
#pragma mark Mail Accounts

// Use general collection methods for Mail Accounts to get/set filtered mail accounts

#if defined(__WIN32__)

/* Get / set the current DCA configuration info.  Note that this call, like the designated sync apps, is NOT
   device-specific. The caller is responsible for releasing the returned values for enabled and threshold. */
extern AMSStatus AMSGetDataChangeAlertInfo(CFNumberRef *enabled, CFNumberRef *threshold);
extern AMSStatus AMSSetDataChangeAlertInfo(CFNumberRef enabled, CFNumberRef threshold);

#define kAMSSyncPlanEndedDataClassNames CFSTR("AMSSyncPlanEndedDataClassNames")
#define kAMSSyncPlanEndedClientIdentifiers CFSTR("AMSSyncPlanEndedClientIdentifiers")
#define kAMSSyncPlanEndedDidFinish CFSTR("AMSSyncPlanEndedDidFinish")
    
/* This callback will be called when a sync plan ends.  This could mean that the plan finished or the sync plan 
   was cancelled.  The dictionary passed in has information about the sync plan as follows:
   kAMSSyncPlanEndedDataClassNames = CFArrayRef of CFStringRefs
   kAMSSyncPlanEndedClientIdentifiers = CFArrayRef of CFStringRefs
   kAMSSyncPlanEndedDidFinish = CFBooleanRef
*/
typedef void (*AMSNoteSyncPlanEndedCallback)(CFDictionaryRef planInformation, void *syncPlanEndedContext);
typedef Boolean (*AMSDisplayDataChangeAlertCallback)(CFDictionaryRef changeInformation);
typedef void (*AMSNoteConflictsChangedCallback)(void);
// dcaCallback and conflictsCallback are deprecated and ignored
extern void AMSRegisterCallbacks (AMSDisplayDataChangeAlertCallback dcaCallback, AMSNoteConflictsChangedCallback conflictsCallback, AMSNoteSyncPlanEndedCallback syncPlanEndedCallback, void *syncPlanEndedContext);
    
#endif //__WIN32__

#ifdef __cplusplus
}
#endif

#endif /* APPLE_MOBILE_SYNC_H */

