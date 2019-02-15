/*
 *  ATSR3BVirtualATSR3BoxClient.h
 *  ATSR3BoxSupport
 *
 *  Created by Marin Balde on 4/16/12.
 *  Copyright © 2012 Apple Inc. All Rights Reserved.
 *
 */


@protocol ATSR3BVirtualATSR3BoxClient <NSObject>

- (void)processATFR3Data:(NSData *)data;

@end
