#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QApplication>

// ЗАПУСК ТАЙМЕРА
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

// ОТДЫХ (КНОПКА ОТДОХНУТЬ)
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

// ОБНОВЛЕНИЕ ТАЙМЕРА
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