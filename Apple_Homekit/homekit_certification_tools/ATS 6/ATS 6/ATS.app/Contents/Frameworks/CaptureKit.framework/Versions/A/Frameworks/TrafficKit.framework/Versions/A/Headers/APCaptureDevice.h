//
//  APCaptureDevice.h
//  APTS
//
//  Created by Mark Hamlin on 6/22/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

/*!
 @enum      APInterfaceType
 @abstract  The capture interface type
 */
typedef enum {
    // SCNetworkInterfaceType => @"Ethernet"
    APInterfaceTypeEthernet = 1,
    
    // SCNetworkInterfaceType => @"IEEE80211"
    APInterfaceTypeWireless = 2,
    
    // SCNetworkInterfaceType => @"FireWire"
    APInterfaceTypeFireWire = 3,
    
    // Unknown -- IMPORTANT: Value should be 0 so it's always default
    APInterfaceTypeUnknown = 0, // Default is 0
} APInterfaceType;

/*!
 @class     APCaptureDevice
 @abstract  Represents a capture device for packet capture including attributes such as name (eg. en0), description, and addresses
 */
@interface APCaptureDevice : NSObject

/*!
 @property  interface
 @abstract  The name of the interface, eg. @"en0"
 */
@property (nonatomic, readonly, strong) NSString *interface;

/*!
 @property      info
 @abstract      Description of the interface as provided by libpcap
 @discussion    This is optional and may be nil. Infact, it is usually nil.
 */
@property (nonatomic, readonly, strong) NSString *info;

/*!
 @property  addresses
 @abstract  Array of addresses for this device, eg. [ @"127.0.0.1", ... ]
 */
@property (nonatomic, readonly, strong) NSArray *addresses;

/*!
 @property      type
 @abstract      The interface type
 @discussion    Detected by reading /Library/Preferences/SystemConfiguration/NetworkInterfaces.plist
 */
@property (nonatomic, readonly, assign) APInterfaceType type;

/*!
 @method    allDevices
 @abstract  Returns an array of available interfaces, as APCaptureDevice instances
 @return    Array of available interfaces, as APCaptureDevice instances
 */
+ (NSArray *)allDevices;

/*!
 @method    allAddresses
 @abstract  Determines all interface addresses we own
 @return    All interface addresses, as strings
 */
+ (NSArray *)allAddresses;

/*!
 @method    defaultDevice
 @abstract  The default capture device
 @return    Default capture device
 */
+ (APCaptureDevice *)defaultDevice;

/*!
 @method    deviceWithInterface:
 @abstract  Looks up the capture device by the interface's name
 @param     interface
            The interface represented by this device, eg @"en0"
 @return    Desired capture device, or nil if not found
 */
+ (APCaptureDevice *)deviceWithInterface:(NSString *)interface;

/*!
 @method    deviceWithInterfaceType:
 @abstract  Looks up the capture device by the interface's type
 @param     type
            Type of the interface
 @return    Desired capture device, or nil if not found
 */
+ (APCaptureDevice *)deviceWithInterfaceType:(APInterfaceType)type;

@end
