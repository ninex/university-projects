package project3;
import java.util.ArrayList;

/**
 * This class models a route.
 */

public final class Route
	implements Cloneable
{
	// we need its concrete type to successfully clone this field
	private ArrayList nodes = new ArrayList();
	
	private int distance = 0;
	
	/**
	 * Instances of this class are created by the {@link RouteBuilder}.
	 */	
	Route()
	{
	}
	
	public Object clone()
	{
		Route newInstance = null;
		
		try
		{
			 newInstance = (Route) super.clone();	
		}
		catch (CloneNotSupportedException cnfe)
		{
			// we are Cloneable: this should never happen
			//assert false : cnfe;
		}
		
		newInstance.nodes = (ArrayList) nodes.clone();
		
		return newInstance;
	}
	
	/**
	 * Add a new stop to this route with the given distance.
	 * If this is the first stop (i.e. the starting point), the
	 * <code>distance</code> argument is meaningless.
	 * 
	 * @param stop the next ndoe on this route.
	 * @param distance the distance between the previous node and this one.
	 */
	void addStop(node stop, int distance)
	{
		if (!nodes.isEmpty())
		{
			this.distance += distance;		
		}
		
		cities.add(stop);
	}
	
	/**
	 * @return the total distance of this route.
	 */
	public int getDistance()
	{
		return distance;	
	}
	
	/**
	 * @return the number of stops on this route. The starting node is not
	 * considered a stop and thus is not counted.
	 */
	public int getLength()
	{
		return (nodes.isEmpty()) ? 0 : cities.size() - 1;
	}
	
	/**
	 * @return the last stop on this route. The last stop may be the
	 * starting point if there are no other stops, or NULL is this route
	 * has no stops.
	 */
	public node getLastStop()
	{
		if (nodes.isEmpty())
		{
			return null;
		}
		else
		{
			return (node) nodes.get(nodes.size() - 1);
		}
	}
	
	/**
	 * @return whether this route goes through the given node.
	 */
	public boolean hasCity(node anode)
	{
		return nodes.contains(anode);
	}
	
	public String toString()
	{
		StringBuffer temp = new StringBuffer();
		
		temp.append("l=").append( getLength() )
			.append(" d=").append( getDistance() )
			.append(" ").append(nodes);
			
		return temp.toString();
	}
}