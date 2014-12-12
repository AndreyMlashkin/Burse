#include <QDebug>
#include <QtAlgorithms>

#include "transactionprocessor.h"
#include "transaction.h"

TransactionProcessor::TransactionProcessor()
    : m_lastDealCost(0)
{}

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
//    QList<Transaction*> *queueToStore;

    if(_transaction->type() == Transaction::Buy)
    {
        queueToProcess = &m_sell;
//        queueToStore = &m_buy;
    }
    else if(_transaction->type() == Transaction::Sell)
    {
        queueToProcess = &m_buy;
//        queueToStore = &m_sell;
    }
    else if(_transaction->type() == Transaction::Invalid)
    {
        qDebug() << "Invalid transaction passed in " << Q_FUNC_INFO;
        return;
    }

    // !!! It seems, that comparator function need to compare in case of buy and sell. !!!
    while(!queueToProcess->isEmpty() && queueToProcess->first()->cost() <= _transaction->cost())
    {
        qreal transationVolume = _transaction->volume();
        qreal topQueueVolume = queueToProcess->first()->volume();

        if(transationVolume >= topQueueVolume)
        {
            _transaction->setVolume(transationVolume - topQueueVolume);
            deleteTransaction(queueToProcess->first());
            //delete queueToProcess->first();
            queueToProcess->removeFirst();
        }
        else
        {
            _transaction->setVolume(0);
            queueToProcess->first()->setVolume(topQueueVolume - transationVolume);
            break;
        }
    }

    Q_ASSERT(_transaction->volume() >= 0);
    if(!queueToProcess->isEmpty())
        Q_ASSERT(queueToProcess->first()->volume() >= 0);

    if(_transaction->volume())
        insertInSortedQueue(_transaction);
//       *queueToStore << _transaction;
    else
        deleteTransaction(_transaction);
//        delete _transaction;

    debugReport();
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

void TransactionProcessor::insertInSortedQueue(Transaction *_transaction)
{
    // It can be optimised
    int i = 0;
    if(_transaction->type() == Transaction::Buy)
    {
        if(!m_buy.isEmpty())
            while(m_buy.at(i)->cost() < _transaction->cost())
                ++i;
        m_buy.insert(i, _transaction);
    }
    else if(_transaction->type() == Transaction::Sell)
    {
        if(!m_sell.isEmpty())
            while(m_sell.at(i)->cost() > _transaction->cost())
                ++i;
        m_sell.insert(i, _transaction);
    }
}

void TransactionProcessor::deleteTransaction(Transaction *_transaction)
{
    m_lastDealCost = _transaction->cost();
}

void TransactionProcessor::debugReport() const
{
    qDebug() << "\ncurrent price =" << currentPrice()
             << "current demand =" << currentDemand()
             << "current offer =" << currentOffer();

    qDebug() << "buying:\n";
    foreach(Transaction* t, m_buy)
        qDebug() << t->cost() << t->volume();
    qDebug() << "selling:\n";
    foreach(Transaction* t, m_sell)
        qDebug() << t->cost() << t->volume();
}
