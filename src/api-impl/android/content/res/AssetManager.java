/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.content.res;

import android.content.Context;
import android.os.ParcelFileDescriptor;
import android.os.Trace;
import android.util.AttributeSet;
import android.util.Log;
import android.util.TypedValue;

import com.reandroid.arsc.array.ResValueMapArray;
import com.reandroid.arsc.chunk.PackageBlock;
import com.reandroid.arsc.chunk.TableBlock;
import com.reandroid.arsc.chunk.xml.ResXmlDocument;
import com.reandroid.arsc.chunk.xml.ResXmlPullParser;
import com.reandroid.arsc.group.EntryGroup;
import com.reandroid.arsc.item.StringItem;
import com.reandroid.arsc.value.Entry;
import com.reandroid.arsc.value.EntryHeaderMap;
import com.reandroid.arsc.item.TableString;
import com.reandroid.arsc.value.ResValue;
import com.reandroid.arsc.value.ResValueMap;
import com.reandroid.arsc.value.ValueItem;
import com.reandroid.arsc.value.ValueType;
import com.reandroid.arsc.value.plurals.PluralsBag;
import com.reandroid.arsc.value.plurals.PluralsQuantity;

import java.io.FileDescriptor;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.List;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

/**
 * Provides access to an application's raw asset files; see {@link Resources}
 * for the way most applications will want to retrieve their resource data.
 * This class presents a lower-level API that allows you to open and read raw
 * files that have been bundled with the application as a simple stream of
 * bytes.
 */
public final class AssetManager {
	/* modes used when opening an asset */

	/**
	 * Mode for {@link #open(String, int)}: no specific information about how
	 * data will be accessed.
	 */
	public static final int ACCESS_UNKNOWN = 0;
	/**
	 * Mode for {@link #open(String, int)}: Read chunks, and seek forward and
	 * backward.
	 */
	public static final int ACCESS_RANDOM = 1;
	/**
	 * Mode for {@link #open(String, int)}: Read sequentially, with an
	 * occasional forward seek.
	 */
	public static final int ACCESS_STREAMING = 2;
	/**
	 * Mode for {@link #open(String, int)}: Attempt to load contents into
	 * memory, for fast small reads.
	 */
	public static final int ACCESS_BUFFER = 3;

	private static final String TAG = "AssetManager";
	private static final boolean localLOGV = false || false;

	private static final boolean DEBUG_REFS = false;

	private static final Object sSync = new Object();
	/*package*/ static AssetManager sSystem = null;

	private final TypedValue mValue = new TypedValue();
	private final long[] mOffsets = new long[2];

	// For communication with native code.
	private int mObject;
	private int mNObject; // used by the NDK

	private StringBlock mStringBlocks[] = null;

	private int mNumRefs = 1;
	private boolean mOpen = true;
	private HashMap<Integer, RuntimeException> mRefStacks;

	private List<TableBlock> tableBlocks;

	/**
	 * Create a new AssetManager containing only the basic system assets.
	 * Applications will not generally use this method, instead retrieving the
	 * appropriate asset manager with {@link Resources#getAssets}.    Not for
	 * use by applications.
	 * {@hide}
	 */
	public AssetManager() {
		try {
			tableBlocks = new ArrayList<>();
			Enumeration<URL> resources = ClassLoader.getSystemClassLoader().getResources("resources.arsc");
			while (resources.hasMoreElements()) {
				URL resource = resources.nextElement();
				if (!resource.getFile().contains("com.google.android.gms")) { // ignore MicroG .apk
					tableBlocks.add(TableBlock.load(resource.openStream()));
				}
			}
		} catch (IOException e) {
			Log.e(TAG, "failed to load resources.arsc" + e);
		}

		// FIXME: evaluate if this can be axed
		synchronized (this) {
			if (DEBUG_REFS) {
				mNumRefs = 0;
				incRefsLocked(this.hashCode());
			}
			init();
			if (localLOGV)
				Log.v(TAG, "New asset manager: " + this);
			//            ensureSystemAssets()
		}
	}

	private Map<Integer, EntryGroup> tableBlockCache = new HashMap<>();
	private EntryGroup tableBlockSearch(int resId) {
		if (tableBlockCache.containsKey(resId)) {
			return tableBlockCache.get(resId);
		}
		EntryGroup ret = null;
		for (TableBlock tableBlock : tableBlocks) {
			ret = tableBlock.search(resId);
			if (ret != null)
				break;
		}
		tableBlockCache.put(resId, ret);
		return ret;
	}

