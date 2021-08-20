//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef NS_ENUM(NSInteger, IntuneMAMSaveLocation)
{
    IntuneMAMSaveLocationOther = 0,
    IntuneMAMSaveLocationOneDriveForBusiness = 1<<0,
    IntuneMAMSaveLocationSharePoint = 1<<1,
    IntuneMAMSaveLocationBox = 1<<2,
    IntuneMAMSaveLocationDropbox = 1<<3,
    IntuneMAMSaveLocationGoogleDrive = 1<<4,
    IntuneMAMSaveLocationLocalDrive = 1<<5,
    IntuneMAMSaveLocationCameraRoll = 1<<6,
    IntuneMAMSaveLocationAccountDocument = 1<<7, // When the location is not listed in this enum but is accessed with a managed account, use this value

};

typedef NS_ENUM(NSInteger, IntuneMAMOpenLocation)
{
    IntuneMAMOpenLocationOther = 0,
    IntuneMAMOpenLocationOneDriveForBusiness = 1<<0,
    IntuneMAMOpenLocationSharePoint = 1<<1,
    IntuneMAMOpenLocationCamera = 1<<2,
    IntuneMAMOpenLocationLocalStorage = 1<<3,
    IntuneMAMOpenLocationAccountDocument = 1<<4, // When opening a document that has a managed account identity or the location is not listed in this enum but is accessed with a managed account, use this value
};

// IntuneMAMNotificationPolicyAllow - All notifications for the managed user should be allowed
// IntuneMAMNotificationPolicyBlockOrgData - Only static notifications without specific details
// should be shown for the managed user e.g. "You've got mail" or "You have a meeting".
// IntuneMAMNotificationPolicyBlock - All notifications for the managed user should be suppressed.
typedef NS_ENUM(NSInteger, IntuneMAMNotificationPolicy)
{
    IntuneMAMNotificationPolicyAllow = 0,
    IntuneMAMNotificationPolicyBlockOrgData = 1,
    IntuneMAMNotificationPolicyBlock = 2,
};

__attribute__((visibility("default")))
@protocol IntuneMAMPolicy <NSObject>

@required

// TRUE if the management policy requires the user to enter a PIN.
// The SDK will automatically handle displaying the PIN UI. If the application
// has its own PIN support and this method returns true, the application should
// not show its own PIN UI.
@property (readonly) BOOL isPINRequired;


// TRUE if the management policy allows applications to save managed files to the account accountName
// in the given location. Applications should check this policy and if FALSE should disable any UI
// which allows users to save managed documents to this account in this location.
// If the accountName for the location is unknown, set this argument to nil. 
- (BOOL) isSaveToAllowedForLocation: (IntuneMAMSaveLocation) location withAccountName: (NSString*_Nullable) accountName;

// TRUE if the management policy allows applications to open files from the account accountName into
// the managed app. Applications should check this policy and if FALSE should disabled any UI which
// allows users to open documents from this account and location into the managed app.
// If the accountName for the location is unknown, set this argument to nil.
- (BOOL) isOpenFromAllowedForLocation: (IntuneMAMOpenLocation) location withAccountName: (NSString* _Nullable) accountName;

// Returns a dictionary mapping of all the IntuneMAMSaveLocations and whether each is allowed to have
// data saved to it. Both the IntuneMAMSaveLocation keys and the BOOL allowed value are wrapped in
// NSNumbers. Calling this is the same as calling isSaveToAllowedForLocation:withAccountName: for each
// IntuneMAMSaveLocation in the enum.
- (NSDictionary<NSNumber *, NSNumber *>* _Nonnull) getSaveToLocationsForAccount:(NSString* _Nullable)toAccount;

// Returns a dictionary mapping of all the IntuneMAMOpenFromLocations and whether each is allowed to
// have data opened from it. Both the IntuneMAMOpenLocation keys and the BOOL allowed values are wrapped
// in NSNumbers. Calling this is the same as calling isOpenFromAllowedForLocation:withAccountName: for
// each IntuneMAMOpenLocation in the enum.
- (NSDictionary<NSNumber *, NSNumber *>* _Nonnull) getOpenFromLocationsForAccount:(NSString* _Nullable)fromAccount;

