#ifndef MAIN_H
#define MAIN_H
#ifdef __cplusplus

/*
#include <cstring>
#include <stdexcept>
#include <assert.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <sys/types.h>

#include <memory>
#include <iomanip>
#include <list>
#include <map>
#include <vector>
#include <set>
#include <array>
#include <tuple>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <string>
#include <limits>

#include <functional>
#include <memory>

#include <boost/any.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string/predicate.hpp>
*/


#define DECLARE_CLASS_PTR(type) \
    class type; \
    typedef std::shared_ptr<type> type##Ptr; \
    typedef const std::shared_ptr<type> type##CPtr; \
    typedef std::weak_ptr<type> type##WPtr;

#define DECLARE_NS_CLASS_PTR(ns, type) \
    namespace ns { \
    class type; \
    typedef std::shared_ptr<type> type##Ptr; \
    typedef const std::shared_ptr<type> type##CPtr; \
    typedef std::weak_ptr<type> type##WPtr; \
    }


#endif
#endif
