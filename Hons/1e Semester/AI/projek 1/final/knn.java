
import java.util.*;
import java.io.*;

public class knn {
	static boolean debug;
	
	int k,d,groups,amount;
	boolean s,w;
	String train,test;
	ArrayList[] set;
	int[][] voted = new int[3][3];ArrayList options = new ArrayList();
	
	public knn(int k, int d, boolean s, boolean w, String train, String test) {
		this.k = k;
		this.d = d;
		this.s = s;
		this.w = w;
		this.train = train;
		this.test = test;
		
		debug("\nTraining file: "+train);
		debug("Test file: "+test);
		debug("Number of neighbours: "+k);
		debug("Type of distance: "+d);
		debug("Is scaled: "+s);
		debug("Distant neighbours contribute less: "+w);
		
	}

	public void train() {
		//create a list that will store everything that's read from the input file
		ArrayList list = new ArrayList();
		try {
			//setup the file to be read token by token
			FileInputStream f = new FileInputStream(train);
			Reader input = new BufferedReader(new InputStreamReader(f));
			StreamTokenizer stream = new StreamTokenizer(input);
			
			//end of line characters will also be read as tokens
			stream.eolIsSignificant(true);
			
			//read the file to the end
			while (stream.nextToken() != StreamTokenizer.TT_EOF) {
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
			//close the file
			f.close();
		}catch (Exception e) {
			//any possible errors that may have occured
			//e.printStackTrace();
			System.out.println("\nError reading training file. Exiting...");
			System.exit(1);
		}
		
		groups = list.indexOf(new String("\n"));
		amount = list.size() / (groups+1);
		try{
			Double temp = (Double)list.get(groups-1);
			debug("No category definition");
		}catch (Exception e){
			//groups--;
		}
		debug("There are "+groups+" classification groups");
		
		debug("There are "+amount+" sets");
		
		set = new ArrayList[amount];
		for (int i=0;i< amount;i++) {
			set[i] = new ArrayList(groups);
		}
		for (int j=0;j<amount;j++) {
			for (int i=0;i<groups;i++) {
				set[j].add(list.remove(0));
			}
			list.remove(0);
		}
		debug("Training data loaded");
	}

	public ArrayList readLine(StreamTokenizer stream){
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
			System.out.println("\nError reading test file. Exiting...");
			System.exit(1);
		}
		return list;
	}

	public void predict() {
		
		try {
			//setup the file to be read token by token
			FileInputStream f = new FileInputStream(test);
			Reader input = new BufferedReader(new InputStreamReader(f));
			StreamTokenizer stream = new StreamTokenizer(input);
			
			//end of line characters will also be read as tokens
			stream.eolIsSignificant(true);
			
			debug("Reading test file");
			
			double distance,max = Double.MAX_VALUE;
			int max_index = 0;
			
			
			options.add("Iris-setosa");
			options.add("Iris-versicolor");
			options.add("Iris-virginica");

			ArrayList list = readLine(stream);
			
			while (!list.isEmpty()){
				ArrayList nn = new ArrayList(k);
				double[] distances = new double[k];	
				for (int i=0;i < amount;i++){
					distance = distance(set[i],list);
					//System.out.println("Distance = "+distance(set[i],list));
					if (nn.size() < k) {
						nn.add(set[i]);
						distances[nn.indexOf(set[i])] = distance;
						max_index = findmax(distances);
						max = distances[max_index];
					}else{
						if (distance < max) {
							nn.remove(max_index);
							nn.add(set[i]);
							distances[nn.indexOf(set[i])] = distance;
							max_index = findmax(distances);
							max = distances[max_index];
						}
					}
				}
				printNeighbours(nn);
				String result = vote(nn), actual = (String)list.get(list.size()-1);
				System.out.println("Voted: "+result+", data was: "+actual);
				if (result.compareTo(actual) != 0) {
					debug("\tVoted incorrectly");
				}
				voted[options.indexOf(actual)][options.indexOf(result)]++;
				list = readLine(stream);
			}
			debug("Done reading test file");
			//close the file
			f.close();
		}catch (Exception e) {
			//any possible errors that may have occured
			//e.printStackTrace();
			System.out.println("\nError reading test file. Exiting...");
			System.exit(1);
		}
	
	}
	
