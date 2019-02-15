//
//  I2MArrayParameter.h
//  iAP2MessageKit
//
//  Created by Wesley McCloy on 7/3/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <iAP2MessageKit/iAP2MessageKit.h>

/**
 `I2MArrayParameter` is a subclass of `I2MParameter` to be represent an ordered collection of parameters. It is essentially just a wrapper of an NSArray.
  */
@interface I2MArrayParameter : I2MParameter

/**
 This is the array of parameters that this I2MArrayParameter represents
 */
@property (nonatomic, strong) NSArray<I2MParameter *> *subparameters;

@end
