/*
 * parse.java
 *
 * Created on 09 May 2006, 10:42
 */

package project6;

import java.io.*;
import java.util.*;
/**
 *
 * @author Abrie
 */
public class parse {
    String fileName;
    ArrayList list;
    /**
     * Creates a new instance of parse
     * @param fileName 
     */
    public parse(String fileName) {
        this.fileName = fileName;
        list = new ArrayList();
        try{
            FileReader f = new FileReader(fileName);
            Reader input = new BufferedReader(f);
            StreamTokenizer stream = new StreamTokenizer(input);
            //stream.eolIsSignificant(true);

            while (stream.nextToken() != StreamTokenizer.TT_EOF) {
		//is a word being read
            	if (stream.ttype == StreamTokenizer.TT_WORD) {
			list.add(new String(stream.sval));
                }
                //is a number being read
                if (stream.ttype == StreamTokenizer.TT_NUMBER) {
			list.add(new Double(stream.nval));
                }
                /*if (stream.ttype == StreamTokenizer.TT_EOL) {
            		list.add(new String("\n"));
            	}*/
            }
        }catch (Exception e){
            System.out.println("File Error");
            System.exit(1);
        }
    }

    public void createDFA(){
        int state,trans,state2;
        String info;
        Object temp;
        while (!list.isEmpty()){
            temp = list.remove(0);
            state = -1;
            try{
                state = ((Double)temp).intValue();
                temp = list.remove(0);
                trans = ((Double)temp).intValue();
                temp = list.remove(0);
                state2 = ((Double)temp).intValue();
                System.out.println("Transition from "+state+" to "+state2+" on "+trans);
            }catch (Exception e){
                info = (String)temp;
                if ((state != -1) && (info.compareTo("FINAL") == 0)){
                    System.out.println(state+" is a final state");
                }else{
                    if (info.compareTo("START") == 0){
                        temp = list.remove(0);
                        state = ((Double)temp).intValue();
                        System.out.println(state+" is a start state");
                    }
                }
                
            }

        }

    }
}
