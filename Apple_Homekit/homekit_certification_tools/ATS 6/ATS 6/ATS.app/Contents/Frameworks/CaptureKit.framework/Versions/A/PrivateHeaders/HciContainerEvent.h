//
//  L2CAPFrameEvent.h
//  ATSMacApp
//
//  Created by Edgard Lima on 08/13/18.
//  Copyright (c) 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureKit.h>
#import <ATSImportKit/IKBluetoothHciContainerImportPacket.h>

@interface HciContainerEvent : CaptureEvent<ReportAnalyzerDisplayProtocol>

+ (instancetype)eventWithTimestamp:(UInt64)timestamp hciContainerPacket:(IKBluetoothHciContainerImportPacket *)hciContainerPacket;

- (APReportAnalyzerEntry *)reportAnalyzerEntry;

@property IKBluetoothHciContainerImportPacket * hciContainerPacket;

@end
