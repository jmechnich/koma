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

#ifndef MAINWIDGET_HH
#define MAINWIDGET_HH

#include <qmainwindow.h>
#include <qapplication.h>
#include <q3table.h>
#include <qpixmap.h>
#include <q3popupmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <q3hbox.h>
#include <qdatetime.h>
#include <qmessagebox.h>
#include <qfiledialog.h>

#include <iostream>

/* XPM */
static const char * p1_xpm[] = {
"16 16 3 1",
"       c None",
".      c #000000000000",
"X      c #FFFFFFFFFFFF",
"                ",
"   ......       ",
"   .XXX.X.      ",
"   .XXX.XX.     ",
"   .XXX.XXX.    ",
"   .XXX.....    ",
"   .XXXXXXX.    ",
"   .XXXXXXX.    ",
"   .XXXXXXX.    ",
"   .XXXXXXX.    ",
"   .XXXXXXX.    ",
"   .XXXXXXX.    ",
"   .XXXXXXX.    ",
"   .........    ",
"                ",
"                "};

/* XPM */
static const char * p2_xpm[] = {
"16 16 3 1",
"       c None",
".      c #000000000000",
"X      c #FFFFFFFF0000",
"                ",
"                ",
"         ....   ",
"        .XXXX.  ",
" .............. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .............. ",
"                "};

/* XPM */
static const char * p3_xpm[] = {
"16 16 3 1",
"       c None",
".      c #000000000000",
"X      c #FFFFFFFFFFFF",
"                ",
"                ",
"   .........    ",
"  ...........   ",
"  ........ ..   ",
"  ...........   ",
"  ...........   ",
"  ...........   ",
"  ...........   ",
"  ...XXXXX...   ",
"  ...XXXXX...   ",
"  ...XXXXX...   ",
"  ...XXXXX...   ",
"   .........    ",
"                ",
"                "};

class MainWidget : public QMainWindow
{
  Q_OBJECT

public:
  MainWidget()
          :QMainWindow(), _table( 0)
        {
          init();
        }

  enum Columns
  {
      Date=0,
      Purpose=1,
      Amount=2
  };
  
private:
  void init()
        {
          resize( 800, 600);

          QPixmap p1( p1_xpm);
          QPixmap p2( p2_xpm);
          QPixmap p3( p3_xpm);

          Q3PopupMenu *file = new Q3PopupMenu( this);
          file->insertItem( p1, tr( "&New"),  this, SLOT( news()), Qt::CTRL+Qt::Key_N);
          file->insertItem( p2, tr( "&Open"), this, SLOT( open()), Qt::CTRL+Qt::Key_O);
          file->insertItem( p3, tr( "&Save"), this, SLOT( save()), Qt::CTRL+Qt::Key_S);
          file->insertSeparator();
          file->insertItem( tr( "E&xit"), this, SLOT( exit()), Qt::CTRL+Qt::Key_X);
          
          menuBar()->insertItem( tr( "&File"), file);
          
          _sbFilename = new QLabel( "no file", statusBar());
          Q3HBox* sumLayout = new Q3HBox( statusBar());
          sumLayout->move( 300, 0);
          _sbSumLbl = new QLabel( tr( "Sum:"), sumLayout);
          _sbSum = new QLabel( tr( "0"), sumLayout);
          sumLayout->move( 300, 0);
          
          statusBar()->addWidget( _sbFilename);
          statusBar()->addWidget( sumLayout);
        }

private slots:
  void news()
        {
          destroyTable();
                    
          _table = new Q3Table( 3, 3, this);
          connect( _table, SIGNAL( valueChanged(int,int)),
                   this, SLOT( handleTableChange(int, int)));
          connect( _table, SIGNAL( doubleClicked(int,int,int,const QPoint&)),
                   this, SLOT( handleDoubleClick(int, int,int,const QPoint&)));
          connect( _table, SIGNAL( contextMenuRequested(int,int,const QPoint&)),
                   this, SLOT( handleContextMenuRequest(int, int,const QPoint&)));
          _table->horizontalHeader()->setLabel( Date, tr( "Date"));
          _table->horizontalHeader()->setLabel( Purpose, tr( "Purpose"));
          _table->horizontalHeader()->setLabel( Amount, tr( "Amount"));
          _table->show();
          setCentralWidget( _table);
        }
  
