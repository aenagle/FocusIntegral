#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include <cstdlib>
#include <ctime>
#include <QDir>
#include <QFile>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))
    , remainingSeconds(0)
    , timerActive(false)
    , restButtonBlocked(false)
    , currentActiveTask("")
    , isRestMode(false)
    , restSeconds(0)
    , workSeconds(0)
{
    ui->setupUi(this);

    QString imagePath = QCoreApplication::applicationDirPath() + "/fon.jpg";
    if (QFile::exists(imagePath))
    {
        this->setStyleSheet("QMainWindow { background-image: url('" + imagePath + "'); background-repeat: no-repeat; background-position: center;}");
    }
    else
    {
        this->setStyleSheet("QMainWindow { background-color: #ddf5ff; }");
    }

    srand(time(nullptr));

    ui->currentTaskLabel->setText("Текущая задача: не выбрана");
    ui->currentTaskLabel->setAlignment(Qt::AlignCenter);
    ui->restButton->setEnabled(false);

    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
}

MainWindow::~MainWindow()
{
    delete ui;
}