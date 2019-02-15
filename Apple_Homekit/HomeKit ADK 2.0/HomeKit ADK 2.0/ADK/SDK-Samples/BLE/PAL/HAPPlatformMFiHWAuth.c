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

#include <stdio.h>

#include "app_util_platform.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

#include "HAPPlatformMFiHWAuth+Init.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "MFiHWAuth"
};

void HAPPlatformMFiHWAuthCreate(
    HAPPlatformMFiHWAuthRef mfiHWAuth,
    const HAPPlatformMFiHWAuthOptions *options)
{
    HAPPrecondition(mfiHWAuth);
    HAPPrecondition(options);

    // Get configuration.
    mfiHWAuth->vccPin = options->vccPin;
    mfiHWAuth->sclPin = options->sclPin;
    mfiHWAuth->sdaPin = options->sdaPin;
    switch (options->twiInstanceID) {
        #if TWI0_ENABLED
        case 0: mfiHWAuth->twiInstanceID = (nrf_drv_twi_t) NRF_DRV_TWI_INSTANCE(0); break;
        #endif
        #if TWI1_ENABLED
        case 1: mfiHWAuth->twiInstanceID = (nrf_drv_twi_t) NRF_DRV_TWI_INSTANCE(1); break;
        #endif
        default: HAPPreconditionFailure();
    }
    HAPPrecondition(options->i2cAddress == 0x10 || options->i2cAddress == 0x11);
    mfiHWAuth->i2cAddress = options->i2cAddress;

    // Configure pins.
    nrf_gpio_pin_set(mfiHWAuth->vccPin);
    nrf_gpio_cfg(mfiHWAuth->vccPin,
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        NRF_GPIO_PIN_NOPULL,
        NRF_GPIO_PIN_H0H1,
        NRF_GPIO_PIN_NOSENSE);

    // Initialize I2C module.
    ret_code_t e = nrf_drv_twi_init(&mfiHWAuth->twiInstanceID, &(const nrf_drv_twi_config_t) {
        .scl = mfiHWAuth->sclPin,
        .sda = mfiHWAuth->sdaPin,
        .frequency = NRF_DRV_TWI_FREQ_100K,
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
        .clear_bus_init = false,
        .hold_bus_uninit = false
    }, /* event_handler: */ NULL, /* p_context: */ NULL);
    if (e) {
        HAPAssert(e == NRF_ERROR_INVALID_STATE || e == NRF_ERROR_BUSY);
        HAPLog(&logObject, "nrf_drv_twi_init failed: %lu.", e);
        HAPFatalError();
    }

    // Enter power-saving.
    mfiHWAuth->enabled = false;
    mfiHWAuth->sclConfiguration = NRF_GPIO->PIN_CNF[mfiHWAuth->sclPin];
    mfiHWAuth->sdaConfiguration = NRF_GPIO->PIN_CNF[mfiHWAuth->sdaPin];
    nrf_gpio_cfg_output(mfiHWAuth->sclPin);
    nrf_gpio_pin_clear(mfiHWAuth->sclPin);
    nrf_gpio_cfg_output(mfiHWAuth->sdaPin);
    nrf_gpio_pin_clear(mfiHWAuth->sdaPin);
    nrf_gpio_pin_clear(mfiHWAuth->vccPin);

    // Initialization complete.
    mfiHWAuth->initialized = true;
}

void HAPPlatformMFiHWAuthRelease(
    HAPPlatformMFiHWAuthRef mfiHWAuth)
{
    HAPPrecondition(mfiHWAuth);
    HAPPrecondition(mfiHWAuth->initialized);

    // Disable and deinitialize I2C module.
    if (mfiHWAuth->enabled) {
        nrf_drv_twi_disable(&mfiHWAuth->twiInstanceID);
        mfiHWAuth->enabled = false;
    }
    nrf_drv_twi_uninit(&mfiHWAuth->twiInstanceID);

    // Enter power-saving.
    nrf_gpio_cfg_output(mfiHWAuth->sclPin);
    nrf_gpio_pin_clear(mfiHWAuth->sclPin);
    nrf_gpio_cfg_output(mfiHWAuth->sdaPin);
    nrf_gpio_pin_clear(mfiHWAuth->sdaPin);
    nrf_gpio_pin_clear(mfiHWAuth->vccPin);

    mfiHWAuth->initialized = false;
}

