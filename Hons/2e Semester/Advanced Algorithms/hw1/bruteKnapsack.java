import javax.swing.*;
public class bruteKnapsack{
	
	Item[] items;
	int W;
	
	public bruteKnapsack(Item[] items,int W){
		this.items = items;
		this.W = W;
	}
	
	public void sort(){
		int w = 0,max = 0,a,mindex = 0;
		while (w < W){
			max = 0;
			for (int i=0;i<items.length;i++){
				if ((items[i].x == 0) && (items[i].benefit > max)){
					max = items[i].benefit;
					mindex = i;
				}
			}
			a = Math.min(items[mindex].weight,W-w);
			if (a == items[mindex].weight){
				items[mindex].x = 1;
			}
			w+= a;
			
		}
		
	}
	public void results(JTextArea text){
		int total=0;
		text.setText("");
		for (int i=0;i<items.length;i++){
			text.append("Item "+i+" took "+items[i].x+"\n");
			if (items[i].x > 0){
				total+=items[i].benefit;
			}
		}
		text.append("Total benefit:"+total+"\n");
	}
	
	public static void fill(Item[] items){
		items[0] = new Item(4,20);
		items[1] = new Item(2,3);
		items[2] = new Item(2,6);
		items[3] = new Item(6,25);
		items[4] = new Item(2,80);
	}
	
	/*public static void main(String[] args){
		Item[] items = new Item[5];
		int W = 9;
		fill(items);
		bruteKnapsack sack = new bruteKnapsack(items,W);
		sack.sort();
		sack.results();
	}*/
}