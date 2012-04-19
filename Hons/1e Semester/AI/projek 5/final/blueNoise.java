import java.util.*;
import java.io.*;
import java.awt.image.*;
import java.awt.*;
import javax.imageio.*;

public class blueNoise{
	String train,output,imgName;
	boolean debug_out;
	int items,depth,imgNum = 0,statenum,tester = 0;
	double[] counts;
	HashMap hash;
	StreamTokenizer stream;
	state root;
	ArrayList red,blue,done;
	boolean[] passed;
	
	public blueNoise(String train,String output,String imgName, boolean debug_out){
		this.train = train;
		this.output = output;
		this.imgName = imgName;
		this.debug_out = debug_out;
		red = new ArrayList();
		blue = new ArrayList();
		hash = new HashMap();
		try {
			//setup the file to be read token by token
			FileInputStream f = new FileInputStream(train);
			Reader input = new BufferedReader(new InputStreamReader(f));
			stream = new StreamTokenizer(input);
			//end of line characters will also be read as tokens
			//stream.resetSyntax();
			//stream.wordChars(32,127);
			//stream.parseNumbers();
			stream.eolIsSignificant(true);
		}catch (Exception e){
			printf("\nError reading training file. Exiting...");
			System.exit(1);
		}
	}
//=============================================================	
//		private functions
//=============================================================
	private void debug(String str){
		if (debug_out){
			System.out.println(str);
		}
	}
	
	private void debugNoLn(String str){
		if (debug_out){
			System.out.print(str);
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
			printf("\nError reading file. Exiting...");
			System.exit(1);
		}
		return list;
	}
	
	
	public void draw(state leaf,Graphics2D g,int px,int py,int prev,int lvl){
		double l = lvl;
		counts[lvl]++;
		
		double xfraq = l / depth;
		double yfraq = counts[lvl] / (Math.pow(2,lvl)+1);
		int x = new Double(800*xfraq).intValue();
		int y = new Double(600*yfraq).intValue();
		
		leaf.x = x;
		leaf.y = y;
		done.add(leaf);
		
		int col = leaf.getColor();
		switch (col){
			case 1:
				g.setColor(Color.red);
				g.fillOval(x,y,20,20);
				break;
			case 2:
				g.setColor(Color.blue);
				g.fillOval(x,y,20,20);
				break;
			case 3:
				g.drawOval(x,y,20,20);
				break;
			default:printf("Unknown problem: "+col);
		}
		
		g.setColor(Color.black);
		if (leaf.getAccept()){
			g.drawString("A:"+leaf.num,x,y);
		}else{
			g.drawString(""+leaf.num,x,y);
		}
		
		g.drawLine(x+10,y+10,px+10,py+10);
		g.drawRect(x,y+15,3,3);
		g.drawString(""+prev,px+(x-px)/2,py+(y-py)/2+10);
		
		lvl++;
		
		if (leaf.getTrans(0) != null){
			if (!done.contains(leaf.getTrans(0))){
				draw(leaf.getTrans(0),g,x,y,0,lvl);
			}else{
				px = leaf.getTrans(0).x+10;
				py = leaf.getTrans(0).y+10;
				g.drawLine(x+10,y+10,px,py);
				g.drawRect(x,y+15,3,3);
				g.drawString(""+0,px+(x-px)/2,py+(y-py)/2+10);
			}
		}
		if (leaf.getTrans(1) != null){
			if (!done.contains(leaf.getTrans(1))){
				draw(leaf.getTrans(1),g,x,y,1,lvl);
			}else{
				px = leaf.getTrans(1).x+10;
				py = leaf.getTrans(1).y+10;
				g.drawLine(x+10,y+10,px,py);
				g.drawRect(x,y+15,3,3);
				g.drawString(""+1,px+(x-px)/2,py+(y-py)/2+10);
			}
		}
	}
	
	private int score(state node1, state node2){
		int l,r,count;
		state rn,bn;
		//check 0 transition
		bn = node1.getTrans(0);
		rn = node2.getTrans(0);
		if ((rn == null) || (bn == null)){
			l = 0;
		}else{
			if (rn.getAccept() != bn.getAccept()){
				return -2;
			}else{
				l = score(bn,rn)+1;
			}
		}
		//check 1 transition
		bn = node1.getTrans(1);
		rn = node2.getTrans(1);
		
		if ((rn == null) || (bn == null)){
			r = 0;
		}else{
			if (rn.getAccept() != bn.getAccept()){
				return -2;
			}else{
				r = score(bn,rn)+1;
			}
		}	
		if ((l == -2) && (r == -2)){
			return -1;
		}
		if (l == 0){
			return r;
		}
		if (r == 0){
			return l;
		}
		
		count = Math.max(l,r);
		
		return count;
	}
	
