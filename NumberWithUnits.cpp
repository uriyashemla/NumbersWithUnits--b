#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "NumberWithUnits.hpp"
using namespace std;

static vector<pair<string, string>> matches;

namespace ariel
{
    map<string, map<string, double>> NumberWithUnits::data = {};

    NumberWithUnits::NumberWithUnits(double num, const std::string &unit)
    {
        if (data.count(unit) == 0)
        {
            throw invalid_argument{"Unit isn't legal!"};
        }
        this->_num = num;
        this->_unit = unit;
    }

    void NumberWithUnits::read_units(ifstream &units_file)
    {
        string eq;
        string unit1;
        string unit2;
        double num1 = 0;
        double num2 = 0;
        while (units_file >> num1 >> unit1 >> eq >> num2 >> unit2)
        {
            // adding to the map
            data[unit1][unit2] = num2;
            data[unit2][unit1] = 1 / num2;

            //adding to the vector
            matches.emplace_back(unit1, unit2);
            matches.emplace_back(unit2, unit1);

            // check if theres more units that match
            NumberWithUnits::SharingConvertion();
        }
    }

    void NumberWithUnits::SharingConvertion()
    {
        for (auto &pair : matches)
        {
            string unit1 = pair.first;
            string unit2 = pair.second;
            for (auto &current : NumberWithUnits::data[unit2])
            {
                string unit3 = current.first;
                if (unit3 == unit1)
                {
                    continue;
                }
                NumberWithUnits::data[unit1][unit3] = NumberWithUnits::data[unit2][unit3] * NumberWithUnits::data[unit1][unit2];
                NumberWithUnits::data[unit3][unit1] = 1 / NumberWithUnits::data[unit1][unit3];
            }
        }
    }

    // Convert the first unit like the second one
    double NumberWithUnits::Convertor(double num, const string &unit1, const string &unit2)
    {
        if (unit1 == unit2)
        {
            return num;
        }
        try
        {
            double con = NumberWithUnits::data.at(unit1).at(unit2);
            return (num * con);
        }
        catch (const out_of_range &e)
        {
            throw invalid_argument{"Units doesn't match - [" + unit1 + "] cannot be converted to [" + unit2 + "]"};
        }
    }

    NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &n)
    {
        double converted = NumberWithUnits::Convertor(n._num, n._unit, this->_unit);
        return NumberWithUnits(this->_num + converted, this->_unit);
    }

    NumberWithUnits NumberWithUnits::operator+()
    {
        return NumberWithUnits(+this->_num, this->_unit);
    }

    NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &n)
    {

        this->_num += NumberWithUnits::Convertor(n._num, n._unit, this->_unit);
        return *this;
    }

    NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &n)
    {
        double converted = NumberWithUnits::Convertor(n._num, n._unit, this->_unit);
        return NumberWithUnits(this->_num - converted, this->_unit);
    }

    NumberWithUnits NumberWithUnits::operator-()
    {
        return NumberWithUnits(-this->_num, this->_unit);
    }

    NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &n)
    {
        this->_num -= NumberWithUnits::Convertor(n._num, n._unit, this->_unit);
        return *this;
    }

    int compare(const NumberWithUnits &n1, const NumberWithUnits &n2)
    {
        double n2_as_unit1 = NumberWithUnits::Convertor(n2._num, n2._unit, n1._unit);
        int ans;
        if (n2_as_unit1 - n1._num > NumberWithUnits::Epsilon)
        {
            ans = -1;
        }
        else if (n1._num - n2_as_unit1 > NumberWithUnits::Epsilon)
        {
            ans = 1;
        }
        else
        {
            ans = 0;
        }
        return ans;
    }

    bool operator>(const NumberWithUnits &n1, const NumberWithUnits &n2)
    {
        return !(n1 <= n2);
    }

    bool operator>=(const NumberWithUnits &n1, const NumberWithUnits &n2)
    {
        return (compare(n1, n2) >= 0);
    }

    bool operator<(const NumberWithUnits &n1, const NumberWithUnits &n2)
    {
        return !(n1 >= n2);
    }

    bool operator<=(const NumberWithUnits &n1, const NumberWithUnits &n2)
    {
        return (compare(n1, n2) <= 0);
    }

    bool operator==(const NumberWithUnits &n1, const NumberWithUnits &n2)
    {
        return (compare(n1, n2) == 0);
    }

    bool operator!=(const NumberWithUnits &n1, const NumberWithUnits &n2)
    {
        return !(n1 == n2);
    }

    // postfix incrment
    NumberWithUnits NumberWithUnits::operator++(int)
    {
        return NumberWithUnits((this->_num)++, this->_unit);
    }

    // prefix increment
    NumberWithUnits &NumberWithUnits::operator++()
    {
        ++(this->_num);
        return *this;
    }

    // postfix decrement
    NumberWithUnits NumberWithUnits::operator--(int)
    {
        return NumberWithUnits((this->_num)--, this->_unit);
    }

    // prefix increment
    NumberWithUnits &NumberWithUnits::operator--()
    {
        --(this->_num);
        return *this;
    }

    NumberWithUnits operator*(const NumberWithUnits &n, double num)
    {
        return NumberWithUnits(n._num * num, n._unit);
    }

    NumberWithUnits operator*(double num, const NumberWithUnits &n)
    {
        return NumberWithUnits(n._num * num, n._unit);
    }

    NumberWithUnits &NumberWithUnits::operator*=(double num)
    {
        this->_num *= num;
        return *this;
    }

    ostream &operator<<(ostream &out, const NumberWithUnits &n)
    {
        out << n._num << "[" << n._unit << "]";
        return out;
    }

    istream &operator>>(istream &in, NumberWithUnits &n)
    {

        string unit = " ";
        double num = 0;

        in >> skipws >> num >> skipws >> unit;

        while (!unit.empty() && unit.at(unit.length() - 1) != ']')
        {
            string s;
            in >> s;
            unit += s;
        }

        unit.erase(remove(unit.begin(), unit.end(), '['), unit.end());
        unit.erase(remove(unit.begin(), unit.end(), ']'), unit.end());

        if (NumberWithUnits::data.count(unit) != 0)
        {
            n._unit = unit;
            n._num = num;
            return in;
        }
        else
        {
            throw invalid_argument("There's no unit that match!");
        }
    }
}