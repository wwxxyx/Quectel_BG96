//
//  SelfCertKitCommon.h
//  SelfCertKit
//
//  Created by Vinod Madigeri on 11/18/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, SKError) {
    SKErrorClientIssue                      = -10400,    /// Client error (HTTP: 4XX)
    SKErrorClientNotAuthenticated           = -10401,    /// User is allowed to but couldn’t login or isn't logged in (HTTP: 401)
    SKErrorClientForbidden                  = -10402,    /// User isn't allowed to perform the request (HTTP: 403)
    SKErrorClientDeprecatedAPIVersion       = -10403,    /// SelfCertKit version is older than permitted (HTTP: 410)
    SKErrorServerNoConnection               = -10500,    /// SelfCert server is not reachable
    SKErrorServerIssue                      = -10501,    /// Server returned an error (HTTP: 5XX)
    SKErrorServerMaintainanceMode           = -10502,    /// SelfCertKit version is older than permitted (HTTP: 503)
    SKErrorInternalLogOutInProgress         = -10600,    /// SelfCert is logging out, do not send new requests
    SKErrorInternalNoInternetConnection     = -10601,    /// No internet connection
    SKErrorInternalInvalidAttachment        = -10602,    /// Invalid attachment
    SKErrorInternalSSLConnectionFailed      = -10603,    /// Cert pinning failed
    SKErrorInternalNoResponseDataFromServer = -10604,    /// Server did not send any response data
    SKErrorUserCancelled                    = -10605,    /// The action was canceled on the user's request
};

#import "SKSession.h"
#import "SKAuthenticationViewController.h"
#import "SKProductPlan.h"
#import "SKSubmission.h"
#import "SKTypes.h"
