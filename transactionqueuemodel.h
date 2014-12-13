#ifndef TRANSACTIONQUEUEMODEL_H
#define TRANSACTIONQUEUEMODEL_H

#include <QList>
#include <QAbstractTableModel>

class Transaction;

class TransactionQueueModel : public QAbstractTableModel
{
public:
    TransactionQueueModel(const QList<Transaction*> &_buy, const QList<Transaction*> &_sell);
    ~TransactionQueueModel();

    int rowCount(const QModelIndex &_parent) const;
    int columnCount(const QModelIndex &_parent) const;
    QVariant data(const QModelIndex &_index, int _role) const;

    void update(const QList<Transaction*> &_buy, const QList<Transaction*> &_sell);

private:
    // It's not safe!
    QList<Transaction*> m_sell;
    QList<Transaction*> m_buy;
};

#endif // TRANSACTIONQUEUEMODEL_H
