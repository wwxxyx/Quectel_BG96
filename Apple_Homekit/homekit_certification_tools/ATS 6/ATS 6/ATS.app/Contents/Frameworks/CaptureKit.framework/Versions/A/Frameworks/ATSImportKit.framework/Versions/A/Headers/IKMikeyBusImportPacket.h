//
//  IKMikeyBusImportPacket.h
//  
//
//  Created by Wesley McCloy on 12/2/15.
//
//

#import <ATSImportKit/ATSImportKit.h>

@interface IKMikeyBusImportPacket : IKImportPacket

/*!
 This NSData object represents the raw data at the lowest level of the MikeyBus protocol
 */
@property (nonatomic, retain) NSData *mikeyBusRawData;

@end
