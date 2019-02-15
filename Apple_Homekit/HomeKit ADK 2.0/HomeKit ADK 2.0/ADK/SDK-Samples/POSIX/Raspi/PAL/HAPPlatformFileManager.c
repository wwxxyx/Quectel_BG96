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

#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "HAPPlatform+Init.h"
#include "HAPPlatformFileManager.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "FileManager"
};

HAPError HAPPlatformFileManagerCreateDirectory(
    const char *dirPath)
{
    HAPPrecondition(dirPath);
    
    HAPError err;
    
    char path[PATH_MAX];
    
    // Duplicate string, as each path component needs to be modified to be NULL-terminated.
    // Duplicate is necessary, as input may reside in read-only memory.
    err = HAPStringWithFormat(path, sizeof path, "%s", dirPath);
    if (err) {
        HAPAssert(err == kHAPError_OutOfResources);
        HAPLogError(&logObject, "Directory path too long: %s", dirPath);
        return kHAPError_Unknown;
    }
    
    // Create parent directories.
    for (char *start = path, *end = strchr(start, '/'); end; start = end + 1, end = strchr(start, '/')) {
        if (start == end) {
            // Root, or double separator.
            continue;
        }
        
        // Replace separator with \0 temporarily. Create directory. Restore back.
        *end = '\0';
        int e = mkdir(path, S_IRWXU);
        *end = '/';
        if (e) {
            int _errno = errno;
            HAPAssert(e == -1);
            if (_errno == EEXIST) {
                continue;
            }
            *end = '\0';
            HAPLogError(&logObject, "mkdir %s failed: %d.", path, _errno);
            *end = '/';
            return kHAPError_Unknown;
        }
    }
    
    // Create directory.
    int e = mkdir(path, S_IRWXU);
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        if (_errno != EEXIST) {
            HAPLogError(&logObject, "mkdir %s failed: %d.", path, _errno);
            return kHAPError_Unknown;
        }
    }
    
    return kHAPError_None;
}

