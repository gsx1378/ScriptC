#include <iostream>
#include <map>
#include <string>


/*
`std::map` 是**容器（container）**，用来存放**多条 key-value 成对的数据**，可以存很多条记录。
每一条元素类型是 `std::pair<const Key, T>`。
1. 里面的元素是**有序存放**（红黑树维持顺序，按 key 从小到大）
2. **key 不允许重复**：同一个 key 只能有一条记录；如果给同一个 key 多次赋值，会覆盖旧 value
1. `comp_map.find()` 返回 **迭代器**；`*iter` 才是 `std::pair<const string, string>`
2. `comp_map.end()` 返回**尾后迭代器**，不代表真实元素，只作为查找失败的标记
3. 判断查找成功：`iter != comp_map.end()`
*/


std::map<std::string, std::string> GetRegistry()
{
	std::map<std::string, std::string> map_;
	return map_;
}


int main() {
    // key:string 组件名；value:string 组件描述
    std::map<std::string, std::string> comp_map;

    // 插入
    comp_map["DMS"] = "驾驶员监控组件";
    comp_map["YOLO"] = "目标检测组件";
    comp_map["ADAS"] = "辅助驾驶组件";

    // 遍历：std::map 会自动按 key 字符串升序输出！
    std::cout << "遍历map：\n";
    for(auto& item : comp_map) {
        // item 是 std::pair<const string, string>
        std::cout << item.first << " : " << item.second << '\n';
    }

    // 查询
    auto iter = comp_map.find("DMS");
    if(iter != comp_map.end()) {
        std::cout << "找到DMS: " << iter->second << '\n';
    }
    
    
    /******************************************************/
		auto map2 = GetRegistry();
    map2["Test"] = "Test value";
    auto iter2 = map2.find("Test");
    if(iter2 != map2.end()) {
        std::cout << "Test: " << iter2->second << '\n';
    }
    
    

    return 0;
}


