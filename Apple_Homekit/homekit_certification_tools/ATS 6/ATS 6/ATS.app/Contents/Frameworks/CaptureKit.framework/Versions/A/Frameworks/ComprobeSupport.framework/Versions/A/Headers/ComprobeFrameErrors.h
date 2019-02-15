//
//  ComprobeFrameErrors.h
//  Comprobe Frame Errors
//
//  Created by Edgard Lima on 08/13/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

/// Code for Comprobe error. From ComprobeFrameErrorDomain domain.
typedef NS_ENUM(NSUInteger, ComprobeFrameErrorType) {
    /// The Comprobe Frame header is too short.
    ComprobeFrameErrorTypeInvalidHeaderLength,
    /// The playload size value present in the Header is too big. It wouldn't fit into the whole Comprobe Frame.
    ComprobeFrameErrorTypePayloadDoesNotFit,
    /// The Frame Data has more Bytes than Header + Payload. The data at the end will be discarded.
    ComprobeFrameErrorTypeFrameDataDiscarded,
    /// The value is invalid. It is not listed in the specification or is out-of-range.
    ComprobeFrameErrorTypeInvalidValueForField
};

extern NSString * const kComprobeFrameErrorDomain;

@interface ComprobeFrameErrors : NSObject

/*!
 Add a ComprobeFrameErrorTypeInvalidHeaderLength error to the errors array.
 
 @param errors Array to receive the ComprobeFrameErrorTypeInvalidHeaderLength error. If it is nil, nothing is done.
 @param actualHeaderLength The actual header length of the Comprobe Frame. It will be used to fill up the error description.
 @param expectedHeaderLength The expected header length of the Comprobe Frame. It will be used to fill up the error description.
 @note see <ComprobeFrameErrorTypePayloadDoesNotFit> localized string.
 */
+ (void)setErrorsForInvalidShortHeader:(NSMutableArray<NSError *> *)errors actualHeaderLength:(NSUInteger)actualHeaderLength expectedHeaderLength:(NSUInteger)expectedHeaderLength;

/*!
 Add a ComprobeFrameErrorTypePayloadDoesNotFit error to the errors array.
 
 @param errors Array to receive the ComprobeFrameErrorTypePayloadDoesNotFit error.  If it is nil, nothing is done.
 @param payloadOffset The offset of the payload into the frameData, i.e. the header length. It will be used to fill up the error description.
 @param payloadLength The length of the payload as decribed by the Comprobe header. It will be used to fill up the error description.
 @param frameLength the length of the frameData, i.e. header + data following header. It will be used to fill up the error description.
 @note see <ComprobeFrameErrorTypeFrameDataDiscarded> localized string.
 */
+ (void)setErrorsForPayloadDoesNotFit:(NSMutableArray<NSError *> *)errors payloadOffset:(NSUInteger)payloadOffset payloadLength:(NSUInteger)payloadLength frameLength:(NSUInteger)frameLength;

/*!
 Add a ComprobeFrameErrorTypeFrameDataDiscarded error to the errors array.
 
 @param errors Array to receive the ComprobeFrameErrorTypeFrameDataDiscarded error.  If it is nil, nothing is done.
 @param frameDataLength the length of the frameData, i.e. header + data following header. It will be used to fill up the error description.
 @param headerLength the header length. It will be used to fill up the error description.
 @param payloadLength The length of the payload as decribed by the Comprobe header. It will be used to fill up the error description.
 @note see <ComprobeFrameErrorTypeFrameDataDiscarded> localized string.
 */
+ (void)setErrorsForFrameDataDiscarded:(NSMutableArray<NSError *> *)errors frameDataLength:(NSUInteger)frameDataLength headerLength:(NSUInteger)headerLength payloadLength:(NSUInteger)payloadLength;

/*!
 Add a ComprobeFrameErrorTypeInvalidValueForField error to the errors array.
 
 @param errors Array to receive the ComprobeFrameErrorTypeInvalidValueForField error.  If it is nil, nothing is done.
 @param fieldStr the field name. It will be used to fill up the error description.
 @param description the reason why the field is invalid.
 @note see <ComprobeFrameErrorTypeInvalidValueForField> localized string.
 */
+ (void)setErrorsForInvalidValueForField:(NSMutableArray<NSError *> *)errors field:(NSString*)fieldStr description:(NSString*)description;

@end



