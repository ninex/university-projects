import java.util.*;
import java.io.*;
public class input {
	
	public static ArrayList run(String filename) {
		//create a list that will store everything that's read from the input file
		ArrayList list = new ArrayList();
		try {
			//setup the file to be read token by token
			FileInputStream f = new FileInputStream(filename);
			Reader input = new BufferedReader(new InputStreamReader(f));
			StreamTokenizer stream = new StreamTokenizer(input);
			
			stream.eolIsSignificant(true);
			
			//read the file to the end
			while (stream.nextToken() != StreamTokenizer.TT_EOF) {
				//is a word being read
				if (stream.sval != null) {
					list.add(new String(stream.sval));
				}
				//is a number being read
				if (stream.ttype == StreamTokenizer.TT_NUMBER) {
					list.add(new String(""+ (new Double(stream.nval) ).intValue()));
				}
				if (stream.ttype == StreamTokenizer.TT_EOL) {
					list.add(new String("\n"));
				}
			}
			//close the file
			f.close();
		}catch (Exception e) {
			//any possible errors that may have occured
			e.printStackTrace();
			System.out.println("Error reading input file. Exiting...");
			System.exit(1);
		}
		//return the list of strings/numbers read
		return list;
	}
	
	public static state[][] parse(String filename) {
		int rmax = 0,cmax = 0;
		int index = 0,row,col,drow,dcol;
		boolean isRow = true;
		Integer temp;
		
		//find out the size of the CA
		ArrayList templist = run(filename);
		index = templist.indexOf(new String("SIZE"));
		if (index != -1) {
			templist.remove(index);
			rmax = Integer.parseInt((String)templist.remove(index));
			cmax = Integer.parseInt((String)templist.remove(index));
			templist.remove(index);
		}else {
			System.out.println("CA size must be specified");
			System.exit(1);
		}
		
		Object [] list = templist.toArray();
		
		System.out.println("CA is "+rmax+" by "+cmax);
		state[][] ca = new state[rmax][cmax];
		
		String read;
		index = 0;
		//traverse the tokens
		while (index < list.length) {
			read = (String)list[index];
			
			//find the initial state
			if (read.compareTo("INITIAL") == 0) {
				while (((String)list[index]).compareTo("\n") != 0) {
					index++;
					try {
						row = (new Integer((String)list[index]).intValue());
						index++;
						col = (new Integer((String)list[index]).intValue());
						index++;
						System.out.println("Setting initial "+row+","+col);
						if (ca[row][col] == null) {
							ca[row][col] = new state();
						}
						ca[row][col].initial();
						
					}catch (Exception e) {
						System.out.println("Invalid definition");
						System.exit(1);
					}
				}
			}else {
				//get the goal state
				if (read.compareTo("GOAL") == 0) {
					row = Integer.parseInt((String)list[index+1]);
					col = Integer.parseInt((String)list[index+2]);
					if (ca[row][col] == null) {
						ca[row][col] = new state();
					}
					ca[row][col].goal();
					System.out.println("Setting goal "+row+","+col);
					index = index + 3;
				}else {
					//get the walls
					try{
						if (read.compareTo("WALL") != 0) {
							System.out.println("Expected WALL");
							System.exit(1);
						}
						index++;
						while (((String)list[index]).compareTo("\n") != 0) {
							try {
								row = (new Integer((String)list[index]).intValue());
								index++;
								col = (new Integer((String)list[index]).intValue());
								index++;
								System.out.println("Setting wall "+row+","+col);
								if (ca[row][col] == null) {
									ca[row][col] = new state();
								}
								ca[row][col].wall();
								
							}catch (Exception e) {
								e.printStackTrace();
								System.out.println("Invalid definition");
								System.exit(1);
							}
						}
						
					}catch (Exception e) {
						System.out.println("Invalid syntax");
						System.exit(1);
					}
				}
			}
			index++;
			while ((index < list.length) && (((String)list[index]).compareTo("\n") == 0)){
				index++;
			}
		}
		return ca;
	}
}