package android.text;

public class Html {
	public static Spanned fromHtml(String source) {
		return new SpannableString(source.replace("<br/>", "\n")
		                                 .replace("<br>", "\n")
		                                 .replace("&nbsp;", " "));
		// TODO when JTidy is in use: s/<br \/>//g
	}

	public static String escapeHtml(CharSequence source) {
		StringBuilder out = new StringBuilder(source.length());
		for (int i = 0; i < source.length(); i++) {
			char c = source.charAt(i);
			if (c == '<' || c == '>' || c == '&' || c == '"' || c == '\'' || c > 0x7F) {
				out.append("&#");
				out.append((int) c);
				out.append(';');
			} else {
				out.append(c);
			}
		}
		return out.toString();
	}
}
