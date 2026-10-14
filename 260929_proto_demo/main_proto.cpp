#include <iostream>
#include <string>
#include "app_config.pb.h"

// 外部函数，参数传const引用，不需要修改proto生成的类
bool ComponentStartup(const ComponentConfig& comp_cfg) {
    if (!comp_cfg.enable()) {
        std::cout << "组件[" << comp_cfg.name() << "] enable=false，跳过启动\n";
        return false;
    }
    std::cout << "组件[" << comp_cfg.name() << "] 启动成功\n";
    return true;
}

int main() {
    // 1. 构造AppConfig对象，填充protobuf数据
    AppConfig app_cfg;
    // 填充 optional ParamServerConfig
    ParamServerConfig* ps_cfg = app_cfg.mutable_param_server();
    ps_cfg->set_host("127.0.0.1");
    ps_cfg->set_port(8080);
    // 往 repeated component 数组添加2个组件
    ComponentConfig* comp1 = app_cfg.add_component();
    comp1->set_name("DMS");
    comp1->set_enable(true);
    ComponentConfig* comp2 = app_cfg.add_component();
    comp2->set_name("ALGO");
    comp2->set_enable(false);
    // ========= 2. 遍历repeated字段 =========
    std::cout << "\n===== 遍历所有组件 =====\n";
    for (int32_t i = 0; i < app_cfg.component_size(); ++i) {
        auto& comp_cfg = app_cfg.component(i);
        ComponentStartup(comp_cfg); // 调用外部函数
    }
    // ========= 3. 序列化到字符串 =========
    std::string serialize_str;
    if (app_cfg.SerializeToString(&serialize_str)) {
        std::cout << "\n序列化成功，字节长度:" << serialize_str.size() << "\n";
    }
    // ========= 4. 反序列化 =========
    AppConfig load_cfg;
    if (load_cfg.ParseFromString(serialize_str)) {
        std::cout << "\n===== 反序列化后读取配置 =====\n";
        std::cout << "param server host:" << load_cfg.param_server().host()
                  << " port:" << load_cfg.param_server().port() << "\n";
    }
    return 0;
}
