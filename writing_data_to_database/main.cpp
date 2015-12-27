#include <QApplication>

#include <QSqlDatabase>

#include <QStringList>

#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
QSqlQuery *query;
#include <QSqlQuery>
#include <string>
#include <QString>
#include <QTime>
#include <QMutex>
#include <QWaitCondition>
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
bool createConnection()

{

    QStringList drivers = QSqlDatabase::drivers();

    foreach(QString driver, drivers)

        qDebug() << "\t" << driver;




    qDebug() << "ODBC driver valid?" << db.isValid();



// 注意,对于express版本数据库, 一定要加\\sqlexpress这种后缀 SERVER=58.67.161.109\\sqlexpress

    QString dsn = "DRIVER={SQL SERVER};SERVER=TIANMN-PC\\SQLEXPRESS;DATABASE=temp_test;";

  //  db.setHostName("127.0.0.1");   //数据库服务器，我这里不需要

//即使dsn中已经设置了UID和PASSWD，仍然需要执行setUserName和setPassword的步骤

//还要注意这里的用户名要有创建表的权限，不然创建下面的表student会不成功。

    db.setDatabaseName(dsn);   //数据库名

    db.setUserName("tianmn2109");//登录名，我再dsn里设置UID和PWD后，就不需要设置了

    db.setPassword("10000000");//密码，我再dsn里设置UID和PWD后，就不需要设置了

    if(!db.open ())

    {

        QSqlError error = db.lastError();
        qDebug () << error.text() << endl;

        QMessageBox::warning (0, "Warning", QString("Failed to open database!Error:%1").arg(error.text()));

        return false;

    }
    query = new QSqlQuery(db);;

return true;

}


int main(int argc, char* argv[])

{

     QApplication app(argc, argv);
     QTime t = QTime::currentTime();
     srand(t.msec() + t.second()*1000);

     long long count = 0;
     if (true == createConnection()) {
         qDebug() << "connection success" << endl;
     }
     while (true) {

     qDebug() << "update count = " << count++ << endl;
     int r = qrand() % 30;
     QString strSql = "update dbo.testData set value = '" +  QString::number(60 +r) + "' where tbim = '1' and alias = '1'";
     if (true == query->exec(strSql)) {
      //   qDebug() << "update success" << endl;
     }
     r = qrand() % 30;
     strSql = "update dbo.testData set value = '" +  QString::number(60 +r) + "' where tbim = '1' and alias = '2'";
     if (true == query->exec(strSql)) {
       //  qDebug() << "update success" << endl;
     }

     r = qrand() % 25;
     strSql = "update dbo.testData set value = '" +  QString::number(-19 +r) + "' where tbim = '1' and alias = '3'";
     if (true == query->exec(strSql)) {
      //   qDebug() << "update success" << endl;
     }

     r = qrand() % 30;
     strSql = "update dbo.testData set value = '" +  QString::number(50 +r) + "' where tbim = '1' and alias = '4'";
     if (true == query->exec(strSql)) {
      //   qDebug() << "update success" << endl;
     }

     r = qrand() % 30;
     strSql = "update dbo.testData set value = '" +  QString::number(60 +r) + "' where tbim = '2' and alias = '5'";
     if (true == query->exec(strSql)) {
       //  qDebug() << "update success" << endl;
     }
     r = qrand() % 50;
     strSql = "update dbo.testData set value = '" +  QString::number(10 +r) + "' where tbim = '2' and alias = '6'";
     if (true == query->exec(strSql)) {
       //  qDebug() << "update success" << endl;
     }


     r = qrand() % 40;
     strSql = "update dbo.testData set value = '" +  QString::number(-20 +r) + "' where tbim = '3' and alias = '8'";
     if (true == query->exec(strSql)) {
        // qDebug() << "update success" << endl;
     }
     r = qrand() % 100;
     strSql = "update dbo.testData set value = '" +  QString::number(0 +r) + "' where tbim = '3' and alias = '9'";
     if (true == query->exec(strSql)) {
        // qDebug() << "update success" << endl;
     }
     r = qrand() %60;
     strSql = "update dbo.testData set value = '" +  QString::number(-(5 + r)) + "' where tbim = '3' and alias = '10'";
     if (true == query->exec(strSql)) {
        // qDebug() << "update success" << endl;
     }

     r = qrand() %2;
     strSql = "update dbo.testData set value = '" +  QString::number(r) + "' where tbim = '7' and alias = '11'";
     if (true == query->exec(strSql)) {
        // qDebug() << "update success" << endl;
     }
     \
     r = qrand() %200;
     strSql = "update dbo.testData set value = '" +  QString::number(r) + "' where tbim = '7' and alias = '12'";
     if (true == query->exec(strSql)) {
        // qDebug() << "update success" << endl;
     }
      QMutex mutex;
      QWaitCondition sleep;
      mutex.lock();
      sleep.wait(&mutex, 800);
      mutex.unlock();
     }
     return 0;
}
