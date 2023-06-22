package android.webkit;

import android.content.Context;
import android.view.View;

// the only reason we need to implement this is that some app developers are such scumbags that they try to use this for tracking purposes
public class WebView extends View {
	public WebView(Context context) {
		super(context);
	}

	public WebSettings getSettings() {
		return new WebSettings();
	}

	public void setDownloadListener(DownloadListener downloadListener) {
	}

	public void setScrollBarStyle(int scrollBarStyle) {
	}

	public void setWebViewClient(WebViewClient webViewClient) {
	}
}
