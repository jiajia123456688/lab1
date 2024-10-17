#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    digitBTNs = {{Qt::Key_0, ui->btnNum0},
        {Qt::Key_1, ui->btnNum1},
        {Qt::Key_2, ui->btnNum2},
        {Qt::Key_3, ui->btnNum3},
        {Qt::Key_4, ui->btnNum4},
        {Qt::Key_5, ui->btnNum5},
        {Qt::Key_6, ui->btnNum6},
        {Qt::Key_7, ui->btnNum7},
        {Qt::Key_8, ui->btnNum8},
        {Qt::Key_9, ui->btnNum9},
    };
    digitcopes = {
        {Qt::Key_Plus, ui->btnPlus},
        {Qt::Key_Minus, ui->btnMinus},
        {Qt::Key_multiply, ui->btnmultiple},
        {Qt::Key_division, ui->btndivision},
    };

    foreach (auto btn, digitBTNs)
        connect(btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));

    foreach (auto btn, digitcopes) {
        connect(btn, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClick()));

    }

    // connect(ui->btnNum0, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum1, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum2, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum3, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum4, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum5, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum6, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum7, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum8, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum9, SIGNAL(clicked()), this, SLOT(btnNumClicked()));

    // connect(ui->btnPlus, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClick()));
    // connect(ui->btnMinus, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClick()));
    // connect(ui->btnMultiple, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClick()));
    // connect(ui->btnDivision, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClick()));

    connect(ui->btnPer, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClick()));
    connect(ui->btnInverse, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClick()));
    connect(ui->btnSquare, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClick()));
    connect(ui->btnSqrt, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClick()));
    connect(ui->btnPeriod, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClick()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnEqual_clicked()
{
    // 检查当前操作数是否为空，如果不为空，将其推入操作数栈
    if (!operand.isEmpty()) {
        operands.push_back(operand);
        operand.clear();  // 清空当前操作数
    }

    // 确保操作符和操作数栈至少各有一个元素
    if (operands.size() < 2 || opcodes.isEmpty()) {
        ui->statusbar->showMessage("Error: Insufficient operands or operators");
        return;
    }

    // 执行计算并获取结果
    QString result = calculation();

    // 检查是否发生错误
    if (result == "Error") {
        ui->disPaly->setText("Error");
        return;
    }

    // 将计算结果显示出来
    ui->disPaly->setText(result);

    // 清空操作符栈，并将结果作为后续操作的第一个操作数
    opcodes.clear();
    operands.clear();  // 清空之前的操作数
    operands.push_back(result);  // 将结果存入操作数栈中
}



QString MainWindow::calculation(bool *ok )
{
    double result = 0;

    // 确保操作数和操作符栈里有足够的元素
    if (operands.size() >= 2 && opcodes.size() > 0) {
        // 取操作数
        double operand1 = operands.front().toDouble();
        operands.pop_front();
        double operand2 = operands.front().toDouble();
        operands.pop_front();

        // 取操作符
        QString op = opcodes.front();
        opcodes.pop_front();

        // 执行加减乘除运算
        if (op == "+") {
            result = operand1 + operand2;
        } else if (op == "-") {
            result = operand1 - operand2;
        } else if (op == "×") {
            result = operand1 * operand2;
        } else if (op == "/") {
            if (operand2 == 0) {
                ui->statusbar->showMessage("Error: Division by zero");
                return "Error";
            }
            result = operand1 / operand2;
        }

        // 将计算结果重新存入操作数列表
        operands.push_back(QString::number(result));

        // 显示操作数和操作符堆栈的大小
        ui->statusbar->showMessage(QString("Operands: %1, Operators: %2").arg(operands.size()).arg(opcodes.size()));
    } else {
        ui->statusbar->showMessage("Error: Insufficient operands or operators");
        return "Error";
    }

    return QString::number(result);
}


void MainWindow::btnNumClicked()
{
    QString digit = qobject_cast<QPushButton *> (sender())->text();
    ui->statusbar->showMessage(digit + "+" + operand);
    if (digit == "0" && operand == "0")
        digit = "";
    if (operand == "0" && digit != "0")
        operand = "";
    operand += digit;
    ui->disPaly->setText(operand);
}

void MainWindow::btnBinaryOperatorClick()
{
    QString opcode = qobject_cast<QPushButton *>(sender())->text();  // 获取操作符

    // 如果当前有输入的操作数，将其推入操作数栈
    if (!operand.isEmpty()) {
        operands.push_back(operand);
        operand.clear();  // 清空当前操作数
    }

    // 如果操作数栈只有一个数并且之前有结果，则将其复制为第一个操作数
    if (operands.size() == 1 && !opcodes.isEmpty()) {
        operands.push_back(operands.top());  // 将结果作为新的操作数
    }

    // 将操作符推入操作符栈
    opcodes.push_back(opcode);
}


void MainWindow::btnUnaryOperatorClick()
{
    // 如果没有新的操作数，使用计算后的最后结果
    if (operand.isEmpty() && !operands.isEmpty()) {
        operand = operands.back();
        operands.pop_back();
    }

    if (!operand.isEmpty()) {
        double result = operand.toDouble();
        operand.clear();

        QString op = qobject_cast<QPushButton *>(sender())->text();

        if (op == "%") {
            result /= 100.0;
        } else if (op == "1/x") {
            if (result == 0) {
                ui->statusbar->showMessage("Error: Division by zero");
                return;
            }
            result = 1 / result;
        } else if (op == "x^2") {
            result *= result;
        } else if (op == "√") {
            if (result < 0) {
                ui->statusbar->showMessage("Error: Negative square root");
                return;
            }
            result = sqrt(result);
        } else if (op == "+/-") {
            result = -result;
        }

        operand = QString::number(result);

        // 更新显示
        ui->disPaly->setText(operand);

        // 将计算结果重新压入操作数队列
        operands.push_back(operand);
    }
}


void MainWindow::on_btnSign_clicked()
{
    if (!operand.contains("."))
        operand += qobject_cast<QPushButton * > (sender())->text();
    ui->disPaly->setText(operand);

}


void MainWindow::on_btnDel_clicked()
{
    operand = operand.left(operand.length() - 1);
    ui->disPaly->setText(operand);

}

void MainWindow::on_btnClearAll_clicked()
{
    operand.clear();
    operands.clear();
    opcodes.clear();
    opcode.clear();
    ui->disPaly->setText(operand);

}


void MainWindow::keyPressEvent(QKeyEvent * event)
{
    foreach (auto btnKey, digitBTNs.keys()) {
        if (event->key() == btnKey)
            digitBTNs[btnKey]->animateClick();
    }

    foreach (auto btnKey, digitcopes.keys()) {
        if (event->key() == btnKey)
            digitcopes[btnKey]->animateClick();
    }

    if (event->key() == Qt::Key_Period)
        ui->btnSign->animateClick();

    if (event->key() == Qt::Key_Slash)
        ui->btndivision->animateClick();

    if (event->key() == Qt::Key_Asterisk)
        ui->btnmultiple->animateClick();

    if (event->key() == Qt::Key_Equal)
        ui->btnEqual->animateClick();

    if (event->key() == Qt::Key_Delete)
        ui->btnDel->animateClick();
}


