package android.content;

import android.content.pm.PackageParser;

public class ContentProvider {

	static void createContentProviders() throws Exception {
		for (PackageParser.Provider provider_parsed : Context.pkg.providers) {
			String providerName = provider_parsed.className;
			System.out.println("creating " + providerName);
			Class<? extends ContentProvider> providerCls = Class.forName(providerName).asSubclass(ContentProvider.class);
			ContentProvider provider = providerCls.getConstructor().newInstance();
			provider.onCreate();
		}
	}

	public boolean onCreate() {return false;}

	public Context getContext() {
		return Context.this_application;
	}

}
