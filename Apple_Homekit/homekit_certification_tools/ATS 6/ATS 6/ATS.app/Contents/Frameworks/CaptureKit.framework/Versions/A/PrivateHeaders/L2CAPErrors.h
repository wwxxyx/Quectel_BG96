//
//  L2CAPErrors.h
//  Header for Bluetooth L2CAP Errors
//
//  Created by Edgard Lima on 08/13/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, L2CAPFrameErrorType) {
    /// No frameData. It is NULL. Probably a programming error.
    L2CAPFrameErrorTypeNoFrameData,
    /// The L2CAP Frame header is too short.
    L2CAPFrameErrorTypeInvalidHeaderLength,
    /// Payload doesn't fit in frame data
    L2CAPFrameErrorTypePayloadDoesNotFit,
    /// Invalid value for Field.
    L2CAPFrameErrorTypeInvalidValueForField,
    /// Frame Data is too small to hold Signaling Configuration.
    L2CAPFrameErrorTypeConfigurationDoesNotFit,
    /// The Frame Data has more Bytes than Header + Payload. The data at the end will be discarded.
    L2CAPFrameErrorTypeFrameDataDiscarded,
    /// The Configuration Data has more Bytes than Header + Paramenters. The data at the end will be discarded.
    L2CAPFrameErrorTypeConfigurationDataDiscarded,
    /// The length of option data as in header has an expected type for the configuration type
    L2CAPFrameErrorTypeOptionDataLenghtNotExpected
};

extern NSString * const kL2CAPFrameErrorDomain;

@interface L2CAPErrors : NSObject

/*!
 Add a L2CAPFrameErrorTypeNoFrameData error to the errors array.
 
 @param errors Array to receive the L2CAPFrameErrorTypeNoFrameData error. If it is nil, nothing is done.
 @param actualHeaderLength The actual header length of the L2CAP Frame. It will be used to fill up the error description.
 @param expectedHeaderLength The expected header length of the L2CAP Frame. It will be used to fill up the error description.
 @note see <L2CAPFrameErrorTypeNoFrameData> localized string.
 */
+ (void)setErrorsForNoFrameData:(NSMutableArray<NSError *> *)errors;

/*!
 Add a L2CAPFrameErrorTypeInvalidHeaderLength error to the errors array.
 
 @param errors Array to receive the L2CAPFrameErrorTypeInvalidHeaderLength error. If it is nil, nothing is done.
 @param actualHeaderLength The actual header length of the L2CAP Frame. It will be used to fill up the error description.
 @param expectedHeaderLength The expected header length of the L2CAP Frame. It will be used to fill up the error description.
 @note see <L2CAPFrameErrorTypePayloadDoesNotFit> localized string.
 */
+ (void)setErrorsForInvalidShortHeader:(NSMutableArray<NSError *> *)errors actualHeaderLength:(NSUInteger)actualHeaderLength expectedHeaderLength:(NSUInteger)expectedHeaderLength;

/*!
 Add a L2CAPFrameErrorTypePayloadDoesNotFit error to the errors array.
 
 @param errors Array to receive the L2CAPFrameErrorTypePayloadDoesNotFit error. If it is nil, nothing is done.
 @param payloadOffset The offset of the payload into the frameData, i.e. the header length. It will be used to fill up the error description.
 @param payloadLength The length of the payload as decribed by the L2CAP header. It will be used to fill up the error description.
 @param frameLength the length of the frameData, i.e. header + data following header. It will be used to fill up the error description.
 @note see <L2CAPFrameErrorTypeFrameDataDiscarded> localized string.
 */
+ (void)setErrorsForPayloadDoesNotFit:(NSMutableArray<NSError *> *)errors payloadOffset:(NSUInteger)payloadOffset payloadLength:(NSUInteger)payloadLength frameLength:(NSUInteger)frameLength;

