# About The Package
FacebookQml is designed to provide quick and easy access to the Facebook SDK for the 80% use case.

Currently this package only supports Facebook Login (and sending the access token to a backend server). There are plans to extend this package to support local account management (such as exposing the Facebook profile) and deeper analytics integration, however there is no fixed date on a release (pull requests are welcome).

## Usage Example
```
import FacebookQml 2.0

Rectangle {
    // Listen for Facebook's SDK signals
    // ========================================================================
    Connections {
        target: Facebook;

        onLoginSuccessful: {
            console.log("Facebook Login Successful:", Facebook.accessToken);
        }

        onLoginCancelled: {
            console.log("Facebook Login Cancelled");
        }

        onLoginError: {
            console.log("Facebook Login Error:", error);
        }
    }

    // Our Facebook login button
    // ========================================================================
    Button {
        id: button:
        text: "Continue with Facebook";

        onClicked: {
            Facebook.login([
                "public_profile",
                "email"
            ]);
        }
    }
}
```

> Facebook.login() accepts an array of Facebook permissions that you would like to request.

## Getting Started

#### iOS

You'll need to follow the same installation steps that you would for a standard iOS project which can be found [here](https://developers.facebook.com/docs/facebook-login/ios).

The only custom step required when using iOS is telling FacebookQml where to find the Facebook SDK frameworks. You can do this by adding the following to your `.pro` file:

```
ios {
    # Include Facebook
    FACEBOOKSDKPATH = $$PWD/bundles/ios/Facebook
}
```

FacebookQml reqiures the following framework files:
* Bolts.framework
* FBSDKCoreKit.framework
* FBSDKLoginKit.framework

#### Android

You'll (mostly) need to follow the same installation steps that you would for a standard Android project which can be found [here](https://developers.facebook.com/docs/facebook-login/android). Additionally you'll need to extend the default Qt Android activity so we can forward activity events to the Facebook SDK:

```java
package com.myapp;

import android.content.Intent;
import com.lukevear.facebookqml.FacebookQml;
import java.lang.UnsatisfiedLinkError;

public class MyCustomActivity extends org.qtproject.qt5.android.bindings.QtActivity {

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        try {
            FacebookQml.onActivityResult(requestCode, resultCode, data);
        } catch (UnsatisfiedLinkError e) {}
    }

}
```

> You should wrap all FacebookQml calls in a try/catch to prevent application crashes if the package is incorrectly initialised.

## Final Notes
You should always call `Facebook.logout()` as part of your logout workflow to ensure that the access token granted by Facebook is removed.