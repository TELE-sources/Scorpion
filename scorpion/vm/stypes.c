/**
* Copyright (C) 2015 The Scorpion Programming Language
* Braxton Nunnally, see full copyright licence in main.c
* 
* Standard Routines for data type processing in scorpion
* 
*/

#include "stypes.h"
#include <limits.h>

bool num_type(int type)
{ return (type > 0 && type <= 9); }

void create_object(int type, SObject* o, slong size_t, bool array)
{
    if(o->alloc) return;
    o->alloc = true;
    o->size_t = size_t;
    o->array = array;
    o->name = NULL;
    o->elements = NULL;
    o->o = NULL;
    
    o->o = (SType*)malloc(((size_t<=0) ? 1 : size_t)*sizeof(SType));
    if(o->o == NULL) return; // TODO: throw exception
    
    o->o->type = type;
}

void delete_object(SObject* o)
{
    if(!o->alloc) return;
    o->alloc = false;
    
    o->size_t = 0;
    o->array = 0;
    if(o->o->type == node)
    {
        if(o->elements != NULL)
          free( o->elements );
    }
    free( o->o );
}

double sValue(SObject* o, slong ndx)
{
    if(!o->alloc) return LLONG_MAX;
    if(o->array && (ndx <0 || ndx >= o->size_t)) return LLONG_MAX; // TODO: throw exception
    switch( o->o->type )
    {
        case primitive_bit:
        { 
            if(o->array) return o->o[ndx].s_bit;
            else return o->o->s_bit;
        }
        case primitive_boolean:
        {
            if(o->array) return o->o[ndx].s_bool;
            else return o->o->s_bool;
        }
        case primitive_byte:
        {
            if(o->array) return o->o[ndx].s_byte;
            else return o->o->s_byte;
        }
        case primitive_char:
        {
            if(o->array) return o->o[ndx].s_char;
            else return o->o->s_char;
        }
        case primitive_double:
        {
            if(o->array) return o->o[ndx].s_double;
            else return o->o->s_double;
        }
        case primitive_float:
        {
            if(o->array) return o->o[ndx].s_float;
            else return o->o->s_float;
        }
        case primitive_int:
        {
            if(o->array) return o->o[ndx].s_int;
            else return o->o->s_int;
        }
        case primitive_long:
        {
            if(o->array) return o->o[ndx].s_long;
            else return o->o->s_long;
        }
        case primitive_short:
        {
            if(o->array) return o->o[ndx].s_short;
            else return o->o->s_short;
        }
    }
    return LLONG_MAX;
}

void sSet(SObject* o, double v, slong ndx)
{
    if(!o->alloc) return;
    if(o->array && (ndx <0 || ndx >= o->size_t)) return; // TODO: throw exception
    switch( o->o->type )
    {
        case primitive_bit:
        { 
            if(o->array) o->o[ndx].s_bit = v;
            else o->o->s_bit = v;
            return;
        }
        case primitive_boolean:
        {
            if(o->array) o->o[ndx].s_bool = v;
            else o->o->s_bool = v;
            return;
        }
        case primitive_byte:
        {
            if(o->array) o->o[ndx].s_byte = v;
            else o->o->s_byte = v;
            return;
        }
        case primitive_char:
        {
            if(o->array) o->o[ndx].s_char = v;
            else o->o->s_char = v;
            return;
        }
        case primitive_double:
        {
            if(o->array) o->o[ndx].s_double = v;
            else o->o->s_double = v;
            return;
        }
        case primitive_float:
        {
            if(o->array) o->o[ndx].s_float = v;
            else o->o->s_float = v;
            return;
        }
        case primitive_int:
        {
            if(o->array) o->o[ndx].s_int = v;
            else o->o->s_int = v;
            return;
        }
        case primitive_long:
        {
            if(o->array) o->o[ndx].s_long = v;
            else o->o->s_long = v;
            return;
        }
        case primitive_short:
        {
            if(o->array) o->o[ndx].s_short = v;
            else o->o->s_short = v;
            return;
        }
    }
}

bool isnumber(SObject* o)
{
    return (o->alloc && num_type(o->o->type));
}

bool isarray(SObject* o)
{
    return (o->alloc && o->array);
}

