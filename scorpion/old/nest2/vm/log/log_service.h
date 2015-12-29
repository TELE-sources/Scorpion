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
 #ifndef SCORPION_LOG_SREVICE
 #define SCORPION_LOG_SREVICE
 
 
 extern const int VERBOSE;

 extern const int DEBUG;

 extern const int INFO;

 extern const int WARN;

 extern const int ERROR;

 extern const int ASSERT;
 
 string t_ch(int priority);
 
 void println(int priority,string tag,string message, const char* file, 
            string& stackmsg, string package);
 
 void print(int priority, string tag, string message, bool log, const char* file, 
           string& stackmsg, int p, string package);
 
 void a(string tag, string message, string file, bool log, string& stackmsg, int p, string package);

 void e(string tag,string message, string file, bool log, string& stackmsg, int p, string package);

 void w(string tag,string message, string file, bool log, string& stackmsg, int p, string package);

 void i(string tag,string message, string file, bool log, string& stackmsg, int p, string package);

 void d(string tag,string message, string file, bool log, string& stackmsg, int p, string package);
 
 void v(string tag,string message, string file, bool log, string& stackmsg, int p, string package);
 
 #endif // SCORPION_LOG_SREVICE   