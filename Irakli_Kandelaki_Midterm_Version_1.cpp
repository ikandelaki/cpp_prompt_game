#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

const double HP_THRESHOLD = 50;
const double HP_INCREASE_STEP = 10;
const double BASE_DAMAGE_FACTOR = 5.0;
const double FLEE_STAMINA_THRESHOLD = 20;
const double CRITICAL_STRIKE_VALUE = 1.5;

void printStats(double hp, double opponentHp, unsigned short stamina);
void displayMenu(int& choice);
bool handleAttack(double& opponentHp);
void handleSprint(unsigned short& stamina);
void handleHeal(double& playerHp);
bool handleFlee(unsigned short stamina, double playerHp);

int main() {
    // Variables to store player attributes
    double playerHp;
    double opponentHp = 100;
    unsigned short stamina;
    bool alive = true;
    int choice;
    bool exit = false;

    // Seed the random num
    srand(time(NULL));

    cout << "Enter your starting health (HP): ";
    cin >> playerHp;

    cout << "Enter your starting stamina (0-100): ";
    cin >> stamina;

    do {
        if (!alive || playerHp <= 0 || exit) {
            break;
        }

        printStats(playerHp, opponentHp, stamina);
        displayMenu(choice);

        if (choice == -1) {
            goto VICTORY;
        }
        
        switch(choice) {
            case 1: {
                bool won = handleAttack(opponentHp);

                if (won) goto VICTORY;

                break;
            }
            case 2: {
                handleSprint(stamina);
                break;
            }
            case 3: {
                handleHeal(playerHp);
                break;
            }
            case 4: {
                bool flee = handleFlee(stamina, playerHp);

                if (flee) goto END;

                break;
            }
            case 5: {
                // Placeholder for future
                ;
                break;
            }
            default: {
                cout << "Invalid choice." << endl;
                break;
            }
        }
    } while (playerHp > 0 && !exit);

VICTORY:
    cout << "\n--- Game Over ---" << endl;
END:
    cout << "\nThanks for playing." << endl;

    return 0;
}

/**
 * Print available stats:
 * Player stats:
 * 1) Player Hp
 * 2) Player stamina
 * 3) Player health status - Either Healthy or Critical
 * 
 * Opponent stats:
 * 1) Opponent Hp
 */
void printStats(double hp, double opponentHp, unsigned short stamina) {
    string status = hp > HP_THRESHOLD ? "(Healthy)" : "(Critical)";

    cout << "\n--- Your Stats: ---" << endl;
    cout << "HP: " << hp << " " << status << endl;
    cout << "Stamina: " << stamina << endl;
    cout << "Opponent HP: " << opponentHp << endl;
}

/**
 * Display action menu to the player
 * 1) Attack
 * 2) Sprint
 * 3) heal
 * 4) Flee
 */
void displayMenu(int& choice) {
    cout << "\n1. Attack  2. Sprint  3. Heal  4. Flee" << endl;
    cout << "Choice (or -1 for invincibility): ";
    cin >> choice;
}

/**
 * Calculate damage to the enemy and substract the value from the opponent
 * Returns:
 *  true if opponentHp <= 0, so if the game should be finished.
 *  false if opponent is still healthy and the game should be continued.
 */
bool handleAttack(double& opponentHp) {
    double inputAttackValue;

    cout << "Enter Input Attack Value: ";
    cin >> inputAttackValue;

    double rawDamage = (sqrt(inputAttackValue) * BASE_DAMAGE_FACTOR) / 2.0;
    int randVal = rand() % 100 + 1;
    bool isCriticalStrike = (randVal % 3 == 0);
    int finalDamage = isCriticalStrike
        ? static_cast<int>(rawDamage * CRITICAL_STRIKE_VALUE)
        : static_cast<int>(rawDamage);

    opponentHp -= finalDamage;

    cout << "You dealt " << finalDamage << " damage! Opponent HP: " << opponentHp << endl;

    if (isCriticalStrike) {
        cout << "Critical Strike!" << endl;
    }

    if (opponentHp <= 0) {
        cout << "Opponent defeated! " << endl;
    }

    return opponentHp <= 0;
}

void handleSprint(unsigned short& stamina) {
    cout << "--- Sprinting! ---" << endl;

    for (int meter = 1; meter <= 10; meter++) {
        if (stamina == 0) {
            cout << "Out of stamina! Sprinting stopped at meter " << meter - 1 << endl;
            break;
        }

        if (meter % 2 == 0) {
            continue;
        }
        
        --stamina;
        cout << "Meter: " << meter << " complete." << endl;
    }
}

void handleHeal(double& playerHp) {
    cout << "--- HEALING ---" << endl;
    int count = 0;

    while (count < 3) {
        playerHp += HP_INCREASE_STEP;
        count++;
        cout << "Heal + " << HP_INCREASE_STEP << ". Current HP: " << playerHp << endl;
    }
}

bool handleFlee(unsigned short stamina, double playerHp) {
    if (stamina >= FLEE_STAMINA_THRESHOLD && playerHp > 0) {
        cout << "You fled successfully!" << endl;
        return true;
    } else {
        cout << "Not enough stamina or HP" << endl;
        return false;
    }
}