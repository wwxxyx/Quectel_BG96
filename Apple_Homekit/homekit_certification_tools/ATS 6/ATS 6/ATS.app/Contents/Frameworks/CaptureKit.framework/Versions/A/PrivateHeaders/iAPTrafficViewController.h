//
//  iAPTrafficViewController.h
//  ATS
//
//  Created by Joe Basu on 1/15/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/TrafficViewController.h>
#import <CaptureKit/iAP1PacketEvent.h>
#import <CaptureKit/ATSRuleEngine.h>

@interface iAPTrafficViewController : TrafficViewController {
    NSString *_keyPrefix;

    CGFloat _descriptionColumnWidth;

    BOOL _includesSyncBytes;

    BOOL (^_syncByteFilterBlock)(CaptureEvent *event);
}

@property (nonatomic, copy) NSString *keyPrefix;
@property (nonatomic, assign) BOOL includesSyncBytes;

/*!
 @method    transformPacket:safeToAccessFields:
 @abstract  Determines whether or not the packet's command property is safe to access via -fieldForKey:, and may modify the packet to make it valid if necessary. For example, a SetFIDTokenValues packet with invalid tokens will have the invalid tokens stripped and a new valid packet returned
 @param     packetEvent
            The incoming packet event
 @param     safeToAccessFields
            BOOL assigned to when the returned event should be passed into entities that want to examine its fields
 @return    The transformed packet, if any transformation was required
 */
- (iAP1PacketEvent *)transformPacket:(iAP1PacketEvent *)packetEvent safeToAccessFields:(BOOL *)safeToAccessFields;

/*!
 @method    recordPacketError:
 @abstract  Posts an ATSRuleFailedNotification upon a packet error (badly formatted packet or token)
 @param     error
            RuleError instance
 */
- (void)recordPacketError:(NSError *)error;

+ (iAPTrafficViewController *)sampleiAPTrafficViewControllerWithRect:(NSRect)rect;

@end
