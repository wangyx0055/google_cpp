// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "chrome/browser/ui/cocoa/apps/app_shim_menu_controller_mac.h"

#include "base/mac/scoped_nsautorelease_pool.h"
#include "base/strings/sys_string_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/apps/app_shim/extension_app_shim_handler_mac.h"
#include "chrome/browser/apps/app_window_registry_util.h"
#include "chrome/browser/profiles/profile.h"
#import "chrome/browser/ui/cocoa/apps/native_app_window_cocoa.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/grit/generated_resources.h"
#include "extensions/browser/app_window/app_window.h"
#include "extensions/common/extension.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/l10n/l10n_util_mac.h"

namespace {

// Gets an item from the main menu given the tag of the top level item
// |menu_tag| and the tag of the item |item_tag|.
NSMenuItem* GetItemByTag(NSInteger menu_tag, NSInteger item_tag) {
  return [[[[NSApp mainMenu] itemWithTag:menu_tag] submenu]
      itemWithTag:item_tag];
}

// Finds a top level menu item using |menu_tag| and creates a new NSMenuItem
// with the same title.
NSMenuItem* NewTopLevelItemFrom(NSInteger menu_tag) {
  NSMenuItem* original = [[NSApp mainMenu] itemWithTag:menu_tag];
  base::scoped_nsobject<NSMenuItem> item([[NSMenuItem alloc]
      initWithTitle:[original title]
             action:nil
      keyEquivalent:@""]);
  DCHECK([original hasSubmenu]);
  base::scoped_nsobject<NSMenu> sub_menu([[NSMenu alloc]
      initWithTitle:[[original submenu] title]]);
  [item setSubmenu:sub_menu];
  return item.autorelease();
}

// Finds an item using |menu_tag| and |item_tag| and adds a duplicate of it to
// the submenu of |top_level_item|.
void AddDuplicateItem(NSMenuItem* top_level_item,
                      NSInteger menu_tag,
                      NSInteger item_tag) {
  base::scoped_nsobject<NSMenuItem> item(
      [GetItemByTag(menu_tag, item_tag) copy]);
  DCHECK(item);
  [[top_level_item submenu] addItem:item];
}

// Finds an item with |item_tag| and removes it from the submenu of
// |top_level_item|.
void RemoveMenuItemWithTag(NSMenuItem* top_level_item,
                           NSInteger item_tag,
                           bool remove_following_separator) {
  NSMenu* submenu = [top_level_item submenu];
  NSInteger index = [submenu indexOfItemWithTag:item_tag];
  if (index < 0)
    return;

  [submenu removeItemAtIndex:index];

  if (!remove_following_separator || index == [submenu numberOfItems])
    return;

  NSMenuItem* nextItem = [submenu itemAtIndex:index];
  if ([nextItem isSeparatorItem])
    [submenu removeItem:nextItem];
}

}  // namespace

// Used by AppShimMenuController to manage menu items that are a copy of a
// Chrome menu item but with a different action. This manages unsetting and
// restoring the original item's key equivalent, so that we can use the same
// key equivalent in the copied item with a different action. If |resourceId_|
// is non-zero, this will also update the title to include the app name.
// If the copy (menuItem) has no key equivalent, and the title does not have the
// app name, then enableForApp and disable do not need to be called. I.e. the
// doppelganger just copies the item and sets a new action.
@interface DoppelgangerMenuItem : NSObject {
 @private
  base::scoped_nsobject<NSMenuItem> menuItem_;
  base::scoped_nsobject<NSMenuItem> sourceItem_;
  base::scoped_nsobject<NSString> sourceKeyEquivalent_;
  int resourceId_;
}

@property(readonly, nonatomic) NSMenuItem* menuItem;

// Get the source item using the tags and create the menu item.
- (id)initWithController:(AppShimMenuController*)controller
                 menuTag:(NSInteger)menuTag
                 itemTag:(NSInteger)itemTag
              resourceId:(int)resourceId
                  action:(SEL)action
           keyEquivalent:(NSString*)keyEquivalent;
// Set the title using |resourceId_| and unset the source item's key equivalent.
- (void)enableForApp:(const extensions::Extension*)app;
// Restore the source item's key equivalent.
- (void)disable;
@end

@implementation DoppelgangerMenuItem

- (NSMenuItem*)menuItem {
  return menuItem_;
}

- (id)initWithController:(AppShimMenuController*)controller
                 menuTag:(NSInteger)menuTag
                 itemTag:(NSInteger)itemTag
              resourceId:(int)resourceId
                  action:(SEL)action
           keyEquivalent:(NSString*)keyEquivalent {
  if ((self = [super init])) {
    sourceItem_.reset([GetItemByTag(menuTag, itemTag) retain]);
    DCHECK(sourceItem_);
    sourceKeyEquivalent_.reset([[sourceItem_ keyEquivalent] copy]);
    menuItem_.reset([[NSMenuItem alloc]
        initWithTitle:[sourceItem_ title]
               action:action
        keyEquivalent:keyEquivalent]);
    [menuItem_ setTarget:controller];
    [menuItem_ setTag:itemTag];
    resourceId_ = resourceId;
  }
  return self;
}

