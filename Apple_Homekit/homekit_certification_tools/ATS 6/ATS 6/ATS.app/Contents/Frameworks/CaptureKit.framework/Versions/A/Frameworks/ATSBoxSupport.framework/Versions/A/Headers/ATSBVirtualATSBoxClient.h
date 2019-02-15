/*
 *  ATSBVirtualATSBoxClient.h
 *  ATSBoxSupport
 *
 *  Created by Joe Basu on 5/14/10.
 *  Copyright © 2010 Apple Inc. All Rights Reserved.
 *
 */


@protocol ATSBVirtualATSBoxClient <NSObject>

- (void)processATFData:(NSData *)data;

@end
