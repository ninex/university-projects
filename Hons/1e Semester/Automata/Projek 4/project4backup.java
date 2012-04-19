import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferStrategy;
import java.util.*;
import javax.swing.*;

public class project4 implements MouseListener, KeyListener{
	
	private static final double INITIAL = 20;
	private static final double THRESHOLD = 21.0;
	private static final double ALPHA = 0.3;
	
	Frame mainFrame;
	BufferStrategy bufferStrategy;
	Rectangle bounds;
	GraphicsDevice device;
	
	int rows,cols,rowInc,colInc,kx,ky,rx,ry,ix,iy;
	int pbx,pby,pex,pey,limit;
	boolean done,mouseIn = false,pressed = false,running = false,havePlate = false,haveClay = false;
	int bx,by,dim;
	double[][] clay;

	public project4(GraphicsDevice device,int dim) {
		this.dim = dim;
		this.device = device;
		clay = new double[dim][dim];
		//initiate the fullscreen graphics
		try {
			GraphicsConfiguration gc = device.getDefaultConfiguration();
			mainFrame = new Frame(gc);
			mainFrame.addMouseListener(this);
			mainFrame.addKeyListener(this);
			mainFrame.setUndecorated(true);
			mainFrame.setIgnoreRepaint(false);
			device.setFullScreenWindow(mainFrame);

			bounds = mainFrame.getBounds();
			mainFrame.createBufferStrategy(2);
			bufferStrategy = mainFrame.getBufferStrategy();
			
			rowInc = bounds.height / dim;
			colInc = bounds.width / dim;
			
			System.out.println("Width = "+bounds.width+" Height = "+bounds.height);
		
		} catch (Exception e) {
			System.out.println("Problem entering fullscreen mode");
			System.exit(1);
		}
	}

