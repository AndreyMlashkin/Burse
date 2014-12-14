#include <QDebug>

#include "transactionqueuemodel.h"
#include "transaction.h"

TransactionQueueModel::TransactionQueueModel(const QList<Transaction *> &_transactions)
{
    update(_transactions);
}

TransactionQueueModel::~TransactionQueueModel()
{
    // !!! memory leak !!!
}

int TransactionQueueModel::rowCount(const QModelIndex &_parent) const
{
    Q_UNUSED(_parent);
    return m_transactions.size();
}

int TransactionQueueModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant TransactionQueueModel::data(const QModelIndex &_index, int _role) const
{
    if(_role != Qt::DisplayRole)
        return QVariant();

    const Transaction* t = m_transactions.at(_index.row());

    if(_index.column() == 0)
        return t->cost();
    else
        return t->volume();
}

QVariant TransactionQueueModel::headerData(int _section, Qt::Orientation _orientation, int _role) const
{
    if(_orientation == Qt::Horizontal && _role == Qt::DisplayRole)
        return (_section == 0)? "Цена" : "Объём";

    return QVariant();
}

void TransactionQueueModel::update(const QList<Transaction*> &_transactions)
{
    beginResetModel();
    m_transactions = _transactions;
    endResetModel();
}
