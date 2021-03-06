import java.util.*;
import java.io.*;

public class project2 {
	private static node root = null;
	private static ArrayList patterns = null;
	
	public static void searchPatterns(String filename) {
		try {
			String str,pattern;
			int index;
			ArrayList children,transitions;
			int[] counter =  new int[patterns.size()];
			
			FileInputStream f = new FileInputStream(filename);
			Reader input = new BufferedReader(new InputStreamReader(f));
			StreamTokenizer stream = new StreamTokenizer(input);
			stream.whitespaceChars(1,47);
			
			
			while (stream.nextToken() != StreamTokenizer.TT_EOF) {
				//System.out.println("Checking "+stream.sval);
				pattern = "";
				if (stream.sval != null) {
					str = new String(stream.sval);
					//System.out.println(str);
					index = 0;
					node current = root;
					
					while (index < str.length()) {
						transitions = current.getTransitions();
						children = current.getChildren();
						if (transitions.contains(new Character(str.charAt(index)))) {
							pattern = pattern + str.charAt(index);
							current = (node)children.get(transitions.indexOf(new Character(str.charAt(index))));
							if (current.isFinal()) {
								//System.out.println("Found pattern "+pattern);
								if (patterns.contains(pattern)) {
									counter[patterns.indexOf(pattern)]++;
								}else {
									System.out.println("Problem with "+pattern);
								}
							}
							index++;
						}else {
							//	System.out.println("Doesn't exist");
							if (current.getFailure() != null) {
								current = current.getFailure();
								pattern = pattern.substring(pattern.length() - current.getLength()-1);
							}else {
								break;
							}
							index++;
						}
					}
				}
			}
			f.close();
			for (int i = 0; i < counter.length; i++) {
				System.out.println("Found pattern "+patterns.get(i)+" "+counter[i]+" time(s).");
			}
			System.out.println("\nSearch complete\n");
		}catch (Exception e) {
			System.out.println("Error reading dictionary file. Exiting...");
			e.printStackTrace();
			System.exit(1);
		}
	}

	public static void main (String[] args) {
		if (args.length != 2) {
			System.out.println("Usage java project2 <pattern file> <dictionary file>");
			System.exit(1);
		}
		patterns = parse.run(args[0]);
		Object[] txt = patterns.toArray();
		System.out.println("\nSearching for following patterns");
		System.out.println("================================");
		/*for (int i = 0; i < txt.length; i++) {
			System.out.println(txt[i]);
		}*/
		root = buildTrie.createRoot(txt);
		System.out.println("\nCreating Failure Functions\n==========================");
		buildTrie.createFailureFunctions(root,"");
		System.out.println("\n\nSearching for patterns in "+args[1]+"\n\n============================");
		searchPatterns(args[1]);
	}
}