//
//  ANPSignature.h
//  AnyPay
//
//  Created by Ankit Gupta on 14/05/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#if TARGET_OS_IPHONE
@import UIKit;
#else
@import AppKit;
#endif

#import "ANPDrawPath.h"

@interface ANPSignature : AnyPayModel

@property (nonatomic, strong) NSMutableArray<ANPDrawPath *> <ANPDrawPath> *signaturePointsArray;
@property (nonatomic, copy) NSString *details;
@property (nonatomic, strong) AnyPayModel *info;

#if TARGET_OS_IPHONE
@property (nonatomic, strong) UIBezierPath *signaturePath;
#else
@property (nonatomic, strong) NSBezierPath *signaturePath;
#endif

@end
