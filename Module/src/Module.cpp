#include "Module.h"
#include <cassert>

void BudgetTable::Add( const int nYear, const uint32_t uMonth, const BudgetTable::Money uMoney )
{
    const QDate d = CreateDate( nYear, uMonth );
    assert( uMoney % d.daysInMonth() == 0 );
    m_kBudgets.insert( std::make_pair( d, uMoney ) );
}

BudgetTable::Money BudgetTable::QueryBetween( const QDate& d1, const QDate& d2 ) const
{
    if ( d1.year() == d2.year() )
    {
        return GetBudgetInOneYear( d1, d2 );
    }
    else
    {
        int total = 0;

        // first year
        total += GetBudgetInOneYear( d1, CreateYearEnd( d1.year() ) );

        // intermediate years
        for ( int y = d1.year() + 1; y < d2.year(); ++y )
        {
            total += GetBudgetInOneYear( CreateYearStart( y ), CreateYearEnd( y ) );
        }

        // last year
        total += GetBudgetInOneYear( CreateYearStart( d2.year() ), d2 );
        return total;
    }
}

BudgetTable::Money BudgetTable::GetBudgetInOneYear( const QDate &d1, const QDate &d2 ) const
{
    if ( d1.year() != d2.year() )
    {
        assert( 0 );
        return -1;
    }
    if ( d1.month() == d2.month() )
    {
        return GetBudgetInOneMonth( d1, d1.daysTo( d2 ) + 1 );
    }

    int total = 0;

    const int nDaysInFirstMonth = d1.daysInMonth() - d1.day() + 1;
    total += GetBudgetInOneMonth( d1, nDaysInFirstMonth );

    for ( int m = d1.month() + 1; m < d2.month(); ++m )
    {
        QDate d = CreateDate( d1.year(), m );
        total += GetBudgetInOneMonth( d, d.daysInMonth() );
    }

    const int nDaysInLastMonth = d2.day();
    total += GetBudgetInOneMonth( d2, nDaysInLastMonth );

    return total;
}

BudgetTable::Money BudgetTable::GetBudgetInOneMonth( const QDate& kMonth, const int nDayCount ) const
{
    return GetMonthlyBudget( kMonth.year(), kMonth.month() ) / kMonth.daysInMonth() * nDayCount;
}

BudgetTable::Money BudgetTable::GetMonthlyBudget( const int nYear, const uint32_t uMonth ) const
{
    const QDate d = CreateDate( nYear, uMonth );
    auto it = m_kBudgets.find( d );
    if ( it != m_kBudgets.end() )
    {
        return ( *it ).second;
    }

    return 0;
}
