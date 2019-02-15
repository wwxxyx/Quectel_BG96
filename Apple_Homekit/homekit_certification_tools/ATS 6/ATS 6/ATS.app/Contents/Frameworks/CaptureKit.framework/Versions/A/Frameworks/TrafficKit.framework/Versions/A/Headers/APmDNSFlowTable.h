//
//  APmDNSFlowTable.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 10/06/17.
//  Copyright (c) 2017 Apple Inc. All rights reserved.
//

@class APBonjourServiceInstance;
@class APmDNSPacket;
@class APmDNSFlowTable;

@protocol APmDSNBonjourServiceDelegate

/**
 Delegates receive every new Bonjour Service Instances seen over network.
 
 @note: Same copy might be sent more than once if new IP addresses for the Bonjour Service are detected.
 */
- (void)mDNSFlowTable:(APmDNSFlowTable *)flowTable didSeeBonjourService:(APBonjourServiceInstance *)bonjourServiceInstance;

@end

@interface APmDNSFlowTable: NSObject

/**
 Stores all the Bonjour service instances everytime a new one is created.
 */
@property (nonatomic, strong, readonly) NSArray <APBonjourServiceInstance *> *allBonjourServiceInstances;

/**
 Every CarPlay Service records are reported if the delegate is set.
 */
@property (weak) id <APmDSNBonjourServiceDelegate> bonjourServiceDelegate;

/**
 Return all the Bonjour service instances matching the input Bonjour service type.
 
 @param serviceType Bonjour service type string of the resulting Bonjour service instances
 
 @return Bonjour instances matching the input Bonjour service type.
 If there were no service instances matching the input service type string, nil is returned.
 
 @note: The Bonjour service record name will be used to check for the service type string inside of it.
 */
- (NSArray <APBonjourServiceInstance *> *)allBonjourServiceInstancesOfServiceType:(NSString *)serviceType;

/**
 Method to process mDNS packets seen over UDP.
 */
- (void)processmDNSPacket:(APmDNSPacket *)packet;

/**
 Method to clear all the processed mDNS packet information.
 */
- (void)reset;

@end
