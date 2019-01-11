//
// Created by 李寅斌 on 2019/1/9.
//

#ifndef KOB_KSL_ALLOCATOR_BASE_H_
#define KOB_KSL_ALLOCATOR_BASE_H_
#include <kob/ksl/detail/config.h>
#include <stddef.h>

namespace ksl {

    template <typename T>
    class allocator_base {
    public:
        typedef  T          value_type;
        typedef  T*         pointer;
        typedef  const T*   const_pointer;
        typedef  T&         reference_type;
        typedef  const T&   const_reference;
        typedef  size_t     size_type;
        typedef  ptrdiff_t  difference_type;

        using is_always_equal = std::true_type;
        using propagate_on_container_move_assignment = std::true_type;

        pointer address(reference x) const KOB_NOEXCEPT
        {
            return std::addressof(x);
        }

        const_pointer address(const_reference x) const KOB_NOEXCEPT
        {
            return std::addressof(x);
        }

        size_type max_size() const KOB_NOEXCEPT
        {
            return size_t(-1) / sizeof(Type);
        }

        void construct(pointer p, const_reference value)
        {
            ::new (static_cast<void*>(p))Type(value);
        }

        void destroy(pointer p) const KOB_NOEXCEPT
        {
            p->~Type();
        }

        virtual pointer allocate(size_type n)
        {
            return ::operator new(sizeof(T) * n);
        }

        virtual void deallocate(pointer p, size_type n)
        {
            ::operator delete p;
        }

        template <typename SubType, typename... Args>
        void construct(SubType* p, Args&& ... args)
        {
            ::new (static_cast<void*>(p))SubType(std::forward<Args>(args) ...);
        }

        template <typename SubType>
        void destroy(SubType* p) const KOB_NOEXCEPT
        {
            p->~SubType();
        }
    };
}
#endif //KOB_KSL_ALLOCATOR_BASE_H_