	private static void ensureSystemAssets() {
		synchronized (sSync) {
			if (sSystem == null) {
				AssetManager system = new AssetManager(true);
				system.makeStringBlocks(false);
				sSystem = system;
			}
		}
	}

	private AssetManager(boolean isSystem) {
		if (DEBUG_REFS) {
			synchronized (this) {
				mNumRefs = 0;
				incRefsLocked(this.hashCode());
			}
		}
		init();
		if (localLOGV)
			Log.v(TAG, "New asset manager: " + this);
	}

	/**
	 * Return a global shared asset manager that provides access to only
	 * system assets (no application assets).
	 * {@hide}
	 */
	public static AssetManager getSystem() {
		ensureSystemAssets();
		return sSystem;
	}

	/**
	 * Close this asset manager.
	 */
	public void close() {
		synchronized (this) {
			// System.out.println("Release: num=" + mNumRefs
			//                    + ", released=" + mReleased);
			if (mOpen) {
				mOpen = false;
				decRefsLocked(this.hashCode());
			}
		}
	}

	/**
	 * Retrieve the string value associated with a particular resource
	 * identifier for the current configuration / skin.
	 */
	/*package*/ final CharSequence getResourceText(int id) {
		if (id == 0)
			return "";
		ResValue resValue = tableBlockSearch(id).pickOne().getResValue();
		if (resValue.getValueType() == ValueType.REFERENCE) {
			if(id == resValue.getData()) {
				System.out.println("getResourceText: self-reference... returing \"\"");
				return "";
			}
			return getResourceText(resValue.getData());
		}
		if (resValue.getValueType() == ValueType.INT_COLOR_RGB8) {
			return String.format("#%08x", resValue.getData());
		}
		return resValue.getDataAsPoolString().get();
	}

	/**
	 * Retrieve the string value associated with a particular resource
	 * identifier for the current configuration / skin.
	 */
	/*package*/ final CharSequence getResourceBagText(int ident, int bagEntryId) {
		PluralsBag pluralsBag = PluralsBag.create(tableBlockSearch(ident).pickOne());
		return pluralsBag.getQuantityString(PluralsQuantity.valueOf((short)bagEntryId));

//		synchronized (this) {
//			TypedValue tmpValue = mValue;
//			int block = loadResourceBagValue(ident, bagEntryId, tmpValue, true);
//			if (block >= 0) {
//				if (tmpValue.type == TypedValue.TYPE_STRING) {
//					return mStringBlocks[block].get(tmpValue.data);
//				}
//				return tmpValue.coerceToString();
//			}
//		}
//		return null;
	}

	/**
	 * Retrieve the string array associated with a particular resource
	 * identifier.
	 * @param id Resource id of the string array
	 */
	/*package*/ final String[] getResourceStringArray(final int id) {
		ArrayList<String> values = new ArrayList<String>();
		for (ResValueMap map : tableBlockSearch(id).pickOne().getResValueMapArray().getChildes()) {
			if (map.getType() == TypedValue.TYPE_REFERENCE) {
				values.add(String.valueOf(getResourceText(map.getData())));
			} else if (map.getType() == TypedValue.TYPE_STRING) {
				values.add(map.getValueAsString());
			} else {
				values.add("value of unknown type " + map.getType());
			}
		}
		return values.toArray(new String[0]);
	}

	/*package*/ final boolean getResourceValue(int ident,
						   int density,
						   TypedValue outValue,
						   boolean resolveRefs) {
		/*int block = loadResourceValue(ident, (short) density, outValue, resolveRefs);
		if (block >= 0) {
		    if (outValue.type != TypedValue.TYPE_STRING) {
			return true;
		    }
		    outValue.string = mStringBlocks[block].get(outValue.data);
		    return true;
		}*/
		EntryGroup entryGroup = tableBlockSearch(ident);
		if (entryGroup == null)
			return false;  // not found
		ResValue resValue = entryGroup.pickOne().getResValue();
		if (resValue == null)
			return false;  // not found
		outValue.resourceId = ident;
		outValue.type = resValue.getType();
		outValue.data = resValue.getData();
		if (outValue.type == TypedValue.TYPE_STRING)
			outValue.string = getResourceText(ident);
		return true;
	}

