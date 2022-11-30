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
#define EL_ENERGY_PRICE_KWH 4.259

#define WATER_CONSUMPTION_PER_PERSON  0.1   // mˆ3
#define AZIMUTH_SLOPE Azimuth0_Slope30
#define SUM_OF_RADIATION_INCLINED_SURFACE radiationSumInclinedSurface_0

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

class SolarCollector {

    public:
    
    int t_km = 35; // mean daily temperature in solar collectors
    float n0 = 0.78; // opticial efficency (change to 0.78 (CosmoSOL 8203))
    float a1 = 3.123; // linear heat loss coefficient of the collector (change to 3.123 (CosmoSOL 8203))
    float a2 = 0.019;  // Quadratic heat loss coefficient of the collector (change to 0.019 (CosmoSOL 8203))
    // average outside temperature during sunshine
    float t_es[12] = {2.2, 3.4, 6.5, 12.1, 16.6, 20.6, 22.5, 22.6, 19.4, 13.8, 7.3, 3.5}; 
    float A_k = 10; // aperture area of solar collectors
    float p; // the value of the deduction from the heat gains of the solar collectors due to the heat losses of the solar system
    std::vector<int>G_TM; // average daily solar radiation for a given slope and orientation
    std::vector<float>H_t_den; // actual daily dose of solar radiation
    std::vector<int> daysInMonths = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    std::vector<float> theoreticallyUsableGains;
    std::vector<float> actualUsableGains;
    std::vector<float> difference;


    SolarCollector(std::vector<int>G_TM,  std::vector<float>H_t_den)
    {
        this->G_TM = G_TM;
        this->H_t_den = H_t_den;
        this->countP();
    }


    float countCollectorEfficiencyForOneMonth(int month) 
    {   

        std::cout << "n0: " << n0  << "a1: " << a1  << "a2: " << a2 << "t_km: " << t_km << "t_es: " << t_es[month] << "G_TM: " << G_TM[month] << std::endl;
        return  n0 - a1 * ((t_km - t_es[month])/G_TM[month]) - a2 * (pow((t_km - t_es[month]) ,2) / G_TM[month]);
    }


    float countUsableGainsForOneMonth(float n_, int month)
    {   
         std::cout << "n: " << n_  << "H_t_den[1]: " << H_t_den[month]/31  << "n:" << this->daysInMonths[month] << "A_k: " << A_k << "p: " << this->p <<  std::endl;
        return 0.9 * n_ * H_t_den[month]/31 * this->daysInMonths[month] * A_k * (1-this->p);
    }

    void countP()
    {
        this->p = (0.26 / A_k) + 100 * (A_k / 9915);
    }

    void countTheoreticallyUsableGainsYear() 
    {
        // this for loop counts theoretically usable gains from solar collectors in a year 
        for (int i = 0; i < 12; ++i)
        {
            float solarCollectorEfficiency = this->countCollectorEfficiencyForOneMonth(i);
            float solarCollectorGains = this->countUsableGainsForOneMonth(solarCollectorEfficiency, i);
            // std::cout << "Month: "<< i << " " << solarCollectorGains << std::endl;  // DEBUG PRINT 
            this->theoreticallyUsableGains.insert(this->theoreticallyUsableGains.end(), solarCollectorGains);
        }

       
    }
    
    void countActualUsableGains(std::vector<float>::iterator THNit)
    {   
        
        std::vector<float>::iterator TUGit; // iterator for theoretically usable gains of solar energy gains

        // Qss,u = min (Qk,u; Qp,c)
        // this loop takes minimum of two numbers (generated energy, energy needed for water heating) 
        // and insert into a vector, which means actual usable solar energy gains (Qss,u)
        for (TUGit = theoreticallyUsableGains.begin(); TUGit != theoreticallyUsableGains.end(); ++TUGit)
        {
            actualUsableGains.insert(actualUsableGains.begin(), std::min(*TUGit, *THNit));
        }
    }

    void countDifference(std::vector<float>::iterator THNit)
    {   
        std::vector<float>::iterator AUGit; // iterator for actual usable gains of solar energy gains

        // this for loop counts calculates the difference between the actual usable gains and total heed needed
        for (AUGit = actualUsableGains.begin(); AUGit != actualUsableGains.end(); ++AUGit)
        {
            difference.insert(difference.end(), (*THNit - *AUGit));
            ++THNit;
        }

    }

    float countSollarCollectorsYearsShare() 
    {
        return 100 * (5452/7900);
    }

};

class Savings {

    public:

    float elPriceWithoutPanels;
    float elPriceWithPanels;
    int numOfPanels;
    float yearSavings;
    int investmentCosts = ONE_SOLAR_PANEL_PRICE * 4 + PUMP + EXPANSION_TANK * 2;

    Savings(int numOfPanels)
    {
        this->numOfPanels = numOfPanels;
    }

    void countElectricEnergyPriceWithPanels(std::vector<float> difference)
    {
        this->elPriceWithPanels = std::accumulate(difference.begin(), difference.end(), 0) * EL_ENERGY_PRICE_KWH;
    }

    void countElectricEnergyPriceWithoutPanels(std::vector<float> WconsumptionYear)
    {
        this->elPriceWithoutPanels = std::accumulate(WconsumptionYear.begin(), WconsumptionYear.end(), 0) * EL_ENERGY_PRICE_KWH;
    }

    void countYearSavings()
    {
        this->yearSavings = elPriceWithoutPanels - elPriceWithPanels;
    }

    float countReturnOfInvestments()
    {
        return investmentCosts / yearSavings; 
    }

};


int main(int argc, char **argv) 
{   
    // counts monthly water consumption of 4 persons for a one year
    WaterConsumption consumption(WATER_CONSUMPTION_PER_PERSON);

    std::vector<float> WaterConsumptionYear = consumption.countOneYear();

    // DEBUG 
    for (int i = 0; i < WaterConsumptionYear.size(); i++) {
        std::cout << " " << WaterConsumptionYear[i] << std::endl;
    }

    std::cout << std::accumulate(WaterConsumptionYear.begin(), WaterConsumptionYear.end(), 0) << std::endl;

    SolarCollector collector(AZIMUTH_SLOPE, SUM_OF_RADIATION_INCLINED_SURFACE);

    collector.countTheoreticallyUsableGainsYear();
    
     // DEBUG
    for (int i = 0; i < collector.theoreticallyUsableGains.size(); i++) {
        std::cout << " " << collector.theoreticallyUsableGains[i] << std::endl;
    }

    


    // std::vector<float>::iterator THNit = WconsumptionYear.begin(); // iterator for Total Heat Nedded 

    return 0;
}