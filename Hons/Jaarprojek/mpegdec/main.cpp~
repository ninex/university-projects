#include <qapplication.h>
#include <qimage.h>
#include <qpainter.h>
#include <qpixmap.h>
#include "interface.h"



int main( int argc, char ** argv ){
	QApplication a( argc, argv );
	Form1 w;
	QImage pic;
	QPainter p1;
	QPixmap map;
	w.show();
	
	
	
	pic.load("1.jpg");
	//pic.scale(w.width(),w.height());
	
	p1.begin(&w);
	map.convertFromImage(pic,0);
	//map.scale(w.width(),w.height());
	//w.setFixedSize(pic.width(),pic.height());
	p1.drawPixmap(0,0,pic);
	p1.drawLine(12,12,120,120);
	p1.end();
	int i,j,r,g,b,al;
	QRgb rgba;
	
	for (i=0;i<pic.width();i++){
		for (j=0;j<pic.height();j++){
			rgba = pic.pixel(i,j);
			r = qRed(rgba);
			g = qGreen(rgba);
			b = qBlue(rgba);
			al = qAlpha(rgba);
			//printf("Pixel %d,%d Red %d, green %d, blue %d, alpha %d\n",i,j,r,g,b,al);
		}
	}
	
	a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
	return a.exec();
}
