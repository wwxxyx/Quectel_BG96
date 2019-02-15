//
//  IdentificationMessagesUsedDetailsViewSection.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 3/1/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/StringDetailsViewSection.h>

@interface IdentificationMessagesUsedDetailsViewSection : StringDetailsViewSection

/*!
 @method    identificationMessagesUsedSectionWithData:isSent:title:identifier:
 @abstract  Initializes a details view section with the given messages sent or received blob
 */
+ (instancetype)identificationMessagesUsedSectionWithData:(NSData *)blob isSent:(BOOL)isSent title:(NSString *)title identifier:(NSString *)identifier;

@end
