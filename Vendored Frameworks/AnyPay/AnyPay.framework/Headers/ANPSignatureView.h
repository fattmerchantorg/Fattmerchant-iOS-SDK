//
//  BFSSignatureView.h
//  BFS Capital
//
//  Created by Ankit Gupta on 3/2/16.
//  Copyright © 2018 NetSecure. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ANPSignature.h"

IB_DESIGNABLE
@interface ANPSignatureView : UIView

@property (nonatomic, strong, readonly) ANPSignature *signature;
@property (nonatomic, strong) IBInspectable UIColor *strokeColor;
@property (nonatomic, strong) IBInspectable UIColor *borderColor;
@property (nonatomic) BOOL hasSignature;

- (void)erase;
- (void)drawImage:(UIImage *)image;
- (UIImage *)signatureAsImage;

- (void)drawSignature:(ANPSignature *)signature;

@end
