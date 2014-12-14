#ifndef TRANSACTIONPROCESSOR_H
#define TRANSACTIONPROCESSOR_H

#include <QList>
#include <QPointer>

class QAbstractItemModel;
class TransactionQueueModel;
class Transaction;

class TransactionProcessor
{
public:
    TransactionProcessor();
    ~TransactionProcessor();

    void insert(Transaction* _transaction);
    void process();

    qreal currentDemand() const;
    qreal currentOffer() const;
    qreal currentPrice() const;

    QAbstractItemModel *buyModel();
    QAbstractItemModel *sellModel();
    QAbstractItemModel *logModel();

private:
    void updateModels();
    void debugReport() const;

private:
    QList<Transaction*> m_buy;
    QList<Transaction*> m_sell;
    QList<Transaction*> m_log;
    qreal m_lastDealCost;

    QPointer <TransactionQueueModel> m_buyModel;
    QPointer <TransactionQueueModel> m_sellModel;
    QPointer <TransactionQueueModel> m_logModel;
};

#endif // TRANSACTIONPROCESSOR_H
