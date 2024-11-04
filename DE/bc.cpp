#include "bc.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>

BC::BC(QWidget *parent)
    : QWidget(parent), leftOperand(0), waitingForOperand(true)
{
    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);

    QGridLayout *gridLayout = new QGridLayout;

    struct ButtonConfig {
        QString text;
        QSize size;
        int row;
        int col;
        int rowSpan;
        int colSpan;
    };

    QList<ButtonConfig> buttons = {
        {"C", QSize(50, 35), 0, 0, 1, 1},
        {"/", QSize(50, 35), 0, 1, 1, 1},
        {"*", QSize(50, 35), 0, 2, 1, 1},
        {"-", QSize(50, 35), 0, 3, 1, 1},
        {"7", QSize(50, 35), 1, 0, 1, 1},
        {"8", QSize(50, 35), 1, 1, 1, 1},
        {"9", QSize(50, 35), 1, 2, 1, 1},
        {"+", QSize(50, 50), 1, 3, 2, 1},
        {"4", QSize(50, 35), 2, 0, 1, 1},
        {"5", QSize(50, 35), 2, 1, 1, 1},
        {"6", QSize(50, 35), 2, 2, 1, 1},
        {"=", QSize(50, 50), 2, 3, 3, 1},
        {"1", QSize(50, 35), 3, 0, 1, 1},
        {"2", QSize(50, 35), 3, 1, 1, 1},
        {"3", QSize(50, 35), 3, 2, 1, 1},
        {"0", QSize(110, 35), 4, 0, 1, 2},
        {".", QSize(50, 35), 4, 2, 1, 1}
    };

    for (const ButtonConfig &config : buttons) {
        QPushButton *button = new QPushButton(config.text);
        button->setFixedSize(config.size);
        gridLayout->addWidget(button, config.row, config.col, config.rowSpan, config.colSpan);

        if (config.text == "C") {
            connect(button, &QPushButton::clicked, this, &BC::onClearPressed);
        } else if (config.text == "=") {
            connect(button, &QPushButton::clicked, this, &BC::onEqualPressed);
        } else if (config.text == "+" || config.text == "-" || config.text == "*" || config.text == "/") {
            connect(button, &QPushButton::clicked, this, &BC::onOperatorPressed);
        } else if (config.text == ".") {
            connect(button, &QPushButton::clicked, this, [this]() {
                if (!display->text().contains('.')) {
                    display->setText(display->text() + ".");
                }
            });
        } else {
            connect(button, &QPushButton::clicked, this, &BC::onDigitPressed);
        }
    }

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(display);
    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);
}

void BC::onDigitPressed()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    QString digitValue = button->text();

    if (display->text() == "0" || waitingForOperand) {
        display->setText(digitValue);
        waitingForOperand = false;
    } else {
        display->setText(display->text() + digitValue);
    }
}

void BC::onOperatorPressed()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    QString clickedOperator = button->text();
    double operand = display->text().toDouble();

    if (!waitingForOperand) {
        calculate(operand, currentOperator);
        display->setText(QString::number(leftOperand));
    }

    currentOperator = clickedOperator;
    waitingForOperand = true;
}

void BC::onEqualPressed()
{
    double operand = display->text().toDouble();
    calculate(operand, currentOperator);
    display->setText(QString::number(leftOperand));
    currentOperator.clear();
    waitingForOperand = true;
}

void BC::onClearPressed()
{
    leftOperand = 0;
    currentOperator.clear();
    display->setText("0");
    waitingForOperand = true;
}

void BC::calculate(double rightOperand, const QString &op)
{
    if (op == "+") {
        leftOperand += rightOperand;
    } else if (op == "-") {
        leftOperand -= rightOperand;
    } else if (op == "*") {
        leftOperand *= rightOperand;
    } else if (op == "/") {
        if (rightOperand != 0.0) {
            leftOperand /= rightOperand;
        }
    } else {
        leftOperand = rightOperand;
    }
}
