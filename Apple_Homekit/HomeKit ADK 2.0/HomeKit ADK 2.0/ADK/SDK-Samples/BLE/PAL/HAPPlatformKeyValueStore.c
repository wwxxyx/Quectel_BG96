//
// Package: HomeKit-ADK
// Version: 2.0 (16A62)
//
// Disclaimer: IMPORTANT: This Apple software is supplied to you, by Apple Inc. ("Apple"), in your
// capacity as a current, and in good standing, Licensee in the MFi Licensing Program. Use of this
// Apple software is governed by and subject to the terms and conditions of your MFi License,
// including, but not limited to, the restrictions specified in the provision entitled "Public
// Software", and is further subject to your agreement to the following additional terms, and your
// agreement that the use, installation, modification or redistribution of this Apple software
// constitutes acceptance of these additional terms. If you do not agree with these additional terms,
// you may not use, install, modify or redistribute this Apple software.
//
// Subject to all of these terms and in consideration of your agreement to abide by them, Apple grants
// you, for as long as you are a current and in good-standing MFi Licensee, a personal, non-exclusive
// license, under Apple's copyrights in this Apple software (the "Apple Software"), to use,
// reproduce, and modify the Apple Software in source form, and to use, reproduce, modify, and
// redistribute the Apple Software, with or without modifications, in binary form, in each of the
// foregoing cases to the extent necessary to develop and/or manufacture "Proposed Products" and
// "Licensed Products" in accordance with the terms of your MFi License. While you may not
// redistribute the Apple Software in source form, should you redistribute the Apple Software in binary
// form, you must retain this notice and the following text and disclaimers in all such redistributions
// of the Apple Software. Neither the name, trademarks, service marks, or logos of Apple Inc. may be
// used to endorse or promote products derived from the Apple Software without specific prior written
// permission from Apple. Except as expressly stated in this notice, no other rights or licenses,
// express or implied, are granted by Apple herein, including but not limited to any patent rights that
// may be infringed by your derivative works or by other works in which the Apple Software may be
// incorporated. Apple may terminate this license to the Apple Software by removing it from the list
// of Licensed Technology in the MFi License, or otherwise in accordance with the terms of such MFi License.
//
// Unless you explicitly state otherwise, if you provide any ideas, suggestions, recommendations, bug
// fixes or enhancements to Apple in connection with this software ("Feedback"), you hereby grant to
// Apple a non-exclusive, fully paid-up, perpetual, irrevocable, worldwide license to make, use,
// reproduce, incorporate, modify, display, perform, sell, make or have made derivative works of,
// distribute (directly or indirectly) and sublicense, such Feedback in connection with Apple products
// and services. Providing this Feedback is voluntary, but if you do provide Feedback to Apple, you
// acknowledge and agree that Apple may exercise the license granted above without the payment of
// royalties or further consideration to Participant.
//
// The Apple Software is provided by Apple on an "AS IS" basis. APPLE MAKES NO WARRANTIES, EXPRESS OR
// IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR
// IN COMBINATION WITH YOUR PRODUCTS.
//
// IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION
// AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
// (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (C) 2018 Apple Inc. All Rights Reserved.
//

#include "app_scheduler.h"
#include "fds.h"

#include "HAPPlatformKeyValueStore+Init.h"
#include "HAPPlatformKeyValueStore+SDKDomains.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "KeyValueStore"
};

/**
 * Type of asynchronous operation.
 *
 * Format in queue:
 * -   init: [ type ]
 * -  write: [ type ][ dom ][ key ][ #words (16bit) ][ 0 ][ 0 ][ 0 ][ #pad ][ buf ]
 * - remove: [ type ][ dom ][ key ]
 * -  purge: [ type ][ dom ]
 */
HAP_ENUM_BEGIN(uint8_t, KeyValueStoreOperation) {
    /**
     * Initialization in progress.
     */
    kKeyValueStoreOperation_Initialize,
    
    /**
     * GC in progress.
     */
    kKeyValueStoreOperation_CollectGarbage,
    
    /**
     * Write in progress.
     */
    kKeyValueStoreOperation_Write,
    
    /**
     * GC for write in progress.
     */
    kKeyValueStoreOperation_CollectGarbageForWrite,
    
    /**
     * Removal in progress.
     */
    kKeyValueStoreOperation_Remove,
    
    /**
     * Purge in progress.
     */
    kKeyValueStoreOperation_Purge
} HAP_ENUM_END(uint8_t, KeyValueStoreOperation);

/**
 * Computes the FDS file ID corresponding to a domain.
 *
 * @param      keyValueStore        Key-value store.
 * @param      domain               Domain.
 *
 * @return FDS file ID.
 */
static uint16_t GetFDSFileID(
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformKeyValueStoreDomain domain)
{
    return keyValueStore->_.baseFDSFileID + domain;
}

/**
 * Computes the FDS record ID corresponding to a key.
 *
 * @param      keyValueStore        Key-value store.
 * @param      key                  Platform key.
 *
 * @return FDS record ID.
 */
static uint16_t GetFDSRecordID(
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformKeyValueStoreKey key)
{
    return keyValueStore->_.baseFDSRecordID + key;
}

