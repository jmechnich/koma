/**************************************************************************
**       Title: 
**    $RCSfile: $
**   $Revision: $$Name:  $
**       $Date: $
**   Copyright: GPL $Author: $
** Description:
**
**    
**
**-------------------------------------------------------------------------
**
**  $Log: $
**
**
**************************************************************************/

#include <qapplication.h>
#include "MainWidget.hh"

int main( int argc, char** argv)
{
  QApplication app( argc, argv);
  MainWidget wid;
  wid.show();
  app.setMainWidget( &wid);
  
  return app.exec();
}
