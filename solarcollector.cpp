/**
*   IMS Project - solar collectors and their efficiency
*   
*   @author Tadeáš Kachyňa, <xkachy00@stud.fit.vutbr.cz>, Lucia Makaiova , <xmakai00@stud.fit.vutbr.cz>
*   @date 4/12/2022
*   @file solarcollector.cpp
*
*   @brief A project in a subject IMS at FIT BUT dealing with the problem 
*          of solar collectors and their efficiency and then saving on thermal energy
*/

#include <iostream>
#include <cmath>  
#include <vector>
#include <map>
#include <numeric>
#include <getopt.h>
#include "solarcollector.hpp"

/*  ------- OTHER VARIABLES USER CAN CHANGE  ------- */

/* Expenses - Collectors, Other Componens */

#define ONE_SOLAR_COLLECTOR_PRICE 18900 // CosmoSOL 8253L
#define PUMP 12753 // Wilo Yonos Para ST 15-130/7
#define EXPANSION_TANK 1119 // COSMO  MAG-S 18L 

#define EL_ENERGY_PRICE_KWH 4.259

#define WATER_CONSUMPTION_PER_PERSON  0.08  // mˆ3

#define NO_OF_COLLECTORS 5

/*------- CHANGED BY PROGRAM'S ARGUMENTS ------- */

u_int8_t azimuth = 0;
u_int8_t slope = 0;
#define AZIMUTH_SLOPE AzimuthAndSlope(azimuth, slope)
#define SUM_OF_RADIATION_INCLINED_SURFACE radiationSumInclinedSurface(slope)

/**
*   @class WaterConsumption
*   
*   @brief counts monthly and yearly water consumption and the heat needed for heating
*/
class WaterConsumption {
    
    public:
        float z = 0.3; // surcharge for the inclusion of heat losses related to the preparation of hot water water
        float ro = 999.7; // water density
        int c = 4184; // heat capacity of water

        float V_tv_day; // average daily water consumption
        int n, t_tv, t_sv; // number of days in month, temp of cold water, temp of hot water
        
        WaterConsumption(float V_tv_day, int n = 31, int t_tv = 60, int t_sv = 15) // constructor
        {   
            this->V_tv_day = V_tv_day;
            this->n = n;
            this->t_tv = t_tv;
            this->t_sv = t_sv;
        };

        /* Counts heat needed for one month */
        float countOneMonth(int n) 
        {
            return (1 + z) * ((n * 4 * V_tv_day * ro * c * (t_tv-t_sv))/(3.6 * 1000000));
        }

        /* Counts heat needed for one year */
        std::vector<float> countOneYear()
        {
            std::vector<int> months = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            std::vector<float> result;
            float auxResult;

            std::cout << "---------------------------------------" << std::endl;
            std::cout << "Monthly energy needed for heating water:" << std::endl;

            for (int i = 0; i < 12; ++i)
            {   
                auxResult = this->countOneMonth(months[i]);
                std::cout << "Month: " << i+1 << " >> " << auxResult << " kWh" << std::endl;
                result.insert(result.end(), auxResult);
            }

            std::cout << "---------------------------------------" << std::endl;
            std::cout << "Yearly energy needed for heating water:" << std::endl;  
            std::cout << std::accumulate(result.begin(), result.end(), 0) << " kWh" << std::endl;          

            return result;
        }
};

/**
*   @class SolarCollector 
*   
*   @brief this class contains all details about a sollar collactor 
*          and counts its efficiency, gains and so on
*/
class SolarCollector {

    public:
    
