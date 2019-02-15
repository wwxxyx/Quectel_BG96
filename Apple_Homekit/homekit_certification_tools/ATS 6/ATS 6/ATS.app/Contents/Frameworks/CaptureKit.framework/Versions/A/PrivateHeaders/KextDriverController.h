//
//  KextDriverController.h
//  BBSniff
//
//  Created by Bob Burrough on 9/15/09.
//  Copyright 2009 Apple Inc.. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface Version : NSObject {
    NSUInteger major;
    NSUInteger minor;
    NSUInteger revision;
}

- (id)initWithVersionString:(NSString *)vers;
- (id)initWithMajor:(NSUInteger)majorArg minor:(NSUInteger)minorArg revision:(NSUInteger)revisionArg;
- (BOOL)isGreaterThanVersion:(Version *)vers;
- (BOOL)isGreaterThanOrEqualToVersion:(Version *)vers;

@end

@interface KextDriverController : NSObject {
    Version *version;
}

@property (nonatomic, copy, readonly) NSString *bundleIdentifier;
@property (nonatomic, copy, readonly) NSString *installerPackageDriverVersion;
@property (nonatomic, copy, readonly) NSString *installerPackagePath;

+ (id)comProbeKextDriverController;

- (id)initWithBundleIdentifier:(NSString *)bundleIdentifier installerPackageDriverVersion:(NSString *)installerPackageDriverVersion installerPackagePath:(NSString *)installerPackagePath;
- (id)initWithBundleIdentifier:(NSString *)bundleIdentifier installerPackageDriverVersion:(NSString *)installerPackageDriverVersion installerPackageName:(NSString *)installerPackageName;

- (NSString *)getVersionString;
- (BOOL)isKextInstalled;
- (void)setVersionWithString:(NSString *)vers;
- (BOOL)needsUpdate;
- (Version *)bundledVersion;
- (void)updateDriverBlocking:(BOOL)blocking;

@end
