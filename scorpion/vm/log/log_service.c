/*
 * Copyright (C) 2015 The Scorpion Programming Language
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Portions of the code surrounded by "// Begin Dalvik code" and
 * "// END Delvik code" are copyrighted and licensed separately, as
 * follows:
 *
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. The ASF licenses this file to You
 * under the Apache License, Version 2.0 (the "License"); you may not use 
 * this file except in compliance with the License.  You may obtain a copy
 * of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
 
 #include "../../clib/filestream.h"
 #include "sys_time.h"
 #include "log_service.h"
 
 string scorpionvm::log::log_service::t_ch(int priority)
 {
   switch( priority ){
      case 2:
        return "V";
      case 3:
        return "D";
      case 4:
        return "I";
      case 5:
        return "W";
      case 6:
        return "E";
      case 7:
        return "A";
      default:
        return "?";
   }
 }
 
 void scorpionvm::log::log_service::println(int priority,string tag,string message, const char* file, 
            string& stackmsg, string package)
 {
     stackmsg += scorpionvm::system_time::currtime() + " " + package + " " + t_ch(priority) + "/" + tag + ": " + message + "\n";

     FileStream::out(file, stackmsg);
 }
 
 void scorpionvm::log::log_service::print(int priority, string tag, string message, bool log, const char* file, 
           string& stackmsg, int p, string package)
 {
     if(!log) return;
     else if(log){
         if(priority >= p)
             println(priority,tag,message, file, stackmsg, package);
     }
 }
 
 void scorpionvm::log::log_service::a(string tag, string message, string file, bool log, string& stackmsg, int p, string package)
 {
    print(ASSERT,tag,message,log,file.c_str(), stackmsg, p, package);
 }

 void scorpionvm::log::log_service::e(string tag,string message, string file, bool log, string& stackmsg, int p, string package)
 {
    print(ERROR,tag,message,log,file.c_str(), stackmsg, p, package);
 }

 void scorpionvm::log::log_service::w(string tag,string message, string file, bool log, string& stackmsg, int p, string package)
 {
    print(WARN,tag,message,log,file.c_str(), stackmsg, p, package);
 }

 void scorpionvm::log::log_service::i(string tag,string message, string file, bool log, string& stackmsg, int p, string package)
 {
    print(INFO,tag,message,log,file.c_str(), stackmsg, p, package);
 }

 void scorpionvm::log::log_service::d(string tag,string message, string file, bool log, string& stackmsg, int p, string package)
 {
    print(DEBUG,tag,message,log,file.c_str(), stackmsg, p, package);
 }
 
 void scorpionvm::log::log_service::v(string tag,string message, string file, bool log, string& stackmsg, int p, string package)
 {
    print(VERBOSE,tag,message,log,file.c_str(), stackmsg, p, package);
 }
  