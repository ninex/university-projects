
public class MyEdge {
	MyVertex v1,v2;
	int weight;
	boolean path,visited,tour;
	public MyEdge(MyVertex v1, MyVertex v2){
		this.v1 = v1;
		this.v2 = v2;
		path = false;
		visited = false;
		tour = false;
		weight = new Double(Math.sqrt(Math.pow(v1.x-v2.x,2)+Math.pow(v1.y-v2.y,2))).intValue();
	}
}
