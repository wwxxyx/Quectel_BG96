//
//  RTNetworkTypes.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/22/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef uint8_t RTEthernetAddress[6];
typedef uint32_t RTIPv4Address;
typedef uint8_t RTIPv6Address[16];

// https://en.wikipedia.org/wiki/EtherType
typedef enum : uint16_t {
                   RTEthernetTypeIPv4 = 0x0800,
                   RTEthernetTypeIPv6 = 0x86DD,
                   RTEthernetTypeARP = 0x0806,
               } RTEthernetType;

typedef enum : uint8_t {
                   RTIPTypeTCP = 0x06,
                   RTIPTypeUDP = 0x11,
                   RTIPTypeIPv4 = 0x04,
                   RTIPTypeIPv6 = 0x29,
               } RTIPType;

typedef enum : uint8_t {
                   RTTCPACKFlag = ((0x1 << 4)),
                   RTTCPSYNFlag = ((0x1 << 1)),
                   RTTCPFINFlag = ((0x1 << 0)),
               } RTTCPFlags;

typedef enum : uint8_t {
                   RTNCMEmulationMode, // Ethernet encapsulated in NCM
                                       // FUTURE: Others...
               } RTNetworkEmulationMode;

#pragma mark - MAC addresses -

#define kRTAppleDeviceMACAddress (((RTEthernetAddress){0x0a, 0x09, 0x09, 0x01, 0x03, 0x00})) // "apple".. sortof
#define kRTAccessoryMACAddress (((RTEthernetAddress){0x0a, 0x0c, 0x0c, 0x0e, 0x05, 0x05}))   // "access"
#define kRTBroadcastMACAddress (((RTEthernetAddress){0xff, 0xff, 0xff, 0xff, 0xff, 0xff}))

#pragma mark - IPv6 addresses -

#define kRTAppleDeviceIPv6Address (((RTIPv6Address){0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                                    0x0a, 0x09, 0x09, 0x01, 0x0e, 0x00, 0x00, 0x00}))
#define kRTAccessoryIPv6Address (((RTIPv6Address){0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                                  0x0a, 0x0c, 0x0c, 0x0e, 0x05, 0x05, 0x00, 0x12}))
