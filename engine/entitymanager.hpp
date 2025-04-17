//**************************************************************************
//*************************     ENTITY MANAGER      ************************
//**************************************************************************
#pragma once
#include <vector>
#include <stdio.h>
#include <cstdint>
#include "componentStorage.hpp"

//11001                   
template <typename CMPS, typename TAGS, std::size_t Capacity = 10>
struct ManEntity {
    static constexpr std::size_t maxEntities = Capacity;
    using cmps_info = component_traits<CMPS>;
    using tags_info = tags_traits<TAGS>;
    using storage_type = ComponentStorage<CMPS, maxEntities>;

    template <typename T>
    using to_key_type = typename Slotmap<T, Capacity>::key_type;
    using key_type_list = replace_key_type_t<to_key_type, CMPS>;
    using key_type_tuple = replace_t<std::tuple, key_type_list>;
    ManEntity(){
        entityarray.reserve(Capacity);
    }
    ~ManEntity(){
        entityarray.clear();
    }

    struct Entity { 
        Entity(){
            cmps_mask = 0;
            tags_mask = 0;
        }
        template <typename CMP>
        void addComponent (to_key_type<CMP> key){
            cmps_mask |= cmps_info::template mask<CMP>();
            std::get< to_key_type<CMP> >(cmps_keys) = key;
        }
        template <typename TAG>
        void addTag (){
            tags_mask |= tags_info::template mask<TAG>();
        }
        //freestyle
        template <typename TAG>
        void eraseTag (){
            tags_mask ^= tags_info::template mask<TAG>();
        }

        template <typename CMP>
        auto getComponent() const noexcept{
            return std::get<to_key_type<CMP>>(cmps_keys);
        }

        template <typename CMP>
        bool hasComponent() const noexcept{
            auto m = cmps_info::template mask<CMP>();
            return cmps_mask & m;
        }
        template <typename TAG>
        bool hasTag() const noexcept{
            auto m = tags_info::template mask<TAG>();
            return tags_mask & m;
        }

        auto getCMPMask(){
            return cmps_mask;
        }    

        auto getTAGMask(){
            return tags_mask;
        }    

        std::size_t getID (){
            return id;
        }
        void resetNextID() {
            nextID = 1;
        }
        private:


        std::size_t id {nextID++};
        inline static std::size_t nextID { 1 };
        cmps_info::mask_type cmps_mask {};
        tags_info::mask_type tags_mask {};
        key_type_tuple cmps_keys {};
    };


    template <typename...Ts>
    Entity& createEntity_withCMPS(){ 
        //Entity* e = new Entity();
        auto& e = entityarray.emplace_back();
        FunctionToEachOneInPack<Ts...>(e);
        
        return e;
    }   
    template<typename TAG>
    void addTagToEntity(Entity& e){
        e.template addTag<TAG>();
    }
    
    //template <typename... Ts> void FunctionToEachOneInPack() { addComponentToEntity<>(); }
    template <typename... Ts> 
    void FunctionToEachOneInPack(Entity& e) {  (... , addComponentToEntity<Ts>(e));  }
    //template <typename T, typename... Ts>
    //void FunctionToEachOneInPack(){
    //    if constexpr (sizeof...(Ts) !=0 ){
    //        addComponentToEntity<T>();
    //        FunctionToEachOneInPack< Ts... >();
    //    }else{
    //        addComponentToEntity<T>();
    //    }
    //}


    template <typename CMP>
    CMP& addComponentToEntity(Entity& e){
        
        if (e.template hasComponent<CMP>()){
            return getEntityCMP<CMP>(e);
        }
        //create cmp
        return createCMP<CMP>(e);
    };

    template<typename CMP,typename Data>
    void defineCMP(Entity& e, Data&& dat){
        getEntityCMP<CMP>(e) = dat;
    }

    template <typename CMP>
    CMP& getEntityCMP(Entity const& e){
        assert(e.template hasComponent<CMP>());
        auto key         = e.template getComponent<CMP>();
        auto& slotmap    = components_.template getStorage<CMP>();
            
        return slotmap[key];
    }
    template <typename CMP>
    CMP const& getEntityCMP(Entity const& e) const{
        
        auto key        = e.template getComponent<CMP>();
        auto& slotmap    = components_.template getStorage<CMP>();

        return slotmap[key];
    }

    void forall(auto&& function){
        for (auto& e : entityarray){
            function(e);
        }
    };
    template <typename CMPs, typename TAGs>
    void foreach(auto&& function){
        foreach_impl(function, CMPs{}, TAGs{});
    };

    void resetEntityArray(){ entityarray.clear(); }

    storage_type& getComponentStorage(){
        return components_;
    }
    std::vector<Entity>& getEntityArray(){
        return entityarray;
    }
    std::vector<Entity*>& getInventory(){
        return inventory;
    }


    Entity& getEntityByID(std::size_t const id){
        std::size_t cont = 0;
        for (std::size_t i=0; i < entityarray.size(); ++i){
            if (id == entityarray[i].getID()){
                return entityarray[i]; 
                cont++;
            }
        }
        assert(cont!=0);

        //esto esta puesto para quitar el warning de no return 
        //nunca deberia llegar aqui, por eso el assert de encima
        return entityarray[0];
    }
    
    private:

        template <typename CMP>
        CMP& createCMP(Entity& e){
            auto& storage = components_.template getStorage<CMP>();
            auto key = storage.push_back(CMP{});
            e.template addComponent<CMP>(key);
            
            return storage[key];
        }

        template <typename... CMPs, typename... TAGs>
        void foreach_impl(auto&& function, Typelist<CMPs...>, Typelist<TAGs...>){
            for (auto& e : entityarray){
                auto hasCMPs = (true && ... && e.template hasComponent<CMPs>());
                auto hasTAGs = (true && ... && e.template hasTag<TAGs>());
                if(hasCMPs && hasTAGs){
                    function(e, getEntityCMP<CMPs>(e)...);
                }
            }
        }

        std::vector<Entity> entityarray{}; 
        storage_type components_{}; 
        std::vector<Entity*> inventory{};
};


