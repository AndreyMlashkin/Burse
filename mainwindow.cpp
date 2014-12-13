#include "qcustomplot/qcustomplot.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bursetools.h"
#include "transactionprocessor.h"
#include "transaction.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_processor(new TransactionProcessor()),
    m_plotter(new QCustomPlot()),
    m_minDemand(0),
    m_maxOffer(0),
    m_modelTime(0)
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
    const int offset = 3;
    ++m_modelTime;

    m_processor->process(formTransaction());

    Position currentPos(m_processor->currentPrice(),
                        m_processor->currentDemand(),
                        m_processor->currentOffer());
    addPosition(currentPos);


    m_minDemand = qMin(m_minDemand, m_processor->currentDemand());
    m_maxOffer =  qMax(m_maxOffer, m_processor->currentOffer());

    m_plotter->yAxis->setRange(0, m_maxOffer + offset);
    m_plotter->xAxis->setRange(0, m_modelTime + offset);
    m_plotter->replot();
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

void MainWindow::addPosition(const Position &_pos)
{
    m_plotter->graph(Cost)->addData(m_modelTime, _pos.cost);
    m_plotter->graph(Demand)->addData(m_modelTime, _pos.demand);
    m_plotter->graph(Offer)->addData(m_modelTime, _pos.offer);
}

void MainWindow::initGraphics()
{
    m_plotter->axisRect()->setupFullAxesBox();
    m_plotter->plotLayout()->insertRow(0);
    m_plotter->plotLayout()->addElement(0, 0, new QCPPlotTitle(m_plotter, tr("Торговая сессия")));

    m_plotter->xAxis->setLabel("Время");
    m_plotter->yAxis->setLabel("Цена");

    m_plotter->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    m_plotter->legend->setVisible(true);


    QPen graphPen;
    graphPen.setColor(Qt::black);
    graphPen.setWidthF(5);

    m_plotter->addGraph();
    m_plotter->graph(Cost)->setLineStyle(QCPGraph::lsLine);
    m_plotter->graph(Cost)->setName("Цена");
    m_plotter->graph(Cost)->setPen(graphPen);

    m_plotter->addGraph();
    graphPen.setColor(Qt::green);
    graphPen.setWidthF(2);
    m_plotter->graph(Demand)->setLineStyle(QCPGraph::lsLine);
    m_plotter->graph(Demand)->setName("Спрос");
    m_plotter->graph(Demand)->setPen(graphPen);

    m_plotter->addGraph();
    graphPen.setColor(Qt::red);
    m_plotter->graph(Offer)->setLineStyle(QCPGraph::lsLine);
    m_plotter->graph(Offer)->setName("Предложение");
    m_plotter->graph(Offer)->setPen(graphPen);

    // point in time = 0
    m_plotter->graph(Cost)->addData(0, 0);
    m_plotter->graph(Demand)->addData(0, 0);
    m_plotter->graph(Offer)->addData(0, 0);

}
