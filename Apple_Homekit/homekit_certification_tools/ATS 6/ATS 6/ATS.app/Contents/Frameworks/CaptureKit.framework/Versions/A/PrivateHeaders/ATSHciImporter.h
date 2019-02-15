//
//  ATSHciImporter.h
//  ATSImportKit
//
//  Created by Edgard Lima on 10/17/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ATSImportKit/Hci.h>
#import <ATSImportKit/IKConstants.h>
#import <ATSImportKit/ATSImportLogger.h>

@class ATSCaptureImporter;

@interface ATSHciImporter : NSObject

/// The index of the RFCOMM stream to import.
@property NSInteger streamIndex;

/// A flag indicating whether the accessory is assumed to be the ACL sender.
@property BOOL accessoryIsSender;

/// A flag indicating whether or not the source file contains iAP1 data.
@property BOOL containsiAP1Data;

/// Keeps a list for RFCOMM channels found in the first step of parsing. You can use this to list in the UI, and the user could choose wich ones to be delivered as an RFCOMM event. And then the event listener could show the on UI and parse iAP content on it.
@property NSMutableArray *pendingRfcommStreams;

/**
   Constructs a ATSHciImporter object with a reference to ATSCaptureImporter. Parsed packets are deliverd troughout ATSCaptureImporter.
   @param atsImporter The importer the parsed packets are deliverd troughout.
   @return a new ATSHciImporter object.
 */
- (instancetype)initWithAtsImporter:(ATSCaptureImporter*) atsImporter;

/**
 Decods an HCI ACL frame. A state machine keeps track of fragmented L2CAP frames. IKBluetoothAclImportPacket, IKBluetoothL2CAPImportPacket and IKBluetoothRFCOMMImportPacket may be deliverd trough the referenced ATSCaptureImporter. See initWithAtsImporter.
 Creates a fake HCI ACL frame from payload. It may be usefull to convert ComprobeFrame into HCI ACL.
 
 @param payloadData the payload the will carried by the ACL frame.
 @param timestamp absolute timestamp in nano-seconds, since Epoch, of the packet being decoded
 @param source If the packet being decoded by decodeACL is coming for Apple Device or Accessory.
 
 @return TRUE if it has being decoded without any errors.
 */
- (BOOL)decodeACL:(NSData *)aclData timestamp:(UInt64)timestamp source:(IKPacketSource)source logger:(ATSImportLogger *)logger;

/**
   Creates a fake HCI ACL frame from payload. It may be usefull to convert ComprobeFrame into HCI ACL.
   @param payloadData the payload the will carried by the ACL frame.
   @param packetBoundary says if it is a L2CAP continuation, start or non-fragmentation.
*/
+ (NSData*) aclFrameDataFromPayload:(NSData*)payloadData packetBoundary:(AclPL2capPacketBoundary)packetBoundary;

@end

