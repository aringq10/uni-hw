#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cassert>
#include <vector>

class Dish
{
private:
    static int nextId;
    static int objectCount;

    int id;
    std::string name;
    int calories;
    double protein;
    double carbs;
    double fat;

public:
    Dish(std::string name, int calories)
        : id(nextId++), name(""), calories(0),
          protein(0.0), carbs(0.0), fat(0.0)
    {
        setName(name);
        setCalories(calories);
        objectCount++;
    }

    Dish(std::string name, int calories,
         double protein, double carbs, double fat)
        : id(nextId++), name(""), calories(0),
          protein(0.0), carbs(0.0), fat(0.0)
    {
        setName(name);
        setCalories(calories);
        setProtein(protein);
        setCarbs(carbs);
        setFat(fat);
        objectCount++;
    }

    ~Dish()
    {
        objectCount--;
    }

    int getId()
    {
        return id;
    }

    std::string getName()
    {
        return name;
    }

    int getCalories()
    {
        return calories;
    }

    double getProtein()
    {
        return protein;
    }

    double getCarbs()
    {
        return carbs;
    }

    double getFat()
    {
        return fat;
    }

    static int getObjectCount()
    {
        return objectCount;
    }

    void setName(std::string name)
    {
        if (name.empty())
        {
            throw std::invalid_argument("Name cannot be empty.");
        }
        this->name = name;
    }

    void setCalories(int calories)
    {
        if (calories < 0)
        {
            throw std::invalid_argument("Calories cannot be negative.");
        }
        this->calories = calories;
    }

    void setProtein(double protein)
    {
        if (protein < 0)
        {
            throw std::invalid_argument("Protein cannot be negative.");
        }
        this->protein = protein;
    }

    void setCarbs(double carbs)
    {
        if (carbs < 0)
        {
            throw std::invalid_argument("Carbs cannot be negative.");
        }
        this->carbs = carbs;
    }

    void setFat(double fat)
    {
        if (fat < 0)
        {
            throw std::invalid_argument("Fat cannot be negative.");
        }
        this->fat = fat;
    }

    std::string toString()
    {
        std::stringstream ss;
        ss << "Dish \"" << name
           << "\", ID=" << id
           << ", Calories=" << calories
           << ", Protein=" << protein
           << "g, Carbs=" << carbs
           << "g, Fat=" << fat
           << "g";
        return ss.str();
    }
};

int Dish::nextId = 1;
int Dish::objectCount = 0;

int main()
{
    try
    {
        assert(Dish::getObjectCount() == 0);

        // =========================
        // TEST 1
        // Create object, test getters and toString
        // =========================
        Dish d1("Pasta", 500, 20.0, 60.0, 10.0);

        assert(Dish::getObjectCount() == 1);

        assert(d1.getName() == "Pasta");
        assert(d1.getCalories() == 500);
        assert(d1.getProtein() == 20.0);
        assert(d1.getCarbs() == 60.0);
        assert(d1.getFat() == 10.0);

        std::string expected =
            "Dish \"Pasta\", ID=" + std::to_string(d1.getId()) +
            ", Calories=500, Protein=20g, Carbs=60g, Fat=10g";

        assert(d1.toString() == expected);

        // =========================
        // TEST 2
        // Test all setters
        // =========================
        d1.setName("Rice");
        d1.setCalories(300);
        d1.setProtein(8.0);
        d1.setCarbs(70.0);
        d1.setFat(2.0);

        assert(d1.getName() == "Rice");
        assert(d1.getCalories() == 300);
        assert(d1.getProtein() == 8.0);
        assert(d1.getCarbs() == 70.0);
        assert(d1.getFat() == 2.0);

        // =========================
        // TEST 3
        // Validation and exception checking
        // =========================
        bool exceptionThrown = false;

        try
        {
            d1.setCalories(-10);
        }
        catch (const std::invalid_argument&)
        {
            exceptionThrown = true;
        }
        assert(exceptionThrown);

        exceptionThrown = false;

        try
        {
            d1.setName("");
        }
        catch (const std::invalid_argument&)
        {
            exceptionThrown = true;
        }
        assert(exceptionThrown);

        // =========================
        // TEST 4
        // Automatic numbering (IDs must increase)
        // =========================
        Dish d2("Soup", 200);
        Dish d3("Salad", 150);

        assert(d2.getId() > d1.getId());
        assert(d3.getId() > d2.getId());

        // =========================
        // TEST 5
        // Dynamic allocation and object counting
        // =========================
        std::vector<Dish*> list;

        int initialCount = Dish::getObjectCount();

        for (int i = 0; i < 3; ++i)
        {
            list.push_back(new Dish("Dynamic", 100 + i));
        }

        assert(Dish::getObjectCount() == initialCount + 3);

        for (Dish* ptr : list)
        {
            delete ptr;
        }

        list.clear();

        assert(Dish::getObjectCount() == initialCount);
    }
    catch (...)
    {
        std::cerr << "Unexpected exception occurred.\n";
        return 1;
    }

    assert(Dish::getObjectCount() == 0);

    std::cout << "All tests passed successfully.\n";
    return 0;
}
