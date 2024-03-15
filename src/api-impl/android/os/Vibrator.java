package android.os;

import android.util.Slog;

public class Vibrator {
	public void vibrate(long millis) {
		Slog.v("Vibrator", "vibration motor go burrrr for "+millis+"ms");
	}
	public void vibrate (final long[] pattern, int repeat) {
		Thread t = new Thread(new Runnable() {
			public void run() {
				for(int i = 0; i < pattern.length; i++) {
					if(i%2 == 0)
						try { Thread.sleep(pattern[i]); } catch(InterruptedException e) {}
					else
						vibrate(pattern[i]);
				}
			}
		});
		t.start();
	}
}
