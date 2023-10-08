package android.media;

public class MediaCodecList {

	public static int getCodecCount() {
		return 2;
	}

	public static MediaCodecInfo getCodecInfoAt(int index) {
		switch (index) {
		case 0:
			return new MediaCodecInfo("aac", "audio/mp4a-latm");
		case 1:
			return new MediaCodecInfo("h264", "video/avc");
		default:
			return null;
		}
	}

}
