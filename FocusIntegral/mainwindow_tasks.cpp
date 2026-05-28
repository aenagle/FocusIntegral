#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QListWidgetItem>

// ДОБАВЛЕНИЕ ЗАДАЧИ
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

// УДАЛЕНИЕ ЗАДАЧИ
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

// ВЫБОР ЗАДАЧИ
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