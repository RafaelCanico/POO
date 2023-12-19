#include <iostream>
#include <vector>
#include <map>
#include <string>

// Enumeração para os tipos de combustível
enum class FuelType { DIESEL, ELECTRIC, GASOLINE, LPG, HYBRID };

// Função auxiliar para converter FuelType para string
std::string fuelTypeToString(FuelType fuelType) {
    switch (fuelType) {
    case FuelType::DIESEL: return "DIESEL";
    case FuelType::ELECTRIC: return "ELECTRIC";
    case FuelType::GASOLINE: return "GASOLINE";
    case FuelType::LPG: return "LPG";
    case FuelType::HYBRID: return "HYBRID";
    default: return "UNKNOWN";
    }
}

// Parte I
class Vehicle {
private:
    static int idCounter; // Contador estático para gerar vehicleId
    int vehicleId;
    int licensePlateYear;
    std::string brand;
    FuelType fuelType;

    BEV* bevMotor = nullptr;
    ICE* iceMotor = nullptr;


public:
    // Construtor com argumentos (excluindo vehicleId)
    Vehicle(int year, std::string const& br, FuelType ft) : licensePlateYear(year), brand(br), fuelType(ft) {
        if (brand.empty() || licensePlateYear < 0) {
            throw std::invalid_argument("Invalid brand or year");
        }
        vehicleId = ++idCounter; // Incrementa e atribui o vehicleId
    }

    // Construtor sem argumentos
    Vehicle() : vehicleId(++idCounter), licensePlateYear(0), brand("Unknown"), fuelType(FuelType::GASOLINE) {}

    // Seletores (getters) e modificadores (setters)
    int getVehicleId() const { return vehicleId; }
    int getLicensePlateYear() const { return licensePlateYear; }
    void setLicensePlateYear(int year) {
        if (year < 0) throw std::invalid_argument("Year cannot be negative.");
        licensePlateYear = year;
    }
    std::string getBrand() const { return brand; }
    void setBrand(std::string const& br) {
        if (br.empty()) throw std::invalid_argument("Brand cannot be empty.");
        brand = br;
    }
    FuelType getFuelType() const { return fuelType; }
    void setFuelType(FuelType ft) { fuelType = ft; }

    // Método para imprimir o estado do veículo
    virtual std::string toString() const {
        return std::to_string(vehicleId) + " " + std::to_string(licensePlateYear) + " " + brand + " FuelType: " + fuelTypeToString(fuelType);
    }


    void setBEVMotor(BEV* bev) {
        delete bevMotor; // Limpa o motor anterior, se houver
        bevMotor = bev;
    }

    void setICEMotor(ICE* ice) {
        delete iceMotor; // Limpa o motor anterior, se houver
        iceMotor = ice;
    }

    double power() const {
        double totalPower = 0;
        if (bevMotor != nullptr) {
            totalPower += bevMotor->power(); // Potência do motor BEV
        }
        if (iceMotor != nullptr) {
            totalPower += iceMotor->power(); // Potência do motor ICE
        }
        return totalPower;
    }

};

int Vehicle::idCounter = 0; // Inicialização do contador estático de IDs

// Parte II
class Car : public Vehicle {
protected:
    int numPorts;

public:
    // Construtor com argumentos
    Car(int numP, int year, std::string const& br, FuelType ft) : Vehicle(year, br, ft), numPorts(numP) {}

    // Construtor sem argumentos
    Car() : numPorts(4) {} // Valor padrão para numPorts

    // Seletores e modificadores
    int getNumPorts() const { return numPorts; }
    void setNumPorts(int numP) { numPorts = numP; }

    // Método toString de Car
    std::string toString() const override {
        return Vehicle::toString() + " Ports: " + std::to_string(numPorts);
    }
};

class Coupe : public Car {
public:
    static const int NUMBER_OF_PORTS = 2;

    // Construtor sem argumentos
    Coupe() : Car(NUMBER_OF_PORTS, 0, "Unknown", FuelType::GASOLINE) {}

    // Construtor com argumentos
    Coupe(int year, std::string const& br, FuelType ft) : Car(NUMBER_OF_PORTS, year, br, ft) {}

    // Método toString de Coupe
    std::string toString() const override {
        return Vehicle::toString() + " Coupe";
    }
};

class Sedan : public Car {
public:
    static const int NUMBER_OF_PORTS = 4;

    // Construtor sem argumentos
    Sedan() : Car(NUMBER_OF_PORTS, 0, "Unknown", FuelType::GASOLINE) {}

    // Construtor com argumentos
    Sedan(int year, std::string const& br, FuelType ft) : Car(NUMBER_OF_PORTS, year, br, ft) {}

    // Método toString de Sedan
    std::string toString() const override {
        return Vehicle::toString() + " Sedan";
    }
};

// Parte III
class Stand {
private:
    std::string city;
    int number;
    std::string code;

public:
    // Construtor sem argumentos
    Stand() : city("Unknown"), number(0), code("Unknown00") {}

    // Construtor com argumentos
    Stand(std::string const& cty, int num) : city(cty), number(num) {
        if (number < 1 || number > 99) {
            throw std::invalid_argument("Number must be between 1 and 99.");
        }
        code = city + (number < 10 ? "0" : "") + std::to_string(number);
    }

    // Método toString
    std::string toString() const {
        return code;
    }

    // Sobrecarga do operador < para comparar objetos Stand
    bool operator<(const Stand& other) const {
        if (city == other.city) {
            return number < other.number;
        }
        return city < other.city;
    }
};

