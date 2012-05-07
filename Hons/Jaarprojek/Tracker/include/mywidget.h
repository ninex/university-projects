
#include <qvariant.h>
#include <qslider.h>
#include <qlabel.h>
#include <qcombobox.h>
#include "global.h"

//initial threshold values

#define THRES 0.3
#define SKINTHRES 0.5

/*
//structure used to store data on the hands and elbows
typedef struct point{
	int x,y;
	int vx,vy;
}point;


//a linear y = mx +c equation used for the arms
typedef struct equation{
	float m,c,length;
}equation;
*/

//the main class
class MyWidget : public QWidget{
	Q_OBJECT
	public:
		MyWidget();
		void init();
		
		
		QPushButton *b_play;
		QPushButton *b_save;
		QPushButton *b_open;
		QSlider *s_skin,*s_thres,*s_frame;
		QLabel *l_skin,*l_thres,*status;
		QComboBox *l_algo;
		
	private:
		void loadFirst();
		void loadPic(QImage pic,int frame);
		void hotspots();
		//void clusters(QImage mask, QImage pic, int setpixels);
		
		bool playing;
		unsigned int frameIndex;
		
		

	protected slots:
		void paintEvent(QPaintEvent *);
		void play();
		void open_file();
		void save();
		void skinthresChanged();
		void thresChanged();
		void algorithmChange();
	};

