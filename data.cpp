/**
*   IMS Project - solar collectors and their efficiency
*   
*   @author Tadeáš Kachyňa, <xkachy00@stud.fit.vutbr.cz>
*   @date 4/12/2022
*   @file data.cpp
*
*   @brief This technical standardization information presents a simplified calculation procedure 
*    for the energy assessment of solar thermal systems in basic applications (hot water preparation,
*    heating, pool water heating) under uniform boundary conditions for calculating the monthly balance.
*
*    The data are taken from the standardization and quoted in the documentation
*/

#include <iostream>
#include <vector>
#include <stdint.h>
#include "data.hpp"

std::vector<int> AzimuthAndSlope(uint8_t azimuth, uint8_t slope)
{    
    if (azimuth == 0)
    {   
        switch(slope) 
        {
            case 0:  return {175,253,365,446,498,514,501,462,388,285,195,150}; break;
            case 15: return {273,353,448,501,535,545,534,508,457,375,290,245}; break;
            case 30: return {356,434,506,529,543,546,538,526,501,444,369,325}; break;
            case 45: return {418,489,535,527,521,517,512,515,516,488,427,387}; break;
            case 60: return {454,514,533,496,470,460,459,476,503,502,458,424}; break;
            case 75: return {463,509,500,437,394,379,381,411,461,488,462,436}; break;
            case 90: return {443,473,438,355,299,279,285,325,393,444,438,421}; break;
        }
       
    }
    else if (azimuth == 15)
    {   
        switch(slope) 
        {   
            case 0:   return {175,253,365,446,498,514,501,462,388,285,195,150}; break;
            case 15:  return {270,350,445,499,534,543,533,506,454,372,287,241}; break;
            case 30:  return {349,427,500,525,540,543,535,522,496,438,363,319}; break;
            case 45:  return {408,479,526,521,516,512,508,509,509,479,417,377}; break;
            case 60:  return {442,502,522,488,464,455,453,469,494,491,447,413}; break;
            case 75:  return {449,495,488,428,388,373,375,403,451,475,449,423}; break;
            case 90:  return {429,459,426,346,292,273,279,317,383,431,425,408}; break;
        }
    }
    else if (azimuth == 30)
    {   
        switch(slope) 
        {   
            case 0:  return {175,253,365,446,498,514,501,462,388,285,195,150}; break;
            case 15:  return {259,339,435,492,529,539,528,500,446,362,277,232}; break;
            case 30:  return {329,406,482,512,530,534,526,511,480,419,343,300}; break;
            case 45:  return {380,449,501,502,502,500,495,494,487,452,390,351}; break;
            case 60:  return {408,466,491,465,447,440,438,449,467,459,413,380}; break;
            case 75:  return {411,455,453,403,369,356,358,382,421,439,412,387}; break;
            case 90:  return {390,418,390,320,273,256,261,294,352,394,386,370}; break;
        }
    }
    else if (azimuth == 45)
    {
        switch(slope) 
        {   
            case 0:   return {175,253,365,446,498,514,501,462,388,285,195,150}; break;
            case 15:  return {243,322,421,482,521,532,521,491,434,347,261,216}; break;
            case 30:  return {298,373,453,491,514,520,512,493,456,389,313,270}; break;
            case 45:  return {336,403,460,473,480,481,475,468,453,410,346,308}; break;
            case 60:  return {353,409,442,430,420,416,413,419,425,407,360,328}; break;
            case 75:  return {350,392,398,363,339,330,331,347,374,381,353,329}; break;
            case 90:  return {327,352,333,279,242,229,233,259,303,334,325,310}; break;
        }
    }

    return {0};
}

std::vector<float> radiationSumInclinedSurface(uint8_t slope)
{
    switch(slope)
    {
        case  0:  return {27,48,99,133,185,201,196,159,111,54,26,19,54,26,19}; break;
        case 15:  return {27,56,112,146,199,214,210,175,128,68,32,22,68,32,22}; break;
        case 30:  return {38,64,122,154,208,219,220,118,140,78,36,25,78,36,25}; break;
        case 45:  return {41,70,125,156,200,213,217,186,145,85,39,26,85,39,26}; break;
        case 60:  return {42,72,124,145,181,191,191,174,142,89,41,27,89,41,27}; break;
        case 75:  return {43,72,119,130,150,152,157,153,136,89,42,27,89,42,27}; break;
        case 90:  return {43,69,108,110,113,111,118,128,123,84,41,26,84,41,26}; break;
    }

    return {0};
}


