package android.text;

public class Html {
	public static Spanned fromHtml(String source) {
		return new SpannableString(source.replace("<br/>", "\n")); // TODO when JTidy is in use: s/<br \/>//g
	}
}
