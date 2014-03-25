#include <qdebug.h>
#include <QApplication>
#include "talk/base/ssladapter.h"
//#include <QApplication>

#include "ActivityManager.h"

//#include "talk/base/thread.h"

int main(int argc, char *argv[])
{
	////////////////////////////////
	///Init
    QApplication a(argc,argv);
    talk_base::InitializeSSL();
    ActivityManager *am;
    talk_base::LogMessage::LogToDebug(talk_base::LS_ERROR);
    ///
    ////////////////////////////////

   	am = new ActivityManager();
    am->Start();
    QCoreApplication::exec();


    talk_base::CleanupSSL();
    return EXIT_SUCCESS;
}
