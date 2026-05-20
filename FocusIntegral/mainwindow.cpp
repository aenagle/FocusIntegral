#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QApplication>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <QDir>
#include <QFile>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))
    , remainingSeconds(0)
    , timerActive(false)
    , currentActiveTask("")
    , isRestMode(false)
    , restSeconds(0)
    , workSeconds(0)
{
    ui->setupUi(this);
    QString imagePath = QCoreApplication::applicationDirPath() + "/fon.jpg";
    if (QFile::exists(imagePath)) {
        this->setStyleSheet("QMainWindow { background-image: url('" + imagePath + "'); background-repeat: no-repeat; background-position: center;}");
    } else {
        this->setStyleSheet("QMainWindow { background-color: #2c3e50; }");
    }
    srand(time(nullptr));
    ui->currentTaskLabel->setText("Текущая задача: не выбрана");
    ui->currentTaskLabel->setAlignment(Qt::AlignCenter);
    ui->extendRestButton->setVisible(false);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    QString minutesText = ui->lineEdit->text();
    bool ok;
    int minutes = minutesText.toInt(&ok);

    if (!ok || minutes <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите корректное количество минут!");
        return;
    }

    // Запоминаем рабочее время (в секундах)
    workSeconds = minutes * 60;
    remainingSeconds = workSeconds;
    timerActive = true;
    isRestMode = false;  // Рабочий режим
    timer->start(1000);
    updateTimer();

    ui->startButton->setEnabled(false);
    ui->resetButton->setEnabled(true);
    ui->extendRestButton->setVisible(false);  // Скрываем кнопку продления



    ui->currentTaskLabel->setText("🎯Работаем над: " + currentActiveTask);
}

void MainWindow::on_resetButton_clicked()
{
    if (timerActive) {
        timer->stop();

        int attempts = 0;
        const int MAX_ATTEMPTS = 3;
        bool answerCorrect = false;

        while (attempts < MAX_ATTEMPTS && !answerCorrect) {
            attempts++;

            answerCorrect = showIntegralDialog(attempts, MAX_ATTEMPTS);

            if (!answerCorrect && attempts < MAX_ATTEMPTS) {
                QMessageBox::warning(this, "НЕПРАВИЛЬНО!",
                                     "Неправильный ответ!\n\n"
                                     "Осталось попыток: " + QString::number(MAX_ATTEMPTS - attempts));
            }
        }

        if (answerCorrect) {
            timer->start();
            QMessageBox::information(this, "УСПЕХ!",
                                     "Ты решил интеграл!\n\nТаймер продолжает работу.");
        } else {
            remainingSeconds = 0;
            ui->label->setText("00:00");
            ui->startButton->setEnabled(true);
            ui->resetButton->setEnabled(false);
            QMessageBox::critical(this, "НАКАЗАНИЕ!",
                                  "Ты не решил интеграл за 3 попытки!\n\nТаймер СБРОШЕН!");
        }

    } else {
        QMessageBox::warning(this, "Ошибка", "Таймер не был запущен");
    }
}

void MainWindow::on_addTaskButton_clicked()
{
    QString taskText = ui->taskInput->text();
    if (taskText.trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Введите текст задачи!");
        return;
    }
    ui->taskList->addItem(taskText);
    ui->taskInput->clear();
}
void MainWindow::on_deleteTaskButton_clicked()
{
    QListWidgetItem *currentItem = ui->taskList->currentItem();

    if (currentItem == nullptr) {
        QMessageBox::warning(this, "Ошибка", "Выберите задачу для удаления!");
        return;
    }

    QString currentTask = ui->currentTaskLabel->text();
    if (currentTask.contains(currentItem->text())) {
        // Если удаляем выбранную задачу - сбрасываем отображение
        ui->currentTaskLabel->setText("Текущая задача: не выбрана");
    }

    delete currentItem;
}

void MainWindow::on_taskList_itemClicked(QListWidgetItem *item)
{
    if (item != nullptr) {
        // Получаем текст выбранной задачи
        QString taskText = item->text();

        // Отображаем её наверху
        ui->currentTaskLabel->setText("Текущая задача: " + taskText);
    }
}

