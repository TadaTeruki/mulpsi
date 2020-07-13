
#include <cassert>
#include <initializer_list>
#include <unordered_map>
#include <unordered_set>

namespace spacial {

template <class INDEX, class VALUE, unsigned int DIMENTION = 2>
class mulp6 : public std::unordered_set<INDEX> {

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
        size_t f, b;
        m_access() {
            f = 0;
            b = 0;
        }

        constexpr void inc() {
            b++;
            f += b / DIMENTION;
            b %= DIMENTION;
        }

        bool enable() { return f < 2; }
    };

    using m_container = std::unordered_set<INDEX>;
    using m_iterator = typename std::multiset<m_index>::iterator;

    VALUE extrange[2][DIMENTION];
    m_iterator iter[2][DIMENTION];
    std::multiset<m_index> data[2][DIMENTION];
    std::unordered_map<INDEX, m_flag> flag_data;

    void update_extrange(std::initializer_list<VALUE> &_values) {
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

    constexpr void slide_insertion(m_access _ac) {
        flag_data[(*iter[_ac.f][_ac.b]).index].flag[_ac.f][_ac.b] = true;
        if (flag_data[(*iter[_ac.f][_ac.b]).index].extractable() == true)
            m_container::insert((*iter[_ac.f][_ac.b]).index);
    }

    constexpr void slide_deletion(m_access _ac) {
        if (flag_data[(*iter[_ac.f][_ac.b]).index].extractable() == true)
            m_container::erase((*iter[_ac.f][_ac.b]).index);
        flag_data[(*iter[_ac.f][_ac.b]).index].flag[_ac.f][_ac.b] = false;
    }

    void slide_pointer() {

        if (size() == 0)
            return;

        for (m_access ac; ac.enable(); ac.inc()) {

            if (iter[ac.f][ac.b] != data[ac.f][ac.b].begin()) {
                iter[ac.f][ac.b]--;
                while ((*iter[ac.f][ac.b]).value > extrange[1 - ac.f][ac.b]) {
                    if (ac.f == 0)
                        slide_deletion(ac);
                    else
                        slide_insertion(ac);
                    if (iter[ac.f][ac.b] != data[ac.f][ac.b].begin())
                        iter[ac.f][ac.b]--;
                    else
                        break;
                }
            }

            if (iter[ac.f][ac.b] != data[ac.f][ac.b].end()--) {
                while ((*iter[ac.f][ac.b]).value <= extrange[1 - ac.f][ac.b]) {
                    if (ac.f == 1)
                        slide_deletion(ac);
                    else
                        slide_insertion(ac);
                    if (iter[ac.f][ac.b] != data[ac.f][ac.b].end()--)
                        iter[ac.f][ac.b]++;
                    else
                        break;
                }
            }
        }
    }

    /**/ public:

    mulp6() {
        for (m_access ac; ac.enable(); ac.inc())
            extrange[ac.f][ac.b] = 0;
        init();
    }

    void init(std::initializer_list<VALUE> _values) {
        update_extrange(_values);
        init();
    }

    size_t size() { return data[0][0].size(); }

    void insert(INDEX _index, std::initializer_list<VALUE> _values) {

        auto ins_iter = _values.begin();
        bool register_iter = (size() == 0);

        unsigned int ifor, ibac;

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

    void erase(INDEX _index) {

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

    void slide_pointer(std::initializer_list<VALUE> _values) {
        update_extrange(_values);
        slide_pointer();
    }
};
} // namespace spacial