//
//  NCMTransferBlockEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/20/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/NCMEvent.h>
#import <NCMKit/NCMKit.h>
#import <BusSniffer/BusSniffer.h>

@interface NCMTransferBlockEvent : NCMEvent

/*!
 @property  transferBlockData
 @abstract  The data for this transfer block
 */
@property (nonatomic, retain) NSData *transferBlockData;

/*!
 @property  isRawDatagram
 @abstract  If the transfer block data is a raw datagram without NCM encapsulation
 */
@property (nonatomic, assign) BOOL isRawDatagram;

/*!
 @property  direction
 @abstract  Direction of the transfer
 */
@property (nonatomic, assign) BSUSBTransferDirection direction;

/*!
 @property  errorString
 @abstract  The parser error, if any
 */
@property (nonatomic, retain) NSString *errorString;

/*!
 @method    source
 @abstract  The source for this transfer block
 */
- (NCMSource)source;

/*!
 @method    parsedTransferBlock
 @abstract  The parsed transfer block (parsed on demand)
 */
- (NCMTransferBlock *)parsedTransferBlock;

@end
