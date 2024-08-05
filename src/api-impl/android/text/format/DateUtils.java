package android.text.format;

import java.util.Date;

import android.content.Context;

public class DateUtils {

	public static CharSequence getRelativeTimeSpanString(Context context, long millis, boolean withPreposition) {
		return new Date(millis).toString();
	}
}
