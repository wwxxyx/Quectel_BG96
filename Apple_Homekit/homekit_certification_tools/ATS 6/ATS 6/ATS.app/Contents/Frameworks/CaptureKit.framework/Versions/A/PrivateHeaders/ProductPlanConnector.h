//
//  ProductPlanConnector.h
//  ATSMacApp
//
//  Created by Kip Nicol on 8/13/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/ProductPlanType.h>

@interface ProductPlanConnector : ProductPlanType

@property (nonatomic, readonly, assign) ConnectorType type;

+ (id)connectorWithConnectorType:(ConnectorType)type;
- (id)initWithConnectorType:(ConnectorType)type;

@end
