import java.awt.image.*;
import java.awt.*;
import javax.imageio.*;
import java.io.*;
import java.util.*;
public class writeImage{
	String tree,jpeg;
	ArrayList list;
	int depth = 0;
	node root;
	double[] spacing,counts;
	double max;
	
	public writeImage(String tree,String jpeg){
		this.tree = tree;
		this.jpeg = jpeg;
	}
	
	public void loadTree(){
		System.out.println("Loading tree");
		StreamTokenizer stream = null;
		try{
			FileInputStream f = new FileInputStream(tree);
			Reader input = new BufferedReader(new InputStreamReader(f));
			stream = new StreamTokenizer(input);
			stream.resetSyntax();
			stream.wordChars(32,127);
		}catch (Exception e){
			System.out.println("Error opening "+tree);
			System.exit(1);
		}
		list = new ArrayList();
		try{
			//read the file to the end
			while (stream.nextToken() != StreamTokenizer.TT_EOF){
				//is a word being read
				if (stream.ttype == StreamTokenizer.TT_WORD) {
					list.add(new String(stream.sval));
				}
				//is a number being read
				if (stream.ttype == StreamTokenizer.TT_NUMBER) {
					list.add(new Double(stream.nval));
				}
			}
		}catch (Exception e){
			System.out.println("\nError reading "+tree+". Exiting...");
			System.exit(1);
		}
	}
	
	public void recreateTree(){
		System.out.println("Recreating tree");
		ArrayList nodes = new ArrayList();
		String attr,crit,result;
		int level;
		node leaf;
		while (!list.isEmpty()){
			if (((String)list.remove(0)).compareTo("Node") == 0){
				leaf = new node();
				attr = (String)list.remove(0);
				//System.out.println("ATTR:"+attr);
				crit = (String)list.remove(0);
				try{
					level = (new Double(crit)).intValue();
					crit = null;
				}catch (Exception e){
					//System.out.println("crit:"+crit);
					level = (new Double((String)list.remove(0))).intValue();
				}
				//System.out.println("lvl:"+level);
				if (level > depth){
					depth = level;
				}
				if (((String)list.get(0)).compareTo("Node") != 0){
					result = (String)list.remove(0);
					leaf.setResult(result);
				}
				leaf.setSplitCriteria(crit,0);
				leaf.setLevel(level);
				leaf.setAttr(attr);
				
				if (attr.compareTo("_root") == 0){
					root = leaf;
				}else{
					
					nodes.add(leaf);
				}
				
			}
		}
		depth++;
		levelCount(nodes);
		
		System.out.println("Linking "+nodes.size()+" tree nodes");
		trav(nodes,root);
	}
	
	public void trav(ArrayList nodes,node cur){
		node temp;
		while (!nodes.isEmpty()){
			temp = (node)nodes.get(0);
			if (temp.getLevel()-1 == cur.getLevel()){
				nodes.remove(0);
				cur.addChild(temp);
				trav(nodes,temp);
			}else{
				return;
			}
		}
	}
	
	public void levelCount(ArrayList nodes){
		System.out.println("Doing level counts");
		spacing = new double[depth];
		for (int i=0;i<depth;i++){
			spacing[i] = 0;
		}
		spacing[0]++;
		node leaf;
		for (int i=0;i<nodes.size();i++){
			leaf = (node)nodes.get(i);
			spacing[leaf.getLevel()]++;
		}
		max = 0;
		for (int i=0;i<depth;i++){
			if (spacing[i] > max){
				max = spacing[i];
			}
			//System.out.println("Level "+i+" has "+spacing[i]+" nodes");
		}
	}
	
	public void draw(node leaf,Graphics2D g,int px,int py){
		int lvl = leaf.getLevel();
		double l = lvl;
		counts[lvl]++;
		
		double xfraq = counts[lvl] / (spacing[lvl]+1);
		double yfraq = l / depth;
		int x = new Double(1600*xfraq).intValue();
		int y = new Double(1200*yfraq).intValue()+10;
		
		
		if (leaf.getAttr() != null){
			g.drawString(leaf.getAttr(),x-20,y);
		}
		if (leaf.getCrit() != null){
			g.drawString(leaf.getCrit(),x-20,y+10);
		}
		if (leaf.getResult() != null){
			g.drawString(leaf.getResult(),x-20,y+10);
		}
		g.drawLine(x,y,px,py);
		//g.fillRect(x,y,20,20);
		ArrayList children = leaf.getChildren();
		while (!children.isEmpty()){
			draw((node)children.remove(0),g,x,y);
		}
	}
	
	public void write(){
		System.out.println("Writing image");
		BufferedImage img = new BufferedImage(1600,1200,BufferedImage.TYPE_INT_RGB);
		Graphics2D g = img.createGraphics();
		
		g.setColor(Color.white);
		g.fillRect(0,0,1600,1200);
		g.setColor(Color.black);
		counts = new double[depth];
		for (int i=0;i<depth;i++){
			counts[i] = 0;
		}
		draw(root,g,800,10);
		
		
		try{
			File f = new File(jpeg);
			ImageIO.write((RenderedImage)img,"jpeg",f);
		}catch (Exception e){
			System.out.println("Error writing image data");
		}
	}
	
	public static writeImage parseParams(String[] flags){
		if (flags.length != 2){
			System.out.println("\nUsage writeImage <tree file> <jpeg file>\n");
			return null;
		}
		return new writeImage(flags[0],flags[1]);
	}
	
	public static void main(String[] args){
		writeImage job = parseParams(args);
		if (job != null){
			job.loadTree();
			job.recreateTree();
			job.write();
		}
	}
}