// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_OZONE_PLATFORM_DRI_DRI_WINDOW_MANAGER_H_
#define UI_OZONE_PLATFORM_DRI_DRI_WINDOW_MANAGER_H_

#include <map>

#include "base/memory/scoped_ptr.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Point;
}

namespace ui {

class DriCursor;
class DriGpuPlatformSupportHost;
class DriWindow;

// Responsible for keeping the mapping between the allocated widgets and
// windows.
class DriWindowManager {
 public:
  explicit DriWindowManager(DriGpuPlatformSupportHost* sender);
  ~DriWindowManager();

  gfx::AcceleratedWidget NextAcceleratedWidget();

  // Adds a window for |widget|. Note: |widget| should not be associated when
  // calling this function.
  void AddWindow(gfx::AcceleratedWidget widget, DriWindow* window);

  // Removes the window association for |widget|. Note: |widget| must be
  // associated with a window when calling this function.
  void RemoveWindow(gfx::AcceleratedWidget widget);

  // Returns the window associated with |widget|. Note: This function should
  // only be called if a valid window has been associated.
  DriWindow* GetWindow(gfx::AcceleratedWidget widget);

  // Returns the window containing the specified screen location, or NULL.
  DriWindow* GetWindowAt(const gfx::Point& location);

  DriCursor* cursor() const { return cursor_.get(); }

  // Tries to set a given widget as the recipient for events. It will
  // fail if there is already another widget as recipient.
  void GrabEvents(gfx::AcceleratedWidget widget);

  // Unsets a given widget as the recipient for events.
  void UngrabEvents(gfx::AcceleratedWidget widget);

  // Gets the current widget recipient of mouse events.
  gfx::AcceleratedWidget event_grabber() const { return event_grabber_; }

 private:
  // Reset the cursor location based on the list of active windows.
  void ResetCursorLocation();

  typedef std::map<gfx::AcceleratedWidget, DriWindow*> WidgetToWindowMap;

  gfx::AcceleratedWidget last_allocated_widget_;
  WidgetToWindowMap window_map_;

  scoped_ptr<DriCursor> cursor_;

  gfx::AcceleratedWidget event_grabber_;

  DISALLOW_COPY_AND_ASSIGN(DriWindowManager);
};

}  // namespace ui

#endif  // UI_OZONE_PLATFORM_DRI_DRI_WINDOW_MANAGER_H_