//
//  ComprobeDataFrameParser.h
//  ComprobeSupport
//
//  Created by Henrique Magalhaes on 9/4/13.
//  Copyright (c) 2013 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

@class ComprobeFrame;
@class ComprobeFrameParserResult;

@interface ComprobeDataFrameParser : NSObject 

/*!
 @method    parseComprobeFrame:
 @abstract  parse the captured comprobe frame
 @param     comprobeFrame The captured comprobe frame
 @return    Return a ComprobeFrameParserResult with the parsed comprobe frame
 */
- (ComprobeFrameParserResult *)parseComprobeFrame:(ComprobeFrame *)comprobeFrame;

@end
