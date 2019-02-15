//
//  APPacket.h
//  APTS
//
//  Created by Mark Hamlin on 6/22/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <TrafficUIKit/TrafficUIKit.h>

// We used to import pcap.h here, but that was causing non-modular-header problems and we really just need to forward-declare this one struct
struct pcap_pkthdr;

#define kEthernetLinkLayerType 1
#define kNULLLinkLayerType 0

/*!
 @class         APPacket
 @abstract      Represents a generic packet
 @discussion    This class is the base class for a generic packet, and provides utilities for determining packet base types
 */
@interface APPacket : NSObject

/*!
 @property      timestamp
 @abstract      Timestamp for this packet in seconds
 @discussion    The timestamp is in seconds, and is absolute
 */
@property (readonly, nonatomic, assign) double timestamp;

/*!
 @property      rawData
 @abstract      The raw captured data
 */
@property (readonly, nonatomic, strong) NSData *rawData;

/*!
 @property      encapsulatingPacket
 @abstract      The packet encapsulating this one
 */
@property (nonatomic, strong) APPacket *encapsulatingPacket;

/*!
 @property      encapsulatedPacket
 @abstract      The packet which this one contains
 */
@property (nonatomic, weak) APPacket *encapsulatedPacket;

/*!
 @property      interface
 @abstract      The interface this was captured on
 */
@property (nonatomic, strong) NSString *interface;

/*!
 @property      lengthOnWire
 @abstract      How long this packet was on the wire. "rawData"  may be shorter due to snaplength restrictions
 */
@property (readonly, nonatomic, assign) unsigned int lengthOnWire;

/*!
 The link layer type, determined by finding the link layer level
 */
- (int)linkLayerType;

/*!
 @method    rootPacket
 @abstract  Follows the protocol encapsulation list to the root (the base APPacket instance)
 @return    Root packet instance
 */
- (APPacket *)rootPacket;

/*!
 Follows protocol encapsulation list until a match is found
 @param     classObject
            The class representing the protocol
 @return    Instance in the encapsulation list with this protocol/class
 */
- (id)packetWithClass:(Class)classObject;

/*!
 @method        packetWithHeader:data:linkLayerType:
 @abstract      Returns a high-level packet given the captured data
 @param         header
                pcap header. Does not keep an out of scope reference
 @param         data
                packet data. Does not keep an out of scope reference
 @param         linkLayerType
                The type of link layer this packet was captured from
 @discussion    Determines the proper APPacket subclass and creates that
 */
+ (APPacket *)packetWithHeader:(struct pcap_pkthdr *)header data:(const uint8_t *)data linkLayerType:(int)linkLayerType;

/*!
 @method        packetWithRawData:classes:
 @abstract      Construct a layered packet with data when the classes are known. Doing this bypasses the encapsulated type detection and directly allocates them
 @param         classes
                Array of APPacket subclasses (this is checked), with the highest-level class first (eg. @[ TCP, IPv4, Eth ])
 */
+ (APPacket *)packetWithRawData:(NSData *)rawData classes:(NSArray *)classes timestamp:(double)timestamp lengthOnWire:(unsigned int)lengthOnWire;

/*!
 @method        pcapHeaderForRepresentativePacket:
 @abstract      Generic pcap file header data
 */
+ (NSData *)pcapHeaderForRepresentativePacket:(APPacket *)packet;
+ (NSData *)pcapHeaderForLinkLayerType:(int)linkLayerType;

/*!
 Returns the file entry which can be written to a .pcap file
 
 @note absoluteTimestampSince1970 is needed because the packet itself only stores relative timestamp (ATS nuance).
 */
- (NSData *)pcapFileEntryWithAbsoluteTimestampSince1970:(UInt64)absoluteTimestampSince1970;

#pragma mark -
#pragma mark Subclassers

/*!
 @method    name
 @abstract  Name of the type of packet
 */
- (NSString *)name;

/*!
 @method    reportAnalyzerEntry
 @abstract  Construct and return a report analyzer entry, for only this packet
 */
- (APReportAnalyzerEntry *)reportAnalyzerEntry;

/*!
 @method        allowedParentProtocols
 @abstract      Which protocols may contain this packet
 @return        Set of allowed protocols
 */
+ (NSSet *)allowedParentProtocols;

/*!
 @method        restrictedChildProtocols
 @abstract      Set of protocols allowed as children in the payload
 @return        Set of protocol classes allowed as children in the payload, or nil for no restriction
 @discussion    If the child type is known, a set with 1 protocol can be returned. For example, IP will know if the child is TCP or UDP, so it should return a set with only one of those.
 */
- (NSSet *)restrictedChildProtocols;
- (NSSet *)restrictedChildProtocolsWithLinkLayerType:(int)linkLayerType;

/*!
 @method        packetFromData:length:parent:consumed:
 @abstract      Whether or not this data describes the current class, and performs parsing here if it is
 @param         data
                Raw bytes (starting from end of parent)
 @param         length
                Length of data remaining
 @param         parent
                The parent (encapsulating) packet
 @param         consumed
                How many bytes were consumed in creating this packet
 @return        APPacket instance, or nil if it couldn't be parsed
 */
+ (id)packetFromData:(const uint8_t *)data length:(unsigned int)length parent:(id)parent consumed:(unsigned int *)consumed;

@end
