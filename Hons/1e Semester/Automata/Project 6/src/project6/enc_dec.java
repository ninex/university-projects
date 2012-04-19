/*
 * enc_dec.java
 *
 * Created on 10 May 2006, 01:53
 */

package project6;
import java.util.*;
/**
 *
 * @author Abrie
 */
public class enc_dec {
    private static final int block = 8;
    BitSet bits;
    /** Creates a new instance of enc_dec */
    public enc_dec() {
        
        bits = new BitSet(block);
    }
    
    public int encode(int text){
        return rule1(text);
    }


    public int decode(int text){
        return inv_rule1(text);
    }


    private int rule1(int text){
        //System.out.println("before:"+text);
        for (int i=0;i<block;i++){
            if ((text % 2) == 1){
		bits.set(block-1-i);
            }else{
                bits.clear(block-1-i);
            }
            text = (text - (text%2)) / 2;
	}
        //System.out.println(bits.toString()+","+bit2int(bits));
        BitSet copy = bits.get(0,block);
        for (int i=0;i<block-1;i++){
            bits.set(i,!(copy.get(i) ^ copy.get(i+1)));
        }
        bits.set(block-1,!copy.get(block-1));
        return bit2int(bits);
    }

    public int inv_rule1(int text){
        //System.out.println("before:"+text);
        for (int i=0;i<block;i++){
            if ((text % 2) == 1){
		bits.set(block-1-i);
            }else{
                bits.clear(block-1-i);
            }
            text = (text - (text%2)) / 2;
	}
        //System.out.println(bits.toString()+","+bit2int(bits));
        BitSet copy = bits.get(0,block);
        //System.out.println("copy:"+copy.toString());
        bits.set(block-1,!copy.get(block-1));
        for (int i=block-2;i>=0;i--){
            bits.set(i,(!copy.get(i)) ^ bits.get(i+1));
        }
        //System.out.println("after:"+bits.toString()+","+bit2int(bits));
        return bit2int(bits);
    }

    int bit2int(BitSet set){
	double count = 0;
	for (int i=0;i<block;i++){
		if (set.get(i)){
			count = count + Math.pow(2,block-1-i);
		}
	}
        
	return new Double(count).intValue();
    }
}