	/**
	 * Retrieve the text array associated with a particular resource
	 * identifier.
	 * @param id Resource id of the string array
	 */
	/*package*/ final CharSequence[] getResourceTextArray(final int id) {
		ResValueMap children[] = tableBlockSearch(id).pickOne().getResValueMapArray().getChildes();
		CharSequence values[] = new CharSequence[children.length];
		for (int i = 0; i < children.length; i++) {
			StringItem stringItem = children[i].getDataAsPoolString();
			values[i] = (stringItem != null) ? stringItem.get() : "";
		}
		return values;
	}

	public Map<Integer,ValueItem> loadStyle(int resId) {
		Map<Integer,ValueItem> map = new HashMap<>();
		EntryGroup entryGroup = tableBlockSearch(resId);
		while (entryGroup != null) {
			Entry entry = entryGroup.pickOne();
			ResValueMapArray array = entry.getResValueMapArray();
			for (int i = 0; i < array.childesCount(); i++) {
				map.putIfAbsent(array.get(i).getName(), array.get(i));
			}
			int parent_id = ((EntryHeaderMap)entry.getHeader()).getParentId();
			entryGroup = tableBlockSearch(parent_id);
		}
		return map;
	}

	/*package*/ final boolean getThemeValue(Map<Integer,ValueItem> style, int ident,
						TypedValue outValue, boolean resolveRefs) {
		EntryGroup entryGroup = tableBlockSearch(ident);
		if (entryGroup == null)
			return false;
		Entry entry = entryGroup.pickOne();
		ValueItem valueItem = null;
		while ("attr".equals(entry.getTypeName())) {
			valueItem = null;
			if (style.containsKey(ident))
				valueItem = style.get(ident);
			if (valueItem != null && valueItem.getValueType() == ValueType.ATTRIBUTE) {
				ident = valueItem.getData();
				entryGroup = tableBlockSearch(ident);
				if (entryGroup == null) {
					break;
				}
				entry = entryGroup.pickOne();
			} else {
				break;
			}
		}

		ident = 0;
		if (valueItem != null && valueItem.getValueType() == ValueType.REFERENCE) {
			while (valueItem.getValueType() == ValueType.REFERENCE) {
				ident = valueItem.getData();
				valueItem = null;
				if (ident == 0)
					break;
				valueItem = tableBlockSearch(ident).pickOne().getResValue();
				if (valueItem == null)
					break;
			}
		}
		if (valueItem == null)
			return false;
		outValue.resourceId = ident;
		outValue.type = valueItem.getType();
		outValue.data = valueItem.getData();
		outValue.assetCookie = -1;
		if (outValue.type == TypedValue.TYPE_STRING) {
			outValue.string = valueItem.getDataAsPoolString().get();
		}
		return true;
	}

	/*package*/ final void ensureStringBlocks() {
		if (mStringBlocks == null) {
			synchronized (this) {
				if (mStringBlocks == null) {
					makeStringBlocks(true);
				}
			}
		}
	}

	/*package*/ final void makeStringBlocks(boolean copyFromSystem) {
		final int sysNum = copyFromSystem ? sSystem.mStringBlocks.length : 0;
		final int num = getStringBlockCount();
		mStringBlocks = new StringBlock[num];
		if (localLOGV)
			Log.v(TAG, "Making string blocks for " + this + ": " + num);
		for (int i = 0; i < num; i++) {
			if (i < sysNum) {
				mStringBlocks[i] = sSystem.mStringBlocks[i];
			} else {
				mStringBlocks[i] = new StringBlock(getNativeStringBlock(i), true);
			}
		}
	}

	/*package*/ final CharSequence getPooledString(int block, int id) {
		// System.out.println("Get pooled: block=" + block
		//                    + ", id=#" + Integer.toHexString(id)
		//                    + ", blocks=" + mStringBlocks);
		TableString string = tableBlocks.get(block).getStringPool().get(id);
		return string != null ? string.get() : null;
	}

	/**
	 * Open an asset using ACCESS_STREAMING mode.  This provides access to
	 * files that have been bundled with an application as assets -- that is,
	 * files placed in to the "assets" directory.
	 *
	 * @param fileName The name of the asset to open.  This name can be
	 *                 hierarchical.
	 *
	 * @see #open(String, int)
	 * @see #list
	 */
	public final InputStream open(String fileName) throws IOException {
		return open(fileName, ACCESS_STREAMING);
	}

