//
//  ComprobeFrameParserResult.h
//  ComprobeSupport
//
//  Created by Natália Cabral on 03/12/13.
//  Copyright (c) 2013 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

/*!
 @enumtype  ComprobeFrameParserResultContentType
 @abstract  High-level payload type of the comprobe frame
 */
enum
{
    ComprobeFrameParserResultContentTypeiAPData,
    ComprobeFrameParserResultContentTypeRFCOMMSABM,
    ComprobeFrameParserResultContentTypeRFCOMMUA,
    ComprobeFrameParserResultContentTypeRFCOMMDM,
    ComprobeFrameParserResultContentTypeRFCOMMDISC,
    ComprobeFrameParserResultContentTypeRFCOMMSABMForPossibleiAPChannel,
    ComprobeFrameParserResultContentTypeSDPDetectediAPChannel,
    ComprobeFrameParserResultContentTypeiAPDataAndiAPChannelDetected,
    ComprobeFrameParserResultContentTypeSDPWithInvalidUUID,
    ComprobeFrameParserResultContentTypeHID,
    ComprobeFrameParserResultContentTypeHIDReportDescriptorList
};
typedef UInt8 ComprobeFrameParserResultContentType;


@interface ComprobeFrameParserResult : NSObject

/*!
 @property   contentType
 @abstract   Property that represents the type of the comprobe frame content.
 */
@property(nonatomic, readonly) ComprobeFrameParserResultContentType contentType;

/*!
 @property   contentData
 @abstract   Property that contains the data of the parsed comprobe frame.
 */
@property(nonatomic, readonly) NSData *contentData;

/*!
 Used for Bluetooth parser to create the L2CAPFrameEvent object
 
 @property   l2capPayloadData
 @abstract   Property that contains the data of L2CAP payload processed
 */
@property(nonatomic, readwrite, strong) NSData *l2capFrameData;

/*!
 Used for RFCOMMSABMForPossibleiAPChannel
 
 @property   context
 @abstract   Property that contains a context object for certain content types
 */
@property(nonatomic, readonly) id context;

/*!
 @method     initWithContentType:contentData:
 @abstract   Inits a ComprobeFrameParserResult with the specified content type and data.
 @return     A new ComprobeFrameParserResult with the specified content type and data.
 */
- (instancetype)initWithContentType:(ComprobeFrameParserResultContentType)comprobeFrameContentType contentData:(NSData *)comprobeFrameContentData;

/*!
 @method     initWithContentType:context:
 @abstract   Inits a ComprobeFrameParserResult with the specified content type and context.
 @return     A new ComprobeFrameParserResult with the specified content type and context.
 */
- (instancetype)initWithContentType:(ComprobeFrameParserResultContentType)comprobeFrameContentType context:(id)contextObject;

@end
