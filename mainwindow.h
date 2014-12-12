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
    Transaction* formTransaction() const;

    void initGraphics();

private:
    Ui::MainWindow *m_ui;
    TransactionProcessor* m_processor;
    QCustomPlot* m_plotter;
};

#endif // MAINWINDOW_H
