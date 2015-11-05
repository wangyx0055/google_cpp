// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_RENDER_FRAME_H_
#define CONTENT_PUBLIC_RENDERER_RENDER_FRAME_H_

#include "base/callback_forward.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"
#include "third_party/WebKit/public/web/WebNavigationPolicy.h"

class GURL;

namespace blink {
class WebFrame;
class WebLocalFrame;
class WebNode;
class WebPlugin;
class WebURLRequest;
class WebURLResponse;
struct WebPluginParams;
}

namespace gfx {
class Range;
}

namespace v8 {
template <typename T> class Handle;
class Context;
class Isolate;
}

namespace content {
class ContextMenuClient;
class RenderView;
class ServiceRegistry;
struct ContextMenuParams;
struct WebPluginInfo;
struct WebPreferences;

// This interface wraps functionality, which is specific to frames, such as
// navigation. It provides communication with a corresponding RenderFrameHost
// in the browser process.
class CONTENT_EXPORT RenderFrame : public IPC::Listener,
                                   public IPC::Sender {
 public:
  enum PluginPowerSaverMode {
    // Plugin content is main content, and therefore never throttled.
    POWER_SAVER_MODE_ESSENTIAL = 0,
    // Plugin content is peripheral, but throttling is disabled.
    POWER_SAVER_MODE_PERIPHERAL_UNTHROTTLED = 1,
    // Plugin content is peripheral, and throttling is enabled.
    POWER_SAVER_MODE_PERIPHERAL_THROTTLED = 2
  };

  // Returns the RenderFrame given a WebFrame.
  static RenderFrame* FromWebFrame(blink::WebFrame* web_frame);

  // Returns the RenderView associated with this frame.
  virtual RenderView* GetRenderView() = 0;

  // Get the routing ID of the frame.
  virtual int GetRoutingID() = 0;

  // Returns the associated WebFrame.
  virtual blink::WebLocalFrame* GetWebFrame() = 0;

   // Gets WebKit related preferences associated with this frame.
  virtual WebPreferences& GetWebkitPreferences() = 0;

  // Shows a context menu with the given information. The given client will
  // be called with the result.
  //
  // The request ID will be returned by this function. This is passed to the
  // client functions for identification.
  //
  // If the client is destroyed, CancelContextMenu() should be called with the
  // request ID returned by this function.
  //
  // Note: if you end up having clients outliving the RenderFrame, we should add
  // a CancelContextMenuCallback function that takes a request id.
  virtual int ShowContextMenu(ContextMenuClient* client,
                              const ContextMenuParams& params) = 0;

  // Cancels a context menu in the event that the client is destroyed before the
  // menu is closed.
  virtual void CancelContextMenu(int request_id) = 0;

  // Gets the node that the context menu was pressed over.
  virtual blink::WebNode GetContextMenuNode() const = 0;

  // Create a new NPAPI/Pepper plugin depending on |info|. Returns NULL if no
  // plugin was found.
  virtual blink::WebPlugin* CreatePlugin(
      blink::WebFrame* frame,
      const WebPluginInfo& info,
      const blink::WebPluginParams& params,
      PluginPowerSaverMode power_saver_mode) = 0;

  // The client should handle the navigation externally.
  virtual void LoadURLExternally(blink::WebLocalFrame* frame,
                                 const blink::WebURLRequest& request,
                                 blink::WebNavigationPolicy policy) = 0;

  // Execute a string of JavaScript in this frame's context.
  virtual void ExecuteJavaScript(const base::string16& javascript) = 0;

  // Return true if this frame is hidden.
  virtual bool IsHidden() = 0;

  // Returns the ServiceRegistry for this frame.
  virtual ServiceRegistry* GetServiceRegistry() = 0;

#if defined(ENABLE_PLUGINS)
  // Registers a plugin that has been marked peripheral. If the origin
  // whitelist is later updated and includes |content_origin|, then
  // |unthrottle_callback| will be called.
  virtual void RegisterPeripheralPlugin(
      const GURL& content_origin,
      const base::Closure& unthrottle_callback) = 0;

  // Returns true if this plugin should have power saver enabled.
  //
  // Power Saver is enabled for plugin content that are cross-origin and
  // heuristically determined to be not essential to the web page content.
  //
  // Plugin content is defined to be cross-origin when the plugin source's
  // origin differs from the top level frame's origin. For example:
  //  - Cross-origin:  a.com -> b.com/plugin.swf
  //  - Cross-origin:  a.com -> b.com/iframe.html -> b.com/plugin.swf
  //  - Same-origin:   a.com -> b.com/iframe-to-a.html -> a.com/plugin.swf
  //
  // |page_frame_url| is the URL of the frame containing the plugin, which may
  // be different from the URL of the top level document.
  //
  // |poster_image| may be NULL. It is set to the absolute URL of the poster
  // image if it exists and this method returns true. Otherwise, an empty GURL.
  //
  // |cross_origin_main_content| may be NULL. It is set to true if the
  // plugin content is cross-origin but still the "main attraction" of the page.
  virtual bool ShouldThrottleContent(const blink::WebPluginParams& params,
                                     const GURL& page_frame_url,
                                     GURL* poster_image,
                                     bool* cross_origin_main_content) const = 0;

  // Whitelists a |content_origin| so its content will never be throttled in
  // this RenderFrame. Whitelist is cleared by top level navigation.
  virtual void WhitelistContentOrigin(const GURL& content_origin) = 0;
#endif

  // Returns true if this frame is a FTP directory listing.
  virtual bool IsFTPDirectoryListing() = 0;

  // Attaches the browser plugin identified by |element_instance_id| to guest
  // content created by the embedder.
  virtual void AttachGuest(int element_instance_id) = 0;

  // Detaches the browser plugin identified by |element_instance_id| from guest
  // content created by the embedder.
  virtual void DetachGuest(int element_instance_id) = 0;

  // Notifies the browser of text selection changes made.
  virtual void SetSelectedText(const base::string16& selection_text,
                               size_t offset,
                               const gfx::Range& range) = 0;

  // Ensures that builtin mojo bindings modules are available in |context|.
  virtual void EnsureMojoBuiltinsAreAvailable(
      v8::Isolate* isolate,
      v8::Handle<v8::Context> context) = 0;

 protected:
  ~RenderFrame() override {}

 private:
  // This interface should only be implemented inside content.
  friend class RenderFrameImpl;
  RenderFrame() {}
};

}  // namespace content

#endif  // CONTENT_PUBLIC_RENDERER_RENDER_FRAME_H_
