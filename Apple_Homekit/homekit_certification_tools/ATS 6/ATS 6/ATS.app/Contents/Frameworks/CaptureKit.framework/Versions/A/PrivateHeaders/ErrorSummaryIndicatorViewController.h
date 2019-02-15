//
//  ErrorSummaryIndicatorViewController.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 7/26/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ErrorSummaryIndicatorViewController : NSViewController <NSCollectionViewDelegate>

/*!
 @method    init
 @abstract  Default init, loads with correct xib
 */
- (id)init;

#if APPLE_INTERNAL
/*!
 @method        setInternalTag:
 @abstract      Sets the string displayed on the internal tag
 @discussion    Available for internal builds only. Otherwise not displayed
 @param         tag
                The tag string to display
 */
- (void)setInternalTag:(NSString *)tag;
#endif

/*!
 @method    setCountOfErrors:warnings:
 @abstract  Sets the displayed count of errors and warnings
 @param     errorCount
            Count of errors
 @param     warningCount
            Count of warnings
 */
- (void)setCountOfErrors:(NSUInteger)errorCount warnings:(NSUInteger)warningCount;

@end

/*!
 @class     ErrorSummaryRuleErrorItemViewController
 @abstract  Represents a rule error or warning as an icon plus a number
 */
@interface ErrorSummaryRuleErrorItemViewController : NSCollectionViewItem
@property (nonatomic, assign) IBOutlet NSImageView *icon;
@property (nonatomic, assign) IBOutlet NSTextField *count;
@end
