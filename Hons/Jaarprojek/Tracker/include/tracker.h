
class Tracker {
	public:
	
		Tracker(QVector<ptrFrame> *vector_list);
		void trackFrame(QImage pic,int frame,double edge,double skin,QString type);
		
		void setAllStatsEdgeValues(double stats_ur,double stats_ug,double stats_ub,double stats_sr,double stats_sg,double stats_sb);
		void setAllStatsSkinValues(double stats_s_ur,double stats_s_ug,double stats_s_ub,double stats_s_sr,double stats_s_sg,double stats_s_sb);
		void setAllHeadValues(int stats_hl,int stats_hr,int stats_ht,int stats_hb);
		void setAllBoxValues(int stats_ml,int stats_mr,int stats_ml2,int stats_mh,int width, int height);
		void notifyArms();
		int cx,cy;
		
	private:
		void motiontrack(QImage pic);
		void track(QImage pic);
		
		void calculateStats();
		void searchLeftEstimates(QImage pic,int lex,int ley);
		void searchRightEstimates(QImage pic,int rex,int rey);
		bool searchLeftHandEstimates(QImage pic,int ex,int ey,int x,int y,int x2, int y2);
		bool searchRightHandEstimates(QImage pic,int ex,int ey,int x,int y,int x2, int y2);
		bool searchLeftParams(int x,int y);
		bool searchRightParams(int x,int y);
		void searchLeftElbowEstimates(QImage pic,int lex,int ley);
		void searchRightElbowEstimates(QImage pic,int rex,int rey);
		void elbowMotiontrack(QImage pic,QImage mask,bool skipLeft,bool skipRight,double a,double c,double ra,double rc);
		
		void findIt(QImage pic,double c,double rc,bool findLeft, bool findRight);
		void allocBody();
		
		//width height, movement area
		int w,h,ml,mr,ml2,mh;
		//head coordinates
		int hl,hr,ht,hb;
		//intial elbow coordinates
		int lex,ley,rex,rey;
		//initial hand coordinates
		int lhx,lhy,rhx,rhy;
		
		double minArmLeft,minArmRight,maxArmLeft,maxArmRight;
		
		double ur,ug,ub,sr,sg,sb;
		
		double s_ur,s_ug,s_ub,s_sr,s_sg,s_sb;
		double edge_t,skin_t;
		
		//frame
		int framecount,type;
		
		bool hybrid;
		
		
		QVector<ptrFrame> *list;
		ptrFrame *ptr,*prev;
		

	};
