//
//  ATSBPacketizer.h
//  ATSBoxSupport
//
//  Created by Joe Basu on 1/13/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <ATSBoxSupport/ATSBoxTypes.h>

@interface ATSBPacketizer : NSObject {
	BOOL _processingATF;
	struct ATFHeader _currentATFHeader;
	UInt8 _currentATFBytes[10];
	UInt8 _currentATFSize;
	UInt8 _currentATFByteOffset;
	
	UInt8 _currentATFPayload;
	UInt16 _currentATFTimestamp;
	UInt16 _currentATFExtendedPayload;
}

+ (NSData *)atfDataWithSource:(ATFSource)source
					  command:(ATFCommand)command
					  payload:(UInt8)payload
					timestamp:(const UInt16 *)timestampRef
			  extendedPayload:(const UInt16 *)extendedPayloadRef;

+ (NSData *)atfDataForSerialDataBufferCommandWithSource:(ATFSource)source
												   data:(NSData *)data;

- (NSData *)processATSBoxByte:(UInt8)byte;


@end
