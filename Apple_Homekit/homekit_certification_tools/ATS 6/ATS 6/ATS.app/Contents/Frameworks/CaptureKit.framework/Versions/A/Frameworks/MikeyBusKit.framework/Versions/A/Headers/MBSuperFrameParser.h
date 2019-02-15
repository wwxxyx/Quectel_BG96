//
//  MBSuperFrameParser.h
//  MikeyBusKit
//
//  Created by Wesley McCloy on 9/18/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

/*!
 @header        MBSuperFrameParser
 @discussion    This header defines the functions and properties used to interface with a SuperFrame Parsing operation.
 */

#import <Foundation/Foundation.h>

@class MBSuperFrame;

@interface MBSuperFrameParser : NSObject

/*!
 Runs the parsing operation with the given data parameter and the current context.
 
 @param data A 16 bit unsigned integer as the input data to the parsing
 @return A SuperFrame, valid if complete and usable.
 */
-(MBSuperFrame *)processData:(uint16_t)data;

@end