// Parte IV
class VehicleStand {
private:
    std::map<Stand, std::vector<Vehicle*>> stands;

public:
    // Construtor sem argumentos
    VehicleStand() {}

    // Método para adicionar e associar um veículo a um stand
    void addVehicle(Stand const& stand, Vehicle* vehicle) {
        stands[stand].push_back(vehicle);
    }

    // Método toString
    std::string toString() const {
        std::string result;
        for (auto const& entry : stands) {
            result += entry.first.toString() + ":\n";
            for (auto const& vehicle : entry.second) {
                result += "  " + vehicle->toString() + "\n";
            }
        }
        return result;
    }
};

// Parte V
class Motorcycle : public Vehicle {
private:
    bool hasFairing;

public:
    // Construtores
    Motorcycle() : hasFairing(false) {}
    Motorcycle(bool fairing, int year, std::string const& br, FuelType ft) : Vehicle(year, br, ft), hasFairing(fairing) {}

    // Seletores e modificadores
    bool getHasFairing() const { return hasFairing; }
    void setHasFairing(bool fairing) { hasFairing = fairing; }

    // Método toString
    std::string toString() const override {
        return Vehicle::toString() + (hasFairing ? " com carnagem" : "");
    }
};


// Parte VI
class VehicleFactory {
public:
    // Método para criar um Car
    static Car createCar(int numPorts, int year, const std::string& brand, FuelType fuelType) {
        return Car(numPorts, year, brand, fuelType);
    }

    // Método para criar um Coupe
    static Coupe createCoupe(int year, const std::string& brand, FuelType fuelType) {
        return Coupe(year, brand, fuelType);
    }

    // Método para criar um Sedan
    static Sedan createSedan(int year, const std::string& brand, FuelType fuelType) {
        return Sedan(year, brand, fuelType);
    }

    // Método para criar uma Motorcycle
    static Motorcycle createMotorcycle(bool hasFairing, int year, const std::string& brand, FuelType fuelType) {
        return Motorcycle(hasFairing, year, brand, fuelType);
    }
};





// Parte IX
class Motor {
public:
    virtual ~Motor() {}
    virtual double power() const = 0; // Método virtual puro para potência
};

class BEV : public Motor {
private:
    double capacity; // Capacidade da bateria em kWh
    double powerKW;  // Potência em kW

public:
    BEV(double cap, double pow) : capacity(cap), powerKW(pow) {}

    double power() const override {
        return powerKW; // Retorna a potência em kW
    }

    double getCapacity() const { return capacity; }
    void setCapacity(double cap) { capacity = cap; }
};

class ICE : public Motor {
private:
    FuelType fuelType; // Tipo de combustível
    double capacity;   // Capacidade do tanque em litros
    double powerCV;    // Potência em cavalos-vapor (cV)

public:
    ICE(FuelType ft, double cap, double pow) : fuelType(ft), capacity(cap), powerCV(pow) {}

    double power() const override {
        return powerCV; // Retorna a potência em cV
    }

    FuelType getFuelType() const { return fuelType; }
    void setFuelType(FuelType ft) { fuelType = ft; }

    double getCapacity() const { return capacity; }
    void setCapacity(double cap) { capacity = cap; }
};

#pragma region Ficheiros




//pseudo code
// 
// for(every line)
// 
//      payload string
//      typeconvert switch 
// 
//      switch(vehicle type)
//          car -> 
//              swithc(car type)
//                  sadan -> sadan
//                  coupe -> coupe
//         motorcycle -> motorcycle()



#pragma endregion 

// Método main para testes
int main() {
    // Testando a criação de veículos
    try {
        Car car(4, 2021, "Toyota", FuelType::GASOLINE);
        std::cout << car.toString() << std::endl;

        Coupe coupe(2017, "Opel", FuelType::GASOLINE);
        std::cout << coupe.toString() << std::endl;

        Sedan sedan(2018, "BMW", FuelType::DIESEL);
        std::cout << sedan.toString() << std::endl;

        Motorcycle motorcycle(true, 2015, "Honda", FuelType::GASOLINE);
        std::cout << motorcycle.toString() << std::endl;

        // Testando a criação de stands
        Stand lisbonStand("Lisboa", 1);
        Stand aveiroStand("Aveiro", 2);
        Stand barcelosStand("Barcelos", 3);

        // Associando veículos a stands
        VehicleStand vehicleStand;
        vehicleStand.addVehicle(lisbonStand, &car);
        vehicleStand.addVehicle(aveiroStand, &coupe);
        vehicleStand.addVehicle(barcelosStand, &sedan);
        vehicleStand.addVehicle(lisbonStand, &motorcycle);

        // Imprimindo todos os stands e veículos associados
        std::cout << vehicleStand.toString() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }


    try {

        std::cout << "FACTORY:\n";

        Car car = VehicleFactory::createCar(4, 2021, "Toyota", FuelType::GASOLINE);
        std::cout << car.toString() << std::endl;

        Coupe coupe = VehicleFactory::createCoupe(2017, "Opel", FuelType::GASOLINE);
        std::cout << coupe.toString() << std::endl;

        Sedan sedan = VehicleFactory::createSedan(2018, "BMW", FuelType::DIESEL);
        std::cout << sedan.toString() << std::endl;

        Motorcycle motorcycle = VehicleFactory::createMotorcycle(true, 2015, "Honda", FuelType::GASOLINE);
        std::cout << motorcycle.toString() << std::endl;

        // Resto do código de teste...
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }


    return 0;
}
