

#include <qapplication.h>


#include "include/mywidget.h"
#include "include/interface.h"
#include "include/mpeg2dec.h"

//the main calling function which initializes the widget


int main( int argc, char * argv[] ){
	QApplication a( argc, argv );
 	
	MyWidget widget;
	widget.show();


	widget.init();
	
	return a.exec();

}

