#ifndef TRANSACTIONPROCESSOR_H
#define TRANSACTIONPROCESSOR_H

#include <QList>
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

private:
    QList<Transaction*> m_buy;
    QList<Transaction*> m_sell;

    qreal m_lastDealCost;
};

#endif // TRANSACTIONPROCESSOR_H
