#pragma once

#include <string>
#include <utility>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace ariel
{
    class NumberWithUnits
    {
        static void SharingConvertion();
        static double Convertor(double number, const std::string &unit1, const std::string &unit2);
        double _num;
        std::string _unit;
        static constexpr double Epsilon = 0.0001;

    public:
        static std::map<std::string, std::map<std::string, double>> data;
        NumberWithUnits(double num, const std::string &unit);

        static void read_units(std::ifstream &units_file);

        NumberWithUnits operator+(const NumberWithUnits &n);
        NumberWithUnits operator+();
        NumberWithUnits &operator+=(const NumberWithUnits &n);
        NumberWithUnits operator-(const NumberWithUnits &n);
        NumberWithUnits operator-();
        NumberWithUnits &operator-=(const NumberWithUnits &n);
        friend int compare(const NumberWithUnits &n1, const NumberWithUnits &n2);
        friend bool operator>(const NumberWithUnits &n1, const NumberWithUnits &n2);
        friend bool operator>=(const NumberWithUnits &n1, const NumberWithUnits &n2);
        friend bool operator<(const NumberWithUnits &n1, const NumberWithUnits &n2);
        friend bool operator<=(const NumberWithUnits &n1, const NumberWithUnits &n2);
        friend bool operator==(const NumberWithUnits &n1, const NumberWithUnits &n2);
        friend bool operator!=(const NumberWithUnits &n1, const NumberWithUnits &n2);
        NumberWithUnits operator++(int);
        NumberWithUnits &operator++();
        NumberWithUnits operator--(int);
        NumberWithUnits &operator--();
        friend NumberWithUnits operator*(const NumberWithUnits &n, double num);
        friend NumberWithUnits operator*(double num, const NumberWithUnits &n);
        NumberWithUnits &operator*=(double num);
        friend std::ostream &operator<<(std::ostream &out, const NumberWithUnits &n);
        friend std::istream &operator>>(std::istream &in, NumberWithUnits &n);
    };
}