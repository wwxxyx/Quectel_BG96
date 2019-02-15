//
//  ATSSerialCaptureDeviceManager+Protected.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 10/16/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import "ATSSerialCaptureDeviceManager.h"

@class ATSSerialCaptureDevice;

/**
 Context used when listening to available devices.
 */
extern NSString * const ATSSerialCaptureDeviceManagerContext;

/**
 String used as device path when a ATS serial capture device is in DFU mode and can't provide a valid device path.
 */
extern NSString * const ATSSerialDFUModeDeviceInvalidPath;

@interface ATSSerialCaptureDeviceManager (Protected)

/**
 List of all the attached serial capture devices.
 */
@property (nonatomic, readwrite) NSMutableArray *attachedDevices;

/**
 Class object for physical capture devices that the device manager manages.
 */
@property (nonatomic, readonly) Class physicalCaptureDeviceClass;

/**
 Initialize an instance of ATSSerialCaptureDeviceManager with the input product ID. Used by derived class to specialize the base class for either ATS Lightning Box or ATS Smart Connector Box.
 
 @param productID Product ID of the ATS capture hardware to start the IO notifier to.
 @param VendorID Vendor ID of the ATS capture hardware to start the IO notifier to.
 */
- (instancetype)initWithProductID:(NSNumber *)productID andVendorID:(NSNumber *)vendorID;

/**
 Method to handle adding a newly connected serial Capture Hardware.
 
 @param properties Device IO properties dictionary derived from the device tree.
 */
- (void)serialDeviceAdded:(NSDictionary *)properties;

/**
 Create and add an ATS serial capture device to the list of available devices.
 
 @param productName Product display name of the capture device used in capture configuration.
 @param devicePath device path of the capture device to be removed from the list of available devices.
 @param productID Product ID of the capture device to be removed from the list of available devices.
 @param locationID Location ID of the capture device to be removed from the list of available devices.
 */
- (void)addATSSerialCaptureDeviceWithProductDisplayName:(NSString *)productDisplayName
                                      devicePath:(NSString *)devicePath
                                       productID:(NSNumber *)productID
                                      locationID:(NSNumber *)locationID;

/**
 Remove the ATS serial capture device from the list of available devices based on the input location ID.
 
 @param locationID Location ID of the capture device to be removed from the list of available devices.
 */
- (void)removeATSSerialCaptureDeviceWithLocationID:(NSNumber *)locationID;

/**
 Get board path from the device IO properties derived from the device tree.
 
 @param properties device IO properties derived from the device tree.
 */
+ (NSString *)boardPathForATSSerialDeviceProperties:(NSDictionary *)properties;

/**
 Get product name from the device IO properties derived from the device tree.
 
 @param properties device IO properties derived from the device tree.
 */
+ (NSString *)productNameForATSSerialDeviceProperties:(NSDictionary *)properties;

/**
 Get product ID from the device IO properties derived from the device tree.
 
 @param properties device IO properties derived from the device tree.
 */
+ (NSNumber *)productIDForATSSerialDeviceProperties:(NSDictionary *)properties;

/**
 Get location ID from the device IO properties derived from the device tree.
 
 @param properties device IO properties derived from the device tree.
 */
+ (NSNumber *)locationIdForATSSerialDeviceProperties:(NSDictionary *)properties;

/**
 Get serial number of the USB device from the device IO properties derived from the device tree.
 
 @param properties device IO properties derived from the device tree.
 */
+ (NSString *)usbSerialNumberForATSSerialDeviceProperties:(NSDictionary *)properties;

/**
 Method to add a new serial capture device to the attached devices array.
 @param serialCaptureDevice A new capture device.
 */
- (void)addATSSerialCaptureDevice:(ATSSerialCaptureDevice *)serialCaptureDevice;

/**
 Method to remove a new serial capture device to the attached devices array.
 @param serialCaptureDevice capture device reference to be removed.
 */
- (void)removeATSSerialCaptureDevice:(ATSSerialCaptureDevice *)serialCaptureDevice;

/**
 Method to remove product code from the input display name, if present.
 
 @param productDisplayName Input product display name to remove the product code string from.
 
 @note: After removing the product code, if the product display name turns out to be an empty string, the method falls back to using the product name instead.
 */
- (NSString *)sanitizeProductDisplayNameFromProductCode:(NSString *)productDisplayName;

@end
