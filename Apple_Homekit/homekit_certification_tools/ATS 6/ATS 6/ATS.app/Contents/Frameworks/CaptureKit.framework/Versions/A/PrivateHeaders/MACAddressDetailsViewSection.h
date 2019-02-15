//
//  MACAddressDetailsViewSection.h
//  ATSMacApp
//
//  Created by Kip Nicol on 3/30/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/StringDetailsViewSection.h>

@interface MACAddressDetailsViewSection : StringDetailsViewSection

/*!
 @method    +macAddressSectionWithData:title:identifier:
 @abstract  Creates and returns a MACAddressDetailsViewSection with the given params.
 @param     macAddressData
            The data for the MAC address.
 @param     title
            The title of the section.
 @param     identifier
            The identifier of the section.
 */
+ (instancetype)macAddressSectionWithData:(NSData *)macAddressData title:(NSString *)title identifier:(NSString *)identifier;

@end
