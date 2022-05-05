package com.hq.arscresourcesparser;

import com.hq.arscresourcesparser.arsc.ArscFile;
import com.hq.arscresourcesparser.arsc.ResTableEntry;
import com.hq.arscresourcesparser.arsc.ResTableValueEntry;
import com.hq.arscresourcesparser.arsc.ResTableMapEntry;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ByteArrayOutputStream;
import java.net.URL;

//import java.util.zip.ZipEntry;
//import java.util.zip.ZipFile;

public class ArscResourcesParser {
	public final int TYPE_COLOR = 1;
	public final int TYPE_DRAWABLE = 2;
	public final int TYPE_LAYOUT = 3;
	public final int TYPE_DIMEN = 4;
	public final int TYPE_STRING = 5;
	public final int TYPE_ARRAY = 6;
	public final int TYPE_STYLE = 7;
	public final int TYPE_MENU = 8;
	public final int TYPE_ID = 9;

	private ArscFile arscFile;

	public ArscResourcesParser(URL file) {
		try {
			InputStream amis = file.openStream();
			System.out.println(amis);

			byte[] buffer = new byte[1000];
			ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
			try {
				int temp;

				while ((temp = amis.read(buffer)) != -1) {
					byteArrayOutputStream.write(buffer, 0, temp);
				}
			} catch (IOException e) {
				// Display the exception/s on the console
				System.out.println(e);
			}

			byte[] byteArray = byteArrayOutputStream.toByteArray();

			arscFile = new ArscFile();
			arscFile.parse(byteArray);
			amis.close();
		} catch (IOException e) {
			System.out.println("ArscResourcesParser: IOException raised: " + e.toString());
		}
	}

	public String getResource(int resId) {
		ResTableValueEntry res = (ResTableValueEntry)arscFile.getResource(resId);

		if(res == null) {
			return "oops, arscFile.getResource returned null";
		}

		return res.resValue.toString();
	}

	public String getResourceByName(String name, int typeId) {
		ResTableValueEntry res = (ResTableValueEntry)arscFile.getResourceByName(name, typeId);

		return res.resValue.toString();
	}

	public String[] getResourceArray(int resId) {
		ResTableMapEntry resArray = (ResTableMapEntry)arscFile.getResource(resId);

		return resArray.asStringArray();
	}

	public int getResourceId(String name, int typeId) {
		System.out.println("¯¯ in getResourceId");
		ResTableValueEntry res = (ResTableValueEntry)arscFile.getResourceByName(name, typeId);

		return res.entryId;
	}
}
