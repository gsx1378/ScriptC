#include <iostream>
#include <functional> // std::function
#include <memory>     // std::shared_ptr 关键！漏掉就报你这个错
#include <string>

int main()
{
    // ========== 1. 最简单lambda：无参数，无捕获 ==========
    auto hello = [](){
        std::cout << "hello lambda\n";
    };
    hello(); // 调用，和普通函数一样

    // ========== 2. 带参数的lambda ==========
    auto add = [](int a, int b){
        return a + b;
    };
    std::cout << "1+2 = " << add(1,2) << "\n";

    // ========== 3. 捕获外部变量【重点！】==========
    int num = 100;
    // [=] 按值捕获：拷贝一份num到lambda内部，外面num改了里面不受影响
    auto print_val = [=](){
        std::cout << "num = " << num << "\n";
    };
    print_val();

    // [&] 按引用捕获：直接引用外面变量，lambda内部修改会影响外部
    auto add_ref = [&](int x){
        num += x;
    };
    add_ref(50);
    std::cout << "after add_ref num = " << num << "\n";

    // ========== 4. lambda存到 std::function，就是你组件注册表的用法！==========
    using CreatorFunc = std::function<std::shared_ptr<std::string>()>;

    CreatorFunc maker = [](){
        return std::make_shared<std::string>("组件实例");
    };

    auto ptr = maker(); // 调用工厂函数，创建对象
    std::cout << *ptr << std::endl;

    int a = 10;
    auto f = [=](){ std::cout << a; };
    f();


    return 0;
}


// 


