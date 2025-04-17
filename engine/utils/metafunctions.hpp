#pragma once

#include <cstdint>
#include <iostream>

//IS_SAME,  CONSTANT, FOR CHECK IF THE TAGLIST OR CMPLIST HAVE A TAG OR A CMP
//
//
template <typename T, T VAL>
struct constant { static constexpr T value { VAL };};

struct true_type : constant<bool, true>{};
struct false_type: constant<bool, false>{};
////// Is same to compare two elements
//
//
template<typename T, typename U>
struct is_same : false_type{};
template <typename T>
struct is_same<T,T> : true_type{};
template <typename T, typename U>
constexpr bool is_same_v = is_same<T,U>::value;

/////////// TYPE_ID
//
//
template <typename T>
struct type_id { using type = T; };

/////////// NTH_TYPE
//
//
template <std::size_t N,typename... Ts>
struct nth_type { static_assert(sizeof...(Ts) != 0, "ERROR ASSERT NTH");};
template <std::size_t N, typename... Ts>
using nth_type_t = typename nth_type<N, Ts...>::type;
template<typename T, typename... Ts>
struct nth_type<0,T,Ts...> : type_id <T>{};
template  <std::size_t N, typename T, typename... Ts>
struct nth_type<N,T,Ts...> : type_id <nth_type_t<N-1, Ts...> > {};

/////////// POS_TYPE
//
//
template <typename T, typename... Ts>
struct pos_type { static_assert(sizeof...(Ts) != 0); };
template <typename T, typename... Ts>
constexpr std::size_t pos_type_v = pos_type<T, Ts...>::value;
template <typename T, typename... Ts>
struct pos_type<T, T, Ts...> : constant<std::size_t, 0> {};
template <typename T, typename U, typename... Ts>
struct pos_type <T, U, Ts...> : constant< std::size_t, 1 + pos_type_v< T, Ts...> > {};


/////////// TypeList
//
//
template<typename T>
void seetypeM(T){std::cout<< __PRETTY_FUNCTION__ << "\n"; }

template <typename...Ts>
struct Typelist{
    
    consteval static std::size_t size() noexcept { return sizeof...(Ts); }
    template<typename T>
    consteval static bool    contains() noexcept { 
        return (false || ... || is_same_v<T, Ts>);
    }
    template <typename T>
    consteval static std::size_t  pos() noexcept { 
        static_assert(contains<T>());
        return pos_type_v<T, Ts...>;
    }
    template<int Pos>
    static auto getType(){
        nth_type_t<Pos, Ts...> t;

        //seetypeM(hola);
        return t;
    }

};



/////////// REPLACE LIST TO NEW VARIADIC TEMPLATE
// 
// 
template <template <typename...> class New, typename List>
struct replace {};
template <template <typename...> class New, typename... Ts>
struct replace <New, Typelist<Ts...>> : type_id <New<Ts...> > {};
template <template <typename...>class New, typename List>
using replace_t = typename replace<New, List>::type;

///REPLACE IN A LIST ALL PARAMETERS TO SLOTMAP(PARAM, CAPACITY)
//
//
template <template <typename, std::size_t> class New, std::size_t Cap, typename List>
struct replace_slots {};
template <template <typename, std::size_t> class New, std::size_t Cap, typename... Ts>
struct replace_slots<New,Cap, Typelist<Ts...>> : type_id <Typelist <New<Ts, Cap>...>> {};
template <template <typename, std::size_t> class New, std::size_t Cap, typename List>
using replace_slots_t = typename replace_slots<New, Cap, List>::type;




template <template <typename> class New, typename List>
struct replace_key_type {};
template <template <typename> class New, typename... Ts>
struct replace_key_type<New, Typelist<Ts...>> : type_id <Typelist <New<Ts>...>> {};
template <template <typename> class New, typename List>
using replace_key_type_t = typename replace_key_type<New, List>::type;

// typelist <c,c,c> -->    add<c>()      add<c>()
 


////////// IFT (IF FOR TYPES)
// conditional if made with templates
//
template <bool Cond, typename T, typename F>
struct IFT : type_id < F > {};
template <typename T, typename F>
struct IFT<true, T, F> : type_id < T > {};
template <bool Cond, typename T, typename F>
using IFT_t =typename IFT <Cond, T, F>::type; 




///////////TAGS TRAITS  TAGLIST
//
//
template <typename TAGLIST>
struct tags_traits{
    using mask_type = 
    IFT_t<(TAGLIST::size() <= 16)
        , uint16_t
        , IFT_t<(TAGLIST::size()<=32)
            , uint32_t
            , IFT_t<(TAGLIST::size()<=64)
                , uint64_t 
                , std::size_t
            >
        >
    >;

    consteval static std::size_t size() noexcept { return TAGLIST::size(); }
    template <typename TAG>
    consteval static std::size_t   id() noexcept { 
        static_assert(TAGLIST::template contains<TAG>()); //must be true if have an id
        return TAGLIST::template pos<TAG>(); //return the id of the tag
    }
    template <typename TAG>
    consteval static mask_type mask() noexcept { return (1ULL << id<TAG>()); }

};
///////////CMP TRAITS  CMPSLIST
//
//
template <typename CMPSLIST>
struct component_traits : tags_traits<CMPSLIST>{

};
