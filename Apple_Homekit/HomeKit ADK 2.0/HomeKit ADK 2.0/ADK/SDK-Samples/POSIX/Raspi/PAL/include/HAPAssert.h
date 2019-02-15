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

#ifndef HAP_ASSERT_H
#define HAP_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "HAPBase.h"

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

// Validate flag for disabling assertions.
#ifndef HAP_DISABLE_ASSERTS
    #define HAP_DISABLE_ASSERTS (0)
#endif
#if HAP_DISABLE_ASSERTS < 0 || HAP_DISABLE_ASSERTS > 1
    #error "Invalid HAP_DISABLE_ASSERTS."
#endif

// Validate flag for disabling preconditions.
#ifndef HAP_DISABLE_PRECONDITIONS
    #define HAP_DISABLE_PRECONDITIONS (0)
#endif
#if HAP_DISABLE_PRECONDITIONS < 0 || HAP_DISABLE_PRECONDITIONS > 1
    #error "Invalid HAP_DISABLE_PRECONDITIONS."
#endif

/**
 * Aborts the program after a failure without logging.
 *
 * - This function should not be used directly. Use the macros instead.
 */
HAP_DEPRECATED
HAP_NORETURN
void HAPAssertAbortInternal(void);

/**
 * Aborts the program after a failed assertion.
 *
 * - This function should not be used directly. Use HAPAssert instead.
 *
 * @param      callerFunction       Function in which the assertion failed.
 * @param      callerFile           File in which the assertion failed.
 * @param      callerLine           Line at which the assertion failed.
 */
HAP_DEPRECATED
HAP_NORETURN
void HAPAssertInternal(
    const char *callerFunction,
    const char *callerFile,
    int callerLine);

/**
 * Performs an assert if assertions are enabled.
 *
 * @param      condition            The condition to test. Only evaluated when assertions are enabled.
 */
#define HAPAssert(condition) \
    do { \
        if (!HAP_DISABLE_ASSERTS && !(condition)) { \
            HAP_DIAGNOSTIC_PUSH \
            HAP_DIAGNOSTIC_IGNORED_CLANG("-Wdeprecated-declarations") \
            HAP_DIAGNOSTIC_IGNORED_GCC("-Wdeprecated-declarations") \
            HAP_DIAGNOSTIC_IGNORED_ARMCC(1361) \
            HAP_DIAGNOSTIC_IGNORED_ICCARM(Pe1215) \
            if (HAP_LOG_LEVEL >= 1) { \
                HAPAssertInternal(__func__, HAP_FILE, __LINE__); \
            } else { \
                HAPAssertAbortInternal(); \
            } \
            HAP_DIAGNOSTIC_RESTORE_ICCARM(Pe1215) \
            HAP_DIAGNOSTIC_POP \
        } \
    } while (0)

/**
 * Aborts the program after a failed assertion.
 *
 * - This function should not be used directly. Use HAPAssertionFailure instead.
 *
 * @param      callerFunction       Function in which the assertion failed.
 * @param      callerFile           File in which the assertion failed.
 * @param      callerLine           Line at which the assertion failed.
 */
HAP_DEPRECATED
HAP_NORETURN
void HAPAssertionFailureInternal(
    const char *callerFunction,
    const char *callerFile,
    int callerLine);
    
/**
 * Indicates that an internal sanity check failed if assertions are enabled.
 */
#define HAPAssertionFailure() \
    do { \
        if (!HAP_DISABLE_ASSERTS) { \
            HAP_DIAGNOSTIC_PUSH \
            HAP_DIAGNOSTIC_IGNORED_CLANG("-Wdeprecated-declarations") \
            HAP_DIAGNOSTIC_IGNORED_GCC("-Wdeprecated-declarations") \
            HAP_DIAGNOSTIC_IGNORED_ARMCC(1361) \
            HAP_DIAGNOSTIC_IGNORED_ICCARM(Pe1215) \
            if (HAP_LOG_LEVEL >= 1) { \
                HAPAssertionFailureInternal(__func__, HAP_FILE, __LINE__); \
            } else { \
                HAPAssertAbortInternal(); \
            } \
            HAP_DIAGNOSTIC_RESTORE_ICCARM(Pe1215) \
            HAP_DIAGNOSTIC_POP \
        } \
    } while (0)

