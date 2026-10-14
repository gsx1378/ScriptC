#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <unistd.h>   // access
#include <sys/stat.h> // stat, S_ISREG
#include <sstream>

// protobuf头文件
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
// #include <google/protobuf/any.pb.h>

// protobuf类文件
// #include "proto/app_config.pb.h"
#include "proto/app.pb.h"
#include "proto/component.pb.h"




// 宏定义：打印字段，自动判断has_xxx
#define PRINT_PROTO_FIELD(obj, field_name) \
do { \
  if(obj.has_##field_name()){ \
    std::cout << #field_name << " = " << obj.field_name() << "\n"; \
  }else{ \
    std::cout << #field_name << " = [NOT SET]\n"; \
  } \
}while(0)

#define COUT_LOG(v1) \
do { \
    std::cout << #v1 << " = " << v1 << std::endl; \
}while(0)

#define COUT_LOG_IDX(param, idx, len) \
do { \
    std::cout << std::format("{}[{}/{}] = {}\n", #param, idx, len, param); \
}while(0)


// 读取整个二进制文件到字符串out
size_t read_file(const std::string &path, std::string &out) {
  out.clear();
  if (0 != access(path.c_str(), F_OK | R_OK)) {
    std::cerr << "ERROR: can not access file: " << path << std::endl;
    return 0;
  }

  struct stat path_stat;
  if (stat(path.c_str(), &path_stat) == 0) {
    if (!S_ISREG(path_stat.st_mode)) {
      std::cerr << "ERROR: path is not a regular file: " << path << std::endl;
      return 0;
    }
  } else {
    std::cerr << "ERROR: stat failed for file: " << path << std::endl;
    return 0;
  }

  std::ifstream fs(path, std::ios_base::binary);
  if (!fs.is_open()) {
    std::cerr << "ERROR: open failed: " << path << std::endl;
    return 0;
  }

  fs.seekg(0, std::ios_base::end);
  auto size = fs.tellg();
  fs.seekg(0);

  if (size < 0) {
    std::cerr << "ERROR: tellg get file size error" << std::endl;
    return 0;
  }

  out.resize(static_cast<size_t>(size));
  fs.read(&out[0], static_cast<std::streamsize>(size));

  return out.size();
}

int read_pbtxt(size_t size, std::string &data)
{
	  if (size <= 0) {return -1;}
	  
	  // 定义protobuf消息对象
		dyalgo::AppConfig config;
		config.Clear(); // 解析前清空！

	  // 字符串输入流
		std::istringstream iss(data);
		// 将字符串输入流，转为protobuf的输入流
		google::protobuf::io::IstreamInputStream input(&iss);
		// 将protobuf的输入流，转换为文本格式
		bool ret = google::protobuf::TextFormat::Parse(&input, &config);
  
		// 打印config内容
		if (ret) {
		  // 打印全部内容
		  // std::cout << "config content:\n" << config.DebugString() << std::endl;

		  // 紧凑单行版本
		  // std::cout << config.ShortDebugString() << std::endl;
		  
		  // 按字段访问： 普通字段(基础变量)
			PRINT_PROTO_FIELD(config, xml_path);
			PRINT_PROTO_FIELD(config, backtrack_path);
		  PRINT_PROTO_FIELD(config, udisk_path);

      // 按字段访问： 列表字段(repeated)
      int icomsize = config.component_size();
      int icfgsize = config.config_pattern_size();

      COUT_LOG(icomsize);
      COUT_LOG(icfgsize);
      for(int i = 0;i<icfgsize;i++)
      {
        const auto& config_pattern = config.config_pattern(i);
        printf("config_pattern[%d/%d] = %s\n", i, icfgsize, config_pattern.c_str());
      }

      for(int i = 0;i<icomsize;i++)
      {
        const auto& comp = config.component(i);
        PRINT_PROTO_FIELD(comp, name);
        PRINT_PROTO_FIELD(comp, type);
        PRINT_PROTO_FIELD(comp, class_name);
        PRINT_PROTO_FIELD(comp, library_path);
        PRINT_PROTO_FIELD(comp, start_delay);
        PRINT_PROTO_FIELD(comp, config_path);
        PRINT_PROTO_FIELD(comp, startup);
        PRINT_PROTO_FIELD(comp, priority);
        printf("-------------------------------\n");
        // printf("config_pattern[%d/%d] = %s\n", i, icfgsize, config_pattern.c_str());
      }

      // 按字段访问： 嵌套Message访问
      if (config.has_param_server()){
          // 获取嵌套对象引用
          const auto& ps_cfg = config.param_server();
          std::cout << "host = " << ps_cfg.host() << std::endl;
          std::cout << "port = " << ps_cfg.port() << std::endl;
      }


		}
  
  	return ret ? 0 : -1;
}

int main(int argc, char** argv) {
    std::string file_path = argv[1];
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return -1;
    }
    else{
    	std::cout << "Usage: " << argv[0] << " " << file_path.c_str() << std::endl;
    }
    

    std::string content;
    size_t read_len = read_file(file_path, content);
    
    // protobuf解析
    bool iret = read_pbtxt(read_len, content);
    /*
    std::cout << "read bytes: " << read_len << std::endl;
    if (read_len > 0) {
        std::cout << "first 10 bytes preview:";
        size_t preview_len = std::min((size_t)10, content.size());
        for(size_t i = 0; i < preview_len; i++){
            printf(" %02x", (unsigned char)content[i]);
        }
        std::cout << "\n";
    }
    */
    return 0;
}

