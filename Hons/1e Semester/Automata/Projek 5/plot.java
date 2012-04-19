import java.io.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferStrategy;
import javax.swing.*;

public class plot implements KeyListener{
	
	ArrayList count,list;
	double maxfreq = -1,maxnum = 0;
	boolean dotplot = true;
	long initial;
	Frame mainFrame;
	BufferStrategy bufferStrategy;
	Rectangle bounds;
	GraphicsDevice device;
	
	public plot(GraphicsDevice device){
		
		
		this.device = device;
		
		//initiate the fullscreen graphics
		try {
			GraphicsConfiguration gc = device.getDefaultConfiguration();
			mainFrame = new Frame(gc);
			mainFrame.addKeyListener(this);
			mainFrame.setUndecorated(true);
			mainFrame.setIgnoreRepaint(false);
			
		} catch (Exception e) {
			System.out.println("Problem entering fullscreen mode");
			System.exit(1);
		}
	}
	//put the screen in fullscreen
	public void fullScreen(){
		device.setFullScreenWindow(mainFrame);

		bounds = mainFrame.getBounds();
		mainFrame.createBufferStrategy(2);
		bufferStrategy = mainFrame.getBufferStrategy();
		System.out.println("Screen resolution: "+bounds.width+"X"+bounds.height);
	}
	//when we need to quit
	public void keyPressed(KeyEvent e){
		
		//the user wants to quit and pressed escape
		if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
			device.setFullScreenWindow(null);
			System.exit(0);
		}
	}
	public void keyReleased(KeyEvent e){}
	public void keyTyped(KeyEvent e){}
	
	//lees al die nommers in die file in 'n lys
	public void parse(){
	
		list = new ArrayList();
		count = new ArrayList();
		String filename = "int.txt";
		int index;
		
		try{
			FileInputStream f = new FileInputStream(filename);
			Reader input = new BufferedReader(new InputStreamReader(f));
			StreamTokenizer stream = new StreamTokenizer(input);
			
			double counter;
			//read the file to the end
			while (stream.nextToken() != StreamTokenizer.TT_EOF) {
				
				//is a number being read
				if (stream.ttype == StreamTokenizer.TT_NUMBER) {
					maxnum = Math.max(stream.nval,maxnum);
					//dotplot hoef net die getal te kry
					if (dotplot){
						list.add(new Double(stream.nval));
					}else{
						//as 'n frequency plot gedoen word moet daar ook getel word
						if (list.contains(new Double(stream.nval))){
						
							index = list.indexOf(new Double(stream.nval));
							counter = ((Double)count.get(index)).doubleValue();
							counter++;
							count.set(index,new Double(counter));
							maxfreq = Math.max(counter,maxfreq);
							
						}else{
							list.add(new Double(stream.nval));
							count.add(new Double(1));
							maxfreq = Math.max(1,maxfreq);
						}
					}
					
				}
			}
			//close the file
			f.close();
		}catch (Exception e) {e.printStackTrace();}
		initial = list.size();
		
	}
	
	//teken alles op die skerm
	public void draw(){
		Graphics g = bufferStrategy.getDrawGraphics();
		
		g.setColor(Color.black);
		//vertical
		g.drawLine(10,10,10,bounds.height-10);
		g.drawString("Frequency",10,10);
		g.drawString(""+maxfreq,20,20);
		//horizontal
		g.drawLine(10,bounds.height-10,bounds.width-10,bounds.height-10);
		g.drawString("Number",bounds.width/2,bounds.height);
		g.drawString(""+maxnum,bounds.width-80,bounds.height);
		//System.out.println(""+((double)(initial-list.size())/(double)initial)*100+"% done");
		
		
		g.setColor(Color.red);
		double num,counter;
		int x,y;
		
		if (dotplot){
			if (!list.isEmpty()){
				g.setColor(Color.white);
				g.fillRect(0,0,bounds.width,10);
				g.setColor(Color.black);
				g.drawString(""+((double)(initial-list.size())/(double)initial)*100+"% done",bounds.width/2,10);
				
				num = ((Double)list.remove(0)).doubleValue();
				counter = ((Double)list.remove(0)).doubleValue();
				x = (new Double((double)(num/maxnum * (bounds.width-20)+10))).intValue();
				y = bounds.height - (new Double((double)((counter/maxnum * (bounds.height-20))))).intValue()-10;
				//System.out.println("Plotting "+num+" against "+counter+" at "+x+","+y);
				g.fillRect(x,y,(new Double(bounds.width/maxnum)).intValue()+1,(new Double(bounds.height/maxnum)).intValue()+1);
			}
		}else{
			while (!list.isEmpty()){
				num = ((Double)list.remove(0)).doubleValue();
				counter = ((Double)count.remove(0)).doubleValue();
				x = (new Double((double)(num/maxnum * (bounds.width-20)+10))).intValue();
				y = bounds.height - (new Double((double)((counter/maxfreq * (bounds.height-20))))).intValue()-10;
				System.out.println("Plotting "+num+" with freq "+counter+" at "+x+","+y);
				g.fillRect(x-5,y-5,5,5);
			}
		}
		
		bufferStrategy.show();
		g.dispose();
	}
	//tel watter getalle is nooit gegeneereer nie
	public void count(int bits){
		int max = (new Double(Math.pow(2,bits))).intValue();
		boolean[] counter = new boolean[max];
		int num;
		
		//clear the array
		for (int i=0; i<max;i++){
			counter[i] = false;
		}
		for (int i=0;i<list.size();i++){
			counter[((Double)list.get(i)).intValue()] = true;
		}
		int overall = 0;
		System.out.println("Following numbers were not generated in the range 0 to "+(max-1));
		for (int i=0; i<max;i++){
			if (!counter[i]){
				System.out.println(""+i);
				overall++;
			}
		}
		System.out.println("Amount of numbers not generated: "+overall+". That is "+(overall*100/max)+"% of the numbers");
	}
	//laat weet of ons nie dotplot soek nie
	public void setFreq(){
		dotplot = false;
	}
	
	public static void main(String[]args){
		String temp;
		int bits = 0;
		boolean input = true;
		//laat weet die program hoeveel bits was die getalle
		while (input){
			temp = JOptionPane.showInputDialog(null,"How many bits is the input numbers","Bits",JOptionPane.QUESTION_MESSAGE);
			try{
				bits = Integer.parseInt(temp);
				
				if (bits > 0) {
					input = false;
				}
			}catch (Exception e){}
		}
		
		
		
		GraphicsEnvironment env = GraphicsEnvironment.getLocalGraphicsEnvironment();
		GraphicsDevice device = env.getDefaultScreenDevice();
		
		plot graph = new plot(device);
		
		if ((args.length == 1) && (args[0].compareTo("-f") == 0)){
			graph.setFreq();
		}
		
		
		System.out.println("Parsing");
		
		graph.parse();
		
		System.out.println("Counting");
		
		graph.count(bits);
		
		graph.fullScreen();
		System.out.println("Drawing");
		while (true){
			graph.draw();
			try{
				Thread.sleep(1);
			}catch (Exception e){}
		}
		
	}
}