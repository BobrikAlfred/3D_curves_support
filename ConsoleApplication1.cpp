#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include <random>
#include <cmath>
#include <string>


struct Point
{
    double x, y, z;
public:
    Point(double x, double y, double z) : x(x), y(y), z(z) {};
    std::string toString()
    {
        return "(" + std::to_string(static_cast<int>(x)) + ";" + std::to_string(static_cast<int>(y)) + ";" + std::to_string(static_cast<int>(z)) + ")";
    }
};
class Circle
{
public:
    std::string name = "";
    double rX = 0;
    Circle(double r, std::string s) : rX(r), name(s) {};

    virtual Point getPoint(double t)
    {
        return Point(rX * std::cos(t), rX * std::sin(t), 0);
    }
    virtual Point getDer(double t)
    {
        return Point(-rX * std::sin(t), rX * std::cos(t), 0);;
    }
    static bool compareByRX(const std::unique_ptr<Circle>& a, const std::unique_ptr<Circle>& b) {
        return a->rX < b->rX;
    }
};
class Ellipse : public Circle
{
public:
    double rY = 0;
    Ellipse(double X, double Y, std::string s) : Circle(X, s), rY(Y) {};
    Point getPoint(double t)
    {
        return Point(rX * std::cos(t), rY * std::sin(t), 0);
    }
    Point getDer(double t)
    {
        return Point(-rX * std::sin(t), rY * std::cos(t), 0);
    }
};
class Helix : public Ellipse
{
public:
    double step = 0;
    Helix(double X, double Y, double Z, std::string s) : Ellipse(X, Y, s), step(Z) {};
    Point getPoint(double t)
    {
        return Point(rX * std::cos(t), rY * std::sin(t), step);
    }
    Point getDer(double t)
    {
        return Point(-rX * std::sin(t), rY * std::cos(t), step);
    }
};
int main()
{
    double count = 0;
    // Создание генератора случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 100.0);
    std::vector<std::unique_ptr<Circle>> curves;
    for (int i = 0; i < 10; i++)
    {
        if (dist(gen) < 33)
        {
            curves.push_back(std::make_unique<Circle>(dist(gen), "Circle"));
        }
        else if (dist(gen) < 66)
        {
            curves.push_back(std::make_unique<Ellipse>(dist(gen), dist(gen), "Ellipse"));
        }
        else
        {
            curves.push_back(std::make_unique<Helix>(dist(gen), dist(gen), dist(gen) / 10, "Helix"));
        }
    }
    for (const auto& ptr : curves)
    {
        Circle& c = *ptr;
        std::cout << c.name << " Point coordinate:" << c.getPoint(M_PI_4).toString() << "." << "Derivative:" << c.getDer(M_PI_4).toString() << "." << std::endl;
    }
    std::cout << std::endl;
    std::vector<std::unique_ptr<Circle>> circles;
    for (const auto& ptr : curves)
    {
        Circle& c = *ptr;
        if (typeid(c) == typeid(Circle)) {
            circles.push_back(std::make_unique<Circle>(c));
        }
    }
    std::sort(circles.begin(),circles.end(), Circle::compareByRX);
    
    for (const auto& ptr : circles)
    {
        Circle& c = *ptr;
        count += static_cast<int>(c.rX);
        std::cout << c.name << " Radius: " << static_cast<int>(c.rX) << std::endl;
    }
    std::cout << "\nSum of radius: " << static_cast<int>(count) << "\n";
}