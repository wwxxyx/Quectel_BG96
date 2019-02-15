//
//  RuleTests+USB.h
//  ATSMacApp
//
//  Created by Joe Basu on 8/14/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleTests.h>
#import <BusSniffer/BusSniffer.h>

@class USBNativeHIDReportDescriptorTransferEvent;
@class USBNativeHIDReportTransferEvent;
@class USBTransferEvent;

#define kRTCarPlayAppleDeviceEndpoint 0x06
#define kRTCarPlayAccessoryEndpoint 0x85

@interface RuleTests (USB)

/*!
 @method    emulateAppleDeviceUSBHIDEnumeration
 @abstract  Emulates the enumeration of an Apple device that speaks iAP over USB HID
 */
- (void)emulateAppleDeviceUSBHIDEnumeration;

/*!
 @method    emulateAppleDeviceiAPOverUSBHIDPacket:
 @abstract  Emulates sending an iAP over USB HID packet from an Apple device
 @parameter packetData The packet data to send
 @return    An array of emulated events
 */
- (NSArray *)emulateAppleDeviceiAPOverUSBHIDPacket:(NSData *)packetData;

/*!
 @method    emulateAccessoryiAPOverUSBHIDPacket:
 @abstract  Emulates sending an iAP over USB HID packet from an accessory
 @parameter packetData The packet data to send
 @return    An array of emulated events
 */
- (NSArray *)emulateAccessoryiAPOverUSBHIDPacket:(NSData *)packetData;

/*!
 @method    emulateAccessoryUSBBulkPipeEnumeration
 @abstract  Emulates the enumeration of an accessory that speaks iAP over USB bulk pipe without using the interrupt endpoint
 */
- (void)emulateAccessoryUSBBulkPipeEnumeration;

/*!
 @method    emulateAccessoryUSBBulkPipeEnumerationUsingDeprecatedInterruptEndpoint
 @abstract  Emulates the enumeration of an accessory that speaks iAP over USB bulk pipe, including the deprecated interrupt IN endpoint
 */
- (USBTransferEvent *)emulateAccessoryUSBBulkPipeEnumerationUsingDeprecatedInterruptEndpoint;

/*!
 @method    emulateAccessoryUSBBulkPipeEANativeTransportEnumeration
 @abstract  Emulates the enumeration of an accessory that speaks iAP over USB bulk pipe and supports EA native transport
 */
- (void)emulateAccessoryUSBBulkPipeEANativeTransportEnumeration;

/*!
 @method    emulateAccessoryUSBBulkPipeHIDEnumeration
 @abstract  Emulates the enumeration of an accessory that speaks iAP over USB bulk pipe and supports a generic HID interface
 */
- (void)emulateAccessoryUSBBulkPipeHIDEnumeration;

/*!
 @method    emulateAccessoryUSBBulkPipeCarPlayEnumeration
 @abstract  Emulates the enumeration of an accessory that speaks iAP over USB bulk pipe and supports CarPlay
 */
- (void)emulateAccessoryUSBBulkPipeCarPlayEnumeration;

/*!
 @method    emulateAccessoryUSBBulkPipeEnumerationWithAdditionalInterfaceDescriptorData
 @abstract  Emulates the enumeration of an accessory that speaks iAP over USB bulk pipe and includes
            an additional interface descriptor.
 @param     interfaceDescriptorData
            The interface descriptor's data.
 */
- (void)emulateAccessoryUSBBulkPipeEnumerationWithAdditionalInterfaceDescriptorData:(NSData *)interfaceDescriptorData;

/*!
 @method    emulateAccessoryUSBBulkPipeEnumerationWithAdditionalInterfaceDescriptorsDatas:interfaceDescriptorsDatas
 @abstract  Emulates the enumeration of an accessory that speaks iAP over USB bulk pipe and includes
            additional interface descriptors.
 @param     interfaceDescriptorsDatas
            An array of interface descriptor datas.
 */
- (void)emulateAccessoryUSBBulkPipeEnumerationWithAdditionalInterfaceDescriptorsDatas:(NSArray *)interfaceDescriptorsDatas;

