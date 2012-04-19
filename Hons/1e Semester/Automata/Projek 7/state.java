import java.util.*;

public class state{
	
	HashMap states;
	ArrayList starts,finals;
	HashMap hash;
	String name;
	public state(String name,HashMap hash){
		states = new HashMap();
		starts = new ArrayList();
		finals = new ArrayList();
		this.hash = hash;
		this.name = name;
	}
	
	
	
	
	public void add(String first, String trans, String last){
		System.out.println(first+">"+last+" on "+trans);
		states.put(first+":"+trans,last);
	}
	public void addFinal(String state){
		finals.add(state);
	}
	public void addStart(String state){
		starts.add(state);
	}
}