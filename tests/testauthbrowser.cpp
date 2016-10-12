/***************************************************************************
     testauthbrowser.cpp
     ----------------------
    Date                 : October 2016
    Copyright            : (C) 2016 by Boundless Spatial, Inc. USA
    Author               : Larry Shaffer
    Email                : lshaffer at boundlessgeo dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QtTest/QtTest>
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QTemporaryFile>

#include "testutils.h"
#include <qapplication.h>
#include "qgsapplication.h"
#include "qgsauthmanager.h"

#include <stdio.h>
#include <stdlib.h>


inline QTextStream& qStdout()
{
  static QTextStream r( stdout );
  return r;
}

void suppressDebugHandler( QtMsgType type, const char *msg )
{
  switch ( type )
  {
    case QtDebugMsg:
      break;
    case QtWarningMsg:
      fprintf( stderr, "Warning: %s\n", msg );
      break;
    case QtCriticalMsg:
      fprintf( stderr, "Critical: %s\n", msg );
      break;
    case QtFatalMsg:
      fprintf( stderr, "Fatal: %s\n", msg );
      abort();
  }
}

/** \ingroup UnitTests
 * Unit tests for QGIS Auth Test Browser
 */
class TestAuthBrowser: public QObject
{
    Q_OBJECT

  private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testAuthBrowser();

  private:
    static QString smHashes;
//    static QObject *smParentObj;
    QtMsgHandler mOrigMsgHandler;
};

QString TestAuthBrowser::smHashes = "#####################";
//QObject *TestAuthBrowser::smParentObj = new QObject();

void TestAuthBrowser::initTestCase()
{
  setPrefixEnviron();
  mOrigMsgHandler = qInstallMsgHandler( suppressDebugHandler );

  QgsApplication::init();
  QgsApplication::initQgis();
  if ( QgsAuthManager::instance()->isDisabled() )
    QSKIP( "Auth system is disabled, skipping test case", SkipAll );

  qInstallMsgHandler( mOrigMsgHandler );

//  qDebug() << QgsApplication::showSettings();
}

void TestAuthBrowser::cleanupTestCase()
{
  qDebug() << "\n";
  qInstallMsgHandler( suppressDebugHandler );
  QgsApplication::exitQgis();
}

void TestAuthBrowser::init()
{
  qStdout() << "\n" << smHashes << " Start "
  << QTest::currentTestFunction() << " " << smHashes << "\n";
  qStdout().flush();
}

void TestAuthBrowser::cleanup()
{
  qStdout() << smHashes << " End "
  << QTest::currentTestFunction() << " " << smHashes << "\n";
  qStdout().flush();
}

void TestAuthBrowser::testAuthBrowser()
{
  qDebug() << "Verify loading urls";
}

QTEST_MAIN( TestAuthBrowser )
#include "testauthbrowser.moc"
