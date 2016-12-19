package com.lukevear.facebookqml.login;

import com.facebook.FacebookCallback;
import com.facebook.login.LoginResult;
import com.facebook.FacebookException;

public class FacebookLoginCallbackManager implements FacebookCallback<LoginResult> {
    
    @Override
    public void onSuccess(LoginResult loginResult) {
        FacebookLoginCallbackManager.notifyLoginSuccess();
    }

    @Override
    public void onCancel() {
        FacebookLoginCallbackManager.notifyLoginCancelled();
    }

    @Override
    public void onError(FacebookException exception) {
        FacebookLoginCallbackManager.notifyLoginError(exception.toString());
    }

    // These native methods are registered in the FacebookLoginAndroid constructor
    public static native void notifyLoginSuccess();
    public static native void notifyLoginError(String error);
    public static native void notifyLoginCancelled();
}