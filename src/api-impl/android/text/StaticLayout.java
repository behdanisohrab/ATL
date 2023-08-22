package android.text;

import android.text.Layout.Alignment;

public class StaticLayout {

	private CharSequence text;

	public StaticLayout(CharSequence source, int bufstart, int bufend,
						TextPaint paint, int outerwidth,
						Alignment align, TextDirectionHeuristic textDir,
						float spacingmult, float spacingadd,
						boolean includepad,
						TextUtils.TruncateAt ellipsize, int ellipsizedWidth, int maxLines) {
		this.text = source;
	}

	public CharSequence getText() {
		return text;
	}
}