/*!
 Add a L2CAPFrameErrorTypeInvalidValueForField error to the errors array.
 
 @param errors Array to receive the L2CAPFrameErrorTypeInvalidValueForField error. If it is nil, nothing is done.
 @param fieldStr the field name. It will be used to fill up the error description.
 @param description the reason why the field is invalid.
 @note see <L2CAPFrameErrorTypeInvalidValueForField> localized string.
 */
+ (void)setErrorsForInvalidValueForField:(NSMutableArray<NSError *> *)errors field:(NSString*)fieldStr description:(NSString*)description;

/*!
 Add a L2CAPFrameErrorTypeConfigurationDoesNotFit error to the errors array.
 
 @param errors Array to receive the L2CAPFrameErrorTypeConfigurationDoesNotFit error. If it is nil, nothing is done.
 @param payloadOffset The offset of the payload into the frameData, i.e. the header length. It will be used to fill up the error description.
 @param payloadLength The length of the payload as decribed by the L2CAP header. It will be used to fill up the error description.
 @param frameLength the length of the frameData, i.e. header + data following header. It will be used to fill up the error description.
 @note see <L2CAPFrameErrorTypeConfigurationDoesNotFit> localized string.
 */
+ (void)setErrorsForConfigurationDoesNotFit:(NSMutableArray<NSError *> *)errors typeStr:(NSString*)typeStr payloadLength:(NSUInteger)payloadLength requiredLength:(NSUInteger)requiredLength;

/*!
 Add a L2CAPFrameErrorTypeFrameDataDiscarded error to the errors array.
 
 @param errors Array to receive the L2CAPFrameErrorTypeFrameDataDiscarded error. If it is nil, nothing is done.
 @param frameDataLength the length of the frameData, i.e. header + data following header. It will be used to fill up the error description.
 @param headerLength the header length. It will be used to fill up the error description.
 @param payloadLength The length of the payload as decribed by the L2CAP header. It will be used to fill up the error description.
 @note see <L2CAPFrameErrorTypeFrameDataDiscarded> localized string.
 */
+ (void)setErrorsForFrameDataDiscarded:(NSMutableArray<NSError *> *)errors frameDataLength:(NSUInteger)frameDataLength headerLength:(NSUInteger)headerLength payloadLength:(NSUInteger)payloadLength;

/*!
 Add a L2CAPFrameErrorTypeConfigurationDataDiscarded error to the errors array.
 
 @param errors Array to receive the L2CAPFrameErrorTypeConfigurationDataDiscarded error. If it is nil, nothing is done.
 @param frameDataLength the length of the frameData, i.e. header + data following header. It will be used to fill up the error description.
 @param headerLength the header length. It will be used to fill up the error description.
 @param payloadLength The length of the payload as decribed by the L2CAP header. It will be used to fill up the error description.
 @note see <L2CAPFrameErrorTypeConfigurationDataDiscarded> localized string.
 */
+ (void)setErrorsForConfigurationDataDiscarded:(NSMutableArray<NSError *> *)errors configurationDataLength:(NSUInteger)configurationDataLength headerLength:(NSUInteger)headerLength parametersLength:(NSUInteger)parametersLength;

/*!
 Add a L2CAPFrameErrorTypeOptionDataLenghtNotExpected error to the errors array.
 
 @param errors Array to receive the L2CAPFrameErrorTypeOptionDataLenghtNotExpected error. If it is nil, nothing is done.
 @param frameDataLength the length of the frameData, i.e. header + data following header. It will be used to fill up the error description.
 @param headerLength the header length. It will be used to fill up the error description.
 @param payloadLength The length of the payload as decribed by the L2CAP header. It will be used to fill up the error description.
 @note see <L2CAPFrameErrorTypeOptionDataLenghtNotExpected> localized string.
 */
+ (void)setErrorsForConfigurationOptionDataLenghtNotExpected:(NSMutableArray<NSError *> *)errors configurationTypeStr:(NSString*)configurationTypeStr optionDataLength:(NSUInteger)optionDataLength expectedOptionDataLength:(NSUInteger)expectedOptionDataLength;

@end
