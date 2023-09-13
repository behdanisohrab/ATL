package android.content;

import java.util.List;

import com.reandroid.arsc.chunk.xml.AndroidManifestBlock;
import com.reandroid.arsc.chunk.xml.ResXmlElement;

public class ContentProvider {

	static void createContentProviders() throws ReflectiveOperationException {
		List<ResXmlElement> providers = Context.manifest.getApplicationElement().listElements(AndroidManifestBlock.TAG_provider);
		System.out.println(providers);
		for (ResXmlElement providerElement : providers) {
			String providerName = providerElement.searchAttributeByResourceId(AndroidManifestBlock.ID_name).getValueAsString();
			if (providerName.startsWith(".")) {
				providerName = Context.manifest.getPackageName() + providerName;
			}
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
