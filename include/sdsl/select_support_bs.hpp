/* sdsl - succinct data structures library
    Copyright (C) 2008 Simon Gog

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/ .
*/
/*! \file select_support_bs.hpp
    \brief select_support_bs.hpp contains the select_support_bs class that support a sdsl::bit_vector with constant time select information.
	\author Simon Gog
*/
#ifndef INCLUDED_SDSL_SELECT_SUPPORT_BS
#define INCLUDED_SDSL_SELECT_SUPPORT_BS

#include "int_vector.hpp"
#include "rank_support.hpp"
#include "select_support.hpp"

//! Namespace for the succinct data structure library.
namespace sdsl
{

//! A class supporting select quries by using a rank_support and binary search.
/*!
 * @ingroup select_support_group
 */
template<class RankSupport=rank_support_v<> >
class select_support_bs : public select_support
{
    public:
        typedef typename RankSupport::bit_vector_type bit_vector_type;
    private:
        const RankSupport* m_rs;
        void copy(const select_support_bs& ss);
    public:
        explicit select_support_bs(const bit_vector* v=nullptr, const RankSupport* m_rs=nullptr);
        select_support_bs(const select_support_bs& ss);
        ~select_support_bs() {}

        inline const size_type select(size_type) const;
        //! Alias for select(i).
        inline const size_type operator()(size_type)const;

        size_type serialize(std::ostream& out, structure_tree_node* v=nullptr, std::string name="")const;
        void load(std::istream& in, const bit_vector* v=nullptr);
        void set_vector(const bit_vector* v=nullptr);
        select_support_bs& operator=(const select_support_bs& ss);
        void swap(select_support_bs& ss);
};

template<class RankSupport>
select_support_bs<RankSupport>::select_support_bs(const bit_vector* v, const RankSupport* rs):select_support(v)
{
    m_rs  = rs;
}

template<class RankSupport>
select_support_bs<RankSupport>::select_support_bs(const select_support_bs& ss):select_support(ss.m_v)
{
    copy(ss);
}

template<class RankSupport>
select_support_bs<RankSupport>& select_support_bs<RankSupport>::operator=(const select_support_bs& ss)
{
    copy(ss);
    return *this;
}

template<class RankSupport>
void select_support_bs<RankSupport>::swap(select_support_bs& ss)
{
    std::swap(m_rs, ss.m_rs);
}

template<class RankSupport>
void select_support_bs<RankSupport>::copy(const select_support_bs& ss)
{
    m_v 	= ss.m_v;
    m_rs	= ss.m_rs;
}

template<class RankSupport>
inline const typename select_support_bs<RankSupport>::size_type select_support_bs<RankSupport>::select(size_type i)const
{
    size_type min = i, max = m_v->bit_size()+1; // min included, max excluded
    assert(i <= m_rs->rank(m_v->bit_size()-1) && i>0);
    size_type idx = m_v->bit_size()/2, r;
    // binary search t
    do {
        idx	=	(min+max)>>1;
        r	=	m_rs->rank(idx); // ones in the prefix [0..idx-1]
        if (r>i)
            max = idx;
        else if (r<i)
            min = idx+1;
        else { // rank(idx)==i
            if ((*m_v)[idx-1])
                return idx-1;
            else
                max = idx;
        }
    } while (true);
}



template<class RankSupport>
typename select_support_bs<RankSupport>::size_type select_support_bs<RankSupport>::serialize(SDSL_UNUSED std::ostream& out, structure_tree_node* v, std::string name)const
{
    structure_tree_node* child = structure_tree::add_child(v, name, util::class_name(*this));
    structure_tree::add_size(child, 0);
    return 0;
}

template<class RankSupport>
void select_support_bs<RankSupport>::load(std::istream&, const bit_vector* v)
{
    set_vector(v);
}

template<class RankSupport>
void select_support_bs<RankSupport>::set_vector(const bit_vector* v)
{
    m_v = v;
}

template<class RankSupport>
inline const typename select_support_bs<RankSupport>::size_type select_support_bs<RankSupport>::operator()(size_type i)const
{
    return select(i);
}


}

#endif
