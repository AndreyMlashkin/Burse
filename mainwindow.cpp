#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "transactionprocessor.h"
#include "transaction.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_processor(new TransactionProcessor())
{
    m_ui->setupUi(this);
    connect(m_ui->add, SIGNAL(clicked()), SLOT(addTransaction()));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::addTransaction()
{
    m_processor->process(formTransaction());
}

Transaction *MainWindow::formTransaction() const
{
    Transaction::Type type = Transaction::Invalid;
    if(m_ui->buy->isChecked())
        type = Transaction::Buy;
    else if(m_ui->sell->isChecked())
        type = Transaction::Sell;

    return new Transaction(m_ui->cost->value(), m_ui->volume->value(), type);
}
