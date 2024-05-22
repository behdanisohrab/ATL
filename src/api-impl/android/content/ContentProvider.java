package android.content;

import android.content.res.XmlResourceParser;

public class ContentProvider {

	static void createContentProviders() throws Exception {
		XmlResourceParser parser = Context.this_application.getAssets().openXmlResourceParser("AndroidManifest.xml");
		for (; parser.getEventType() != XmlResourceParser.END_DOCUMENT; parser.next()) {
			if (parser.getEventType() != XmlResourceParser.START_TAG || !"provider".equals(parser.getName()))
				continue;
			String providerName = parser.getAttributeValue("http://schemas.android.com/apk/res/android", "name");
			if (providerName.startsWith(".")) {
				providerName = Context.this_application.getPackageName() + providerName;
			}
			System.out.println("creating " + providerName);
			Class<? extends ContentProvider> providerCls = Class.forName(providerName).asSubclass(ContentProvider.class);
			ContentProvider provider = providerCls.getConstructor().newInstance();
			provider.onCreate();
		}
		parser.close();
	}

	public boolean onCreate() {return false;}

	public Context getContext() {
		return Context.this_application;
	}

}
