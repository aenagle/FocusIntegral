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
#include <QDebug>

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
        this->setStyleSheet("QMainWindow { background-color: #2c3e50; }");
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

void MainWindow::on_startButton_clicked()
{
    QString minutesText = ui->lineEdit->text();
    bool ok;
    int minutes = minutesText.toInt(&ok);

    if (!ok || minutes <= 0)
    {
        QMessageBox::warning(this, "Ошибка", "Введите корректное количество минут!");
        return;
    }

    QString labelText = ui->currentTaskLabel->text();

    if (labelText.contains("не выбрана") ||
        labelText.contains("Отдых закончен") ||
        labelText.contains("Выберите задачу") ||
        labelText.contains("ОТДЫХ"))
    {
        currentActiveTask = "не выбрана";
        ui->currentTaskLabel->setText("Текущая задача: не выбрана");
    }
    else
    {
        currentActiveTask = labelText;
        currentActiveTask = currentActiveTask.replace("Работаем над: ", "");
        currentActiveTask = currentActiveTask.replace("Текущая задача: ", "");
        currentActiveTask = currentActiveTask.replace("ОТДЫХ! Осталось: ", "");
    }

    workSeconds = minutes * 60;
    remainingSeconds = workSeconds;
    timerActive = true;
    isRestMode = false;
    restButtonBlocked = false;

    timer->start(1000);
    updateTimer();

    ui->startButton->setEnabled(false);
    ui->restButton->setEnabled(true);

    ui->deleteTaskButton->setEnabled(false);
    ui->addTaskButton->setEnabled(true);
    ui->taskInput->setEnabled(true);

    ui->currentTaskLabel->setText("Работаем над: " + currentActiveTask);
}

void MainWindow::on_restButton_clicked()
{
    if (!timerActive || isRestMode)
    {
        QMessageBox::warning(this, "Ошибка", "Таймер не запущен или уже отдых");
        return;
    }

    if (restButtonBlocked)
    {
        QMessageBox::warning(this, "Доступ запрещён",
                             "Вы не решили интеграл за 3 попытки.\n"
                             "Кнопка 'Отдохнуть' заблокирована до конца таймера.");
        return;
    }

    timer->stop();

    int attempts = 0;
    const int MAX_ATTEMPTS = 3;
    bool answerCorrect = false;

    while (attempts < MAX_ATTEMPTS && !answerCorrect)
    {
        attempts++;
        answerCorrect = showIntegralDialog(attempts, MAX_ATTEMPTS);

        if (!answerCorrect && attempts < MAX_ATTEMPTS)
        {
            QMessageBox::warning(this, "НЕПРАВИЛЬНО!",
                                 "Осталось попыток: " + QString::number(MAX_ATTEMPTS - attempts));
        }
    }

    if (answerCorrect) {
        int restMinutes = workSeconds / 4;
        if (restMinutes < 60) restMinutes = 60;

        isRestMode = true;
        restSeconds = restMinutes;
        remainingSeconds = restMinutes;

        timer->start(1000);

        QMessageBox::information(this, "Отдых начался",
                                 "✅ Вы решили интеграл!\n"
                                 "Отдых: " + QString::number(restMinutes/60) + " мин.");
    }
    else
    {
        restButtonBlocked = true;
        ui->restButton->setEnabled(false);
        timer->start();

        QMessageBox::critical(this, "Попытки закончились",
                              "❌ Вы не решили интеграл за 3 попытки.\n"
                              "Кнопка 'Отдохнуть' заблокирована до конца таймера.");
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
    if (timerActive && !isRestMode)
    {
        QMessageBox::warning(this, "Ошибка", "Нельзя удалять задачу во время работы таймера!");
        return;
    }

    QListWidgetItem *currentItem = ui->taskList->currentItem();
    if (!currentItem)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите задачу для удаления!");
        return;
    }

    if (ui->currentTaskLabel->text().contains(currentItem->text()))
    {
        ui->currentTaskLabel->setText("Текущая задача: не выбрана");
    }

    delete currentItem;
}

