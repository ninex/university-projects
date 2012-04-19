package project3;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeSet;

/**
 * An implementation of Dijkstra's shortest path algorithm. It computes the shortest path (in distance)
 * to all nodes in the map. The output of the algorithm is the shortest distance from the start node 
 * to every other node, and the shortest path from the start node to every other.
 */

public class Dijkstra
{
    /**
     * Infinity value for distances.
     */
    public static final int INFINITE_DISTANCE = Integer.MAX_VALUE;
    
    /**
     * This comparator orders nodes according to their shortest distances,
     * in ascending fashion. If two nodes have the same shortest distance,
     * we compare the nodes themselves.
     */
    private final Comparator shortestDistanceComparator = new Comparator()
        {
            public int compare(Object left, Object right)
            {
                //assert left instanceof node && right instanceof node : "invalid comparison";
                return compare((node) left, (node) right);
            }
            
            private int compare(node left, node right)
            {
                // note that this trick doesn't work for huge distances, close to Integer.MAX_VALUE
                int result = getShortestDistance(left) - getShortestDistance(right);
                
                return (result == 0) ? left.compareTo(right) : result;
            }
        };
    
    /**
     * The graph.
     */
    private final RoutesMap map;
    
    /**
     * The working set of nodes, kept ordered by shortest distance.
     */
    private final SortedSet unsettledNodes = new TreeSet(shortestDistanceComparator);
    
    /**
     * The set of nodes for which the shortest distance to the source
     * has been found.
     */
    private final Set settledNodes = new HashSet();
    
    /**
     * The currently known shortest distance for all nodes.
     */
    private final Map shortestDistances = new HashMap();

    /**
     * Predecessors list: maps a node to its predecessor in the spanning tree of
     * shortest paths.
     */
    private final Map predecessors = new HashMap();
    
    /**
     * Constructor.
     */
    public Dijkstra(RoutesMap map)
    {
        this.map = map;
    }

    /**
     * Initialize all data structures used by the algorithm.
     * 
     * @param start the source node
     */
    private void init(node start)
    {
        settledNodes.clear();
        unsettledNodes.clear();
        
        shortestDistances.clear();
        predecessors.clear();
        
        // add source
        setShortestDistance(start, 0);
        unsettledNodes.add(start);
    }
    
    /**
     * Run Dijkstra's shortest path algorithm on the map.
     * The results of the algorithm are available through
     * {@link #getPredecessor(node)}
     * and 
     * {@link #getShortestDistance(node)}
     * upon completion of this method.
     * 
     * @param start the starting node
     * @param destination the destination node. If this argument is <code>null</code>, the algorithm is
     * run on the entire graph, instead of being stopped as soon as the destination is reached.
     */
    public void execute(node start, node destination)
    {
        init(start);
        
        // the current node
        node u;
        
        // extract the node with the shortest distance
        while ((u = extractMin()) != null)
        {
            //assert !isSettled(u);
            
            // destination reached, stop
            if (u == destination) break;
            
            markSettled(u);
            
            relaxNeighbors(u);
        }
    }

    /**
     * Extract the node with the currently shortest distance, and remove it from
     * the priority queue.
     * 
     * @return the minimum node, or <code>null</code> if the queue is empty.
     */
    private node extractMin()
    {
    	if (unsettledNodes.isEmpty()) return null;
    	
        node min = (node) unsettledNodes.first();
        unsettledNodes.remove(min);
        
        return min;
    }
    
	/**
	 * Compute new shortest distance for neighboring nodes and update if a better
	 * distance is found.
	 * 
	 * @param u the node
	 */
    private void relaxNeighbors(node u)
    {
        for (Iterator i = map.getDestinations(u).iterator(); i.hasNext(); )
        {
            node v = (node) i.next();
            
            // skip node already settled
            if (isSettled(v)) continue;
            
            if (getShortestDistance(v) > getShortestDistance(u) + map.getDistance(u, v))
            {
            	// assign new shortest distance and mark unsettled
                setShortestDistance(v, getShortestDistance(u) + map.getDistance(u, v));
                                
                // assign predecessor in shortest path
                setPredecessor(v, u);
            }
        }        
    }

	/**
	 * Mark a node as settled.
	 * 
	 * @param u the node
	 */
	private void markSettled(node u)
	{
		settledNodes.add(u);    
	}

	/**
	 * Test a node.
	 * 
     * @param v the node to consider
     * 
     * @return whether the node is settled, ie. its shortest distance
     * has been found.
     */
    private boolean isSettled(node v)
    {
        return settledNodes.contains(v);
    }

    /**
     * @return the shortest distance from the source to the given node, or
     * {@link Dijkstra#INFINITE_DISTANCE} if there is no route to the destination.
     */    
    public int getShortestDistance(node anode)
    {
        Integer d = (Integer) shortestDistances.get(anode);
        return (d == null) ? INFINITE_DISTANCE : d.intValue();
    }

	/**
	 * Set the new shortest distance for the given node,
	 * and re-balance the set according to new shortest distances.
	 * 
	 * @param node the node to set
	 * @param distance new shortest distance value
	 */        
    private void setShortestDistance(node anode, int distance)
    {
        // this crucial step ensure no duplicates will be created in the queue
        // when an existing unsettled node is updated with a new shortest distance
        unsettledNodes.remove(anode);

        shortestDistances.put(anode, new Integer(distance));
        
		// re-balance the sorted set according to the new shortest distance found
		// (see the comparator the set was initialized with)
		unsettledNodes.add(anode);        
    }
    
    /**
     * @return the node leading to the given node on the shortest path, or
     * <code>null</code> if there is no route to the destination.
     */
    public node getPredecessor(int anode)
    {
        return (node) predecessors.get(node.valueOf(anode));
    }
    
    private void setPredecessor(node a, node b)
    {
        predecessors.put(a, b);
    }

}
