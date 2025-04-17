/*************************************************
************* COMPONENT STORAGE ******************
**************************************************/

#pragma once

#include <cstddef>
#include <cstdint>
#include <tuple>
#include <stdint.h>

#include "utils/slotmap.hpp"
#include "utils/metafunctions.hpp"

template <typename CMPS, std::size_t Capacity = 10> 
struct ComponentStorage {
    using IDtype   = std::uint8_t;
    using MaskType = std::size_t;
    using key_type = struct{std::size_t id; std::size_t gen; };

	using typelist_with_slots = replace_slots_t<Slotmap,Capacity, CMPS>;
	using storage_type = replace_t<std::tuple, typelist_with_slots>;


    using cmps_info = component_traits<CMPS>;

    template <typename CMP>
    [[nodiscard]] constexpr MaskType getId() noexcept { 
        return cmps_info::template id<CMP>();
        //return getId(CMP{}); 
    } 

    template <typename CMP>
    [[nodiscard]] constexpr MaskType getMask() noexcept { 
        return cmps_info::template mask<CMP>();
        //return getMask(getId<CMP>()); 
    } 
    
    template <typename CMP>
    [[nodiscard]] constexpr auto& getStorage() noexcept { 
        constexpr auto id { cmps_info::template id<CMP>() };
        //return std::get<getId<CMP>()>(componentTuple_);
        return std::get<id>(componentTuple_);
    }
    
    template<typename CMP>
    constexpr auto addComponent(CMP c) noexcept {
        return getStorage<CMP>().push_back(c);
    }

    
    void clearCS(){ clearImplementation(typelist_with_slots{}); }


    private:
    template<typename... CS>
    bool clearImplementation(Typelist<CS...>) {
        auto descartar = (true && ... && std::get<CS>(componentTuple_).clear());
        descartar =true;
        return descartar;
    }

    storage_type componentTuple_{};

};