/**
 * Logs the current KVS queue utilization.
 *
 * @param      keyValueStore        Key-value store.
 * @param      operation            Operation that is in progress.
 */
static void LogQueueUtilization(
    HAPPlatformKeyValueStoreRef keyValueStore,
    const char *operation)
{
    HAPPrecondition(keyValueStore);
    HAPPrecondition(operation);
    
    HAPLogDebug(&logObject, "[%s] KVS queue utilization: %lu bytes (%u%%).",
        operation, (unsigned long) keyValueStore->numBytes,
        100 * keyValueStore->numBytes / keyValueStore->_.maxBytes);
    
    if (keyValueStore->numBytes > keyValueStore->peakNumBytes) {
        keyValueStore->peakNumBytes = keyValueStore->numBytes;
        HAPLogInfo(&logObject, "New peak KVS queue utilization: %lu bytes (%u%%).",
            (unsigned long) keyValueStore->peakNumBytes,
            100 * keyValueStore->peakNumBytes / keyValueStore->_.maxBytes);
    }
}

/**
 * Completes an asynchronous operation.
 *
 * @param      keyValueStore        Key-value store.
 * @param      numProcessedBytes    Number of bytes in the operation queue that have been processed.
 */
static void Complete(
    HAPPlatformKeyValueStoreRef keyValueStore,
    size_t numProcessedBytes)
{
    HAPAssert(keyValueStore->busy);
    HAPAssert(keyValueStore->numBytes >= numProcessedBytes);
    uint8_t *queueBytes = keyValueStore->_.bytes;
    HAPRawBufferCopyBytes(&queueBytes[0], &queueBytes[numProcessedBytes], keyValueStore->numBytes - numProcessedBytes);
    keyValueStore->numBytes -= numProcessedBytes;
    keyValueStore->busy = false;
}

/**
 * Advances the queue of active operations.
 *
 * @param      keyValueStore        Key-value store.
 */
