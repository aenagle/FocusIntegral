#include "mainwindow.h"
#include <QCoreApplication>
#include <QFile>
#include <ctime>

// ФОРМАТИРОВАНИЕ ВРЕМЕНИ
QString MainWindow::formatTime(int seconds)
{
    int minutes = seconds / 60;
    int secs = seconds % 60;
    return QString("%1 мин %2 сек").arg(minutes).arg(secs);
}

// НОРМАЛИЗАЦИЯ ОТВЕТОВ
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