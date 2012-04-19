import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import jdsl.graph.api.*;
import jdsl.graph.ref.*;
import jdsl.graph.algo.*;



public class projek2  extends JFrame implements MouseListener, MouseMotionListener {
	JButton b_calc = new JButton("Calculate");
    JButton b_reset = new JButton("Reset");
	JTextArea txt = new JTextArea(15,35);
	JLabel pos = new JLabel("(000,000)");
	JPanel pane1 = new JPanel();
	Graph graph;
	ArrayList list = new ArrayList();
	
	public projek2(){
		super("2-approximation for the traveling salesmen problem ");
		setSize(700,600);
		
		graph = new IncidenceListGraph();
		JPanel pane2 = new JPanel();
		JPanel pane3 = new JPanel();
		JPanel pane4 = new JPanel();
		
		
		pane3.add(new JScrollPane(txt),BorderLayout.WEST);
		
		pane4.add(b_calc,null);
        pane4.add(b_reset,null);
        pane4.add(pos,null);
		
		pane2.add(pane3,BorderLayout.WEST);
		pane2.add(pane4,BorderLayout.EAST);
		
		//Container main = this.getContentPane();
		add(pane1,BorderLayout.CENTER);
		pane1.setSize(300,300);
		add(pane2,BorderLayout.SOUTH);
		b_calc.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e){
				if (!graph.isEmpty()){
					calculate();	
				}
			}
		});
                b_reset.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e){
                            pane1.repaint(0);
                            graph = new IncidenceListGraph();
			}
		});
		pane1.addMouseListener(this);
		pane1.addMouseMotionListener(this);
	}
	
	public void calculate(){
		txt.setText("");
		Prim prim = new Prim();
		MyEdge e;
		EdgeIterator ei = graph.edges();
		while (ei.hasNext()){
			e = (MyEdge)(ei.nextEdge().element());
			e.path =false;
			e.tour = false;
			e.visited = false;
		}
		Vertex v = graph.aVertex();
		prim.executeAll(graph,v);
		list.clear();
		list.add(v);
		tour(v);
		
		drawTree();
	}
	
	public void tour(Vertex parent){
		EdgeIterator ei = graph.incidentEdges(parent);
		MyEdge me;
		Edge e = null;
		/*boolean test = false;
		Vertex iter = null;*/
		while (ei.hasNext()){
			e = ei.nextEdge();
			me = (MyEdge)e.element();
			if (!me.visited && me.path){
				me.visited = true;
				me.tour = true;
				list.add(graph.opposite(parent,e));
				tour(graph.opposite(parent,e));
			}
		}
	}

	
	public void drawTree(){
		Graphics g = pane1.getGraphics();
		EdgeIterator ei = graph.edges();
		MyVertex v1,v2;
		
		while (ei.hasNext()){
			//Edge e = ei.nextEdge();
			MyEdge e = (MyEdge)ei.nextEdge().element();
			if (e.path){
				g.setColor(Color.red);
			}else{
				g.setColor(Color.gray);
			}
			g.drawLine(e.v1.x,e.v1.y,e.v2.x,e.v2.y);
						
		}
		VertexIterator vi = graph.vertices();
		MyVertex v;
		g.setColor(Color.pink);
		while (vi.hasNext()){
			v = (MyVertex)vi.nextVertex().element();
			g.fillOval(v.x-5,v.y-5,10,10);	
		}
		
		if (!list.isEmpty()){
			g.setColor(Color.blue);
			MyVertex first = (MyVertex)((Vertex)list.remove(0)).element();
			v1 = first;
			//System.out.println(first);
			while (!list.isEmpty()){
			
				v2 = (MyVertex)((Vertex)list.remove(0)).element();
				g.drawLine(v1.x,v1.y,v2.x,v2.y);
				txt.append("Node:"+v1.x+","+v1.y+" to Node:"+v2.x+","+v2.y+"\n");
				v1 = v2;
			}
			g.drawLine(first.x,first.y,v1.x,v1.y);
		}
	}
	
	
	public void createEdges(int x,int y){
		VertexIterator vi = graph.vertices();
		Graphics g = pane1.getGraphics();
		MyVertex myv = new MyVertex(x,y);
		Vertex newv = graph.insertVertex(myv);
		Vertex oldv;
		while (vi.hasNext()){
			oldv = vi.nextVertex();
			MyVertex v = (MyVertex)(oldv.element());
			g.drawLine(x,y,v.x,v.y);
			graph.insertEdge(newv,oldv,new MyEdge(myv,v));
		}
		
		
	}
	
	public void mouseClicked(MouseEvent e){
		Graphics g = pane1.getGraphics();
		g.setColor(Color.pink);
		g.fillOval(e.getX()-5,e.getY()-5,10,10);
		
		createEdges(e.getX(),e.getY());
	}
	public void mouseReleased(MouseEvent e){}
	public void mouseEntered(MouseEvent e){}
	public void mouseExited(MouseEvent e){}
	public void mousePressed(MouseEvent e){}
	
	public void mouseDragged(MouseEvent e){}  
    public void mouseMoved(MouseEvent e){
    	String x = ""+e.getX(),y = ""+e.getY();
    	while (x.length() < 3){
    		x = "0"+x;
    	}
    	while (y.length() < 3){
    		y = "0"+y;
    	}
    	pos.setText("("+x+","+y+")");
    	
    }  

	public class Prim extends IntegerPrimTemplate{

		protected int weight(Edge arg0) {
			
			return ((MyEdge)arg0.element()).weight;
		}
		protected void treeEdgeFound(Vertex v, Edge vparent, int treeWeight){
			if (vparent != null){
				((MyEdge)vparent.element()).path = true;
			}
		}
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		projek2 gui = new projek2();
		gui.setVisible(true);
	}

}
