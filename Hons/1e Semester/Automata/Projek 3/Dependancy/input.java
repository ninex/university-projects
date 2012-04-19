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
			
			//end of line characters will also be read as tokens
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
		boolean isRow = true;
		Integer temp;
		
		//get all the tokens
		ArrayList templist = run(filename);
		Object [] list = templist.toArray();
		
		//traverse the list to find the size of the CA
		for (int i=0; i < list.length;i++) {
			try {
				temp = new Integer((String)list[i]);
				if (isRow && (temp.intValue() > rmax)) {
					rmax = temp.intValue();
				}else {
					if (!isRow && (temp.intValue() > cmax)) {
						cmax = temp.intValue();
					}
				}
				isRow = !isRow;
			}catch (Exception e){}
		}
		rmax++;
		cmax++;
		System.out.println("CA is "+rmax+" by "+cmax);
		state[][] ca = new state[rmax][cmax];
		
		String read;
		int index = 0,row,col,drow,dcol;
		
		//parse all the tokens
		while (index < list.length) {
			read = (String)list[index];
			//do we read the initial states
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
				//ignore the boundary setting
				if (read.compareTo("BOUNDARY") == 0) {
					index = index + 2;
				}else {
					//read al the dependancies
					try{
						row = (new Integer((String)list[index]).intValue());
						index++;
						col = (new Integer((String)list[index]).intValue());
						index++;
						if (ca[row][col] == null) {
							ca[row][col] = new state();
						}
						if (((String)list[index]).compareTo("DEPENDS") != 0) {
							System.out.println("Expected DEPENDS");
							System.exit(1);
						}
						index++;
						while (((String)list[index]).compareTo("\n") != 0) {
							try {
								drow = (new Integer((String)list[index]).intValue());
								index++;
								dcol = (new Integer((String)list[index]).intValue());
								index++;
								System.out.println("Setting dependancy "+drow+","+dcol+" for "+row+","+col);
								if (ca[drow][dcol] == null) {
									ca[drow][dcol] = new state();
								}
								ca[row][col].add(ca[drow][dcol]);
								
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
		}
		return ca;
	}
}