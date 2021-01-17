#ifndef COMMON_LOGGER_H_
#define COMMON_LOGGER_H_

#include <string.h>
#include <iostream>

#define LOG_INFO(name) LogInfo(#name, (name))

template<typename T>
void LogInfo(const char* var_name, T* var){
  char addr_buf[200];
  snprintf(addr_buf, 200, "%s%s%s", "Address of ", var_name, ": ");
  char name_addr_buf[200];
  snprintf(name_addr_buf, 200, "%s%s%s", "Address of ", var_name, "'s `name_` member:");
  char name_buf[200];
  snprintf(name_buf, 200, "%s%s%s", "Name of ", var_name, ": ");

  printf("%-*s => %p\n", 50, addr_buf, (void*)var);
  printf("%-*s => %p\n", 50, name_addr_buf, (void*)var->GetName());
  printf("%-*s => %s\n", 50, name_buf, var->GetName());
}

#endif