	public String vote(ArrayList nn) {
		ArrayList temp,votes = new ArrayList(),count = new ArrayList();
		int counter,index,max = -1;
		while (!nn.isEmpty()) {
			temp = (ArrayList)nn.remove(0);
			if (votes.contains(temp.get(temp.size()-1))) {
				index = votes.indexOf(temp.get(temp.size()-1));
				counter = ((Integer)count.get(index)).intValue() +1;
				count.set(index,new Integer(counter));
			}else {
				votes.add(temp.get(temp.size()-1));
				count.add((new Integer(1)));
			}
		}
		index = -1;
		while (!count.isEmpty()){
			counter = ((Integer)count.remove(count.size()-1)).intValue();
			if (counter > max) {
				index = count.size();
			}
		}
		return (String)votes.get(index);
	}
	
	public void printNeighbours(ArrayList nn) {
		ArrayList temp;
		int index = 0;
		while (index < nn.size()) {
			temp = (ArrayList)nn.get(index);
			debug("Item: "+ (String)(temp.get(temp.size()-1)));
			index++;
		}
	}
	
	public int findmax(double[] distances) {
		int index = -1;
		double max = -1;
		for (int i=0;i<distances.length;i++){
			if (distances[i] > max) {
				max = distances[i];
				index = i;
			}
		}
		return index;
	}
	
	public double distance(ArrayList a, ArrayList b) {
		switch (d) {
		case 0:return Euclidean(a,b);
		case 1:return Manhattan(a,b);
		case 2:return Hamming(a,b);
		}
		return -1;
	}

	public double Euclidean(ArrayList a, ArrayList b) {
		double count = 0;
		for (int i=0; i< (groups-1);i++) {
			count = count + Math.pow( ((Double)a.get(i)).doubleValue()-((Double)b.get(i)).doubleValue(),2);
		}
		return Math.sqrt(count);
	}
	
	public double Manhattan(ArrayList a, ArrayList b) {
		return 1;
	}
	
	public double Hamming(ArrayList a, ArrayList b) {
		return 1;
	}
	
	public void matrix(){
		debug("Confusion matrix");
		for (int i=0;i<3;i++){
			System.out.print(options.get(i)+"\t");
			for (int j=0;j<3;j++){
				System.out.print(" "+voted[i][j]);
			}
			System.out.println();
		}
	}
	
	public static void debug(String str) {
		if (debug) {
			System.out.println(str);
		}
	}
	
	public static knn parseParameters(String[] flags) {
		//default parameter values
		
		int k = 1,d = 0;
		boolean s = false,w = false;
		String train = null,test = null;
		
		if (flags.length < 2) {
			return null;
		}
		int index = 0;
		while (index < (flags.length-2)) {
			if (flags[index].compareTo("-k") == 0) {
				index++;
				try{
					k = Integer.parseInt(flags[index]);
				}catch (Exception e){
					System.out.println("\n-k: Value expected");
					return null;
				}
			}else {
				if (flags[index].compareTo("-s") == 0) {
					s = true;
				}else {
					if (flags[index].compareTo("-d") == 0) {
						index++;
						try{
							d = Integer.parseInt(flags[index]);
						}catch (Exception e){
							System.out.println("\n-d: Value expected");
							return null;
						}
					}else {
						if (flags[index].compareTo("-w") == 0) {
							w = true;
						}else {
							System.out.println("\nInvalid parameters");
							return null;
						}
					}
				}
			}
			index++;
		}
		
		train = flags[index];
		test = flags[index+1];
		
		return new knn(k,d,s,w,train,test);
	}
	
	public static void main (String[] args) {
		debug = true;
		knn neighbour = parseParameters(args);
		if (neighbour == null) {
			System.out.println("\nUsage knn [options] <training data file> <test data file>");
			System.out.println("Options:");
			System.out.println("-k [value]\tnumber of nearest neighbours");
			System.out.println("-s\t\tscale data");
			System.out.println("-d [value]\tType of distance. 0 Euclidean, 1 Manhattan, 2 Hamming");
			System.out.println("-w\t\tMore distant neighbours contribute less\n");
			System.exit(1);
		}
		neighbour.train();
		neighbour.predict();
		neighbour.matrix();
		//gui interf = new gui();
		//interf.show();
	}
}