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
    m_modelTime(0),
    m_prevPos(0, 0, 0)
{    
    m_ui->setupUi(this);
    centralWidget()->layout()->addWidget(m_plotter);
    m_plotter->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    m_ui->buyTable->setModel(m_processor->buyModel());
    m_ui->sellTable->setModel(m_processor->sellModel());

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

void MainWindow::addPosition(Position _pos)
{
    // if new pos coord = 0, than take old val
    _pos.cost  = (_pos.cost   == 0)? m_prevPos.cost   : _pos.cost;
    _pos.demand= (_pos.demand == 0)? m_prevPos.demand : _pos.demand;
    _pos.offer = (_pos.offer  == 0)? m_prevPos.offer  : _pos.offer;
    m_prevPos = _pos;

//    m_plotter->graph(DealCost)->addData(m_modelTime, _pos.cost);
    qreal mid = (_pos.demand + _pos.offer) / 2;
    m_plotter->graph(MidCost)->addData(m_modelTime, mid);
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


    for(int i = 0; i < GraphicsCount; i++)
        m_plotter->addGraph();

    QPen graphPen;
    graphPen.setColor(Qt::black);
    graphPen.setWidthF(5);

    m_plotter->graph(MidCost)->setLineStyle(QCPGraph::lsLine);
    m_plotter->graph(MidCost)->setName("Цена (средняя)");
    m_plotter->graph(MidCost)->setPen(graphPen);

/*    graphPen.setStyle(Qt::SolidLine);
    m_plotter->graph(DealCost)->setLineStyle(QCPGraph::lsLine);
    m_plotter->graph(DealCost)->setName("Цена (сделки)");
    m_plotter->graph(DealCost)->setPen(graphPen);
    m_plotter->graph(DealCost)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 5));
*/
    graphPen.setColor(Qt::green);
    graphPen.setWidthF(2);
    m_plotter->graph(Demand)->setLineStyle(QCPGraph::lsLine);
    m_plotter->graph(Demand)->setName("Спрос");
    m_plotter->graph(Demand)->setPen(graphPen);

    graphPen.setColor(Qt::red);
    m_plotter->graph(Offer)->setLineStyle(QCPGraph::lsLine);
    m_plotter->graph(Offer)->setName("Предложение");
    m_plotter->graph(Offer)->setPen(graphPen);

    // point in time = 0
    Position pos(0, 0, 0);
    addPosition(pos);
}
