/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QListWidget *taskList;
    QPushButton *startButton;
    QPushButton *deleteTaskButton;
    QLabel *label;
    QLineEdit *taskInput;
    QPushButton *resetButton;
    QPushButton *addTaskButton;
    QLineEdit *lineEdit;
    QLabel *currentTaskLabel;
    QPushButton *extendRestButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(800, 600));
        MainWindow->setMaximumSize(QSize(800, 600));
        QFont font;
        font.setPointSize(10);
        MainWindow->setFont(font);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        taskList = new QListWidget(centralwidget);
        taskList->setObjectName("taskList");
        taskList->setGeometry(QRect(520, 70, 256, 371));
        taskList->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        startButton = new QPushButton(centralwidget);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(80, 400, 151, 29));
        deleteTaskButton = new QPushButton(centralwidget);
        deleteTaskButton->setObjectName("deleteTaskButton");
        deleteTaskButton->setGeometry(QRect(650, 490, 151, 29));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(100, 130, 351, 201));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Arial")});
        font1.setPointSize(80);
        font1.setBold(false);
        font1.setUnderline(false);
        font1.setKerning(true);
        label->setFont(font1);
        label->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 127);"));
        label->setFrameShape(QFrame::Shape::NoFrame);
        taskInput = new QLineEdit(centralwidget);
        taskInput->setObjectName("taskInput");
        taskInput->setGeometry(QRect(530, 450, 251, 26));
        resetButton = new QPushButton(centralwidget);
        resetButton->setObjectName("resetButton");
        resetButton->setGeometry(QRect(280, 400, 151, 29));
        addTaskButton = new QPushButton(centralwidget);
        addTaskButton->setObjectName("addTaskButton");
        addTaskButton->setGeometry(QRect(490, 490, 151, 29));
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(120, 330, 271, 26));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Arial")});
        font2.setPointSize(10);
        lineEdit->setFont(font2);
        lineEdit->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        lineEdit->setAutoFillBackground(false);
        lineEdit->setFrame(false);
        lineEdit->setReadOnly(false);
        currentTaskLabel = new QLabel(centralwidget);
        currentTaskLabel->setObjectName("currentTaskLabel");
        currentTaskLabel->setGeometry(QRect(100, 90, 341, 41));
        currentTaskLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        extendRestButton = new QPushButton(centralwidget);
        extendRestButton->setObjectName("extendRestButton");
        extendRestButton->setGeometry(QRect(10, 500, 251, 29));
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(extendRestButton->sizePolicy().hasHeightForWidth());
        extendRestButton->setSizePolicy(sizePolicy1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 28));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "FocusIntegral ", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\321\203\321\201\321\202\320\270\321\202\321\214 \321\202\320\260\320\271\320\274\320\265\321\200", nullptr));
        deleteTaskButton->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\267\320\260\320\264\320\260\321\207\321\203", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "00:00", nullptr));
        taskInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\235\320\276\320\262\320\260\321\217 \320\267\320\260\320\264\320\260\321\207\320\260...", nullptr));
        resetButton->setText(QCoreApplication::translate("MainWindow", "\320\241\320\261\321\200\320\276\321\201\320\270\321\202\321\214 \321\202\320\260\320\271\320\274\320\265\321\200", nullptr));
        addTaskButton->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\267\320\260\320\264\320\260\321\207\321\203", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\274\320\270\320\275\321\203\321\202\321\213 (\320\274\320\260\320\272\321\201\320\270\320\274\321\203\320\274 120 \320\274\320\270\320\275\321\203\321\202)", nullptr));
        currentTaskLabel->setText(QCoreApplication::translate("MainWindow", "\320\242\320\265\320\272\321\203\321\211\320\260\321\217 \320\267\320\260\320\264\320\260\321\207\320\260: \320\275\320\265 \320\262\321\213\320\261\321\200\320\260\320\275\320\260", nullptr));
        extendRestButton->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\276\320\264\320\273\320\270\321\202\321\214 \320\276\321\202\320\264\321\213\321\205 (+\320\270\320\275\321\202\320\265\320\263\321\200\320\260\320\273)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
