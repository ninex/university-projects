public class state{
	String trans,target;
	public state(String trans, String target){
		this.trans = trans;
		this.target = target;
	}
	
	public String getTrans(){
		return trans;
	}
	
	public String getTarget(){
		return target;
	}
}