	//keyboard events
	public void keyPressed(KeyEvent e){
		if (e.getKeyCode() == KeyEvent.VK_SPACE) {
			pressed = true;
		}
		if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
			device.setFullScreenWindow(null);
			System.exit(0);
		}
		if (e.getKeyCode() == KeyEvent.VK_ENTER) {
			if (havePlate && haveClay){
				running = true;
			}else{
				JOptionPane.showMessageDialog(null,"Clay and Plate areas must be defined before running system");
			}
		}
	}
	public void keyReleased(KeyEvent e){
		pressed = false;
	}
	public void keyTyped(KeyEvent e){}
	


	//mouse events
	public void mouseReleased(MouseEvent e){
		if (e.getButton() == MouseEvent.BUTTON1){
			if (pressed) {
				int ex = e.getX();
				int ey = e.getY();
				
				/*for (int i=kx;i<ex;i=i+colInc) {
					for (int j=ky;j<ey;j=j+rowInc){
						clay[Math.round(i/colInc)][Math.round(j/rowInc)] = Double.MAX_VALUE;
					}
				}*/
				pex = Math.round((ex+5) / colInc);
				pey = Math.round((ey-1) / rowInc);
				pbx = Math.round(kx / colInc);
				pby = Math.round(ky / rowInc);
				havePlate = true;
			}else{
				int ex = e.getX();
				int ey = e.getY();
				
				for (int i=bx;i<ex;i=i+colInc) {
					for (int j=by;j<ey;j=j+rowInc){
						clay[Math.round(i/colInc)][Math.round(j/rowInc)] = INITIAL;
					}
				}
				haveClay = true;
				limit = Math.round(ey/rowInc) + 1;
			}
			mouseIn = false;
		}
	}
	public void mouseExited(MouseEvent e){}
	public void mouseEntered(MouseEvent e){}
	public void mousePressed(MouseEvent e){
		if (!running){
			if ((e.getButton() == MouseEvent.BUTTON1) && (!pressed)) {
				for (int i=0;i<dim;i++) {
					for (int j=0;j<dim;j++){
						clay[i][j] = 0;
					}
				}
				bx = e.getX();
				by = e.getY();
				mouseIn = true;
			}else{
				if ((e.getButton() == MouseEvent.BUTTON1) && (pressed)) {
					kx = e.getX();
					ky = e.getY();
					mouseIn = true;
				}
			}
		}
	}
	public void mouseClicked(MouseEvent e){
		/*if (e.getButton() == MouseEvent.BUTTON3) {
			device.setFullScreenWindow(null);
			System.exit(0);
		}else{
			if (e.getButton() == MouseEvent.BUTTON2) {
				JOptionPane.showInputDialog(null,"What is the vertical dimension of the model?","Dimensions",JOptionPane.QUESTION_MESSAGE);
			}
			//device.setFullScreenWindow(null);
			//System.exit(0);
		}*/
	}
	
	public void select(Graphics g){
		int x = MouseInfo.getPointerInfo().getLocation().x;
		int y = MouseInfo.getPointerInfo().getLocation().y;
			
		if (pressed) {
			g.setColor(Color.red);
			g.fillRect(kx,ky,x-kx,y-ky);
		}else{
			g.setColor(Color.black);
			g.fillRect(bx,by,x-bx,y-by);
		}
	}
	
	public boolean mouseIn() {
		return mouseIn;
	}
	public boolean running(){
		return running;
	}
	
	public void drawGrid(Graphics g) {
		g.setColor(Color.black);
		for (int i=0;i<bounds.width;i=i+colInc) {
			g.drawLine(i,0,i,bounds.height);
		}
		for (int i=0;i<bounds.height;i=i+rowInc) {
			g.drawLine(0,i,bounds.width,i);
		}
	}
	
	
	
	public void drawSystem(Graphics g){
		//g.setColor(Color.black);
		for (int i=0;i<dim;i++) {
			for (int j=0;j<dim;j++) {
				if (clay[i][j] > 0.0){
				
					try{
						g.setColor(new Color(0.0f,(new Float(clay[i][j]/(2*THRESHOLD))).floatValue(),0.0f));
					}catch (Exception e){}
				}else{
					g.setColor(Color.white);
				}
				
				g.fillRect(i*colInc,j*rowInc,i+colInc,j+rowInc);
			}
		}
		g.setColor(Color.red);
		g.fillRect(pbx*colInc,pby*rowInc,pex*colInc - pbx*colInc,pey*rowInc - pby*rowInc);
	}
	
	public boolean isObstacle(int x, int y){
		return ((x >= pbx) && (y >= pby) && (x <= pex) && (y <= pey));
	}
	
	public void Margolus(boolean odd,int x,int y){
		
		if ((clay[x][y] > THRESHOLD) && (!isObstacle(x,y))) {
		
			if (odd){
				
				
			
				
					if ((clay[x-1][y] <= THRESHOLD) && (clay[x][y-1] <= THRESHOLD) && (clay[x-1][y-1] <= THRESHOLD)){
						double dm1 = clay[x][y] * ALPHA;
						clay[x][y] = clay[x][y] - dm1;
						
						if ((isObstacle(x-1,y)) && (isObstacle(x,y-1)) && (isObstacle(x-1,y-1)) ){
						}else{
							clay[x][y] = clay[x][y] - dm1;
							
							if ((!isObstacle(x-1,y)) && (isObstacle(x,y-1)) && (isObstacle(x-1,y-1)) ){
								clay[x-1][y] = clay[x-1][y] + dm1;
							}else{
								if ((!isObstacle(x-1,y)) && (!isObstacle(x,y-1)) && (isObstacle(x-1,y-1)) ){
									clay[x-1][y] = clay[x-1][y] + dm1/2;
									clay[x][y-1] = clay[x][y-1] + dm1/2;
								}else{
									if ((isObstacle(x-1,y)) && (!isObstacle(x,y-1)) && (isObstacle(x-1,y-1)) ){
										clay[x][y-1] = clay[x][y-1] + dm1;
									}else{
										if ((isObstacle(x-1,y)) && (!isObstacle(x,y-1)) && (!isObstacle(x-1,y-1)) ){
											clay[x][y-1] = clay[x][y-1] + dm1/2;
											clay[x-1][y-1] = clay[x-1][y-1] + dm1/2;
										}else{
											if ((isObstacle(x-1,y)) && (isObstacle(x,y-1)) && (!isObstacle(x-1,y-1)) ){
												clay[x-1][y-1] = clay[x-1][y-1] + dm1;
											}else{
												clay[x-1][y] = clay[x-1][y] + dm1/3;
												clay[x][y-1] = clay[x][y-1] + dm1/3;
												clay[x-1][y-1] = clay[x-1][y-1] + dm1/3;
											}
										}
									}
								}
							}
						}
						
						
					}else{
						if ((clay[x-1][y] > THRESHOLD) && (clay[x][y-1] <= THRESHOLD) && (clay[x-1][y-1] <= THRESHOLD)){
							double dm1 = clay[x][y] * ALPHA;
							double dm2 = clay[x-1][y] * ALPHA;
							
							
							
							if ((isObstacle(x,y-1)) && (isObstacle(x-1,y-1)) ){
							}else{
								clay[x][y] = clay[x][y] - dm1;
								clay[x-1][y] = clay[x-1][y] - dm2;
								
								if ((!isObstacle(x,y-1)) && (isObstacle(x-1,y-1)) ){
									clay[x][y] = clay[x][y] - dm1;
									
									clay[x][y-1] = clay[x][y-1] + (dm1);
									
								}else{
									if ((!isObstacle(x,y-1)) && (!isObstacle(x-1,y-1)) ){
										clay[x][y] = clay[x][y] - dm1;
										clay[x-1][y] = clay[x-1][y] - dm2;
										
										clay[x][y-1] = clay[x][y-1] + (dm1+dm2)/2;
										clay[x-1][y-1] = clay[x-1][y-1] + (dm1+dm2)/2;
									}else{
										if ((isObstacle(x,y-1)) && (!isObstacle(x-1,y-1)) ){
											clay[x-1][y] = clay[x-1][y] - dm2;
											
											clay[x-1][y-1] = clay[x-1][y-1] + (dm2);
										}
									}
								}
							}
						}else{
							if ((clay[x-1][y] <= THRESHOLD) && (clay[x][y-1] <= THRESHOLD) && (clay[x-1][y-1] > THRESHOLD)){
								double dm1 = clay[x][y] * ALPHA;
								double dm2 = clay[x-1][y-1] * ALPHA;
								
								if ((isObstacle(x-1,y)) && (isObstacle(x,y-1))){
								}else{
									clay[x][y] = clay[x][y] - dm1;
									clay[x-1][y-1] = clay[x-1][y-1] - dm2;
									
									if ((!isObstacle(x-1,y)) && (isObstacle(x,y-1))){
										clay[x-1][y] = clay[x-1][y] + (dm1+dm2);
									}else{
										if ((isObstacle(x-1,y)) && (!isObstacle(x,y-1)) ){
											clay[x][y-1] = clay[x][y-1] + (dm1+dm2);
										}else{
											clay[x-1][y] = clay[x-1][y] + (dm1+dm2)/2;
											clay[x][y-1] = clay[x][y-1] + (dm1+dm2)/2;
										}
									}
								}
								
							}else{
								if ((clay[x-1][y] > THRESHOLD) && (clay[x][y-1] <= THRESHOLD) && (clay[x-1][y-1] > THRESHOLD)){
									double dm1 = clay[x][y] * ALPHA;
									double dm2 = clay[x-1][y] * ALPHA;
									double dm3 = clay[x-1][y-1] * ALPHA;
									
									if ((isObstacle(x-1,y)) && (isObstacle(x,y-1)) && (isObstacle(x-1,y-1)) ){
									}else{
										clay[x][y] = clay[x][y] - dm1;
										if ((!isObstacle(x-1,y)) && (isObstacle(x,y-1)) && (isObstacle(x-1,y-1)) ){
										}else{
											if ((!isObstacle(x-1,y)) && (!isObstacle(x,y-1)) && (isObstacle(x-1,y-1)) ){
											}else{
												if ((isObstacle(x-1,y)) && (!isObstacle(x,y-1)) && (isObstacle(x-1,y-1)) ){
												}else{
													if ((isObstacle(x-1,y)) && (!isObstacle(x,y-1)) && (!isObstacle(x-1,y-1)) ){
													}else{
														if ((isObstacle(x-1,y)) && (isObstacle(x,y-1)) && (!isObstacle(x-1,y-1)) ){
														}else{
														}
													}
												}
											}
										}
									}
									
									clay[x][y] = clay[x][y] - dm1;
									clay[x-1][y] = clay[x-1][y] - dm2;
									clay[x-1][y-1] = clay[x-1][y-1] - dm3;
									
									clay[x-1][y] = clay[x-1][y] + (dm1+dm2+dm3);
								}
							}
						}
					}
				
			
			}else{
				if ((isObstacle(x,y)) || (isObstacle(x+1,y)) || (isObstacle(x,y+1)) || (isObstacle(x+1,y+1)) ){
				}else{
					if ((clay[x+1][y] <= THRESHOLD) && (clay[x][y+1] <= THRESHOLD) && (clay[x+1][y+1] <= THRESHOLD)){
						double dm1 = clay[x][y] * ALPHA;
						clay[x][y] = clay[x][y] - dm1;
						
						clay[x+1][y] = clay[x+1][y] + dm1/3;
						clay[x][y+1] = clay[x][y+1] + dm1/3;
						clay[x+1][y+1] = clay[x+1][y+1] + dm1/3;
					}else{
						if ((clay[x+1][y] > THRESHOLD) && (clay[x][y+1] <= THRESHOLD) && (clay[x+1][y+1] <= THRESHOLD)){
							double dm1 = clay[x][y] * ALPHA;
							double dm2 = clay[x+1][y] * ALPHA;
							clay[x][y] = clay[x][y] - dm1;
							clay[x+1][y] = clay[x+1][y] - dm2;
							
							clay[x][y+1] = clay[x][y+1] + (dm1+dm2)/2;
							clay[x+1][y+1] = clay[x+1][y+1] + (dm1+dm2)/2;
						}else{
							if ((clay[x+1][y] <= THRESHOLD) && (clay[x][y+1] <= THRESHOLD) && (clay[x+1][y+1] > THRESHOLD)){
								double dm1 = clay[x][y] * ALPHA;
								double dm2 = clay[x+1][y+1] * ALPHA;
								clay[x][y] = clay[x][y] - dm1;
								clay[x+1][y+1] = clay[x+1][y+1] - dm2;
								
								clay[x+1][y] = clay[x+1][y] + (dm1+dm2)/2;
								clay[x][y+1] = clay[x][y+1] + (dm1+dm2)/2;
							}else{
								if ((clay[x+1][y] > THRESHOLD) && (clay[x][y+1] <= THRESHOLD) && (clay[x+1][y+1] > THRESHOLD)){
									double dm1 = clay[x][y] * ALPHA;
									double dm2 = clay[x+1][y] * ALPHA;
									double dm3 = clay[x+1][y+1] * ALPHA;
									clay[x][y] = clay[x][y] - dm1;
									clay[x+1][y] = clay[x+1][y] - dm2;
									clay[x+1][y+1] = clay[x+1][y+1] - dm3;
									
									clay[x+1][y] = clay[x+1][y] + (dm1+dm2+dm3);
								}
							}
						}
					}
				}
			}
		}
	}
		
		
	
	
	
	public void runTheSystem(){
		boolean odd,notDone = false;
		double[][] old = new double[dim][dim];
		System.out.println("pb "+pbx+","+pby+" pe "+pex+","+pey);
		if (pby < limit){
			pby++;
			pey++;
			System.out.println("nr.2 pb "+pbx+","+pby+" pe "+pex+","+pey);
			for (int i=pbx;i<pex;i++){
				if (clay[i][pey] > 0){
					clay[i][pey+1] = clay[i][pey+1] + clay[i][pey];
					clay[i][pey] = 0;
				}
			}
		}
		
		
		for (int row=0;row<dim;row++){
			for (int col=0;col<dim;col++){
				if (clay[col][row] > THRESHOLD){
					notDone = true;
				}
			}
		}
		
		while (notDone){
			odd = false;
			notDone = false;
			for (int row=0;row<limit;row++){
				for (int col=0;col<dim-1;col++){
					Margolus(odd,col,row);
				}
			}
			odd = true;
			for (int row=1;row<limit;row++){
				for (int col=1;col<dim;col++){
					Margolus(odd,col,row);
				}
			}
			for (int row=0;row<limit;row++){
				for (int col=0;col<dim;col++){
					if (clay[col][row] > THRESHOLD){
						notDone = true;
					}
				}
			}
			
			
		}
	}
	
	public BufferStrategy getStrategy() {
		return bufferStrategy;
	}
	
	public static void main (String[] args){
	
		
		//info for the user
		
//JOptionPane.showMessageDialog(null,"To exit left-click on the screen.\nTo reset the robot's position right-click on the screen","System is running.",JOptionPane.INFORMATION_MESSAGE);
		String temp;
		boolean input = true;
		int dim = 0;
		while (input){
			temp = JOptionPane.showInputDialog(null,"What is the vertical dimension of the model?","Dimensions",JOptionPane.QUESTION_MESSAGE);
			try{
				dim = Integer.parseInt(temp);
				input = false;
			}catch (Exception e){}
		}
		
		GraphicsEnvironment env = GraphicsEnvironment.getLocalGraphicsEnvironment();
		GraphicsDevice device = env.getDefaultScreenDevice();
		Graphics g;
		
		project4 gui = new project4(device,dim);
		while (true) {
		
		g = gui.getStrategy().getDrawGraphics();
		
		gui.drawSystem(g);
		
		if (gui.mouseIn()){
			gui.select(g);
		}else{
		}
		
		gui.drawGrid(g);
		
		if (gui.running()){
			gui.runTheSystem();
		}
		
		gui.getStrategy().show();
		g.dispose();
		try {
			Thread.sleep(10);
		}catch(Exception e){}
		}
	}
}