	/**
	 * Open an asset using an explicit access mode, returning an InputStream to
	 * read its contents.  This provides access to files that have been bundled
	 * with an application as assets -- that is, files placed in to the
	 * "assets" directory.
	 *
	 * @param fileName The name of the asset to open.  This name can be
	 *                 hierarchical.
	 * @param accessMode Desired access mode for retrieving the data.
	 *
	 * @see #ACCESS_UNKNOWN
	 * @see #ACCESS_STREAMING
	 * @see #ACCESS_RANDOM
	 * @see #ACCESS_BUFFER
	 * @see #open(String)
	 * @see #list
	 */
	public final InputStream open(String fileName, int accessMode) throws IOException {
		int asset;
		// try loading from filesystem
		synchronized (this) {
			if (!mOpen) {
				throw new RuntimeException("Assetmanager has been closed");
			}
			asset = openAsset(fileName, accessMode);
			if (asset >= 0) {
				AssetInputStream res = new AssetInputStream(asset, "/assets/" + fileName);
				incRefsLocked(res.hashCode());
				return res;
			}
		}
		// alternatively load directly from APK
		InputStream ret = ClassLoader.getSystemClassLoader().getResourceAsStream("assets/" + fileName);
		if(ret == null)
			throw new FileNotFoundException("Asset file: " + fileName + ", errno: " + asset);

		return ret;
	}

	public final AssetFileDescriptor openFd(String fileName)
	    throws IOException {
		int asset;
		synchronized (this) {
			if (!mOpen) {
				throw new RuntimeException("Assetmanager has been closed");
			}
			asset = openAsset(fileName, 0);
			if (asset < 0)
				throw new FileNotFoundException("Asset file: " + fileName + ", errno: " + asset);

			FileDescriptor fd = new FileDescriptor();
			fd.setInt$(asset);
			ParcelFileDescriptor pfd = new ParcelFileDescriptor(fd);
			if (pfd != null) {
				AssetFileDescriptor afd = new AssetFileDescriptor(pfd, 0, getAssetLength(asset));
				afd.fileName = "/assets/" + fileName;
				return afd;
			}
		}
		throw new FileNotFoundException("Asset file: " + fileName);
	}

	/**
	 * Return a String array of all the assets at the given path.
	 *
	 * @param path A relative path within the assets, i.e., "docs/home.html".
	 *
	 * @return String[] Array of strings, one for each asset.  These file
	 *         names are relative to 'path'.  You can open the file by
	 *         concatenating 'path' and a name in the returned string (via
	 *         File) and passing that to open().
	 *
	 * @see #open
	 */
	public native final String[] list(String path)
	    throws IOException;

	/**
	 * {@hide}
	 * Open a non-asset file as an asset using ACCESS_STREAMING mode.  This
	 * provides direct access to all of the files included in an application
	 * package (not only its assets).  Applications should not normally use
	 * this.
	 *
	 * @see #open(String)
	 */
	public final InputStream openNonAsset(String fileName) throws IOException {
		return openNonAsset(0, fileName, ACCESS_STREAMING);
	}

	/**
	 * {@hide}
	 * Open a non-asset file as an asset using a specific access mode.  This
	 * provides direct access to all of the files included in an application
	 * package (not only its assets).  Applications should not normally use
	 * this.
	 *
	 * @see #open(String, int)
	 */
	public final InputStream openNonAsset(String fileName, int accessMode)
	    throws IOException {
		return openNonAsset(0, fileName, accessMode);
	}

	/**
	 * {@hide}
	 * Open a non-asset in a specified package.  Not for use by applications.
	 *
	 * @param cookie Identifier of the package to be opened.
	 * @param fileName Name of the asset to retrieve.
	 */
	public final InputStream openNonAsset(int cookie, String fileName)
	    throws IOException {
		return openNonAsset(cookie, fileName, ACCESS_STREAMING);
	}

	/**
	 * {@hide}
	 * Open a non-asset in a specified package.  Not for use by applications.
	 *
	 * @param cookie Identifier of the package to be opened.
	 * @param fileName Name of the asset to retrieve.
	 * @param accessMode Desired access mode for retrieving the data.
	 */
	public final InputStream openNonAsset(int cookie, String fileName, int accessMode) throws IOException {
		int asset;
		// try loading from filesystem
		synchronized (this) {
			if (!mOpen) {
				throw new RuntimeException("Assetmanager has been closed");
			}
			asset = openNonAssetNative(cookie, fileName, accessMode);
			if (asset >= 0) {
				AssetInputStream res = new AssetInputStream(asset, fileName);
				incRefsLocked(res.hashCode());
				return res;
			}
		}
		// alternatively load directly from APK
		return ClassLoader.getSystemClassLoader().getResourceAsStream(fileName);
	}

