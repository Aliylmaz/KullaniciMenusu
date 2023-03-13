#include "register.h"
#include "ui_register.h"
#include <QSqlQuery>//>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QCryptographicHash>
#include <mainwindow.h>>


Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
}

Register::~Register()
{
    delete ui;
}

void Register::on_RegisterButton_clicked()
{
    // QLineEdit nesnelerinden verileri al
    QString Ad = ui->isim_1->text();
    QString Soyad = ui->lineEdit->text();
    QString Mail = ui->lineEdit_2->text();
    QString Telefon = ui->lineEdit_3->text();
    QString Sifre = ui->lineEdit_4->text();

    // Veritabanına bağlan
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/sqlProject/database.db");
    if(!db.open()) {
        QMessageBox::critical(0, "Hata", "Veritabanına bağlanılamadı.");
        return;
    }

       QString passwordHash = QString(QCryptographicHash::hash((Sifre.toUtf8()), QCryptographicHash::Sha256).toHex());
    // Verileri veritabanına ekle
    QSqlQuery query;
    query.prepare("INSERT INTO users (Ad, Soyad, Mail, Telefon, Sifre) "
                  "VALUES (:Ad, :Soyad, :Mail, :Telefon, :Sifre)");
    query.bindValue(":Ad", Ad);
    query.bindValue(":Soyad", Soyad);
    query.bindValue(":Mail", Mail);
    query.bindValue(":Telefon", Telefon);
    query.bindValue(":Sifre", passwordHash);


    // Şifreyi şifrele

    if(!query.exec()) {
        QMessageBox::critical(0, "Hata", "Veritabanına kaydedilemedi. Hata: " + query.lastError().text());
        return;
    }

    QMessageBox::information(0, "Başarılı", "Kayıt başarılı.");
    MainWindow *mainWin = new MainWindow();
        mainWin->show();
        this->hide();

    db.close();

}






void Register::on_pushButton_toggled(bool checked)
{
    if (checked) {
        ui->lineEdit_4->setEchoMode(QLineEdit::Normal);
        ui->pushButton->setText("Gizle");
    } else {
        ui->lineEdit_4->setEchoMode(QLineEdit::Password);
        ui->pushButton->setText("Göster");
    }
}

