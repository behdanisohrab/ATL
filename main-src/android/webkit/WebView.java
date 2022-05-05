package android.webkit;

import android.content.Context;

// the only reason we need to implement this is that some app developers are such scumbags that they try to use this for tracking purposes
public class WebView {
	public WebView (Context context) {
	}

	public WebSettings getSettings() {
		return new WebSettings();
	}
}
