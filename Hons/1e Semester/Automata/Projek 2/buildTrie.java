import java.util.*;

public class buildTrie {
	
	private static node mainRoot = null;
	
	public static node createRoot(Object[] txt) {
		System.out.println("\nCreating trie\n=============");
		mainRoot = new node(null);
		mainRoot.setFailure(null,0);
		char cur;
		int index = 0;
		//ArrayList options = new ArrayList();
		node child;
		while (index < txt.length) {
			//System.out.println("\nMoving to next pattern\n");
			cur = ((String)(txt[index])).charAt(0);
			child = mainRoot.addChild((String)txt[index]);
			buildRest(child);
			index++;
		}
		return mainRoot;
	}
	public static void buildRest(node root) {
		char cur;
		root.setFailure(null,0);
		node child;
		if ((root.getString() == null) || (root.getString().length() == 0)) {
			root.setFinal();
			//System.out.println("Final state set");
		}else{
			child = root.addChild(root.getString());
			buildRest(child);
		}
	}
	
	public static void createFailureFunctions(node root, String str) {
		getFailure(root,str);
		int index = 0;
		node child;
		char cur;
		String watch;
		ArrayList children = root.getChildren();
		ArrayList trans = root.getTransitions();
		while (index < children.size()) {
			child = (node)children.get(index);
			cur = ((Character)trans.get(index)).charValue();
			watch = str+cur;
			//System.out.println("Watching "+watch);
			createFailureFunctions(child,watch);
			index++;
		}
	}
	
	public static void getFailure(node root,String str) {
		if (str.length() >= 2) {
			while (str.length() > 1) {
				str = str.substring(1);
				if (hasSuffix(root,str)) {
					break;
				}
			}
		}
	}
	
	private static boolean hasSuffix(node root,String str) {
		//System.out.println("Checking suffix "+str);
		int index = 0;
		node current = mainRoot;
		ArrayList children,transitions;
		while (index < str.length()) {
			transitions = current.getTransitions();
			children = current.getChildren();
			if (transitions.contains(new Character(str.charAt(index)))) {
				current = (node)children.get(transitions.indexOf(new Character(str.charAt(index))));
				index++;
			}else {
				//System.out.println("Doesn't exist");
				return false;
			}
		}
		root.setFailure(current,str.length());
		//System.out.println(str+" has a suffix");
		return true;
	}
}