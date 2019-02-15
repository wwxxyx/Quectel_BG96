//
//  BSUSBHIDiAP2LinkPacket.h
//  BusSniffer
//
//  Created by Joe Basu on 3/13/12.
//  Copyright © 2012 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSiAP2LinkPacket.h>

@protocol BSUSBHIDiAP2LinkPacket <BSiAP2LinkPacket>

@property (nonatomic, readonly, copy) NSArray *hidReports;

@end

@interface BSUSBHIDiAP2LinkPacket : BSiAP2LinkPacket <BSUSBHIDiAP2LinkPacket> {
	NSArray *_hidReports;
}

+ (id)usbHIDiAP2LinkPacketWithTimestamp:(UInt64)timestamp
                                 source:(BSiAPSource)source
                                   data:(NSData *)data
                             hidReports:(NSArray *)hidReports;

- (id)initWithTimestamp:(UInt64)timestamp
				 source:(BSiAPSource)source
				   data:(NSData *)data
			 hidReports:(NSArray *)hidReports;
@end
