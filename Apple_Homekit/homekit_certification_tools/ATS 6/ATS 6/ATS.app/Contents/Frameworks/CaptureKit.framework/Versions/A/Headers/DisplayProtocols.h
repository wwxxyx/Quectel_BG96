//
//  DisplayProtocols.h
//  ATSMacApp
//
//  Created by Marin Balde on 8/31/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

@class DetailsViewTree;
@class APReportAnalyzerEntry;

@protocol CommonDisplay
@required
- (NSString *)briefDescription;
- (NSImage *)statusImage;
- (NSString *)statusDescription;
- (DetailsViewTree *)detailsViewTreeDescription;
- (NSString *)valuePlaceholder;
- (NSString *)sourceDescription;
@end

@protocol ATFDisplay <CommonDisplay>
@required
- (NSString *)localTimestamp;
- (NSString *)typeDescription;
- (NSString *)payloadDescription;
@end

@protocol ATFR3Display <CommonDisplay>
@required
- (NSString *)commandDescription;
- (NSString *)atfPayloadDescription;
- (NSString *)description;
@end

@protocol IDBusDisplay <CommonDisplay>
@required
- (NSImage *)statusImage;
- (NSString *)commandDescription;
- (NSString *)payloadDescription;
- (DetailsViewTree *)detailsViewTreeDescription;
@end

@protocol USBPacketDisplay <CommonDisplay>
@required
- (NSString *)packetIDDescription;
- (NSString *)payloadDescription;
@end

@protocol USBTransactionDisplay <CommonDisplay>
@required
- (NSString *)typeDescription;
- (NSString *)addressDescription;
- (NSString *)endpointDescription;
- (NSString *)lengthDescription;
- (NSString *)transactionDescription;
@end

@protocol USBTransferDisplay <CommonDisplay>
@required
- (NSString *)typeDescription;
- (NSString *)addressDescription;
- (NSString *)endpointDescription;
- (NSString *)lengthDescription;
- (NSString *)transferDescription;
@end

@protocol USBHIDReportDisplay <CommonDisplay>
@required
- (NSString *)typeDescription;
- (NSString *)reportIDDescription;
- (NSString *)lengthDescription;
- (NSString *)continuationDescription;
- (NSString *)moreToFollowDescription;
- (NSString *)reportDescription;
@end

@protocol iAPPacketDisplay <CommonDisplay>
@required
- (NSString *)lingoDescription;
- (NSString *)commandDescription;
- (NSString *)transactionIDDescription;
- (NSString *)payloadDescription;

@end

@protocol iAP2LinkPacketDisplay <CommonDisplay>
@required
- (NSString *)typeDescription;
- (NSString *)sequenceNumberDescription;
- (NSString *)acknowledgementNumberDescription;
- (NSString *)lengthDescription;
- (NSString *)payloadDescription;
@end

@protocol iAP2MessageDisplay <CommonDisplay>
@required
- (NSString *)nameDescription;
- (NSString *)messageDescription;
@end

@protocol iAP2FileTransferDatagramDisplay <CommonDisplay>
@required
- (NSString *)identifierDescription;
- (NSString *)typeDescription;
- (NSString *)payloadDescription;
- (NSString *)payloadSizeDescription;
@end

@protocol ReportAnalyzerDisplayProtocol
@required

/*!
 @method    reportAnalyzerEntry
 @abstract  Generates and returns a report analyzer entry for this event
 */
- (APReportAnalyzerEntry *)reportAnalyzerEntry;

@end
