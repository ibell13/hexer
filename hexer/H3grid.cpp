#include <assert.h>
#include <map>
#include <iostream>

#include <hexer/H3grid.hpp>
#include <hexer/exception.hpp>
#include <hexer/Processor.hpp>

#include <h3/include/h3api.h>

namespace hexer 
{

    // add stuff for processing unknown resolution
    
    void H3Grid::addLatLng(LatLng *ll)
    {
        if(m_res == -1) {
            m_sample.push_back(*ll);
            if (m_sample.size() >= m_maxSample)
                processH3Sample();
            return;
        }

        H3Index index(0);
        H3Error err = latLngToCell(ll, m_res, &index);

        if (err == E_SUCCESS) {
            ++m_map[index];
        }
        else {
            throw hexer_error("H3 index not found!");
        }
    }

    void H3Grid::processGrid()
    {
        for(auto it = m_map.begin(); it != m_map.end();) {
            if(it->second < m_dense_limit)
                it = m_map.erase(it);
            else
                ++it;
        }
        if (m_map.empty()) {
            throw hexer_error("No areas of sufficient density - no shapes. "
                "Decrease density or area size.");
        }
    }

    void H3Grid::processH3Sample()
    {
        if (m_res > 0 || m_sample.empty())
            return;
        double height = computeHexSize(m_sample, m_dense_limit);

        // bins for H3 auto resolution: 
        // - H3 level ~roughly~ equivalent to hexer hexagon size at same edge value
        //     - (since our coords are in degrees, the appropriate values will vary based on 
        //       location. Some way of scaling this by latitude would be more accurate)
        // - does not automatically make very large (>1km^2) or very small (<6m^2) hexagons
        double min_8(2.62E-4);
        double min_9(6.28E-5);
        double min_10(2.09E-5);
        double min_11(8.73E-6);
        double min_12(3.32E-6);
        double min_13(1.4E-6);

        if (min_8 < height)
            m_res = 8;
        else if (min_9 < height && height < min_8)
            m_res = 9;
        else if (min_10 < height && height < min_9)
            m_res = 10;
        else if (min_11 < height && height < min_10)
            m_res = 11;
        else if (min_12 < height && height < min_11)
            m_res = 12;
        else if (min_13 < height && height < min_12)
            m_res = 13;
        else if (height < min_13)
            m_res = 14;
        else
            throw hexer_error("unable to calculate H3 grid size!");
        
        for (auto pi = m_sample.begin(); pi != m_sample.end(); ++pi) {
            LatLng ll = *pi;
            addLatLng(&ll);
        }
        m_sample.clear();
        std::cout << "res: " << m_res <<std::endl; 
    }
} // namespace hexer