public class Item{
	int weight, benefit;
	double x;
	public Item(int weight,int benefit){
		this.weight = weight;
		this.benefit = benefit;
		x = 0;
	}
	double value(){
		return ((double)benefit)/weight;
	}
	
}