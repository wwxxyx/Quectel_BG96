//
//  RuleHtmlBuilder.h
//  ATSMacApp
//
//  Created by Natália Cabral on 30/10/14.
//  Copyright (c) 2014 Apple. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface RuleHtmlBuilder : NSObject

/*!
    Singleton instance
 */
+ (RuleHtmlBuilder *)sharedBuilder;

/*!
    Create a html file with the list of all rules
    @param directory    the directory where the file will be saved
    @param error        pointer to error if the directory is invalid
    @return             YES if it succeded, NO otherwise
 */
- (BOOL)createAllRuleListFileInDirectory:(NSString *)directory error:(NSError **)error;

/*!
    Create a html file with the list of latest rules
    @param directory    the directory where the file will be saved
    @param error        pointer to error if the directory is invalid
    @return             YES if it succeded, NO otherwise
 */
- (BOOL)createLatestRuleListFileInDirectory:(NSString *)directory error:(NSError **)error;

/*!
    Open the file with all rules placed at the specified directory
    @param directory    the directory where the file is saved
 */
- (void)openAllRuleListAtDirectory:(NSString *)directory;

/*!
    Open the file with the latest rules placed at the specified directory
    @param directory    the directory where the file is saved
 */
- (void)openLatestRuleListAtDirectory:(NSString *)directory;

/*!
    Validates all the Rule labels to make sure they map to an appropriate AI Spec XML's spec reference. Currently the only way to validate all the rules made by RULE macro is by building them and making sure they build fine.
 */
- (void)validateRuleLabels;

@end
