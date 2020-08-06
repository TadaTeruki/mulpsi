/*
mulp-si
Copyright (C) 2020  Tada Teruki [JPN : 多田 瑛貴 (ただ てるき)]

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#include <set>
#include <cassert>
#include <compare>
#include <initializer_list>
#include <unordered_map>
#include <unordered_set>

namespace spacial {

template <class INDEX, class VALUE, unsigned int DIMENTION = 2>
class mulpsi : public std::unordered_set<INDEX> {

    struct m_index;
    struct m_flag;
    struct m_access;

    using m_container = std::unordered_set<INDEX>;
    using m_iterator = typename std::multiset<m_index>::iterator;

    VALUE extrange[2][DIMENTION];
    m_iterator iter[2][DIMENTION];
    std::multiset<m_index> data[2][DIMENTION];
    std::unordered_map<INDEX, m_flag> flag_data;

    struct m_index {
        INDEX index;
        VALUE value;
        friend bool operator<(m_index a, m_index b) {
            return a.value < b.value;
        }
    };

    struct m_flag {
        bool flag[2][DIMENTION];
        constexpr bool extractable() {
            for (m_access ac; ac.enable(); ac.inc()) {
                if (flag[ac.f][ac.b] == false)
                    return false;
            }
            return true;
        }
    };

    struct m_access {
        bool         f;
        unsigned int b;
        
        m_access() {
            f = false;
            b = 0;
        }

        constexpr void inc() {
            if(++b == DIMENTION){
                if(f == false){
                    f = true;
                    b = 0;
                }
                else
                    b = DIMENTION;
            }
        }

        bool enable() { return b != DIMENTION; }
    };

    void update_extrange(const std::initializer_list<VALUE> &_values) {
        auto ins_iter = _values.begin();
        for (m_access ac; ac.enable(); ac.inc()) {
            extrange[ac.f][ac.b] = *ins_iter;
            ins_iter++;
        }
    }

    void init() {
        for (m_access ac; ac.enable(); ac.inc())
            data[ac.f][ac.b].clear();
        flag_data.clear();
        m_container::clear();
    }

    void slide_insertion(const m_access _ac) {
        flag_data[(*iter[_ac.f][_ac.b]).index].flag[_ac.f][_ac.b] = true;
        if (flag_data[(*iter[_ac.f][_ac.b]).index].extractable() == true)
            m_container::insert((*iter[_ac.f][_ac.b]).index);
    }

    void slide_deletion(const m_access _ac) {
        if (flag_data[(*iter[_ac.f][_ac.b]).index].extractable() == true)
            m_container::erase((*iter[_ac.f][_ac.b]).index);
        flag_data[(*iter[_ac.f][_ac.b]).index].flag[_ac.f][_ac.b] = false;
    }

    bool slide_comp(const m_access _ac){
        const std::partial_ordering comp =
            (*iter[_ac.f][_ac.b]).value <=> extrange[1 - _ac.f][_ac.b];
        return comp > 0 or (_ac.f == true and comp == 0);
    }

    constexpr void slide_pointer() {

        if (size() == 0)
            return;

        for (m_access ac; ac.enable(); ac.inc()) {
            
            while(iter[ac.f][ac.b] != data[ac.f][ac.b].begin()){
                iter[ac.f][ac.b]--;
                if(slide_comp(ac) == true)
                    ac.f == false ? slide_deletion(ac):slide_insertion(ac);
                else break;
            }
            
            while( (iter[ac.f][ac.b] != data[ac.f][ac.b].end()--) and slide_comp(ac) == false){
                ac.f == false ? slide_insertion(ac):slide_deletion(ac);
                iter[ac.f][ac.b]++;
            }

        }


    }

    public:

    mulpsi() {
        for (m_access ac; ac.enable(); ac.inc())
            extrange[ac.f][ac.b] = 0;
        init();
    }

    void init(const std::initializer_list<VALUE> _values) {
        update_extrange(_values);
        init();
    }

    const size_t size() { return data[0][0].size(); }

    const size_t extracted_size() { return m_container::size(); } 

    void insert(const INDEX _index, const std::initializer_list<VALUE> _values) {

        auto ins_iter = _values.begin();
        bool register_iter = (size() == 0);

        m_flag iflag;

        for (m_access ac; ac.enable(); ac.inc()) {
            data[ac.f][ac.b].insert(m_index{_index, *ins_iter});

            if ((ac.f == 0) == ((*ins_iter) < extrange[1 - ac.f][ac.b]))
                iflag.flag[ac.f][ac.b] = true;
            else
                iflag.flag[ac.f][ac.b] = false;

            ins_iter++;
        }

        flag_data.emplace(_index, iflag);

        if (iflag.extractable() == true)
            m_container::insert(_index);

        if (register_iter == true) {
            for (m_access ac; ac.enable(); ac.inc())
                iter[ac.f][ac.b] = data[ac.f][ac.b].begin();
        }
        slide_pointer();

    }

    void erase(const INDEX _index) {

        for (m_access ac; ac.enable(); ac.inc()) {
            if ((*iter[ac.f][ac.b]).index == _index) {
                iter[ac.f][ac.b]++;
            }
            for (auto it = data[ac.f][ac.b].begin();
                 it != data[ac.f][ac.b].end(); it++) {
                if ((*it).index == _index) {
                    data[ac.f][ac.b].erase(it);
                    break;
                }
            }
        }

        flag_data.erase(_index);
        m_container::erase(_index);

        if (size() == 0)
            return;

        for (m_access ac; ac.enable(); ac.inc()) {
            if (iter[ac.f][ac.b] == data[ac.f][ac.b].end()) {
                iter[ac.f][ac.b]--;
            }
        }
        slide_pointer();
    }

    void slide_pointer(const std::initializer_list<VALUE> _values) {
        update_extrange(_values);
        slide_pointer();
    }
};
} // namespace spacial