/**
 * Aborts the program after a failed precondition.
 *
 * - This function should not be used directly. Use HAPPrecondition instead.
 *
 * @param      condition            The condition of the precondition that failed.
 * @param      callerFunction       Function in which the precondition failed.
 */
HAP_DEPRECATED
HAP_NORETURN
void HAPPreconditionInternal(
    const char *condition,
    const char *callerFunction);

/**
 * Checks a necessary condition for making forward progress if precondition checks are enabled.
 *
 * @param      condition            The condition to test. Only evaluated when precondition checks are enabled.
 */
#define HAPPrecondition(condition) \
    do { \
        if (!HAP_DISABLE_PRECONDITIONS && !(condition)) { \
            HAP_DIAGNOSTIC_PUSH \
            HAP_DIAGNOSTIC_IGNORED_CLANG("-Wdeprecated-declarations") \
            HAP_DIAGNOSTIC_IGNORED_GCC("-Wdeprecated-declarations") \
            HAP_DIAGNOSTIC_IGNORED_ARMCC(1361) \
            HAP_DIAGNOSTIC_IGNORED_ICCARM(Pe1215) \
            if (HAP_LOG_LEVEL >= 1) { \
                HAPPreconditionInternal(#condition, __func__); \
            } else { \
                HAPAssertAbortInternal(); \
            } \
            HAP_DIAGNOSTIC_RESTORE_ICCARM(Pe1215) \
            HAP_DIAGNOSTIC_POP \
        } \
    } while (0)

/**
 * Aborts the program after a failed precondition.
 *
 * - This function should not be used directly. Use HAPPreconditionFailure instead.
 *
 * @param      callerFunction       Function in which the precondition failed.
 */
HAP_DEPRECATED
HAP_NORETURN
void HAPPreconditionFailureInternal(
    const char *callerFunction);
    
/**
 * Indicates that a precondition was violated if precondition checks are enabled.
 */
#define HAPPreconditionFailure() \
    do { \
        if (!HAP_DISABLE_PRECONDITIONS) { \
            HAP_DIAGNOSTIC_PUSH \
            HAP_DIAGNOSTIC_IGNORED_CLANG("-Wdeprecated-declarations") \
            HAP_DIAGNOSTIC_IGNORED_GCC("-Wdeprecated-declarations") \
            HAP_DIAGNOSTIC_IGNORED_ARMCC(1361) \
            HAP_DIAGNOSTIC_IGNORED_ICCARM(Pe1215) \
            if (HAP_LOG_LEVEL >= 1) { \
                HAPPreconditionFailureInternal(__func__); \
            } else { \
                HAPAssertAbortInternal(); \
            } \
            HAP_DIAGNOSTIC_RESTORE_ICCARM(Pe1215) \
            HAP_DIAGNOSTIC_POP \
        } \
    } while (0)

/**
 * Aborts the program after a fatal error.
 *
 * - This function should not be used directly. Use HAPFatalError instead.
 *
 * @param      callerFunction       Function in which the fatal error occurred.
 * @param      callerFile           File in which the fatal error occurred.
 * @param      callerLine           Line at which the fatal error occurred.
 */
HAP_DEPRECATED
HAP_NORETURN
void HAPFatalErrorInternal(
    const char *callerFunction,
    const char *callerFile,
    int callerLine);
    
/**
 * Unconditionally indicates a fatal error.
 */
#define HAPFatalError() \
    do { \
        HAP_DIAGNOSTIC_PUSH \
        HAP_DIAGNOSTIC_IGNORED_CLANG("-Wdeprecated-declarations") \
        HAP_DIAGNOSTIC_IGNORED_GCC("-Wdeprecated-declarations") \
        HAP_DIAGNOSTIC_IGNORED_ARMCC(1361) \
        HAP_DIAGNOSTIC_IGNORED_ICCARM(Pe1215) \
        if (HAP_LOG_LEVEL >= 1) { \
            HAPFatalErrorInternal(__func__, HAP_FILE, __LINE__); \
        } else { \
            HAPAssertAbortInternal(); \
        } \
        HAP_DIAGNOSTIC_RESTORE_ICCARM(Pe1215) \
        HAP_DIAGNOSTIC_POP \
    } while (0)

#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif

#ifdef __cplusplus
}
#endif

#endif