  void open()
        {
          destroyTable();
          std::cout << __PRETTY_FUNCTION__ << std::endl;
          QString filename = QFileDialog::getOpenFileName( _filename, "*.kom",
                                                           this,
                                                           "FileOpenDialog",
                                                           tr("Open file ..."));
          std::cout << "Selected file '" << filename.toStdString() << "'" << std::endl;
        }
  
  void save()
        {
          if( _table)
          {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            QString filename = QFileDialog::getSaveFileName( _filename, "*.kom",
                                                             this,
                                                             "FileSaveDialog",
                                                             tr("Save file ..."));
            std::cout << "Selected file '" << filename.toStdString() << "'" << std::endl;
          }
        }
  
  void exit()
        {
          if( destroyTable() != 1)
          {
            qApp->quit();
          }
        }
  
  int destroyTable()
        {
          int ret = 0;
          
          if( _table)
          {
            ret = QMessageBox::warning( this, tr(" -- Konto Manager"),
                                        tr("A table is still open."
                                           " Save first ?"),
                                        QMessageBox::Yes,
                                        QMessageBox::No,
                                        QMessageBox::Cancel);
            switch( ret)
            {
            case( QMessageBox::Yes):
              save();
              delete _table;
              _table = 0;
              return 0;
              break;
            case( QMessageBox::No):
              delete _table;
              _table = 0;
              return 0;
              break;
            case( QMessageBox::Cancel):
              return 1;
              break;
            default:
              break;
            }
          }
          
          return ret;
        }
  
  void handleTableChange( int row, int col)
        {
          if( row == _table->numRows()-1)
              _table->setNumRows( _table->numRows()+1);

          if( col == Amount)
          {
            updateStatusBar();
          }
          
          _table->adjustColumn( col);
        }
  
  void handleContextMenuRequest( int row, int col, const QPoint& pos)
        {
          (void) row;
          std::cout << __PRETTY_FUNCTION__ << std::endl;
          switch( col)
          {
          case( Date):
          {
            Q3PopupMenu* datePopup = new Q3PopupMenu( _table);
            datePopup->insertItem( QDate::currentDate().toString( Qt::ISODate));
            datePopup->popup( pos);
            break;
          }
          case( Purpose):
            break;
          case( Amount):
            break;
          default:
            break;
          }
        }
  
  void handleDoubleClick( int row, int col, int button, const QPoint& pos)
        {
          (void) row;
          (void) button;
          (void) pos;
          std::cout << __PRETTY_FUNCTION__ << std::endl;
          switch( col)
          {
          case( Date):
            break;
          case( Purpose):
            break;
          case( Amount):
            break;
          default:
            break;
          }
        }
  
  void updateStatusBar()
        {
          double sum = 0;
          bool ok = true;
          for( int i=0; i < _table->numRows(); ++i)
          {
            if( _table->text( i, Amount) != "")
            {
              sum += _table->text( i, Amount).toDouble( &ok);
              if( !ok)
              {
                QMessageBox::information( this,
                                          tr( "Error parsing table"),
                                          tr( "Error parsing amount in line ") +
                                          QString::number( i+1));
                _table->selectRow( i);
                return;
              }
            }
          }
          
          
          _sbSum->setText( QString::number( sum));
        }
  

private:
  Q3Table* _table;
  QLabel* _sbFilename;
  QLabel* _sbSumLbl;
  QLabel* _sbSum;
  QString _filename;
};

#endif
