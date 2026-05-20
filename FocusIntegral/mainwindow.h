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
    void on_startButton_clicked();      // запуск таймера
    void on_resetButton_clicked();      // сброс таймера
    void on_addTaskButton_clicked();    // добавить задачу
    void on_deleteTaskButton_clicked(); // удалить задачу
    void updateTimer();                  // обновление отображения таймера
    void on_taskList_itemClicked(QListWidgetItem *item);
    void on_extendRestButton_clicked();


private:
    Ui::MainWindow *ui;
    QTimer *timer;          // объект таймера
    int remainingSeconds;   // сколько секунд осталось
    bool timerActive;       // активен ли таймер
    QString currentActiveTask;

    bool isRestMode;           // true = отдых, false = работа
    int restSeconds;           // сколько секунд отдыха осталось
    int workSeconds;           // исходное время работы (для расчета отдыха)

    // Для интегралов
    int integralType;  // 1=табличный, 2=определенный, 3=неберущийся
    QString currentQuestion;
    QString currentAnswer;
    double lowerLimit;  // нижний предел
    double upperLimit;  // верхний предел
    QString formatTime(int seconds);

    bool showIntegralDialog(int currentAttempt, int maxAttempts);
    void generateIntegral();  // генерация разных интегралов
    void generateTableIntegral();
    void generateDefiniteIntegral();
    void generatePatternIntegral();
    void generateNonIntegrable();

    friend class TestMinuteInput;

};

#endif