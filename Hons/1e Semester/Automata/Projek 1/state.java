public class state {
	private int first,second;
	private char chr;
	
	public state(int first, int second, char chr) {
		this.first = first;
		this.second = second;
		this.chr = chr;
	}
	
	public int getFirst() {
		return first;
	}
	
	public int getSecond() {
		return second;
	}
	
	public char getChar() {
		return chr;
	}
}