// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/public/common/sandbox_init.h"

#include "base/files/scoped_file.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/sandbox_linux/sandbox_seccomp_bpf_linux.h"
#include "sandbox/linux/bpf_dsl/policy.h"

namespace content {

bool InitializeSandbox(scoped_ptr<sandbox::bpf_dsl::Policy> policy,
                       base::ScopedFD proc_task_fd) {
  return SandboxSeccompBPF::StartSandboxWithExternalPolicy(policy.Pass(),
                                                           proc_task_fd.Pass());
}

scoped_ptr<sandbox::bpf_dsl::Policy> GetBPFSandboxBaselinePolicy() {
  return SandboxSeccompBPF::GetBaselinePolicy().Pass();
}

}  // namespace content