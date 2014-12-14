#include "transaction.h"

Transaction::Transaction()
    : m_cost(-1),
      m_volume(-1),
      m_type(Invalid)
{}

Transaction::Transaction(qreal _cost, qreal _volume, int _time, Type _type)
    : m_cost(_cost),
      m_volume(_volume),
      m_time(_time),
      m_type(_type)
{}

bool Transaction::isValid() const
{
    bool invalid = m_type == Invalid || m_cost < 0 || m_volume < 0;
    return !invalid;
}

bool Transaction::operator <(const Transaction& _tr) const
{
    return m_cost < _tr.cost();
}

bool Transaction::operator >(const Transaction &_tr) const
{
    return m_cost > _tr.cost();
}


bool lessOrEqual(const Transaction &_tr1, const Transaction &_tr2)
{
    return _tr1.cost() <= _tr2.cost();
}


bool greaterOrEqual(const Transaction &_tr1, const Transaction &_tr2)
{
    return _tr1.cost() >= _tr2.cost();
}
