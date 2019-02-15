/*=========================================================================
| Beagle Interface Library
|--------------------------------------------------------------------------
| Copyright (c) 2004-2011 Total Phase, Inc.
| All rights reserved.
| www.totalphase.com
|
| Redistribution and use in source and binary forms, with or without
| modification, are permitted provided that the following conditions
| are met:
|
| - Redistributions of source code must retain the above copyright
|   notice, this list of conditions and the following disclaimer.
|
| - Redistributions in binary form must reproduce the above copyright
|   notice, this list of conditions and the following disclaimer in the
|   documentation and/or other materials provided with the distribution.
|
| - Neither the name of Total Phase, Inc. nor the names of its
|   contributors may be used to endorse or promote products derived from
|   this software without specific prior written permission.
|
| THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
| "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
| LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
| FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
| COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
| INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
| BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
| LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
| CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
| LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
| ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
| POSSIBILITY OF SUCH DAMAGE.
|--------------------------------------------------------------------------
| To access Total Phase Beagle devices through the API:
|
| 1) Use one of the following shared objects:
|      beagle.so        --  Linux shared object
|          or
|      beagle.dll       --  Windows dynamic link library
|
| 2) Along with one of the following language modules:
|      beagle.c/h       --  C/C++ API header file and interface module
|      beagle_py.py     --  Python API
|      beagle.bas       --  Visual Basic 6 API
|      beagle.cs        --  C# .NET source
|      beagle_net.dll   --  Compiled .NET binding
 ========================================================================*/


#ifndef __beagle_h__
#define __beagle_h__

