/**
*   IMS Project - solar collectors and their efficiency
*   
*   @author Tadeáš Kachyňa, <xkachy00@stud.fit.vutbr.cz>
*   @date 4/12/2022
*   @file data.hpp
*
*   @brief This technical standardization information presents a simplified calculation procedure 
*    for the energy assessment of solar thermal systems in basic applications (hot water preparation,
*    heating, pool water heating) under uniform boundary conditions for calculating the monthly balance.
*/

#include <vector>
#include <stdint.h>

std::vector<int> AzimuthAndSlope(uint8_t azimuth, uint8_t slope);

std::vector<float> radiationSumInclinedSurface(uint8_t slope);