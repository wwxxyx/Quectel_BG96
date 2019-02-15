//
//  BasebandDefs.h
//  Header file for Baseband types and definitions.
//
//  Created by Edgard Lima on 08/18/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

/// 3 bits (amAddr)
extern const UInt8 kBasebandLtAddressBroadcast;

/// 1 bit. In ACL logical transport only
extern const UInt8 kBasebandLinkFlowStop;
/// 1 bit. In ACL logical transport onlyp;
extern const UInt8 kBasebandLinkFlowGo;
/// 1 bit. eSCO transmission
extern const UInt8 kBasebandLinkFlowESCOTx;
/// 1 bit. Connectionless Slave Broadcast transmission (ignored in RX)
extern const UInt8 kBasebandLinkFlowCSBTx;

/// 1 bit. received payload with CRC
extern const UInt8 kBasebandLinkARQNAck;
/// 1 bit. received payload with bad CRC
extern const UInt8 kBasebandLinkARQNNack;
/// 1 bit. Connectionless Slave Broadcast transmission (ignored in RX)
extern const UInt8 kBasebandLinkARQNCBSTx;

/// 1 bit. Connectionless Slave Broadcast transmission (ignored in RX)
extern const UInt8 kBasebandLinkSEQNCBSTx;

/// 1 bit. L2CAP flow-off
extern const UInt8 kBasebandL2CAPFlowStop;
/// 1 bit. L2CAP flow-on
extern const UInt8 kBasebandL2CAPFlowGo;

typedef NS_ENUM(UInt8, BasebandChannelType) {
    /// Undefined (N.A. Not Applicable)
    BasebandChannelTypeUndefined = 0x0,
    /// L2CAP Continuation
    BasebandChannelTypeBasebandLogicalLinkContinuation = 0x1,
    /// L2CAP Start or No Fragmentation.
    BasebandChannelTypeBasebandLogicalLinkStartOrNoFragmentation = 0x2,
    /// LMP
    BasebandChannelTypeBasebandLogicalLinkLMP = 0x3
};
