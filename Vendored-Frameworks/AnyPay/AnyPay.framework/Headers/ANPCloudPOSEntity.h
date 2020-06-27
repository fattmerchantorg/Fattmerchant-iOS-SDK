//
//  ANPCloudPOSEntity.h
//  AnyPay
//
//  Created by Ankit Gupta on 13/09/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "AnyPayModel.h"

@class ANPConfiguration;
@interface ANPCloudPOSEntity : AnyPayModel

@property (nonatomic, copy) NSString *uuid;
@property (nonatomic, copy) NSString *id;
@property (nonatomic, copy) NSString *parentID;
@property (nonatomic, copy) NSString *portfolio;
@property (nonatomic, strong) ANPConfiguration *configuration;
@property (nonatomic, copy) NSString *dateCreated;
@property (nonatomic, copy) NSString *dateModified;

@end
