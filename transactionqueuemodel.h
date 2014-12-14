#ifndef TRANSACTIONQUEUEMODEL_H
#define TRANSACTIONQUEUEMODEL_H

#include <QList>
#include <QAbstractTableModel>

class Transaction;

class TransactionQueueModel : public QAbstractTableModel
{
public:
    TransactionQueueModel(const QList<Transaction*> &_transactions);
    ~TransactionQueueModel();

    int rowCount(const QModelIndex &_parent) const;
    int columnCount(const QModelIndex &_parent) const;

    QVariant data(const QModelIndex &_index, int _role) const;
    QVariant headerData(int _section, Qt::Orientation _orientation, int _role = Qt::DisplayRole) const;

    void update(const QList<Transaction*> &_transactions);

private:
    QList<Transaction*> m_transactions;
};

#endif // TRANSACTIONQUEUEMODEL_H