    int WaterConsumptionYear = 0;
    int t_km = 35; // mean daily temperature in solar collectors
    float n0 = 0.78; // opticial efficency (change to 0.78 (CosmoSOL 8203))
    float a1 = 3.123; // linear heat loss coefficient of the collector (change to 3.123 (CosmoSOL 8203))
    float a2 = 0.019;  // Quadratic heat loss coefficient of the collector (change to 0.019 (CosmoSOL 8203))
    // average outside temperature during sunshine
    float t_es[12] = {2.2, 3.4, 6.5, 12.1, 16.6, 20.6, 22.5, 22.6, 19.4, 13.8, 7.3, 3.5}; 
    float A_k = 2.5 * NO_OF_COLLECTORS; // aperture area of solar collectors
    float p; // the value of the deduction from the heat gains of the solar collectors due to the heat losses of the solar system
    std::vector<int>G_TM; // average daily solar radiation for a given slope and orientation
    std::vector<float>H_t_den; // actual daily dose of solar radiation
    std::vector<float>WaterConsumptionYearVector; // actual daily dose of solar radiation
    std::vector<int> daysInMonths = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    std::vector<float> theoreticallyUsableGains;
    std::vector<float> actualUsableGains;
    std::vector<float> difference;


    SolarCollector(std::vector<int>G_TM,  std::vector<float>H_t_den, int WaterConsumptionYear,  std::vector<float>WaterConsumptionYearVector)
    {
        this->G_TM = G_TM;
        this->H_t_den = H_t_den;
        this->p = (0.26 / A_k) + 100 * (A_k /WaterConsumptionYear);
        this->WaterConsumptionYear = WaterConsumptionYear;
        this->WaterConsumptionYearVector = WaterConsumptionYearVector;
    }


    float countCollectorEfficiency(int month) 
    {   
       // std::cout << " n0: " << n0  << " a1: " << a1  << " a2: " << a2 << " t_km: " << t_km << " t_es: " << t_es[month] << " G_TM: " << G_TM[month] << std::endl;
        return  n0 - a1 * ((t_km - t_es[month])/G_TM[month]) - a2 * (pow((t_km - t_es[month]) ,2) / G_TM[month]);
    }


    float  countTheorUsableGainsOneMonth(float n_, int month)
    {   
       // std::cout << " n: " << n_  << " H_t_den[1]: " << H_t_den[month]/31  << " n:" << this->daysInMonths[month] << " A_k: " << A_k << " p: " << (1-this->p) <<  std::endl;
        return 0.9 * n_ * H_t_den[month]/31 * this->daysInMonths[month] * A_k * (1-this->p);
    }


    void countTheoreticallyUsableGainsYear() 
    {   
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Monthly theoretically usable gains:" << std::endl;

        // this for loop counts theoretically usable gains from solar collectors in a year 
        for (int i = 0; i < 12; ++i)
        {
            float solarCollectorEfficiency = this->countCollectorEfficiency(i);
            float solarCollectorGains = this->countTheorUsableGainsOneMonth(solarCollectorEfficiency, i);
            std::cout  << "Month: " << i+1 << " >> " << solarCollectorGains << " kWh" << std::endl;
            this->theoreticallyUsableGains.insert(this->theoreticallyUsableGains.end(), solarCollectorGains);
        }

        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Yearly theoretically usable gains:" << std::endl;
        std::cout << std::accumulate(theoreticallyUsableGains.begin(), theoreticallyUsableGains.end(), 0) << " kWh" << std::endl; 

    }
    
    void countActualUsableGains()
    {      
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Monthly actual usable gains:" << std::endl;


        // std::vector<float>::iterator TUGit; // iterator for theoretically usable gains of solar energy gains
        // Qss,u = min (Qk,u; Qp,c)
        // this loop takes minimum of two numbers (generated energy, energy needed for water heating) 
        // and insert into a vector, which means actual usable solar energy gains (Qss,u)
        for (int i = 0; i < 12; ++i)
        {   
             std::cout  << "Month: " << i+1 << " >> " << std::min(theoreticallyUsableGains[i], WaterConsumptionYearVector[i]) << " kWh" << std::endl;
            float min = std::min(theoreticallyUsableGains[i], WaterConsumptionYearVector[i]);
            this->actualUsableGains.insert(this->actualUsableGains.end(), min);
        }
    }

    void countDifference()
    {   

        // this for loop counts calculates the difference between the actual usable gains and total heed needed
        for (int i = 0; i < 12; ++i)
        {   
            float dif = (WaterConsumptionYearVector[i] - this->actualUsableGains[i]);
            difference.insert(difference.end(), dif);
        }
    }

