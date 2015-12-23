//
//  TeamchatGroupController.h
//  TeamChat
//
//  Created by Vidyalaxmi V Shenoy on 13/10/15.
//  Copyright (c) 2015 Webaroo. All rights reserved.
//

#import <Foundation/Foundation.h>

@class TeamchatGroup;

typedef void (^TeamchatGroupCreationCompletionHandler)(BOOL success, NSError* error, NSString *msg, TeamchatGroup *createdGroup);
typedef void (^TCPendingMembersFetchCompletionHandler)(BOOL success, NSError *error, NSString *msg, NSArray *pendingmembersArray);
typedef void (^TeamchatGroupOperationCompletionHandler)(BOOL success, NSError* error, NSString *msg);

typedef NS_ENUM(NSInteger, TCGroupControllerErrorCodes)
{
    TCGroupNameNotSetError          = -1000,
    TCGroupNameExceedsMaxCharacters = -1001,
    TCMembersNotSetError            = -1002,
    TCInvalidMembersError           = -1003,
    TCInvalidGroupError             = -1004,
    TCUserNotGroupAdminError        = -1005,
    TCInvalidAccessError            = -1006,
    TCInvalidGroupMemberError       = -1007,
    TCInvalidMessageError           = -1008,
    TCTeamchatNotInitializedError   = -1009,
    TCNoNetworkError                = -1010
};

#pragma mark -- TeamchatGroupMember

@interface TeamchatGroupMember : NSObject

@property (nonatomic, copy) NSString *memberName;
@property (nonatomic, copy) NSString *memberID;
@property (nonatomic, copy) NSString *teamchatGroupID;

+ (TeamchatGroupMember *)memberWithID:(NSString *)memberID groupID:(NSString *)groupID;

@end

#pragma mark -- TeamchatGroupPendingMember

@interface TeamchatGroupPendingMember : NSObject

@property (nonatomic, copy) NSString *pendingMemberName;
@property (nonatomic, copy) NSString *pendingMemberID;
@property (nonatomic, copy) NSString *pendingMemberType;

@end

#pragma mark -- TeamchatGroupCreator

@interface TeamchatGroupCreator : NSObject

// Mandatory methods for group creation
- (void)setGroupName:(NSString *)groupName; // Max length of group name is 20 characters.
- (void)setGroupMembers:(NSArray *)members; // Array of TCTeamchatContact objects.

// Optional methods.
- (void)setAdminOnly:(BOOL)adminOnly;
- (void)setShouldHideMemberProfiles:(BOOL)shouldHideMemberProfiles;

// Call this method after setting group name and members.
- (void)createGroupWithCompletionHandler:(TeamchatGroupCreationCompletionHandler)completionHandler;

@end


#pragma mark -- TeamchatGroupController

@interface TeamchatGroupController : NSObject

/*!
 @abstract Use this method add members to an existing group. You can add members only if you are the admin of the group.
 @param membersToBeAdded An array of 'TCTeamchatContact' objects.
 @param teamchatGroup The 'TeamchatGroup' to which members are to be added.
 @param completionHandler This block has no return value and takes a Boolean argument that indicates whether or not the members' addition was successful before the completion handler was called, a NSError argument to indicate error if any and a NSString argument with error message if any */
+ (void)addMembers:(NSArray *)membersToBeAdded toGroup:(TeamchatGroup *)teamchatGroup
        completion:(TeamchatGroupOperationCompletionHandler)completionHandler;

/*!
 @abstract If you are the admin of the group, the group will be terminated permanently, all members will be removed and all data will be deleted. If you are not the admin of the group, you will just be removed from the group.
 @param teamchatGroup The 'TeamchatGroup' from which you want to exit.
 @param completionHandler This block has no return value and takes a Boolean argument that indicates whether or not the exit operation was successful before the completion handler was called, a NSError argument to indicate error if any and a NSString argument with error message if any */
+ (void)exitGroup:(TeamchatGroup *)teamchatGroup completion:(TeamchatGroupOperationCompletionHandler)completionHandler;

/*!
 @abstract Use this method to get the list of active members of the group that you are a member of.
 @param teamchatGroup The 'TeamchatGroup' from which you want to fetch active members.
 @param error An error object passed by reference.
 @return An array of 'TeamchatGroupMember' objects will be returned if no error.*/
+ (NSArray *)getActiveMembersOfGroup:(TeamchatGroup *)teamchatGroup error:(NSError **)error;

/*!
 @abstract Use this method to get the list of pending members of the group that you are a member of.
 @param teamchatGroup The 'TeamchatGroup' from which you want to fetch pending members.
 @param completionHandler This block has no return value and takes a Boolean argument that indicates whether or not the fetch was successful before the completion handler was called, a NSError argument to indicate error if any and a NSString argument with error message if any, a NSArray argument will have the fetched members if fetch was successful*/
+ (void)getPendingMembersOfGroup:(TeamchatGroup *)teamchatGroup
                           completion:(TCPendingMembersFetchCompletionHandler)completionHandler;

