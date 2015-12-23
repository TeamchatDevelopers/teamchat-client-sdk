//
//  TeamchatEventTracker.h
//  TeamChat
//
//  Created by Vidyalaxmi V Shenoy on 14/09/15.
//  Copyright (c) 2015 Webaroo. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol TeamchatEventTrackerDelegate <NSObject>

- (void)eventTracked:(NSString *)eventName withProperties:(NSDictionary *)properties;

@end

@interface TeamchatEventTracker : NSObject

+ (instancetype)sharedEventTrackerInstance;

- (void)setEventTrackerDelegate:(id<TeamchatEventTrackerDelegate>)eventTrackerDelegate;

- (id<TeamchatEventTrackerDelegate>)eventTrackerDelegate;

@end
