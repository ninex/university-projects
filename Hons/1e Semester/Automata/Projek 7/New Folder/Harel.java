import java.util.*;
import java.io.*;

public class Harel{

	public static void execute(String file,HashMap hash, String root){
		try{
			FileReader f = new FileReader(file);
			int status;
			String read;
			child node = (child)hash.get(root);
			node.start();
			while ((status = f.read()) != -1){
				read = String.valueOf((char)status);
				System.out.println(read);
				node.pass(read);
				System.out.println(Thread.activeCount());
			}
			if (node.allAccepted()){
				System.out.println("Statechart satisfied");
			}
		}catch (Exception e){
			System.out.println("Error processing input file");e.printStackTrace();
			System.exit(1);
		}
	}

	public static void main(String[] args){
		String def = null,input = null;
		if (args.length != 2){
			System.out.println("\nUsage: java Harel -d=<definition file> -i=<input file>");
			System.out.println("<definition file>: The file containing the definition of the State Diagram");
			System.out.println("<input file>: The file containing the input text to pass through the State Diagram\n");
			System.exit(1);
		}
		for (int i=0;i<args.length;i++){
			if (args[i].startsWith("-d=")){
				def = args[i].substring(3);
			}else{
				if (args[i].startsWith("-i=")){
					input = args[i].substring(3);
				}else{
					System.out.println("Invalid parameter: "+args[i]);
					System.exit(1);
				}
			}
		}
		if ((def == null) || (input == null)){
			System.out.println("\nUsage: java Harel -d=<definition file> -i=<input file>");
			System.out.println("<definition file>: The file containing the definition of the State Diagram");
			System.out.println("<input file>: The file containing the input text to pass through the State Diagram\n");
			System.exit(1);
		}
		parser parse = new parser(def);
		parse.load();
		HashMap hash = parse.run();
		String root = parse.getRoot();
		execute(input,hash,root);
	}
}