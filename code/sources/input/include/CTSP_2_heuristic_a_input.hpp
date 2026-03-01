#pragma once

#include "CTSP_instance.hpp"
#include "sync_heuristic_a_input.hpp"

namespace CTSP
{

    class CTSP_2_heuristic_a_input : public SYNC_LIB::sync_heuristic_a_input
    {
    public:
        CTSP_2_heuristic_a_input(const CTSP::instance &instance);
        virtual ~CTSP_2_heuristic_a_input();
    };
}