#include <qthread.h>

class Mpeg : public QThread {
	public:
		Mpeg:: Mpeg(QVector<ptrFrame> *vectorlist,char* inFile);
		void run();
		bool Processing();
		void load();
		
	private:
		
		bool processing,active;
		QVector<ptrFrame> *list;
		char* in;
	};
