//
//  TCChatViewController.h
//  TeamChat
//
//  Created by Vidyalaxmi V Shenoy on 13/07/15.
//  Copyright (c) 2015 Webaroo. All rights reserved.
//

#import <UIKit/UIKit.h>

@class CurrentUserInfo;
@class TeamchatGroup;

typedef NS_ENUM(NSInteger, TCSessionState){
    TCSessionStateInactive = 0, // TeamChat session is inactive
    TCSessionStateLoggedIn, // User has logged in to TeamChat
    TCSessionStateActive,   // Session is active after initial set up
    TCSessionStateInvalidatedOnUserDelete // Session is invalidated when user is deleted from organisation
};

typedef NS_ENUM(NSInteger, TCPasscodeViewType){
    TCPasscodeViewTypeEnablePasscode, // To show view to enable passcode by setting a new passcode.
    TCPasscodeViewTypeDisablePasscode, // To show view to disable passcode after entering the current passcode.
    TCPasscodeViewTypeUnlock, // To show view to enter the passcode and unlock Teamchat.
    TCPasscodeViewTypeResetPasscode // To show view to reset the passcode.
};

//Nofification is posted when active session is invalidated.
extern NSString *const TCActiveSessionIsInvalidatedNotification;

//Nofification is posted when a ChatRoom is updated.
extern NSString *const TCTeamchatGroupUpdatedNotification;

// Notification is posted when Teamchat is upgrading its DB
extern NSString *const TCTeamchatWillBeginDatabaseUpgradeNotification;
extern NSString *const TCTeamchatCompletedUpgradingDatabaseNotification;

extern NSString *const TCChatRoomGroupFilteringCriterionChangedNotification;

typedef NS_OPTIONS(NSUInteger, TCShowContactOptions) {
    TCShowContactFromLocalAddressBook  = 1 <<  0, //List the address book contacts from phone address book
    TCShowContactFromServerAddressBook   = 1 <<  1, //List the contacts from server in users organisation
    TCShowContactFromCustomAddressBook   = 1 <<  2  //List the contacts from app contact provider as per TCCustomContactProvider
};

typedef NS_ENUM(NSInteger, TCGroupType) {
    TCGroupTypeMultiMemberGroup = 0, //Indicated its a group with many members
    TCGroupTypeOneToOne //Indicates its a One-To-One type
};

@interface TeamchatGroup : NSObject

@property (nonatomic, copy) NSString * name;
@property (nonatomic, copy) NSString * groupID;
@property (nonatomic, assign,readonly) TCGroupType groupType;

/*! 
 @param groupID A valid groupID
 @return A TeamchatGroup object if groupID is valid */
+ (TeamchatGroup *)groupWithID:(NSString *)groupID;

/*! 
 @return YES if the group is one-to-one
 */
- (BOOL)isGroupOneToOne;

@end

@interface TeamchatInviteContact : NSObject

@property (nonatomic, copy) NSString *name;
@property (nonatomic, copy) NSString *emailID;

@end

@interface TCTeamchatContact : NSObject

@property (nonatomic, copy) NSString *profileName;
@property (nonatomic, copy) NSString *email;
@property (nonatomic, copy) NSString *phone;

@end

@protocol TCCustomContactProvider <NSObject>

- (NSArray*)customAddressBookContacts; //Array of TCTeamchatContact objects

@end

@interface TeamchatSplashScreenWithLogo : NSObject

+ (UIView *)splashScreenWithLogoView;

+ (UIImage *)logoImage;

+ (UIImage *)coBrandingImage;

@end


typedef NS_ENUM(NSInteger, TCErrorCodes)
{
    TCParametersNotInitializedError = -1000,
    TCInvalidGroupIDError           = -1001,
    TCGroupsFetchFailedError        = -1002,
    TCInitMethodNotCalledError      = -1003,
    TCLoginFailureError             = -1004,
    TCPasscodeAlreadyEnabledError   = -1005,
    TCPasscodeNotEnabledError       = -1006
};

