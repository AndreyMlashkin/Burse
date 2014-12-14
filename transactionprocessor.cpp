#include <QDebug>
#include <QtAlgorithms>

#include "transactionprocessor.h"
#include "transaction.h"

#include "transactionqueuemodel.h"

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
    bool (*comparator)(const Transaction&, const Transaction&);

    if(_transaction->type() == Transaction::Buy)
    {
        queueToProcess = &m_sell;
        comparator = lessOrEqual;
    }
    else if(_transaction->type() == Transaction::Sell)
    {
        queueToProcess = &m_buy;
        comparator = greaterOrEqual;
    }

    else if(_transaction->type() == Transaction::Invalid)
    {
        qDebug() << "Invalid transaction passed in " << Q_FUNC_INFO;
        return;
    }

    while(!queueToProcess->isEmpty() && comparator(*queueToProcess->first(), *_transaction))
    {
        qreal transationVolume = _transaction->volume();
        qreal topQueueVolume = queueToProcess->first()->volume();

        if(transationVolume >= topQueueVolume)
        {
            _transaction->setVolume(transationVolume - topQueueVolume);
            deleteTransaction(queueToProcess->first());
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
    else
        deleteTransaction(_transaction);

    if(m_buyModel)
        m_buyModel->update(m_buy);
    if(m_sellModel)
        m_sellModel->update(m_sell);
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

QAbstractItemModel *TransactionProcessor::buyModel()
{
    if(!m_buyModel)
        m_buyModel = new TransactionQueueModel(m_buy);

    return m_buyModel;
}

QAbstractItemModel *TransactionProcessor::sellModel()
{
    if(!m_sellModel)
        m_sellModel = new TransactionQueueModel(m_sell);

    return m_sellModel;
}

void TransactionProcessor::insertInSortedQueue(Transaction *_transaction)
{
    // It can be optimised
    int i = 0;
    if(_transaction->type() == Transaction::Buy)
    {
        while(m_buy.size() > i && m_buy.at(i)->cost() > _transaction->cost())
            ++i;
        m_buy.insert(i, _transaction);
    }
    else if(_transaction->type() == Transaction::Sell)
    {
        while(m_sell.size() > i && m_sell.at(i)->cost() < _transaction->cost())
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