void MainWindow::updateTimer()
{
    if (remainingSeconds <= 0) {
        timer->stop();

        if (!isRestMode) {
            // РАБОЧИЙ ТАЙМЕР ЗАКОНЧИЛСЯ → ОТДЫХ
            isRestMode = true;

            int restMinutes = workSeconds / 4;
            if (restMinutes < 60) restMinutes = 60;
            restSeconds = restMinutes;
            remainingSeconds = restSeconds;

            timer->start(1000);

            ui->currentTaskLabel->setText("☕ ОТДЫХ! Осталось: " + formatTime(restSeconds));
            ui->extendRestButton->setVisible(true);
            ui->resetButton->setEnabled(true);

            QApplication::beep();
            QMessageBox::information(this, "Время отдыха!",
                                     "Рабочий таймер завершен!\nНачинается отдых " + QString::number(restMinutes/60) + " мин");
        } else {
            // ОТДЫХ ЗАКОНЧИЛСЯ
            isRestMode = false;
            timerActive = false;
            ui->label->setText("00:00");
            ui->startButton->setEnabled(true);
            ui->resetButton->setEnabled(false);
            ui->extendRestButton->setVisible(false);

            ui->taskInput->setEnabled(true);
            ui->addTaskButton->setEnabled(true);
            ui->deleteTaskButton->setEnabled(true);
            ui->taskList->setEnabled(true);

            ui->currentTaskLabel->setText("📋 Отдых закончен! Выберите задачу");
            currentActiveTask = "";

            QApplication::beep();
            QMessageBox::information(this, "Отдых завершен!", "Можно начинать новую работу!");
        }
        return;
    }

    remainingSeconds--;

    if (isRestMode) {
        restSeconds = remainingSeconds;
        ui->currentTaskLabel->setText("☕ ОТДЫХ! Осталось: " + formatTime(remainingSeconds));
    }

    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;
    QString timeStr = QString("%1:%2")
                          .arg(minutes, 2, 10, QChar('0'))
                          .arg(seconds, 2, 10, QChar('0'));
    ui->label->setText(timeStr);
}
void MainWindow::on_extendRestButton_clicked()
{
    if (!isRestMode) return;

    timer->stop();

    bool answerCorrect = showIntegralDialog(1, 1);

    if (answerCorrect) {
        int extendSeconds = workSeconds / 4;
        if (extendSeconds < 60) extendSeconds = 60;
        remainingSeconds += extendSeconds;
        restSeconds = remainingSeconds;

        timer->start();

        QMessageBox::information(this, "Отдых продлён!",
                                 "Отдых продлён на " + QString::number(extendSeconds/60) + " мин!");
        ui->currentTaskLabel->setText("ОТДЫХ ПРОДЛЁН! Осталось: " + formatTime(remainingSeconds));
    } else {
        isRestMode = false;
        timerActive = false;
        remainingSeconds = 0;
        ui->label->setText("00:00");
        ui->startButton->setEnabled(true);
        ui->resetButton->setEnabled(false);
        ui->extendRestButton->setVisible(false);

        ui->taskInput->setEnabled(true);
        ui->addTaskButton->setEnabled(true);
        ui->deleteTaskButton->setEnabled(true);
        ui->taskList->setEnabled(true);

        ui->currentTaskLabel->setText("Отдых прерван! Выберите задачу");
        currentActiveTask = "";

        QMessageBox::critical(this, "Отдых завершён!", "Ты не решил интеграл! Отдых закончен.");
    }
}

