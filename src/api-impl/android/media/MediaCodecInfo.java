package android.media;

public class MediaCodecInfo {

	private String name;
	private String mime;

	public MediaCodecInfo(String name, String mime) {
		this.name = name;
		this.mime = mime;		
	}

	public String getName() {
		return name;
	}

	public boolean isEncoder() {
		return false;
	}

	public String[] getSupportedTypes() {
		return new String[] { mime };
	}

	public CodecCapabilities getCapabilitiesForType(String type) {
		return null;
	}

	public static class CodecCapabilities {}

	public static class CodecProfileLevel {}
}
