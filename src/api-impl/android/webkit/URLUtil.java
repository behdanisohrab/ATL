package android.webkit;

public class URLUtil {

	public static String guessFileName(String url, String contentDisposition, String mimeType) {
		return url.substring(url.lastIndexOf('/') + 1);
	}
}
