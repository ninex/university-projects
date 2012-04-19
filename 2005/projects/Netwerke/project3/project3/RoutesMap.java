package project3;
import java.util.List;

/**
 * This interface defines the object storing the graph of all routes in the
 * system.
 * 
 * @author Renaud Waldura &lt;renaud+tw@waldura.com&gt;
 * @version $Id: RoutesMap.java,v 1.1 2002/11/16 20:37:51 renaud Exp $
 */

public interface RoutesMap
{
	/**
	 * Enter a new segment in the graph.
	 */
	public void addDirectRoute(int start, int end, int distance);
	
	/**
	 * Enter a new segment in the graph.
	 */
	public void addUndirectRoute(int start, int end, int distance);
	
	/**
	 * Get the value of a segment.
	 */
	public int getDistance(node start, node end);
	
	/**
	 * Get the list of nodes that can be reached from the given node.
	 */
	public List getDestinations(node anode); 
	
	/**
	 * Get the list of nodes that lead to the given node.
	 */
	public List getPredecessors(node anode);
	
	/**
	 * @return the transposed graph of this graph, as a new RoutesMap instance.
	 */
	public RoutesMap getInverse();
}