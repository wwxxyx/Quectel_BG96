//
//  ImageCache.h
//  ATS
//
//  Created by Joe Basu on 2/4/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <Cocoa/Cocoa.h>

@interface ImageCache : NSObject

+ (NSImage *)redLightImage;
+ (NSImage *)greenLightImage;
+ (NSImage *)yellowLightImage;
+ (NSImage *)clearLightImage;

+ (NSImage *)greenCheckImage;
+ (NSImage *)redExclamationImage;
+ (NSImage *)grayExclamationImage;
+ (NSImage *)yellowExclamationImage;
+ (NSImage *)alertNoteImage;
+ (NSImage *)updateFirmwareImage;
+ (NSImage *)failImage;
+ (NSImage *)successImage;

@end
