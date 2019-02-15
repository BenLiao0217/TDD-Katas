#include <gmock/gmock.h>
#include <QDate>
#include "Module/Module.h"

using namespace testing;

TEST( QDateTest, HowToUseIt )
{
    // 不會計入 end 那天，是一個 [) 的概念
    QDate d1( 2019, 2, 27 );
    QDate d2( 2019, 2, 28 );
    ASSERT_THAT( d1.daysTo( d2 ), Eq( 1 ) );

    // 有正確處理閏年
    int oneLeapYear = 2020;
    QDate d3( oneLeapYear, 2, 28 );
    QDate d4( oneLeapYear, 3, 1 );
    ASSERT_THAT( d3.daysTo( d4 ), Eq( 2 ) );
    ASSERT_THAT( QDate::isLeapYear( oneLeapYear ), Eq( true ) );
}

const uint32_t arbitraryYear = 1000;

class EmptyBudget : public Test
{
public:
    BudgetTable bt;
};

// one day
TEST_F( EmptyBudget, ReturnsZero )
{
    QDate d1( arbitraryYear, 1, 1 );
    QDate d2( arbitraryYear, 1, 1 );;
    ASSERT_THAT( bt.QueryBetween( d1, d2 ), Eq( 0 ) );
}

class OneMonthBudget : public EmptyBudget
{
protected:
    void SetUp() override
    {
        bt.Add( arbitraryYear, 1, 31 );
    }
};
TEST_F( OneMonthBudget, OneDay )
{
    QDate d1( arbitraryYear, 1, 1 );
    QDate d2( arbitraryYear, 1, 1 );;
    ASSERT_THAT( bt.QueryBetween( d1, d2 ), Eq( 1 ) );
}

// two days
TEST_F( OneMonthBudget, TwoDays )
{
    QDate d1( arbitraryYear, 1, 1 );
    QDate d2( arbitraryYear, 1, 2 );
    ASSERT_THAT( bt.QueryBetween( d1, d2 ), Eq( 2 ) );
}

// whole month
TEST_F( OneMonthBudget, WholeMonth )
{
    QDate d1( arbitraryYear, 1, 1 );
    QDate d2( arbitraryYear, 1, 31 );
    ASSERT_THAT( bt.QueryBetween( d1, d2 ), Eq( 31 ) );
}

TEST_F( OneMonthBudget, TwoMonth )
{
    QDate d1( arbitraryYear, 1, 1 );
    QDate d2( arbitraryYear, 2, 28 );
    ASSERT_THAT( bt.QueryBetween( d1, d2 ), Eq( 31 ) );
}

class TwoMonthBudget : public OneMonthBudget
{
protected:
    void SetUp() override
    {
        OneMonthBudget::SetUp();
        bt.Add( arbitraryYear, 2, 28 );
    }
};

TEST_F( TwoMonthBudget, TwoMonth )
{
    QDate d1( arbitraryYear, 1, 1 );
    QDate d2( arbitraryYear, 2, 28 );
    ASSERT_THAT( bt.QueryBetween( d1, d2 ), Eq( 31 + 28 ) );
}

TEST_F( TwoMonthBudget, T2 )
{
    QDate d1( arbitraryYear, 1, 28 );
    QDate d2( arbitraryYear, 2, 2 );
    ASSERT_THAT( bt.QueryBetween( d1, d2 ), Eq( 4 + 2 ) );
}

class ThreeMonthBudget : public TwoMonthBudget
{
protected:
    void SetUp() override
    {
        TwoMonthBudget::SetUp();
        bt.Add( arbitraryYear, 3, 31 );
    }
};

TEST_F( ThreeMonthBudget, T1 )
{
    QDate d1( arbitraryYear, 1, 31 );
    QDate d2( arbitraryYear, 3, 1 );
    ASSERT_THAT( bt.QueryBetween( d1, d2 ), Eq( 1+28+1 ) );
}

class Discotinus2Month : public OneMonthBudget
{
protected:
    void SetUp() override
    {
        OneMonthBudget::SetUp();
        bt.Add( arbitraryYear, 5, 31 );
    }
};

TEST_F( Discotinus2Month, T1 )
{
    QDate d1( arbitraryYear, 1, 31 );
    QDate d2( arbitraryYear, 3, 1 );
    ASSERT_THAT( bt.QueryBetween( d1, d2 ), Eq( 1 ) );
}

class JumpOneYear : public OneMonthBudget
{
protected:
    void SetUp() override
    {
        OneMonthBudget::SetUp();
        bt.Add( 1001, 1, 31 );
    }
};

TEST_F( JumpOneYear, T1 )
{
    QDate d1( arbitraryYear, 1, 31 );
    QDate d2( 1001, 3, 1 );
    ASSERT_THAT( bt.QueryBetween( d1, d2 ), Eq( 1 + 31 ) );
}