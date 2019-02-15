//
//  BSUSBAnalyzer.h
//  BusSniffer
//
//  Created by Joe Basu on 3/27/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBTypes.h>
#import <BusSniffer/BSUSBPacket.h>
#import <BusSniffer/BSUSBTransaction.h>
#import <BusSniffer/BSUSBTransfer.h>


@class BSUSBDeviceRequestInfo;
@class BSUSBDeviceDescriptor;
@class BSUSBAnalyzerTransactionState;
@class BSUSBAnalyzerTransferState;
@class BSUSBInterfaceDescriptor;
@class BSUSBConfigurationDescriptor;

@protocol BSUSBAnalyzerDelegate;

@interface BSUSBAnalyzer : NSObject {
	BSUSBSpeed _busSpeed;

	// transaction recognizer
	BSUSBAnalyzerTransactionState *_activeTransactionState;
	NSMutableArray *_recognizedTransactions;	

	// transfer recognizer
	BSUSBAnalyzerTransferState *_activeTransferState;
	NSMutableArray *_recognizedTransfers;
    BOOL _ignoringStatusStage;
	
	// device information
	BSUSBDeviceDescriptor *_deviceDescriptor;
	UInt8 _activeConfigurationValue;
	UInt8 _maxPacketSizeForEndpointZero;
	
	id <BSUSBAnalyzerDelegate> _delegate;
}

@property (nonatomic) BSUSBSpeed busSpeed;
@property (nonatomic, assign) id <BSUSBAnalyzerDelegate> delegate;

- (void)reset;

- (NSArray *)processPacket:(id <BSUSBPacket>)packet;
- (NSArray *)processDefaultControlPipeTransaction:(id <BSUSBTransaction>)transaction;

- (BOOL)isKnownEndpointNumber:(UInt8)number direction:(UInt8)direction;
- (BSUSBEndpointType)typeOfEndpointNumber:(UInt8)number direction:(BSUSBEndpointDirection)direction;
- (UInt16)maxPacketSizeOfEndpointNumber:(UInt8)endpointNumber direction:(BSUSBEndpointDirection)direction;
- (BSUSBInterfaceDescriptor *)interfaceDescriptorContainingEndpointNumber:(UInt8)number direction:(BSUSBEndpointDirection)direction;

@end

@protocol BSUSBAnalyzerDelegate <NSObject>

- (void)analyzer:(BSUSBAnalyzer *)analyzer usbDeviceConfigurationChanged:(BSUSBConfigurationDescriptor *)configurationDescriptor atTimestamp:(NSNumber *)timestamp;

/**
 This specifies whether the USB Analyzer should raise errors when there are problems with USB transactions
 
 @param analyzer the analyzer sending the message
 @return YES if the analyzer should raise transaction errors, and NO otherwise
 */
- (BOOL)analyzerShouldRaiseUSBTransactionErrors:(BSUSBAnalyzer *)analyzer;

@end
