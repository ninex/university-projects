import java.io.*;
import java.util.*;

public class random{

	col[] refs;
	int [] prop,noise;
	int count;
	Random ran;
	FileWriter f;
	
	public random(){
		refs = new col[6];
		prop = new int[6];
		noise = new int[6];
		ran = new Random();
		//hash = new HashMap(50000);
	}
	
	public void makeRefPoints(){
		float r,g,b;
		for (int i=0;i<6;i++){
			r = ran.nextInt(256);
			g = ran.nextInt(256);
			b = ran.nextInt(256);
			refs[i] = new col(r,g,b,0,0);
		}
	}
	
	public void proportions(){
		int lim = 50000,c=0;
		
		for (int i=0;i<5;i++){
			prop[i] = ran.nextInt(lim)+1;
			
			lim = lim - prop[i];
			prop[i]++;
			c += prop[i];
		}
		prop[5] = 50000 - c;
		
	}
	public void makeNoise(){
		int lim = 25;
		for (int i=0;i<6;i++){
			noise[i] = ran.nextInt(lim);
			noise[i]++;
		}
	}
	
	public void makePoints(){
		float r,g,b,c;
		count = 0;
		for (int i=0;i<6;i++){
			for (int j=0;j<=prop[i];j++){
				c = ran.nextInt(noise[i]);
				if (c < noise[i]){
					r = refs[i].r-c;
					if (r < 0){
						r = 0;
					}
				}else{
					r = refs[i].r+c;
					if (r > 255){
						r = 255;
					}
				}
				
				c = ran.nextInt(noise[i]);
				if (c < noise[i]){
					g = refs[i].g-c;
					if (g < 0){
						g = 0;
					}
				}else{
					g = refs[i].g+c;
					if (g > 255){
						g = 255;
					}
				}
				
				c = ran.nextInt(noise[i]);
				if (c < noise[i]){
					b = refs[i].b-c;
					if (b < 0){
						b = 0;
					}
				}else{
					b = refs[i].b+c;
					if (b > 255){
						b = 255;
					}
				}
				//hash.put(""+count,new col(r,g,b,0,0));
				try{
					f.write(""+r);
					f.write(' ');
					f.write(""+g);
					f.write(' ');
					f.write(""+b);
					f.write('\n');
				}catch (Exception e){}
				
				count++;
				
			}
		}
	}
	
	public void dumpPoints(String str){
		try{
			col temp;
			f = new FileWriter(str);
			//for (int i=0;i<count;i++){
			//	temp = (col)hash.get(""+i);
			//	
			//}
		}catch (Exception e){
			System.out.println("Error writing to file");
		}
		
	}
	
	public static void main(String[] args){
		random r = new random();
		r.dumpPoints(args[0]);
		r.makeRefPoints();
		r.proportions();
		r.makeNoise();
		r.makePoints();
		
	}
}