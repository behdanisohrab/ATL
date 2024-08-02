package android.media;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayDeque;
import java.util.Queue;
import android.view.Surface;

public class MediaCodec {

	public static final int BUFFER_FLAG_END_OF_STREAM = 0x4;

	private String codecName;
	private ByteBuffer[] inputBuffers;
	private ByteBuffer[] outputBuffers;
	private long[] inputBufferTimestamps;
	private long native_codec;
	private boolean outputFormatSet = false;
	private MediaFormat mediaFormat;
	private boolean isReleased = false;

	private Queue<Integer> freeOutputBuffers;
	private Queue<Integer> queuedInputBuffers;
	private Queue<Integer> freeInputBuffers;

	private MediaCodec(String codecName) throws IOException {
		this.codecName = codecName;
		native_codec = native_constructor(codecName);
		if (native_codec == 0) {
			throw new IOException("Unable to create MediaCodec: " + codecName);
		}
		inputBuffers = new ByteBuffer[1];
		inputBufferTimestamps = new long[inputBuffers.length];
		freeInputBuffers = new ArrayDeque<>(inputBuffers.length);
		queuedInputBuffers = new ArrayDeque<>(inputBuffers.length);
		for (int i = 0; i < inputBuffers.length; i++) {
			inputBuffers[i] = ByteBuffer.allocate(262144).order(ByteOrder.LITTLE_ENDIAN);
			freeInputBuffers.add(i);
		}
		outputBuffers = new ByteBuffer[2];
		freeOutputBuffers = new ArrayDeque<>(outputBuffers.length);
		for (int i = 0; i < outputBuffers.length; i++) {
			outputBuffers[i] = ByteBuffer.allocate(4096).order(ByteOrder.LITTLE_ENDIAN);
			freeOutputBuffers.add(i);
		}
	}

	public static MediaCodec createByCodecName(String codecName) throws IOException {
		return new MediaCodec(codecName);
	}

	public void configure(MediaFormat format, Surface surface, MediaCrypto crypto, int flags) {
		System.out.println("MediaCodec.configure(" + format + ", " + surface + ", " + crypto +	", " + flags + "): codecName=" + codecName);
		this.mediaFormat = format;
		if ("aac".equals(codecName)) {
			native_configure_audio(native_codec, format.getByteBuffer("csd-0"), format.getInteger("sample-rate"), format.getInteger("channel-count"));
		} else if ("h264".equals(codecName)) {
			native_configure_video(native_codec, format.getByteBuffer("csd-0"), format.getByteBuffer("csd-1"), surface);
		}
	}

	public void start() {
		System.out.println("MediaCodec.start(): codecName=" + codecName);
		native_start(native_codec);
	}

	public ByteBuffer[] getInputBuffers() {
		return inputBuffers;
	}

	public ByteBuffer[] getOutputBuffers() {
		return outputBuffers;
	}

	public int dequeueOutputBuffer(BufferInfo info, long timeoutUs) {
		if (!outputFormatSet) {
			outputFormatSet = true;
			return /*INFO_OUTPUT_FORMAT_CHANGED*/ -2;
		}
		Integer index = freeOutputBuffers.poll();
		if (index == null) {
			return /*INFO_TRY_AGAIN_LATER*/ -1;
		}
		outputBuffers[index].clear();
		int ret = native_dequeueOutputBuffer(native_codec, outputBuffers[index], info);
		if (ret == 0) {
			ret = index;
		} else {
			freeOutputBuffers.add(index);
			ret = /*INFO_TRY_AGAIN_LATER*/ -1;
		}
		return ret;
	}

	public void releaseOutputBuffer(int index, boolean render) {
		native_releaseOutputBuffer(native_codec, outputBuffers[index], render);
		freeOutputBuffers.add(index);
	}

	public MediaFormat getOutputFormat() {
		return mediaFormat;
	}

	public void flush() {}

	private void tryProcessInputBuffer() {
		Integer index = queuedInputBuffers.peek();
		if (index != null) {
			int ret;
			if (index == -1) {  // end of stream
				ret = native_queueInputBuffer(native_codec, null, 0);
			} else {
				ret = native_queueInputBuffer(native_codec, inputBuffers[index], inputBufferTimestamps[index]);
			}
			if (ret == 0) {
				queuedInputBuffers.remove(index);
				if (index != -1)
					freeInputBuffers.add(index);
			}
		}
	}

	public int dequeueInputBuffer(long timeoutUs) {
		tryProcessInputBuffer();
		Integer index = freeInputBuffers.poll();
		if (index == null) {
			return /*INFO_TRY_AGAIN_LATER*/ -1;
		} else {
			return index;
		}
	}

	public void queueInputBuffer(int index, int offset, int size, long presentationTimeUs, int flags) {
		if ((flags & BUFFER_FLAG_END_OF_STREAM) != 0) {
			queuedInputBuffers.add(-1);
			tryProcessInputBuffer();
			return;
		}
		inputBufferTimestamps[index] = presentationTimeUs;
		queuedInputBuffers.add(index);
		tryProcessInputBuffer();
	}

	public void setVideoScalingMode(int mode) {
		System.out.println("MediaCodec.setVideoScalingMode(" + mode + "): codecName=" + codecName);
	}

	public void release() {
		System.out.println("MediaCodec.release(): codecName=" + codecName);
		native_release(native_codec);
		isReleased = true;
	}

	@Override
	@SuppressWarnings("deprecation")
	protected void finalize() throws Throwable {
		try {
			super.finalize();
		} finally {
			if (!isReleased) {
				release();
			}
		}
	}

	private native long native_constructor(String codecName);
	private native void native_configure_audio(long codec, ByteBuffer extradata, int sampleRate, int channelCount);
	private native void native_configure_video(long codec, ByteBuffer csd0, ByteBuffer csd1, Surface surface);
	private native void native_start(long codec);
	private native int native_queueInputBuffer(long codec, ByteBuffer buffer, long presentationTimeUs);
	private native int native_dequeueOutputBuffer(long codec, ByteBuffer buffer, BufferInfo info);
	private native void native_releaseOutputBuffer(long codec, ByteBuffer buffer, boolean render);
	private native void native_release(long codec);

	public static final class CryptoInfo {}

	public static final class BufferInfo {
		public int size;
		public int flags;
		public int offset;
		public long presentationTimeUs;
	}

	public static interface OnFrameRenderedListener {}
	
}
