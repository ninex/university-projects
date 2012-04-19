import javax.swing.*;
import java.util.*;
import java.io.*;

public class project5_2{
	int seed, cycle;
	double limit;
	boolean[] ca;
	String bitDump = "bit.txt", intDump = "int.txt";
	FileWriter f1,f2;
	
	public project5_2(int seed, int cycle, double limit){
		this.seed = seed;
		this.cycle = cycle;
		this.limit = limit;
		ca = new boolean[cycle];
		
		try{
			f1 = new FileWriter(bitDump);
			f2 = new FileWriter(intDump);
		}catch (Exception e){}
		
		
		
		int temp = seed;
		for (int i=0;i<cycle;i++){
			if ((temp % 2) == 1){
				ca[i] = true;
			}else{
				ca[i] = false;
			}
			temp = (temp - (temp%2)) / 2;
		}
	}
	
	public void generate(){
		System.out.println("Generating "+limit+" numbers with seed "+seed+" and life cycle "+cycle);
		double val;
		for (double i=0;i<limit;i++){
			calculate();
			val = bit2int(ca);
			dump(val);
			//System.out.println("Current: "+val+" in binary: "+bitRepr(val));
			//System.out.println( (i/limit*100)+"% complete");
		}
		try{
			f1.close();
			f2.close();
		}catch (Exception e){}
	}
	
	boolean[] copy(){
		boolean[] cpy = new boolean[cycle];
		for (int i=0;i<cycle;i++){
			cpy[i] = ca[i];
		}
		return cpy;
	}
	
	void calculate(){
		boolean[] old = copy();
		for (int i=0;i<cycle;i++){
			ca[i] = function(i,old);
		}
	}
	
	boolean function(int index, boolean[] old){
		boolean ret;
		
		//ai(t+1) = ai-1 XOR (ai OR ai+1)
		if (index == 0){
			ret = old[cycle-1] ^ (old[index] | old[index+1]);
		}else{
			if (index == (cycle-1)){
				ret = old[index-1] ^ (old[index] | old[0]);
			}else{
				ret = old[index-1] ^ (old[index] | old[index+1]);
			}
		}
		return ret;
	}
	
	double bit2int(boolean[] set){
		double count = 0;
		for (int i=0;i<cycle;i++){
			if (set[i]){
				count = count + Math.pow(2,i);
			}
		}
		return count;
	}
	
	String bitRepr(double val){
		double temp = val;
		String str = "";
		for (int i=0;i<cycle;i++){
			if ((temp % 2) == 1){
				str = "1" + str;
			}else{
				str = "0" + str;
			}
			temp = (temp - (temp%2)) / 2;
		}
		return str;
	}
	
	void dump(double val){
		try{
			//dump the bits
			f1.write(bitRepr(val)+"\n");
			//dump the doubles
			f2.write(Double.toString(val)+"\n");
		}catch (Exception e){}
	}
	
//==============================================================================================	
	
	public static void main(String[] args){
		String temp;
		boolean input = true;
		int seed = 0,cycle = 0;
		double limit = 0;
		//kry die seed van die user af
		while (input){
			temp = JOptionPane.showInputDialog(null,"What is the seed for the generator?","Seed",JOptionPane.QUESTION_MESSAGE);
			try{
				seed = Integer.parseInt(temp);
				//die seed moet 'n positiewe getal wees
				if (seed >= 0) {
					input = false;
				}
			}catch (Exception e){}
		}
		input = true;
		while (input){
			temp = JOptionPane.showInputDialog(null,"What is the length of a cycle?","Cycle",JOptionPane.QUESTION_MESSAGE);
			try{
				cycle = Integer.parseInt(temp);
				//die seed moet 'n positiewe getal wees
				if (cycle >= 0) {
					input = false;
				}
			}catch (Exception e){}
		}
		input = true;
		while (input){
			temp = JOptionPane.showInputDialog(null,"How many numbers must be generated?","Limit",JOptionPane.QUESTION_MESSAGE);
			try{
				limit = Double.parseDouble(temp);
				//die seed moet 'n positiewe getal wees
				if (limit > 0) {
					input = false;
				}
			}catch (Exception e){}
		}
		project5_2 main = new project5_2(seed,cycle,limit);
		main.generate();
	}
}