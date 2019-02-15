//
//  BtsnoopTypes.h
//  Header for BTSnoop types.
//
//  Created by Edgard Lima on 09/12/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString * kBtsnnopException;

typedef NS_ENUM(UInt32, BtsnoopDataLinkType) {
    // 0 - 1000 - reserved.
    /// Un-encapsulated HCI (H1). Raw HCI packets without framing.
    BtsnoopDataLinkTypeH1 = 1001,
    /// HCI UART (H4). HCI UART packets including packet type header.
    BtsnoopDataLinkTypeH4 = 1002,
    /// HCI BSCP. HCI Bluecore Serial Protocol including framing.
    BtsnoopDataLinkTypeBSCP = 1003,
    /// HCI Serial (H5). HCI 3 wire UART packets including framing.
    BtsnoopDataLinkTypeH5 = 1004,
    // 1005 - 4294967295 - Unassigned
};