- (void)enableForApp:(const extensions::Extension*)app {
  // It seems that two menu items that have the same key equivalent must also
  // have the same action for the keyboard shortcut to work. (This refers to the
  // original keyboard shortcut, regardless of any overrides set in OSX).
  // In order to let the app menu items have a different action, we remove the
  // key equivalent of the original items and restore them later.
  [sourceItem_ setKeyEquivalent:@""];
  if (!resourceId_)
    return;

  [menuItem_ setTitle:l10n_util::GetNSStringF(resourceId_,
                                              base::UTF8ToUTF16(app->name()))];
}

- (void)disable {
  // Restore the keyboard shortcut to Chrome. This just needs to be set back to
  // the original keyboard shortcut, regardless of any overrides in OSX. The
  // overrides still work as they are based on the title of the menu item.
  [sourceItem_ setKeyEquivalent:sourceKeyEquivalent_];
}

@end

@interface AppShimMenuController ()
// Construct the NSMenuItems for apps.
- (void)buildAppMenuItems;
// Register for NSWindow notifications.
- (void)registerEventHandlers;
// If the window is an app window, add or remove menu items.
- (void)windowMainStatusChanged:(NSNotification*)notification;
// Add menu items for an app and hide Chrome menu items.
- (void)addMenuItems:(const extensions::Extension*)app;
// If the window belongs to the currently focused app, remove the menu items and
// unhide Chrome menu items.
- (void)removeMenuItems;
// If the currently focused window belongs to a platform app, quit the app.
- (void)quitCurrentPlatformApp;
// If the currently focused window belongs to a platform app, hide the app.
- (void)hideCurrentPlatformApp;
// If the currently focused window belongs to a platform app, focus the app.
- (void)focusCurrentPlatformApp;
@end

@implementation AppShimMenuController

- (id)init {
  if ((self = [super init])) {
    [self buildAppMenuItems];
    [self registerEventHandlers];
  }
  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  [super dealloc];
}

