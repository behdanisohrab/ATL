package android.app;

import android.content.Context;

public class Dialog {
	public void show() {
		System.out.println("totally showing the Dialog "+this+" right now, most definitely doing that");
	}

	public void dismiss() {
		System.out.println("totally dismissing the Dialog "+this+" right now, which was most definitely being shown just a moment ago");
	}

	public class Builder {
		public Builder(Context context){
			System.out.println("making a Dialog$Builder as we speak, my word!");
		}
	}
}
