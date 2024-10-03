package android.webkit;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

public class WebView extends View {
	public WebView(Context context) {
		this(context, null);
	}

	public WebView(Context context, AttributeSet attrs) {
		this(context, attrs, 0);
	}

	public WebView(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs, defStyleAttr);
	}

	public WebSettings getSettings() {
		return new WebSettings();
	}

	public void setDownloadListener(DownloadListener downloadListener) {}

	public void setScrollBarStyle(int scrollBarStyle) {}

	public void setWebViewClient(WebViewClient webViewClient) {}

	public void setVerticalScrollBarEnabled(boolean enabled) {}

	public void addJavascriptInterface(Object object, String name) {
		// HACK: directly call onRenderingDone for OctoDroid, as the javascript interface is not implemented yet
		if (object.getClass().getName().startsWith("com.gh4a") && "NativeClient".equals(name)) {
			try {
				object.getClass().getMethod("onRenderingDone").invoke(object);
			} catch (ReflectiveOperationException e) {
				e.printStackTrace();
			}
		}
	}

	public void setWebChromeClient(WebChromeClient dummy) {}

	public void removeAllViews() {}

	public void destroy() {}

	public void loadDataWithBaseURL(String baseUrl, String data, String mimeType, String encoding, String historyUrl) {
		native_loadDataWithBaseURL(widget, baseUrl, data, mimeType, encoding);
	}

	public void loadUrl(String url) {
		if (url.startsWith("javascript:")) {
			System.out.println("loadUrl: " + url + " - not implemented yet");
			return;
		}
		native_loadUrl(widget, url);
	}

	public void stopLoading() {}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);
	private native void native_loadDataWithBaseURL(long widget, String baseUrl, String data, String mimeType, String encoding);
	private native void native_loadUrl(long widget, String url);
}
