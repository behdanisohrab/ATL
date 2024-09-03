package android.webkit;

public class URLUtil {

	public static String guessFileName(String url, String contentDisposition, String mimeType) {
		String filename = url.substring(url.lastIndexOf('/') + 1);
		if (filename.contains("?"))
			filename = filename.substring(0, filename.indexOf('?'));
		return filename;
	}
}
