
#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <cassert>

/****************************************************************************************
*****************************************************************************************
* **************GUIDE FOR UNDERSTAND AND USE SLOTMAPS************************************
*
*-----GLOSSARY
*           using           -> creates a personalized type to use and latter if i want to 
*                              change the types of my Slotmap i will do without any problem  
*           Datatype        -> The type used for the implementation. Can be any type 
*                              (float, EntityComponents ...)
*           [[nodiscard]]   -> Have to use it. If you dont use it , an error will appear
*           [[maybe_unused]]-> If the function have a [[nodiscard]] this in other variable 
*                               can discard the result 
*           const           -> Used for functions or variables that should not change
*           constexpr       -> Functions or variables with constexpr means that can be used in 
*                              compilation time and isn't necessary use it in execution time 
*           
*           noexcept        -> Used for functions that should never throw an exception
*           pragma once     -> Makes sure this file is only included once
*
*           && (parameter)  -> When you have && you are going to pass a temporal object to the function
*                              The object only exists in the function.
*           assert()        -> Comprobation for programmers, if an assert fail, there's an error in the code
*-----GUIDE OF USE
*           Slotmap()       -> The constructor. Clean the slotmap and initialize all
*           size()          -> Return size_ , the length of data_
*           capacity()      -> Return Capacity, max space stored for data_
*           clear()         -> Clear the slotmap
*           freelist_init() -> Initializes the freelist
*           push_back(&&)   -> Push in the array data the new DataType, change index, erase, gen, freelist
*           push_back(const&)->Push in the array data not temporal DataTypes
*           erase()         -> clean the slot with a key parameter
*           free()          -> this method is for erase, clean and update the data and updates the freelist
*           is_valid(key_type)-> is a method for check if one key is valid or not
*
*           the form to iterate one slotmap -> for (auto it = component.begin(); it != names.end(); it++) //
*                                              for (auto& cmp : component)                                || component is a slotmap
*                                                                                                         \\
*           operator[] -> like [] in arrray, for catch the data in the position in []
*
*
*
*	 			        Slotmap bucle <for> used in game.cpp
*	
*	for (auto& e : man_e.getComponentStorage().getStorage<InfoComponent>()){
*		std::cout<< " Nombre de la entidad: " << e.entity_name  << std::endl;
*	}
*	
*****************************END GUIDE***************************************************
*****************************************************************************************
*****************************************************************************************/




template <typename DataType, std::size_t Capacity = 10>
struct Slotmap {
public:
    using value_type = DataType;
    using index_type = std::uint32_t;
    using   gen_type = index_type;
    using   key_type = struct { index_type id; gen_type gen; };
    using   iterator = value_type*;
    using  citerator = value_type const*;

    constexpr explicit Slotmap() { clear(); }

    [[nodiscard]] constexpr std::size_t size()      const noexcept { return size_; }
    [[nodiscard]] constexpr std::size_t capacity()  const noexcept { return Capacity; }
    [[nodiscard]] constexpr key_type    push_back(value_type const& newVal) {
        return push_back( value_type{ newVal } ); 
    }
    [[nodiscard]] constexpr key_type    push_back(value_type&& newVal) {
        auto reservedid = allocate();
        auto& slot = indices_[reservedid];
        //Move data
        data_[slot.id]  = std::move(newVal); 
        erase_[slot.id] = reservedid;
        //Key for the user
        auto key { slot };
        key.id = reservedid;
        
        return key;
    }
    constexpr bool clear() noexcept { freelist_init(); size_ = 0; return true;}

    constexpr bool erase(key_type key) noexcept {
        if ( !is_valid(key) ){ return false;};
        //Erase
        free(key);
        return true;    
    }
    [[nodiscard]]constexpr bool is_valid(key_type key) const noexcept{
        if ( key.id >= Capacity || indices_[key.id].gen != key.gen ) {return false;}
        return true;
    }


    constexpr iterator  begin() noexcept { return data_.begin(); }
    constexpr iterator cbegin() const noexcept { return data_.cbegin(); }

    constexpr iterator   end() noexcept { return data_.begin() + size_; }
    constexpr iterator  cend() const noexcept { return data_.cbegin() + size_; }


    value_type& operator[](key_type const& key) noexcept {
        assert(is_valid(key));
        auto dataID = indices_[key.id].id;
        return data_[dataID];
    }
    value_type const& operator[](key_type const key) const noexcept {
        assert(is_valid(key));
        auto dataID = indices_[key.id].id;
        return data_[dataID];
    }

    std::array<value_type, Capacity>& getData(){
        return data_;
    }
    
    auto getFreelist(){
        return freelist_;
    }

private:
    [[nodiscard]]constexpr index_type allocate() {
        if (size_ >= Capacity) throw std::runtime_error(" No space left in the slotmap");
        assert(freelist_ < Capacity);

        //Reserve
        auto slotid = freelist_;
        freelist_ = indices_[slotid].id; //freelist to first slot free
        //Init Slot
        auto& slot = indices_[slotid];
        slot.id = size_;
        slot.gen = generation_;
        //Update space and generation
        ++size_;
        ++generation_;

        return slotid;
    }
    constexpr void free(key_type key) noexcept {
        assert(is_valid(key)); // if here the key is not valid , bad......
        
        auto& slot = indices_[key.id];
        auto dataid = slot.id; //Save id of data slot to check if it is last or not
        //Update freelist
        slot.id = freelist_;
        slot.gen = generation_;
        freelist_ = key.id;
        //Copy data to free slot???
        if ( dataid != size_ - 1 ){
            //data slot is not last, copy last here
            data_[dataid]   = data_[size_ - 1];
            erase_[dataid]  = erase_[size_ - 1];
            indices_[erase_[dataid]].id = dataid;
        }
        //Update size
        --size_;
        ++generation_;
    }
    constexpr void freelist_init() noexcept {
        for(index_type i{}; i < indices_.size(); ++i){
            indices_[i].id = i+1;
        }
        freelist_ = 0;
    }
    index_type                              size_{};
    index_type                              freelist_{};
    gen_type                                generation_{};
    std::array<  key_type, Capacity>        indices_{};
    std::array<index_type, Capacity>        erase_{};
    std::array<value_type, Capacity>        data_{};
};  