// FALSE if the management policy blocks application opening/querying the specified URL.
// Returns TRUE otherwise, regardless of whether the scheme is listed in the application's
// LSApplicationQueriesSchemes.  Applications can check this policy to customize their UI.
// Policy enforcement will be entirely handled by the SDK.
- (BOOL) isURLAllowed: (NSURL*_Nonnull) url;

// FALSE if the management policy blocks the application from opening the specified URL with the
// UIApplicationOpenURLOptionUniversalLinksOnly option set to @YES. Returns TRUE otherwise.
// Applications can check this policy to customize their UI. Policy enforcement will be entirely handled
// by the SDK.
- (BOOL) isUniversalLinkAllowed: (NSURL*_Nonnull) url;

// FALSE if the management policy blocks the specified document picker mode.  Returns TRUE
// otherwise, regardless of whether there are managed document picker extensions in the
// UIDocumentPickerViewController that can accept the managed file. Applications can check
// this policy to customize their UI. Policy enforcement will be entirely handled by the SDK.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
- (BOOL) isDocumentPickerAllowed: (UIDocumentPickerMode) mode;
#pragma clang diagnostic pop

// TRUE if the management policy requires the Intune Managed Browser to handle HTTP/HTTPS
// requests. Applications can check this policy to customize their UI. Policy enforcement
// will be entirely handled by the SDK.
@property (readonly) BOOL isManagedBrowserRequired;

// TRUE if the management policy allows applications to sync corporate contact data to
// the native app.  Multi-Identitity applications should check this policy and if FALSE should not sync
// corporate contacts to the native app.  For single identity applications, policy enforcement will be
// entirely handled by the SDK.
@property (readonly) BOOL isContactSyncAllowed;

// TRUE if the management policy allows applications to index coporate data for
// Core Spotlight.  Multi-Identity applications should check this policy and if FALSE should set
// eligibleForPublicIndexing and eligibleForSearch to FALSE for corporate NSUserActivity objects
// and disallow indexing of corporate data through Core Spotlight.  For single identity applications,
// policy enforcement will be entirely handled by the SDK.
@property (readonly) BOOL isSpotlightIndexingAllowed;

// TRUE if the management policy allows applications to use Siri intents.
// Multi-Identitity applications should check this policy and if FALSE should block Siri intents.
// For single identity applications, policy enforcement will be entirely handled by the SDK.
@property (readonly) BOOL areSiriIntentsAllowed;

// FALSE if the management policy blocks sharing via the UIActivityViewController/
// UIDocumentInteractionViewController.  Returns TRUE otherwise, regardless of whether there are
// managed applications or share extensions available to share the data with.  Applications can check
// this policy to customize their UI. Policy enforcement will be entirely handled by the SDK.
@property (readonly) BOOL isAppSharingAllowed;

// TRUE if management policy requires File Provider extensions to encrypt files. Applications
// should check for this policy in their File provider extension if they are supporting iOS11 or higher
// File Provider APIs. Policy enforcement will not be handled by the SDK. Application will explicitly
// need to invoke encryptFile:forIdentity API in IntuneMAMFileProtectionManager for each file exposed by
// the File Provider.
@property (readonly) BOOL shouldFileProviderEncryptFiles;

// Returns an IntuneMAMNotificationPolicy value indicating the current notification policy
// IntuneMAMNotificationPolicyAllow - All notifications for the managed user should be allowed
// IntuneMAMNotificationPolicyBlockOrgData - Only static notifications without specific details
// should be shown for the managed user e.g. "You've got mail" or "You have a meeting".
// IntuneMAMNotificationPolicyBlock - All notifications for the managed user should be suppressed.
@property (readonly) IntuneMAMNotificationPolicy notificationPolicy;

// TRUE if management policy requires software encryption of files on disk. FALSE otherwise.
// If TRUE the app should encrypt all managed files on disk.
@property (readonly) BOOL isFileEncryptionRequired;

@end
