package com.lukevear.facebookqml;

import android.content.Context;
import com.facebook.FacebookSdk;
import com.facebook.appevents.AppEventsLogger;
import com.facebook.FacebookCallback;
import com.facebook.login.LoginResult;
import com.facebook.FacebookException;
import android.content.Intent;

public class FacebookQml implements FacebookCallback<LoginResult> {

	static FacebookQml _instance;
	private AppEventsLogger logger;

	public FacebookQml(Context context) {
		this._instance = this;

		FacebookSdk.sdkInitialize(context);
		AppEventsLogger.activateApp(context);
		this.logger = AppEventsLogger.newLogger(context);
	}

	public static FacebookQml getInstance() {
		return _instance;
	}

	public AppEventsLogger getLogger() {
		return this.logger;
	}

    @Override
    public void onSuccess(LoginResult loginResult) {
        this.notifyLoginSuccess();
    }

    @Override
    public void onCancel() {
        this.notifyLoginCancelled();
    }

    @Override
    public void onError(FacebookException exception) {
        this.notifyLoginFailed(exception.toString());
    }

    // These native methods are registered in the FacebookQmlPlatformImpl constructor
    public static native void notifyLoginSuccess();
    public static native void notifyLoginFailed(String error);
    public static native void notifyLoginCancelled();
    public static native boolean onActivityResult(final int requestCode, final int resultCode, final Intent data);
}
