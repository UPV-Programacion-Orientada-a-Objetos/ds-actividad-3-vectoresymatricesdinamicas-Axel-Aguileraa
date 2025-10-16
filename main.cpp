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

    void modifyProduction() {
        int productIndex, weekIndex;
        float newQuantity;

        cout << "\n--- Modificar Produccion ---" << endl;
        cout << "Ingrese Producto (0-" << NUM_PRODUCTS - 1 << "): ";
        cin >> productIndex;
        cout << "Ingrese Semana (0-" << currentWeeks - 1 << "): ";
        cin >> weekIndex;

        if (cin.fail() || productIndex < 0 || productIndex >= NUM_PRODUCTS || 
            weekIndex < 0 || weekIndex >= currentWeeks) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Error: Producto o Semana invalida." << endl;
            return;
        }

        cout << "Cantidad a producir: ";
        cin >> newQuantity;
        if (cin.fail() || newQuantity < 0) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Error: Cantidad invalida." << endl;
            return;
        }

        planningMatrix[productIndex][weekIndex] = newQuantity;
        cout << "Produccion de " << PRODUCT_NAMES[productIndex] << " en Sem. " 
             << weekIndex << " actualizada a " << newQuantity << " unidades." << endl;
    }

    void resizeMatrix() {
        int newWeeks = currentWeeks + 1;
        cout << "\nAgregando Semana " << currentWeeks << "..." << endl;

        float** newMatrix = new float*[NUM_PRODUCTS];
        for (int i = 0; i < NUM_PRODUCTS; ++i) {
            newMatrix[i] = new float[newWeeks];
        }

        for (int i = 0; i < NUM_PRODUCTS; ++i) {
            for (int j = 0; j < currentWeeks; ++j) {
                newMatrix[i][j] = planningMatrix[i][j];
            }
            newMatrix[i][currentWeeks] = 0.0f;
            
            delete[] planningMatrix[i];
        }
        delete[] planningMatrix;

        planningMatrix = newMatrix;
        currentWeeks = newWeeks;

        cout << "Matriz redimensionada a " << NUM_PRODUCTS << "x" << currentWeeks 
             << ". Memoria gestionada con exito!" << endl;
    }

    float calculateMPConsumption(int weekIndex, float* consumptionVector) const {
        float totalUnitsProduced = 0.0f;

        for (int k = 0; k < NUM_MP_TYPES; ++k) {
            consumptionVector[k] = 0.0f;
        }

        for (int k = 0; k < NUM_MP_TYPES; ++k) {
            for (int i = 0; i < NUM_PRODUCTS; ++i) {
                consumptionVector[k] += planningMatrix[i][weekIndex] * materialRequirements[i][k];
            }
        }

        for (int i = 0; i < NUM_PRODUCTS; ++i) {
            totalUnitsProduced += planningMatrix[i][weekIndex];
        }

        return totalUnitsProduced;
    }

    void calculateCOGS(int weekIndex) const {
        float* consumptionVector = new float[NUM_MP_TYPES];
        float* remainingMP = new float[NUM_MP_TYPES];

        float totalUnitsProduced = calculateMPConsumption(weekIndex, consumptionVector);

        float cogsValue = 0.0f;
        for (int i = 0; i < NUM_PRODUCTS; ++i) {
            cogsValue += planningMatrix[i][weekIndex] * unitCostsVector[i];
        }

        float finalInventoryValue = 0.0f;
        for (int k = 0; k < NUM_MP_TYPES; ++k) {
            remainingMP[k] = rawMaterialInventory[k] - consumptionVector[k];
            if (remainingMP[k] < 0) {
                remainingMP[k] = 0.0f;
            }
            
            finalInventoryValue += remainingMP[k] * MP_ACQUISITION_COSTS[k];
        }

        cout << fixed << setprecision(2);
        cout << "\n--- Reporte de Costos (Semana " << weekIndex << ") ---" << endl;
        cout << "Produccion Total: " << totalUnitsProduced << " unidades." << endl;
        cout << "Costo de Produccion (COGS): $" << cogsValue << endl;
        
        cout << "\nConsumo de Materia Prima:" << endl;
        for (int k = 0; k < NUM_MP_TYPES; ++k) {
            cout << "  - " << MP_NAMES[k] << ": " << consumptionVector[k] << " unidades." << endl;
        }
        
        cout << "Valor del Inventario Final (M.P. restante): $" << finalInventoryValue << endl;
        
        delete[] consumptionVector;
        delete[] remainingMP;
    }
    
    void displayPlanningMatrix() const {
        cout << "\n--- Plan de Produccion (Productos vs. Semanas) ---" << endl;
        
        cout << setw(20) << "Producto \\ Semana |";
        for (int j = 0; j < currentWeeks; ++j) {
            cout << setw(8) << "W" + to_string(j) + " |";
        }
        cout << endl;
        cout << string(20 + 9 * currentWeeks, '-') << endl;

        cout << fixed << setprecision(0);
        for (int i = 0; i < NUM_PRODUCTS; ++i) {
            cout << setw(20) << PRODUCT_NAMES[i] << " |";
            for (int j = 0; j < currentWeeks; ++j) {
                cout << setw(8) << planningMatrix[i][j] << " |";
            }
            cout << endl;
        }
        cout << endl;
    }
    
    void runMenu() {
        int choice;
        do {
            cout << "\n--- Menu Principal ---" << endl;
            cout << "1. Ver Plan de Produccion" << endl;
            cout << "2. Agregar Nueva Semana (Redimensionar Matriz)" << endl;
            cout << "3. Modificar Produccion" << endl;
            cout << "4. Calcular COGS y Final Inventory" << endl;
            cout << "5. Salir" << endl;
            cout << "Opcion seleccionada: ";
            
            if (!(cin >> choice)) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "Entrada invalida. Por favor, ingrese un numero." << endl;
                choice = 0; 
            } else {
                cin.ignore(10000, '\n');
            }

            switch (choice) {
                case 1:
                    displayPlanningMatrix();
                    break;
                case 2:
                    resizeMatrix();
                    break;
                case 3:
                    modifyProduction();
                    break;
                case 4: {
                    int week;
                    cout << "Ingrese Semana para el calculo (0-" << currentWeeks - 1 << "): ";
                    if (!(cin >> week) || week < 0 || week >= currentWeeks) {
                        cin.clear(); cin.ignore(10000, '\n');
                        cout << "Error: Entrada de semana invalida o fuera de rango." << endl;
                    } else {
                        calculateCOGS(week);
                    }
                    cin.ignore(10000, '\n');
                    break;
                }
                case 5:
                    break;
                default:
                    if (choice != 0) {
                        cout << "Opcion no valida." << endl;
                    }
                    break;
            }
        } while (choice != 5);
    }
};

int main() {
    try {
        ProductionSystem system;
        system.runMenu();
    } catch (const exception& e) {
        cerr << "Error inesperado: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Error desconocido." << endl;
        return 1;
    }

    return 0;
}
