package com.hq.arscresourcesparser;

import com.hq.arscresourcesparser.arsc.ArscFile;
import com.hq.arscresourcesparser.arsc.ResTableEntry;
import com.hq.arscresourcesparser.arsc.ResTableValueEntry;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class ArscResourcesParser {
	private ArscFile arscFile;

    public ArscResourcesParser(String fileName) {
		try {
			ZipFile zip = new ZipFile(fileName);
			ZipEntry amz;
			amz = zip.getEntry("resources.arsc");
			InputStream amis = zip.getInputStream(amz);
			int BUFFER_SIZE = (int) (amz.getSize() > 51200 ? 51200 : amz.getSize());
			byte[] buf = new byte[BUFFER_SIZE];
			int bytesRead = amis.read(buf);
			arscFile = new ArscFile();
			arscFile.parse(buf);
			amis.close();
		} catch (IOException e) {
			System.out.println("ArscResourcesParser: IOException raised: " + e.toString());
		}
	}

	public String getResource(int resId) {
		ResTableValueEntry res = (ResTableValueEntry)arscFile.getResource(resId);

		return res.resValue.toString();
	}
}
