/*
 * Main.java
 *
 * Created on 09 May 2006, 09:23
 */

package project6;


/**
 *
 * @author Abrie
 */
public class Main {
    
    /** Creates a new instance of Main */
    public Main() {
    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
            
            /*if (args.length != 1){
                System.exit(1);
            }
            parse parser = new parse(args[0]);
            parser.createDFA();*/
            Main_Frame gui = new Main_Frame();
            gui.show();
            
            
            
    }
    
}
