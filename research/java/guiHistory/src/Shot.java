
public class Shot {
	public int start_timestamp;
	public int end_timestamp;
	public int id;
	
	public Shot(int id, int start_timestamp, int end_timestamp){
		this.id = id;
		this.start_timestamp = start_timestamp;
		this.end_timestamp = end_timestamp;
	}
	
	public int getDuration(){
		return end_timestamp - start_timestamp;
	}
}
