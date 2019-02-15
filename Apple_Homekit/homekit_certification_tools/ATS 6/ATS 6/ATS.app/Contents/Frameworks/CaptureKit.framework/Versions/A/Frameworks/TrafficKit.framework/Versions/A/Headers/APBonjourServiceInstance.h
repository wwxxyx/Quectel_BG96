//
//  APBonjourServiceInstance.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 11/04/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class APDNSSRVResource;
@class APDNSTXTResource;
@class APDNSIPv4AddressResource;
@class APDNSIPv6AddressResource;

/**
 Service instance stores the SRV record, TXT record and a list of addresses for a Bonjour service
 */
@interface APBonjourServiceInstance : NSObject <NSCopying>

/**
 This value is used to make sure that the CarPlay IP addresses, when discovered in bonjour records, are only reported once.
 */
@property (nonatomic) NSUInteger numberOfIPAddressesReported;

/**
 Name of the Bonjour service record instance "<Instance Name>.<Service Type>.<Domain>"
 */
@property (nonatomic, copy) NSString *serviceName;

/**
 Bonjour SRV Record that contains the host name and port
 */
@property (nonatomic, strong) APDNSSRVResource *serviceRecord;

/**
 Bonjour TXT record that contains the service keys
 */
@property (nonatomic, strong) APDNSTXTResource *txtRecord;

/**
 Array of address strings for this service
 */
@property (nonatomic, readonly) NSArray *ipAddresses;

/**
 Add the currently found IP Addresses to this record.
 
 @param ipAddresses IP addresses to add. Can be IPv4 / IPv6
 */
- (void)addIPAddresses:(NSSet <NSString *> *)ipAddresses;

@end
