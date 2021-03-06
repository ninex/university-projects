import java.util.*;
import java.io.*;

public class dtree{
	String train,output,prune;
	StreamTokenizer stream = null;
	String[] hdr;
	HashMap hash,phash;
	int count,pcount;
	node root = null;
	
	boolean debug_output;
	
	//constructor
	public dtree(String train,String prune,String output,boolean debug_output){
		this.train = train;
		this.output = output;
		this.prune = prune;
		this.debug_output = debug_output;
		try {
			//setup the file to be read token by token
			FileInputStream f = new FileInputStream(train);
			Reader input = new BufferedReader(new InputStreamReader(f));
			stream = new StreamTokenizer(input);
			//end of line characters will also be read as tokens
			stream.resetSyntax();
			stream.wordChars(32,127);
			//stream.parseNumbers();
			stream.eolIsSignificant(true);
		}catch (Exception e){
			System.out.println("\nError reading training file. Exiting...");
			System.exit(1);
		}
	}
	
	//public functions
	public void readHeader(){
		System.out.println("\nReading header\n");
		String header = (String)readLine(stream).remove(0);
		hdr = header.split(",");
		for (int i=0;i<hdr.length;i++){
			debug(hdr[i]);
		}
		
	}
	
	public void hashData(){
		
		System.out.println("\nLoading data into hashtable\n");
		hash = new HashMap();
		ArrayList line = readLine(stream);
		String[] items;
		count = 0;
		while (!line.isEmpty()){
			items = ((String)line.remove(0)).split(",");
			hash.put(""+count,items);
			//debug(""+line.get(line.size()-1));
			count++;
			line = readLine(stream);
		}
	}
	
	public void printData(){
		String[] temp;
		for (int i=0;i<count;i++){
			temp = (String[])hash.get(""+i);
			debug(temp[temp.length-1]);
		}
	}
	
	public double calcEntropy(boolean[] ilist){
		//debug("\nCalculating entropy\n");
		String temp = hdr[hdr.length-1];
		String[] temp2 = temp.split(":");
		String target = temp2[0];
		String type = temp2[1];
		if (type.compareTo("bool") != 0){
			System.out.println("Error: "+target+" must be of type boolean");
			System.exit(1);
		}
		double entropy;
		String[] options = new String[2];
		double[] counter = new double[2];
		options[0] = "";
		options[1] = "";
		counter[0] = 0;
		counter[1] = 0;
		for (int i=0;i<count;i++){
			if (ilist[i]){
				
				String[] line = (String[])hash.get(""+i);
				if (line[line.length-1].compareTo(options[0]) == 0){
					counter[0]++;
				}else{
					if (line[line.length-1].compareTo(options[1]) == 0){
						counter[1]++;
					}else{
						if (options[0].compareTo("") == 0){
							options[0] = line[line.length-1];
							counter[0]++;
						}else{
							if (options[1].compareTo("") == 0){
								options[1] = line[line.length-1];
								counter[1]++;
							}else{
								System.out.println("Error in data");
								System.exit(1);
							}
						}
					}
				}
			}
		}
		double fraq1 = counter[0]/(counter[0]+counter[1]);
		double fraq2 = counter[1]/(counter[0]+counter[1]);
		
		if (fraq1 == 0){
			entropy = 0;
		}else{
			entropy = 0 - fraq1* Math.log(fraq1)/Math.log(2);
		}
		if (fraq2 != 0){
			entropy = entropy - fraq2* Math.log(fraq2)/Math.log(2);
		}
		//System.out.println("Entropy:"+entropy);
		return entropy;
	}
	
	public void buildTree(){
		System.out.println("\nBuilding Tree\n");
		
		boolean[] ilist = new boolean[count];
		for (int i=0;i<ilist.length;i++){
			ilist[i] = true;
		}
		boolean[] cats = new boolean[hdr.length-1];
		for (int i=0;i<cats.length;i++){
			cats[i] = true;
		}
		root = new node(ilist,cats);
		root.setAttr("_root");
		branch(root);
		//hash = null;
	}
	
