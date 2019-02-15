//
//  Hci.h
//  Header file for HCI (Host-Controller Interface).
//
//  Created by Edgard Lima on 09/12/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(UInt8, AclPL2capPacketBoundary) {
    /// First non-automatically-flushable packet of Higher Layer Message (start of a non-automatically-flushable L2CAP PDU).
    AclPL2capPacketBoundaryStartOfNonAutoFlushable = 0x0,
    /// Continuing fragment packet of Higher Layer Message.
    AclPL2capPacketBoundaryContinuingFragment = 0x1,
    /// First automatically-flushable packet of Higher Layer Message (start of an automatically-flushable L2CAP PDU).
    AclPL2capPacketBoundaryStartOfAutoFlushable = 0x2,
    /// A complete L2CAP PDU. Automatically flushable.
    AclPL2capPacketBoundaryNotFragmented = 0x3
};
