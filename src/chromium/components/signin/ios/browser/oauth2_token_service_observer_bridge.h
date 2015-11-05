// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_SIGNIN_IOS_BROWSER_OAUTH2_TOKEN_SERVICE_OBSERVER_BRIDGE_H_
#define COMPONENTS_SIGNIN_IOS_BROWSER_OAUTH2_TOKEN_SERVICE_OBSERVER_BRIDGE_H_

#import <Foundation/Foundation.h>

#include "google_apis/gaia/oauth2_token_service.h"

@protocol OAuth2TokenServiceObserverBridgeDelegate <NSObject>

@optional

// Informs the delegate that a refresh token is avaible for |account_id|.
- (void)onRefreshTokenAvailable:(const std::string&)account_id;

// Informs the delegate that the refresh token was revoked for |account_id|.
- (void)onRefreshTokenRevoked:(const std::string&)account_id;

// Informs the delegate that the token service has finished loading the tokens.
- (void)onRefreshTokensLoaded;

// Informs the delegate that a batch of refresh token changes will start.
- (void)onStartBatchChanges;

// Informs the delegate that a batch of refresh token changes has ended.
- (void)onEndBatchChanges;

@end

// Bridge class that listens for |OAuth2TokenService| notifications and passes
// them to its Objective-C delegate.
class OAuth2TokenServiceObserverBridge : public OAuth2TokenService::Observer {
 public:
  OAuth2TokenServiceObserverBridge(
      OAuth2TokenService* token_service,
      id<OAuth2TokenServiceObserverBridgeDelegate> delegate);
  virtual ~OAuth2TokenServiceObserverBridge();

  // OAuth2TokenService::Observer
  virtual void OnRefreshTokenAvailable(const std::string& account_id) override;
  virtual void OnRefreshTokenRevoked(const std::string& account_id) override;
  virtual void OnRefreshTokensLoaded() override;
  virtual void OnStartBatchChanges() override;
  virtual void OnEndBatchChanges() override;

 private:
  OAuth2TokenService* token_service_;  // weak
  id<OAuth2TokenServiceObserverBridgeDelegate> delegate_;

  DISALLOW_COPY_AND_ASSIGN(OAuth2TokenServiceObserverBridge);
};

#endif  // COMPONENTS_SIGNIN_IOS_BROWSER_OAUTH2_TOKEN_SERVICE_OBSERVER_BRIDGE_H_
