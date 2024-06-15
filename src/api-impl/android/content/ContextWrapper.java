package android.content;

public class ContextWrapper extends Context {
	private Context baseContext;

	public ContextWrapper(Context baseContext) {
		this.baseContext = baseContext;
	}

	public Context getBaseContext() {
		return baseContext;
	}

	protected void attachBaseContext(Context baseContext) {
		this.baseContext = baseContext;
	}

}