HAP_RESULT_USE_CHECK
bool HAPPlatformMFiHWAuthIsPoweredOn(
    HAPPlatformMFiHWAuthRef mfiHWAuth)
{
    HAPPrecondition(mfiHWAuth);
    HAPPrecondition(mfiHWAuth->initialized);

    return mfiHWAuth->enabled;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformMFiHWAuthPowerOn(
    HAPPlatformMFiHWAuthRef mfiHWAuth)
{
    HAPPrecondition(mfiHWAuth);
    HAPPrecondition(mfiHWAuth->initialized);
    HAPPrecondition(!mfiHWAuth->enabled);

    NRF_GPIO->PIN_CNF[mfiHWAuth->sclPin] = mfiHWAuth->sclConfiguration;
    NRF_GPIO->PIN_CNF[mfiHWAuth->sdaPin] = mfiHWAuth->sdaConfiguration;
    nrf_drv_twi_enable(&mfiHWAuth->twiInstanceID);
    nrf_gpio_pin_set(mfiHWAuth->vccPin);
    nrf_delay_ms(10); // 10ms startup time !!!
    mfiHWAuth->enabled = true;
    return kHAPError_None;
}

void HAPPlatformMFiHWAuthPowerOff(
    HAPPlatformMFiHWAuthRef mfiHWAuth)
{
    HAPPrecondition(mfiHWAuth);
    HAPPrecondition(mfiHWAuth->initialized);
    HAPPrecondition(mfiHWAuth->enabled);

    mfiHWAuth->sclConfiguration = NRF_GPIO->PIN_CNF[mfiHWAuth->sclPin];
    mfiHWAuth->sdaConfiguration = NRF_GPIO->PIN_CNF[mfiHWAuth->sdaPin];
    nrf_drv_twi_disable(&mfiHWAuth->twiInstanceID);
    nrf_gpio_cfg_output(mfiHWAuth->sclPin);
    nrf_gpio_pin_clear(mfiHWAuth->sclPin);
    nrf_gpio_cfg_output(mfiHWAuth->sdaPin);
    nrf_gpio_pin_clear(mfiHWAuth->sdaPin);
    nrf_gpio_pin_clear(mfiHWAuth->vccPin);
    mfiHWAuth->enabled = false;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformMFiHWAuthWrite(
    HAPPlatformMFiHWAuthRef mfiHWAuth,
    const void *bytes,
    size_t numBytes)
{
    HAPPrecondition(mfiHWAuth);
    HAPPrecondition(mfiHWAuth->initialized);
    HAPPrecondition(mfiHWAuth->enabled);
    HAPPrecondition(bytes);
    HAPPrecondition(numBytes >= 1 && numBytes <= 128);

    HAPLogBufferDebug(&logObject, bytes, numBytes, "MFi >");
    for (int repeat = 0; repeat < 1000; repeat++) {
        ret_code_t e = nrf_drv_twi_tx(&mfiHWAuth->twiInstanceID,
            mfiHWAuth->i2cAddress, bytes, (uint8_t) numBytes, /* no_stop: */ false);
        if (e) {
            HAPAssert(
                e == NRF_ERROR_BUSY ||
                e == NRF_ERROR_INTERNAL ||
                e == NRF_ERROR_DRV_TWI_ERR_ANACK ||
                e == NRF_ERROR_DRV_TWI_ERR_DNACK);
            nrf_delay_us(500);
            continue;
        }
        HAPLogDebug(&logObject, "MFi write complete.");
        return kHAPError_None;
    }
    HAPLog(&logObject, "nrf_drv_twi_tx failed too often. Giving up.");
    return kHAPError_Unknown;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformMFiHWAuthRead(
    HAPPlatformMFiHWAuthRef mfiHWAuth,
    uint8_t registerAddress,
    void *bytes,
    size_t numBytes)
{
    HAPPrecondition(mfiHWAuth);
    HAPPrecondition(mfiHWAuth->initialized);
    HAPPrecondition(mfiHWAuth->enabled);
    HAPPrecondition(bytes);
    HAPPrecondition(numBytes >= 1 && numBytes <= 128);

    HAPLogDebug(&logObject, "MFi read 0x%02x.", registerAddress);
    int repeat;
    for (repeat = 0; repeat < 1000; repeat++) {
        ret_code_t e = nrf_drv_twi_tx(&mfiHWAuth->twiInstanceID,
            mfiHWAuth->i2cAddress, &registerAddress, 1, /* no_stop: */ false);
        if (e) {
            HAPAssert(
                e == NRF_ERROR_BUSY ||
                e == NRF_ERROR_INTERNAL ||
                e == NRF_ERROR_DRV_TWI_ERR_ANACK ||
                e == NRF_ERROR_DRV_TWI_ERR_DNACK);
            nrf_delay_us(500);
            continue;
        }
        break;
    }
    for (; repeat < 1000; repeat++) {
        ret_code_t e = nrf_drv_twi_rx(&mfiHWAuth->twiInstanceID,
            mfiHWAuth->i2cAddress, bytes, (uint8_t) numBytes);
        if (e) {
            HAPAssert(
                e == NRF_ERROR_BUSY ||
                e == NRF_ERROR_INTERNAL ||
                e == NRF_ERROR_DRV_TWI_ERR_ANACK ||
                e == NRF_ERROR_DRV_TWI_ERR_DNACK);
            nrf_delay_us(500);
            continue;
        }
        HAPLogBufferDebug(&logObject, bytes, numBytes, "MFi < %02x", registerAddress);
        return kHAPError_None;
    }
    HAPLog(&logObject, "nrf_drv_twi_tx / rx failed too often. Giving up.");
    return kHAPError_Unknown;
}
