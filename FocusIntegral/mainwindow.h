#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void on_restButton_clicked();
    void on_addTaskButton_clicked();
    void on_deleteTaskButton_clicked();
    void on_taskList_itemClicked(QListWidgetItem *item);
    void updateTimer();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int remainingSeconds;
    bool timerActive;
    bool restButtonBlocked;
    QString currentActiveTask;
    QString normalizeAnswer(const QString &answer);

    bool isRestMode;
    int restSeconds;
    int workSeconds;

    // Интегралы
    int integralType;
    QString currentQuestion;
    QString currentAnswer;
    double lowerLimit;
    double upperLimit;

    QString formatTime(int seconds);
    bool showIntegralDialog(int currentAttempt, int maxAttempts);
    void generateIntegral();
    void generateTableIntegral();
    void generateDefiniteIntegral();
    void generatePatternIntegral();
    void generateNonIntegrable();
};

#endif