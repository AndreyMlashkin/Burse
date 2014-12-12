#include "transaction.h"

Transaction::Transaction()
    : m_cost(-1),
      m_volume(-1),
      m_type(Invalid)
{}

Transaction::Transaction(qreal _cost, qreal _volume, Type _type)
    : m_cost(_cost),
      m_volume(_volume),
      m_type(_type)
{}

bool Transaction::isValid() const
{
    bool invalid = m_type == Invalid || m_cost < 0 || m_volume < 0;
    return !invalid;
}