typedef void (^TeamchatCompletionHandler)(BOOL success, NSError* error, NSString *errMsg);
typedef void (^TeamchatContactFetchCompletionHandler)(BOOL success, NSError* error, NSArray *fetchedContacts);
typedef void (^TeamchatInitializationWithAppIDCompletionHandler)(BOOL success, NSError *error);
typedef void (^TeamchatPublicGroupsCompletionHandler)(TeamchatGroup *selectedGroup);
typedef void (^TeamchatUserLoggedOutCompletionHandler)();
typedef void (^TeamchatPasscodeControllerCompletionHandler)(BOOL didProcessPasscode, NSError *error, UIViewController *passcodeViewController);

@interface Teamchat : NSObject

+ (void)setHostURL:(NSString *)hostURL;

+ (NSString *)currentHostURL;

//This method should be called in - (BOOL)application: didFinishLaunchingWithOptions:
+ (void)initializeWithAppID:(NSString *)appID withCompletionHanlder:(TeamchatInitializationWithAppIDCompletionHandler)teamchatInitializationWithAppIDCompletionHandler;

+ (NSUInteger)sessionState;

/*!
 Use this method to reset active session and create new object using different parameters.
 Session state will be TCSessionStateInactive after reset.
 */
+ (void)resetTeamchatSession;

/*!
 Use this method to get the count of unread messages that were received.
 */
+ (NSInteger)numberOfUnreadMessages;

/*!
 'YES' will show the Teamchat logs everytime.
 */
+ (void)setLoggingEnabled:(BOOL)loggingEnabled;

+ (void)setAuthenticationCode:(NSString *)authenticationCode;

+ (void)setUserEmailID:(NSString *)emailID;

+ (void)setUserID:(NSString *)userID;

+ (void)setNavigationTitle:(NSString*)newTitle;

+ (void)setNavigationTitleColor:(UIColor*)newTitleColor;

+ (void)setMediaIcon:(UIImage*)mediaIcon;

+ (void)setChatletIcon:(UIImage*)chatletIcon;

+ (void)setTableViewSeparatorColor:(UIColor*)tableViewSeparatorColor;

+ (void)setPNEnvironmentID:(NSString*)pnEnvironmentID;

//This method should be called in - (void)application: didRegisterForRemoteNotificationsWithDeviceToken:
+ (void)setRemoteNotificationsDeviceToken:(NSString*)deviceToken;

+ (void)setShowContactOptions:(NSUInteger)showContactOptions;

+ (void)setCustomAddressBookContactProvider:(id<TCCustomContactProvider>)customAddressBookContactProvider;

+ (id<TCCustomContactProvider>)customAddressBookContactProvider;

+ (NSUInteger)showContactOptions;

+ (void)getTeamchatServerContactsWithCompletion:(TeamchatContactFetchCompletionHandler)completionHandler;

+ (NSString*)navigationTitle;

+ (UIColor*)navigationTitleColor;

+ (UIImage*)mediaIcon;

+ (UIImage*)chatletIcon;

+ (UIColor*)tableViewSeparatorColor;

/*!
 'YES' will hide 'Cancel' button. Hide 'Cancel' button when Teamchat view is added to TabBarController.
 */
+ (void)shouldHideCancelButton:(BOOL)shouldHide;

+ (BOOL)isCancelButtonHidden;

/*!
 Setting 'YES' to this method disables all AppLock/Passcode related features.
 */
+ (void)setAppLockFeatureEnabled:(BOOL)shouldEnable;

/*!
 Setting 'YES' to this method disables all Sandboxing related features.
 */
+ (void)setSandboxingFeatureEnabled:(BOOL)shouldEnable;

/*!
 @return YES if passcode is enabled.
 */
+ (BOOL)isPasscodeEnabled;

/*!
 @return YES if passcode view can be shown for the specified type.
 */
