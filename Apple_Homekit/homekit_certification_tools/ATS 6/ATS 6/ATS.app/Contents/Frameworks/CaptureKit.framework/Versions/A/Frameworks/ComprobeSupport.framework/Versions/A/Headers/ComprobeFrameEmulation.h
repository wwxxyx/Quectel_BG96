//
//  ComprobeFrameEmulation.h
//  ComprobeSupport
//
//  Created by Natália Cabral on 27/11/13.
//  Copyright (c) 2013 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ComprobeFrame.h"

@interface ComprobeFrameEmulation : NSObject

/*!
 @method    comprobeFrameWithRFCOMMControlCode:source:timestamp:
 @abstract  Returns an RFCOMM control code frame (SABM, UIH, etc) suitable for emulation
 */
- (ComprobeFrame *)comprobeFrameWithRFCOMMControlCode:(UInt8)controlCode source:(ComprobeFrameSource)source timestamp:(UInt64)timestamp;

/*!
 @method comprobeFrameDataWithIAPData:appleDeviceSource:
 @abstract generate an emulated frame data for device apple and accessory using the given iAP data
 @param iAPData The iAP data value
 @param isAppleDevice If TRUE value, the frame data's source is apple device otherwise the source is accessory.
 @return The generated frame data
 */
- (NSArray *)comprobeFrameDataWithIAPData:(NSData *)iAPData appleDeviceSource:(BOOL)isAppleDevice;

/*!
 @method comprobeFrameWithLMPOpcode:andOriginalOpcode:appleDeviceSource:timestamp:
 @abstract generate an emulated frame data for device apple and accessory using the given LMP opcode and original opcode
 @param opcode The LMP opcode
 @param andOriginalOpcode The LMP original opcode
 @param isAppleDevice If TRUE value, the frame data's source is apple device otherwise the source is accessory.
 @param timestamp the comprobe frame timestamp
 @return The generated comprobe frame
 */
- (ComprobeFrame *)comprobeFrameWithLMPOpcode:(UInt8)opcode andOriginalOpcode:(UInt8)originalOpcode appleDeviceSource:(BOOL)isAppleDevice timestamp:(UInt64)timestamp;

/*!
 @method comprobeFrameWithL2capSignalingChannelUsingCode:code:protocol:appleDeviceSource:timestamp:
 @abstract generate an emulated frame data for device apple and accessory using the given l2cap signaling code, the l2cap request protocol and the frame timestamp
 @param code The signaling channel code
 @param protocol The l2cap request protocol
 @param isAppleDevice If TRUE value, the frame data's source is apple device otherwise the source is accessory.
 @param timestamp the comprobe frame timestamp
 @return The generated comprobe frame
 */
- (ComprobeFrame *)comprobeFrameWithL2capSignalingChannelUsingCode:(UInt8)code protocol:(UInt16)protocol appleDeviceSource:(BOOL)isAppleDevice timestamp:(UInt64)timestamp;

/*!
 @method    comprobeFrameWithL2capCode:appleDeviceSource:timestamp:
 @abstract  Returns a frame suitable for emulation with the passed L2CAP opcode and empty data portion
 */
- (ComprobeFrame *)comprobeFrameWithL2capCode:(UInt8)code appleDeviceSource:(BOOL)isAppleDevice timestamp:(UInt64)timestamp;

/*!
 @method comprobeFrameWithSDPServiceAttributeRequestUsingTimestamp:
 @abstract generate an emulated frame data containing the SDP Attribute Service Request set for device apple and accessory using the given frame timestamp
 @param timestamp
 @return The generated comprobe frame
 */
- (ComprobeFrame *)comprobeFrameWithSDPServiceAttributeRequestUsingTimestamp:(UInt64)timestamp;

/*!
 @method comprobeFrameWithSDPServiceAttributeResponseUsingTimestamp:
 @abstract generate an emulated frame data containing the SDP Attribute Service Response set for device apple and accessory using the given frame timestamp
 @param timestamp
 @return The generated comprobe frame
 */
- (ComprobeFrame *)comprobeFrameWithSDPServiceAttributeResponseUsingTimestamp:(UInt64)timestamp;
@end