	private void merge(state[] pair){
		printf("Merging "+pair[0].num+" and "+pair[1].num);
		state temp;
		for (int i=0;i < red.size();i++){
			temp = (state)red.get(i);
			if (temp.getTrans(0) == pair[0]){
				temp.setTrans(0,pair[1]);
			}
			if (temp.getTrans(1) == pair[0]){
				temp.setTrans(1,pair[1]);
			}
		}
		blue.remove(blue.indexOf(pair[0]));
		
		
		
		mergeTrav(pair[0],pair[1]);
		
		
		for (int i=0;i < red.size();i++){
			temp = (state)red.get(i);
			if ((temp.getTrans(0) != null) && (temp.getTrans(0).getColor() == 3)){
				temp.getTrans(0).setColor(2);
				if (!blue.contains(temp.getTrans(0))){
					blue.add(temp.getTrans(0));
				}
			}
			if ((temp.getTrans(1) != null) && (temp.getTrans(1).getColor() == 3)){
				temp.getTrans(1).setColor(2);
				if (!blue.contains(temp.getTrans(1))){
					blue.add(temp.getTrans(1));
				}
			}
		}
	}
	
	private void mergeTrav(state node1,state node2){
		state bn,rn;
		
		bn = node1.getTrans(0);
		rn = node2.getTrans(0);
		if (rn == null){
			if (bn != null){
				node2.setTrans(0,bn);
			}
		}else{
			if (bn != null){
				mergeTrav(bn,rn);
				
			}
		}
		
		bn = node1.getTrans(1);
		rn = node2.getTrans(1);
		if (rn == null){
			if (bn != null){
				node2.setTrans(1,bn);
			}
		}else{
			if (bn != null){
				mergeTrav(bn,rn);
			}
		}
	}
	
	private void promote(){
		printf("Promoting all blues");
		state temp,temp2;
		ArrayList tempBlue = new ArrayList();
		while (!blue.isEmpty()){
			temp = (state)blue.remove(0);
			temp.setColor(1);
			red.add(temp);
			
		}
		for (int i=0;i<red.size();i++){
			temp = (state)red.get(i);
			
			temp2 = temp.getTrans(0);
			if ((temp2 != null) && (temp2.getColor() != 1)){
				temp2.setColor(2);
				blue.add(temp2);
			}
			temp2 = temp.getTrans(1);
			if ((temp2 != null) && (temp2.getColor() != 1)){
				temp2.setColor(2);
				blue.add(temp2);
			}
		}
	}
	
//=============================================================	
//		public functions
//=============================================================	
	public void loadHash(){
		ArrayList line;
		line = readLine(stream);
		items = ((Double)line.remove(0)).intValue();
		debug("There is "+items+" items");
		
		for (int i=0;i<items;i++){
			line = readLine(stream);
			hash.put(""+i,line);
		}
		passed = new boolean[items];
	}
	public void loadTrain(int prob){
		printf("\nLoading training data");
		statenum = 0;
		depth = 0;
		root = new state(statenum);
		statenum++;
		state cur = root,temp;
		red.clear();
		blue.clear();
		ArrayList line;
		
		
		
		
		//hash = new HashMap(items);
		int result,count,chr;
		
		for (int i=0;i<items;i++){
			if ((i != prob) && (hash.containsKey(""+i))){
				cur = root;
				//line = readLine(stream);
				
				line = (ArrayList)hash.get(""+i);
				
				//hash.put(""+i,line);
				result = ((Double)line.get(0)).intValue();
				count = ((Double)line.get(1)).intValue();
				if (count > depth){
					depth = count;
				}
				for (int j=2;j<count+2;j++){
					chr = ((Double)line.get(j)).intValue();
					if (cur.getTrans(chr) == null){
						temp = new state(statenum);
						statenum++;
						cur.setTrans(chr,temp);
						cur = temp;
					}else{
						cur = cur.getTrans(chr);
					}
				}
				if (result == 1){
					cur.setAccept();
				}
			}
		}
		depth++;
		
		
	}
	
	public void initColors(){
		printf("Doing initial node coloring\n");
		state cur;
		root.setColor(1);
		red.add(root);
		
		cur = root.getTrans(0);
		cur.setColor(2);
		blue.add(cur);
		
		cur = root.getTrans(1);
		cur.setColor(2);
		blue.add(cur);
	}
	
	public boolean mergeAndScore(){
		state rednode,bluenode;
		int score,bestScore = -1;
		state[] best = new state[2];
		
		for (int i=0;i<red.size();i++){
			rednode = (state)red.get(i);
			for (int j=0;j<blue.size();j++){
				score = -1;
				bluenode = (state)blue.get(j);
				
				if (bluenode.getAccept() == rednode.getAccept()){
				
					debug("blue:"+bluenode.num+", red:"+rednode.num);
					score = score(bluenode,rednode);
					debug("score: "+score);
				}
				
				if (score > bestScore){
					best[0] = bluenode;
					best[1] = rednode;
					bestScore = score;
				}
			}
		}
		debug("best score: "+bestScore);
		if (bestScore > 0){
			debug("Best scoring states:"+best[0].num+" and "+best[1].num);
			merge(best);
			return true;
		}else{
			if (blue.isEmpty()){
				return false;
			}else{
				promote();
				return true;
			}
		}
	}
	
