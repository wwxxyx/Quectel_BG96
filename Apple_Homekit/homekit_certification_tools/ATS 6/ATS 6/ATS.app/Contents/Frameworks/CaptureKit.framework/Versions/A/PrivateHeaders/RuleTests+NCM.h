//
//  RuleTests+NCM.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/20/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleTests.h>
#import <NCMKit/NCMKit.h>

@interface RuleTests (NCM)

/*!
 @method        emulateNCMDatagram:source;
 @abstract      Emulate an NCM datagram from the passed source
 */
- (NSArray *)emulateNCMDatagram:(NSData *)datagram source:(NCMSource)source;

/*!
 @method    emulateGetNTBParametersWithFormats:inMaxSize:inDivisor:inPayloadRemainder:inAlignment:outMaxSize:outDivisor:outPayloadRemainder:outAlignment:
 @abstract  Emulates GetNTBParameters
 */
- (NSArray *)emulateGetNTBParamtersWithFormats:(UInt16)bmNtbFormatsSupported
                                     inMaxSize:(UInt32)dwNtbInMaxSize
                                     inDivisor:(UInt16)wNdpInDivisor
                            inPayloadRemainder:(UInt16)wNdpInPayloadRemainder
                                   inAlignment:(UInt16)wNdpInAlignment
                                    outMaxSize:(UInt32)dwNtbOutMaxSize
                                    outDivisor:(UInt16)wNdpOutDivisor
                           outPayloadRemainder:(UInt16)wNdpOutPayloadRemainder
                                  outAlignment:(UInt16)wNdpOutAlignment;

/*!
 @method    emulateSetNTBInputSize:
 @abstract  Emulates setting the NTB input size
 */
- (NSArray *)emulateSetNTBInputSize:(UInt32)inputSize;

@end
