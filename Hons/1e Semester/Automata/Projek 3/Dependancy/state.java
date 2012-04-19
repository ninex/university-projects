import java.util.*;

public class state {
	ArrayList depend;
	BitSet st;
	public state() {
		depend = new ArrayList();
		st = new BitSet(1);
		st.set(0);
	}
	public void add(state dep) {
		depend.add(dep);
	}
	public BitSet get() {
		return st;
	}
	public void set(BitSet st) {
		this.st = st;
	}
	public void initial() {
		st.set(0);
	}
	//compute the dependancy function
	public void run() {
		int index = depend.size() - 1;
		while (index >= 0) {
			st.xor( ((state)depend.get(index)).get());
			index--;
		}
	}
}