import java.util.*;
public class child extends Thread{
	HashMap states;
	ArrayList starts,finals,running,active,accepts,notifies;
	HashMap hash;
	private boolean waiting,hasStarted;
	String name;
	child parent;
	
	public child(state cur){
		states = cur.states;
		starts = cur.starts;
		finals = cur.finals;
		running = new ArrayList();
		active = new ArrayList();
		accepts = new ArrayList();
		notifies = new ArrayList();
		this.hash = cur.hash;
		this.name = cur.name;
		waiting = true;
		hasStarted = false;
	}
	
	public void setParent(child parent){
		this.parent = parent;
	}
	
	public void kill(){
		child node;
		for (int i=0;i<running.size();i++){
			node = (child)running.get(i);
			node.kill();
		}
		waiting = false;
	}
	
	public boolean hasChildren(){
		return !states.isEmpty();
	}
	
	public boolean allAccepted(){
		if (accepts.size() == starts.size()){
			waiting = false;
			return true;
		}else{
			return false;
		}
	}
	
	public synchronized  void notifyParent(String let,child node){
		System.out.println("notifying");
		
		
		if (node.allAccepted()){
			if (running.contains(node)){
				running.remove(running.indexOf(node));
			}
			//arunning.remove(running.indexOf(node));
			String str = node.name();
			node.kill();
			str = (String)states.get(str+":"+let);
			if (finals.contains(str)){
				System.out.println(str + " accepted in "+name);
				accepts.add(str);
			}else{
				if (str != null){
					notifies.add(str);
					System.out.println("Transition. to: "+str+" in node "+name+" on "+let);
				}else{
					accepts.add(str);
					if (parent != null){
						parent.notifyParent(let,this);
					}
					System.out.println("Reject "+node.name()+" on "+let+" at level "+name);
					
				}
			}
		}else{
			//node.pass(let)
		}
		
	}
	
	public String name(){
		return name;
	}
	
	public synchronized void pass(String let){
		String str;
		if (hasStarted){
			ArrayList temprun = new ArrayList();
			for (int i=0;i<running.size();i++){
				child node = (child)running.get(i);
				if (node.allAccepted()){
					str = node.name();
					
					
					str = (String)states.get(str+":"+let);
					if (finals.contains(str)){
						System.out.println(str + " accepted in "+name);
						accepts.add(str);
					}else{
						if (str != null){
							active.add(str);
							System.out.println("Transition to: "+str+" in node "+name+" on "+let);
						}else{
							accepts.add(str);
							if (parent != null){
								parent.notifyParent(let,this);
							}
							System.out.println("Rejecta "+node.name()+" on "+let+" at level "+name);
							
						}
					}
				}else{
					node.pass(let);
					temprun.add(node);
				}
			}
			running.clear();
			for (int i=0;i<temprun.size();i++){
				running.add(temprun.get(i));
			}
			ArrayList tempact = new ArrayList();
			for (int i=0;i<active.size();i++){
				str = (String)active.get(i);
				
				if (hash.containsKey(str)){
					state info = (state)hash.get(str);
					child node = new child(info);
					if (node.hasChildren()){
						System.out.println("Starting thread: "+str+" in "+name);
						
						running.add(node);
						node.setParent(this);
						node.start();
						node.pass(let);
					}else{
						System.out.println("Hoekom hier");
					}
				}else{
					String temp = str;
					str = (String)states.get(str+":"+let);
					if (finals.contains(str)){
						System.out.println(str + " accepted in "+name);
						accepts.add(str);
					}else{
						if (str != null){
							tempact.add(str);
							System.out.println("Transition to: "+str+" in node "+name+" on "+let);
						}else{
							accepts.add(str);
							if (parent != null){
								parent.notifyParent(let,this);
							}
							
							System.out.println("Reject "+temp+" on "+let+" at level "+name);
						}
					}
				}
			}
			active.clear();
			for (int i=0;i<tempact.size();i++){
				active.add(tempact.get(i));
			}
		}else{
			for (int i=0;i<starts.size();i++){
				str = (String)starts.get(i);
				
				if (hash.containsKey(str)){
					state info = (state)hash.get(str);
					child node = new child(info);
					if (node.hasChildren()){
						System.out.println("Starting thread: "+str+" from "+name);
						node.setParent(this);
						running.add(node);
						node.start();
						node.pass(let);
					}else{
						
					}
				}else{
					String temp = str;
					str = (String)states.get(str+":"+let);
					if (finals.contains(str)){
						System.out.println(str + " accepted in "+name);
						accepts.add(str);
					}else{
						if (str != null){
							active.add(str);
							System.out.println("Transition to: "+str+" in node "+name+" on "+let);
						}else{
							accepts.add(str);
							
							if (parent != null){
								parent.notifyParent(let,this);
							}
							System.out.println("Reject "+temp+" on "+let+" at level "+name);
						}
					}
				}
			}
			hasStarted = true;
		}
		for (int i=0;i<notifies.size();i++){
			active.add(notifies.get(i));
		}
		notifies.clear();
	}
	
	public void run(){
		
		
		while (waiting){
			
		}
		//destroy();
	}
}