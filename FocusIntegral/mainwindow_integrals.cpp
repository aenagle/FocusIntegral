#include "mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <cmath>

// ДИАЛОГ РЕШЕНИЯ ИНТЕГРАЛА
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
        hint = "\n\n💡 Если интеграл неберущийся, то\n"
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

// ГЕНЕРАЦИЯ ИНТЕГРАЛА (ОСНОВНАЯ)
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

// ТАБЛИЧНЫЕ ИНТЕГРАЛЫ
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

// ОПРЕДЕЛЕННЫЕ ИНТЕГРАЛЫ
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

// ИНТЕГРАЛЫ ПО ШАБЛОНАМ
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

// НЕБЕРУЩИЕСЯ ИНТЕГРАЛЫ
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