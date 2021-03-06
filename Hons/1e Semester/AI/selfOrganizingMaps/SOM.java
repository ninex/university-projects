import java.util.*;
import java.io.*;

public class SOM{

	private int RADIUS;

	int xdim,ydim,samples;
	float t = 0,grad;
	long iter = 1000;
	String sampleFile;
	boolean debugOut;
	Random ran;
	HashMap data;
	StreamTokenizer stream;
	col[][] grid,umap;
	gui iface,imap;

	public SOM(int x,int y,String sample,boolean debugOut){
		xdim = x;
		ydim = y;
		RADIUS = x*10;
		sampleFile = sample;
		this.debugOut = debugOut;
		ran = new Random();
		data = new HashMap();
		iface = new gui(x,y);
		iface.show();
		imap = new gui(x,y);
		imap.setLocation(400,0);
		imap.show();
		try {
			//setup the file to be read token by token
			FileInputStream f = new FileInputStream(sampleFile);
			Reader input = new BufferedReader(new InputStreamReader(f));
			stream = new StreamTokenizer(input);
			//end of line characters will also be read as tokens
			//stream.resetSyntax();
			//stream.wordChars(32,127);
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
	
	private col randomSample(){
		int i = ran.nextInt(samples);
		return (col)data.get(""+i);
	}
	
	private double distance(col sample1, col sample2){
		return Math.pow(sample1.r - sample2.r,2)+Math.pow(sample1.g - sample2.g,2)+Math.pow(sample1.b - sample2.b,2);
	}
	
	private col bestMatch(col sample){
		
		double dist,small = Double.MAX_VALUE;
		ArrayList list = new ArrayList();
		
		for (int i=0;i<xdim;i++){
			for (int j=0;j<ydim;j++){
				dist = distance(sample,grid[i][j]);
				if (dist < small){
					small = dist;
					list.clear();
					list.add(grid[i][j]);
				}else{
					if (dist == small){
						list.add(grid[i][j]);
					}
				}
			}
		}
		
		if (list.size() == 1){
			return (col)list.get(0);
		}else{
			int index = ran.nextInt(list.size());
			return (col)list.get(index);
		}
	}
	
	private double gauss(int x,int y,double normal){
		return Math.exp( (-20.0f/3.0f) * ((x*x+y*y)/normal));
	}
	
	
	private void scale(col match, col sample, float t){
		
		int rad = Math.round((RADIUS*(1.0f-t))/2.0f);
		double normal = Math.sqrt(rad*rad+rad*rad);
		
		double f;
		col temp;
		
		for (int i=-rad;i<rad;i++){
			for (int j=-rad;j<rad;j++){
				if ((((match.x + i) >= 0) && ((match.x + i) < xdim)) && (((match.y + j) >= 0) && ((match.y + j) < ydim))){
					
					//scaling
					f = gauss(i,j,normal);
					
					//f/=(t*4.0f+1.0f);
					
					//learning
					temp = new col((float)(grid[match.x+i][match.y+j].r*(1.0f-f)+sample.r*f), (float)(grid[match.x+i][match.y+j].g*(1.0f-f)+sample.g*f), (float)(grid[match.x+i][match.y+j].b*(1.0f-f)+sample.b*f), match.x+i,match.y+j);
					grid[match.x+i][match.y+j] = temp;
				}
			}
		}
		
	}
	public float uMap() {
	
	
	float map[][] = new float[xdim][ydim];
	float max=0.0f,offset;
	
	for (int i=0; i<xdim; i++){
		for (int j=0; j<ydim; j++) {
			col pnt;
			pnt = grid[i][j];
		
			int numinave=0;
			float total=0.0f;
		
			//Total up the distances
			for (int k=-9; k<=9; k++){
				for (int l=-9; l<=9; l++){
					if ((k + i) >=0   && (k + i) < xdim && (l + j) >=0  && (l + j) < ydim) {
						total+=distance(grid[i+k][j+l],pnt);
						numinave++;
					}
				}
			}
	
			//-1 is for the center, no cuenta
			total/=(float)(numinave-1);
			if (total>max){
				max=total;
			}
		
			//Put all the totals into a buffer for later scaling
			map[i][j]=total;
		}
	}
	
	//The colors here are scaled in order to use all the colors
	float total=0.0f;
	for (int i=0; i<xdim; i++){
		for (int j=0; j<ydim; j++) {
			offset = 255-Math.round((map[i][j] / max)*255.0f);
			umap[i][j] = new col(offset,offset,offset,i,j);
			total+=map[i][j];
		}
	}
	
	//total is now the sum of the average distances which is a value
	//  describing how good the map is at showing similarities
	return total;
	}
//=============================================================	
//		public functions
//=============================================================
	
	public void loadSamples(){
		printf("Loading sample data");
		ArrayList line;
		int r,g,b;
		
		line = readLine(stream);
		samples = 0;
		while (!line.isEmpty()){
			r = ((Double)line.remove(0)).intValue();
			g = ((Double)line.remove(0)).intValue();
			b = ((Double)line.remove(0)).intValue();
			
			col pnt = new col(r,g,b,0,0);
			
			data.put(""+samples,pnt);
			samples++;
			
			line = readLine(stream);
		}
	}
	
	public void init(){
		printf("Initializing "+xdim+" by "+ydim+" grid");
		
		grid = new col[xdim][ydim];
		umap = new col[xdim][ydim];
		int r,g,b;
		for (int i=0;i<xdim;i++){
			for (int j=0;j<ydim;j++){
				r = ran.nextInt(256);
				g = ran.nextInt(256);
				b = ran.nextInt(256);
				grid[i][j] = new col(r,g,b,i,j);
			}
		}
		iface.update(grid);
	}
	
	public void runAlgorithm(){
		printf("Running algorithm");
		
		col sample,match;
		
		while (t <= 1){
			
			sample = randomSample();
			match = bestMatch(sample);
			
			scale(match,sample,t);
						t+= 1.0f/iter;
			iface.update(grid);
			//imap.update(umap);
		}
grad = (float)Math.sqrt(uMap()/(xdim*ydim));

		imap.update(umap);
		printf("All iterations finished. Total gradient = "+grad);
	}
	
//=============================================================	
//		static functions
//=============================================================	
	public static SOM parseParameters(String[] flags) {
		//default parameter values
		
		boolean d = false;
		int x = 50,y = 50;
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
					if (flags[index].compareTo("-x") == 0) {
						index++;
						x = new Integer(flags[index]).intValue();
					}else{
						if (flags[index].compareTo("-y") == 0) {
							index++;
							y = new Integer(flags[index]).intValue();
						}
					}
				}
			}
			index++;
		}
		
		return new SOM(x,y,sample,d);
	}


	public static void main (String[] args){
		
		SOM map = parseParameters(args);
		
		if (map == null){
			System.out.println("\nUsage SOM [options]");
			System.out.println("Options:");
			System.out.println("-x <dim>\tX dimension. Default is 50");
			System.out.println("-y <dim>\tY dimension. Default is 50");
			System.out.println("-s <file name>\tSample data");
			System.out.println("-d\t\tTurn on debug output");
			System.exit(1);	
		}
		
		map.loadSamples();
		map.init();
		map.runAlgorithm();
	}
}