	public final AssetFileDescriptor openNonAssetFd(String fileName)
	    throws IOException {
		return openNonAssetFd(0, fileName);
	}

	public final AssetFileDescriptor openNonAssetFd(int cookie,
							String fileName) throws IOException {
		synchronized (this) {
			if (!mOpen) {
				throw new RuntimeException("Assetmanager has been closed");
			}
			ParcelFileDescriptor pfd = openNonAssetFdNative(cookie,
									fileName, mOffsets);
			if (pfd != null) {
				return new AssetFileDescriptor(pfd, mOffsets[0], mOffsets[1]);
			}
		}
		throw new FileNotFoundException("Asset absolute file: " + fileName);
	}

	/**
	 * Retrieve a parser for a compiled XML file.
	 *
	 * @param fileName The name of the file to retrieve.
	 */
	public final XmlResourceParser openXmlResourceParser(String fileName)
	    throws /*IO*/ Exception {
		return openXmlResourceParser(0, fileName);
	}

	/**
	 * Retrieve a parser for a compiled XML file.
	 *
	 * @param cookie Identifier of the package to be opened.
	 * @param fileName The name of the file to retrieve.
	 */
	public final XmlResourceParser openXmlResourceParser(int cookie,
							     String fileName) throws IOException {
		/*        XmlBlock block = openXmlBlockAsset(cookie, fileName);
			XmlResourceParser rp = block.newParser();
			block.close();
			return rp;*/

		InputStream inStream = ClassLoader.getSystemClassLoader().getResourceAsStream(fileName);
		if (inStream == null) {
			return null;
		}
		ResXmlDocument resXmlDocument = new ResXmlDocument();
		resXmlDocument.readBytes(inStream);
		ResXmlPullParser xpp = new ResXmlPullParser();
		xpp.setResXmlDocument(resXmlDocument);
		return xpp;
	}

	/**
	 * {@hide}
	 * Retrieve a non-asset as a compiled XML file.  Not for use by
	 * applications.
	 *
	 * @param fileName The name of the file to retrieve.
	 */
	/*package*/ final XmlBlock openXmlBlockAsset(String fileName)
	    throws IOException {
		return openXmlBlockAsset(0, fileName);
	}

	/**
	 * {@hide}
	 * Retrieve a non-asset as a compiled XML file.  Not for use by
	 * applications.
	 *
	 * @param cookie Identifier of the package to be opened.
	 * @param fileName Name of the asset to retrieve.
	 */
	/*package*/ final XmlBlock openXmlBlockAsset(int cookie, String fileName) throws IOException {
		int xmlBlock;
		synchronized (this) {
			if (!mOpen) {
				throw new RuntimeException("Assetmanager has been closed");
			}
			xmlBlock = openXmlAssetNative(cookie, fileName);
			if (xmlBlock >= 0) {
				XmlBlock res = new XmlBlock(this, xmlBlock);
				incRefsLocked(res.hashCode());
				return res;
			}
		}
		throw new FileNotFoundException("Asset XML file: " + fileName + ", errno : " + xmlBlock);
	}

	/*package*/ void xmlBlockGone(int id) {
		synchronized (this) {
			decRefsLocked(id);
		}
	}

	/*package*/ final int createTheme() {
		synchronized (this) {
			if (!mOpen) {
				throw new RuntimeException("Assetmanager has been closed");
			}
			int res = newTheme();
			incRefsLocked(res);
			return res;
		}
	}

	/*package*/ final void releaseTheme(int theme) {
		synchronized (this) {
			// deleteTheme(theme);
			decRefsLocked(theme);
		}
	}

	protected void finalize() throws Throwable {
		try {
			if (DEBUG_REFS && mNumRefs != 0) {
				Log.w(TAG, "AssetManager " + this + " finalized with non-zero refs: " + mNumRefs);
				if (mRefStacks != null) {
					for (RuntimeException e : mRefStacks.values()) {
						Log.w(TAG, "Reference from here", e);
					}
				}
			}
			destroy();
		} finally {
			super.finalize();
		}
	}