- (void)buildAppMenuItems {
  aboutDoppelganger_.reset([[DoppelgangerMenuItem alloc]
      initWithController:self
                 menuTag:IDC_CHROME_MENU
                 itemTag:IDC_ABOUT
              resourceId:IDS_ABOUT_MAC
                  action:nil
           keyEquivalent:@""]);
  hideDoppelganger_.reset([[DoppelgangerMenuItem alloc]
      initWithController:self
                 menuTag:IDC_CHROME_MENU
                 itemTag:IDC_HIDE_APP
              resourceId:IDS_HIDE_APP_MAC
                  action:@selector(hideCurrentPlatformApp)
           keyEquivalent:@"h"]);
  quitDoppelganger_.reset([[DoppelgangerMenuItem alloc]
      initWithController:self
                 menuTag:IDC_CHROME_MENU
                 itemTag:IDC_EXIT
              resourceId:IDS_EXIT_MAC
                  action:@selector(quitCurrentPlatformApp)
           keyEquivalent:@"q"]);
  newDoppelganger_.reset([[DoppelgangerMenuItem alloc]
      initWithController:self
                 menuTag:IDC_FILE_MENU
                 itemTag:IDC_NEW_WINDOW
              resourceId:0
                  action:nil
           keyEquivalent:@"n"]);
  // For apps, the "Window" part of "New Window" is dropped to match the default
  // menu set given to Cocoa Apps.
  [[newDoppelganger_ menuItem] setTitle:l10n_util::GetNSString(IDS_NEW_MAC)];
  openDoppelganger_.reset([[DoppelgangerMenuItem alloc]
      initWithController:self
                 menuTag:IDC_FILE_MENU
                 itemTag:IDC_OPEN_FILE
              resourceId:0
                  action:nil
           keyEquivalent:@"o"]);
  allToFrontDoppelganger_.reset([[DoppelgangerMenuItem alloc]
      initWithController:self
                 menuTag:IDC_WINDOW_MENU
                 itemTag:IDC_ALL_WINDOWS_FRONT
              resourceId:0
                  action:@selector(focusCurrentPlatformApp)
           keyEquivalent:@""]);

  // The app's menu.
  appMenuItem_.reset([[NSMenuItem alloc] initWithTitle:@""
                                                action:nil
                                         keyEquivalent:@""]);
  base::scoped_nsobject<NSMenu> appMenu([[NSMenu alloc] initWithTitle:@""]);
  [appMenuItem_ setSubmenu:appMenu];
  [appMenu setAutoenablesItems:NO];

  [appMenu addItem:[aboutDoppelganger_ menuItem]];
  [[aboutDoppelganger_ menuItem] setEnabled:NO];  // Not implemented yet.
  [appMenu addItem:[NSMenuItem separatorItem]];
  [appMenu addItem:[hideDoppelganger_ menuItem]];
  [appMenu addItem:[NSMenuItem separatorItem]];
  [appMenu addItem:[quitDoppelganger_ menuItem]];

  // File menu.
  fileMenuItem_.reset([NewTopLevelItemFrom(IDC_FILE_MENU) retain]);
  [[fileMenuItem_ submenu] addItem:[newDoppelganger_ menuItem]];
  [[fileMenuItem_ submenu] addItem:[openDoppelganger_ menuItem]];
  [[fileMenuItem_ submenu] addItem:[NSMenuItem separatorItem]];
  AddDuplicateItem(fileMenuItem_, IDC_FILE_MENU, IDC_CLOSE_WINDOW);
  // Set the expected key equivalent explicitly here because
  // -[AppControllerMac adjustCloseWindowMenuItemKeyEquivalent:] sets it to
  // "W" (Cmd+Shift+w) when a tabbed window has focus; it will change it back
  // to Cmd+w when a non-tabbed window has focus.
  NSMenuItem* closeWindowMenuItem =
      [[fileMenuItem_ submenu] itemWithTag:IDC_CLOSE_WINDOW];
  [closeWindowMenuItem setKeyEquivalent:@"w"];
  [closeWindowMenuItem setKeyEquivalentModifierMask:NSCommandKeyMask];

  // Edit menu. This copies the menu entirely and removes
  // "Paste and Match Style" and "Find". This is because the last two items,
  // "Start Dictation" and "Special Characters" are added by OSX, so we can't
  // copy them explicitly.
  editMenuItem_.reset([[[NSApp mainMenu] itemWithTag:IDC_EDIT_MENU] copy]);
  RemoveMenuItemWithTag(editMenuItem_,
                        IDC_CONTENT_CONTEXT_PASTE_AND_MATCH_STYLE, NO);
  RemoveMenuItemWithTag(editMenuItem_, IDC_FIND_MENU, NO);

  // View menu. Remove "Always Show Bookmark Bar" and separator.
  viewMenuItem_.reset([[[NSApp mainMenu] itemWithTag:IDC_VIEW_MENU] copy]);
  RemoveMenuItemWithTag(viewMenuItem_, IDC_SHOW_BOOKMARK_BAR, YES);

  // History menu.
  historyMenuItem_.reset([NewTopLevelItemFrom(IDC_HISTORY_MENU) retain]);
  AddDuplicateItem(historyMenuItem_, IDC_HISTORY_MENU, IDC_BACK);
  AddDuplicateItem(historyMenuItem_, IDC_HISTORY_MENU, IDC_FORWARD);

  // Window menu.
  windowMenuItem_.reset([NewTopLevelItemFrom(IDC_WINDOW_MENU) retain]);
  AddDuplicateItem(windowMenuItem_, IDC_WINDOW_MENU, IDC_MINIMIZE_WINDOW);
  AddDuplicateItem(windowMenuItem_, IDC_WINDOW_MENU, IDC_MAXIMIZE_WINDOW);
  [[windowMenuItem_ submenu] addItem:[NSMenuItem separatorItem]];
  [[windowMenuItem_ submenu] addItem:[allToFrontDoppelganger_ menuItem]];
}

- (void)registerEventHandlers {
  [[NSNotificationCenter defaultCenter]
      addObserver:self
         selector:@selector(windowMainStatusChanged:)
             name:NSWindowDidBecomeMainNotification
           object:nil];

  [[NSNotificationCenter defaultCenter]
      addObserver:self
         selector:@selector(windowMainStatusChanged:)
             name:NSWindowWillCloseNotification
           object:nil];
}

