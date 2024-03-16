package android.graphics.drawable;

public class AnimationDrawable extends Drawable {
	private int num_frames = 0;

	public int getNumberOfFrames() {
		return num_frames;
	}

	public void addFrame(Drawable drawable, int duration) {
		num_frames++;
	}
	public void start() {
		for(int i = 0; i < num_frames; i++) {
			run();
		}
	}
	public void stop() {}

	public void run() {}
}