#ifdef __cplusplus
extern "C" {
#endif


/*=========================================================================
| TYPEDEFS
 ========================================================================*/
#ifndef TOTALPHASE_DATA_TYPES
#define TOTALPHASE_DATA_TYPES

#ifndef _MSC_VER
/* C99-compliant compilers (GCC) */
#include <stdint.h>
typedef uint8_t   u08;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int8_t    s08;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;

#else
/* Microsoft compilers (Visual C++) */
typedef unsigned __int8   u08;
typedef unsigned __int16  u16;
typedef unsigned __int32  u32;
typedef unsigned __int64  u64;
typedef signed   __int8   s08;
typedef signed   __int16  s16;
typedef signed   __int32  s32;
typedef signed   __int64  s64;

#endif /* __MSC_VER */

typedef float   f32;
typedef double  f64;

#endif /* TOTALPHASE_DATA_TYPES */


/*=========================================================================
| DEBUG
 ========================================================================*/
/* Set the following macro to '1' for debugging */
#define BG_DEBUG 0


/*=========================================================================
| VERSION
 ========================================================================*/
#define BG_HEADER_VERSION  0x050a   /* v5.10 */


/*=========================================================================
| STATUS CODES
 ========================================================================*/
/*
 * All API functions return an integer which is the result of the
 * transaction, or a status code if negative.  The status codes are
 * defined as follows:
 */
enum BeagleStatus {
    /* General codes (0 to -99) */
    BG_OK                                          =    0,
    BG_UNABLE_TO_LOAD_LIBRARY                      =   -1,
    BG_UNABLE_TO_LOAD_DRIVER                       =   -2,
    BG_UNABLE_TO_LOAD_FUNCTION                     =   -3,
    BG_INCOMPATIBLE_LIBRARY                        =   -4,
    BG_INCOMPATIBLE_DEVICE                         =   -5,
    BG_INCOMPATIBLE_DRIVER                         =   -6,
    BG_COMMUNICATION_ERROR                         =   -7,
    BG_UNABLE_TO_OPEN                              =   -8,
    BG_UNABLE_TO_CLOSE                             =   -9,
    BG_INVALID_HANDLE                              =  -10,
    BG_CONFIG_ERROR                                =  -11,
    BG_UNKNOWN_PROTOCOL                            =  -12,
    BG_STILL_ACTIVE                                =  -13,
    BG_FUNCTION_NOT_AVAILABLE                      =  -14,
    BG_INVALID_LICENSE                             =  -15,
    BG_CAPTURE_NOT_TRIGGERED                       =  -16,
    BG_CAPTURE_NOT_READY_FOR_DOWNLOAD              =  -17,

    /* COMMTEST codes (-100 to -199) */
    BG_COMMTEST_NOT_AVAILABLE                      = -100,
    BG_COMMTEST_NOT_ENABLED                        = -101,

    /* I2C codes (-200 to -299) */
    BG_I2C_NOT_AVAILABLE                           = -200,
    BG_I2C_NOT_ENABLED                             = -201,

    /* SPI codes (-300 to -399) */
    BG_SPI_NOT_AVAILABLE                           = -300,
    BG_SPI_NOT_ENABLED                             = -301,

    /* USB codes (-400 to -499) */
    BG_USB_NOT_AVAILABLE                           = -400,
    BG_USB_NOT_ENABLED                             = -401,
    BG_USB2_NOT_ENABLED                            = -402,
    BG_USB3_NOT_ENABLED                            = -403,

    /* Cross-Analyzer Sync codes (-410 to -413) */
    BG_CROSS_ANALYZER_SYNC_DISTURBED_RE_ENABLE     = -410,
    BG_CROSS_ANALYZER_SYNC_DISTURBED_RECONNECT     = -411,
    BG_CROSS_ANALYZER_SYNC_UNLICENSED_SELF         = -412,
    BG_CROSS_ANALYZER_SYNC_UNLICENSED_OTHER        = -413,

    /* Complex Triggering Config codes (-450 to -469) */
    BG_COMPLEX_CONFIG_ERROR_NO_STATES              = -450,
    BG_COMPLEX_CONFIG_ERROR_DATA_PACKET_TYPE       = -451,
    BG_COMPLEX_CONFIG_ERROR_DATA_FIELD             = -452,
    BG_COMPLEX_CONFIG_ERROR_ERR_MATCH_FIELD        = -453,
    BG_COMPLEX_CONFIG_ERROR_DATA_RESOURCES         = -454,
    BG_COMPLEX_CONFIG_ERROR_DP_MATCH_TYPE          = -455,
    BG_COMPLEX_CONFIG_ERROR_DP_MATCH_VAL           = -456,
    BG_COMPLEX_CONFIG_ERROR_DP_REQUIRED            = -457,
    BG_COMPLEX_CONFIG_ERROR_DP_RESOURCES           = -458,
    BG_COMPLEX_CONFIG_ERROR_TIMER_UNIT             = -459,
    BG_COMPLEX_CONFIG_ERROR_TIMER_BOUNDS           = -460,
    BG_COMPLEX_CONFIG_ERROR_ASYNC_EVENT            = -461,
    BG_COMPLEX_CONFIG_ERROR_ASYNC_EDGE             = -462,
    BG_COMPLEX_CONFIG_ERROR_ACTION_FILTER          = -463,
    BG_COMPLEX_CONFIG_ERROR_ACTION_GOTO_SEL        = -464,
    BG_COMPLEX_CONFIG_ERROR_ACTION_GOTO_DEST       = -465,
    BG_COMPLEX_CONFIG_ERROR_BAD_VBUS_TRIGGER_TYPE  = -466,
    BG_COMPLEX_CONFIG_ERROR_BAD_VBUS_TRIGGER_THRES = -467,
    BG_COMPLEX_CONFIG_ERROR_NO_MULTI_VBUS_TRIGGERS = -468,
    BG_COMPLEX_CONFIG_ERROR_IV_MONITOR_NOT_ENABLED = -469,

    /* MDIO codes (-500 to -599) */
    BG_MDIO_NOT_AVAILABLE                          = -500,
    BG_MDIO_NOT_ENABLED                            = -501,
    BG_MDIO_BAD_TURNAROUND                         = -502,

    /* IV MON codes (-600 to -699) */
    BG_IV_MON_NULL_PACKET                          = -600,
    BG_IV_MON_INVALID_PACKET_LENGTH                = -601
};
#ifndef __cplusplus
typedef enum BeagleStatus BeagleStatus;
#endif


/*=========================================================================
| GENERAL TYPE DEFINITIONS
 ========================================================================*/
/* Beagle handle type definition */
typedef int Beagle;

/*
 * Beagle version matrix.
 *
 * This matrix describes the various version dependencies
 * of Beagle components.  It can be used to determine
 * which component caused an incompatibility error.
 *
 * All version numbers are of the format:
 *   (major << 8) | minor
 *
 * ex. v1.20 would be encoded as:  0x0114
 */
struct BeagleVersion {
    /* Software, firmware, and hardware versions. */
    u16 software;
    u16 firmware;
    u16 hardware;

    /*
     * Hardware revisions that are compatible with this software version.
     * The top 16 bits gives the maximum accepted hw revision.
     * The lower 16 bits gives the minimum accepted hw revision.
     */
    u32 hw_revs_for_sw;

    /*
     * Firmware revisions that are compatible with this software version.
     * The top 16 bits gives the maximum accepted fw revision.
     * The lower 16 bits gives the minimum accepted fw revision.
     */
    u32 fw_revs_for_sw;

    /*
     * Driver revisions that are compatible with this software version.
     * The top 16 bits gives the maximum accepted driver revision.
     * The lower 16 bits gives the minimum accepted driver revision.
     * This version checking is currently only pertinent for WIN32
     * platforms.
     */
    u32 drv_revs_for_sw;

    /* Software requires that the API interface must be >= this version. */
    u16 api_req_by_sw;
};
#ifndef __cplusplus
typedef struct BeagleVersion BeagleVersion;
#endif


/*=========================================================================
| GENERAL API
 ========================================================================*/
/*
 * Get a list of ports to which Beagle devices are attached.
 *
 * num_devices = maximum number of elements to return
 * devices     = array into which the port numbers are returned
 *
 * Each element of the array is written with the port number.
 * Devices that are in-use are ORed with BG_PORT_NOT_FREE
 * (0x8000).
 *
 * ex.  devices are attached to ports 0, 1, 2
 *      ports 0 and 2 are available, and port 1 is in-use.
 *      array => 0x0000, 0x8001, 0x0002
 *
 * If the array is NULL, it is not filled with any values.
 * If there are more devices than the array size, only the
 * first nmemb port numbers will be written into the array.
 *
 * Returns the number of devices found, regardless of the
 * array size.
 */
#define BG_PORT_NOT_FREE 0x8000
int bg_find_devices (
    int   num_devices,
    u16 * devices
);


/*
 * Get a list of ports to which Beagle devices are attached
 *
 * This function is the same as bg_find_devices() except that
 * it returns the unique IDs of each Beagle device.  The IDs
 * are guaranteed to be non-zero if valid.
 *
 * The IDs are the unsigned integer representation of the 10-digit
 * serial numbers.
 */
int bg_find_devices_ext (
    int   num_devices,
    u16 * devices,
    int   num_ids,
    u32 * unique_ids
);


/*
 * Open the Beagle port.
 *
 * The port number is a zero-indexed integer.
 *
 * The port number is the same as that obtained from the
 * bg_find_devices() function above.
 *
 * Returns an Beagle handle, which is guaranteed to be
 * greater than zero if it is valid.
 *
 * This function is recommended for use in simple applications
 * where extended information is not required.  For more complex
 * applications, the use of bg_open_ext() is recommended.
 */
Beagle bg_open (
    int port_number
);


/*
 * Open the Beagle port, returning extended information
 * in the supplied structure.  Behavior is otherwise identical
 * to bg_open() above.  If 0 is passed as the pointer to the
 * structure, this function is exactly equivalent to bg_open().
 *
 * The structure is zeroed before the open is attempted.
 * It is filled with whatever information is available.
 *
 * For example, if the hardware version is not filled, then
 * the device could not be queried for its version number.
 *
 * This function is recommended for use in complex applications
 * where extended information is required.  For more simple
 * applications, the use of bg_open() is recommended.
 */
struct BeagleExt {
    /* Version matrix */
    BeagleVersion   version;

    /* Features of this device. */
    int             features;
};
#ifndef __cplusplus
typedef struct BeagleExt BeagleExt;
#endif

Beagle bg_open_ext (
    int         port_number,
    BeagleExt * bg_ext
);


/* Close the Beagle port. */
int bg_close (
    Beagle beagle
);


/*
 * Return the port for this Beagle handle.
 *
 * The port number is a zero-indexed integer.
 */
int bg_port (
    Beagle beagle
);


/*
 * Return the device features as a bit-mask of values, or
 * an error code if the handle is not valid.
 */
#define BG_FEATURE_NONE 0x00000000
#define BG_FEATURE_I2C 0x00000001
#define BG_FEATURE_SPI 0x00000002
#define BG_FEATURE_USB 0x00000004
#define BG_FEATURE_MDIO 0x00000008
#define BG_FEATURE_USB_HS 0x00000010
#define BG_FEATURE_USB_SS 0x00000020
int bg_features (
    Beagle beagle
);


int bg_unique_id_to_features (
    u32 unique_id
);


/*
 * Return the unique ID for this Beagle adapter.
 * IDs are guaranteed to be non-zero if valid.
 * The ID is the unsigned integer representation of the
 * 10-digit serial number.
 */
u32 bg_unique_id (
    Beagle beagle
);


/*
 * Return the status string for the given status code.
 * If the code is not valid or the library function cannot
 * be loaded, return a NULL string.
 */
const char * bg_status_string (
    int status
);


/*
 * Return the version matrix for the device attached to the
 * given handle.  If the handle is 0 or invalid, only the
 * software and required api versions are set.
 */
int bg_version (
    Beagle          beagle,
    BeagleVersion * version
);


/*
 * Set the capture latency to the specified number of milliseconds.
 * This number determines the minimum time that a read call will
 * block if there is no available data.  Lower times result in
 * faster turnaround at the expense of reduced buffering.  Setting
 * this parameter too low can cause packets to be dropped.
 */
int bg_latency (
    Beagle beagle,
    u32    milliseconds
);


/*
 * Set the capture timeout to the specified number of milliseconds.
 * If any read call has a longer idle than this value, that call
 * will return with a count of 0 bytes.
 */
int bg_timeout (
    Beagle beagle,
    u32    milliseconds
);


/*
 * Sleep for the specified number of milliseconds
 * Accuracy depends on the operating system scheduler
 * Returns the number of milliseconds slept
 */
u32 bg_sleep_ms (
    u32 milliseconds
);


/* Configure the target power pin. */
#define BG_TARGET_POWER_OFF 0x00
#define BG_TARGET_POWER_ON 0x01
#define BG_TARGET_POWER_QUERY 0x80
int bg_target_power (
    Beagle beagle,
    u08    power_flag
);


#define BG_HOST_IFCE_FULL_SPEED 0x00
#define BG_HOST_IFCE_HIGH_SPEED 0x01
#define BG_HOST_IFCE_SUPER_SPEED 0x02
int bg_host_ifce_speed (
    Beagle beagle
);


/* Returns the device address that the beagle is attached to. */
int bg_dev_addr (
    Beagle beagle
);



/*=========================================================================
| BUFFERING API
 ==========================================================================
| Set the amount of buffering that is to be allocated on the PC.
| Pass zero to num_bytes to query the existing buffer siz*/
int bg_host_buffer_size (
    Beagle beagle,
    u32    num_bytes
);


/* Query the amount of buffering that is unused and free for buffering. */
int bg_host_buffer_free (
    Beagle beagle
);


/* Query the amount of buffering that is used and no longer available. */
int bg_host_buffer_used (
    Beagle beagle
);


/* Benchmark the speed of the host to Beagle interface */
int bg_commtest (
    Beagle beagle,
    int    num_samples,
    int    delay_count
);



/*=========================================================================
| MONITORING API
 ========================================================================*/
/* Protocol codes */
enum BeagleProtocol {
    BG_PROTOCOL_NONE     = 0,
    BG_PROTOCOL_COMMTEST = 1,
    BG_PROTOCOL_USB      = 2,
    BG_PROTOCOL_I2C      = 3,
    BG_PROTOCOL_SPI      = 4,
    BG_PROTOCOL_MDIO     = 5
};
#ifndef __cplusplus
typedef enum BeagleProtocol BeagleProtocol;
#endif

/*
 * Common Beagle read status codes
 * PARTIAL_LAST_BYTE Unused by USB 480 and 5000
 */
#define BG_READ_OK 0x00000000
#define BG_READ_TIMEOUT 0x00000100
#define BG_READ_ERR_MIDDLE_OF_PACKET 0x00000200
#define BG_READ_ERR_SHORT_BUFFER 0x00000400
#define BG_READ_ERR_PARTIAL_LAST_BYTE 0x00000800
#define BG_READ_ERR_PARTIAL_LAST_BYTE_MASK 0x0000000f
#define BG_READ_ERR_UNEXPECTED 0x00001000
/* Enable the Beagle monitor */
int bg_enable (
    Beagle         beagle,
    BeagleProtocol protocol
);


/* Disable the Beagle monitor */
int bg_disable (
    Beagle beagle
);


/*
 * Capture stop function only supported for analyzers with
 * on-board triggering capability.  For other analyzers, use
 * bg_disable to stop the capture and download to PC.
 */
int bg_capture_stop (
    Beagle beagle
);


int bg_capture_trigger (
    Beagle beagle
);


/*
 * Capture status; general across protocols but used in
 * protocol-specific capture-status-query functions as well
 */
enum BeagleCaptureStatus {
    BG_CAPTURE_STATUS_UNKNOWN          = -1,
    BG_CAPTURE_STATUS_INACTIVE         =  0,
    BG_CAPTURE_STATUS_SYNC_STANDBY     =  1,
    BG_CAPTURE_STATUS_PRE_TRIGGER      =  2,
    BG_CAPTURE_STATUS_PRE_TRIGGER_SYNC =  3,
    BG_CAPTURE_STATUS_POST_TRIGGER     =  4,
    BG_CAPTURE_STATUS_TRANSFER         =  5,
    BG_CAPTURE_STATUS_COMPLETE         =  6
};
#ifndef __cplusplus
typedef enum BeagleCaptureStatus BeagleCaptureStatus;
#endif

int bg_capture_trigger_wait (
    Beagle                beagle,
    u32                   timeout_ms,
    BeagleCaptureStatus * status
);


/* Set the sample rate in kilohertz. */
int bg_samplerate (
    Beagle beagle,
    int    samplerate_khz
);


/*
 * Get the number of bits for the given number of bytes in the
 * given protocol.
 * Use this to determine how large a bit_timing array to allocate
 * for bg_*_read_bit_timing functions.
 */
int bg_bit_timing_size (
    BeagleProtocol protocol,
    int            num_data_bytes
);



/*=========================================================================
| I2C API
 ========================================================================*/
/* Configure the I2C pullup resistors. */
#define BG_I2C_PULLUP_OFF 0x00
#define BG_I2C_PULLUP_ON 0x01
#define BG_I2C_PULLUP_QUERY 0x80
int bg_i2c_pullup (
    Beagle beagle,
    u08    pullup_flag
);


#define BG_I2C_MONITOR_DATA 0x00ff
#define BG_I2C_MONITOR_NACK 0x0100
#define BG_READ_I2C_NO_STOP 0x00010000
int bg_i2c_read (
    Beagle beagle,
    u32 *  status,
    u64 *  time_sop,
    u64 *  time_duration,
    u32 *  time_dataoffset,
    int    max_bytes,
    u16 *  data_in
);


int bg_i2c_read_data_timing (
    Beagle beagle,
    u32 *  status,
    u64 *  time_sop,
    u64 *  time_duration,
    u32 *  time_dataoffset,
    int    max_bytes,
    u16 *  data_in,
    int    max_timing,
    u32 *  data_timing
);


int bg_i2c_read_bit_timing (
    Beagle beagle,
    u32 *  status,
    u64 *  time_sop,
    u64 *  time_duration,
    u32 *  time_dataoffset,
    int    max_bytes,
    u16 *  data_in,
    int    max_timing,
    u32 *  bit_timing
);



/*=========================================================================
| SPI API
 ========================================================================*/
enum BeagleSpiSSPolarity {
    BG_SPI_SS_ACTIVE_LOW  = 0,
    BG_SPI_SS_ACTIVE_HIGH = 1
};
#ifndef __cplusplus
typedef enum BeagleSpiSSPolarity BeagleSpiSSPolarity;
#endif

enum BeagleSpiSckSamplingEdge {
    BG_SPI_SCK_SAMPLING_EDGE_RISING  = 0,
    BG_SPI_SCK_SAMPLING_EDGE_FALLING = 1
};
#ifndef __cplusplus
typedef enum BeagleSpiSckSamplingEdge BeagleSpiSckSamplingEdge;
#endif

enum BeagleSpiBitorder {
    BG_SPI_BITORDER_MSB = 0,
    BG_SPI_BITORDER_LSB = 1
};
#ifndef __cplusplus
typedef enum BeagleSpiBitorder BeagleSpiBitorder;
#endif

int bg_spi_configure (
    Beagle                   beagle,
    BeagleSpiSSPolarity      ss_polarity,
    BeagleSpiSckSamplingEdge sck_sampling_edge,
    BeagleSpiBitorder        bitorder
);


int bg_spi_read (
    Beagle beagle,
    u32 *  status,
    u64 *  time_sop,
    u64 *  time_duration,
    u32 *  time_dataoffset,
    int    mosi_max_bytes,
    u08 *  data_mosi,
    int    miso_max_bytes,
    u08 *  data_miso
);


int bg_spi_read_data_timing (
    Beagle beagle,
    u32 *  status,
    u64 *  time_sop,
    u64 *  time_duration,
    u32 *  time_dataoffset,
    int    mosi_max_bytes,
    u08 *  data_mosi,
    int    miso_max_bytes,
    u08 *  data_miso,
    int    max_timing,
    u32 *  data_timing
);


int bg_spi_read_bit_timing (
    Beagle beagle,
    u32 *  status,
    u64 *  time_sop,
    u64 *  time_duration,
    u32 *  time_dataoffset,
    int    mosi_max_bytes,
    u08 *  data_mosi,
    int    miso_max_bytes,
    u08 *  data_miso,
    int    max_timing,
    u32 *  bit_timing
);



/*=========================================================================
| USB API
 ========================================================================*/
/* USB packet PID definitions */
#define BG_USB_PID_OUT 0xe1
#define BG_USB_PID_IN 0x69
#define BG_USB_PID_SOF 0xa5
#define BG_USB_PID_SETUP 0x2d
#define BG_USB_PID_DATA0 0xc3
#define BG_USB_PID_DATA1 0x4b
#define BG_USB_PID_DATA2 0x87
#define BG_USB_PID_MDATA 0x0f
#define BG_USB_PID_ACK 0xd2
#define BG_USB_PID_NAK 0x5a
#define BG_USB_PID_STALL 0x1e
#define BG_USB_PID_NYET 0x96
#define BG_USB_PID_PRE 0x3c
#define BG_USB_PID_ERR 0x3c
#define BG_USB_PID_SPLIT 0x78
#define BG_USB_PID_PING 0xb4
#define BG_USB_PID_EXT 0xf0
#define BG_USB_PID_CORRUPTED 0xff
/* The following codes are returned for USB 12, 480, and 5000 captures */
#define BG_READ_USB_ERR_BAD_SIGNALS 0x00010000
#define BG_READ_USB_ERR_BAD_PID 0x00200000
#define BG_READ_USB_ERR_BAD_CRC 0x00400000
/* The following codes are only returned for USB 12 captures */
#define BG_READ_USB_ERR_BAD_SYNC 0x00020000
#define BG_READ_USB_ERR_BIT_STUFF 0x00040000
#define BG_READ_USB_ERR_FALSE_EOP 0x00080000
#define BG_READ_USB_ERR_LONG_EOP 0x00100000
/* The following codes are only returned for USB 480 and 5000  captures */
#define BG_READ_USB_TRUNCATION_LEN_MASK 0x000000ff
#define BG_READ_USB_TRUNCATION_MODE 0x20000000
#define BG_READ_USB_END_OF_CAPTURE 0x40000000
/* The following codes are only returned for USB 5000 captures */
#define BG_READ_USB_ERR_BAD_SLC_CRC_1 0x00008000
#define BG_READ_USB_ERR_BAD_SLC_CRC_2 0x00010000
#define BG_READ_USB_ERR_BAD_SHP_CRC_16 0x00008000
#define BG_READ_USB_ERR_BAD_SHP_CRC_5 0x00010000
#define BG_READ_USB_ERR_BAD_SDP_CRC 0x00008000
#define BG_READ_USB_EDB_FRAMING 0x00020000
#define BG_READ_USB_ERR_UNK_END_OF_FRAME 0x00040000
#define BG_READ_USB_ERR_DATA_LEN_INVALID 0x00080000
#define BG_READ_USB_PKT_TYPE_LINK 0x00100000
#define BG_READ_USB_PKT_TYPE_HDR 0x00200000
#define BG_READ_USB_PKT_TYPE_DP 0x00400000
#define BG_READ_USB_PKT_TYPE_TSEQ 0x00800000
#define BG_READ_USB_PKT_TYPE_TS1 0x01000000
#define BG_READ_USB_PKT_TYPE_TS2 0x02000000
#define BG_READ_USB_ERR_BAD_TS 0x04000000
#define BG_READ_USB_ERR_FRAMING 0x08000000
/* The following events are returned for USB 12, 480, and 5000 captures */
#define BG_EVENT_USB_HOST_DISCONNECT 0x00000100
#define BG_EVENT_USB_TARGET_DISCONNECT 0x00000200
#define BG_EVENT_USB_HOST_CONNECT 0x00000400
#define BG_EVENT_USB_TARGET_CONNECT 0x00000800
#define BG_EVENT_USB_RESET 0x00001000
/*
 * USB 480 and 5000 specific event codes
 * USB 2.0
 */
#define BG_EVENT_USB_DIGITAL_INPUT_MASK 0x0000000f
#define BG_EVENT_USB_CHIRP_J 0x00002000
#define BG_EVENT_USB_CHIRP_K 0x00004000
#define BG_EVENT_USB_SPEED_UNKNOWN 0x00008000
#define BG_EVENT_USB_LOW_SPEED 0x00010000
#define BG_EVENT_USB_FULL_SPEED 0x00020000
#define BG_EVENT_USB_HIGH_SPEED 0x00040000
#define BG_EVENT_USB_LOW_OVER_FULL_SPEED 0x00080000
#define BG_EVENT_USB_SUSPEND 0x00100000
#define BG_EVENT_USB_RESUME 0x00200000
#define BG_EVENT_USB_KEEP_ALIVE 0x00400000
#define BG_EVENT_USB_DIGITAL_INPUT 0x00800000
#define BG_EVENT_USB_OTG_HNP 0x01000000
#define BG_EVENT_USB_OTG_SRP_DATA_PULSE 0x02000000
#define BG_EVENT_USB_OTG_SRP_VBUS_PULSE 0x04000000
/*
 * USB 5000 specific event codes
 * USB 2.0
 */
#define BG_EVENT_USB_SMA_EXTIN_DETECTED 0x08000000
#define BG_EVENT_USB_CHIRP_DETECTED 0x00000080
/* USB 2.0 and USB 3.0 Trigger Event information */
#define BG_EVENT_USB_VBUS_TRIGGER 0x08000000
#define BG_EVENT_USB_COMPLEX_TIMER 0x10000000
#define BG_EVENT_USB_CROSS_TRIGGER 0x20000000
#define BG_EVENT_USB_COMPLEX_TRIGGER 0x40000000
#define BG_EVENT_USB_TRIGGER 0x80000000
#define BG_EVENT_USB_TRIGGER_STATE_MASK 0x00000070
#define BG_EVENT_USB_TRIGGER_STATE_SHIFT 4
#define BG_EVENT_USB_TRIGGER_STATE_0 0x00000000
#define BG_EVENT_USB_TRIGGER_STATE_1 0x00000010
#define BG_EVENT_USB_TRIGGER_STATE_2 0x00000020
#define BG_EVENT_USB_TRIGGER_STATE_3 0x00000030
#define BG_EVENT_USB_TRIGGER_STATE_4 0x00000040
#define BG_EVENT_USB_TRIGGER_STATE_5 0x00000050
#define BG_EVENT_USB_TRIGGER_STATE_6 0x00000060
#define BG_EVENT_USB_TRIGGER_STATE_7 0x00000070
/* USB 3.0 US and DS, and ASYNC streams */
#define BG_EVENT_USB_LFPS 0x00001000
#define BG_EVENT_USB_LTSSM 0x00002000
#define BG_EVENT_USB_VBUS_PRESENT 0x00010000
#define BG_EVENT_USB_VBUS_ABSENT 0x00020000
#define BG_EVENT_USB_SCRAMBLING_ENABLED 0x00040000
#define BG_EVENT_USB_SCRAMBLING_DISABLED 0x00080000
#define BG_EVENT_USB_POLARITY_NORMAL 0x00100000
#define BG_EVENT_USB_POLARITY_REVERSED 0x00200000
#define BG_EVENT_USB_PHY_ERROR 0x00400000
#define BG_EVENT_USB_LTSSM_MASK 0x000000ff
#define BG_EVENT_USB_LTSSM_STATE_UNKNOWN 0x00000000
#define BG_EVENT_USB_LTSSM_STATE_SS_DISABLED 0x00000001
#define BG_EVENT_USB_LTSSM_STATE_SS_INACTIVE 0x00000002
#define BG_EVENT_USB_LTSSM_STATE_RX_DETECT_RESET 0x00000003
#define BG_EVENT_USB_LTSSM_STATE_RX_DETECT_ACTIVE 0x00000004
#define BG_EVENT_USB_LTSSM_STATE_POLLING_LFPS 0x00000005
#define BG_EVENT_USB_LTSSM_STATE_POLLING_RXEQ 0x00000006
#define BG_EVENT_USB_LTSSM_STATE_POLLING_ACTIVE 0x00000007
#define BG_EVENT_USB_LTSSM_STATE_POLLING_CONFIG 0x00000008
#define BG_EVENT_USB_LTSSM_STATE_POLLING_IDLE 0x00000009
#define BG_EVENT_USB_LTSSM_STATE_U0 0x0000000a
#define BG_EVENT_USB_LTSSM_STATE_U1 0x0000000b
#define BG_EVENT_USB_LTSSM_STATE_U2 0x0000000c
#define BG_EVENT_USB_LTSSM_STATE_U3 0x0000000d
#define BG_EVENT_USB_LTSSM_STATE_RECOVERY_ACTIVE 0x0000000e
#define BG_EVENT_USB_LTSSM_STATE_RECOVERY_CONFIG 0x0000000f
#define BG_EVENT_USB_LTSSM_STATE_RECOVERY_IDLE 0x00000010
#define BG_EVENT_USB_LTSSM_STATE_HOT_RESET_ACTIVE 0x00000011
#define BG_EVENT_USB_LTSSM_STATE_HOT_RESET_EXIT 0x00000012
#define BG_EVENT_USB_LTSSM_STATE_LOOPBACK_ACTIVE 0x00000013
#define BG_EVENT_USB_LTSSM_STATE_LOOPBACK_EXIT 0x00000014
#define BG_EVENT_USB_LTSSM_STATE_COMPLIANCE 0x00000015
#define BG_EVENT_USB_SMA_EXTIN_ASSERTED 0x01000000
#define BG_EVENT_USB_SMA_EXTIN_DEASSERTED 0x02000000
#define BG_EVENT_USB_TRIGGER_5GBIT_START 0x04000000
#define BG_EVENT_USB_TRIGGER_5GBIT_STOP 0x08000000
/* Beagle USB feature bits */
#define BG_USB_FEATURE_NONE 0x00000000
#define BG_USB_FEATURE_USB2_MON 0x00000001
#define BG_USB_FEATURE_USB3_MON 0x00000002
#define BG_USB_FEATURE_SIMUL_23 0x00000004
#define BG_USB_FEATURE_USB3_CMP_TRIG 0x00000008
#define BG_USB_FEATURE_USB3_4G_MEM 0x00000020
#define BG_USB_FEATURE_USB2_CMP_TRIG 0x00000080
#define BG_USB_FEATURE_CROSS_ANALYZER_SYNC 0x00000100
#define BG_USB_FEATURE_USB3_DOWNLINK 0x00000200
#define BG_USB_FEATURE_IV_MON_LITE 0x00000400
int bg_usb_features (
    Beagle beagle
);


/* License constants */
#define BG_USB_LICENSE_LENGTH 60
/*
 * Read the license key string and return the features
 * Length must be set to BG_USB_LICENSE_LENGTH in order
 * for license_key to be populated.
 */
int bg_usb_license_read (
    Beagle beagle,
    int    length,
    u08 *  license_key
);


/*
 * Write the license key string and return the features
 * Length must be set to BG_USB_LICENSE_LENGTH.  If
 * the license is not valid or the length is not set to
 * BG_USB_LICENSE_LENGTH, an invalid license error is
 * returned.
 */
int bg_usb_license_write (
    Beagle      beagle,
    int         length,
    const u08 * license_key
);


/* Capture modes */
#define BG_USB_CAPTURE_USB3 0x01
#define BG_USB_CAPTURE_USB2 0x02
#define BG_USB_CAPTURE_IV_MON_LITE 0x08
/* Trigger modes */
enum BeagleUsbTriggerMode {
    BG_USB_TRIGGER_MODE_EVENT     = 0,
    BG_USB_TRIGGER_MODE_IMMEDIATE = 1
};
#ifndef __cplusplus
typedef enum BeagleUsbTriggerMode BeagleUsbTriggerMode;
#endif

int bg_usb_configure (
    Beagle               beagle,
    u08                  cap_mask,
    BeagleUsbTriggerMode trigger_mode
);


/* USB Target Power */
enum BeagleUsbTargetPower {
    BG_USB_TARGET_POWER_HOST_SUPPLIED = 0,
    BG_USB_TARGET_POWER_OFF           = 1
};
#ifndef __cplusplus
typedef enum BeagleUsbTargetPower BeagleUsbTargetPower;
#endif

int bg_usb_target_power (
    Beagle               beagle,
    BeagleUsbTargetPower power_flag
);


/* USB 2 Configuration */
/* USB 2 Capture modes */
enum BeagleUsb2CaptureMode {
    BG_USB2_CAPTURE_REALTIME                 = 0,
    BG_USB2_CAPTURE_REALTIME_WITH_PROTECTION = 1,
    BG_USB2_CAPTURE_DELAYED_DOWNLOAD         = 2
};
#ifndef __cplusplus
typedef enum BeagleUsb2CaptureMode BeagleUsb2CaptureMode;
#endif

int bg_usb2_capture_config (
    Beagle                beagle,
    BeagleUsb2CaptureMode capture_mode
);


/* Target configs */
#define BG_USB2_AUTO_SPEED_DETECT 0
#define BG_USB2_LOW_SPEED 1
#define BG_USB2_FULL_SPEED 2
#define BG_USB2_HIGH_SPEED 3
#define BG_USB2_VBUS_OVERRIDE 0x00000080
int bg_usb2_target_config (
    Beagle beagle,
    u32    target_config
);


/* General constants */
#define BG_USB_CAPTURE_SIZE_INFINITE 0
#define BG_USB_CAPTURE_SIZE_SCALE 0xffffffff
/* USB 2 Capture modes */
int bg_usb2_capture_buffer_config (
    Beagle beagle,
    u32    pretrig_kb,
    u32    capture_kb
);


int bg_usb2_capture_buffer_config_query (
    Beagle beagle,
    u32 *  pretrig_kb,
    u32 *  capture_kb
);


int bg_usb2_capture_status (
    Beagle                beagle,
    BeagleCaptureStatus * status,
    u32 *                 pretrig_remaining_kb,
    u32 *                 pretrig_total_kb,
    u32 *                 capture_remaining_kb,
    u32 *                 capture_total_kb
);


/* Digital output configuration */
#define BG_USB2_DIGITAL_OUT_ENABLE_PIN1 0x01
#define BG_USB2_DIGITAL_OUT_PIN1_ACTIVE_HIGH 0x01
#define BG_USB2_DIGITAL_OUT_PIN1_ACTIVE_LOW 0x00
#define BG_USB2_DIGITAL_OUT_ENABLE_PIN2 0x02
#define BG_USB2_DIGITAL_OUT_PIN2_ACTIVE_HIGH 0x02
#define BG_USB2_DIGITAL_OUT_PIN2_ACTIVE_LOW 0x00
#define BG_USB2_DIGITAL_OUT_ENABLE_PIN3 0x04
#define BG_USB2_DIGITAL_OUT_PIN3_ACTIVE_HIGH 0x04
#define BG_USB2_DIGITAL_OUT_PIN3_ACTIVE_LOW 0x00
#define BG_USB2_DIGITAL_OUT_ENABLE_PIN4 0x08
#define BG_USB2_DIGITAL_OUT_PIN4_ACTIVE_HIGH 0x08
#define BG_USB2_DIGITAL_OUT_PIN4_ACTIVE_LOW 0x00
int bg_usb2_digital_out_config (
    Beagle beagle,
    u08    out_enable_mask,
    u08    out_polarity_mask
);


/* Digital output match pin configuration */
enum BeagleUsb2DigitalOutMatchPins {
    BG_USB2_DIGITAL_OUT_MATCH_PIN3 = 3,
    BG_USB2_DIGITAL_OUT_MATCH_PIN4 = 4
};
#ifndef __cplusplus
typedef enum BeagleUsb2DigitalOutMatchPins BeagleUsb2DigitalOutMatchPins;
#endif

/* Packet matching modes */
enum BeagleUsb2MatchType {
    BG_USB2_MATCH_TYPE_DISABLED  = 0,
    BG_USB2_MATCH_TYPE_EQUAL     = 1,
    BG_USB2_MATCH_TYPE_NOT_EQUAL = 2
};
#ifndef __cplusplus
typedef enum BeagleUsb2MatchType BeagleUsb2MatchType;
#endif

/* Digital ouput matching configuration */
struct BeagleUsb2PacketMatch {
    BeagleUsb2MatchType pid_match_type;
    u08                 pid_match_val;
    BeagleUsb2MatchType dev_match_type;
    u08                 dev_match_val;
    BeagleUsb2MatchType ep_match_type;
    u08                 ep_match_val;
};
#ifndef __cplusplus
typedef struct BeagleUsb2PacketMatch BeagleUsb2PacketMatch;
#endif

/* Data match PID mask */
#define BG_USB2_DATA_MATCH_DATA0 0x01
#define BG_USB2_DATA_MATCH_DATA1 0x02
#define BG_USB2_DATA_MATCH_DATA2 0x04
#define BG_USB2_DATA_MATCH_MDATA 0x08
struct BeagleUsb2DataMatch {
    BeagleUsb2MatchType data_match_type;
    u08                 data_match_pid;
    u16                 data_length;
    u08 *               data;
    u16                 data_valid_length;
    u08 *               data_valid;
};
#ifndef __cplusplus
typedef struct BeagleUsb2DataMatch BeagleUsb2DataMatch;
#endif

int bg_usb2_digital_out_match (
    Beagle                        beagle,
    BeagleUsb2DigitalOutMatchPins pin_num,
    const BeagleUsb2PacketMatch * packet_match,
    const BeagleUsb2DataMatch *   data_match
);


/* Digital input pin configuration */
#define BG_USB2_DIGITAL_IN_ENABLE_PIN1 0x01
#define BG_USB2_DIGITAL_IN_ENABLE_PIN2 0x02
#define BG_USB2_DIGITAL_IN_ENABLE_PIN3 0x04
#define BG_USB2_DIGITAL_IN_ENABLE_PIN4 0x08
int bg_usb2_digital_in_config (
    Beagle beagle,
    u08    in_enable_mask
);


/* Hardware filtering configuration */
#define BG_USB2_HW_FILTER_PID_SOF 0x01
#define BG_USB2_HW_FILTER_PID_IN 0x02
#define BG_USB2_HW_FILTER_PID_PING 0x04
#define BG_USB2_HW_FILTER_PID_PRE 0x08
#define BG_USB2_HW_FILTER_PID_SPLIT 0x10
#define BG_USB2_HW_FILTER_SELF 0x20
int bg_usb2_hw_filter_config (
    Beagle beagle,
    u08    filter_enable_mask
);


int bg_usb2_simple_match_config (
    Beagle beagle,
    u08    dig_in_pin_pos_edge_mask,
    u08    dig_in_pin_neg_edge_mask,
    u08    dig_out_match_pin_mask
);


/* USB 2.0 Complex matching enable/disable */
int bg_usb2_complex_match_enable (
    Beagle beagle
);


int bg_usb2_complex_match_disable (
    Beagle beagle
);


enum BeagleUsbMatchType {
    BG_USB_MATCH_TYPE_DISABLED      = 0,
    BG_USB_MATCH_TYPE_EQUAL         = 1,
    BG_USB_MATCH_TYPE_LESS_EQUAL    = 2,
    BG_USB_MATCH_TYPE_GREATER_EQUAL = 3,
    BG_USB_MATCH_TYPE_NOT_EQUAL     = 4
};
#ifndef __cplusplus
typedef enum BeagleUsbMatchType BeagleUsbMatchType;
#endif

enum BeagleUsb2DataMatchDirection {
    BG_USB2_MATCH_DIRECTION_DISABLED  = 0,
    BG_USB2_MATCH_DIRECTION_IN        = 1,
    BG_USB2_MATCH_DIRECTION_OUT_SETUP = 2,
    BG_USB2_MATCH_DIRECTION_SETUP     = 3
};
#ifndef __cplusplus
typedef enum BeagleUsb2DataMatchDirection BeagleUsb2DataMatchDirection;
#endif

struct BeagleUsb2DataProperties {
    BeagleUsb2DataMatchDirection direction;
    BeagleUsbMatchType           ep_match_type;
    u08                          ep_match_val;
    BeagleUsbMatchType           dev_match_type;
    u08                          dev_match_val;
    BeagleUsbMatchType           data_len_match_type;
    u16                          data_len_match_val;
};
#ifndef __cplusplus
typedef struct BeagleUsb2DataProperties BeagleUsb2DataProperties;
#endif

enum BeagleUsb2PacketType {
    BG_USB2_MATCH_PACKET_IN          = 0x0009,
    BG_USB2_MATCH_PACKET_OUT         = 0x0001,
    BG_USB2_MATCH_PACKET_SETUP       = 0x000d,
    BG_USB2_MATCH_PACKET_SOF         = 0x0005,

    BG_USB2_MATCH_PACKET_DATA0       = 0x0003,
    BG_USB2_MATCH_PACKET_DATA1       = 0x000b,
    BG_USB2_MATCH_PACKET_DATA2       = 0x0007,
    BG_USB2_MATCH_PACKET_MDATA       = 0x000f,

    BG_USB2_MATCH_PACKET_ACK         = 0x0002,
    BG_USB2_MATCH_PACKET_NAK         = 0x000a,
    BG_USB2_MATCH_PACKET_STALL       = 0x000e,
    BG_USB2_MATCH_PACKET_NYET        = 0x0006,
    BG_USB2_MATCH_PACKET_PRE         = 0x000c,
    BG_USB2_MATCH_PACKET_ERR         = 0x010c,
    BG_USB2_MATCH_PACKET_SPLIT       = 0x0008,
    BG_USB2_MATCH_PACKET_EXT         = 0x0000,

    BG_USB2_MATCH_PACKET_ANY         = 0x0010,
    BG_USB2_MATCH_PACKET_DATA0_DATA1 = 0x0020,
    BG_USB2_MATCH_PACKET_DATAX       = 0x0040,
    BG_USB2_MATCH_PACKET_SUBPID_MASK = 0x0100,

    BG_USB2_MATCH_PACKET_ERROR       = 0x1000
};
#ifndef __cplusplus
typedef enum BeagleUsb2PacketType BeagleUsb2PacketType;
#endif

enum BeagleUsb2DataMatchPrefix {
    BG_USB2_MATCH_PREFIX_DISABLED     = 0,
    BG_USB2_MATCH_PREFIX_IN           = 1,
    BG_USB2_MATCH_PREFIX_OUT          = 2,
    BG_USB2_MATCH_PREFIX_SETUP        = 3,
    BG_USB2_MATCH_PREFIX_CSPLIT       = 4,
    BG_USB2_MATCH_PREFIX_CSPLIT_IN    = 5,
    BG_USB2_MATCH_PREFIX_SSPLIT_OUT   = 6,
    BG_USB2_MATCH_PREFIX_SSPLIT_SETUP = 7
};
#ifndef __cplusplus
typedef enum BeagleUsb2DataMatchPrefix BeagleUsb2DataMatchPrefix;
#endif

#define BG_USB2_MATCH_HANDSHAKE_MASK_DISABLED 0x00000000
#define BG_USB2_MATCH_HANDSHAKE_MASK_NONE 0x00000001
#define BG_USB2_MATCH_HANDSHAKE_MASK_ACK 0x00000002
#define BG_USB2_MATCH_HANDSHAKE_MASK_NAK 0x00000004
#define BG_USB2_MATCH_HANDSHAKE_MASK_NYET 0x00000008
#define BG_USB2_MATCH_HANDSHAKE_MASK_STALL 0x00000010
enum BeagleUsb2ErrorType {
    BG_USB2_MATCH_CRC_DONT_CARE          =    0,
    BG_USB2_MATCH_CRC_VALID              =    1,
    BG_USB2_MATCH_CRC_INVALID            =    2,
    BG_USB2_MATCH_ERR_MASK_CORRUPTED_PID = 0x10,
    BG_USB2_MATCH_ERR_MASK_CRC           = 0x20,
    BG_USB2_MATCH_ERR_MASK_RXERROR       = 0x40,
    BG_USB2_MATCH_ERR_MASK_JABBER        = 0x80
};
#ifndef __cplusplus
typedef enum BeagleUsb2ErrorType BeagleUsb2ErrorType;
#endif

enum BeagleUsb2MatchModifier {
    BG_USB2_MATCH_MODIFIER_0 = 0,
    BG_USB2_MATCH_MODIFIER_1 = 1,
    BG_USB2_MATCH_MODIFIER_2 = 2,
    BG_USB2_MATCH_MODIFIER_3 = 3
};
#ifndef __cplusplus
typedef enum BeagleUsb2MatchModifier BeagleUsb2MatchModifier;
#endif

#define BG_USB_COMPLEX_MATCH_ACTION_EXTOUT 0x01
#define BG_USB_COMPLEX_MATCH_ACTION_TRIGGER 0x02
#define BG_USB_COMPLEX_MATCH_ACTION_FILTER 0x04
#define BG_USB_COMPLEX_MATCH_ACTION_GOTO 0x08
struct BeagleUsb2DataMatchUnit {
    BeagleUsb2PacketType       packet_type;
    BeagleUsb2DataMatchPrefix  prefix;
    u08                        handshake;
    u16                        data_length;
    u08 *                      data;
    u16                        data_valid_length;
    u08 *                      data_valid;
    BeagleUsb2ErrorType        err_match;
    u08                        data_properties_valid;
    BeagleUsb2DataProperties   data_properties;
    BeagleUsb2MatchModifier    match_modifier;
    u16                        repeat_count;
    u08                        sticky_action;
    u08                        action_mask;
    u08                        goto_selector;
};
#ifndef __cplusplus
typedef struct BeagleUsb2DataMatchUnit BeagleUsb2DataMatchUnit;
#endif

enum BeagleUsbTimerUnit {
    BG_USB_TIMER_UNIT_DISABLED = 0,
    BG_USB_TIMER_UNIT_NS       = 1,
    BG_USB_TIMER_UNIT_US       = 2,
    BG_USB_TIMER_UNIT_MS       = 3,
    BG_USB_TIMER_UNIT_SEC      = 4
};
#ifndef __cplusplus
typedef enum BeagleUsbTimerUnit BeagleUsbTimerUnit;
#endif

struct BeagleUsb2TimerMatchUnit {
    BeagleUsbTimerUnit timer_unit;
    u32                timer_val;
    u08                action_mask;
    u08                goto_selector;
};
#ifndef __cplusplus
typedef struct BeagleUsb2TimerMatchUnit BeagleUsb2TimerMatchUnit;
#endif

enum BeagleUsb2AsyncEventType {
    BG_USB2_COMPLEX_MATCH_EVENT_DIGIN1        =  0,
    BG_USB2_COMPLEX_MATCH_EVENT_DIGIN2        =  1,
    BG_USB2_COMPLEX_MATCH_EVENT_DIGIN3        =  2,
    BG_USB2_COMPLEX_MATCH_EVENT_DIGIN4        =  3,



    BG_USB2_COMPLEX_MATCH_EVENT_CHIRP         = 13,
    BG_USB2_COMPLEX_MATCH_EVENT_SMA_EXTIN     = 14,
    BG_USB2_COMPLEX_MATCH_EVENT_CROSS_TRIGGER = 15,
    BG_USB2_COMPLEX_MATCH_EVENT_VBUS_TRIGGER  = 16
};
#ifndef __cplusplus
typedef enum BeagleUsb2AsyncEventType BeagleUsb2AsyncEventType;
#endif

enum BeagleUsb2VbusTriggerType {
    BG_USB2_VBUS_TRIGGER_TYPE_CURRENT = 1,
    BG_USB2_VBUS_TRIGGER_TYPE_VOLTAGE = 2
};
#ifndef __cplusplus
typedef enum BeagleUsb2VbusTriggerType BeagleUsb2VbusTriggerType;
#endif

struct BeagleUsb2AsyncEventMatchUnit {
    BeagleUsb2AsyncEventType  event_type;
    u08                       edge_mask;
    u16                       repeat_count;
    u08                       sticky_action;
    u08                       action_mask;
    u08                       goto_selector;
    BeagleUsb2VbusTriggerType vbus_trigger_type;
    f32                       vbus_trigger_val;
};
#ifndef __cplusplus
typedef struct BeagleUsb2AsyncEventMatchUnit BeagleUsb2AsyncEventMatchUnit;
#endif

struct BeagleUsb2ComplexMatchState {
    u08                             data_0_valid;
    BeagleUsb2DataMatchUnit         data_0;
    u08                             data_1_valid;
    BeagleUsb2DataMatchUnit         data_1;
    u08                             data_2_valid;
    BeagleUsb2DataMatchUnit         data_2;
    u08                             data_3_valid;
    BeagleUsb2DataMatchUnit         data_3;
    u08                             timer_valid;
    BeagleUsb2TimerMatchUnit        timer;
    u08                             async_valid;
    BeagleUsb2AsyncEventMatchUnit   async;
    u08                             goto_0;
    u08                             goto_1;
    u08                             goto_2;
};
#ifndef __cplusplus
typedef struct BeagleUsb2ComplexMatchState BeagleUsb2ComplexMatchState;
#endif

int bg_usb2_complex_match_config (
    Beagle                              beagle,
    u08                                 validate,
    u08                                 digout,
    const BeagleUsb2ComplexMatchState * state_0,
    const BeagleUsb2ComplexMatchState * state_1,
    const BeagleUsb2ComplexMatchState * state_2,
    const BeagleUsb2ComplexMatchState * state_3,
    const BeagleUsb2ComplexMatchState * state_4,
    const BeagleUsb2ComplexMatchState * state_5,
    const BeagleUsb2ComplexMatchState * state_6,
    const BeagleUsb2ComplexMatchState * state_7
);


int bg_usb2_complex_match_config_single (
    Beagle                              beagle,
    u08                                 validate,
    u08                                 digout,
    const BeagleUsb2ComplexMatchState * state
);


enum BeagleUsbExtoutType {
    BG_USB_EXTOUT_LOW       = 0,
    BG_USB_EXTOUT_HIGH      = 1,
    BG_USB_EXTOUT_POS_PULSE = 2,
    BG_USB_EXTOUT_NEG_PULSE = 3,
    BG_USB_EXTOUT_TOGGLE_0  = 4,
    BG_USB_EXTOUT_TOGGLE_1  = 5
};
#ifndef __cplusplus
typedef enum BeagleUsbExtoutType BeagleUsbExtoutType;
#endif

int bg_usb2_extout_config (
    Beagle              beagle,
    BeagleUsbExtoutType extout_modulation
);


enum BeagleMemoryTestResult {
    BG_USB_MEMORY_TEST_PASS = 0,
    BG_USB_MEMORY_TEST_FAIL = 1
};
#ifndef __cplusplus
typedef enum BeagleMemoryTestResult BeagleMemoryTestResult;
#endif

int bg_usb2_memory_test (
    Beagle beagle
);


/* USB 3 Configuration */
/* USB 3 Capture modes */
int bg_usb3_capture_buffer_config (
    Beagle beagle,
    u32    pretrig_kb,
    u32    capture_kb
);


int bg_usb3_capture_buffer_config_query (
    Beagle beagle,
    u32 *  pretrig_kb,
    u32 *  capture_kb
);


int bg_usb3_capture_status (
    Beagle                beagle,
    BeagleCaptureStatus * status,
    u32 *                 pretrig_remaining_kb,
    u32 *                 pretrig_total_kb,
    u32 *                 capture_remaining_kb,
    u32 *                 capture_total_kb
);


#define BG_USB3_PHY_CONFIG_POLARITY_NON_INVERT 0x00
#define BG_USB3_PHY_CONFIG_POLARITY_INVERT 0x01
#define BG_USB3_PHY_CONFIG_POLARITY_AUTO 0x02
#define BG_USB3_PHY_CONFIG_POLARITY_MASK 0x03
#define BG_USB3_PHY_CONFIG_DESCRAMBLER_ON 0x00
#define BG_USB3_PHY_CONFIG_DESCRAMBLER_OFF 0x04
#define BG_USB3_PHY_CONFIG_DESCRAMBLER_AUTO 0x08
#define BG_USB3_PHY_CONFIG_DESCRAMBLER_MASK 0x0c
#define BG_USB3_PHY_CONFIG_RXTERM_ON 0x00
#define BG_USB3_PHY_CONFIG_RXTERM_OFF 0x10
#define BG_USB3_PHY_CONFIG_RXTERM_AUTO 0x20
#define BG_USB3_PHY_CONFIG_RXTERM_MASK 0x30
int bg_usb3_phy_config (
    Beagle beagle,
    u08    tx,
    u08    rx
);


#define BG_USB3_TRUNCATION_OFF 0x00
#define BG_USB3_TRUNCATION_20 0x01
#define BG_USB3_TRUNCATION_36 0x02
#define BG_USB3_TRUNCATION_68 0x03
int bg_usb3_truncation_mode (
    Beagle beagle,
    u08    tx_truncation_mode,
    u08    rx_truncation_mode
);


/* Channel Configuration */
#define BG_USB3_EQUALIZATION_OFF 0
#define BG_USB3_EQUALIZATION_MIN 1
#define BG_USB3_EQUALIZATION_MOD 2
#define BG_USB3_EQUALIZATION_MAX 3
struct BeagleUsb3Channel {
    u08 input_equalization_short;
    u08 input_equalization_medium;
    u08 input_equalization_long;
    u08 pre_emphasis_short_level;
    u08 pre_emphasis_short_decay;
    u08 pre_emphasis_long_level;
    u08 pre_emphasis_long_decay;
    u08 output_level;
};
#ifndef __cplusplus
typedef struct BeagleUsb3Channel BeagleUsb3Channel;
#endif

int bg_usb3_link_config (
    Beagle                    beagle,
    const BeagleUsb3Channel * tx,
    const BeagleUsb3Channel * rx
);


/* Simple match configuration */
#define BG_USB3_SIMPLE_MATCH_NONE 0x00000000
#define BG_USB3_SIMPLE_MATCH_SSTX_IPS 0x00000001
#define BG_USB3_SIMPLE_MATCH_SSTX_SLC 0x00000002
#define BG_USB3_SIMPLE_MATCH_SSTX_SHP 0x00000004
#define BG_USB3_SIMPLE_MATCH_SSTX_SDP 0x00000008
#define BG_USB3_SIMPLE_MATCH_SSRX_IPS 0x00000010
#define BG_USB3_SIMPLE_MATCH_SSRX_SLC 0x00000020
#define BG_USB3_SIMPLE_MATCH_SSRX_SHP 0x00000040
#define BG_USB3_SIMPLE_MATCH_SSRX_SDP 0x00000080
#define BG_USB3_SIMPLE_MATCH_SSTX_SLC_CRC_5A_CRC_5B 0x00000100
#define BG_USB3_SIMPLE_MATCH_SSTX_SHP_CRC_5 0x00000200
#define BG_USB3_SIMPLE_MATCH_SSTX_SHP_CRC_16 0x00000400
#define BG_USB3_SIMPLE_MATCH_SSTX_SDP_CRC 0x00000800
#define BG_USB3_SIMPLE_MATCH_SSRX_SLC_CRC_5A_CRC_5B 0x00001000
#define BG_USB3_SIMPLE_MATCH_SSRX_SHP_CRC_5 0x00002000
#define BG_USB3_SIMPLE_MATCH_SSRX_SHP_CRC_16 0x00004000
#define BG_USB3_SIMPLE_MATCH_SSRX_SDP_CRC 0x00008000
#define BG_USB3_SIMPLE_MATCH_EVENT_SSTX_LFPS 0x00010000
#define BG_USB3_SIMPLE_MATCH_EVENT_SSTX_POLARITY 0x00020000
#define BG_USB3_SIMPLE_MATCH_EVENT_SSTX_DETECTED 0x00400000
#define BG_USB3_SIMPLE_MATCH_EVENT_SSTX_SCRAMBLE 0x00080000
#define BG_USB3_SIMPLE_MATCH_EVENT_SSRX_LFPS 0x00100000
#define BG_USB3_SIMPLE_MATCH_EVENT_SSRX_POLARITY 0x00200000
#define BG_USB3_SIMPLE_MATCH_EVENT_SSRX_DETECTED 0x00040000
#define BG_USB3_SIMPLE_MATCH_EVENT_SSRX_SCRAMBLE 0x00800000
#define BG_USB3_SIMPLE_MATCH_EVENT_VBUS_PRESENT 0x08000000
#define BG_USB3_SIMPLE_MATCH_EVENT_SSTX_PHYERR 0x10000000
#define BG_USB3_SIMPLE_MATCH_EVENT_SSRX_PHYERR 0x20000000
#define BG_USB3_SIMPLE_MATCH_EVENT_SMA_EXTIN 0x40000000
#define BG_USB_EDGE_RISING 0x01
#define BG_USB_EDGE_PULSE 0x01
#define BG_USB_EDGE_FALLING 0x02
#define BG_USB_EDGE_DEVICE_CHIRP 0x01
#define BG_USB_EDGE_HOST_CHIRP 0x02
enum BeagleUsb3ExtoutMode {
    BG_USB3_EXTOUT_DISABLED     = 0,
    BG_USB3_EXTOUT_TRIGGER_MODE = 1,
    BG_USB3_EXTOUT_EVENTS_MODE  = 2
};
#ifndef __cplusplus
typedef enum BeagleUsb3ExtoutMode BeagleUsb3ExtoutMode;
#endif

enum BeagleUsb3IPSType {
    BG_USB3_IPS_TYPE_DISABLED = 0,
    BG_USB3_IPS_TYPE_TS1      = 1,
    BG_USB3_IPS_TYPE_TS2      = 2,
    BG_USB3_IPS_TYPE_TSEQ     = 3,
    BG_USB3_IPS_TYPE_TSx      = 4,
    BG_USB3_IPS_TYPE_TS_ANY   = 5
};
#ifndef __cplusplus
typedef enum BeagleUsb3IPSType BeagleUsb3IPSType;
#endif

int bg_usb3_simple_match_config (
    Beagle               beagle,
    u32                  trigger_mask,
    u32                  extout_mask,
    BeagleUsb3ExtoutMode extout_mode,
    u08                  extin_edge_mask,
    BeagleUsb3IPSType    tx_ips_type,
    BeagleUsb3IPSType    rx_ips_type
);


/* USB 3.0 Complex matching enable/disable */
int bg_usb3_complex_match_enable (
    Beagle beagle
);


int bg_usb3_complex_match_disable (
    Beagle beagle
);


enum BeagleUsbSource {
    BG_USB_SOURCE_USB3_ASYNC = 0,
    BG_USB_SOURCE_USB3_RX    = 1,
    BG_USB_SOURCE_USB3_TX    = 2,
    BG_USB_SOURCE_USB2       = 3,
    BG_USB_SOURCE_IV_MON     = 4
};
#ifndef __cplusplus
typedef enum BeagleUsbSource BeagleUsbSource;
#endif

struct BeagleUsb3DataProperties {
    BeagleUsbMatchType source_match_type;
    BeagleUsbSource    source_match_val;
    BeagleUsbMatchType ep_match_type;
    u08                ep_match_val;
    BeagleUsbMatchType dev_match_type;
    u08                dev_match_val;
    BeagleUsbMatchType stream_id_match_type;
    u16                stream_id_match_val;
    BeagleUsbMatchType data_len_match_type;
    u16                data_len_match_val;
};
#ifndef __cplusplus
typedef struct BeagleUsb3DataProperties BeagleUsb3DataProperties;
#endif

enum BeagleUsb3PacketType {
    BG_USB3_MATCH_PACKET_SLC         = 0,
    BG_USB3_MATCH_PACKET_SHP         = 1,
    BG_USB3_MATCH_PACKET_SDP         = 2,
    BG_USB3_MATCH_PACKET_SHP_SDP     = 3,
    BG_USB3_MATCH_PACKET_TSx         = 4,
    BG_USB3_MATCH_PACKET_TSEQ        = 5,
    BG_USB3_MATCH_PACKET_ERROR       = 6,
    BG_USB3_MATCH_PACKET_5GBIT_START = 7,
    BG_USB3_MATCH_PACKET_5GBIT_STOP  = 8
};
#ifndef __cplusplus
typedef enum BeagleUsb3PacketType BeagleUsb3PacketType;
#endif

enum BeagleUsb3ErrorType {
    BG_USB3_MATCH_CRC_DONT_CARE    =    0,
    BG_USB3_MATCH_CRC_1_VALID      =    1,
    BG_USB3_MATCH_CRC_2_VALID      =    2,
    BG_USB3_MATCH_CRC_BOTH_VALID   =    3,
    BG_USB3_MATCH_CRC_EITHER_FAIL  =    4,
    BG_USB3_MATCH_CRC_1_FAIL       =    5,
    BG_USB3_MATCH_CRC_2_FAIL       =    6,
    BG_USB3_MATCH_CRC_BOTH_FAIL    =    7,
    BG_USB3_MATCH_ERR_MASK_CRC     = 0x10,
    BG_USB3_MATCH_ERR_MASK_FRAMING = 0x20,
    BG_USB3_MATCH_ERR_MASK_UNKNOWN = 0x40
};
#ifndef __cplusplus
typedef enum BeagleUsb3ErrorType BeagleUsb3ErrorType;
#endif

enum BeagleUsb3MatchModifier {
    BG_USB3_MATCH_MODIFIER_0 = 0,
    BG_USB3_MATCH_MODIFIER_1 = 1,
    BG_USB3_MATCH_MODIFIER_2 = 2,
    BG_USB3_MATCH_MODIFIER_3 = 3
};
#ifndef __cplusplus
typedef enum BeagleUsb3MatchModifier BeagleUsb3MatchModifier;
#endif

struct BeagleUsb3DataMatchUnit {
    BeagleUsb3PacketType       packet_type;
    u16                        data_length;
    u08 *                      data;
    u16                        data_valid_length;
    u08 *                      data_valid;
    BeagleUsb3ErrorType        err_match;
    u08                        data_properties_valid;
    BeagleUsb3DataProperties   data_properties;
    BeagleUsb3MatchModifier    match_modifier;
    u16                        repeat_count;
    u08                        sticky_action;
    u08                        action_mask;
    u08                        goto_selector;
};
#ifndef __cplusplus
typedef struct BeagleUsb3DataMatchUnit BeagleUsb3DataMatchUnit;
#endif

struct BeagleUsb3TimerMatchUnit {
    BeagleUsbTimerUnit timer_unit;
    u32                timer_val;
    u08                action_mask;
    u08                goto_selector;
};
#ifndef __cplusplus
typedef struct BeagleUsb3TimerMatchUnit BeagleUsb3TimerMatchUnit;
#endif

enum BeagleUsb3AsyncEventType {
    BG_USB3_COMPLEX_MATCH_EVENT_SSTX_LFPS     =  0,
    BG_USB3_COMPLEX_MATCH_EVENT_SSTX_POLARITY =  1,
    BG_USB3_COMPLEX_MATCH_EVENT_SSTX_DETECTED =  2,
    BG_USB3_COMPLEX_MATCH_EVENT_SSTX_SCRAMBLE =  3,
    BG_USB3_COMPLEX_MATCH_EVENT_SSRX_LFPS     =  4,
    BG_USB3_COMPLEX_MATCH_EVENT_SSRX_POLARITY =  5,
    BG_USB3_COMPLEX_MATCH_EVENT_SSRX_DETECTED =  6,
    BG_USB3_COMPLEX_MATCH_EVENT_SSRX_SCRAMBLE =  7,
    BG_USB3_COMPLEX_MATCH_EVENT_CROSS_TRIGGER =  8,

    BG_USB3_COMPLEX_MATCH_EVENT_VBUS_PRESENT  = 11,
    BG_USB3_COMPLEX_MATCH_EVENT_SSTX_PHYERR   = 12,
    BG_USB3_COMPLEX_MATCH_EVENT_SSRX_PHYERR   = 13,
    BG_USB3_COMPLEX_MATCH_EVENT_SMA_EXTIN     = 14
};
#ifndef __cplusplus
typedef enum BeagleUsb3AsyncEventType BeagleUsb3AsyncEventType;
#endif

struct BeagleUsb3AsyncEventMatchUnit {
    BeagleUsb3AsyncEventType event_type;
    u08                      edge_mask;
    u16                      repeat_count;
    u08                      sticky_action;
    u08                      action_mask;
    u08                      goto_selector;
};
#ifndef __cplusplus
typedef struct BeagleUsb3AsyncEventMatchUnit BeagleUsb3AsyncEventMatchUnit;
#endif

struct BeagleUsb3ComplexMatchState {
    u08                             tx_data_0_valid;
    BeagleUsb3DataMatchUnit         tx_data_0;
    u08                             tx_data_1_valid;
    BeagleUsb3DataMatchUnit         tx_data_1;
    u08                             tx_data_2_valid;
    BeagleUsb3DataMatchUnit         tx_data_2;
    u08                             rx_data_0_valid;
    BeagleUsb3DataMatchUnit         rx_data_0;
    u08                             rx_data_1_valid;
    BeagleUsb3DataMatchUnit         rx_data_1;
    u08                             rx_data_2_valid;
    BeagleUsb3DataMatchUnit         rx_data_2;
    u08                             timer_valid;
    BeagleUsb3TimerMatchUnit        timer;
    u08                             async_valid;
    BeagleUsb3AsyncEventMatchUnit   async;
    u08                             goto_0;
    u08                             goto_1;
    u08                             goto_2;
};
#ifndef __cplusplus
typedef struct BeagleUsb3ComplexMatchState BeagleUsb3ComplexMatchState;
#endif

/* Complex matching configuration */
int bg_usb3_complex_match_config (
    Beagle                              beagle,
    u08                                 validate,
    u08                                 extout,
    const BeagleUsb3ComplexMatchState * state_0,
    const BeagleUsb3ComplexMatchState * state_1,
    const BeagleUsb3ComplexMatchState * state_2,
    const BeagleUsb3ComplexMatchState * state_3,
    const BeagleUsb3ComplexMatchState * state_4,
    const BeagleUsb3ComplexMatchState * state_5,
    const BeagleUsb3ComplexMatchState * state_6,
    const BeagleUsb3ComplexMatchState * state_7
);


/* Complex matching configuration for a single state */
int bg_usb3_complex_match_config_single (
    Beagle                              beagle,
    u08                                 validate,
    u08                                 extout,
    const BeagleUsb3ComplexMatchState * state
);


/* Extout configuration */
int bg_usb3_ext_io_config (
    Beagle              beagle,
    u08                 extin_enable,
    BeagleUsbExtoutType extout_modulation
);


enum BeagleUsb3MemoryTestType {
    BG_USB3_MEMORY_TEST_FAST =  0,
    BG_USB3_MEMORY_TEST_FULL =  1,
    BG_USB3_MEMORY_TEST_SKIP = -1
};
#ifndef __cplusplus
typedef enum BeagleUsb3MemoryTestType BeagleUsb3MemoryTestType;
#endif

int bg_usb3_memory_test (
    Beagle                   beagle,
    BeagleUsb3MemoryTestType test
);


/* Read functions */
int bg_usb2_read (
    Beagle beagle,
    u32 *  status,
    u32 *  events,
    u64 *  time_sop,
    u64 *  time_duration,
    u32 *  time_dataoffset,
    int    max_bytes,
    u08 *  packet
);


int bg_usb_read (
    Beagle            beagle,
    u32 *             status,
    u32 *             events,
    u64 *             time_sop,
    u64 *             time_duration,
    u32 *             time_dataoffset,
    BeagleUsbSource * source,
    int               max_bytes,
    u08 *             packet,
    int               max_k_bytes,
    u08 *             k_data
);


/* | return / 8 */
int bg_usb2_read_data_timing (
    Beagle beagle,
    u32 *  status,
    u32 *  events,
    u64 *  time_sop,
    u64 *  time_duration,
    u32 *  time_dataoffset,
    int    max_bytes,
    u08 *  packet,
    int    max_timing,
    u32 *  data_timing
);


int bg_usb2_read_bit_timing (
    Beagle beagle,
    u32 *  status,
    u32 *  events,
    u64 *  time_sop,
    u64 *  time_duration,
    u32 *  time_dataoffset,
    int    max_bytes,
    u08 *  packet,
    int    max_timing,
    u32 *  bit_timing
);


int bg_usb2_reconstruct_timing (
    u32         target_config,
    int         num_bytes,
    const u08 * packet,
    int         max_timing,
    u32 *       bit_timing
);


/* Hardware-based Statistics */
struct BeagleUsbStatsConfig {
    u08                auto_config;
    BeagleUsbMatchType source_match_type;
    BeagleUsbSource    source_match_val;
    BeagleUsbMatchType ep_match_type;
    u08                ep_match_val;
    BeagleUsbMatchType dev_match_type;
    u08                dev_match_val;
};
#ifndef __cplusplus
typedef struct BeagleUsbStatsConfig BeagleUsbStatsConfig;
#endif

int bg_usb_stats_config (
    Beagle                       beagle,
    const BeagleUsbStatsConfig * config
);


int bg_usb_stats_config_query (
    Beagle                 beagle,
    BeagleUsbStatsConfig * config
);


int bg_usb_stats_reset (
    Beagle beagle
);


struct BeagleUsb3GenStats {
    u64 link;
    u64 lbad;
    u64 slc_crc5;
    u64 txn;
    u64 lmp;
    u64 lgo_u1;
    u64 lgo_u2;
    u64 lgo_u3;
    u64 dp;
    u64 itp;
    u64 shp_crc16_crc5;
    u64 sdp_crc32;
    u64 slc_frm_err;
    u64 shp_frm_err;
    u64 sdp_end_edb_frm_err;
    u64 iso_ips;
    u64 para_ips;
    u64 carry_1k_dp;
};
#ifndef __cplusplus
typedef struct BeagleUsb3GenStats BeagleUsb3GenStats;
#endif

struct BeagleUsb3ConnStats {
    u64 txn;
    u64 dp;
    u64 ack;
    u64 nrdy;
    u64 erdy;
    u64 retry_ack;
    u64 carry_1k_dp;
};
#ifndef __cplusplus
typedef struct BeagleUsb3ConnStats BeagleUsb3ConnStats;
#endif

struct BeagleUsb2Stats {
    u64 sof;
    u64 carry_1k_data;
    u64 data;
    u64 bad_pid;
    u64 crc16;
    u64 crc5;
    u64 rx_error;
    u64 in_nak;
    u64 ping_nak;
};
#ifndef __cplusplus
typedef struct BeagleUsb2Stats BeagleUsb2Stats;
#endif

struct BeagleUsbStats {
    BeagleUsb3GenStats    usb3_tx_gen;
    BeagleUsb3GenStats    usb3_rx_gen;
    BeagleUsb3ConnStats   usb3_tx_conn;
    BeagleUsb3ConnStats   usb3_rx_conn;
    BeagleUsb2Stats       usb2;
};
#ifndef __cplusplus
typedef struct BeagleUsbStats BeagleUsbStats;
#endif

int bg_usb_stats_read (
    Beagle           beagle,
    BeagleUsbStats * stats
);


int bg_usb2_stats_read (
    Beagle            beagle,
    BeagleUsb2Stats * stats
);



/*=========================================================================
| USB 480 API
 ========================================================================*/
/* General constants */
#define BG480_TRUNCATION_LENGTH 4
#define BG480V2_USB2_BUFFER_SIZE_256MB 256

/*=========================================================================
| USB 5000 API
 ========================================================================*/
/* General constants */
#define BG5000_USB2_BUFFER_SIZE_128MB 128
#define BG5000_USB3_BUFFER_SIZE_2GB 2
#define BG5000_USB3_BUFFER_SIZE_4GB 4
/* Cross-Analyzer Sync Configuration */
enum Beagle5000CrossAnalyzerSyncMode {
    BG5000_CROSS_ANALYZER_SYNC_WAIT   = 0,
    BG5000_CROSS_ANALYZER_SYNC_BYPASS = 1
};
#ifndef __cplusplus
typedef enum Beagle5000CrossAnalyzerSyncMode Beagle5000CrossAnalyzerSyncMode;
#endif

enum Beagle5000CrossAnalyzerMode {
    BG5000_CROSS_ANALYZER_ACCEPT = 0,
    BG5000_CROSS_ANALYZER_IGNORE = 1
};
#ifndef __cplusplus
typedef enum Beagle5000CrossAnalyzerMode Beagle5000CrossAnalyzerMode;
#endif

int bg5000_cross_analyzer_sync_config (
    Beagle                          beagle,
    Beagle5000CrossAnalyzerSyncMode cross_sync_mode,
    Beagle5000CrossAnalyzerMode     cross_trigger_mode,
    Beagle5000CrossAnalyzerMode     cross_stop_mode
);


int bg5000_cross_analyzer_sync_release (
    Beagle beagle
);



/*=========================================================================
| MDIO API
 ========================================================================*/
enum BeagleMdioClause {
    BG_MDIO_CLAUSE_22    = 0,
    BG_MDIO_CLAUSE_45    = 1,
    BG_MDIO_CLAUSE_ERROR = 2
};
#ifndef __cplusplus
typedef enum BeagleMdioClause BeagleMdioClause;
#endif

#define BG_MDIO_OPCODE22_WRITE 0x01
#define BG_MDIO_OPCODE22_READ 0x02
#define BG_MDIO_OPCODE22_ERROR 0xff
#define BG_MDIO_OPCODE45_ADDR 0x00
#define BG_MDIO_OPCODE45_WRITE 0x01
#define BG_MDIO_OPCODE45_READ_POSTINC 0x02
#define BG_MDIO_OPCODE45_READ 0x03
/* Read the next MDIO frame. */
int bg_mdio_read (
    Beagle beagle,
    u32 *  status,
    u64 *  time_sop,
    u64 *  time_duration,
    u32 *  time_dataoffset,
    u32 *  data_in
);


int bg_mdio_read_bit_timing (
    Beagle beagle,
    u32 *  status,
    u64 *  time_sop,
    u64 *  time_duration,
    u32 *  time_dataoffset,
    u32 *  data_in,
    int    max_timing,
    u32 *  bit_timing
);


/*
 * Parse the raw MDIO data into the standard format.
 * This function will fill the supplied fields as per
 * the constants defined above.  If the raw data contains
 * a malformed turnaround field, the caller will be
 * notified of the error through the return value of
 * this function (BG_MDIO_BAD_TURNAROUND).
 */
int bg_mdio_parse (
    u32   packet,
    u08 * clause,
    u08 * opcode,
    u08 * addr1,
    u08 * addr2,
    u16 * data
);



/*=========================================================================
| IV MON API
 ==========================================================================
| Extract the current and voltage values in the packet returned by
| bg_usb_read(*/
int bg_iv_mon_parse (
    int         length,
    const u08 * packet,
    f32 *       voltage,
    f32 *       current
);




#ifdef __cplusplus
}
#endif

#endif  /* __beagle_h__ */
