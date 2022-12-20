#include <iostream>
#include "console.h"
#include "simpio.h"
#include "OurHashMap.h"
#include "testing/SimpleTest.h"
using namespace std;

int main()
{
//    if (runSimpleTests(SELECTED_TESTS))
//    {
//        return 0;
//    }
    OurHashMap<int,string> map;
    map.put(1,"hhh");
    map.put(2,"aaa");
    map.put(3,"www");
    map.put(4,"rrr");
    map.put(5,"ggg");
    map.put(6,"ttt");
    map.put(7,"ddd");
    map.put(8,"vvv");
    map.put(9,"eee");
    map.put(11,"fff");
    map.put(12,"sss");
    map.put(20,"nnn");
    cout<<"map.containsKey(5)"<<map.containsKey(5)<<endl;
    map.remove(5);
    cout<<"map.containsKey(5) after map.remove(5)"<<map.containsKey(5)<<endl;
    cout<<"map.isEmpty()"<<map.isEmpty()<<endl;
    cout<<"map.size()"<<map.size()<<endl;
    cout<<"map.keys()"<<map.keys()<<endl;
    cout<<"map.values()"<<map.values()<<endl;
    cout<<"map[6]"<<map[6]<<endl;
    cout<<"map[40]"<<map[40]<<endl;
    map[30] = "lll";
    cout<<"map[30]"<<map[30]<<endl;

    OurHashMap<int,string> map2(map);
    cout<<"map2[30]"<<map2[30]<<endl;

    map.clear();
    cout<<"map.size() after map.clear()"<<map.size()<<endl;

    return 0;
}
