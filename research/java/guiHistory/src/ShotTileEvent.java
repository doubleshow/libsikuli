import java.util.EventObject;

public class ShotTileEvent extends EventObject {
	public int timestamp;
	public ShotTileEvent(ShotTile source, int marker_timestamp){
		super(source);
		timestamp = marker_timestamp;
	}
}
