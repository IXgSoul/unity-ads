//
//  UnityAdsProperties.h
//  UnityAds
//
//  Created by bluesun on 11/2/12.
//  Copyright (c) 2012 Unity Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "../UnityAdsCampaign/UnityAdsRewardItem.h"

@interface UnityAdsProperties : NSObject
  @property (nonatomic, strong) NSString *webViewBaseUrl;
  @property (nonatomic, strong) NSString *analyticsBaseUrl;
  @property (nonatomic, strong) NSString *campaignDataUrl;
  @property (nonatomic, strong) NSString *adsBaseUrl;
  @property (nonatomic, strong) NSString *campaignQueryString;
  @property (nonatomic, strong) NSString *adsGameId;
  @property (nonatomic, strong) NSString *gamerId;
  @property (nonatomic, strong) NSString *gamerSID;
  @property (nonatomic) BOOL testModeEnabled;
  @property (nonatomic, assign) UIViewController *currentViewController;
  @property (nonatomic, assign) int maxNumberOfAnalyticsRetries;
  @property (nonatomic, strong) NSString *expectedSdkVersion;

+ (UnityAdsProperties *)sharedInstance;
- (void)refreshCampaignQueryString;
- (NSString *)adsVersion;

@end