void MainWindow::on_taskList_itemClicked(QListWidgetItem *item)
{
    if (timerActive && !isRestMode)
    {
        QMessageBox::warning(this, "Ошибка", "Нельзя сменить задачу во время работы таймера!");
        return;
    }

    if (item)
    {
        ui->currentTaskLabel->setText("Текущая задача: " + item->text());
    }
}

void MainWindow::updateTimer()
{
    if (remainingSeconds <= 0)
    {
        timer->stop();

        if (!isRestMode)
        {
            isRestMode = true;
            restButtonBlocked = false;

            int restMinutes = workSeconds / 4;
            if (restMinutes < 60) restMinutes = 60;
            restSeconds = restMinutes;
            remainingSeconds = restMinutes;

            timer->start(1000);
            ui->restButton->setEnabled(true);

            QApplication::beep();
            QMessageBox::information(this, "Время отдыха!",
                                     "Рабочий таймер завершён!\n"
                                     "Автоматический отдых: " + QString::number(restMinutes/60) + " мин.");
        }
        else
        {
            // отдых закончился
            isRestMode = false;
            timerActive = false;
            restButtonBlocked = false;

            ui->label->setText("00:00");
            ui->startButton->setEnabled(true);
            ui->restButton->setEnabled(false);
            ui->deleteTaskButton->setEnabled(true);
            ui->addTaskButton->setEnabled(true);
            ui->taskInput->setEnabled(true);
            ui->taskList->setEnabled(true);

            ui->currentTaskLabel->setText("Отдых закончен! Выберите задачу");
            currentActiveTask = "";

            QApplication::beep();
            QMessageBox::information(this, "Отдых завершён!", "Можно начинать новую работу!");
        }
        return;
    }

    remainingSeconds--;

    if (isRestMode)
    {
        restSeconds = remainingSeconds;
        ui->currentTaskLabel->setText("ОТДЫХ! Осталось: " + formatTime(remainingSeconds));
    }

    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;
    QString timeStr = QString("%1:%2")
                          .arg(minutes, 2, 10, QChar('0'))
                          .arg(seconds, 2, 10, QChar('0'));
    ui->label->setText(timeStr);
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

    QString hint = "";
    if (integralType == 4)
    {
        hint = "\n\n💡 Если инетеграл неберущийся,то\n"
               "ответьте: \"неберущийся\" или \"нет\"\n";
    }
    else if (integralType == 2)
    {
        hint = "\n\n💡 Ответ можно ввести как десятичную дробь (10.5)\n"
               "или как обыкновенную дробь (21/2)\n";
    }
    else if (integralType == 3)
    {
        hint = "\n\n💡 В ответе используйте + C";
    }

    bool ok;
    QString userAnswer = QInputDialog::getText(this, "РЕШИ ИНТЕГРАЛ!",
                                               questionWithAttempt + hint + "\n\nВведите ответ:",
                                               QLineEdit::Normal, "", &ok);

    if (!ok || userAnswer.trimmed().isEmpty())
        return false;

    if (integralType == 4)
    {
        QString normalizedAnswer = userAnswer.simplified().remove(' ').toLower();
        if (normalizedAnswer == "неберущийся" ||
            normalizedAnswer == "нет" ||
            normalizedAnswer == "неберущийсяинтеграл")
        {
            QMessageBox::information(this, "ПРАВИЛЬНО!",
                                     QString("✅ Верно!\n\n%1 - %2")
                                         .arg(currentQuestion, currentAnswer));
            return true;
        }
        else
        {
            QMessageBox::critical(this, "НЕПРАВИЛЬНО!",
                                  QString("❌ Ошибка!\n\n"
                                          "Вопрос: %1\n"
                                          "Твой ответ: %2\n"
                                          "Правильный ответ: %3 (неберущийся)")
                                      .arg(currentQuestion, userAnswer, currentAnswer));
            return false;
        }
    }

    QString normalizedUser = normalizeAnswer(userAnswer);
    QString normalizedCorrect = normalizeAnswer(currentAnswer);

    if (normalizedUser == normalizedCorrect)
    {
        QMessageBox::information(this, "ПРАВИЛЬНО!",
                                 QString("✅ Верно!\n\n%1 = %2").arg(currentQuestion, currentAnswer));
        return true;
    }
    else
    {
        QMessageBox::critical(this, "НЕПРАВИЛЬНО!",
                              QString("❌ Ошибка!\n\n"
                                      "Вопрос: %1\n"
                                      "Твой ответ: %2\n"
                                      "Правильный ответ: %3")
                                  .arg(currentQuestion, userAnswer, currentAnswer));
        return false;
    }
}

