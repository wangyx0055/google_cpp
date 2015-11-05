// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser.signin;

import android.content.Context;
import android.content.Intent;
import android.provider.Settings;

import org.chromium.base.CalledByNative;
import org.chromium.base.ThreadUtils;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.profiles.ProfileAccountManagementMetrics;

/**
 * Stub entry points and implementation interface for the account management screen.
 */
public class AccountManagementScreenHelper {
    private static AccountManagementScreenManager sManager;

    /*
     * TODO(guohui): add all Gaia service types.
     * Enum for the Gaia service types, must match GAIAServiceType in
     * signin_header_helper.h
     */
    public static final int GAIA_SERVICE_TYPE_SIGNUP = 5;

    private static final String EXTRA_ACCOUNT_TYPES = "account_types";
    private static final String EXTRA_VALUE_GOOGLE_ACCOUNTS = "com.google";

    /**
     * The screen manager interface.
     */
    public interface AccountManagementScreenManager {
        /**
         * Opens the account management UI screen.
         * @param applicationContext The application context.
         * @param profile The user profile.
         * @param gaiaServiceType A signin::GAIAServiceType value that triggered the dialog.
         */
        void openAccountManagementScreen(
                Context applicationContext, Profile profile, int gaiaServiceType);
    }

    /**
     * Sets the screen manager.
     * @param manager An implementation of AccountManagementScreenManager interface.
     */
    public static void setManager(AccountManagementScreenManager manager) {
        ThreadUtils.assertOnUiThread();
        sManager = manager;
    }

    @CalledByNative
    private static void openAccountManagementScreen(
            Context applicationContext, Profile profile, int gaiaServiceType) {
        ThreadUtils.assertOnUiThread();
        if (sManager == null) return;

        if (gaiaServiceType == GAIA_SERVICE_TYPE_SIGNUP)
            openAndroidAccountCreationScreen(applicationContext);
        else
            sManager.openAccountManagementScreen(applicationContext, profile, gaiaServiceType);
    }

    /**
     * Opens the Android account manager for adding or creating a Google account.
     * @param applicationContext
     */
    private static void openAndroidAccountCreationScreen(
            Context applicationContext) {
        logEvent(ProfileAccountManagementMetrics.DIRECT_ADD_ACCOUNT, GAIA_SERVICE_TYPE_SIGNUP);

        Intent createAccountIntent = new Intent(Settings.ACTION_ADD_ACCOUNT);
        createAccountIntent.putExtra(
                EXTRA_ACCOUNT_TYPES, new String[]{EXTRA_VALUE_GOOGLE_ACCOUNTS});
        createAccountIntent.setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT
                | Intent.FLAG_ACTIVITY_SINGLE_TOP | Intent.FLAG_ACTIVITY_NEW_TASK
                | Intent.FLAG_ACTIVITY_CLEAR_TOP);

        applicationContext.startActivity(createAccountIntent);
    }

    /**
     * Log a UMA event for a given metric and a signin type.
     * @param metric One of ProfileAccountManagementMetrics constants.
     * @param gaiaServiceType A signin::GAIAServiceType.
     */
    public static void logEvent(int metric, int gaiaServiceType) {
        nativeLogEvent(metric, gaiaServiceType);
    }

    // Native methods.
    private static native void nativeLogEvent(int metric, int gaiaServiceType);
}