static void Update(
    HAPPlatformKeyValueStoreRef keyValueStore)
{
    uint8_t *queueBytes = keyValueStore->_.bytes;
    
    for (;;) {
        if (!keyValueStore->numBytes) {
            return;
        }
        
        if (keyValueStore->busy) {
            return;
        }
        keyValueStore->busy = true;
        
        switch (queueBytes[0]) {
            case kKeyValueStoreOperation_Initialize: {
                HAPLogDebug(&logObject, "Initialize");
                HAPAssert(keyValueStore->numBytes >= 1);
                ret_code_t e = fds_init();
                if (e) {
                    HAPAssert(e == FDS_ERR_NO_PAGES);
                    HAPLogError(&logObject, "fds_init failed: %lu (Code may use too much flash).", (unsigned long) e);
                    HAPFatalError();
                }
            } return;
            case kKeyValueStoreOperation_CollectGarbage: {
                HAPLogDebug(&logObject, "CollectGarbage");
                HAPAssert(keyValueStore->numBytes >= 1);
                ret_code_t e = fds_gc();
                if (e) {
                    HAPAssert(e == FDS_ERR_NO_SPACE_IN_QUEUES);
                    HAPLogError(&logObject, "fds_gc failed: %lu.", (unsigned long) e);
                    HAPFatalError();
                }
            } return;
            case kKeyValueStoreOperation_Write:
            case kKeyValueStoreOperation_CollectGarbageForWrite: {
                HAPAssert(keyValueStore->numBytes >= 9);
                HAPPlatformKeyValueStoreDomain domain = (HAPPlatformKeyValueStoreDomain) queueBytes[1];
                HAPPlatformKeyValueStoreKey key = (HAPPlatformKeyValueStoreKey) queueBytes[2];
                HAPLogDebug(&logObject, "Write %02X.%02X", domain, key);
                uint16_t numWords = HAPReadLittleUInt16(&queueBytes[3]);
                HAPAssert(!queueBytes[5]);
                HAPAssert(!queueBytes[6]);
                HAPAssert(!queueBytes[7]);
                void *value = &queueBytes[8];
                HAPAssert(keyValueStore->numBytes >= 8 + 4 * (size_t) numWords);
                
                // Prepare record.
                uint16_t fdsFileID = GetFDSFileID(keyValueStore, domain);
                uint16_t fdsRecordID = GetFDSRecordID(keyValueStore, key);
                fds_record_t record = {
                    .file_id = fdsFileID,
                    .key = fdsRecordID,
                    .data = {
                        .p_data = value,
                        .length_words = numWords
                    }
                };
                
                // Find existing record.
                fds_record_desc_t desc;
                ret_code_t e = fds_record_find(fdsFileID, fdsRecordID, &desc, &(fds_find_token_t) {
                    .p_addr = NULL,
                    .page = 0
                });
                
                // Write record.
                if (e) {
                    HAPAssert(e == FDS_ERR_NOT_FOUND);
                    e = fds_record_write(&desc, &record);
                } else {
                    e = fds_record_update(&desc, &record);
                }
                if (e == FDS_ERR_NO_SPACE_IN_FLASH) {
                    // Assumption: This works after garbage collection.
                    HAPAssert(queueBytes[0] == kKeyValueStoreOperation_Write);
                    HAPLogInfo(&logObject, "FDS_ERR_NO_SPACE_IN_FLASH: Issuing garbage collection.");
                    
                    // Get FDS statistics.
                    fds_stat_t statistics;
                    e = fds_stat(&statistics);
                    if (e) {
                        HAPLogError(&logObject, "fds_stat failed: %lu.", (unsigned long) e);
                        HAPFatalError();
                    }
                    
                    // Assumption: There is enough flash available.
                    if (statistics.freeable_words < numWords) {
                        HAPLogError(&logObject, "Not enough space in flash (available: %u words, required: %u words).",
                            statistics.freeable_words, numWords);
                        HAPFatalError();
                    }
                    
                    // Start GC.
                    queueBytes[0] = kKeyValueStoreOperation_CollectGarbageForWrite;
                    e = fds_gc();
                    if (e) {
                        HAPAssert(e == FDS_ERR_NO_SPACE_IN_QUEUES);
                        HAPLogError(&logObject, "fds_gc failed: %lu.", (unsigned long) e);
                        HAPFatalError();
                    }
                } else if (e) {
                    HAPAssert(e == FDS_ERR_RECORD_TOO_LARGE || e == FDS_ERR_NO_SPACE_IN_QUEUES);
                    HAPLogError(&logObject,
                        "fds_record_write/update %02X.%02X failed: %lu.", domain, key, (unsigned long) e);
                    HAPFatalError();
                }
            } return;
            case kKeyValueStoreOperation_Remove: {
                HAPAssert(keyValueStore->numBytes >= 3);
                HAPPlatformKeyValueStoreDomain domain = (HAPPlatformKeyValueStoreDomain) queueBytes[1];
                HAPPlatformKeyValueStoreKey key = (HAPPlatformKeyValueStoreKey) queueBytes[2];
                HAPLogDebug(&logObject, "Remove %02X.%02X", domain, key);
                
                // Find existing record.
                uint16_t fdsFileID = GetFDSFileID(keyValueStore, domain);
                uint16_t fdsRecordID = GetFDSRecordID(keyValueStore, key);
                fds_record_desc_t desc;
                ret_code_t e = fds_record_find(fdsFileID, fdsRecordID, &desc, &(fds_find_token_t) {
                    .p_addr = NULL,
                    .page = 0
                });
                if (e) {
                    HAPAssert(e == FDS_ERR_NOT_FOUND);
                    // Already deleted. Nothing to do.
                    Complete(keyValueStore, 3);
                    continue;
                }
                
                // Delete record.
                HAPAssert(!keyValueStore->isDeletingRecord);
                keyValueStore->modifiedRecordID = desc.record_id;
                keyValueStore->isDeletingRecord = true;
                e = fds_record_delete(&desc);
                if (e) {
                    HAPAssert(e == FDS_ERR_NO_SPACE_IN_QUEUES);
                    HAPLogError(&logObject,
                        "fds_record_delete %02X.%02X failed: %lu.", domain, key, (unsigned long) e);
                    HAPFatalError();
                }
            } return;
            case kKeyValueStoreOperation_Purge: {
                HAPAssert(keyValueStore->numBytes >= 2);
                HAPPlatformKeyValueStoreDomain domain = (HAPPlatformKeyValueStoreDomain) queueBytes[1];
                HAPLogDebug(&logObject, "Purge %02X", domain);
                
                ret_code_t e = fds_file_delete(GetFDSFileID(keyValueStore, domain));
                if (e) {
                    HAPAssert(e == FDS_ERR_NO_SPACE_IN_QUEUES);
                    HAPLogError(&logObject, "fds_file_delete failed.");
                    HAPFatalError();
                }
            } return;
            default: {
                HAPLogError(&logObject, "Inconsistent KVS queue.");
            } HAPFatalError();
        }
    }
}

/**
 * Singleton key-value store, since FDS callbacks do not support context pointer.
 */
static HAPPlatformKeyValueStoreRef _Nullable fdsCallbackContext;

/**
 * Callback to process FDS events.
 *
 * @param      event                Event to process.
 */
