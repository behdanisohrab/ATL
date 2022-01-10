package com.hq.arscresourcesparser.arsc;

import com.hq.arscresourcesparser.common.Utils;
import com.hq.arscresourcesparser.stream.PositionInputStream;
import java.io.IOException;

import java.util.ArrayList;

/**
 *
 * Created by xueqiulxq on 26/07/2017.
 *
 * @author bilux (i.bilux@gmail.com)
 *
 */
public class ResTableMapEntry extends ResTableEntry {

    public ResTableRef parent;  // Reference parent ResTableMapEntry pkgId, if parent not exists the value should be zero.
    public long count;          // Num of ResTableMap following.
    public ResTableMap[] resTableMaps;

    public static ResTableMapEntry parseFrom(PositionInputStream mStreamer) throws IOException {
        ResTableMapEntry entry = new ResTableMapEntry();
        ResTableEntry.parseFrom(mStreamer, entry);

        entry.parent = ResTableRef.parseFrom(mStreamer);
        entry.count = Utils.readInt(mStreamer);

		if(entry.count < 1) {
			entry.resTableMaps = new ResTableMap[0];
			return entry;
		}

        entry.resTableMaps = new ResTableMap[(int) entry.count];
        for (int i = 0; i < entry.count; ++i) {
            entry.resTableMaps[i] = ResTableMap.parseFrom(mStreamer);
        }

        return entry;
    }

	@Override
	public String toString() {
        StringBuilder builder = new StringBuilder();
        builder.append("|----- ResTableMapEntry printout\n");
		for(ResTableMap map : resTableMaps) {
	        builder.append("|- map:\n");
			builder.append(map.toString()+"\n");
	        builder.append("|- end\n");
		}
        builder.append("|----- end of printout\n");
        return builder.toString();
	}

	public String[] asStringArray() {
			ArrayList<String> values = new ArrayList<String>();
			for(ResTableMap map : resTableMaps) {
				values.add(map.value.toString());
			}
			return values.toArray(new String[0]);
	}

    @Override
    public void translateValues(ResStringPoolChunk globalStringPool,
            ResStringPoolChunk typeStringPool,
            ResStringPoolChunk keyStringPool) {
        super.translateValues(globalStringPool, typeStringPool, keyStringPool);
        for (int i = 0; i < resTableMaps.length; ++i) {
            resTableMaps[i].translateValues(globalStringPool, typeStringPool, keyStringPool);
        }
    }
}
