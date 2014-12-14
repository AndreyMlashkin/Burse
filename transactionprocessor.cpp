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
    foreach(Transaction* t, m_log)
        delete t;
}

void TransactionProcessor::insert(Transaction *_transaction)
{
    if(_transaction->type() == Transaction::Buy)
    {
        int i = 0;
        while(i < m_buy.size() && _transaction->cost() < m_buy[i]->cost())
            ++i;
        m_buy.insert(i, _transaction);
    }
    else if(_transaction->type() == Transaction::Sell)
    {
        int i = 0;
        while(i > m_sell.size() && _transaction->cost() > m_sell[i]->cost())
            ++i;
        m_sell.insert(i, _transaction);
    }
    else
        qDebug() << "Invalid Transaction passed in model!" << Q_FUNC_INFO;

    updateModels();
}

void TransactionProcessor::process()
{
    while(!m_buy.isEmpty() && !m_sell.isEmpty())
    {
        Transaction* buy = m_buy.first();
        Transaction* sell = m_sell.first();

        if(buy->cost() < sell->cost())
            break;

        qreal dealCost = (buy->time() < sell->time()) ? buy->cost() : sell->cost();

        Transaction* forHistory = new Transaction(dealCost, qMin(buy->volume(), sell->volume()));
        m_log << forHistory;

        if(buy->volume() < sell->volume())
        {
            sell->setVolume(sell->volume() - buy->volume());
            m_buy.removeFirst();
            delete buy;
        }
        else if(buy->volume() > sell->volume())
        {
            buy->setVolume(buy->volume() - sell->volume());
            m_sell.removeFirst();
            delete sell;
        }
        else // (buy->volume() == sell->volume())
        {
            m_sell.removeFirst();
            m_buy.removeFirst();
            delete buy;
            delete sell;
        }
    }
    updateModels();
    debugReport();
}

qreal TransactionProcessor::currentDemand() const
{
    if(m_buy.isEmpty())
        return 0;

    if(m_sell.isEmpty())
        return m_buy.first()->cost();

    return qMax(m_sell.first()->cost(), m_buy.first()->cost());
}

qreal TransactionProcessor::currentOffer() const
{
    if(m_sell.isEmpty())
        return 0;

    if(m_buy.isEmpty())
        return m_sell.first()->cost();

    return qMin(m_sell.first()->cost(), m_buy.first()->cost());
}

qreal TransactionProcessor::currentPrice() const
{
    if(m_log.isEmpty())
        return 0;

    return m_log.first()->cost();
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

QAbstractItemModel *TransactionProcessor::logModel()
{
    if(!m_logModel)
        m_logModel = new TransactionQueueModel(m_sell);

    return m_logModel;
}

void TransactionProcessor::updateModels()
{
    if(m_buyModel)
        m_buyModel->update(m_buy);
    if(m_sellModel)
        m_sellModel->update(m_sell);
    if(m_logModel)
        m_logModel->update(m_log);
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
