//
// Created by liyinbin on 2018/11/1.
//

#ifndef KOB_PROFILE_ASSERT_H_
#define KOB_PROFILE_ASSERT_H_
#include <assert.h>

#ifndef KOB_ASSERT_MSG
    #define KOB_ASSERT_MSG(e, m) assert((e)&&(m))
#endif

#ifndef KOB_ASSERT
    #define KOB_ASSERT(e) KOB_ASSERT_MSG(e, #e)
#endif

#ifndef KOB_FAIL_MSG
    #define KOB_FAIL_MSG(m) KOB_ASSERT_MSG(false, (m))

#endif

#ifndef KOB_STATIC_ASSERT_MSG
    #define KOB_STATIC_ASSERT_MSG(e,m) static_assert(e, m)
#endif

#ifndef KOB_STATIC_ASSERT
    #define KOB_STATIC_ASSERT(e) static_assert(e, #e)
#endif

#endif //KOB_PROFILE_ASSERT_H_
