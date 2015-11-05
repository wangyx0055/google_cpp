// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser.prerender;

import org.chromium.base.JNINamespace;
import org.chromium.base.VisibleForTesting;
import org.chromium.chrome.browser.ContentViewUtil;
import org.chromium.chrome.browser.profiles.Profile;

/**
 * A handler class for prerender requests coming from  other applications.
 */
@JNINamespace("prerender")
public class ExternalPrerenderHandler {

    private long mNativeExternalPrerenderHandler;

    public ExternalPrerenderHandler() {
        mNativeExternalPrerenderHandler = nativeInit();
    }

    /**
     * Add a prerender for the given url and given content view dimensions.
     * @param profile The profile to use for the prerender.
     * @param url The url to prerender.
     * @param referrer The referrer for the prerender request.
     * @param width The width for the content view (render widget host view) for the prerender.
     * @param height The height for the content view (render widget host view) for the prerender.
     * @return The native web contents pointer that is linked to this prerender. 0 if unsuccessful.
     */
    public long addPrerender(Profile profile, String url, String referrer, int width, int height) {
        long webContentsPtr = ContentViewUtil.createNativeWebContents(false);
        if (nativeAddPrerender(mNativeExternalPrerenderHandler, profile, webContentsPtr,
                url, referrer, width, height)) {
            return webContentsPtr;
        }
        ContentViewUtil.destroyNativeWebContents(webContentsPtr);
        return 0;
    }

    /**
     * Cancel the current prerender action on this {@link ExternalPrerenderHandler}.
     */
    public void cancelCurrentPrerender() {
        nativeCancelCurrentPrerender(mNativeExternalPrerenderHandler);
    }

    /**
     * Check whether a given url has been prerendering for the given profile and session id for the
     * given web contents.
     * @param profile The profile to check for prerendering.
     * @param url The url to check for prerender.
     * @param webContentsPtr The native pointer for which to compare the session info.
     * @return Whether the given url was prerendered.
     */
    public static boolean hasPrerenderedUrl(Profile profile, String url, long webContentsPtr)  {
        return nativeHasPrerenderedUrl(profile, url, webContentsPtr);
    }

    /**
     * Check whether the cookie store has been loaded. This is needed for prerender manager to allow
     * to add prerenders and this call is to be used to wait until that happens in tests.
     * @param profile The profile to be used.
     * @return Whether the cookie store has been loaded.
     */
    @VisibleForTesting
    public static boolean checkCookieStoreLoadedForTesting(Profile profile)  {
        return nativeHasCookieStoreLoaded(profile);
    }

    private static native long nativeInit();
    private static native boolean nativeAddPrerender(
            long nativeExternalPrerenderHandlerAndroid, Profile profile,
            long webContentsPtr, String url, String referrer, int width, int height);
    private static native boolean nativeHasPrerenderedUrl(
            Profile profile, String url, long webContentsPtr);
    private static native boolean nativeHasCookieStoreLoaded(
            Profile profile);
    private static native void nativeCancelCurrentPrerender(
            long nativeExternalPrerenderHandlerAndroid);
}