/*!
 @method    emulateAppleDeviceiAPOverUSBBulkPipePacket:
 @abstract  Emulates sending an iAP over USB bulk pipe packet from an Apple device
 @parameter packetData The packet data to send
 @return    An array of emulated events
 */
- (NSArray *)emulateAppleDeviceiAPOverUSBBulkPipePacket:(NSData *)packetData;

/*!
 @method    emulateAccessoryiAPOverUSBBulkPipePacket:
 @abstract  Emulates sending an iAP over USB bulk pipe packet from an accessory
 @parameter packetData The packet data to send
 @return    An array of emulated events
 */
- (NSArray *)emulateAccessoryiAPOverUSBBulkPipePacket:(NSData *)packetData;

/*!
 @method    emulateUSBBulkPipeTransfer:source:endpoint:
 @abstract  Emulates a USB bulk pipe transfer from the passed source using the specified endpoint
 */
- (NSArray *)emulateUSBBulkPipeTransfer:(NSData *)packetData source:(BSiAPSource)source endpoint:(UInt8)endpoint;

/*!
 @method    emulateUSBGetStringDescriptorAtIndex:string
 @abstract  Emulates a string descriptor being fetched (works for both USB types)
 @parameter index The descriptor index
 @parameter string The string to emulate
 */
- (void)emulateUSBGetStringDescriptorAtIndex:(UInt8)index string:(NSString *)string;

/*!
 @method    emulateUSBVendorRequest:wValue:wIndex:wLength:
 @abstract  Emulates a USB vendor request with the passed request parameters
 */
- (USBTransferEvent *)emulateUSBVendorRequest:(UInt8)bRequest wValue:(UInt8)wValue wIndex:(UInt8)wIndex wLength:(UInt8)wLength;

/*!
 @method    emulateUSBGetHIDReportDescriptorWithData:forInterfaceIndex:
 @abstract  Emulates a Get HID report descriptor request.
 @param     hidReportDescriptorData
            The data for the hid report descriptor.
 @param     index
            The index for the interface the HID report descriptor is associated with.
 @return    The USBDeviceRequestHIDReportDescriptorEvent instance representing the event.
 */
- (USBNativeHIDReportDescriptorTransferEvent *)emulateUSBGetHIDReportDescriptorWithData:(NSData *)hidReportDescriptorData forInterfaceIndex:(UInt8)index;

/*!
 @method    emulateUSBGetHIDReportWithData:reportID:forInterfaceIndex:
 @abstract  Emulates a Get HID report request.
 @param     hidReportData
            The data for the hid report.
 @param     reportID
            The report ID of the requested HID report.
 @param     index
            The index for the interface the HID report is associated with.
 @return    The USBNativeHIDReportTransferEvent instance representing the event.
 */
- (USBNativeHIDReportTransferEvent *)emulateUSBGetHIDReportWithData:(NSData *)hidReportData reportID:(UInt8)reportID forInterfaceIndex:(UInt8)interfaceIndex;

/*!
 @method    emulateUSBSetHIDReportWithData:reportID:forInterfaceIndex:
 @abstract  Emulates a Set HID report request.
 @param     hidReportData
            The data for the hid report.
 @param     reportID
            The report ID of the HID report.
 @param     index
            The index for the interface the HID report is associated with.
 @return    The USBNativeHIDReportTransferEvent instance representing the event.
 */
- (USBNativeHIDReportTransferEvent *)emulateUSBSetHIDReportWithData:(NSData *)hidReportData reportID:(UInt8)reportID forInterfaceIndex:(UInt8)interfaceIndex;

/*!
 @method    emulateUSBGetConfigurationDescriptorWithData:
 @abstract  Emulates a GetConfiguration request and response.
 @param     configurationDescriptorData
            The configuration descriptor data payload
 */
- (NSArray *)emulateUSBGetConfigurationDescriptorWithData:(NSData *)configurationDescriptorData;

/*!
 @method    emulateAccessoryUSBBulkPipeEnumerationWithConfigurationDescriptorData:
 @abstract  Emulates a configuration descriptor being fetched over USB bulk pipe (Apple device as USB host)
 @parameter configurationDescriptorData The complete configuration descriptor data payload
 */