	public final class AssetInputStream extends InputStream {
		public final int getAssetInt() {
			return mAsset;
		}
		private AssetInputStream(int asset, String fileName) {
			mAsset = asset;
			mLength = getAssetLength(asset);
			this.fileName = fileName;
		}
		public final int read() throws IOException {
			return readAssetChar(mAsset);
		}
		public final boolean markSupported() {
			return true;
		}
		public final int available() throws IOException {
			long len = getAssetRemainingLength(mAsset);
			return len > Integer.MAX_VALUE ? Integer.MAX_VALUE : (int)len;
		}
		public final void close() throws IOException {
			synchronized (AssetManager.this) {
				if (mAsset != 0) {
					destroyAsset(mAsset);
					mAsset = 0;
					decRefsLocked(hashCode());
				}
			}
		}
		public final void mark(int readlimit) {
			mMarkPos = seekAsset(mAsset, 0, 0);
		}
		public final void reset() throws IOException {
			seekAsset(mAsset, mMarkPos, -1);
		}
		public final int read(byte[] b) throws IOException {
			return readAsset(mAsset, b, 0, b.length);
		}
		public final int read(byte[] b, int off, int len) throws IOException {
			return readAsset(mAsset, b, off, len);
		}
		public final long skip(long n) throws IOException {
			long pos = seekAsset(mAsset, 0, 0);
			if ((pos + n) > mLength) {
				n = mLength - pos;
			}
			if (n > 0) {
				seekAsset(mAsset, n, 0);
			}
			return n;
		}

		protected void finalize() throws Throwable {
			close();
		}

		private int mAsset;
		private long mLength;
		private long mMarkPos;
		public String fileName;
	}

	/**
	 * Add an additional set of assets to the asset manager.  This can be
	 * either a directory or ZIP file.  Not for use by applications.  Returns
	 * the cookie of the added asset, or 0 on failure.
	 * {@hide}
	 */
	public final int addAssetPath(String path) {
		int res = addAssetPathNative(path);
		return res;
	}

	private native final int addAssetPathNative(String path);

	/**
	 * Add multiple sets of assets to the asset manager at once.  See
	 * {@link #addAssetPath(String)} for more information.  Returns array of
	 * cookies for each added asset with 0 indicating failure, or null if
	 * the input array of paths is null.
	 * {@hide}
	 */
	public final int[] addAssetPaths(String[] paths) {
		if (paths == null) {
			return null;
		}

		int[] cookies = new int[paths.length];
		for (int i = 0; i < paths.length; i++) {
			cookies[i] = addAssetPath(paths[i]);
		}

		return cookies;
	}

	private static void extractFromAPK(String path, String target) throws IOException {
		if (path.endsWith("/")) { // directory
			try (JarFile apk = new JarFile(Context.this_application.getPackageCodePath())) {
				Enumeration<JarEntry> entries = apk.entries();
				while (entries.hasMoreElements()) {
					JarEntry entry = entries.nextElement();
					if (entry.getName().startsWith(path)) {
						extractFromAPK(entry.getName(), entry.getName().replace(path, target));
					}
				}
			}
		} else { // single file
			Path file = Paths.get(android.os.Environment.getExternalStorageDirectory().getPath(), target);
			if (!Files.exists(file)) {
				try (InputStream inputStream = ClassLoader.getSystemClassLoader().getResourceAsStream(path)) {
					if (inputStream != null) {
						Files.createDirectories(file.getParent());
						Files.copy(inputStream, file);
					}
				}
			}
		}
	}

	/**
	 * Determine whether the state in this asset manager is up-to-date with
	 * the files on the filesystem.  If false is returned, you need to
	 * instantiate a new AssetManager class to see the new data.
	 * {@hide}
	 */
	public native final boolean isUpToDate();

	/**
	 * Change the locale being used by this asset manager.  Not for use by
	 * applications.
	 * {@hide}
	 */
	public native final void setLocale(String locale);

	/**
	 * Get the locales that this asset manager contains data for.
	 */
	public native final String[] getLocales();

	/**
	 * Change the configuation used when retrieving resources.  Not for use by
	 * applications.
	 * {@hide}
	 */
	public /*native*/ final void setConfiguration(int mcc, int mnc, String locale,
						  int orientation, int touchscreen, int density, int keyboard,
						  int keyboardHidden, int navigation, int screenWidth, int screenHeight,
						  int smallestScreenWidthDp, int screenWidthDp, int screenHeightDp,
						  int screenLayout, int uiMode, int majorVersion) {}

