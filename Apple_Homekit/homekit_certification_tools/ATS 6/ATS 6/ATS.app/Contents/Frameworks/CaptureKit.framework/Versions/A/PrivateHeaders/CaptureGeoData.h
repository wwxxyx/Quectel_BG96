//
//  CaptureGeoData.h
//  ATSMacApp
//
//  Created by Natália Cabral on 12/11/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/EventBroker.h>

@interface CaptureGeoData : NSObject

/*!
    Initializes a new CaptureGeoData
 */
+ (CaptureGeoData *)captureGeoData;

/*!
    Add itself as a subscriber to the specified event broker
   
    @param  eventBroker
            the event broker
    @param  transport
            the transport the event should be on
 */
- (void)subscribeForEventBroker:(EventBroker *)eventBroker transport:(CaptureTransport *)transport;

/*!
    Create a CSV formatted report with the captured geo data
 
    @return a geo data report in CSV format
 */
- (NSString *)geoDataAsCSV;

/*!
    Create a KML formatted report with the captured geo data
 
    @return a geo data report in KML format
 */
- (NSString *)geoDataAsKML;

@end
