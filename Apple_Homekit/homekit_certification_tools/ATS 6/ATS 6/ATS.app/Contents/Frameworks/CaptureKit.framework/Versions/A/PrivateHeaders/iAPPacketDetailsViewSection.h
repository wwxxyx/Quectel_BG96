//
//  iAPPacketDetailsViewSection.h
//  ATS
//
//  Created by Joe Basu on 6/1/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/DetailsViewSection.h>
#import <LingoKit/LingoKit.h>

@interface iAPPacketDetailsViewSection : DetailsViewSection {
    LKTCommand *_command;
}

// should this be a copy instead of a retain?
@property (nonatomic, retain) LKTCommand *command;

@end
