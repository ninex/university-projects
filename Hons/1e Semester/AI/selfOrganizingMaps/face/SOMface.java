import java.util.*;
import java.io.*;

public class SOMface{

	private int RADIUS;

	int samples,classers,right,wrong;

	String sampleFile;
	boolean debugOut;
	Random ran;
	HashMap data,classifiers;
	StreamTokenizer stream;
	ArrayList files;

	public SOMface(String sample,boolean debugOut){
		sampleFile = sample;
		this.debugOut = debugOut;
		ran = new Random();
		data = new HashMap();
		classifiers = new HashMap();

		try {
			//setup the file to be read token by token
			FileInputStream f = new FileInputStream(sampleFile);
			Reader input = new BufferedReader(new InputStreamReader(f));
			stream = new StreamTokenizer(input);
			//end of line characters will also be read as tokens
			stream.resetSyntax();
			stream.wordChars(32,127);
			//stream.parseNumbers();
			stream.eolIsSignificant(true);
		}catch (Exception e){
			System.out.println("\nError reading sample file. Exiting...");
			System.exit(1);
		}
	}
	
//=============================================================	
//		private functions
//=============================================================
	private void debug(String str){
		if (debugOut){
			System.out.println(str);
		}
	}
	
	private void printf(String str){
		System.out.println(str);
	}
	
	private ArrayList readLine(StreamTokenizer stream){
		ArrayList list = new ArrayList();
		try{
			//read the file to the end
			while ((stream.nextToken() != StreamTokenizer.TT_EOF) && (stream.ttype != StreamTokenizer.TT_EOL)) {
				//is a word being read
				if (stream.ttype == StreamTokenizer.TT_WORD) {
					list.add(new String(stream.sval));
				}
				//is a number being read
				if (stream.ttype == StreamTokenizer.TT_NUMBER) {
					list.add(new Double(stream.nval));
				}
				if ((stream.ttype == StreamTokenizer.TT_EOL) && ( ((String)list.get(list.size() - 1) ).compareTo("\n") != 0 )) {
					list.add(new String("\n"));
				}
			}
		}catch (Exception e){
			System.out.println("\nError reading file. Exiting...");
			System.exit(1);
		}
		return list;
	}

	
	private double distance(ArrayList sample1, ArrayList sample2){
		int index = 0;
		double count = 0,s1,s2;
		while ((index != sample1.size()) || (index != sample2.size())){
			s1 = ((Double)sample1.get(index)).doubleValue();
			s2 = ((Double)sample2.get(index)).doubleValue();
			count = count + Math.pow(s1-s2,2);
			index++;
		}
		
		return Math.sqrt(count);
	}
	
	private String bestMatch(ArrayList sample){
		
		double dist,small = Double.MAX_VALUE;
		ArrayList list = new ArrayList(),item;
		
		for (int i=0;i<data.size();i++){
			item = (ArrayList)data.get(""+i);
			
			dist = distance(sample,item);
			if (dist < small){
				small = dist;
				list.clear();
				list.add(""+i);
			}else{
				if (dist == small){
					list.add(""+i);
				}
			}
			
		}
		
		if (list.size() == 1){
			return (String)list.get(0);
		}else{
			int index = ran.nextInt(list.size());
			return (String)list.get(index);
		}
	}


//=============================================================	
//		public functions
//=============================================================
	public void loadFileNames(){
		ArrayList line;
		files = new ArrayList();
		line = readLine(stream);
		while (!line.isEmpty()){
			files.add((String)line.remove(0));
			
			line = readLine(stream);
		}
	}
	
	public void loadSamples(){
		printf("Loading sample data");
		String name = null;
		samples = 0;
			classers = 0;
		for (int k=0;k<files.size();k++){
			String str = (String)files.get(k);
			
			
			for (int i=1;i<10;i++){
				name = str + "/"+i+"lr.int";
				try {
					
					debug("Loading"+name);
					//setup the file to be read token by token
					FileInputStream f = new FileInputStream(name);
					Reader input = new BufferedReader(new InputStreamReader(f));
					stream = new StreamTokenizer(input);
					//end of line characters will also be read as tokens
					//stream.resetSyntax();
					//stream.wordChars(32,127);
					//stream.parseNumbers();
					stream.eolIsSignificant(true);
				}catch (Exception e){
					System.out.println("\nError reading "+name+". Exiting...");
					System.exit(1);
				}
				
				ArrayList line,lines = new ArrayList();
				int r,g,b;
				
				line = readLine(stream);
				
				while (!line.isEmpty()){
					lines.add((Double)line.remove(0));
					
					
					line = readLine(stream);
				}
				if (i < 8){
					
					data.put(""+samples,lines);
					samples++;
				}else{
					
					classifiers.put(""+classers,lines);
					classers++;
				}
			}
		}
	}
	
	
	public void runAlgorithm(){
		printf("Running algorithm");
		right = 0;
		wrong = 0;
		ArrayList sample;
		int index,match,j,k;
		String s1,s2;
		
		
		for (int i=0;i<classifiers.size();i++){
			
			sample = (ArrayList)classifiers.get(""+i);
			match =new Integer(bestMatch(sample)).intValue();
			
			
			k = match / 7;
			j = i /2;
			
			s1 = (String)files.get(j);
			s2 = (String)files.get(k);
			
			debug(s1+"/"+(i%2 + 8)+"lr.int"+" was classified in "+s2+"/"+(match % 7 + 1)+"lr.int");
			
			if (s1.compareTo(s2) == 0){
				right++;
			}else{
				wrong++;
			}
		}
		
		printf("Predicted "+right+" correct and "+wrong+" wrong.");
		printf("Success rate:"+(right*100/(right+wrong))+"%");
		printf("Error rate:"+(wrong*100/(right+wrong))+"%");
		
		
	}
	
//=============================================================	
//		static functions
//=============================================================	
	public static SOMface parseParameters(String[] flags) {
		//default parameter values
		
		boolean d = false;
		String sample = null;
		
		if (flags.length < 2) {
			return null;
		}
		int index = 0;
		while (index < (flags.length)) {
			if (flags[index].compareTo("-s") == 0) {
				index++;
				sample = flags[index];
			}else {
				if (flags[index].compareTo("-d") == 0) {
					d = true;
				}else {
					
				}
			}
			index++;
		}
		
		return new SOMface(sample,d);
	}


	public static void main (String[] args){
		
		SOMface map = parseParameters(args);
		
		if (map == null){
			System.out.println("\nUsage SOMface [options]");
			System.out.println("Options:");
			System.out.println("-s <file name>\tSample data");
			System.out.println("-d\t\tTurn on debug output");
			System.exit(1);	
		}
		map.loadFileNames();
		map.loadSamples();
		map.runAlgorithm();
	}
}
