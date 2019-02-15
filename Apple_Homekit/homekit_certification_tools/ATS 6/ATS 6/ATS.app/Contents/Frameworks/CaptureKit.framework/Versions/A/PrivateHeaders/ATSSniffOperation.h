//
//  ATSSniffOperation.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 2/19/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class ATSSniffOperation;

/** Sniff operation delegates receive data buffers and timestamps representing sniffed Ethernet frames. */
@protocol ATSSniffOperationDelegate <NSObject>

/**
 Provides the receiver with a data buffer containing a sniffed Ethernet frame, as well as a timestamp indicating when the frame was sniffed.
 
 @param sniffOperation The sniff operation that sent the message.
 @param frameData A data buffer containing an Ethernet frame.
 @param timestamp The time in nanoseconds when the frame was sniffed.
 */
- (void)sniffOperation:(ATSSniffOperation *)sniffOperation didSniffData:(NSData *)data atTimestamp:(UInt64)timestamp;

@optional

/**
 Provides the receiver with the total number of bytes read so far. This method is invoked periodically at regular intervals.
 
 @param sniffOperation The sniff operation that sent the message.
 @param bytes The number of bytes read so far.
 */
- (void)sniffOperation:(ATSSniffOperation *)sniffOperation hasSniffedTotalBytes:(NSUInteger)bytes;

/**
 Notifies the delegate that the sniff operation has completed.
 
 @param sniffOperation The sniff operation that sent the message.
 */
- (void)sniffOperationDidFinishSniffing:(ATSSniffOperation *)sniffOperation;

@end

/** ATSSniffOperation implements the packet sniff loop that drives WiFi capture. */
@interface ATSSniffOperation : NSOperation

/** The delegate object that will received sniffed data. */
@property (weak) id<ATSSniffOperationDelegate> delegate;

/**
 Initializes and returns a new operation object configured to sniff data from the network interface with the given name.
 
 @param interfaceName The name of the network interface to sniff.
 @return The new sniff operation object.
 */
- (instancetype)initWithInterfaceName:(NSString *)interfaceName;

/**
 Initializes and returns a new operation object configured to sniff data from the file at the given URL.
 
 @param fileURL The URL for the packet capture file from which to sniff data.
 @return The new sniff operation.
 */
- (instancetype)initWithFileURL:(NSURL *)fileURL;

@end
