// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_MOCK_BROWSING_DATA_APPCACHE_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_MOCK_BROWSING_DATA_APPCACHE_HELPER_H_

#include "base/callback_forward.h"
#include "chrome/browser/browsing_data/browsing_data_appcache_helper.h"

class MockBrowsingDataAppCacheHelper
    : public BrowsingDataAppCacheHelper {
 public:
  explicit MockBrowsingDataAppCacheHelper(
      content::BrowserContext* browser_context);

  void StartFetching(const base::Closure& completion_callback) override;
  void DeleteAppCacheGroup(const GURL& manifest_url) override;

 private:
  ~MockBrowsingDataAppCacheHelper() override;

  DISALLOW_COPY_AND_ASSIGN(MockBrowsingDataAppCacheHelper);
};

#endif  // CHROME_BROWSER_BROWSING_DATA_MOCK_BROWSING_DATA_APPCACHE_HELPER_H_