static void FDSCallback(
    const fds_evt_t *event)
{
    HAPPrecondition(event);
    
    if (!fdsCallbackContext) {
        HAPLogError(&logObject, "No FDS callback context.");
        return;
    }
    HAPPlatformKeyValueStoreRef keyValueStore = fdsCallbackContext;
    uint8_t *queueBytes = keyValueStore->_.bytes;
    
    if (!keyValueStore->busy) {
        HAPLogError(&logObject, "FDS event received while not expecting one.");
        return;
    }
    
    HAPAssert(keyValueStore->numBytes);
    
    switch (event->id) {
        case FDS_EVT_INIT: {
            if (queueBytes[0] != kKeyValueStoreOperation_Initialize) {
                HAPLogError(&logObject, "FDS_EVT_INIT received while not expecting one.");
                return;
            }
            HAPAssert(!keyValueStore->initialized);
            if (event->result) {
                HAPLogError(&logObject, "fds_init failed (async): %lu.", (unsigned long) event->result);
                HAPFatalError();
            }
            keyValueStore->initialized = true;
            Complete(keyValueStore, 1);
            LogQueueUtilization(keyValueStore, "FDS_EVT_INIT");
            Update(keyValueStore);
        } return;
        case FDS_EVT_WRITE:
        case FDS_EVT_UPDATE: {
            if (queueBytes[0] != kKeyValueStoreOperation_Write &&
                queueBytes[0] != kKeyValueStoreOperation_CollectGarbageForWrite)
            {
                HAPLogError(&logObject, "%s received while not expecting one.",
                    event->id == FDS_EVT_WRITE ? "FDS_EVT_WRITE" : "FDS_EVT_UPDATE");
                return;
            }
            HAPAssert(keyValueStore->numBytes >= 9);
            // TODO Can we do better to detect concurrent operations by other FDS clients?
            HAPPlatformKeyValueStoreDomain domain = (HAPPlatformKeyValueStoreDomain) queueBytes[1];
            if (event->write.file_id != GetFDSFileID(keyValueStore, domain)) {
                HAPLogError(&logObject, "%s received for different file_id %u other than expected %u.",
                    event->id == FDS_EVT_WRITE ? "FDS_EVT_WRITE" : "FDS_EVT_UPDATE",
                    event->write.file_id, GetFDSFileID(keyValueStore, domain));
                return;
            }
            HAPPlatformKeyValueStoreKey key = (HAPPlatformKeyValueStoreKey) queueBytes[2];
            if (event->write.record_key != GetFDSRecordID(keyValueStore, key)) {
                HAPLogError(&logObject, "%s received for different record_key %u other than expected %u.",
                    event->id == FDS_EVT_WRITE ? "FDS_EVT_WRITE" : "FDS_EVT_UPDATE",
                    event->write.record_key, GetFDSRecordID(keyValueStore, key));
                return;
            }
            if (event->result) {
                HAPLogError(&logObject, "fds_record_write/update failed (async): %lu.", (unsigned long) event->result);
                HAPFatalError();
            }
            uint16_t numWords = HAPReadLittleUInt16(&queueBytes[3]);
            HAPAssert(keyValueStore->numBytes >= 8 + 4 * (size_t) numWords);
            Complete(keyValueStore, 8 + 4 * (size_t) numWords);
            LogQueueUtilization(keyValueStore, event->id == FDS_EVT_WRITE ? "FDS_EVT_WRITE" : "FDS_EVT_UPDATE");
            Update(keyValueStore);
        } return;
        case FDS_EVT_DEL_RECORD: {
            if (queueBytes[0] != kKeyValueStoreOperation_Remove) {
                HAPLogError(&logObject, "FDS_EVT_DEL_RECORD received while not expecting one.");
                return;
            }
            HAPAssert(keyValueStore->numBytes >= 3);
            // TODO Can we do better to detect concurrent operations by other FDS clients?
            HAPPlatformKeyValueStoreDomain domain = (HAPPlatformKeyValueStoreDomain) queueBytes[1];
            if (event->del.file_id != GetFDSFileID(keyValueStore, domain)) {
                HAPLogError(&logObject,
                    "FDS_EVT_DEL_RECORD received for different file_id %u other than expected %u.",
                    event->del.file_id, GetFDSFileID(keyValueStore, domain));
                return;
            }
            HAPPlatformKeyValueStoreKey key = (HAPPlatformKeyValueStoreKey) queueBytes[2];
            if (event->del.record_key != GetFDSRecordID(keyValueStore, key)) {
                HAPLogError(&logObject,
                    "FDS_EVT_DEL_RECORD received for different record_key %u other than expected %u.",
                    event->del.record_key, GetFDSRecordID(keyValueStore, key));
                return;
            }
            if (!keyValueStore->isDeletingRecord) {
                HAPLogError(&logObject, "FDS_EVT_DEL_RECORD received while no deletion in progress.");
                return;
            }
            keyValueStore->isDeletingRecord = false;
            keyValueStore->modifiedRecordID = 0;
            if (event->result) {
                HAPLogError(&logObject, "fds_record_delete failed (async): %lu.", (unsigned long) event->result);
                HAPFatalError();
            }
            Complete(keyValueStore, 3);
            LogQueueUtilization(keyValueStore, "FDS_EVT_DEL_RECORD");
            Update(keyValueStore);
        } return;
        case FDS_EVT_DEL_FILE: {
            if (queueBytes[0] != kKeyValueStoreOperation_Purge) {
                HAPLogError(&logObject, "FDS_EVT_DEL_FILE received while not expecting one.");
                return;
            }
            HAPAssert(keyValueStore->numBytes >= 2);
            // TODO Can we do better to detect concurrent operations by other FDS clients?
            HAPPlatformKeyValueStoreDomain domain = (HAPPlatformKeyValueStoreDomain) queueBytes[1];
            if (event->del.file_id != GetFDSFileID(keyValueStore, domain)) {
                HAPLogError(&logObject, "FDS_EVT_DEL_FILE received for different file_id %u other than expected %u.",
                    event->del.file_id, GetFDSFileID(keyValueStore, domain));
                return;
            }
            if (event->result) {
                HAPLogError(&logObject, "fds_file_delete failed (async): %lu.", (unsigned long) event->result);
                HAPFatalError();
            }
            Complete(keyValueStore, 2);
            LogQueueUtilization(keyValueStore, "FDS_EVT_DEL_FILE");
            Update(keyValueStore);
        } return;
        case FDS_EVT_GC: {
            if (queueBytes[0] != kKeyValueStoreOperation_CollectGarbage &&
                queueBytes[0] != kKeyValueStoreOperation_CollectGarbageForWrite)
            {
                HAPLogError(&logObject, "FDS_EVT_GC received while not expecting one.");
                return;
            }
            size_t n = queueBytes[0] == kKeyValueStoreOperation_CollectGarbage ? 1 : 0;
            HAPAssert(keyValueStore->numBytes >= n);
            Complete(keyValueStore, n);
            LogQueueUtilization(keyValueStore, "FDS_EVT_GC");
            Update(keyValueStore);
        } return;
    }
    HAPFatalError();
}

