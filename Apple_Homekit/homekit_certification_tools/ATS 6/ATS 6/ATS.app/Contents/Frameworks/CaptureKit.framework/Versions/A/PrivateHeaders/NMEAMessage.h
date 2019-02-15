//
//  NMEAMessage.h
//  ATSMacApp
//
//  Created by Natália Cabral on 12/11/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/NMEAField.h>
#import <CaptureKit/NMEAUtil.h>

extern UInt8 const NMEAMessageSentenceIdentifierIndex;
extern UInt8 const NMEAMessageTimestampIndex;
extern NSString *const NMEAMessageGPGGASentenceIdentifier;
extern NSString *const NMEAMessageGPRMCSentenceIdentifier;
extern NSString *const NMEAMessageGPGSVSentenceIdentifier;
extern NSString *const NMEAMessageGPHDTSentenceIdentifier;
extern NSString *const NMEAMessagePASCDSentenceIdentifier;
extern NSString *const NMEAMessagePAGCDSentenceIdentifier;
extern NSString *const NMEAMessagePAACDSentenceIdentifier;

typedef enum : UInt8 {
                   NMEAMessageStatusOk = 1,
                   NMEAMessageStatusWithoutSentence,
                   NMEAMessageStatusWithoutIdentifier,
                   NMEAMessageStatusInvalidIdentifier,
                   NMEAMessageStatusWithoutChecksum,
                   NMEAMessageStatusInvalidChecksum,
                   NMEAMessageStatusUnformattedChecksum,
                   NMEAMessageStatusInvalidSize,
               } NMEAMessageStatus;


@interface NMEAMessage : NSObject

/*!
 The NMEA message identifier that describes tha type of sentence
 */
@property (nonatomic, strong) NSString *sentenceIdentifier;

/*!
 The NMEA sentence checksum
 */
@property (nonatomic) UInt8 checksum;

/*!
    The status of the NMEA message
 */
@property (nonatomic) NMEAMessageStatus status;

/*!
    The NMEA sentence components without the checksum in NSString format
 */
@property (nonatomic, strong) NSMutableArray *components;

/*!
    The NMEA sentence
 */
@property (nonatomic, strong) NSString *sentence;

/*!
    Initializes a new NMEAMessage with the specified message
    
    @param nmeaMessage  the NMEA sentence
    @return a new NMEAMessage
 */
- (instancetype)initWithNMEAMessage:(NSString *)nmeaMessage;

/*!
    Constructs a new NMEAMessage with the correct type with specified message
 
    @param nmeaMessage  the NMEA sentence
    @return a new NMEAMessage
 */
+ (NMEAMessage *)messageWithNMEAMessage:(NSString *)nmeaMessage;

/*!
    The list of NMEA field indexes that are
 
    @return an array with the list of required field indexes
 */
- (NSArray *)requiredFieldIndexes;

/*!
    Calculates the correct lentgh of the NMEA sentence in order to contain all the fields
 
    @return the sentence expected size
 */
- (UInt8)expectedSize;

/*
    Returns the minimum message size, which is the size of the message when all subfields that can have multiple values are left empty
 */
+ (UInt8)minimumSize;

/*!
    Returns an error description if the sentence could not be properly parsed.
 
    @return the nmea sentence error description
 */
- (NSString *)errorDescription;

/*!
    Load all the components as NMEAFields
 */
- (void)loadComponents;

/*!
 Returns an array containing all sentence identifier
 */
+ (NSArray *)NMEAMessagesIdentifiers;

@end