	/**
	 * Retrieve the resource identifier for the given resource name.
	 */
	/*package*/ /*native*/ final int getResourceIdentifier(String name, String type, String defPackage) {
		System.out.println("getResourceIdentifier(" + name + "," + type + "," + defPackage + ") called");
		for (TableBlock tableBlock : tableBlocks) {
			for (PackageBlock packageBlock : tableBlock.listPackages()) {
				if (packageBlock.getName().equals(defPackage)) {
					Entry entry = packageBlock.getEntry("", type, name);
					if(entry != null) {
						return entry.getResourceId();
					} else {
						return -1; // TODO: investigate
					}
				}
			}
		}

		// package not found
		return -1;
	}

	/*package*/ /*native*/ final String getResourceName(int resid) {
		return tableBlockSearch(resid).pickOne().getName();
	}
	/*package*/ /*native*/ final String getResourcePackageName(int resid) {
		return tableBlockSearch(resid).pickOne().getPackageName();
	}
	/*package*/ /*native*/ final String getResourceTypeName(int resid) {
		return tableBlockSearch(resid).pickOne().getTypeName();
	}
	/*package*/ /*native*/ final String getResourceEntryName(int resid) {
		return tableBlockSearch(resid).pickOne().getName();
	}

	private native final int openAsset(String fileName, int accessMode);
	private final native ParcelFileDescriptor openAssetFd(String fileName,
							      long[] outOffsets) throws IOException;
	private /*native*/ final int openNonAssetNative(int cookie, String fileName, int accessMode) {
		return openAsset("../" + fileName, accessMode);
	}
	private native ParcelFileDescriptor openNonAssetFdNative(int cookie,
								 String fileName, long[] outOffsets) throws IOException;
	private native final void destroyAsset(int asset);
	private native final int readAssetChar(int asset);
	private native final int readAsset(int asset, byte[] b, int off, int len);
	private native final long seekAsset(int asset, long offset, int whence);
	private native final long getAssetLength(int asset);
	private native final long getAssetRemainingLength(int asset);

	/**
	 * Returns true if the resource was found, filling in mRetStringBlock and
	 *  mRetData.
	 */
	private native final int loadResourceValue(int ident, short density, TypedValue outValue,
						   boolean resolve);

	private int getCookie(ValueItem valueItem) {
		for (int i=0; i<tableBlocks.size(); i++) {
			if (valueItem.getStringPool() == tableBlocks.get(i).getStringPool())
				return i;
		}
		return -1;
	}
	/**
	 * Returns true if the resource was found, filling in mRetStringBlock and
	 *  mRetData.
	 */
	private native final int loadResourceBagValue(int ident, int bagEntryId, TypedValue outValue,
						      boolean resolve);
	/*package*/ static final int STYLE_NUM_ENTRIES = 6;
	/*package*/ static final int STYLE_TYPE = 0;
	/*package*/ static final int STYLE_DATA = 1;
	/*package*/ static final int STYLE_ASSET_COOKIE = 2;
	/*package*/ static final int STYLE_RESOURCE_ID = 3;
	/*package*/ static final int STYLE_CHANGING_CONFIGURATIONS = 4;
	/*package*/ static final int STYLE_DENSITY = 5;
	/*package*/ /*native*/ final boolean applyStyle(Map<Integer,ValueItem> theme,
							   int defStyleAttr, int defStyleRes, AttributeSet set,
							   int[] inAttrs, int[] outValues, int[] outIndices) {
		if (defStyleRes == 0 && theme.containsKey(defStyleAttr))
			defStyleRes = theme.get(defStyleAttr).getData();
		Map<Integer,ValueItem> defStyle = loadStyle(defStyleRes);

		ResXmlPullParser parser = (ResXmlPullParser)set;
		outIndices[0] = 0;

		Map<Integer,Integer> xmlCache = new HashMap<>();
		if (set != null) {
			for (int j=0; j<set.getAttributeCount(); j++) {
				xmlCache.put(set.getAttributeNameResource(j), j);
			}
		}
		for (int i = 0; i < inAttrs.length; i++) {
			int d = i*AssetManager.STYLE_NUM_ENTRIES;
			// reset values in case of cached array
			outValues[d+AssetManager.STYLE_RESOURCE_ID] = 0;
			outValues[d+AssetManager.STYLE_TYPE] = 0;
			outValues[d+AssetManager.STYLE_DATA] = 0;
			outValues[d+AssetManager.STYLE_ASSET_COOKIE] = 0;
			int resId = inAttrs[i];
			EntryGroup entryGroup = tableBlockSearch(resId);
			if (entryGroup == null)
				continue;
			Entry entry = entryGroup.pickOne();
			ValueItem valueItem = null;
			while ("attr".equals(entry.getTypeName())) {
				valueItem = null;
				if (xmlCache.containsKey(resId)) {
					valueItem = parser.getResXmlAttributeAt(xmlCache.get(resId));
				}
				if (valueItem == null) {
					valueItem = defStyle.get(resId);
					if (valueItem != null && valueItem.getValueType() == ValueType.ATTRIBUTE && valueItem.getData() == resId)
						valueItem = null;
				}
				if (valueItem == null)
					valueItem = theme.get(resId);
				if (valueItem != null && valueItem.getValueType() == ValueType.ATTRIBUTE) {
					resId = valueItem.getData();
					entryGroup = tableBlockSearch(resId);
					if (entryGroup == null) {
						break;
					}
					entry = entryGroup.pickOne();
				} else {
					break;
				}
			}
			if (valueItem == null)
				continue;
			if (valueItem.getValueType() == ValueType.REFERENCE) {
				while (valueItem.getValueType() == ValueType.REFERENCE) {
					resId = valueItem.getData();
					if (resId == 0)
						break;
					entry = tableBlockSearch(resId).pickOne();
					if (entry == null || entry.getResValue() == null)
						break;
					valueItem = entry.getResValue();
				}

				outValues[d + AssetManager.STYLE_RESOURCE_ID] = resId;
			}
			outValues[d + AssetManager.STYLE_TYPE] = valueItem.getType();
			outValues[d + AssetManager.STYLE_DATA] = valueItem.getData();
			outValues[d + AssetManager.STYLE_ASSET_COOKIE] = getCookie(valueItem);
			outIndices[++outIndices[0]] = i;
		}
		return true;
	}
	/*package*/ native final boolean retrieveAttributes(
	    int xmlParser, int[] inAttrs, int[] outValues, int[] outIndices);
	/*package*/ native final int getArraySize(int resource);
	/*package*/ native final int retrieveArray(int resource, int[] outValues);
	private native final int getStringBlockCount();
	private native final int getNativeStringBlock(int block);

