//
//  AttributePseudoEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 7/14/11.
//  Copyright 2011 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CaptureKit/CaptureEvent.h>
#import <CaptureKit/TrafficAttribute.h>

@interface AttributePseudoEvent : CaptureEvent {
}
@property (nonatomic, readonly, strong) TrafficAttribute *attribute;
@property (nonatomic, readonly) NSViewController *viewController;

- (id)initWithAttribute:(TrafficAttribute *)attribute;

- (NSComparisonResult)compareAttribs:(AttributePseudoEvent *)event;

- (BOOL)aboveAssociatedEvent;

- (NSCell *)makeCell;

- (void)updateObjectValue:(id)value;
- (id)objectValue;

/*!
 @method    cleanup
 @abstract  Removes view controller's view from superview immediately
 */
- (void)cleanup;

@end