+ (BOOL)canShowPasscodeViewForType:(TCPasscodeViewType)type error:(NSError **)error;

/*!
 Use this method to get passcode view controller.
 @param passcodeViewType The type of passcode view you want to present. It can be one of TCPasscodeViewTypeUnlock or TCPasscodeViewTypeResetPasscode.
 @param completionHandler This block is called after passcode is unlocked or reset and passes the passcode view controller. Dismiss the view controller in this block.
 */
+ (UIViewController *)passcodeViewControllerOfType:(TCPasscodeViewType)passcodeViewType withCompletion:(TeamchatPasscodeControllerCompletionHandler)completionHandler;

/*!
 @return YES if SDK is already presenting lock screen. 
 */
+ (BOOL)isTeamchatAlreadyPresentingLockScreen;

/*!
 @return YES if SDK is already presenting lock screen.
 @discussion setSandboxingFeatureEnabled should be set to YES before using this method.
 */
+ (BOOL)isSandBoxingEnabled;

/*!
 Enable or disable sandboxing feature.
 @discussion setSandboxingFeatureEnabled should be set to YES before using this method.
 */
+ (void)setSandBoxingEnabled:(BOOL)shouldEnable;

/*!
 Use this method to inivte contacts to Teamchat via email. 
 @param inviteEmailsArray An array of 'TeamchatInviteContact' objects.
 @param inviteCompletionHandler This block has no return value and takes a Boolean argument that indicates whether or not the invite was successful before the completion handler was called, a NSError argument to indicate error if any and a NSString argument with error message if any */
+ (void)inviteContactsToTeamchat:(NSArray *)inviteContacts compeltionHandler:(TeamchatCompletionHandler)inviteCompletionHandler;

/*!
 Pass an array of bar button items that should be set as left bar button items of navigation item.
 */
+ (void)setNavigationItemLeftBarButtonItems:(NSArray *)leftBarButtonItems;

+ (NSArray *)navigationItemLeftBarButtonItems;

//Option 1:By setting Authtoken, Email and UserID
//Should be called before calling any Group creation and viewcontroller creation APIs
+ (void)initWithCompletionHandler:(TeamchatCompletionHandler)intCompletionHandler;

//Option 2 - OTP: Without setting any Authtoken, Email and UserID
//If you set only emailID then login page will have prefilled email field without editing support. Once login is completed then
//loginCompletionHandler is called with success as YES if Authtoken, Email and UserID are set properly.
//If failed success will be NO and error will be passed.
//On success initWithCompletionHandler should be called.
+ (UIViewController*)teamchatLoginViewController:(TeamchatCompletionHandler)loginCompletionHandler;

+ (void)listOfTeamchatGroupsExcludingGroups:(NSArray *)groupsArray andAnyOneToOneUsersWithEmail:(NSArray *)emailsArray;

+ (void)listOfTeamchatGroupsIncludingGroups:(NSArray *)groupsArray andAnyOneToOneUsersWithEmail:(NSArray *)emailsArray;

+ (NSArray *)teamchatGroups:(NSError **)error;

+ (TeamchatGroup*)teamchatGroupWithID:(NSString *)groupID error:(NSError **)error;

+ (UIViewController*)teamchatGroupsViewController;

+ (UIViewController *)createTeamchatWindowViewControllerWithGroup:(TeamchatGroup*)teamChatGroup;

+ (UIViewController *)userProfileViewController;

+ (UIViewController *)settingsViewController;

+ (UIViewController *)publicGroupsViewController:(TeamchatPublicGroupsCompletionHandler)publicGroupsCompletionHandler;

+ (UIViewController *)userLoggedOutViewController:(TeamchatUserLoggedOutCompletionHandler)userLoggedOutCompletionHandler;

+ (UIViewController *)teamchatConfigurationViewController;

+ (void)openTeamchatGroup:(TeamchatGroup *)teamchatGroup fromChatRoomsList:(UIViewController *)chatRoomsListViewController;

@end
