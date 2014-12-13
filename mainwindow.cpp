#include "qcustomplot/qcustomplot.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "transactionprocessor.h"
#include "transaction.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_processor(new TransactionProcessor()),
    m_plotter(new QCustomPlot())
{    
    m_ui->setupUi(this);
    centralWidget()->layout()->addWidget(m_plotter);
    m_plotter->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    connect(m_ui->add, SIGNAL(clicked()), SLOT(addTransaction()));

    initGraphics();
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

void MainWindow::initGraphics()
{
    m_plotter->axisRect()->setupFullAxesBox();
    m_plotter->plotLayout()->insertRow(0);
    m_plotter->plotLayout()->addElement(0, 0, new QCPPlotTitle(m_plotter, tr("Торговая сессия")));

    m_plotter->xAxis->setLabel("Время");
    m_plotter->yAxis->setLabel("Цена");
    m_plotter->legend->setVisible(true);


    QPen graphPen;
    graphPen.setColor(Qt::black);
    graphPen.setWidthF(3);

    m_plotter->addGraph();
    m_plotter->graph(0)->setLineStyle(QCPGraph::lsLine);
    m_plotter->graph(0)->setName("Цена");
    m_plotter->graph(0)->setPen(graphPen);

    m_plotter->addGraph();
    graphPen.setColor(Qt::blue);
    m_plotter->graph(1)->setLineStyle(QCPGraph::lsLine);
    m_plotter->graph(1)->setName("Спрос");
    m_plotter->graph(1)->setPen(graphPen);

    m_plotter->addGraph();
    graphPen.setColor(Qt::red);
    m_plotter->graph(2)->setLineStyle(QCPGraph::lsLine);
    m_plotter->graph(2)->setName("Предложение");
    m_plotter->graph(2)->setPen(graphPen);
}
