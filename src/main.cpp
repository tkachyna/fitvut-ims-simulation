#include <iostream>
#include <cmath>  
#include <vector>
#include <map>
#include <numeric>
#include "main.hpp"

/* Constants */

#define ONE_SOLAR_PANEL_PRICE 14595 // CosmoSOL 8253L
#define PUMP 12753 // Wilo Yonos Para ST 15-130/7
#define EXPANSION_TANK 1462 // COSMO  MAG-S 18L 

class WaterConsumption {
    
    public:
        float z = 0.3;
        float ro = 999.7;
        int c = 4184;

        float V_tv_den;
        int n, t_tv, t_sv;
        
        WaterConsumption(float V_tv_den, int n = 31, int t_tv = 60, int t_sv = 15)
        {   
            this->V_tv_den = V_tv_den;
            this->n = n;
            this->t_tv = t_tv;
            this->t_sv = t_sv;
        };

        float countOneMonth(int n) 
        {
            return (1 + z) * ((n * 4 * V_tv_den * ro * c * (t_tv-t_sv))/(3.6 * 1000000));
        }

        std::vector<float> countOneYear()
        {
            std::vector<int> months = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            std::vector<float> result;
            int auxResult;

            std::vector<int>::iterator ptr;

            for (ptr = months.begin(); ptr < months.end(); ptr++)
            {
                auxResult = this->countOneMonth(*ptr);
                result.insert(result.end(), auxResult);
            }

            return result;
        }
};

class SolarPanel {

    public:
    
    float n0 = 0.8; // opticial efficency (change to 0.78)
    float a1 = 3.1; // linear heat loss coefficient of the collector (change to 3.123)
    float a2 = 0.005; // Quadratic heat loss coefficient of the collector (change to 0.019)
    int numberOfCollectors;
    float t_es[12] = {2.2, 3.4, 6.5, 12.1, 16.6, 20.6, 22.5, 22.6, 19.4, 13.8, 7.3, 3.5};
    int t_km;
    std::vector<int>G_TM;
    std::vector<int>H_t_den;
    float A_k = 10;
    float p;
    int n = 31;

    SolarPanel(int n, int t_km, std::vector<int>G_TM,  std::vector<int>H_t_den)
    {
        this->numberOfCollectors = n;
        this->t_km = t_km;
        this->G_TM = G_TM;
        this->H_t_den = H_t_den;
    }

    float countCollectorEfficiencyForOneMonth( int month) 
    {   

        std::cout << "n0: " << n0  << "a1: " << a1  << "a2: " << a2 << "t_km: " << t_km << "t_es: " << t_es[6] << "G_TM: " << G_TM[month] << std::endl;
        return  n0 - a1 * ((t_km - t_es[month])/G_TM[month]) - a2 * (pow((t_km - t_es[month]) ,2) / G_TM[month]);
    }

    float countUsableGainsForOneMonth(float n_, int month)
    {   
         std::cout << "n: " << n_  << "H_t_den[1]: " << H_t_den[6]/31  << "n:" << n << "A_k: " << A_k << "p: " << p <<  std::endl;
        return 0.9 * n_ * H_t_den[month]/31 * n * A_k * 0.994;
    }

    void countP()
    {
        this->p = (0.26 / A_k) + 100 * (A_k / 9600);
    }

};


int main(int argc, char **argv) 
{      
    int numOfPanels = 4;
    int investmentCosts = ONE_SOLAR_PANEL_PRICE * numOfPanels + PUMP + EXPANSION_TANK * 2;
    float elPriceKWH = 4.259; // in CZK

    WaterConsumption cons(0.1);

    float WconsumptionMonth = cons.countOneMonth(31);
    std::vector<float> WconsumptionYear = cons.countOneYear();

    SolarPanel panel(31, 35, Azimuth0_Slope0, radiationSumInclinedSurface_0);
    panel.countP();

    std::vector<int> gainsYear;
    for (int i = 0; i < 12; ++i)
    {
        float efficiency = panel.countCollectorEfficiencyForOneMonth(i);
        float gains = panel.countUsableGainsForOneMonth(efficiency, i);
        std::cout << "Mesic: "<< i << " " << gains << std::endl;   
        gainsYear.insert(gainsYear.end(), gains);
    }
    
    std::vector<int> usableGainsOneYear;
    std::vector<int> totalHeatConsumption;
    std::vector<int> difference;

    std::vector<int>::iterator UGit;
    std::vector<int>::iterator THCit = totalHeatConsumption.begin();


    for (UGit = usableGainsOneYear.begin(); UGit != usableGainsOneYear.end(); ++UGit)
    {
        difference.insert(difference.end(), (*THCit - *UGit) * elPriceKWH);
        ++THCit;
    }

    
    
    int elPriceWithPanels = std::accumulate(difference.begin(), difference.end(), 0);

    int elPriceWithoutPanels = std::accumulate(totalHeatConsumption.begin(), totalHeatConsumption.end(), 0) * 6;

    int yearSavings = elPriceWithoutPanels - elPriceWithPanels;

    int returnOfInvestments = investmentCosts / yearSavings; 

     /**
     * Investice do solarnich panelu
     * nakladu
     * kdy se to vyplati
     */


    return 0;
}