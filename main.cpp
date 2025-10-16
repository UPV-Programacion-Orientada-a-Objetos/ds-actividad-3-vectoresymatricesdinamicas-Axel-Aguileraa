#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

const int NUM_PRODUCTS = 5;
const int NUM_MP_TYPES = 4;
const int INITIAL_WEEKS = 4;

const string PRODUCT_NAMES[NUM_PRODUCTS] = {"Componente A (P0)", "Componente B (P1)", "Componente C (P2)", "Componente D (P3)", "Componente E (P4)"};
const string MP_NAMES[NUM_MP_TYPES] = {"MP-01 (Resistor)", "MP-02 (Capacitor)", "MP-03 (PCB)", "MP-04 (Chip)"};

const float MP_ACQUISITION_COSTS[NUM_MP_TYPES] = {1.5f, 2.0f, 1.0f, 2.5f};

class ProductionSystem {
private:
    float** planningMatrix;     
    float* unitCostsVector;     
    float* rawMaterialInventory; 

    const int materialRequirements[NUM_PRODUCTS][NUM_MP_TYPES] = {
        {2, 0, 1, 3},
        {1, 1, 0, 2},
        {0, 3, 2, 0},
        {4, 0, 0, 1},
        {1, 1, 1, 1}
    };

    int currentWeeks;

public:
    ProductionSystem() : currentWeeks(INITIAL_WEEKS) {
        cout << "--- Inicializando Sistema de Produccion ---" << endl;
        
        planningMatrix = new float*[NUM_PRODUCTS];
        for (int i = 0; i < NUM_PRODUCTS; ++i) {
            planningMatrix[i] = new float[currentWeeks]; 
        }

        unitCostsVector = new float[NUM_PRODUCTS];
        rawMaterialInventory = new float[NUM_MP_TYPES];
        
        float initialCosts[NUM_PRODUCTS] = {5.0f, 4.5f, 6.0f, 8.0f, 7.5f};
        float initialMP[NUM_MP_TYPES] = {1000.0f, 500.0f, 800.0f, 1200.0f};
        float initialPlan[NUM_PRODUCTS][INITIAL_WEEKS] = {
            {100, 150, 100, 200}, {200, 100, 150, 100}, {50, 200, 100, 50}, 
            {100, 50, 50, 100}, {150, 100, 100, 150}
        };

        for (int i = 0; i < NUM_PRODUCTS; ++i) {
            unitCostsVector[i] = initialCosts[i];
            for (int j = 0; j < currentWeeks; ++j) {
                planningMatrix[i][j] = initialPlan[i][j];
            }
        }
        for (int i = 0; i < NUM_MP_TYPES; ++i) {
            rawMaterialInventory[i] = initialMP[i];
        }

        cout << "Inicializacion exitosa. Matriz: " 
             << NUM_PRODUCTS << " Prod. x " << currentWeeks << " Sem." << endl;
    }

    ~ProductionSystem() {
        cout << "\nLiberando memoria de Matriz y Vectores Dinamicos..." << endl;
        for (int i = 0; i < NUM_PRODUCTS; ++i) {
            delete[] planningMatrix[i];
        }
        delete[] planningMatrix;
        
        delete[] unitCostsVector;
        delete[] rawMaterialInventory;

        cout << "Memoria liberada. Sistema cerrado." << endl;
    }
