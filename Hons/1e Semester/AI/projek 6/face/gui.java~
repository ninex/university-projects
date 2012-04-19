import javax.swing.*;
import java.awt.*;
public class gui extends JFrame{
	double x,y;
	public gui(int x,int y){
		super("SOM");
		this.x = x;
		this.y = y;
		setSize(400,400);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public void update(col[][] grid){
		Graphics g = getGraphics();
		int rx = new Double(400/x).intValue();
		int ry = new Double(400/y).intValue();
		for (int i=0;i<x;i++){
			for (int j=0;j<y;j++){
				g.setColor(new Color((int)grid[i][j].r,(int)grid[i][j].g,(int)grid[i][j].b));
				g.fillRect(new Double(i/x*400).intValue(),new Double(j/y*400).intValue(),rx,ry);
			}
		}
		try{
			//Thread.sleep(10);
		}catch (Exception e){}
	}
}