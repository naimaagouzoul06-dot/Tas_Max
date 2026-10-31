
#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <random> //generation des nombres aleatoire
#include <iomanip> //organisation de tableau d affichage

using namespace std;
using namespace chrono;


// ============================ tas max avec vector =============================================== //


// Insertion
void inserer(vector<int>& tas, int valeur)
{
    tas.push_back(valeur);

    int i = tas.size() - 1;

    while (i > 0)
    {
        int parent = (i - 1) / 2;

        if (tas[i] > tas[parent])
        {
            swap(tas[i], tas[parent]);
            i = parent;
        }
        else
            break;
    }
}

// Suppression du maximum
int extraireMax(vector<int>& tas)
{
    int maximum = tas[0];

    tas[0] = tas.back();
    tas.pop_back();

    int i = 0;
    int n = tas.size();

    while (true)
    {
        int gauche = 2 * i + 1;
        int droite = 2 * i + 2;

        int plusGrand = i;

        if (gauche < n && tas[gauche] > tas[plusGrand])
            plusGrand = gauche;

        if (droite < n && tas[droite] > tas[plusGrand])
            plusGrand = droite;

        if (plusGrand == i)
            break;

        swap(tas[i], tas[plusGrand]);

        i = plusGrand;
    }

    return maximum;
}

//============================= fonction principale ====================================================//

int main()
{
    int tailles[] = {10000, 50000, 100000, 500000, 1000000};

    mt19937 gen(42);
    uniform_int_distribution<int> dist(1, 10000000);

    cout << fixed << setprecision(6);

    cout << "\n========== BENCHMARKING MAX HEAP ==========\n\n";

    cout << left
         << setw(12) << "Elements"
         << setw(22) << "Insertion Vector(ms)"
         << setw(22) << "Insertion PQ(ms)"
         << setw(22) << "Extraction Vector(ms)"
         << setw(22) << "Extraction PQ(ms)"
         << endl;

    cout << string(100, '-') << endl;

    for (int taille : tailles)
    {
        vector<int> donnees;

        // Generer les memes donnees pour les deux tas
        for (int i = 0; i < taille; i++)
            donnees.push_back(dist(gen));

        vector<int> tasVector;
        priority_queue<int> tasPQ;

        // =================================
        // INSERTION VECTOR
        // =================================

        auto debut = steady_clock::now();

        for (int x : donnees)
            inserer(tasVector, x);

        auto fin = steady_clock::now();

        double tempsInsertionVector =
            duration<double, milli>(fin - debut).count();


        // insertion dans le cas du priority queue

        debut = steady_clock::now();

        for (int x : donnees)
            tasPQ.push(x);

        fin = steady_clock::now();

        double tempsInsertionPQ =
            duration<double, milli>(fin - debut).count();

        // extraction du vector dans le cas de vector
        debut = steady_clock::now();

        int maxVector = extraireMax(tasVector);

        fin = steady_clock::now();

        double tempsExtractionVector =
            duration<double, milli>(fin - debut).count();


        // extraction du maximum dans le cas de priority queue
        debut = steady_clock::now();

        int maxPQ = tasPQ.top();
        tasPQ.pop();

        fin = steady_clock::now();

        double tempsExtractionPQ =
            duration<double, milli>(fin - debut).count();


        // AFFICHAGE

        cout << left
             << setw(12) << taille
             << setw(22) << tempsInsertionVector
             << setw(22) << tempsInsertionPQ
             << setw(22) << tempsExtractionVector
             << setw(22) << tempsExtractionPQ
             << endl;

        if (maxVector != maxPQ)
            cout << "Erreur : maximums differents !" << endl;
    }

    return 0;
}
