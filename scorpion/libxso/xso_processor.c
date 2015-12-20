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
 
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include <signal.h>
 #include <assert.h>
 #include <iostream>
 #include <sstream>
 #include <string>
 #include <stdint.h>
 #include "xso_processor.h"
 #include "../vm/internal/globals.h"
 #include "../vm/internal/Opcodes.h"
 
 using namespace std;
 using namespace scorpionvm::vm;
 using namespace scorpionvm::_xso;
 using namespace scorpionvm::xsoprocessor;
 using namespace scorpionvm::log::log_service::debug;
 
 extern Log ldebug;
     // *********************[[ Compiler flag id's ]]**********************
     #define method (0x7f)
     #define instr (0x5a)
     #define string_instr (0x3b)
     #define out_instr (0x2c)
 
     char xso_processor::read_byte()
     {
         if(!(i< image.size()))
         {
           _eof = true;
           return -1;
         }
         return (byte = image.at(i++));
     }
     
     string xso_processor::xso_text()
     {
        char c;
        stringstream ss;
        for(long i2 = i; i2 < image.size(); i2++){
             c = read_byte();
             if(_eof){
                ss.str("");
                return "";
             }
                
             if(c == 0)
               break;
             else
               ss << c;
         }
         
         return ss.str();
     }
     
     void process_err()
     {
         cout << "Sgnmentation fault\n";
         g_Svm._sig_handler.raise_sig(SIGQUIT);
     }
     
     void xso_processor::setbyte(double byte)
     {
         if(g_Svm.vmstates->image_stream.size() > img_t)
         {
             ldebug.LOGV("Image size bigger than specified.");
             process_err();
         }
         
         g_Svm.vmstates->image_stream.add(byte);
     }
     
     void create_method(std::string method_info, long addr)
     {
         if(addr > g_Svm.env->sizeinfo(0, 3))
         {
           stringstream ss;
           ss << "There are more methods than expected. Note: attemp to add method at address # " << addr
               << " max method limit: " << g_Svm.env->sizeinfo(0, 3) << ".";
           ldebug.LOGV(ss.str());
           process_err();
         }
         
         stringstream m_name, class_name, 
                        package, file_name;
         int index = 0;
         bool native = false;
         if(method_info.at(0) == '~'){
           index = 1;
           native = true;
         }
    
         for(int i2 = index; i2 < method_info.size(); i2++){
            if(method_info.at(i2) == '&'){
                index++;
                break;
            }
            else {
              m_name << "" << method_info.at(i2);
            }
            index++;
         }
         
         for(int i2 = index; i2 < method_info.size(); i2++){
            if(method_info.at(i2) == '&'){
                index++;
                break;
            }
            else
              class_name << "" << method_info.at(i2);
            index++;
         }
        
         for(int i2 = index; i2 < method_info.size(); i2++){
            if(method_info.at(i2) == '&'){
                index++;
                break;
            }
            else
              package << "" << method_info.at(i2);
            index++;
         }
        
         for(int i2 = index; i2 < method_info.size(); i2++)
              file_name << "" << method_info.at(i2);
         
         g_Svm.native_methods[addr].name = m_name.str();
         g_Svm.native_methods[addr].clazz = class_name.str();
         g_Svm.native_methods[addr].package = package.str();
         g_Svm.native_methods[addr].native = native;
         g_Svm.native_methods[addr].file = file_name.str();
         g_Svm.native_methods[addr].jmp = 
            g_Svm.vmstates->image_stream.size();
         cout << "addr " << addr << " n " << g_Svm.native_methods[addr].name << " mm" << endl;
         
     }
     
     void xso_processor::process_instruction()
     {
         byte=atof(xso_text().c_str());
         if(!(byte > 0) && !(byte <= sMaxOpcodeLimit))
             process_err();
         
         if(byte == OP_NOP || byte == OP_END || byte == OP_HLT || byte == OP_NO || byte == OP_ENDNO){  
             setbyte(byte);
             read_byte();
             return;
         }
         else if(byte == OP_RETURN  || byte == OP_PUSH || byte == OP_POP || byte == OP_JMP 
            || byte == OP_MTHD || byte == OP_LBL || byte == OP_IF || byte == OP_INC || byte == OP_DEC 
            || byte == OP_KILL || byte == OP_DELETE || byte == OP_NEG || byte == OP_CIN){
            double arg1 = atof(xso_text().c_str());
            setbyte(byte);
            setbyte(arg1);
            
            if(byte == OP_LBL) {// preprocess label locations
            //    u1 sz;
            //    sz.byte1 = 1;
            //    SVM_OBJECT_INIT(gSvm.env->getBitmap().objs[(long) arg1], TYPEDEF_GENERIC_LONG, sz);
            //    svmSetGenericValue(gSvm.env->getBitmap().objs[(long) arg1], streamcount);
            }
            
            read_byte();
            return;
         }
         else if(byte == OP_SCONST || byte == OP_BCONST || byte == OP_CCONST || byte == OP_RSHFT 
           || byte == OP_LSHFT || byte == OP_JIF || byte == OP_CALL
           || byte == OP_JIT || byte == OP_ICONST || byte == OP_DCONST || byte == OP_FCONST 
           || byte == OP_THROW || byte == OP_STR_APND || byte == OP_ASSN
           || byte == OP_STR_ACONST || byte == OP_BYTE_CONST || byte == OP_LCONST
           || byte == OP_DACONST || byte == OP_IACONST || byte == OP_FACONST || byte == OP_CACONST 
           || byte == OP_BACONST || byte == OP_BYTE_ACONST || byte == OP_SACONST || byte == OP_LACONST){
            double arg1 = atof(xso_text().c_str());
            double arg2 = atof(xso_text().c_str());
            
            if(byte == OP_SCONST || byte == OP_BCONST || byte == OP_CCONST
                || byte == OP_ICONST || byte == OP_DCONST || byte == OP_FCONST
                || byte == OP_BYTE_CONST || byte == OP_LCONST)
              setbyte(OP_CONST);
            else if(byte == OP_DACONST || byte == OP_IACONST || byte == OP_FACONST 
                || byte == OP_CACONST || byte == OP_BACONST || byte == OP_BYTE_ACONST 
                || byte == OP_SACONST || byte == OP_LACONST)  
              setbyte(OP_ACONST);
              
            setbyte(byte);
            setbyte(arg1);
            setbyte(arg2);
            
            read_byte();
            return;
         }
         else if(byte == OP_ADD || byte == OP_ISEQ || byte == OP_ISLT || byte == OP_ISLE
           || byte == OP_ISGT || byte == OP_ISGE || byte == OP_SUB || byte == OP_MULT 
           || byte == OP_DIV || byte == OP_MOD || byte == OP_OR || byte == OP_AND
           || byte == OP_AT || byte == OP_ALOAD || byte == OP_ASTORE || byte == OP_CAST){
            double arg1 = atof(xso_text().c_str());
            double arg2 = atof(xso_text().c_str());
            double arg3 = atof(xso_text().c_str());
            
            if(byte == OP_ISEQ || byte == OP_ISLT || byte == OP_ISLE
               || byte == OP_ISGT || byte == OP_ISGE || byte == OP_OR 
               || byte == OP_AND)
              setbyte(OP_CMP);
            
            setbyte(byte);
            setbyte(arg1);
            setbyte(arg2);
            setbyte(arg3);
            
            read_byte();
            return;
         }
         else
           process_err();
     }
     
     void xso_processor::process_str_instruction()
     {
         byte=atof(xso_text().c_str());
         if(byte == OP_COUT){
             string msg = xso_text();
             long arg1 = msg.size();
             
             setbyte(byte);
             setbyte(arg1);
             for(int i = 0; i < arg1; i++)
                 setbyte((int) msg.at(i));
                 
             read_byte();
         }
         else if(byte == OP_STRCONST){
             long arg1 = atoi(xso_text().c_str());
             string msg = xso_text();
             long arg2 = msg.size();
             
             setbyte(byte);
             setbyte(arg1);
             setbyte(arg2);
             for(int i = 0; i < arg2; i++)
                 setbyte((int) msg.at(i));
                 
             read_byte();
         }
         else
            process_err();
     }
     
     void xso_processor::process_method()
     {
        byte = atol(xso_text().c_str());
        if(byte != OP_MTHD)
          process_err();
          
        string method_info = xso_text();
        long m_adr = atol(xso_text().c_str());
        
       g_Svm.vmstates->image_stream.add(OP_MTHD);
       g_Svm.vmstates->image_stream.add(m_adr);
       
       create_method(method_info, m_adr);
       
       read_byte();
     }
     
     int xso_processor::preprocess()
     {
         if(img_t >= xso_max_buf_len)
         {
             stringstream ss;
             ss << "Image size too large. Note: size = " << img_t 
                 << " >= max size (" << xso_max_buf_len << ").";
             ldebug.LOGV(ss.str());
             return 1;
         }
         
         read_byte();
         for( ;; )
         {
            // cout << "byte " << (int)byte << endl;
             switch( byte )
             {
                 case 0:
                    byte = read_byte();
                 break;
                 case method:
                    process_method();
                 break;
                 case instr:
                    process_instruction();
                 break;
                 case string_instr:
                    process_str_instruction();
                 break;
                 case -1:
                  if(_eof){
                    _eof = false;
                  //  gSvm.image = "";
                   // cout << "img sz " << streamcount << " file sz " << gSvm.appheader.filesize.byte1 << endl;
                  /*  if(streamcount != gSvm.appheader.filesize.byte1){
                        alog.setClass("XSO");
                        alog.ALOGV("Image size does not match specified length. Try recompiling your application.");
                        process_err();
                    }*/
                    return 0;
                  }
                  else
                  {
                    stringstream ss;
                    ss << "Invalid flag found in image file (" << (int)byte << ")";
                    ldebug.LOGV(ss.str());
                    return 1;
                  }
                 break;
                 default:
                   return 1;
             }
         }
         
         /* Shouldnt reach this point */
         return 1;
     }
       
       