- (NSArray *)emulateAccessoryUSBBulkPipeEnumerationWithConfigurationDescriptorData:(NSData *)configurationDescriptorData;

/*!
 @method    -emulateUSBHIDReportData:overEndpointNumber:endpointDirection:
 @abstract  Emulates a HID report over the interrupt endpoint.
 @param     hidReportData
            The HID report data.
 @param     endpointNumber
            The interrupt endpoint number.
 @param     endpointDirection
            The interrupt endpoint direction.
 */
- (USBNativeHIDReportTransferEvent *)emulateUSBHIDReportData:(NSData *)hidReportData overEndpointNumber:(UInt8)endpointNumber endpointDirection:(BSUSBEndpointDirection)endpointDirection;

/*!
 @method    dataForConfigurationDescriptorWithbConfigurationValue:iConfiguration:bmAttributes:bMaxPower:interfaceDescriptorsDatas
 @abstract  Returns the data for a configuration descriptor with the given values.
 @params    See USB spec 2.0 for more information on the usb parameters.
 @param     interfaceDescriptorsDatas
            An array of NSData objects representing the interface descriptor data for the configuration descriptor.
 */
- (NSData *)dataForConfigurationDescriptorWithbConfigurationValue:(UInt8)bConfigurationValue
                                                   iConfiguration:(UInt8)iConfiguration
                                                     bmAttributes:(UInt8)bmAttributes
                                                        bMaxPower:(UInt8)bMaxPower
                                        interfaceDescriptorsDatas:(NSArray *)interfaceDescriptorsDatas;

/*!
 @method    dataForInterfaceDescriptorWithbInterfaceNumber:bAlternateSetting:bInterfaceClass:bInterfaceSubClass:bInterfaceProtocol:iInterface:classSpecificDescriptorsDatas:endpointDescriptorsDatas:
 @abstract  Returns the data for an interface descriptor with the given values.
 @params    See USB spec 2.0 for more information on the usb parameters.
 @param     classSpecificDescriptorsDatas
            An array of NSData objects representing the class specific descriptor data for the interface descriptor.
 @param     endpointDescriptorsDatas
            An array of NSData objects representing the endpoint descriptor data for the interface descriptor.
 */
- (NSData *)dataForInterfaceDescriptorWithbInterfaceNumber:(UInt8)bInterfaceNumber
                                         bAlternateSetting:(UInt8)bAlternateSetting
                                           bInterfaceClass:(UInt8)bInterfaceClass
                                        bInterfaceSubClass:(UInt8)bInterfaceSubClass
                                        bInterfaceProtocol:(UInt8)bInterfaceProtocol
                                                iInterface:(UInt8)iInterface
                             classSpecificDescriptorsDatas:(NSArray *)classSpecificDescriptorsDatas
                                  endpointDescriptorsDatas:(NSArray *)endpointDescriptorsDatas;

/*!
 @method    dataForEndpointDescriptorWithbEndpointAddress:bmAttributes:wMaxPacketSize:bInterval:
 @abstract  Returns the data for an endpoing descriptor with the given values.
 @params    See USB spec 2.0 for more information on the usb parameters.
 */
- (NSData *)dataForEndpointDescriptorWithbEndpointAddress:(UInt8)bEndpointAddress
                                             bmAttributes:(UInt8)bmAttributes
                                           wMaxPacketSize:(UInt16)wMaxPacketSize
                                                bInterval:(UInt8)bInterval;

/*!
 @method    dataForClassSpecificDescriptorHIDWithbDescriptorType:wDescriptorLength:
 @abstract  Returns the data for a HID descriptor with the given values.
 @params    See USB spec 2.0 for more information on the usb parameters.
 */
- (NSData *)dataForClassSpecificDescriptorHIDWithbDescriptorType:(UInt8)bDescriptorType
                                               wDescriptorLength:(UInt16)wDescriptorLength;

/*!
 @method    emulateUSBDeviceRequest:withData:
 @abstract  Emulate a USB device request on the control pipe
 */
- (NSArray *)emulateUSBDeviceRequest:(BSUSBDeviceRequestRecord)deviceRequestRecord withData:(NSData *)data;

@end
