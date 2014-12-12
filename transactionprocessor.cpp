#include <QDebug>

#include "transactionprocessor.h"
#include "transaction.h"

TransactionProcessor::TransactionProcessor()
{
}

TransactionProcessor::~TransactionProcessor()
{
    foreach(Transaction* t, m_buy)
        delete t;
    foreach(Transaction* t, m_sell)
        delete t;
}

void TransactionProcessor::process(Transaction *_transaction)
{
    QList<Transaction*> *queueToProcess;
    QList<Transaction*> *queueToStore;

    if(_transaction->type() == Transaction::Buy)
    {
        queueToProcess = &m_sell;
        queueToStore = &m_buy;
    }
    else if(_transaction->type() == Transaction::Sell)
    {
        queueToProcess = &m_buy;
        queueToStore = &m_sell;
    }
    else if(_transaction->type() == Transaction::Invalid)
    {
        qDebug() << "Invalid transaction passed in " << Q_FUNC_INFO;
        return;
    }

    // !!! It seems, that comparator function need to compare in case of buy and sell. !!!
    while(!queueToProcess->isEmpty() && queueToProcess->first()->cost() < _transaction->cost())
    {
        qreal transationVolume = _transaction->volume();
        qreal topQueueVolume = queueToProcess->first()->volume();

        if(transationVolume >= topQueueVolume)
        {
            _transaction->setVolume(transationVolume - topQueueVolume);
            delete queueToProcess->first();
            queueToProcess->removeFirst();
        }
        else
        {
            _transaction->setVolume(0);
            queueToProcess->first()->setVolume(topQueueVolume - transationVolume);
        }
    }

    Q_ASSERT(_transaction->volume() >= 0);
    Q_ASSERT(queueToProcess->first()->volume() >= 0);

    if(_transaction->volume())
        *queueToStore << _transaction;
    else
        delete _transaction;
}

qreal TransactionProcessor::currentDemand() const
{
    if(m_buy.isEmpty())
        return 0;

    return m_buy.first()->cost();
}

qreal TransactionProcessor::currentOffer() const
{
    if(m_sell.isEmpty())
        return 0;

    return m_sell.first()->cost();
}

//QList<Transaction *> &TransactionProcessor::oppositeQueue(int _transactionType)
//{
//    if(_transactionType == Transaction::Buy)
//        return m_sell;
//    else if(_transactionType == Transaction::Sell)
//        return m_buy;
//    else if(_transactionType == Transaction::Invalid)
//        Q_ASSERT(false);
//}
