// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_IN_PROCESS_IMPORTER_BRIDGE_H_
#define CHROME_BROWSER_IMPORTER_IN_PROCESS_IMPORTER_BRIDGE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "build/build_config.h"
#include "chrome/browser/importer/profile_writer.h"
#include "chrome/common/importer/importer_bridge.h"

class GURL;
struct ImportedBookmarkEntry;
struct ImportedFaviconUsage;
class ExternalProcessImporterHost;

namespace importer {
#if defined(OS_WIN)
struct ImporterIE7PasswordInfo;
#endif
struct ImporterURlRow;
struct URLKeywordInfo;
}

class InProcessImporterBridge : public ImporterBridge {
 public:
  InProcessImporterBridge(ProfileWriter* writer,
                          base::WeakPtr<ExternalProcessImporterHost> host);

  // Begin ImporterBridge implementation:
  void AddBookmarks(const std::vector<ImportedBookmarkEntry>& bookmarks,
                    const base::string16& first_folder_name) override;

  void AddHomePage(const GURL& home_page) override;

#if defined(OS_WIN)
  virtual void AddIE7PasswordInfo(
      const importer::ImporterIE7PasswordInfo& password_info) override;
#endif

  void SetFavicons(const std::vector<ImportedFaviconUsage>& favicons) override;

  void SetHistoryItems(const std::vector<ImporterURLRow>& rows,
                       importer::VisitSource visit_source) override;

  void SetKeywords(const std::vector<importer::URLKeywordInfo>& url_keywords,
                   bool unique_on_host_and_path) override;

  void SetFirefoxSearchEnginesXMLData(
      const std::vector<std::string>& search_engine_data) override;

  void SetPasswordForm(const autofill::PasswordForm& form) override;

  void SetAutofillFormData(
      const std::vector<ImporterAutofillFormDataEntry>& entries) override;

  void NotifyStarted() override;
  void NotifyItemStarted(importer::ImportItem item) override;
  void NotifyItemEnded(importer::ImportItem item) override;
  void NotifyEnded() override;

  base::string16 GetLocalizedString(int message_id) override;
  // End ImporterBridge implementation.

 private:
  ~InProcessImporterBridge() override;

  ProfileWriter* const writer_;  // weak
  const base::WeakPtr<ExternalProcessImporterHost> host_;

  DISALLOW_COPY_AND_ASSIGN(InProcessImporterBridge);
};

#endif  // CHROME_BROWSER_IMPORTER_IN_PROCESS_IMPORTER_BRIDGE_H_
