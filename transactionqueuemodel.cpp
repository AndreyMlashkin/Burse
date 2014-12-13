#include "transactionqueuemodel.h"
#include "transaction.h"

TransactionQueueModel::TransactionQueueModel(const QList<Transaction *> &_buy, const QList<Transaction *> &_sell)
//    : m_sell(_sell),
//      m_buy(_buy)
{
    update(_buy, _sell);
}

TransactionQueueModel::~TransactionQueueModel()
{
    // !!! memory leak !!!
}

int TransactionQueueModel::rowCount(const QModelIndex &_parent) const
{
    if(!_parent.isValid())
        return -1;

    if(_parent.column() == 0)
        return m_sell.count();
    else if(_parent.column() == 1)
        return m_buy.count();
    else
        Q_ASSERT(false);
    return -1;
}

int TransactionQueueModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant TransactionQueueModel::data(const QModelIndex &_index, int _role) const
{
    if(_role != Qt::DisplayRole)
        return QVariant();

    const QList<Transaction*>* queue = NULL;
    if(_index.column() == 0)
        queue = &m_sell;
    else if(_index.column() == 1)
        queue = &m_buy;

    QString ans = QString::number(queue->at(_index.row())->cost()) +
                  QString::number(queue->at(_index.row())->volume());

    return ans;

}

void TransactionQueueModel::update(const QList<Transaction *> &_buy, const QList<Transaction *> &_sell)
{
    beginResetModel();

    m_buy = _buy;
    m_sell = _sell;

    endResetModel();
}
