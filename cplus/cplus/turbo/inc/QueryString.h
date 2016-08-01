#ifndef __TURBO_QUERYSTRING_H__
#define __TURBO_QUERYSTRING_H__

#include <string>
#include <vector>
using namespace std;

namespace turbo {

typedef struct _Pair {
    string key;
    string value;
} Pair_t;

class QueryString {
public:
    QueryString(const char* querystring);
    ~QueryString() {}

    size_t getItems(vector<Pair_t>& iems);

    static void UnitTest();

private:
    vector<Pair_t> _items;
};

} // namespace turbo

#endif // __TURBO_QUERYSTRING_H__
