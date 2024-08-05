package android.webkit;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

// the only reason we need to implement this is that some app developers are such scumbags that they try to use this for tracking purposes
public class WebView extends View {
	public WebView(Context context) {
		super(context);
	}

	public WebView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public WebSettings getSettings() {
		return new WebSettings();
	}

	public void setDownloadListener(DownloadListener downloadListener) {}

	public void setScrollBarStyle(int scrollBarStyle) {}

	public void setWebViewClient(WebViewClient webViewClient) {}

	public void setVerticalScrollBarEnabled(boolean enabled) {}

	public void addJavascriptInterface(Object dummy, String dummy2) {}

	public void setWebChromeClient(WebChromeClient dummy) {}

	public void removeAllViews() {}

	public void destroy() {}

	public void loadUrl(String url) {}

	public void stopLoading() {}

	public void loadDataWithBaseURL(String baseUrl, String data, String mimeType, String encoding, String historyUrl) {
		System.out.println("loadDataWithBaseURL(" + baseUrl + ", " + data + ", " + mimeType + ", " + encoding + ", " + historyUrl + ") called");
	}
}
