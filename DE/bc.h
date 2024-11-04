#ifndef BC_H
#define BC_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>

class BC : public QWidget
{
    Q_OBJECT

public:
    explicit BC(QWidget *parent = nullptr);

private slots:
    void onDigitPressed();
    void onOperatorPressed();
    void onEqualPressed();
    void onClearPressed();

private:
    QLineEdit *display;
    QString currentOperator;
    double leftOperand;
    bool waitingForOperand;

    void calculate(double rightOperand, const QString &op);
};

#endif // BC_H
