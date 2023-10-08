#ifndef COMMON_STRUCT
#define COMMON_STRUCT

#ifdef __riscv64__
#include "map.h"
#endif
#include "compose_keys.hxx"


//Номера структур данных в SPE
enum Structures : uint32_t {
    null            = 0,   	//Нулевая структура не используется
    users_pnum    	= 1,	//Таблица 1 
    resources_pnum  = 2		//Таблица 2 
};

#ifdef __riscv64__
//Задание даипазонов и курсоров
template<typename Range>
struct reverse {
        Range r;
        [[gnu::always_inline]] reverse(Range r) : r(r) {}
        [[gnu::always_inline]] auto begin() {return r.rbegin();}
        [[gnu::always_inline]] auto end() {return r.rend();}
};

template<typename K, typename V>
struct Handle {
        bool ret_val;
        K k{get_result_key<K>()};
        V v{get_result_value<V>()};
        [[gnu::always_inline]] Handle(bool ret_val) : ret_val(ret_val) {
        }

        [[gnu::always_inline]] operator bool() const {
                return ret_val;
        }

        [[gnu::always_inline]] K key() const {
                return k;
        }

        [[gnu::always_inline]] V value() const {
                return v;
        }
};
#endif


//////////////////////////////////////
// Описание формата ключа и значения
//////////////////////////////////////


struct users {
	using vertex_t = uint32_t;
    int struct_number;
    constexpr users(int struct_number) : struct_number(struct_number) {}
	static const uint32_t idx_bits = 32;
	static const uint32_t idx_max = (1ull << idx_bits) - 1;
	static const uint32_t idx_min = idx_max; 

	//Запись для формирования ключей (* - наиболее значимые биты поля)
	STRUCT(key)
	{
	    uint32_t	idx	    :idx_bits;	//Поле 0:
	    uint32_t	user    :32; 		//Поле 1*
	};

	//Запись для формирования значений
	STRUCT(val)
	{
	    uint32_t	role	:32;		//Поле 0:
	    time_t		time    :32; 		//Поле 1*
	};
	//Обязательная типизация
	#ifdef __riscv64__
    DEFINE_DEFAULT_KEYVAL(key, val)
	#endif
};

constexpr users USERS(Structures::users_pnum);

#endif //COMMON_STRUCT

