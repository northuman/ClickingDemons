
#pragma once


#include "../cmp/tags.hpp"

#include "../cmp/CmpRender.hpp"

#include "../entitymanager.hpp"


//ADD +1
//TAGS : 34
//CMPS : 16 
using TagList = 
Typelist<TPlayer
        , TEnemy
        , THealth
        >;
using ComponentList = 
Typelist<CmpRender
        >;

using Manentity_type = ManEntity<ComponentList, TagList, 100>;
using Entity = Manentity_type::Entity;