void HAPPlatformKeyValueStoreCreate(
    HAPPlatformKeyValueStoreRef keyValueStore,
    const HAPPlatformKeyValueStoreOptions *_Nullable options)
{
    HAPPrecondition(keyValueStore);
    HAPPrecondition(options);
    HAPPrecondition(!((uintptr_t) options->bytes % 4));
    HAPPrecondition(options->maxBytes >= 2);
    HAPPrecondition(!fdsCallbackContext);
    
    fdsCallbackContext = keyValueStore;
    
    keyValueStore->_ = *options;
    keyValueStore->numBytes = 0;
    keyValueStore->peakNumBytes = 0;
    keyValueStore->busy = false;
    keyValueStore->initialized = false;
    
    uint8_t *queueBytes = keyValueStore->_.bytes;
    
    // Register event handler with FDS.
    ret_code_t e = fds_register(FDSCallback);
    if (e) {
        HAPAssert(e == FDS_ERR_USER_LIMIT_REACHED);
        HAPLogError(&logObject, "fds_register failed: %lu. Consider updating FDS_MAX_USERS.", (unsigned long) e);
        HAPFatalError();
    }
    
    // Queue initialization of file system.
    queueBytes[keyValueStore->numBytes++] = kKeyValueStoreOperation_Initialize;
    LogQueueUtilization(keyValueStore, "Queued Initialize");
    
    // Queue garbage collection.
    queueBytes[keyValueStore->numBytes++] = kKeyValueStoreOperation_CollectGarbage;
    LogQueueUtilization(keyValueStore, "Queued CollectGarbage");
    
    Update(keyValueStore);
    
    // Wait for initialization to complete.
    while (!keyValueStore->initialized) {
        app_sched_execute();
    }
}