/*!
 @abstract Use this method to remove an active member from a group. You can remove a member from a group only if you are admin of the group.
 @param groupMember The 'TeamchatGroupMember' to be removed.
 @param teamchatGroup The 'TeamchatGroup' from which you want to remove member.
 @param completionHandler This block has no return value and takes a Boolean argument that indicates whether or not the remove operation was successful before the completion handler was called, a NSError argument to indicate error if any and a NSString argument with error message if any, a NSArray argument will have the fetched members if fetch was successful.*/
+ (void)removeActiveMember:(TeamchatGroupMember *)groupMember fromGroup:(TeamchatGroup *)teamchatGroup
          completion:(TeamchatGroupOperationCompletionHandler)completionHandler;

/*!
 @abstract Use this method to remove a pending member from a group. You can remove a member from a group only if you are admin of the group.
 @param pendingMember The 'TeamchatGroupPendingMember' to be removed.
 @param teamchatGroup The 'TeamchatGroup' from which you want to remove member.
 @param completionHandler This block has no return value and takes a Boolean argument that indicates whether or not the remove operation was successful before the completion handler was called, a NSError argument to indicate error if any and a NSString argument with error message if any.*/
+ (void)removePendingMember:(TeamchatGroupPendingMember *)pendingMember fromGroup:(TeamchatGroup *)teamchatGroup
                 completion:(TeamchatGroupOperationCompletionHandler)completionHandler;

/*!
 @abstract Use this method to change the admin of the group to another active member of the group. You can change the admin of a group only if you are admin of the group and after changing the admin, you will no longer be the admin of the group.
 @param groupMember The 'TeamchatGroupMember' whom you want to change the admin to.
 @param teamchatGroup The 'TeamchatGroup' whose admin you want to change.
 @param completionHandler This block has no return value and takes a Boolean argument that indicates whether or not the change admin operation was successful before the completion handler was called, a NSError argument to indicate error if any and a NSString argument with error message if any.*/
+ (void)changeAdminOfGroup:(TeamchatGroup *)teamchatGroup toGroupMember:(TeamchatGroupMember *)groupMember
                completion:(TeamchatGroupOperationCompletionHandler)completionHandler;

/*!
 @abstract Use this method to get the admin of the group that you are a member of.
 @param teamchatGroup The 'TeamchatGroup' whose admin you want to get.
 @param error An error object passed by reference.
 @return The admin member of the group if there was no error. Otherwise error object will convey the error that was occurred.*/
+ (TeamchatGroupMember *)getAdminOfGroup:(TeamchatGroup *)teamchatGroup error:(NSError **)error;

/*!
 @abstract Use this method to enable or disable admin only option of the group. You can change this option only if you are the admin of the group.
 @param admin 'YES' to enable admin only option and 'NO' to disable admin only option.
 @param teamchatGroup The 'TeamchatGroup' whose admin only option you want to change.
 @param completionHandler This block has no return value and takes a Boolean argument that indicates whether or not the admin only option was changed successfully before the completion handler was called, a NSError argument to indicate error if any and a NSString argument with error message if any.*/
+ (void)setAdminOnlyOption:(BOOL)adminOnly forGroup:(TeamchatGroup *)teamchatGroup completion:(TeamchatGroupOperationCompletionHandler)completionHandler;

/*!
 @abstract Use this method to show or hide member profiles the group. You can change this option only if you are the admin of the group.
 @param admin 'YES' to hide member profiles and 'NO' to show member profiles.
 @param teamchatGroup The 'TeamchatGroup' whose member profiles you want to show or hide.
 @param completionHandler This block has no return value and takes a Boolean argument that indicates whether or not the option was changed successfully before the completion handler was called, a NSError argument to indicate error if any and a NSString argument with error message if any.*/
+ (void)setMemberProfileHiddenOption:(BOOL)hideMemberProfile forGroup:(TeamchatGroup *)teamchatGroup completion:(TeamchatGroupOperationCompletionHandler)completionHandler;

/*!
 @abstract Use this method to change the name of the group. You can change the group name only if you are the admin of the group.
 @param teamchatGroup The 'TeamchatGroup' whose name you want to change.
 @param newName The new name for the group.
 @param completionHandler This block has no return value and takes a Boolean argument that indicates whether or not the operation was successful before the completion handler was called, a NSError argument to indicate error if any and a NSString argument with error message if any.*/
+ (void)changeNameOfGroup:(TeamchatGroup *)teamchatGroup toName:(NSString *)newName completion:(TeamchatGroupOperationCompletionHandler)completionHandler;

/*!
 @abstract Use this method to clear all the messages from a group. You can clear messages of only those groups which you are a member of.
 @param teamchatGroup The 'TeamchatGroup' whose messages you want to clear.
 @param error An error object passed by reference.
 @return True if messages were cleared successfully. Otherwise error object will convey the error that was occurred.*/
+ (BOOL)clearMessagesFromGroup:(TeamchatGroup *)teamchatGroup error:(NSError **)error;

/*!
 @abstract Use this method to post a message to a group. You can post messages to only those groups which you are a member of.
 @param message The text message to be posted.
 @param teamchatGroup The 'TeamchatGroup' to which you want to post a message.
 @param error An error object passed by reference.
 @return True if message can be posted. Otherwise error object will convey the error that was occurred.*/
+ (BOOL)postMesage:(NSString *)message toGroup:(TeamchatGroup *)teamchatGroup error:(NSError **)error;

@end