	public void dumpToFile(){
		if (output == null){
			System.out.println("No file specified to save tree to");
			System.exit(1);
		}
		System.out.println("\nSaving tree to "+output+"\n");
		try{
			File f1 = new File(output);
			FileOutputStream f = new FileOutputStream(f1);
			int level = -1;
			dumpANode(f,root,level);
		}catch (Exception e){
			System.out.println("Error opening "+output);
			System.exit(1);
		}
		
	}
	
	public void prune(){
		if (prune == null){
			return;
		}
		StreamTokenizer pstream = null;
		
		System.out.println("\nApplying reduced error pruning from set "+prune+"\n");
		try{
			FileInputStream f = new FileInputStream(prune);
			Reader input = new BufferedReader(new InputStreamReader(f));
			pstream = new StreamTokenizer(input);
			//end of line characters will also be read as tokens
			pstream.resetSyntax();
			pstream.wordChars(32,127);
			//stream.parseNumbers();
			pstream.eolIsSignificant(true);
		}catch (Exception e){
			System.out.println("Error reading "+prune);
			System.exit(1);
		}
		phash = new HashMap();
		ArrayList line = readLine(pstream);
		String[] items;
		pcount = 0;
		while (!line.isEmpty()){
			items = ((String)line.remove(0)).split(",");
			phash.put(""+pcount,items);
			//debug(""+line.get(line.size()-1));
			pcount++;
			line = readLine(pstream);
		}
		System.out.println("Prune set loaded into hashtable");
		
		//pruneTrav(hash);
		pruneTrav(phash);
	}
	
	//private functions
	private void debug(String str){
		if (debug_output){
			System.out.println(str);
		}
	}
	
	private void pruneTrav(HashMap has){
		String[] line;
		String attr;
		node cur,par;
		int index;
		ArrayList children,list = new ArrayList();
		boolean loop;
		
		for (int i=0;i<has.size();i++){
			
			loop = true;
			par = root;
			line = (String[])has.get(""+i);
			
			
			while (loop){
				if (par.getResult() != null){
					loop = false;
					//System.out.println("Found "+line[line.length-1]+" predicted "+par.getResult());
					if (line[line.length-1].compareTo(par.getResult()) == 0){
						par.right++;
					}else{//System.out.println(""+i);
					//System.exit(1);
						par.wrong++;
						if (!list.contains(par)){
							list.add(par);
						}
					}
				}else{
					index = par.getCritIndex();
					children = par.getChildren();
					boolean none = true;
					String[] str;
					str = hdr[index].split(":");
					if (str[1].compareTo("num") == 0){
						none = false;
						double mean = par.mean;
						if (new Double(line[index]).doubleValue() < mean){
							cur = (node)children.get(0);
						}else{
							cur = (node)children.get(1);
						}
						par = cur;
					}else{
						for (int j=0;j<children.size();j++){
							cur = (node)children.get(j);
							
							if (cur.getAttr().compareTo(line[index]) == 0){
								none = false;
								par =  cur;
								break;
							}
						}
					}
					if (none){
						System.out.println("Found undefined node for "+line[index]+" prediction "+line[line.length-1]);
						loop = false;
						cur = new node();
						cur.setAttr(line[index]);
						cur.setResult("_null");
						cur.wrong++;
						par.addChild(cur);
						list.add(cur);
					}
					
				}
			}
		}
		remove(list);
	}
	
	private void remove(ArrayList list){
		node temp;
		ArrayList parents = new ArrayList(),child;
		while (!list.isEmpty()){
			temp = (node)list.remove(0);
			if (!parents.contains(temp.getParent())){
				parents.add(temp.getParent());
			}
			//System.out.println(temp.getAttr()+" "+temp.right+":"+temp.wrong);
		}
		int[] errors = new int[parents.size()];
		int[] rights = new int[parents.size()];
		for (int i=0;i<parents.size();i++){
			temp = (node)parents.get(i);
			if (temp != null){
				child = temp.getChildren();
				for (int j=0;j<child.size();j++){
					temp = (node)child.get(j);
					errors[i] = errors[i] + temp.wrong;
					rights[i] = rights[i] + temp.right;
				}
				System.out.println(""+rights[i]+":"+errors[i]);
			}
		}
		
	}
	
	private void writeln(FileOutputStream f,String str){
		try{	
			for(int i=0;i<str.length();i++){
				f.write(str.charAt(i));
			}
			f.write('\n');
		}catch (Exception e){
			System.out.println("Error writing to "+output);
		}
	}
	
