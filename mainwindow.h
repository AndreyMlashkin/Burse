#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class TransactionProcessor;
class Transaction;
class QCustomPlot;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void addTransaction();

private:

    struct Position
    {
        Position(qreal _cost, qreal _demand, qreal _offer)
            : cost(_cost), demand(_demand), offer(_offer)
        {}
        qreal cost;
        qreal demand;
        qreal offer;
    };

    Transaction* formTransaction() const;
    void addPosition();
    void addPosition(Position _pos);
    void initGraphics();

private:
    Ui::MainWindow *m_ui;
    TransactionProcessor* m_processor;
    QCustomPlot* m_plotter;

    qreal m_minDemand;
    qreal m_maxOffer;
    Position m_prevPos;
    qreal m_modelTime;
};

#endif // MAINWINDOW_H
