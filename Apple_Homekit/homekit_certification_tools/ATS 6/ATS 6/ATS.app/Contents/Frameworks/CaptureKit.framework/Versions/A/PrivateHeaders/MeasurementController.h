//
//  MeasurementController.h
//  iAPCapture
//
//  Created by David Silver on 7/19/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class Measurement;
@class MeasurementCollection;

@interface MeasurementController : NSObject {
    NSMutableDictionary *_measurementCollections; //Measurement types (strings, with defaults declared below) are keys
}

+ (NSDictionary *)measurementTypeAttributesDictionaryForType:(NSString *)type; //Dictionary contains information/settings about the corresponding type
                                                                               //Keys within dictionary are listed below

- (MeasurementCollection *)measurementCollectionForType:(NSString *)type;
- (void)addMeasurement:(Measurement *)measurement ofType:(NSString *)type;
- (void)updateCollectionsUsingTimestamp:(UInt64)timestamp; //Adds a fake measurement equal at the given timestamp to each collection equal to the last real measurement
@end

/* Measurement type declarations */
extern NSString *MeasurementTypeAccDetectLogicLevel;
extern NSString *MeasurementTypeiPodDetectLogicLevel;
extern NSString *MeasurementTypeAccPowerLogicLevelAtiPod;
extern NSString *MeasurementTypeResistorID;
extern NSString *MeasurementTypeAccUSBVbusVoltage;
extern NSString *MeasurementTypeAccUSBDPlusVoltage;
extern NSString *MeasurementTypeAccUSBDMinusVoltage;
extern NSString *MeasurementTypeAccAverageCurrent;
extern NSString *MeasurementTypeAccPeakCurrent;

/* Keys for the measurementTypeAttributes dictionaries */
extern NSString *MeasurementTypeAttributesDictionaryNameKey;  //The name of the measurement
extern NSString *MeasurementTypeAttributesDictionaryUnitsKey; //The name of the measurement
extern NSString *MeasurementTypeAttributesDictionaryIsBooleanKey;
extern NSString *MeasurementTypeAttributesDictionaryShouldDisplayMinValueKey;         //Does it make sense to display the min value of this measurement?
extern NSString *MeasurementTypeAttributesDictionaryShouldDisplayMaxValueKey;         //Does it make sense to display the max value of this measurement?
extern NSString *MeasurementTypeAttributesDictionaryShouldDisplayAvgValueKey;         //Does it make sense to display the avg value of this measuremnet?
extern NSString *MeasurementTypeAttributesDictionaryGraphBackgroundRedComponentKey;   //The red component of the measurement's graph's backgound (0.0 to 1.0)
extern NSString *MeasurementTypeAttributesDictionaryGraphBackgroundGreenComponentKey; //The green component of the measurement's graph's backgound (0.0 to 1.0)
extern NSString *MeasurementTypeAttributesDictionaryGraphBackgroundBlueComponentKey;  //The blue component of the measurement's graph's backgound (0.0 to 1.0)
extern NSString *MeasurementTypeAttributesDictionaryTheoreticalMinValueKey;           //The minimum value the measurement can be - eg, a boolean value has a theoretical min of 0.  Theoretial limits set the initial scale of the graph, but can be overridden if data arrives outside these bounds.  Optional.
extern NSString *MeasurementTypeAttributesDictionaryTheoreticalMaxValueKey;           //The maximum value the measurement can be - eg, a boolean value has a theoretical max of 1.  Optional.
extern NSString *MeasurementTypeAttributesDictionaryAbsoluteMaxValueKey;              //The absolute maximum value to be used for scaling the graph.  Does not affect the initial display of the graph.  Only comes into play whenever a value outside this bound arrives.  If a value outside this bound arrives, the scale of the graph will respect this bound regardless of the measurement.  Optional.
