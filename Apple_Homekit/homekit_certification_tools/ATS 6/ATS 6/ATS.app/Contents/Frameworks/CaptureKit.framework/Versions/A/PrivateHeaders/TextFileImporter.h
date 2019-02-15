//
//  TextFileImporter.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 3/28/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TextFileImporter : NSObject

/*!
 @method    importFromURL:
 @abstract  Prompts for import options and creates a capture docment
 */
+ (void)importFromURL:(NSURL *)url;

@end