HAPError HAPPlatformFileManagerWriteFile(
    const char *filePath,
    const void *_Nullable bytes,
    size_t numBytes)
    HAP_DIAGNOSE_ERROR(!bytes && numBytes, "empty buffer cannot have a length")
{
    HAPPrecondition(filePath);
    HAPAssert(bytes || numBytes); // bytes ==> numBytes > 0.
    
    HAPError err;
    
    char targetDirPath[PATH_MAX];
    err = HAPStringWithFormat(targetDirPath, sizeof targetDirPath, "%s", filePath);
    if (err) {
        HAPAssert(err == kHAPError_OutOfResources);
        HAPLogError(&logObject, "Not enough resources to copy string: %s", filePath);
        return kHAPError_Unknown;
    }
    
    // Get split relative file path and dir path
    const char *relativeFilePath = filePath;
    {
        const size_t filePathLength = HAPStringGetNumBytes(filePath);
        HAPPrecondition(filePathLength);
        
        size_t dirPathLength = 0;
        for (size_t i = 0; i < filePathLength; ++i) {
            if (filePath[i] == '/') {
                dirPathLength = i;
                relativeFilePath = &filePath[i+1];
            }
        }
        HAPAssert(dirPathLength < filePathLength);
        targetDirPath[dirPathLength] = '\0';
        
        HAPAssert(dirPathLength == HAPStringGetNumBytes(targetDirPath));
        HAPAssert(dirPathLength + HAPStringGetNumBytes(relativeFilePath) + 1 == filePathLength);
    }

    // Create directory.
    HAPPlatformFileManagerCreateDirectory(targetDirPath);
    
    // Open the target dir
    DIR *targetDirRef = opendir(targetDirPath);
    if (!targetDirRef) {
        int _errno = errno;
        HAPLogError(&logObject, "opendir %s failed: %d.", targetDirPath, _errno);
        return kHAPError_Unknown;
    }
    int targetDirFD = dirfd(targetDirRef);
    if (targetDirFD < 0) {
        int _errno = errno;
        HAPAssert(targetDirFD == -1);
        HAPLogError(&logObject, "dirfd %s failed: %d.", targetDirPath, _errno);
        HAPPlatformFileManagerCloseDirFreeSafe(targetDirRef);
        return kHAPError_Unknown;
    }
    
    // Create the filename of the temporary file.
    char tmpPath[PATH_MAX];
    err = HAPStringWithFormat(tmpPath, sizeof tmpPath, "%s-tmp", relativeFilePath);
    if (err) {
        HAPAssert(err = kHAPError_OutOfResources);
        HAPLogError(&logObject, "Not enough resources to get path: %s-tmp", relativeFilePath);
        HAPPlatformFileManagerCloseDirFreeSafe(targetDirRef);
        return kHAPError_Unknown;
    }
    
    // Open the tempfile
    int tmpPathFD;
    do {
        tmpPathFD = openat(targetDirFD, tmpPath, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR);
    } while (tmpPathFD == -1 && errno == EINTR);
    if (tmpPathFD < 0) {
        int _errno = errno;
        HAPAssert(tmpPathFD == -1);
        HAPLogError(&logObject, "open %s in %s failed: %d.", tmpPath, targetDirPath, _errno);
        HAPPlatformFileManagerCloseDirFreeSafe(targetDirRef);
        return kHAPError_Unknown;
    }
    
    // Write to temporary file.
    if (bytes) {
        size_t o = 0;
        while (o < numBytes) {
            size_t c = numBytes - o;
            if (c > SSIZE_MAX) {
                c = SSIZE_MAX;
            }
        
            ssize_t n;
            do {
                n = write(tmpPathFD, &((const uint8_t *) bytes)[o], c);
            } while (n == -1 && errno == EINTR);
            if (n < 0) {
                int _errno = errno;
                HAPAssert(n == -1);
                HAPLogError(&logObject, "write to temporary file %s failed: %d.", tmpPath, _errno);
                break;
            }
            if (n == 0) {
                HAPLogError(&logObject, "write to temporary file %s returned EOF.", tmpPath);
                break;
            }
        
            HAPAssert((size_t) n <= c);
            o += (size_t) n;
        }
        if (o != numBytes) {
            (void) close(tmpPathFD);
            int e = remove(tmpPath);
            if (e) {
                int _errno = errno;
                HAPAssert(e == -1);
                HAPLogError(&logObject, "remove of temporary file %s failed: %d.", tmpPath, _errno);
                HAPPlatformFileManagerCloseDirFreeSafe(targetDirRef);
                return kHAPError_Unknown;
            }
            HAPLogError(&logObject, "Error writing temporary file %s in %s.", tmpPath, targetDirPath);
            HAPPlatformFileManagerCloseDirFreeSafe(targetDirRef);
            return kHAPError_Unknown;
        }
    }
    
    // Try to synchronize and close the temporary file.
    {
        int e;
        do {
            e = fsync(tmpPathFD);
        } while (e == -1 && errno == EINTR);
        if (e) {
            int _errno = errno;
            HAPAssert(e == -1);
            HAPLogError(&logObject, "fsync of temporary file %s failed: %d.", tmpPath, _errno);
        }
        close(tmpPathFD);
    }
    
    // Fsync dir
    {
        int e;
        do {
            e = fsync(targetDirFD);
        } while (e == -1 && errno == EINTR);
        if (e < 0) {
            int _errno = errno;
            HAPAssert(e == -1);
            HAPLogError(&logObject, "fsync of the target directory %s failed: %d", targetDirPath, _errno);
            HAPPlatformFileManagerCloseDirFreeSafe(targetDirRef);
            return kHAPError_Unknown;
        }
    }
    
    // Rename file
    {
        int e = renameat(targetDirFD, tmpPath, targetDirFD, relativeFilePath);
        if (e) {
            int _errno = errno;
            HAPAssert(e == -1);
            HAPLogError(&logObject, "rename of temporary file %s to %s failed: %d.", tmpPath, filePath, _errno);
            HAPPlatformFileManagerCloseDirFreeSafe(targetDirRef);
            return kHAPError_Unknown;
        }
    }
    
    // Fsync dir
    {
        int e;
        do {
            e = fsync(targetDirFD);
        } while (e == -1 && errno == EINTR);
        if (e < 0) {
            int _errno = errno;
            HAPAssert(e == -1);
            HAPLogError(&logObject, "fsync of the target directory %s failed: %d", targetDirPath, _errno);
            HAPPlatformFileManagerCloseDirFreeSafe(targetDirRef);
            return kHAPError_Unknown;
        }
    }
    
    HAPPlatformFileManagerCloseDirFreeSafe(targetDirRef);
    return kHAPError_None;
}

HAPError HAPPlatformFileManagerReadFile(
    const char *filePath,
    void *_Nullable bytes,
    size_t maxBytes,
    size_t *_Nullable numBytes,
    bool *valid)
{
    HAPPrecondition(filePath);
    HAPPrecondition(!maxBytes || bytes);
    HAPPrecondition((bytes == NULL) == (numBytes == NULL));
    HAPPrecondition(valid);
    
    *valid = false;
    
    int fd;
    do {
        fd = open(filePath, O_RDONLY);
    } while (fd == -1 && errno == EINTR);
    
    if (fd < 0) {
        int _errno = errno;
        if (_errno == ENOENT) {
            return kHAPError_None;
        }
        HAPAssert(fd == -1);
        HAPLogError(&logObject, "open %s failed: %d.", filePath, _errno);
        return kHAPError_Unknown;
    }
    *valid = true;
    
    if (bytes) {
        HAPAssert(numBytes);
        
        size_t o = 0;
        while (o < maxBytes) {
            size_t c = maxBytes - o;
            if (c > SSIZE_MAX) {
                c = SSIZE_MAX;
            }
            
            ssize_t n;
            do {
                n = read(fd, &((uint8_t *) bytes)[o], c);
            } while (n == -1 && errno == EINTR);
            if (n < 0) {
                int _errno = errno;
                HAPAssert(n == -1);
                HAPLogError(&logObject, "read %s failed: %d.", filePath, _errno);
                (void) close(fd);
                return kHAPError_Unknown;
            }
            
            HAPAssert((size_t) n <= c);
            o += (size_t) n;
            
            if (n == 0) {
                break;
            }
        }
        *numBytes = o;
    }
    (void) close(fd);
    
    return kHAPError_None;
}
