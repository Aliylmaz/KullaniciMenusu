#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <register.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QCryptographicHash>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    // QLineEdit nesnelerinden verileri al
        QString Mail = ui->Mail->text();
        QString Sifre = ui->Sifre->text();


        // Veritabanına bağlan
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("C:/sqlProject/database.db");
        if(!db.open()) {
            QMessageBox::critical(0, "Hata", "Veritabanına bağlanılamadı.");
            return;
        }
         QString passwordHash = QString(QCryptographicHash::hash((Sifre.toUtf8()), QCryptographicHash::Sha256).toHex());

        // Verileri veritabanından sorgula
        QSqlQuery query;
        query.prepare("SELECT * FROM users WHERE Mail = :Mail AND Sifre = :Sifre");
        query.bindValue(":Mail", Mail);
        query.bindValue(":Sifre", passwordHash); // Şifrelenmiş şifre kullanıldı
        if(!query.exec()) {
            QMessageBox::critical(0, "Hata", "Veritabanı hatası: " + query.lastError().text());
            return;
        }


        // Sorgu sonucunu değerlendir
        if(query.next()) {
            QMessageBox::information(0, "Başarılı", "Giriş başarılı.");

        } else {
            QMessageBox::critical(0, "Hata", "Giriş başarısız. Lütfen bilgilerinizi kontrol edin.");
        }

        db.close();
}


void MainWindow::on_pushButton_2_clicked()
{

    hide();
    reg =new Register(this);
    reg->show();

}





