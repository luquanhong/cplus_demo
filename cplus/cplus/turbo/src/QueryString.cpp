#include "QueryString.h"
#include "Logger.h"

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

namespace turbo {

QueryString::QueryString(const char* querystring)
{
    _items.clear();

    string qs(querystring);
    qs = trim(qs);
    
    int pos = 0;
    int end_pos = 0;
    bool end = false;
    while (!end) {
        end_pos = qs.find("&", pos);
        if (end_pos < 0) {
            end_pos = qs.length();
            end = true;
        }

        string current_part = qs.substr(pos, end_pos - pos);
        printf("current part %s\n", current_part.c_str());

        // handle current part
        if (current_part.length()) {
            int mark_pos = current_part.find("=");
            if (mark_pos > 0) {
                Pair_t pair;
                pair.key = current_part.substr(0, mark_pos);
                pair.value = current_part.substr(mark_pos + 1, current_part.length() - mark_pos - 1);
                printf("new pair \"%s\"=\"%s\"\n", pair.key.c_str(), pair.value.c_str());
                _items.push_back(pair);
            }
        }

        pos = end_pos + 1;
    }
}

size_t QueryString::getItems(vector<Pair_t>& items)
{
    items.clear();
    for (vector<Pair_t>::iterator it = _items.begin(); it != _items.end(); ++it) {
        items.push_back(*it);
    }
    return _items.size();
}

#define UNIT_TEST_JUDGE(res)  \
    {                         \
        succ_count += (res);  \
        fail_count += !(res); \
    }
void QueryString::UnitTest()
{
    int succ_count = 0;
    int fail_count = 0;
    vector<Pair_t> items;
    size_t size = 0;

    QueryString* q1 = new QueryString("");
    size = q1->getItems(items);
    UNIT_TEST_JUDGE(size == 0);
    delete q1;

    QueryString* q2 = new QueryString("a");
    size = q2->getItems(items);
    UNIT_TEST_JUDGE(size == 0);
    delete q2;

    QueryString* q3 = new QueryString("a=");
    size = q3->getItems(items);
    UNIT_TEST_JUDGE(size == 1);
    int i = 0;
    for (vector<Pair_t>::iterator it = items.begin(); it != items.end(); ++it) {
        if (i == 0) {
            UNIT_TEST_JUDGE(it->key == "a");
            UNIT_TEST_JUDGE(it->value == "");
        }
        ++i;
    }
    delete q3;

    QueryString* q4 = new QueryString("a=1&");
    size = q4->getItems(items);
    UNIT_TEST_JUDGE(size == 1);
    i = 0;
    for (vector<Pair_t>::iterator it = items.begin(); it != items.end(); ++it) {
        if (i == 0) {
            UNIT_TEST_JUDGE(it->key == "a");
            UNIT_TEST_JUDGE(it->value == "1");
        }
        ++i;
    }
    delete q4;

    QueryString* q5 = new QueryString("a=233333&b");
    size = q5->getItems(items);
    UNIT_TEST_JUDGE(size == 1);
    i = 0;
    for (vector<Pair_t>::iterator it = items.begin(); it != items.end(); ++it) {
        if (i == 0) {
            UNIT_TEST_JUDGE(it->key == "a");
            UNIT_TEST_JUDGE(it->value == "233333");
        }
        ++i;
    }
    delete q5;

    QueryString* q6 = new QueryString(" a=1&b= ");
    size = q6->getItems(items);
    UNIT_TEST_JUDGE(size == 2);
    i = 0;
    for (vector<Pair_t>::iterator it = items.begin(); it != items.end(); ++it) {
        if (i == 0) {
            UNIT_TEST_JUDGE(it->key == "a");
            UNIT_TEST_JUDGE(it->value == "1");
        } else if (i == 1) {
            UNIT_TEST_JUDGE(it->key == "b");
            UNIT_TEST_JUDGE(it->value == "");
        }
        ++i;
    }
    delete q6;

    QueryString* q7 = new QueryString("  a=1&b=23333  ");
    size = q7->getItems(items);
    UNIT_TEST_JUDGE(size == 2);
    i = 0;
    for (vector<Pair_t>::iterator it = items.begin(); it != items.end(); ++it) {
        if (i == 0) {
            UNIT_TEST_JUDGE(it->key == "a");
            UNIT_TEST_JUDGE(it->value == "1");
        } else if (i == 1) {
            UNIT_TEST_JUDGE(it->key == "b");
            UNIT_TEST_JUDGE(it->value == "23333");
        }
        ++i;
    }
    delete q7;

    QueryString* q8 = new QueryString(" =  ");
    size = q8->getItems(items);
    UNIT_TEST_JUDGE(size == 0);
    delete q8;

    QueryString* q9 = new QueryString("  a=1&b=&=  ");
    size = q9->getItems(items);
    UNIT_TEST_JUDGE(size == 2);
    i = 0;
    for (vector<Pair_t>::iterator it = items.begin(); it != items.end(); ++it) {
        if (i == 0) {
            UNIT_TEST_JUDGE(it->key == "a");
            UNIT_TEST_JUDGE(it->value == "1");
        } else if (i == 1) {
            UNIT_TEST_JUDGE(it->key == "b");
            UNIT_TEST_JUDGE(it->value == "");
        }
        ++i;
    }
    delete q9;

    string x(" 123");
    x = trim(x);
    UNIT_TEST_JUDGE(x == "123");

    Logger::d("UNITTEST", "UNIT_TEST of QueryString: succ_count=%d, fail_count=%d\n", succ_count, fail_count);
}

} // namespace turbo
