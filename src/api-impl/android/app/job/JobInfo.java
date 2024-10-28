package android.app.job;

import android.content.ComponentName;

public class JobInfo {
    public JobInfo() {
    }

    static public class Builder {
        public Builder(int jobId, ComponentName jobService) {
        }

        public Builder setOverrideDeadline(long a) {
            return this;
        }

        public JobInfo build() {
            return new JobInfo();
        }
    }
}
