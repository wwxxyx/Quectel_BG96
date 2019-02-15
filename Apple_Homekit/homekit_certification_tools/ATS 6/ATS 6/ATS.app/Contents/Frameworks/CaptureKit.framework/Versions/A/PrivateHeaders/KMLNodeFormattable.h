//
//  KMLSerializable.h
//  ATSMacApp
//
//  Created by Natália Cabral on 11/18/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol KMLNodeFormattable <NSObject>

/*!
    Converts the object to KML format
    
    @return a KML node
 */
- (NSXMLElement *)kmlFormat;

@end
