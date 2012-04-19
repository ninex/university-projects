//#include <qwidget.h>
#include <qvariant.h>
#include "global.h"
//#define TRUE 1
//#define FALSE 0
//class QPushButton;

typedef struct point{
	float x,y;
	int r[5][5],g[5][5],b[5][5];
}point;


typedef struct hand{
	point *one, *two,*three,*four,*five;
}hand;

class MyWidget : public QWidget{
	Q_OBJECT
	public:
		MyWidget();
		void init(int argc, char * argv[]);
		void MyWidget:: Isolate(QImage pic,ptrFrame* ptr);
		void MyWidget:: skinData(QImage mask,QImage pic);
		//void bestTrack(point *pnt, ptrFrame *ptr);
		void allocBody();
		
		QPushButton *b_play;
		QPushButton *b_hot;
		
	private:
		void loadPic(QImage pic,ptrFrame* ptr);
		void isolateHead(QImage mask,QImage pic);
		void isolateElbows(QImage mask,QImage pic);
		void isolateHands(QImage mask,QImage pic);
		void clusters(QImage mask, QImage pic, int setpixels);
	
	protected slots:
		void play();
		void hotspots();
	
	};

