//
// Created by 李寅斌 on 2019/1/10.
//

#include <gtest/gtest.h>
#include <kob/ksl/intrusive_list.h>

struct my_node : public ksl::intrusive_list_node {
    int m;
    my_node(int n = 0) :m(n)
    {
        KOB_ASSERT_MSG(next == nullptr, "next not null");
        KOB_ASSERT_MSG(prev == nullptr, "prev not null");
    }
    operator int() const
    {
        return m;
    }
};

class list_init {
public:
    list_init(ksl::intrusive_list<my_node>& container, my_node* pNodeArray)
            : _list(&container), _array(pNodeArray)
    {
        _list->clear();
    }

    list_init& operator+=(int x)
    {
        _array->m = x;
        _list->push_back(*_array++);
        return *this;
    }

protected:
    ksl::intrusive_list<my_node>* _list;
    my_node*                      _array;
};


TEST(intrusive_list, all)
{
    my_node nd(1);
    ksl::intrusive_list<my_node> l;
    EXPECT_EQ(l.size(), 0);
    l.push_back(nd);
    EXPECT_EQ(l.size(), 1);
}