	public void write(){
		if (imgName == null){
			return;
		}
		printf("Writing image");
		BufferedImage img = new BufferedImage(800,600,BufferedImage.TYPE_INT_RGB);
		Graphics2D g = img.createGraphics();
		
		g.setColor(Color.white);
		g.fillRect(0,0,800,600);
		g.setColor(Color.black);
		counts = new double[depth];
		done = new ArrayList();
		for (int i=0;i<depth;i++){
			counts[i] = 0;
		}
		draw(root,g,-10,300,0,0);
		
		
		try{
			File f = new File(imgName+imgNum+".jpeg");
			ImageIO.write((RenderedImage)img,"jpeg",f);
		}catch (Exception e){
			printf("Error writing image data");
		}
		debug(imgName+imgNum+".jpeg written...");
		imgNum++;
		
	}
	
	public void dumpFile(){
		if (output == null){
			return;
		}
		printf("Writing dfa file");
		try{
			FileWriter f1 = new FileWriter(output);
			state temp;
			
			f1.write(red.size()+" 2\n");
			debug(red.size()+" 2");
			while (!red.isEmpty()){
			
				temp = (state)red.remove(0);
				
				f1.write(""+temp.num);
				debugNoLn(""+temp.num);
				if (temp.getAccept()){
					f1.write(" 1");
					debugNoLn(" 1");
				}else{
					f1.write(" 0");
					debugNoLn(" 0");
				}
				
				if (temp.getTrans(0) == null){
					f1.write(" -");
					debugNoLn(" -");
				}else{
					f1.write(" "+temp.getTrans(0).num);
					debugNoLn(" "+temp.getTrans(0).num);
				}
				
				if (temp.getTrans(1) == null){
					f1.write(" -");
					debug(" -");
				}else{
					f1.write(" "+temp.getTrans(1).num);
					debug(" "+temp.getTrans(1).num);
				}
				f1.write("\n");
				f1.flush();
			}
		}catch (Exception e){
			printf("Error writing dfa data");
		}
		if (!blue.isEmpty()){
			printf("There were blue nodes left. program error");
		}
		
	}
	
	public void test(int index){
		state cur = new state(-1);
		state temproot = cur;
		ArrayList line = (ArrayList)hash.get(""+index);
		int score = 0;
		int result = ((Double)line.get(0)).intValue();
		int count = ((Double)line.get(1)).intValue();
		int chr;
		for (int j=2;j<count+2;j++){
			chr = ((Double)line.get(j)).intValue();
			if (cur.getTrans(chr) == null){
				state temp = new state(statenum);
				statenum++;
				cur.setTrans(chr,temp);
				cur = temp;
			}else{
				cur = cur.getTrans(chr);
			}
		}
		score= score(root,temproot);
		if (score >= 0){
			passed[index] = true;
		}else{
			passed[index] = false;
		}
		
	}
	
	public void removeNoise(){
		for (int i=0;i<passed.length;i++){
			if (!passed[i]){
				hash.remove(""+i);
			}
		}
	}
	
//=============================================================	
//		Static functions
//=============================================================
	public static blueNoise parseParameters(String[] flags) {
		//default parameter values
		
		boolean d = false;
		String train = null,output = null,img = null;
		
		if (flags.length < 2) {
			return null;
		}
		int index = 0;
		while (index < (flags.length)) {
			if (flags[index].compareTo("-t") == 0) {
				index++;
				train = flags[index];
			}else {
				if (flags[index].compareTo("-d") == 0) {
					d = true;
				}else {
					if (flags[index].compareTo("-o") == 0) {
						index++;
						output = flags[index];
					}else{
						if (flags[index].compareTo("-i") == 0) {
							index++;
							img = flags[index];
						}
					}
				}
			}
			index++;
		}
		
		return new blueNoise(train,output,img,d);
	}
	
	

	public static void main(String[] args){
		blueNoise dfa = parseParameters(args);
		
		if (dfa == null){
			System.out.println("\nUsage blueNoise [options]");
			System.out.println("Options:");
			System.out.println("-o <file name>\tOutput file to save the dfa to");
			System.out.println("-t <file name>\tData file used for training");
			System.out.println("-i <file name>\tImage file to save DFA. Jpeg extension added automatically");
			System.out.println("-d\t\tTurn on debug output");
			System.exit(1);	
		}
		
		dfa.loadHash();
		for (int i=0;i<dfa.items;i++){
			dfa.loadTrain(i);
			dfa.initColors();
			dfa.write();
			while (dfa.mergeAndScore()){
				dfa.write();
				System.out.println();
			}
			dfa.test(i);
		}
		System.out.println("removing all lines with noise");
		dfa.removeNoise();
		System.out.println("Constructing DFA");
		dfa.loadTrain(-1);
		dfa.initColors();
		dfa.write();
		while (dfa.mergeAndScore()){
			dfa.write();
			System.out.println();
		}
		System.out.println("DFA created");
		dfa.dumpFile();
		
	}
}