    float countSollarCollectorsYearsShare() 
    {   
        float aux = accumulate(actualUsableGains.begin(), actualUsableGains.end(), 0);

        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Solar collectors years share:" << std::endl;
        std::cout << 100 * (aux/WaterConsumptionYear) << " %" << std::endl;

        return 100 * (aux/WaterConsumptionYear);
    }

};


/**
*   @class Savings  
*   
*   @brief counts the payback period of the investment in the collectors
*/
class Savings {

    public:
    float elPriceWithoutPanels;
    float elPriceWithPanels;
    int numOfPanels = NO_OF_COLLECTORS;
    float yearSavings;
    int investmentCosts = ONE_SOLAR_COLLECTOR_PRICE * NO_OF_COLLECTORS + PUMP + EXPANSION_TANK * 2;

    std::vector<float> difference; 
    std::vector<float> WconsumptionYear;

    Savings(int numOfPanels, std::vector<float> difference, std::vector<float> WconsumptionYear)
    {
        this->numOfPanels = numOfPanels;
        this->difference = difference;
        this->WconsumptionYear = WconsumptionYear;
    }

    void countElectricEnergyPriceWithPanels()
    {
        this->elPriceWithPanels = std::accumulate(difference.begin(), difference.end(), 0) * 1.02  * EL_ENERGY_PRICE_KWH;

        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Electric energy price with collectors:" << std::endl;
        std::cout << this->elPriceWithPanels << " CZK" << std::endl;
    }

    void countElectricEnergyPriceWithoutPanels()
    {
        this->elPriceWithoutPanels = std::accumulate(WconsumptionYear.begin(), WconsumptionYear.end(), 0) * 1.02 * EL_ENERGY_PRICE_KWH;

        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Electric energy price without collectors:" << std::endl;
        std::cout <<  this->elPriceWithoutPanels << " CZK" << std::endl;
    }

    void countYearSavings()
    {
        this->yearSavings = elPriceWithoutPanels - elPriceWithPanels;

        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Year savings:" << std::endl;
        std::cout << this->yearSavings << " CZK" << std::endl;
    }

    float countReturnOfInvestments()
    {   
        float years = investmentCosts / yearSavings;
        float months = years*12;
        float days = years*12*31;
        
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Return of investments:" << std::endl;
        std::cout << (int)years << " Years, " << int(months)%12 << " Months" <<  " and " << int(days)%31 << " Days" << std::endl;

        return years; 
    }

};


int main(int argc, char **argv) 
{   
    int azi = 0;
    int slo = 0;
    // argument parsing
    for(;;)
        {
        switch(getopt(argc, argv, "a:s:")) 
        {
            case 'a':
            azimuth = std::stoi(optarg);
            azi = std::stoi(optarg);

            continue;

            case 's':
            slope = std::stoi(optarg);
            slo = std::stoi(optarg);

            continue;

            default :
            break;
        }

        break;
    }

    std::cout << "==================================================" << std::endl;
    std::cout << " >> Running simulation for parameters >> azimuth: " << azi << " and slope: " << slo << std::endl;

    // counts monthly water consumption of 4 persons for a one year
    WaterConsumption consumption(WATER_CONSUMPTION_PER_PERSON);

    std::vector<float> WaterConsumptionYear = consumption.countOneYear();
    
    float aux = std::accumulate(WaterConsumptionYear.begin(), WaterConsumptionYear.end(), 0);
    SolarCollector collector(AZIMUTH_SLOPE, SUM_OF_RADIATION_INCLINED_SURFACE, aux, WaterConsumptionYear);

    collector.countTheoreticallyUsableGainsYear();
    collector.countActualUsableGains();
    collector.countSollarCollectorsYearsShare();
    collector.countDifference();

    Savings savings(4, collector.difference, WaterConsumptionYear);

    savings.countElectricEnergyPriceWithPanels();
    savings.countElectricEnergyPriceWithoutPanels();
    savings.countYearSavings();
    savings.countReturnOfInvestments();

    std::cout << "---------------------------------------" << std::endl;

    return 0;
}