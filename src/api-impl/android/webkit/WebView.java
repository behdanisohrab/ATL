package android.webkit;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.AttributeSet;
import android.view.View;

public class WebView extends View {

	private WebViewClient webViewClient;

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

	public void setWebViewClient(WebViewClient webViewClient) {
		this.webViewClient = webViewClient;
	}

	// to be used by native code
	void internalLoadChanged(int loadState, String url) {
		if (loadState == /*WEBKIT_LOAD_STARTED*/0 && webViewClient != null) {
			webViewClient.onPageStarted(this, url);
		} else if (loadState == /*WEBKIT_LOAD_FINISHED*/3 && webViewClient != null) {
			webViewClient.onPageFinished(this, url);
		}
	}

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
		// webkit doesn't allow overwriting the file:// uri scheme. So we replace it with the android-asset:// scheme
		data = data.replace("file:///android_asset/", "android-asset:///assets/");
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

	// to be used by native code
	AssetManager internalGetAssetManager() {
		return getContext().getResources().getAssets();
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);
	private native void native_loadDataWithBaseURL(long widget, String baseUrl, String data, String mimeType, String encoding);
	private native void native_loadUrl(long widget, String url);
}