QString MainWindow::formatTime(int seconds)
{
    int minutes = seconds / 60;
    int secs = seconds % 60;
    return QString("%1 мин %2 сек").arg(minutes).arg(secs);
}
bool MainWindow::showIntegralDialog(int currentAttempt, int maxAttempts)
{
    generateIntegral();

    QString questionWithAttempt = QString("%1\n\n--- ПОПЫТКА %2 ИЗ %3 ---")
                                      .arg(currentQuestion)
                                      .arg(currentAttempt)
                                      .arg(maxAttempts);

    bool ok;
    QString userAnswer = QInputDialog::getText(this, "РЕШИ ИНТЕГРАЛ!",
                                               questionWithAttempt + "\n\nВведите ответ:",
                                               QLineEdit::Normal, "", &ok);

    if (!ok || userAnswer.trimmed().isEmpty()) {
        return false;
    }

    QString normalizedUser = userAnswer.simplified().remove(' ').toLower();
    QString normalizedCorrect = currentAnswer.simplified().remove(' ').toLower();

    if (normalizedUser == normalizedCorrect) {
        QMessageBox::information(this, "ПРАВИЛЬНО!",
                                 QString("Верно!\n\n%1 = %2").arg(currentQuestion, currentAnswer));
        return true;
    } else {
        QMessageBox::critical(this, "НЕПРАВИЛЬНО!",
                              QString("Ошибка!\n\n"
                                      "Вопрос: %1\n"
                                      "Твой ответ: %2\n"
                                      "Правильный ответ: %3")
                                  .arg(currentQuestion, userAnswer, currentAnswer));
        return false;
    }
}

void MainWindow::generateIntegral()
{
    // Выбираем случайный тип интеграла (1-4)
    integralType = rand() % 4 + 1;

    switch(integralType) {
    case 1:
        generateTableIntegral();
        break;
    case 2:
        generateDefiniteIntegral();
        break;
    case 3:
        generatePatternIntegral();
        break;
    case 4:
        generateNonIntegrable();
        break;
    }
}

// 1. ТАБЛИЧНЫЕ ИНТЕГРАЛЫ (∫ x^n dx)
void MainWindow::generateTableIntegral()
{
    int n = rand() % 5 + 1;  // степень от 1 до 5

    // Красивое отображение с верхним индексом
    currentQuestion = QString("∫ x%1 dx").arg(n == 1 ? "" : "^" + QString::number(n));
    currentAnswer = QString("x^%1/%1 + C").arg(n + 1);

    // Исправляем отображение для x^1 (просто x)
    if (n == 1) {
        currentQuestion = "∫ x dx";
        currentAnswer = "x^2/2 + C";
    }
}

// 2. ОПРЕДЕЛЕННЫЕ ИНТЕГРАЛЫ (∫ₐᵇ x^n dx)
void MainWindow::generateDefiniteIntegral()
{
    int n = rand() % 3 + 1;  // степень 1, 2 или 3
    lowerLimit = rand() % 3;  // 0, 1 или 2
    upperLimit = lowerLimit + rand() % 3 + 1;  // больше нижнего

    // Вычисляем значение
    double result = (pow(upperLimit, n+1) - pow(lowerLimit, n+1)) / (n+1);

    // Красивое отображение с пределами
    currentQuestion = QString("∫ [%1..%2] x^%3 dx")
                          .arg(lowerLimit)
                          .arg(upperLimit)
                          .arg(n);

    // Форматируем ответ (дробь или десятичная)
    if (result == (int)result) {
        currentAnswer = QString::number((int)result);
    } else if (result == 0.5) {
        currentAnswer = "1/2";
    } else if (result == 0.33 || result == 0.3333333333333333) {
        currentAnswer = "1/3";
    } else if (result == 0.66 || result == 0.6666666666666666) {
        currentAnswer = "2/3";
    } else {
        currentAnswer = QString::number(result, 'f', 2);
    }
}

void MainWindow::generatePatternIntegral()
{
    int type = rand() % 2;

    switch(type) {
    case 0:
        currentQuestion = "∫ dx/(x² + a²)";
        currentAnswer = "(1/a)arctg(x/a) + C";
        break;
    case 1:
        currentQuestion = "∫ dx/√(a² - x²)";
        currentAnswer = "arcsin(x/a) + C";
        break;
    }
}

// 4. НЕБЕРУЩИЕСЯ ИНТЕГРАЛЫ
void MainWindow::generateNonIntegrable()
{
    int type = rand() % 3;

    switch(type)
    {
    case 0:
        currentQuestion = "∫ e^(x²) dx";
        currentAnswer = "неберущийся";
        break;
    case 1:
        currentQuestion = "∫ sin(x²) dx";
        currentAnswer = "неберущийся";
        break;
    case 2:
        currentQuestion = "∫ cos(x²) dx";
        currentAnswer = "неберущийся";
        break;
    }
}