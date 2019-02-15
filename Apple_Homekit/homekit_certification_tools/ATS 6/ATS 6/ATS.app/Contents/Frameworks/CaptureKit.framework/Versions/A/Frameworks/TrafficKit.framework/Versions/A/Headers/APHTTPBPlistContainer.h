//
//  APHTTPBPlistContainer.h
//  TrafficKit
//
//  Created by Mark Hamlin on 6/5/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <TrafficUIKit/APReportAnalyzerEntry.h>

NS_ASSUME_NONNULL_BEGIN

@protocol APHTTPBPlistReportAnalyzerDataSource

/*!
 @method    entryForBPlistContainer:key:
 @abstract  Opportunity for the data source to supply a report analyzer entry to override the default
 */
- (nullable APReportAnalyzerEntry *)entryForBPlistContainer:(id)container key:(NSString *)key;

@end

@protocol APHTTPBPlistDataSource

/*!
 @method        representationForDataInContainer:key:errorDescription:
 @abstract      Opportunity for the data source to supply a higher level representation for a data instance
 @discussion    If the delegate returns an object and also an error, the errorDescription will be set for the key. It is then up to the owner of the object to do something with that error later
 */
- (id)representationForDataInContainer:(id)container key:(NSString *)key errorDescription:(NSString *_Nonnull*_Nonnull)errorDescription;

@end

/**
 Error Domain for APHTTPBPlistContainerError.
 */
extern const NSString *APHTTPBPlistContainerErrorDomain;

/*!
 The collection of codes that represent each kind of parse error that can be 
 created while parsing APHTTPBPlistContainer objects.
 */
typedef NS_ENUM(NSUInteger, APHTTPBPlistContainerErrorCode) {
    APHTTPBPlistContainerErrorCodeUnknown = 0,
    APHTTPBPlistContainerErrorCodeMissingRequiredKey,
    APHTTPBPlistContainerErrorCodeUnknownKey,
    APHTTPBPlistContainerErrorCodeValueUnexpectedType,
    APHTTPBPlistContainerErrorCodeReservedBitUsed,
    APHTTPBPlistContainerErrorCodeDataRepresentation,
};

@interface APHTTPBPlistContainerError : NSError

#pragma mark - APHTTPBPlistContainerError Initialization

/*!
 Add a new ParseError that occurs on a key within a container
 
 @param error The leaf error
 @param key The container key
 */
+ (instancetype)errorWithSubContainerError:(APHTTPBPlistContainerError *)error onKey:(NSString *)key;

/*!
 New error for a missing required key
 @param key The affected key
 */
+ (instancetype)missingRequiredKey:(NSString *)key;

/*!
 New error for a value of the wrong type
 @param expectedType The expected type of the key's value
 @param key The affected key
 */
+ (instancetype)valueNotExpectedType:(NSString *)expectedType forKey:(NSString *)key;

/*!
 New error for a unknown key
 @param key The affected key
 */
+ (instancetype)unknownKey:(NSString *)key;

/*!
 New error for a reserved bit that was used
 @param key The affected key
 */
+ (instancetype)reservedBitUsedForKey:(NSString *)key;

/*!
 New error for a data representation error
 @param error The data representation error
 @param key The affected key
 */
+ (instancetype)dataRepresentation:(NSString *)error forKey:(NSString *)key;

#pragma mark APHTTPBPlistContainerError Inheritance-free Identification

+ (BOOL)isAPHTTPBPlistContainerError:(NSError *)error;

/*!
 The affected key that the error occurred on
 */
+ (nullable NSString *)affectedKeyPathForError:(NSError *)error;

/*!
 The string (human-readable) identifier corresponding to the code.
 
 Returns nil if the error has the wrong domain and an invalid code.
 */
+ (nullable NSString *)identifierForError:(NSError *)error;

/*!
 The set of keys that identify the occurence of this error (just the affectedKey in this case)
 
 Returns nil if the error has the wrong domain and an invalid code.
 */
+ (nullable NSSet<NSString *> *)identificationKeysForError:(NSError *)error;

@end

@interface APHTTPBPlistContainer : NSObject

/*!
 @method    allKnownKeys
 @abstract  Return an array of all known key names, including those returned from the superclass (if subclassed)
 */
+ (NSArray *)allKnownKeys;

/*!
 @method    initWithDictionary:errorDescription:dataSource:
 @abstract  Unpack to container object with the passed dictionary. Returns nil on error
 */
- (id)initWithDictionary:(NSDictionary *)dictionary errorDescription:(NSString *_Nonnull*_Nonnull)errorDescription dataSource:(id<APHTTPBPlistDataSource>)dataSource;

/*!
 @method    reportAnalyzerEntryUsingDataSource:
 @abstract  Report analyzer entry to describe the contents
 */
- (APReportAnalyzerEntry *)reportAnalyzerEntryUsingDataSource:(id<APHTTPBPlistReportAnalyzerDataSource>)dataSource;

/*!
 @method        keysWithErrors
 @abstract      Returns an array of keys with errors
 @discussion    These are all the keys that were passed to addErrorDescription:forKey:
 */
- (NSArray *)keysWithErrors;

/*!
 @method    addErrorDescription:forKey:
 @abstract  Sets an error message for the passed key. If using AHTTPBPlistDataSource, this will be done for you
 */
- (void)addErrorDescription:(NSString *)errorDescription forKey:(NSString *)key;

/*!
 @method    addErrorDescription:forUnknownKey:
 @abstract  Sets an error message for the passed key, which the superclass will populate an error for
 */
- (void)addErrorDescription:(NSString *)errorDescription forUnknownKey:(NSString *)key;

/*!
 @method    errorDescriptionForKey:
 @abstract  Returns the error description for the passed key, or nil if there's no error
 */
- (NSString *)errorDescriptionForKey:(NSString *)key;

/*!
 The collection of parse errors on the object. (These are the underlying errors)
 */
@property (nonatomic, readonly) NSArray<NSError *> *parseErrors;

/*!
 Use this to add a a parse error to the object
 
 @param error The APHTTPBPlistContainerError that was encountered
 */
- (void)addError:(APHTTPBPlistContainerError *)error;

NS_ASSUME_NONNULL_END

@end