void MainWindow::generateIntegral()
{
    integralType = rand() % 4 + 1;

    switch(integralType)
    {
    case 1: generateTableIntegral(); break;
    case 2: generateDefiniteIntegral(); break;
    case 3: generatePatternIntegral(); break;
    case 4: generateNonIntegrable(); break;
    }
}

void MainWindow::generateTableIntegral()
{
    int n = rand() % 5 + 1;
    currentQuestion = QString("∫ x%1 dx").arg(n == 1 ? "" : "^" + QString::number(n));
    currentAnswer = QString("x^%1/%1 + C").arg(n + 1);

    if (n == 1)
    {
        currentQuestion = "∫ x dx";
        currentAnswer = "x^2/2 + C";
    }
}

void MainWindow::generateDefiniteIntegral()
{
    int n = rand() % 3 + 1;
    lowerLimit = rand() % 3;
    upperLimit = lowerLimit + rand() % 3 + 1;

    double result = (pow(upperLimit, n+1) - pow(lowerLimit, n+1)) / (n+1);

    currentQuestion = QString("∫ [%1..%2] x^%3 dx")
                          .arg(lowerLimit)
                          .arg(upperLimit)
                          .arg(n);

    if (result == (int)result)
    {
        currentAnswer = QString::number((int)result);
    }
    else
    {
        QString formatted = QString::number(result, 'f', 2);
        if (formatted.endsWith(".00"))
        {
            formatted = formatted.left(formatted.length() - 3);
        }
        while (formatted.endsWith('0') && formatted.contains('.'))
        {
            formatted.chop(1);
        }
        if (formatted.endsWith('.'))
        {
            formatted.chop(1);
        }
        currentAnswer = formatted;
    }
}

void MainWindow::generatePatternIntegral()
{
    int type = rand() % 2;

    switch(type)
    {
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

void MainWindow::generateNonIntegrable()
{
    int type = rand() % 3;
    switch(type)
    {
    case 0: currentQuestion = "∫ e^(x²) dx"; break;
    case 1: currentQuestion = "∫ sin(x²) dx"; break;
    case 2: currentQuestion = "∫ cos(x²) dx"; break;
    }
    currentAnswer = "неберущийся";
}

QString MainWindow::normalizeAnswer(const QString &answer)
{
    QString normalized = answer.simplified().remove(' ').toLower();

    normalized.replace(',', '.');

    if (normalized.contains('.')) {
        while (normalized.endsWith('0') && normalized.contains('.'))
        {
            normalized.chop(1);
        }

        if (normalized.endsWith('.'))
        {
            normalized.chop(1);
        }
    }

    if (normalized.contains('/')) {
        QStringList parts = normalized.split('/');
        if (parts.size() == 2)
        {
            bool ok1, ok2;
            double numerator = parts[0].toDouble(&ok1);
            double denominator = parts[1].toDouble(&ok2);
            if (ok1 && ok2 && denominator != 0)
            {
                double value = numerator / denominator;
                return QString::number(value, 'f', 2);
            }
        }
    }

    bool ok;
    double value = normalized.toDouble(&ok);
    if (ok)
    {
        return QString::number(value, 'f', 2);
    }

    return normalized;
}