	private void dumpANode(FileOutputStream f,node leaf,int level){
		level++;
		ArrayList list = leaf.getChildren();
		String crit = leaf.getCrit();
		String attr = leaf.getAttr();
		node child;
		writeln(f,"Node");
		
		writeln(f,attr);
		if (crit!= null){
			String[] temp = crit.split(":");
			writeln(f,temp[0]);
		}else{
			//writeln(f,"");
		}
		writeln(f,""+level);
		if (leaf.getResult() != null){
			writeln(f,leaf.getResult());
		}
		
		while (!list.isEmpty()){
			child = (node)list.remove(0);
			dumpANode(f,child,level);
		}
	}
	
	private void branch(node leaf){
		double entropy = calcEntropy(leaf.getIlist());
		debug("Entropy: "+entropy);
		if (entropy > 0){
			leaf.setEntropy(entropy);
			double[] gain = calcInfoGain(leaf);
			double max=-1;
			int index=-1;
			for (int i=0;i<hdr.length-1;i++){
				if (gain[i] > max){
					max = gain[i];
					index = i;
				}
			}
			debug("Splitting node on attribute "+hdr[index]+" with info gain:"+gain[index]);
			if (max > 0){
				leaf.setSplitCriteria(hdr[index],index);
				split(leaf,index);
			}else{
				leaf.setResult(result(leaf));
				//System.out.println(result(leaf));
			}
		}else{
			leaf.setResult(result(leaf));
			//System.out.println(result(leaf));
		}
	}
	
	private void split(node leaf,int index){
		boolean[] cats = leaf.getCats();
		boolean[] ilist = leaf.getIlist();
		cats[index] = false;
		ArrayList options = new ArrayList();
		String[] str = hdr[index].split(":");
		String[] line;
		if (str[1].compareTo("num") == 0){
			double mean = getMean(index);
			leaf.addMean(mean);
			boolean[] newlist1 = new boolean[count];
			boolean[] newlist2 = new boolean[count];
			for (int i=0;i<count;i++){
				if (ilist[i]){
					line = (String[])hash.get(""+i);
					if (Double.valueOf(line[index]).doubleValue()<mean){
						newlist1[i] = true;
						newlist2[i] = false;
					}else{
						newlist2[i] = true;
						newlist1[i] = false;
					}
				}else{
					newlist1[i] = false;
					newlist2[i] = false;
				}
			}
			String tstr = ""+mean;
			int ij = tstr.indexOf('.');
			
			tstr = tstr.substring(0,ij+2);
			//System.out.println(tstr);
			node child = new node(newlist1,cats);
			child.setAttr("<"+tstr);
			child.setParent(leaf);
			leaf.addChild(child);
			branch(child);
			child = new node(newlist2,cats);
			child.setAttr(">="+tstr);
			leaf.addChild(child);
			branch(child);
			
		}else{
			
			for (int i=0;i<count;i++){
				if (ilist[i]){
					line = (String[])hash.get(""+i);
					if (!options.contains(line[index])){
						options.add(line[index]);
					}
				}
			}
			debug("Splitting with "+options.size()+" children");
			String option;
			int children = 0;
			while (!options.isEmpty()){
				option = (String)options.remove(0);
				children++;
				boolean[] newlist = new boolean[count];
				for (int i=0;i<count;i++){
					if (ilist[i]){
						line = (String[])hash.get(""+i);
						if (line[index].compareTo(option) == 0){
							newlist[i] = true;
						}else{
							newlist[i] = false;
						}
					}else{
						newlist[i] = false;
					}
				}
				node child = new node(newlist,cats);
				child.setAttr(option);
				child.setParent(leaf);
				leaf.addChild(child);
				if (!((children == 1) && (options.isEmpty()))){
					branch(child);
				}
				
			}
		}
	}
	
