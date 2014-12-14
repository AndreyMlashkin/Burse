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

    void process(Transaction* _transaction);

    qreal currentDemand() const;
    qreal currentOffer() const;
    inline qreal currentPrice() const {return m_lastDealCost;}

    QAbstractItemModel *buyModel();
    QAbstractItemModel *sellModel();

private:
    void insertInSortedQueue(Transaction* _transaction);
    void deleteTransaction(Transaction* _transaction);
    void debugReport() const;

private:
    QList<Transaction*> m_buy;
    QList<Transaction*> m_sell;
    qreal m_lastDealCost;

    QPointer <TransactionQueueModel> m_buyModel;
    QPointer <TransactionQueueModel> m_sellModel;
};

#endif // TRANSACTIONPROCESSOR_H