- (void)windowMainStatusChanged:(NSNotification*)notification {
  // A Yosemite AppKit bug causes this notification to be sent during the
  // -dealloc for a specific NSWindow. Any autoreleases sent to that window
  // must be drained before the window finishes -dealloc. In this method, an
  // autorelease is sent by the invocation of [NSApp windows].
  // http://crbug.com/406944.
  base::mac::ScopedNSAutoreleasePool pool;

  id window = [notification object];
  NSString* name = [notification name];
  if ([name isEqualToString:NSWindowDidBecomeMainNotification]) {
    extensions::AppWindow* appWindow =
        AppWindowRegistryUtil::GetAppWindowForNativeWindowAnyProfile(
            window);

    const extensions::Extension* extension = NULL;
    // If there is no corresponding AppWindow, this could be a hosted app, so
    // check for a browser.
    if (appWindow)
      extension = appWindow->GetExtension();
    else
      extension = apps::ExtensionAppShimHandler::GetAppForBrowser(
          chrome::FindBrowserWithWindow(window));

    if (extension)
      [self addMenuItems:extension];
    else
      [self removeMenuItems];
  } else if ([name isEqualToString:NSWindowWillCloseNotification]) {
    // If there are any other windows that can become main, leave the menu. It
    // will be changed when another window becomes main. Otherwise, restore the
    // Chrome menu.
    for (NSWindow* w : [NSApp windows]) {
      if ([w canBecomeMainWindow] && ![w isEqual:window])
        return;
    }

    [self removeMenuItems];
  } else {
    NOTREACHED();
  }
}

- (void)addMenuItems:(const extensions::Extension*)app {
  NSString* appId = base::SysUTF8ToNSString(app->id());
  NSString* title = base::SysUTF8ToNSString(app->name());

  if ([appId_ isEqualToString:appId])
    return;

  [self removeMenuItems];
  appId_.reset([appId copy]);

  // Hide Chrome menu items.
  NSMenu* mainMenu = [NSApp mainMenu];
  for (NSMenuItem* item in [mainMenu itemArray])
    [item setHidden:YES];

  [aboutDoppelganger_ enableForApp:app];
  [hideDoppelganger_ enableForApp:app];
  [quitDoppelganger_ enableForApp:app];
  [newDoppelganger_ enableForApp:app];
  [openDoppelganger_ enableForApp:app];

  [appMenuItem_ setTitle:appId];
  [[appMenuItem_ submenu] setTitle:title];

  [mainMenu addItem:appMenuItem_];
  [mainMenu addItem:fileMenuItem_];
  [mainMenu addItem:editMenuItem_];
  if (app->is_hosted_app()) {
    [mainMenu addItem:viewMenuItem_];
    [mainMenu addItem:historyMenuItem_];
  }
  [mainMenu addItem:windowMenuItem_];
}

- (void)removeMenuItems {
  if (!appId_)
    return;

  appId_.reset();

  NSMenu* mainMenu = [NSApp mainMenu];
  [mainMenu removeItem:appMenuItem_];
  [mainMenu removeItem:fileMenuItem_];
  if ([mainMenu indexOfItem:viewMenuItem_] >= 0)
    [mainMenu removeItem:viewMenuItem_];
  if ([mainMenu indexOfItem:historyMenuItem_] >= 0)
    [mainMenu removeItem:historyMenuItem_];
  [mainMenu removeItem:editMenuItem_];
  [mainMenu removeItem:windowMenuItem_];

  // Restore the Chrome main menu bar.
  for (NSMenuItem* item in [mainMenu itemArray])
    [item setHidden:NO];

  [aboutDoppelganger_ disable];
  [hideDoppelganger_ disable];
  [quitDoppelganger_ disable];
  [newDoppelganger_ disable];
  [openDoppelganger_ disable];
}

- (void)quitCurrentPlatformApp {
  extensions::AppWindow* appWindow =
      AppWindowRegistryUtil::GetAppWindowForNativeWindowAnyProfile(
          [NSApp keyWindow]);
  if (appWindow) {
    apps::ExtensionAppShimHandler::QuitAppForWindow(appWindow);
  } else {
    Browser* browser = chrome::FindBrowserWithWindow([NSApp keyWindow]);
    const extensions::Extension* extension =
        apps::ExtensionAppShimHandler::GetAppForBrowser(browser);
    if (extension)
      apps::ExtensionAppShimHandler::QuitHostedAppForWindow(browser->profile(),
                                                            extension->id());
  }
}

- (void)hideCurrentPlatformApp {
  extensions::AppWindow* appWindow =
      AppWindowRegistryUtil::GetAppWindowForNativeWindowAnyProfile(
          [NSApp keyWindow]);
  if (appWindow) {
    apps::ExtensionAppShimHandler::HideAppForWindow(appWindow);
  } else {
    Browser* browser = chrome::FindBrowserWithWindow([NSApp keyWindow]);
    const extensions::Extension* extension =
        apps::ExtensionAppShimHandler::GetAppForBrowser(browser);
    if (extension)
      apps::ExtensionAppShimHandler::HideHostedApp(browser->profile(),
                                                   extension->id());
  }
}

- (void)focusCurrentPlatformApp {
  extensions::AppWindow* appWindow =
      AppWindowRegistryUtil::GetAppWindowForNativeWindowAnyProfile(
          [NSApp keyWindow]);
  if (appWindow)
    apps::ExtensionAppShimHandler::FocusAppForWindow(appWindow);
}

@end