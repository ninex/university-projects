import javax.swing.*;
public class Knapsack{
	
	Item[] items;
	int W;
	
	public Knapsack(Item[] items,int W){
		this.items = items;
		this.W = W;
	}
	
	public void sort(JTextArea text){
		int[] B = new int[W+1];
		for (int i=0;i<B.length;i++){
			B[i] = 0;
		}
		for (int i=0;i<items.length;i++){
			for (int w=W;w>=items[i].weight;w--){
				
				if (B[w-items[i].weight]+items[i].benefit > B[w]){
					B[w] = B[w-items[i].weight]+items[i].benefit;
				}
			}
		}
		text.setText("Total benefit:"+B[W]);
	}
	
	
	
	/*public static void main(String[] args){
		Item[] items = new Item[5];
		int W = 10;
		fill(items);
		Knapsack sack = new Knapsack(items,W);
		sack.sort();
	}*/
}