#pragma once
#include <stdint.h>
#include <map>
#include <QDate>

class BudgetTable
{
    using Money = uint32_t;
     
public:
    void Add( const int nYear, const uint32_t uMonth, const Money uMoney );
    Money QueryBetween( const QDate& d1, const QDate& d2 ) const;

private:
    Money GetBudgetInOneYear( const QDate &d1, const QDate &d2 ) const;
    Money GetBudgetInOneMonth( const QDate& kMonth, const int nDayCount ) const;
    Money GetMonthlyBudget( const int nYear, const uint32_t uMonth ) const;

    static QDate CreateDate( const int nYear, const uint32_t uMonth ) { return QDate( nYear, uMonth, 1 ); }
    static QDate CreateYearStart( const int nYear ) { return QDate( nYear, 1, 1 ); }
    static QDate CreateYearEnd( const int nYear ) { return QDate( nYear, 12, 31 ); }

private:
    std::map< QDate, Money > m_kBudgets;
};