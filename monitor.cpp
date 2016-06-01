/*
 * Copyright [2012-2015] DaSE@ECNU
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * /CLAIMS/mornitor.cpp
 *
 *  Created on: May 30, 2016
 *      Author: yuyang
 *		   Email: youngfish93@hotmail.com
 *
 * Description: This is a mornitor to send claimsserver status to influxdb
 *
 */

#include <malloc.h>
#include <arpa/inet.h>
#include <glog/logging.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;

int main() {
  std::string pid = "";
  std::ifstream pid_claims;
  pid_claims.open("cliamsserver_pid");
  pid_claims >> pid;
  pid_claims.close();
  while (1) {
    std::string cmd_top = "top -bn 1 -p ";
    std::ostringstream pid_claimsserver;

    cmd_top += pid;
    cmd_top += " > claims.monitor";

    // std::cout << "cmd = " << cmd_top << std::endl;
    // int sys_ret = system(cmd_top.c_str());
    // cout << "sys return " << sys_ret << endl;

    FILE *fp;
    fp = popen(cmd_top.c_str(), "w");
    if (NULL == fp) {
      perror("\"top\"popen failed!");
    } else {
      int rc = pclose(fp);
      if (-1 == rc) {
        perror("close popen failed!");
      }
    }
    std::ifstream mornitor_file;
    std::string cmd_line;
    std::string exec_time = "";
    std::string exec_stat = "";
    float time_per_h = 0;

    float cpu_used = 0.0;
    float mem_used = 0.0;

    mornitor_file.open("claims.monitor");
    for (int i = 0; i < 9; i++) {
      getline(mornitor_file, cmd_line);
      if (i == 7) {
        std::vector<std::string> tokens;
        std::vector<std::string> temp_tokens;
        boost::split(tokens, cmd_line, boost::is_any_of(" "));
        for (int j = 0; j < tokens.size(); j++) {
          if (tokens[j] != "") {
            temp_tokens.push_back(tokens[j]);
            // cout << j << ": " << tokens[j] << endl;
          }
        }
        exec_time = temp_tokens[10];
        cpu_used = atof(temp_tokens[8].c_str());
        mem_used = atof(temp_tokens[9].c_str());
        //        cout << "!!!!!!!!!!!!!" << endl;
        //        for (int j = 0; j < tokens.size(); j++) {
        //          cout << j << ": " << tokens[j] << endl;
        //        }
      }
    }
    std::vector<std::string> time_tokens;
    // cout << "exec_time = " << exec_time << endl;
    boost::split(time_tokens, exec_time, boost::is_any_of(":."));
    time_per_h = atof(time_tokens[0].c_str()) +
                 atof(time_tokens[1].c_str()) / 60 +
                 atof(time_tokens[2].c_str()) / 3600;
    // cout << "After cal : " << time_per_h << endl;

    // cout << "exec time = " << exec_time << endl;
    // cout << "exec stat = " << exec_stat << endl;
    // cout << "cpu_used = " << cpu_used << "%" << endl;
    // cout << "mem_used = " << mem_used << "%" << endl;
    mornitor_file.close();

    std::string post_to_influxdb = "curl -X POST -d ";
    std::ostringstream series_info;
    series_info
        << "\'[{\"name\":\"claimsserver\","
        << "\"columns\":[\"exec_stat\",\"cpu_used\",\"mem_used\",\"pid\"],"
        << "\"points\":[[" << time_per_h << "," << cpu_used << "," << mem_used
        << "," << pid << "]]}]\' "
        //    << "\'http://localhost:8084/db/collectd/series?u=root&p=root\'";
        << "\'http://58.198.176.92:8084/db/collectd/series?u=root&p=root\'";
    post_to_influxdb += series_info.str();
    // cout << "POST request is: " << post_to_influxdb << endl;
    // int sys_ret = system(post_to_influxdb.c_str());
    // cout << "sys return " << sys_ret << endl;
    fp = popen(post_to_influxdb.c_str(), "w");
    if (NULL == fp) {
      perror("\"top\"popen failed!");
    } else {
      int rc = pclose(fp);
      if (-1 == rc) {
        perror("close popen failed!");
      }
    }

    // cout << "sleeping 3 seconds" << endl;
    sleep(5);
  }
  return 0;
}
