import javax.swing.*;
public class FraqKnapsack{
	
	Item[] items;
	int W;
	
	public FraqKnapsack(Item[] items,int W){
		this.items = items;
		this.W = W;
	}
	
	public void sort(){
		int chose = 0;;
		double high,a,w=0;
		while (w < W){
			//remove Item with highest value
			high = 0;
			for (int i=0;i<items.length;i++){
				if ((items[i].value() > high) && (items[i].weight != 0)){
					high = items[i].value();
					chose = i;
				}
			}
			a = Math.min(items[chose].weight,W-w);
			items[chose].x = a;
			items[chose].weight-=a;
			w = w+a;
		}
	}
	
	public void results(JTextArea text){
		text.setText("");
		for (int i=0;i<items.length;i++){
			text.append("Item "+i+" took "+items[i].x+"\n");
		}
	}
	
	public static void fill(Item[] items){
		items[0] = new Item(4,12);
		items[1] = new Item(8,32);
		items[2] = new Item(2,40);
		items[3] = new Item(6,30);
		items[4] = new Item(1,50);
	}
	
	/*public static void main(String[] args){
		Item[] items = new Item[5];
		int W = 10;
		fill(items);
		FraqKnapsack sack = new FraqKnapsack(items,W);
		sack.sort();
		sack.results();
	}*/
}