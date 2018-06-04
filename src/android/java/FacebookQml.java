package com.lukevear.facebookqml;

import android.content.Context;
import android.content.Intent;
import com.facebook.FacebookSdk;
import com.facebook.FacebookCallback;
import com.facebook.login.LoginResult;
import com.facebook.FacebookException;
import com.facebook.AccessTokenTracker;
import com.facebook.AccessToken;

public class FacebookQml extends AccessTokenTracker implements FacebookCallback<LoginResult> {

    private static FacebookQml _instance;

    public static synchronized FacebookQml getInstance(Context context) {
        if (_instance == null) {
            _instance = new FacebookQml(context);
        }

        return _instance;
    }

    private FacebookQml(Context context) {
        FacebookSdk.sdkInitialize(context);
    }

    @Override
    public void onSuccess(LoginResult loginResult) {
        notifySuccess();
    }

    @Override
    public void onCancel() {
        notifyCancelled();
    }

    @Override
    public void onError(FacebookException exception) {
        notifyError(exception.toString());
    }

    @Override
    protected void onCurrentAccessTokenChanged(AccessToken oldAccessToken, AccessToken currentAccessToken) {
        notifyAccessTokenChanged();
    }

    // These native methods are registered in sdkSetup (facebookqmlandroid.cpp)
    public static native void notifySuccess();
    public static native void notifyCancelled();
    public static native void notifyError(String error);
    public static native void notifyAccessTokenChanged();
    public static native boolean onActivityResult(final int requestCode, final int resultCode, final Intent data);
}