HAP_RESULT_USE_CHECK
bool HAPPlatformKeyValueStoreIsBusy(
    HAPPlatformKeyValueStoreRef keyValueStore)
{
    HAPPrecondition(keyValueStore);
    
    return keyValueStore->busy || keyValueStore->numBytes;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformKeyValueStoreGet(
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformKeyValueStoreDomain domain,
    HAPPlatformKeyValueStoreKey key,
    void *_Nullable bytes,
    size_t maxBytes,
    size_t *_Nullable numBytes,
    bool *found)
HAP_DIAGNOSE_ERROR(!bytes && maxBytes, "empty buffer cannot have a length")
HAP_DIAGNOSE_ERROR((bytes && !numBytes) || (!bytes && numBytes),
    "length is only available if a buffer is supplied")
{
    HAPPrecondition(keyValueStore);
    HAPPrecondition(keyValueStore->initialized);
    HAPPrecondition(!maxBytes || bytes);
    HAPPrecondition((bytes == NULL) == (numBytes == NULL));
    HAPPrecondition(found);
    
    const uint8_t *value = NULL;
    uint16_t numWords = 0;
    
    // Search for more actual data in queue
    bool inQueue = false;
    uint8_t *queueBytes = keyValueStore->_.bytes;
    uint8_t *queueEnd = queueBytes + keyValueStore->numBytes;
    while (queueBytes < queueEnd) {
        switch (*queueBytes++) {
            case kKeyValueStoreOperation_Initialize:
            case kKeyValueStoreOperation_CollectGarbage: {
            } continue;
            case kKeyValueStoreOperation_Write:
            case kKeyValueStoreOperation_CollectGarbageForWrite: {
                HAPAssert(queueBytes <= queueEnd - 7);
                uint16_t words = HAPReadLittleUInt16(&queueBytes[2]);
                if (queueBytes[0] == domain && queueBytes[1] == key) {
                    value = &queueBytes[7];
                    numWords = words;
                    inQueue = true;
                }
                queueBytes += 7 + 4 * words;
            } continue;
            case kKeyValueStoreOperation_Remove: {
                if (queueBytes[0] == domain && queueBytes[1] == key) {
                    value = NULL;
                    numWords = 0;
                    inQueue = true;
                }
                queueBytes += 2;
            } continue;
            case kKeyValueStoreOperation_Purge: {
                if (queueBytes[0] == domain) {
                    value = NULL;
                    inQueue = true;
                }
                queueBytes++;
            } continue;
            default: {
                HAPLogError(&logObject, "Inconsistent KVS queue.");
            } HAPFatalError();
        }
    }
    
    if (inQueue && !value) {
        // remove command in queue
        *found = false;
        return kHAPError_None;
    }
    
    fds_record_desc_t descriptor;
    if (!inQueue) {
        // Find record.
        ret_code_t e = fds_record_find(
            GetFDSFileID(keyValueStore, domain), GetFDSRecordID(keyValueStore, key),
            &descriptor, &(fds_find_token_t) {
                .p_addr = NULL,
                .page = 0
            });
        if (e) {
            HAPAssert(e == FDS_ERR_NOT_FOUND);
            *found = false;
            return kHAPError_None;
        }
        
        // Open record.
        fds_flash_record_t flashRecord;
        e = fds_record_open(&descriptor, &flashRecord);
        if (e) {
            HAPAssert(e == FDS_ERR_CRC_CHECK_FAILED);
            HAPLogError(&logObject, "fds_record_open %02X.%02X failed: %lu.", domain, key, (unsigned long) e);
            return kHAPError_Unknown;
        }
        value = flashRecord.p_data;
        numWords = flashRecord.p_header->length_words;
    }
    *found = true;
    
    // First byte contains number of padded bytes.
    if (!numWords) {
        HAPLog(&logObject, "Corrupted file %02X.%02X contains no number of padded bytes.", domain, key);
        return kHAPError_Unknown;
    }
    uint8_t numPaddedBytes = value[0];
    if (numPaddedBytes >= 4) {
        HAPLog(&logObject, "Corrupted file %02X.%02X contains invalid padding length %u.", domain, key, numPaddedBytes);
        return kHAPError_Unknown;
    }
    
    if (bytes) {
        // Copy content.
        *numBytes = (size_t) numWords * 4 - 1 - numPaddedBytes;
        if (*numBytes > maxBytes) {
            *numBytes = maxBytes;
        }
        HAPRawBufferCopyBytes(bytes, &value[1], *numBytes);
    }
    
    if (!inQueue) {
        // Close record.
        ret_code_t e = fds_record_close(&descriptor);
        if (e) {
            HAPLogError(&logObject, "fds_record_close failed: %lu.", (unsigned long) e);
            HAPFatalError();
        }
    }
    return kHAPError_None;
}

#define CHECK_SPACE_OR_RETURN(NEEDED) \
    do { \
        size_t needed = (NEEDED); \
        size_t available = keyValueStore->_.maxBytes - keyValueStore->numBytes; \
        if (needed > available) { \
        HAPLog(&logObject, "Not enough KVS queue space! Needed: %lu, available: %lu.", \
            (unsigned long) needed, (unsigned long) available); \
        return kHAPError_Unknown; \
        } \
    } while (0)

HAP_RESULT_USE_CHECK
HAPError HAPPlatformKeyValueStoreSet(
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformKeyValueStoreDomain domain,
    HAPPlatformKeyValueStoreKey key,
    const void *bytes,
    size_t numBytes)
{
    HAPPrecondition(keyValueStore);
    HAPPrecondition(keyValueStore->initialized);
    HAPPrecondition(bytes);
    size_t maxBytes = HAPMin(UINT16_MAX / 4, BYTES_PER_WORD * FDS_VIRTUAL_PAGE_SIZE - 14);
    if (numBytes > maxBytes) {
        HAPLogError(&logObject, "Writes > %zu bytes are not supported (requested: %zu bytes).", maxBytes, numBytes);
        return kHAPError_Unknown;
    }
    uint8_t *queueBytes = keyValueStore->_.bytes;
    
    // Storage format: [ num_pad ][ body ][ padding ]
    HAPAssert(BYTES_PER_WORD == 4);
    uint16_t numWords = (uint16_t) (((1 + (uint16_t) numBytes) + 3) / 4);
    uint8_t numPaddedBytes = (uint8_t) (numWords * 4 - (1 + numBytes));
    HAPAssert(numPaddedBytes < 4);
    CHECK_SPACE_OR_RETURN(8 + (1 + numBytes) + numPaddedBytes);
    
    // Queue write.
    // TODO Use reserved write to ensure that there is enough memory available.
    queueBytes[keyValueStore->numBytes++] = kKeyValueStoreOperation_Write;
    queueBytes[keyValueStore->numBytes++] = domain;
    queueBytes[keyValueStore->numBytes++] = key;
    HAPWriteLittleUInt16(&queueBytes[keyValueStore->numBytes], numWords); keyValueStore->numBytes += 2;
    queueBytes[keyValueStore->numBytes++] = 0;
    queueBytes[keyValueStore->numBytes++] = 0;
    queueBytes[keyValueStore->numBytes++] = 0;
    queueBytes[keyValueStore->numBytes++] = numPaddedBytes;
    HAPRawBufferCopyBytes(&queueBytes[keyValueStore->numBytes], bytes, numBytes);
    keyValueStore->numBytes += numBytes;
    HAPRawBufferZero(&queueBytes[keyValueStore->numBytes], numPaddedBytes);
    keyValueStore->numBytes += numPaddedBytes;
    LogQueueUtilization(keyValueStore, "Queued Write");
    
    Update(keyValueStore);
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformKeyValueStoreRemove(
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformKeyValueStoreDomain domain,
    HAPPlatformKeyValueStoreKey key)
{
    HAPPrecondition(keyValueStore);
    HAPPrecondition(keyValueStore->initialized);
    uint8_t *queueBytes = keyValueStore->_.bytes;
    
    CHECK_SPACE_OR_RETURN(3);
    
    // Queue removal.
    queueBytes[keyValueStore->numBytes++] = kKeyValueStoreOperation_Remove;
    queueBytes[keyValueStore->numBytes++] = domain;
    queueBytes[keyValueStore->numBytes++] = key;
    LogQueueUtilization(keyValueStore, "Queued Remove");
    
    Update(keyValueStore);
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformKeyValueStoreEnumerate(
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformKeyValueStoreDomain domain,
    HAPPlatformKeyValueStoreEnumerateCallback callback,
    void *_Nullable context)
{
    HAPPrecondition(keyValueStore);
    HAPPrecondition(keyValueStore->initialized);
    HAPPrecondition(callback);
    
    HAPError err;
    
    // Initialize iterator.
    fds_find_token_t token = {
        .p_addr = NULL,
        .page = 0
    };
    
    bool shouldContinue = true;
    while (shouldContinue) {
        // Find next record.
        fds_record_desc_t descriptor;
        ret_code_t e = fds_record_find_in_file(GetFDSFileID(keyValueStore, domain), &descriptor, &token);
        if (e) {
            HAPAssert(e == FDS_ERR_NOT_FOUND);
            break;
        }
        
        // Work around for when fds_record_find_in_file reports records that are being deleted.
        // In that case, fds_record_open would fail with FDS_ERR_CRC_CHECK_FAILED.
        if (keyValueStore->isDeletingRecord && descriptor.record_id == keyValueStore->modifiedRecordID) {
            HAPLog(&logObject, "fds_record_find_in_file %02X.next reported record being deleted.", domain);
            continue;
        }

        // Open record.
        fds_flash_record_t flashRecord;
        e = fds_record_open(&descriptor, &flashRecord);
        if (e) {
            HAPAssert(e == FDS_ERR_CRC_CHECK_FAILED);
            HAPLogError(&logObject, "fds_record_open %02X.next failed: %lu.", domain, (unsigned long) e);
            return kHAPError_Unknown;
        }
        
        // Extract key.
        uint16_t fdsKeyID = flashRecord.p_header->record_key;
        
        // Close record.
        e = fds_record_close(&descriptor);
        HAPAssert(!e);
        
        // Compute platform key.
        if (fdsKeyID < keyValueStore->_.baseFDSRecordID || fdsKeyID - keyValueStore->_.baseFDSRecordID >= UINT8_MAX) {
            HAPLog(&logObject, "Skipping unexpected FDS key 0x%04X in domain 0x%02X.", fdsKeyID, domain);
            continue;
        }
        HAPPlatformKeyValueStoreKey key = (HAPPlatformKeyValueStoreKey) (fdsKeyID - keyValueStore->_.baseFDSRecordID);
        
        // Check if key has pending modifications.
        bool inQueue = false;
        uint8_t *queueBytes = keyValueStore->_.bytes;
        uint8_t *queueEnd = queueBytes + keyValueStore->numBytes;
        while (!inQueue && queueBytes < queueEnd) {
            switch (*queueBytes++) {
                case kKeyValueStoreOperation_Initialize:
                case kKeyValueStoreOperation_CollectGarbage: {
                } continue;
                case kKeyValueStoreOperation_Write:
                case kKeyValueStoreOperation_CollectGarbageForWrite: {
                    if (queueBytes[0] == domain && queueBytes[1] == key) {
                        inQueue = true;
                    }
                    uint16_t numWords = HAPReadLittleUInt16(&queueBytes[2]);
                    queueBytes += 7 + 4 * numWords;
                } continue;
                case kKeyValueStoreOperation_Remove: {
                    if (queueBytes[0] == domain && queueBytes[1] == key) {
                        inQueue = true;
                    }
                    queueBytes += 2;
                } continue;
                case kKeyValueStoreOperation_Purge: {
                    if (queueBytes[0] == domain) {
                        inQueue = true;
                    }
                    queueBytes++;
                } continue;
                default: {
                    HAPLogError(&logObject, "Inconsistent KVS queue.");
                } HAPFatalError();
            }
        }
        if (inQueue) {
            HAPLogDebug(&logObject, "%02X.%02X Delaying enumeration because of pending modifications.", domain, key);
            continue;
        }
        
        // Invoke callback.
        err = callback(context, keyValueStore, domain, key, &shouldContinue);
        if (err) {
            HAPAssert(err == kHAPError_Unknown);
            return err;
        }
    }
    
    // Report entries with pending writes.
    uint8_t *queueBytes = keyValueStore->_.bytes;
    uint8_t *queueEnd = queueBytes + keyValueStore->numBytes;
    while (shouldContinue && queueBytes < queueEnd) {
        switch (*queueBytes++) {
            case kKeyValueStoreOperation_Initialize:
            case kKeyValueStoreOperation_CollectGarbage: {
            } continue;
            case kKeyValueStoreOperation_Write:
            case kKeyValueStoreOperation_CollectGarbageForWrite: {
                bool report = queueBytes[0] == domain;
                HAPPlatformKeyValueStoreKey key = queueBytes[1];
                uint16_t numWords = HAPReadLittleUInt16(&queueBytes[2]);
                queueBytes += 7 + 4 * numWords;
                if (report) {
                    // Attempt to report, if not modified again.
                    bool inQ = false;
                    uint8_t *q = queueBytes;
                    while (!inQ && q < queueEnd) {
                        switch (*q++) {
                            case kKeyValueStoreOperation_Initialize:
                            case kKeyValueStoreOperation_CollectGarbage: {
                            } continue;
                            case kKeyValueStoreOperation_Write:
                            case kKeyValueStoreOperation_CollectGarbageForWrite: {
                                if (q[0] == domain && q[1] == key) {
                                    inQ = true;
                                }
                                numWords = HAPReadLittleUInt16(&q[2]);
                                q += 7 + 4 * numWords;
                            } continue;
                            case kKeyValueStoreOperation_Remove: {
                                if (q[0] == domain && q[1] == key) {
                                    inQ = true;
                                }
                                q += 2;
                            } continue;
                            case kKeyValueStoreOperation_Purge: {
                                if (q[0] == domain) {
                                    inQ = true;
                                }
                                q++;
                            } continue;
                            default: {
                                HAPLogError(&logObject, "Inconsistent KVS queue.");
                            } HAPFatalError();
                        }
                    }
                    if (inQ) {
                        HAPLogDebug(&logObject,
                            "%02X.%02X Delaying enumeration because of even more pending modifications.", domain, key);
                        continue;
                    }
                    
                    // Invoke callback.
                    err = callback(context, keyValueStore, domain, key, &shouldContinue);
                    if (err) {
                        HAPAssert(err == kHAPError_Unknown);
                        return err;
                    }
                }
            } continue;
            case kKeyValueStoreOperation_Remove: {
                queueBytes += 2;
            } continue;
            case kKeyValueStoreOperation_Purge: {
                queueBytes++;
            } continue;
            default: {
                HAPLogError(&logObject, "Inconsistent KVS queue.");
            } HAPFatalError();
        }
    }
    
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformKeyValueStorePurgeDomain(
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformKeyValueStoreDomain domain)
{
    HAPPrecondition(keyValueStore);
    HAPPrecondition(keyValueStore->initialized);
    CHECK_SPACE_OR_RETURN(2);
    uint8_t *queueBytes = keyValueStore->_.bytes;
    
    // Queue purge.
    queueBytes[keyValueStore->numBytes++] = kKeyValueStoreOperation_Purge;
    queueBytes[keyValueStore->numBytes++] = domain;
    LogQueueUtilization(keyValueStore, "Queued Purge");
    
    Update(keyValueStore);
    return kHAPError_None;
}

bool HAPPlatformKeyValueStoreNeedsTimeToPersist(
    HAPPlatformKeyValueStoreRef keyValueStore)
{
    HAPPrecondition(keyValueStore);
    HAPPrecondition(keyValueStore->initialized);
    
    // If the key-value store queue is getting full, report true.
    size_t numReserveBytes = 1280;
    if (keyValueStore->_.maxBytes < numReserveBytes) {
        numReserveBytes = keyValueStore->_.maxBytes;
    }
    HAPAssert(keyValueStore->numBytes <= keyValueStore->_.maxBytes);
    if (keyValueStore->_.maxBytes - keyValueStore->numBytes < numReserveBytes) {
        HAPLogDebug(&logObject, "Key-value store queue is getting full.");
        return true;
    }
    
    // If a Software Token is being persisted, report true.
    uint8_t *queueBytes = keyValueStore->_.bytes;
    uint8_t *queueEnd = queueBytes + keyValueStore->numBytes;
    while (queueBytes < queueEnd) {
        switch (*queueBytes++) {
            case kKeyValueStoreOperation_Initialize:
            case kKeyValueStoreOperation_CollectGarbage: {
            } continue;
            case kKeyValueStoreOperation_Write:
            case kKeyValueStoreOperation_CollectGarbageForWrite: {
                if (queueBytes[0] == kSDKKeyValueStoreDomain_Provisioning &&
                    queueBytes[1] == kSDKKeyValueStoreKey_Provisioning_MFiToken)
                {
                    HAPLogDebug(&logObject, "Persisting Software Token update.");
                    return true;
                }
                uint16_t numWords = HAPReadLittleUInt16(&queueBytes[2]);
                queueBytes += 7 + 4 * numWords;
            } continue;
            case kKeyValueStoreOperation_Remove: {
                if (queueBytes[0] == kSDKKeyValueStoreDomain_Provisioning &&
                    queueBytes[1] == kSDKKeyValueStoreKey_Provisioning_MFiToken)
                {
                    HAPLogDebug(&logObject, "Persisting Software Token removal.");
                    return true;
                }
                queueBytes += 2;
            } continue;
            case kKeyValueStoreOperation_Purge: {
                if (queueBytes[0] == kSDKKeyValueStoreDomain_Provisioning) {
                    HAPLogDebug(&logObject, "Persisting Software Token domain purge.");
                    return true;
                }
                queueBytes++;
            } continue;
            default: {
                HAPLogError(&logObject, "Inconsistent KVS queue.");
            } HAPFatalError();
        }
    }
    
    return false;
}
