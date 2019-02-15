//
//  ParseError.h
//  ATSMacApp
//
//  Created by Wesley McCloy on 2/27/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 The ParseError subclass of NSError is designed to communicate issues with packets to the end user.
 These go from malformed headers to missing required keys; it is up to the protocol to create these
 errors and populate them with as much useful information as possible. Clients of CaptureKit (e.g.
 Facets) use these errors and their properties.
 
 A ParseError is an encapsulation of an `underlyingError`, this having been created at a lower level. 
 It is an NSError object, any useful information can be put in its `userInfo` dictionary.
 */
@interface ParseError : NSError

/**
 Initializes a new ParseError based on the supplied (lower level) error.
 
 @param error The underlying error that backs this ParseError
 @param identifier The identifier that is unique within the error's domain, if
 nil is supplied then it will come from the value for the "identifier" key in 
 the userInfo dictionary. If that isn't found then an assertion failure will
 occur.
 @param identificationKeys The keys that are used to identify the occurence of 
 this parse error. If nil is supplied, it will be created with the "identifier"
 key in the set.

 @return A new ParseError
 */
+ (nonnull instancetype)parseErrorFromError:(nonnull NSError *)error withIdentifier:(nullable  NSString *)identifier andIdentificationKeys:(nullable NSSet<NSString *> *)identificationKeys;

/**
 The identifier that is unique to the error's lower-level domain.
 */
@property (readonly, nonnull) NSString* identifier;

/**
 A dictionary of all the key/values that are considered necessary for identifying an occurance of a parse error. Includes:
 
 - Domain
 - Identifier
 - Low-level identification key/values
 
 It is at the discretion of the parse error creator to indicate which values are in the identification information. The intent is to be able to compare parse errors across multiple traces that are considered the "same".
 
 For example:
 - In CarPlay, if there's a missing required key "oemicon", this key ("oemicon") would be in the identification information dictionary. This is to distinguish it from other missing keys.
 - In CarPlay, if the value of the key "oemicon" is of the wrong data type, the key ("oemicon") would be part of the identification information but the expected data type isn't necessary to identify this occurence, so it is supplemental.
 - In Lightning Audio, even though an SODP error can happen at a unique slot, frame, with a specific value — these are not considered part of the identification information since there is no perceived value in tracking these values.
 
 At a minimum it contains the "identifier" and "domain" key/value pair.
 */
@property (readonly, nonnull) NSDictionary *identificationInformation;

/**
 The collection of keys that power the identificationInformation dictionary. Minus the domain
 
 At a minimum is contains the "identifier" key.
 */
@property (readonly, nonnull) NSSet<NSString *> *identificationKeys;


@end
