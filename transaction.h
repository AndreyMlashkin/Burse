#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QtGlobal>

class Transaction
{
public:
    enum Type {Invalid, Buy, Sell};
    Transaction();
    Transaction(qreal _cost, qreal _volume, int _time = 1, Type _type = Invalid);

    bool isValid() const;

    inline qreal cost()   const {return m_cost;}
    inline qreal volume() const {return m_volume;}
    inline int  time()    const {return m_time;}
    inline Type type()    const {return m_type;}

    inline void setCost(qreal _cost)     {m_cost = _cost;}
    inline void setVolume(qreal _volume) {m_volume = _volume;}
    inline void setType(Type _type)      {m_type = _type;}

    bool operator < (const Transaction& _tr) const;
    bool operator > (const Transaction& _tr) const;

private:
    qreal m_cost;
    qreal m_volume;
    int m_time;
    Type m_type;
};

bool lessOrEqual(const Transaction& _tr1, const Transaction& _tr2);
bool greaterOrEqual(const Transaction& _tr1, const Transaction& _tr2);

#endif // TRANSACTION_H
