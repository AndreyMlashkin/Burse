#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class TransactionProcessor;
class Transaction;

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

private:
    Ui::MainWindow *m_ui;
    TransactionProcessor* m_processor;
};

#endif // MAINWINDOW_H