	/**
	 * {@hide}
	 */
	public native final String getCookieName(int cookie);

	/**
	 * {@hide}
	 */
	public native static final int getGlobalAssetCount();

	/**
	 * {@hide}
	 */
	public native static final String getAssetAllocations();

	/**
	 * {@hide}
	 */
	public native static final int getGlobalAssetManagerCount();

	private native final int newTheme();
	private native final void deleteTheme(int theme);
	/*package*/ native static final void applyThemeStyle(int theme, int styleRes, boolean force);
	/*package*/ native static final void copyTheme(int dest, int source);
	/*package*/ native static final int loadThemeAttributeValue(int theme, int ident,
								    TypedValue outValue,
								    boolean resolve);
	/*package*/ native static final void dumpTheme(int theme, int priority, String tag, String prefix);

	private /*native*/ final int openXmlAssetNative(int cookie, String fileName) {
		return openAsset("../" + fileName, 0);
	}

	private native final String[] getArrayStringResource(int arrayRes);
	private native final int[] getArrayStringInfo(int arrayRes);
	/*package*/ final int[] getArrayIntResource(int arrayRes) {
		ResValueMap children[] = tableBlockSearch(arrayRes).pickOne().getResValueMapArray().getChildes();
		int values[] = new int[children.length];
		for (int i = 0; i < children.length; i++) {
			values[i] = children[i].getData();
		}
		return values;
	}

	private /* native */ final void init() {}
	private native final void destroy();

	private final void incRefsLocked(int id) {
		if (DEBUG_REFS) {
			if (mRefStacks == null) {
				mRefStacks = new HashMap<Integer, RuntimeException>();
				RuntimeException ex = new RuntimeException();
				ex.fillInStackTrace();
				mRefStacks.put(this.hashCode(), ex);
			}
		}
		mNumRefs++;
	}

	private final void decRefsLocked(int id) {
		if (DEBUG_REFS && mRefStacks != null) {
			mRefStacks.remove(id);
		}
		mNumRefs--;
		// System.out.println("Dec streams: mNumRefs=" + mNumRefs
		//                    + " mReleased=" + mReleased);
		if (mNumRefs == 0) {
			destroy();
		}
	}
}
