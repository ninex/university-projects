package project3;
import java.util.ArrayList;
import java.util.List;
import java.io.*;

/**
 * This map stores routes in a matrix, a nxn array. 
 */

class DenseRoutesMap
	implements RoutesMap ,Serializable
{
	private final int[][] distances;
	private int numnodes;
	DenseRoutesMap(int numnodes)
	{
		this.numnodes = numnodes;
		distances = new int[numnodes][numnodes];
	}
	
	public int getNodes()
	{
		return numnodes;
	}
	
	/**
	 * Link two ndoes by a direct route with the given distance.
	 */
	public void addDirectRoute(int start, int end, int distance)
	{
		distances[start][end] = distance;
	}
	
	/**
	 * Link two nodes by a undirect route with the given distance.
	 */
	public void addUndirectRoute(int start, int end, int distance)
	{
		distances[start][end] = distance;
		distances[end][start] = distance;
		System.out.println("UNDIRECT ROUTE "+start+" and "+end);
	}
	
	/**
	 * @return the distance between the two nodes, or 0 if no path exists.
	 */
	public int getDistance(node start, node end)
	{
		return distances[start.getIndex()][end.getIndex()];
	}
	
	public int getDistance(int start, int end)
	{
		return distances[start][end];
	}
	
	/**
	 * @return the list of all valid destinations from the given node.
	 */
	public List getDestinations(node anode)
	{
		List list = new ArrayList();
		
		for (int i = 0; i < distances.length; i++)
		{
			if (distances[anode.getIndex()][i] > 0)
			{
				list.add( node.valueOf(i) );
			}
		}
		
		return list;
	}

	/**
	 * @return the list of all nodes leading to the given city.
	 */
	public List getPredecessors(node anode)
	{
		List list = new ArrayList();
		
		for (int i = 0; i < distances.length; i++)
		{
			if (distances[i][anode.getIndex()] > 0)
			{
				list.add( node.valueOf(i) );
			}
		}
		
		return list;
	}
	
	/**
	 * @return the transposed graph of this graph, as a new RoutesMap instance.
	 */
	public RoutesMap getInverse()
	{
		DenseRoutesMap transposed = new DenseRoutesMap(distances.length);
		
		for (int i = 0; i < distances.length; i++)
		{
			for (int j = 0; j < distances.length; j++)
			{
				transposed.distances[i][j] = distances[j][i];
			}
		}
		
		return transposed;
	}
}
