//
//  CertificateCollection.h
//  ATSMacApp
//
//  Created by Marin Balde on 10/21/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class LKTCommand;

@interface CertificateCollection : NSObject {
    UInt8 _maximumSectionIndex;
    NSMutableArray *_certDataSections;
    NSData *_certSerialNumberData;
    SInt16 _certClass;
    SInt16 _authMajor;
    SInt16 _authMinor;
}

@property (nonatomic, readonly) NSString *certSerial;
@property (nonatomic, readonly, assign) SInt16 certClass;
@property (nonatomic, readonly, assign) SInt16 authMajor;
@property (nonatomic, readonly, assign) SInt16 authMinor;

- (void)processGetAccessoryAuthenticationInfoCommand:(LKTCommand *)command;
- (void)processRetAccessoryAuthenticationInfoCommand:(LKTCommand *)command;
- (void)reset;

- (void)updateSerialNumberDataFromCertData:(const NSData *)certData;
- (void)updateCertClass;

@end
