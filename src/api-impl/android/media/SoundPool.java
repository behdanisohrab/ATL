package android.media;

import android.content.res.AssetFileDescriptor;

public class SoundPool {

    private long nativePool;

	public interface OnLoadCompleteListener {
        /**
         * Called when a sound has completed loading.
         *
         * @param soundPool SoundPool object from the load() method
         * @param soundPool the sample ID of the sound loaded.
         * @param status the status of the load operation (0 = success)
         */
        public void onLoadComplete(SoundPool soundPool, int sampleId, int status);
    }

	public SoundPool (int maxStreams, int streamType, int srcQuality) {
        nativePool = native_constructor();
	}

	public int load(AssetFileDescriptor afd, int priority) {
        return nativeLoad(nativePool, android.os.Environment.getExternalStorageDirectory().getPath() +  afd.fileName);
	}

	/**
     * Sets the callback hook for the OnLoadCompleteListener.
     */
    public void setOnLoadCompleteListener(OnLoadCompleteListener listener) {
        System.out.println("WARNING: SoundPool.setOnLoadCompleteListener not implemented yet");
    }

    public final int play(int soundID, float leftVolume, float rightVolume, int priority, int loop, float rate) {
        return nativePlay(nativePool, soundID);
    }

    private static native long native_constructor();
    private static native int nativeLoad(long nativePool, String path);
    public static native int nativePlay(long nativePool, int soundID);
}