	private double[] calcInfoGain(node leaf){
		double entropy = leaf.getEntropy();
		boolean[] cats = leaf.getCats();
		boolean[] ilist = leaf.getIlist();
		double[] gain = new double[hdr.length-1];
		
		for (int j=0;j<hdr.length-1;j++){
			if (cats[j]){
				ArrayList options = new ArrayList();
				ArrayList counts = new ArrayList();
				double counter = 0;
				boolean[][] ilists= new boolean[0][count];
				String[] str = hdr[j].split(":");
				if (str[1].compareTo("num") == 0){
					double mean = getMean(j),c1=0,c2=0;
					ilists = new boolean[2][count];
					for (int i=0;i<count;i++){
						if (ilist[i]){
							String[] line = (String[])hash.get(""+i);
							if ((Double.valueOf(line[j]).doubleValue()) < mean){
								c1++;
								ilists[0][i] = true;
								ilists[1][i] = false;
							}else{
								c2++;
								ilists[1][i] = true;
								ilists[0][i] = false;
							}
						}
						counter++;
					}
					//debug("CCC"+c1+","+c2);
				
					counts.add(new Double(c1));
					counts.add(new Double(c2));
					
				}else{
					
					for (int i=0;i<count;i++){
						if (ilist[i]){
							String[] line = (String[])hash.get(""+i);
							//debug("Calculating info gain for "+line[j]);
							if (options.contains(line[j])){
								double temp = ((Double)counts.get(options.indexOf(line[j]))).doubleValue();
								counts.set(options.indexOf(line[j]),new Double(temp+1));
								ilists[options.indexOf(line[j])][i] = true;
							}else{
								options.add(line[j]);
								counts.add(new Double(1));
								boolean[][] temp2 = new boolean[ilists.length+1][count];
								for (int k=0;k<temp2.length-1;k++){
									temp2[k] = ilists[k];
								}
								for (int k=0;k<count;k++){
									temp2[ilists.length][k] = false;
								}
								ilists = temp2;
								ilists[counts.size()-1][i] = true;
							}
							counter++;
						}
					}
				}
				gain[j] = entropy;
				int index=0;
				
				while (!counts.isEmpty()){
					
					double temp2 = ((Double)counts.remove(0)).doubleValue() / counter;
					//debug(""+temp2);
					gain[j] = gain[j] - temp2*calcEntropy(ilists[index]);//a/entropy;
					index++;
				}
				gain[j] = gain[j]/entropy;
				debug("Info gain for "+hdr[j]+" is "+gain[j]);
			}
		}
		return gain;
	}
	
	private double getMean(int index){
		double mean,counter = 0;
		
		String[] line;
		for (int i=0;i<count;i++){
			line = (String[])hash.get(""+i);
				counter = counter + (Double.valueOf(line[index]).doubleValue());
			try{
				
			}catch (Exception e){
				System.out.println("Casting error");
				System.exit(1);
			}
		}
		//debug("Mean is "+counter/count);
		return counter/count;
	}
	
	private String result(node leaf){
		boolean[] ilist = leaf.getIlist();
		int i=0;
		while (!ilist[i]){
			i++;
		}
		String[] line = (String[])hash.get(""+i);
		return line[line.length-1];
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
			System.out.println("\nError reading test file. Exiting...");
			System.exit(1);
		}
		return list;
	}
	
	//static functions
	
	public static dtree parseParameters(String[] flags) {
		//default parameter values
		
		int k = 1;
		boolean d = false,w = false;
		String train = null,output = null,prune = null;
		
		if (flags.length < 1) {
			return null;
		}
		int index = 0;
		while (index < (flags.length-1)) {
			if (flags[index].compareTo("-o") == 0) {
				index++;
				output = flags[index];
			}else {
				if (flags[index].compareTo("-d") == 0) {
					d = true;
				}else {
					if (flags[index].compareTo("-p") == 0) {
						index++;
						prune = flags[index];
					}
				}
			}
			index++;
		}
		
		train = flags[index];
		//test = flags[index+1];
		
		return new dtree(train,prune,output,d);
	}
	
	
	
	public static void main(String[] args){
		dtree tree = parseParameters(args);
		if (tree == null) {
			System.out.println("\nUsage dtree [options] <training data file>");
			System.out.println("Options:");
			System.out.println("-o <file name>\tOutput file to save the tree to");
			System.out.println("-p <file name>\tData set used for error pruning");
			System.out.println("-d\t\tTurn on debug output");
			System.exit(1);
		}
		tree.readHeader();
		tree.hashData();
		tree.buildTree();
		tree.prune();
		tree.dumpToFile();
	}
}