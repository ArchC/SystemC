/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_isdb_trace.cpp - Implementation of ISDB tracing.

  Original Author: Ulli Holtmann, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Ali Dasdan, Synopsys, Inc.
  Description of Modification: - Replaced 'width' of sc_(u)int with their
                                 'bitwidth()'.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include <assert.h>
#include <ctime>
#include <stdlib.h>

#include "systemc/kernel/sc_simcontext.h"
#include "systemc/kernel/sc_ver.h"
#include "systemc/datatypes/bit/sc_bit.h"
#include "systemc/datatypes/bit/sc_logic.h"
#include "systemc/datatypes/int/sc_signed.h"
#include "systemc/datatypes/int/sc_unsigned.h"
#include "systemc/datatypes/int/sc_int_base.h"
#include "systemc/datatypes/int/sc_uint_base.h"
#include "systemc/datatypes/fx/fx.h"
#include "systemc/tracing/sc_isdb_trace.h"
#include "systemc/utils/sc_iostream.h"
#include "systemc/utils/sc_string.h"


static bool running_regression = false;

// Forward declarations for functions that come later in the file
// Map sc_logic to printable ISDB
static unsigned int map_sc_logic_state_to_isdb_binval (char);

// Print ISDB error message
static void isdb_put_error_message(const char* msg, bool just_warning);


/*****************************************************************************/

// Base class for the traces
class isdb_trace {
public:
    isdb_trace(const sc_string& name_, const sc_string& isdb_name_);

    // Needs to be pure virtual as has to be defined by the particular
    // type being traced
    virtual void write(ISDB_Conn database) = 0;
    
    virtual void set_width();

    // Comparison function needs to be pure virtual too
    virtual bool changed() = 0;

    // Make this virtual as some derived classes may overwrite
    virtual void declare_variable(ISDB_Conn database);

    void compose_data_line(char* rawdata, char* compdata);

    virtual ~isdb_trace();

    const sc_string name;
    const sc_string isdb_name;
    const char* isdb_var_typ_name;
    int bit_width; 
    ISDB_Signal isdb_id;
};


isdb_trace::isdb_trace(const sc_string& name_, const sc_string& isdb_name_)
       : name(name_), isdb_name(isdb_name_), bit_width(0)
{
    /* Intentionally blank */
}
        
void isdb_trace::compose_data_line(char* rawdata, char* compdata)
{
    assert(rawdata != compdata);

    if(bit_width == 0) {
      compdata[0] = '\0'; 
    } else {
        if(bit_width == 1){
            compdata[0] = rawdata[0];
            strcpy(&(compdata[1]), isdb_name);
        }
        else{
            char* effective_begin = rawdata;
            sprintf(compdata, "b%s %s", effective_begin, (const char *) isdb_name);
        }
    }
}

void isdb_trace::declare_variable (ISDB_Conn /* database */)
{
  /* Intentionally Blank, should be defined for each type separately */
}

void isdb_trace::set_width()
{
  /* Intentionally Blank, should be defined for each type separately */
}

isdb_trace::~isdb_trace()
{
  /* Intentionally Blank */
}


/*****************************************************************************/


class isdb_bool_trace : public isdb_trace {
public:
    isdb_bool_trace(const bool& object, const sc_string& name_, const sc_string& isdb_name_);
    void write(ISDB_Conn database);
    bool changed();
    void declare_variable(ISDB_Conn database);

protected:    
    const bool& object;
    bool old_value;
};

isdb_bool_trace::isdb_bool_trace(const bool& object_, const sc_string& name_, const sc_string& isdb_name_)
  : isdb_trace(name_, isdb_name_), object( object_)
{
    isdb_var_typ_name = "wire";
    bit_width = 1;
    old_value = object;
}

void isdb_bool_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Binary, (const char*)name);
  write(database);
}

bool isdb_bool_trace::changed()
{
  return object != old_value;
}

void isdb_bool_trace::write(ISDB_Conn database)
{
  int val = object==true ? ISDB_HIGH : ISDB_LOW;
  ISDB_SetValue (database, isdb_id, val);

  old_value = object;
}

/*****************************************************************************/

class isdb_sc_bit_trace
: public isdb_trace
{
public:
    isdb_sc_bit_trace(const sc_bit& , const sc_string& , const sc_string& );
    void write(ISDB_Conn database);
    bool changed();
    void declare_variable(ISDB_Conn database);

protected:    
    const sc_bit& object;
    sc_bit old_value;
};

isdb_sc_bit_trace::isdb_sc_bit_trace( const sc_bit& object_,
				      const sc_string& name_,
				      const sc_string& isdb_name )
: isdb_trace( name_, isdb_name ), object( object_ )
{
    isdb_var_typ_name = "wire";
    bit_width = 1;
    old_value = object;
}

void isdb_sc_bit_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Binary, (const char*)name);
  write(database);
}

bool isdb_sc_bit_trace::changed()
{
  return object != old_value;
}

void isdb_sc_bit_trace::write(ISDB_Conn database)
{
  int val = object==true ? ISDB_HIGH : ISDB_LOW;
  ISDB_SetValue (database, isdb_id, val);

  old_value = object;
}

/*****************************************************************************/

class isdb_sc_logic_trace : public isdb_trace {
public:
    isdb_sc_logic_trace(const sc_logic& object, const sc_string& name_, const sc_string& isdb_name_);
    void write(ISDB_Conn database);
    bool changed();
    void declare_variable(ISDB_Conn database);

protected:    
    const sc_logic& object;
    sc_logic old_value;
};


isdb_sc_logic_trace::isdb_sc_logic_trace(const sc_logic& object_, const sc_string& name_, const sc_string& isdb_name_) 
  : isdb_trace( name_, isdb_name_), object( object_)
{
    isdb_var_typ_name = "wire";
    bit_width = 1;
    old_value = object;
}

void isdb_sc_logic_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Binary, name);
  write(database);
}

bool isdb_sc_logic_trace::changed()
{
    return object != old_value;
}


void isdb_sc_logic_trace::write(ISDB_Conn database)
{
  int val = map_sc_logic_state_to_isdb_binval(object.to_char());
  ISDB_SetValue (database, isdb_id, val);

  old_value = object;
}


/*****************************************************************************/

class isdb_sc_unsigned_trace : public isdb_trace {
public:
    isdb_sc_unsigned_trace(const sc_unsigned& object, const sc_string& name_, const sc_string& isdb_name_);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();
    void set_width();

protected:
    const sc_unsigned& object;
    sc_unsigned old_value;
};


isdb_sc_unsigned_trace::isdb_sc_unsigned_trace(const sc_unsigned& object_, const sc_string& name_, const sc_string& isdb_name_) 
  : isdb_trace( name_, isdb_name_), object( object_), old_value( object_.length()) // The last may look strange, but is correct
{
    isdb_var_typ_name = "wire";
    old_value = object;
}

void isdb_sc_unsigned_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Integer, name);
  write(database);
}

bool isdb_sc_unsigned_trace::changed()
{
    return object != old_value;
}

void isdb_sc_unsigned_trace::write(ISDB_Conn database)
{
  ISDB_SetValue (database, isdb_id, object.to_uint());

  old_value = object;
}

void isdb_sc_unsigned_trace::set_width()
{
    bit_width = object.length();
}


/*****************************************************************************/

class isdb_sc_signed_trace : public isdb_trace {
public:
    isdb_sc_signed_trace(const sc_signed& object, const sc_string& name_, const sc_string& isdb_name_);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();
    void set_width();

protected:    
    const sc_signed& object;
    sc_signed old_value;
};


isdb_sc_signed_trace::isdb_sc_signed_trace(const sc_signed& object_, const sc_string& name_, const sc_string& isdb_name_) 
  : isdb_trace( name_, isdb_name_), object( object_), old_value( object_.length())
{
    isdb_var_typ_name = "wire";
    old_value = object;
}

void isdb_sc_signed_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Integer, name);
  write(database);
}

bool isdb_sc_signed_trace::changed()
{
    return object != old_value;
}

void isdb_sc_signed_trace::write(ISDB_Conn database)
{
  ISDB_SetValue (database, isdb_id, object.to_int());

  old_value = object;
}

void isdb_sc_signed_trace::set_width()
{
    bit_width = object.length();
}


/*****************************************************************************/

class isdb_sc_uint_base_trace : public isdb_trace {
public:
    isdb_sc_uint_base_trace(const sc_uint_base& object, const sc_string& name_, const sc_string& isdb_name_);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();
    void set_width();

protected:    
    const sc_uint_base& object;
    sc_uint_base old_value;
};


isdb_sc_uint_base_trace::isdb_sc_uint_base_trace(const sc_uint_base& object_, const sc_string& name_, const sc_string& isdb_name_) 
  : isdb_trace( name_, isdb_name_), object( object_), old_value( object_.length()) // The last may look strange, but is correct
{
    isdb_var_typ_name = "wire";
    old_value = object;
}

void isdb_sc_uint_base_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Integer, name);
  write(database);
}

bool isdb_sc_uint_base_trace::changed()
{
    return object != old_value;
}

void isdb_sc_uint_base_trace::write(ISDB_Conn database)
{
  ISDB_SetValue (database, isdb_id, uint64(object));

  old_value = object;
}

void isdb_sc_uint_base_trace::set_width()
{
    bit_width = object.length();
}


/*****************************************************************************/

class isdb_sc_int_base_trace : public isdb_trace {
public:
    isdb_sc_int_base_trace(const sc_int_base& object, const sc_string& name_, const sc_string& isdb_name_);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();
    void set_width();

protected:    
    const sc_int_base& object;
    sc_int_base old_value;
};


isdb_sc_int_base_trace::isdb_sc_int_base_trace(const sc_int_base& object_, const sc_string& name_, const sc_string& isdb_name_) 
  : isdb_trace( name_, isdb_name_), object( object_), old_value( object_.length())
{
    isdb_var_typ_name = "wire";
    old_value = object;
}

void isdb_sc_int_base_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Integer, name);
  write(database);
}

bool isdb_sc_int_base_trace::changed()
{
    return object != old_value;
}

void isdb_sc_int_base_trace::write(ISDB_Conn database)
{
  ISDB_SetValue (database, isdb_id, int64(object));

  old_value = object;
}

void isdb_sc_int_base_trace::set_width()
{
    bit_width = object.length();
}


/*****************************************************************************/

class isdb_sc_fxval_trace : public isdb_trace
{
public:

    isdb_sc_fxval_trace( const sc_fxval& object,
			 const sc_string& name_,
			 const sc_string& isdb_name_ );
    void declare_variable( ISDB_Conn database );
    void write( ISDB_Conn database );
    bool changed();

protected:    

    const sc_fxval& object;
    sc_fxval old_value;

};

isdb_sc_fxval_trace::isdb_sc_fxval_trace( const sc_fxval& object_,
					  const sc_string& name_,
					  const sc_string& isdb_name_ )
: isdb_trace( name_, isdb_name_ ),
  object( object_ )
{
    isdb_var_typ_name = "real";
    bit_width = 1;
    old_value = object;
}

void
isdb_sc_fxval_trace::declare_variable ( ISDB_Conn database )
{
    isdb_id = ISDB_DefineSignal( database, ISDB_Float, name );
    write( database );
}

bool
isdb_sc_fxval_trace::changed()
{
    return object != old_value;
}

void
isdb_sc_fxval_trace::write( ISDB_Conn database )
{
    ISDB_SetValue( database, isdb_id, object.to_double() );

    old_value = object;
}

/*****************************************************************************/

class isdb_sc_fxval_fast_trace : public isdb_trace
{
public:

    isdb_sc_fxval_fast_trace( const sc_fxval_fast& object,
			      const sc_string& name_,
			      const sc_string& isdb_name_ );
    void declare_variable( ISDB_Conn database );
    void write( ISDB_Conn database );
    bool changed();

protected:    

    const sc_fxval_fast& object;
    sc_fxval_fast old_value;

};

isdb_sc_fxval_fast_trace::isdb_sc_fxval_fast_trace( const sc_fxval_fast& object_,
						    const sc_string& name_,
						    const sc_string& isdb_name_ )
: isdb_trace( name_, isdb_name_ ),
  object( object_ )
{
    isdb_var_typ_name = "real";
    bit_width = 1;
    old_value = object;
}

void
isdb_sc_fxval_fast_trace::declare_variable ( ISDB_Conn database )
{
    isdb_id = ISDB_DefineSignal( database, ISDB_Float, name );
    write( database );
}

bool
isdb_sc_fxval_fast_trace::changed()
{
    return object != old_value;
}

void
isdb_sc_fxval_fast_trace::write( ISDB_Conn database )
{
    ISDB_SetValue( database, isdb_id, object.to_double() );

    old_value = object;
}

/*****************************************************************************/

class isdb_sc_fxnum_trace : public isdb_trace
{
public:

    isdb_sc_fxnum_trace( const sc_fxnum& object,
			 const sc_string& name_,
			 const sc_string& isdb_name_ );
    void declare_variable( ISDB_Conn database );
    void write( ISDB_Conn database );
    bool changed();
    void set_width();

protected:

    const sc_fxnum& object;
    sc_fxnum old_value;

};

isdb_sc_fxnum_trace::isdb_sc_fxnum_trace( const sc_fxnum& object_,
					  const sc_string& name_,
					  const sc_string& isdb_name_ )
: isdb_trace( name_, isdb_name_ ),
  object( object_ ),
  old_value( object_.m_params.type_params(),
	     object_.m_params.enc(),
	     object_.m_params.cast_switch(),
	     0 )
{
    isdb_var_typ_name = "wire";
    old_value = object;
}

void
isdb_sc_fxnum_trace::declare_variable ( ISDB_Conn database )
{
    isdb_id = ISDB_DefineSignal( database, ISDB_Array, name,
				 ISDB_Binary, 0, bit_width - 1 );
    write( database );
}

bool
isdb_sc_fxnum_trace::changed()
{
    return object != old_value;
}

void
isdb_sc_fxnum_trace::write( ISDB_Conn database )
{
    // TBD: make val a member function ==> less malloc calls

    unsigned int* val = (unsigned int*) malloc( sizeof(unsigned int) *
						bit_width );
    int bitindex;
    unsigned int isdb_val;

    for( bitindex = object.wl() - 1; bitindex >= 0; -- bitindex )
    {
	isdb_val = object[bitindex] ? ISDB_HIGH : ISDB_LOW;
	val[bitindex] = isdb_val;
    }
    ISDB_SetValue( database, isdb_id, sizeof(val[0]), val );
    free( val );

    old_value = object;
}

void
isdb_sc_fxnum_trace::set_width()
{
    bit_width = object.wl();
}

/*****************************************************************************/

class isdb_sc_fxnum_fast_trace : public isdb_trace
{
public:

    isdb_sc_fxnum_fast_trace( const sc_fxnum_fast& object,
			      const sc_string& name_,
			      const sc_string& isdb_name_ );
    void declare_variable( ISDB_Conn database );
    void write( ISDB_Conn database );
    bool changed();
    void set_width();

protected:

    const sc_fxnum_fast& object;
    sc_fxnum_fast old_value;

};

isdb_sc_fxnum_fast_trace::isdb_sc_fxnum_fast_trace( const sc_fxnum_fast& object_,
						    const sc_string& name_,
						    const sc_string& isdb_name_ )
: isdb_trace( name_, isdb_name_ ),
  object( object_ ),
  old_value( object_.m_params.type_params(),
	     object_.m_params.enc(),
	     object_.m_params.cast_switch(),
	     0 )
{
    isdb_var_typ_name = "wire";
    old_value = object;
}

void
isdb_sc_fxnum_fast_trace::declare_variable ( ISDB_Conn database )
{
    isdb_id = ISDB_DefineSignal( database, ISDB_Array, name,
				 ISDB_Binary, 0, bit_width - 1 );
    write( database );
}

bool
isdb_sc_fxnum_fast_trace::changed()
{
    return object != old_value;
}

void
isdb_sc_fxnum_fast_trace::write( ISDB_Conn database )
{
    // TBD: make val a member function ==> less malloc calls

    unsigned int* val = (unsigned int*) malloc( sizeof(unsigned int) *
						bit_width );
    int bitindex;
    unsigned int isdb_val;

    for( bitindex = object.wl() - 1; bitindex >= 0; -- bitindex )
    {
	isdb_val = object[bitindex] ? ISDB_HIGH : ISDB_LOW;
	val[bitindex] = isdb_val;
    }
    ISDB_SetValue( database, isdb_id, sizeof(val[0]), val );
    free( val );

    old_value = object;
}

void
isdb_sc_fxnum_fast_trace::set_width()
{
    bit_width = object.wl();
}


/*****************************************************************************/

class isdb_unsigned_int_trace : public isdb_trace {
public:
    isdb_unsigned_int_trace(const unsigned& object, const sc_string& name_, const sc_string& isdb_name_, int width_);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();

protected:
    const unsigned& object;
    unsigned old_value;
    unsigned mask; 
};


isdb_unsigned_int_trace::isdb_unsigned_int_trace(const unsigned& object_, const sc_string& name_, const sc_string& isdb_name_, int width_) 
  : isdb_trace( name_, isdb_name_), object( object_)
{
    bit_width = width_;
    if (bit_width < 32) {
        mask = ~(-1 << bit_width);
    } else {
        mask = static_cast<unsigned int>( -1 ); // 0xffffffff
    }

    isdb_var_typ_name = "wire";
    old_value = object;
}

void isdb_unsigned_int_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Integer, name);
  write(database);
}

bool isdb_unsigned_int_trace::changed()
{
    return object != old_value;
}

void isdb_unsigned_int_trace::write(ISDB_Conn database)
{
  ISDB_SetValue (database, isdb_id, object);

  old_value = object;
}

/*****************************************************************************/

class isdb_unsigned_short_trace : public isdb_trace {
public:
    isdb_unsigned_short_trace(const unsigned short& object, const sc_string& name_, const sc_string& isdb_name_, int width_);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();

protected:
    const unsigned short& object;
    unsigned short old_value;
    unsigned short mask; 
};


isdb_unsigned_short_trace::isdb_unsigned_short_trace(const unsigned short& object_, const sc_string& name_, const sc_string& isdb_name_, int width_) 
  : isdb_trace( name_, isdb_name_), object( object_)
{
    bit_width = width_;
    if (bit_width < 16) {
        mask = static_cast<unsigned short>( ~(-1 << bit_width) );
    } else {
        mask = static_cast<unsigned short>( -1 ); // 0xffff
    }

    isdb_var_typ_name = "wire";
    old_value = object;
}

void isdb_unsigned_short_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Integer, name);
  write(database);
}

bool isdb_unsigned_short_trace::changed()
{
    return object != old_value;
}

void isdb_unsigned_short_trace::write(ISDB_Conn database)
{
  ISDB_SetValue (database, isdb_id, object);

  old_value = object;
}

/*****************************************************************************/

class isdb_unsigned_char_trace : public isdb_trace {
public:
    isdb_unsigned_char_trace(const unsigned char& object, const sc_string& name_, const sc_string& isdb_name_, int width_);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();

protected:
    const unsigned char& object;
    unsigned char old_value;
    unsigned char mask; 
};


isdb_unsigned_char_trace::isdb_unsigned_char_trace(const unsigned char& object_, const sc_string& name_, const sc_string& isdb_name_, int width_) 
  : isdb_trace( name_, isdb_name_), object( object_)
{
    bit_width = width_;
    if (bit_width < 8) {
        mask = static_cast<unsigned char>( ~(-1 << bit_width) );
    } else {
        mask = static_cast<unsigned char>( -1 ); // 0xff
    }

    isdb_var_typ_name = "wire";
    old_value = object;
}

void isdb_unsigned_char_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Integer, name);
  write(database);
}

bool isdb_unsigned_char_trace::changed()
{
    return object != old_value;
}

void isdb_unsigned_char_trace::write(ISDB_Conn database)
{
  ISDB_SetValue (database, isdb_id, object);

  old_value = object;
}

/*****************************************************************************/

class isdb_unsigned_long_trace : public isdb_trace {
public:
    isdb_unsigned_long_trace(const unsigned long& object, const sc_string& name_, const sc_string& isdb_name_, int width_);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();

protected:
    const unsigned long& object;
    unsigned long old_value;
    unsigned long mask; 
};


isdb_unsigned_long_trace::isdb_unsigned_long_trace(const unsigned long& object_, const sc_string& name_, const sc_string& isdb_name_, int width_) 
  : isdb_trace( name_, isdb_name_), object( object_)
{
    bit_width = width_;
    if (bit_width < 32) {
        mask = ~(-1 << bit_width);
    } else {
        mask = static_cast<unsigned long>( -1 ); // 0xffffffff
    }

    isdb_var_typ_name = "wire";
    old_value = object;
}

void isdb_unsigned_long_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Integer, name);
  write(database);
}

bool isdb_unsigned_long_trace::changed()
{
    return object != old_value;
}

void isdb_unsigned_long_trace::write(ISDB_Conn database)
{
  ISDB_SetValue (database, isdb_id, object);

  old_value = object;
}

/*****************************************************************************/

class isdb_signed_int_trace : public isdb_trace {
public:
    isdb_signed_int_trace(const int& object, const sc_string& name_, const sc_string& isdb_name_, int width_);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();

protected:
    const int& object;
    int old_value;
    unsigned mask; 
};


isdb_signed_int_trace::isdb_signed_int_trace(const signed& object_, const sc_string& name_, const sc_string& isdb_name_, int width_) 
  : isdb_trace( name_, isdb_name_), object( object_)
{
    bit_width = width_;
    if (bit_width < 32) {
        mask = ~(-1 << bit_width);
    } else {
        mask = static_cast<unsigned int>( -1 ); // 0xffffffff
    }

    isdb_var_typ_name = "wire";
    old_value = object;
}

void isdb_signed_int_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Integer, name);
  write(database);
}

bool isdb_signed_int_trace::changed()
{
    return object != old_value;
}

void isdb_signed_int_trace::write(ISDB_Conn database)
{
  ISDB_SetValue (database, isdb_id, object);

  old_value = object;
}

/*****************************************************************************/

class isdb_signed_short_trace : public isdb_trace {
public:
    isdb_signed_short_trace(const short& object, const sc_string& name_, const sc_string& isdb_name_, int width_);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();

protected:
    const short& object;
    short old_value;
    unsigned short mask; 
};


isdb_signed_short_trace::isdb_signed_short_trace(const short& object_, const sc_string& name_, const sc_string& isdb_name_, int width_) 
  : isdb_trace( name_, isdb_name_), object( object_)
{
    bit_width = width_;
    if (bit_width < 16) {
        mask = static_cast<unsigned short>( ~(-1 << bit_width) );
    } else {
        mask = static_cast<unsigned short>( -1 ); // 0xffff
    }

    isdb_var_typ_name = "wire";
    old_value = object;
}

void isdb_signed_short_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Integer, name);
  write(database);
}


bool isdb_signed_short_trace::changed()
{
    return object != old_value;
}


void isdb_signed_short_trace::write(ISDB_Conn database)
{
  ISDB_SetValue (database, isdb_id, object);

  old_value = object;
}

/*****************************************************************************/

class isdb_signed_char_trace : public isdb_trace {
public:
    isdb_signed_char_trace(const char& object, const sc_string& name_, const sc_string& isdb_name_, int width_);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();

protected:
    const char& object;
    char old_value;
    unsigned char mask; 
};


isdb_signed_char_trace::isdb_signed_char_trace(const char& object_, const sc_string& name_, const sc_string& isdb_name_, int width_) 
  : isdb_trace( name_, isdb_name_), object( object_)
{
    bit_width = width_;
    if (bit_width < 8) {
        mask = static_cast<unsigned char>( ~(-1 << bit_width) );
    } else {
        mask = static_cast<unsigned char>( -1 ); // 0xff
    }

    isdb_var_typ_name = "wire";
    old_value = object;
}

void isdb_signed_char_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Integer, name);
  write(database);
}


bool isdb_signed_char_trace::changed()
{
    return object != old_value;
}


void isdb_signed_char_trace::write(ISDB_Conn database)
{
  ISDB_SetValue (database, isdb_id, object);

  old_value = object;
}

/*****************************************************************************/

class isdb_signed_long_trace : public isdb_trace {
public:
    isdb_signed_long_trace(const long& object, const sc_string& name_, const sc_string& isdb_name_, int width_);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();

protected:
    const long& object;
    long old_value;
    unsigned long mask; 
};


isdb_signed_long_trace::isdb_signed_long_trace(const long& object_, const sc_string& name_, const sc_string& isdb_name_, int width_) 
  : isdb_trace( name_, isdb_name_), object( object_)
{
    bit_width = width_;
    if (bit_width < 32) {
        mask = ~(-1 << bit_width);
    } else {
        mask = static_cast<unsigned long>( -1 ); // 0xffffffff
    }

    isdb_var_typ_name = "wire";
    old_value = object;
}

void isdb_signed_long_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Integer, name);
  write(database);
}


bool isdb_signed_long_trace::changed()
{
    return object != old_value;
}


void isdb_signed_long_trace::write(ISDB_Conn database)
{
  ISDB_SetValue (database, isdb_id, object);

  old_value = object;
}


/*****************************************************************************/

class isdb_float_trace : public isdb_trace {
public:
    isdb_float_trace(const float& object, const sc_string& name_, const sc_string& isdb_name_);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();

protected:    
    const float& object;
    float old_value;
};

isdb_float_trace::isdb_float_trace(const float& object_, const sc_string& name_, const sc_string& isdb_name_)
  : isdb_trace( name_, isdb_name_), object( object_)
{
    isdb_var_typ_name = "real";
    bit_width = 1;
    old_value = object;
}

void isdb_float_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Float, name);
  write(database);
}

bool isdb_float_trace::changed()
{
    return object != old_value;
}

void isdb_float_trace::write(ISDB_Conn database)
{
  ISDB_SetValue (database, isdb_id, object);

  old_value = object;
}

/*****************************************************************************/

class isdb_double_trace : public isdb_trace {
public:
    isdb_double_trace(const double& object, const sc_string& name_, const sc_string& isdb_name_);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();

protected:    
    const double& object;
    double old_value;
};

isdb_double_trace::isdb_double_trace(const double& object_, const sc_string& name_, const sc_string& isdb_name_)
  : isdb_trace( name_, isdb_name_), object( object_)
{
    isdb_var_typ_name = "real";
    bit_width = 1;
    old_value = object;
}

void isdb_double_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Float, name);
  write(database);
}

bool isdb_double_trace::changed()
{
    return object != old_value;
}

void isdb_double_trace::write(ISDB_Conn database)
{
  ISDB_SetValue (database, isdb_id, object);

  old_value = object;
}


/*****************************************************************************/

class isdb_enum_trace : public isdb_trace {
public:
    isdb_enum_trace(const unsigned& object_, const sc_string& name_, const sc_string& isdb_name_, const char** enum_literals);
    void declare_variable(ISDB_Conn database);
    void write(ISDB_Conn database);
    bool changed();

protected:
    const unsigned& object;
    unsigned old_value;
    unsigned mask;
    const char** literals;
    unsigned nliterals;
};


isdb_enum_trace::isdb_enum_trace(const unsigned& object_, const sc_string& name_, const sc_string& isdb_name_, const char** enum_literals_) 
  : isdb_trace( name_, isdb_name_), object( object_), literals(enum_literals_)
{
    // find number of bits required to represent enumeration literal - counting loop
    for (nliterals = 0; enum_literals_[nliterals]; nliterals++);

    // Figure out number of bits required to represent the number of literals
    bit_width = 0;
    unsigned shifted_maxindex = nliterals-1;
    while(shifted_maxindex != 0){
        shifted_maxindex >>= 1;
        bit_width++;
    }

    // Set the mask
    if (bit_width < 32) {
      mask = ~(-1 << bit_width);
    } else {
      mask = static_cast<unsigned int>( -1 ); // 0xffffffff
    }

    isdb_var_typ_name = "wire";
    old_value = object;
}       

void isdb_enum_trace::declare_variable (ISDB_Conn database)
{
  isdb_id = ISDB_DefineSignal (database, ISDB_Integer, name);
  write(database);
}

bool isdb_enum_trace::changed()
{
    return object != old_value;
}

void isdb_enum_trace::write(ISDB_Conn database)
{
  /* TBD: this may be not good enough. Do we need to consider the mask? */
  ISDB_SetValue (database, isdb_id, object);

  old_value = object;
}





/*****************************************************************************
           isdb_trace_file functions
 *****************************************************************************/


void isdb_trace_error_handler (ISDB_Conn /* database */, 
			       ISDB_ErrorType /* type */, 
			       char *msg)
{
  cout << "WARNING: error reported from ISDB: " << msg << endl;
}


isdb_trace_file::isdb_trace_file(const char *name)
{
  sc_string file_name = name ;
  file_name += ".isdb";

  //
  // Establish an error handler
  //
  ISDB_InstallErrorHandler (isdb_trace_error_handler);

  // ISDB_CreateDB takes only char* but not const char*,
  // therefore must perform a dreaded conversion to char*
  int len = file_name.length();
  char* fname = (char*)malloc(len+1 *sizeof(char));
  for (int n=0; n<len; n++) {
    fname[n] = file_name[n];
  }
  fname[len] = '\0';
			      
  database = ISDB_CreateDB (fname, 
			    ISDB_IncludeDeltaCycle);
  free(fname);

  if (!database) {
    sc_string msg = sc_string("Cannot write trace file '") + file_name + "'";
    cerr << "FATAL: " << msg << "\n";
    exit(1);
  }

  trace_delta_cycles = false;
  initialized = false;
  isdb_name_index = 0;
  
  //default timestep = 1 sec
  timescale_unit = 1; 
  timescale_set_by_user = false;
}


void isdb_trace_file::initialize()
{
  char timescale[10];
  ISDB_Time isdb_time;

  //
  // convert timescale into string
  //
  if     (timescale_unit == 1e-15) sprintf(timescale, "1fs");
  else if(timescale_unit == 1e-14) sprintf(timescale, "10fs");
  else if(timescale_unit == 1e-13) sprintf(timescale, "100fs");
  else if(timescale_unit == 1e-12) sprintf(timescale, "1ps");
  else if(timescale_unit == 1e-11) sprintf(timescale, "10ps");
  else if(timescale_unit == 1e-10) sprintf(timescale, "100ps");
  else if(timescale_unit == 1e-9)  sprintf(timescale, "1ns");
  else if(timescale_unit == 1e-8)  sprintf(timescale, "10ns");
  else if(timescale_unit == 1e-7)  sprintf(timescale, "100ns");
  else if(timescale_unit == 1e-6)  sprintf(timescale, "1us");
  else if(timescale_unit == 1e-5)  sprintf(timescale, "10us");
  else if(timescale_unit == 1e-4)  sprintf(timescale, "100us");
  else if(timescale_unit == 1e-3)  sprintf(timescale, "1ms");
  else if(timescale_unit == 1e-2)  sprintf(timescale, "10ms");
  else if(timescale_unit == 1e-1)  sprintf(timescale, "100ms");
  else if(timescale_unit == 1e0)   sprintf(timescale, "1s");
  else if(timescale_unit == 1e1)   sprintf(timescale, "10s");
  else if(timescale_unit == 1e2)   sprintf(timescale, "100s");

  //
  // Set previous-time
  // Convert init time to ISDB_Time
  // 
  
  // double inittime = sc_simulation_time();
  double inittime = sc_time_stamp().to_double();

  double_to_special_int64(inittime/timescale_unit,
			  &previous_time_units_high,
			  &previous_time_units_low );
  isdb_time.low = previous_time_units_low;
  isdb_time.high = previous_time_units_high;

  //
  // Configure ISDB database
  //
  ISDB_Configure (database, 
		  ISDB_TimeUnits,       timescale, 
		  ISDB_StartTime,       &isdb_time,
		  ISDB_PathSeparator,   "/",
		  ISDB_StructSeparator, "/",
		  NULL);

  running_regression = ( getenv( "SYSTEMC_REGRESSION" ) != NULL );
  // Don't print message if running regression
  if( ! timescale_set_by_user && ! running_regression ) {
      cout << "WARNING: Default time step is used for ISDB tracing." << endl;
  }

  // Define variables
  int i;
  for (i = 0; i < traces.size(); i++) {
    isdb_trace* t = traces[i];
    t->set_width(); // needed for all vectors
    t->declare_variable(database);
  }

}


void isdb_trace_file::sc_set_isdb_time_unit(int exponent10_seconds)
{
  /* TBD: save the exponent, exponent10_seconds, internally and use
     it later to compute the TimeScale_string more easily  */

  if(initialized){
    isdb_put_error_message("ISDB trace timescale unit cannot be changed once tracing has begun.\n"
			   "To change the scale, create a new trace file.",
			   false);
    return;
  }

  
  if(exponent10_seconds < -15 || exponent10_seconds >  2){
        isdb_put_error_message("set_isdb_time_unit() has valid exponent range -15...+2.", false);
        return;
  }

  if     (exponent10_seconds == -15) timescale_unit = 1e-15;
  else if(exponent10_seconds == -14) timescale_unit = 1e-14;
  else if(exponent10_seconds == -13) timescale_unit = 1e-13;
  else if(exponent10_seconds == -12) timescale_unit = 1e-12;
  else if(exponent10_seconds == -11) timescale_unit = 1e-11;
  else if(exponent10_seconds == -10) timescale_unit = 1e-10;
  else if(exponent10_seconds ==  -9) timescale_unit = 1e-9;
  else if(exponent10_seconds ==  -8) timescale_unit = 1e-8;
  else if(exponent10_seconds ==  -7) timescale_unit = 1e-7;
  else if(exponent10_seconds ==  -6) timescale_unit = 1e-6;
  else if(exponent10_seconds ==  -5) timescale_unit = 1e-5;
  else if(exponent10_seconds ==  -4) timescale_unit = 1e-4;
  else if(exponent10_seconds ==  -3) timescale_unit = 1e-3;
  else if(exponent10_seconds ==  -2) timescale_unit = 1e-2;
  else if(exponent10_seconds ==  -1) timescale_unit = 1e-1;
  else if(exponent10_seconds ==   0) timescale_unit = 1e0;
  else if(exponent10_seconds ==   1) timescale_unit = 1e1;
  else if(exponent10_seconds ==   2) timescale_unit = 1e2;
  
  char buf[200];
  sprintf(buf, "Note: ISDB trace timescale unit is set by user to 1e%d sec.\n", exponent10_seconds);
  cout << buf << flush;
  timescale_set_by_user = true;
}


void isdb_trace_file::trace(const bool& object, const sc_string& name)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_bool_trace(object, name, temp_isdb_name));
}

void isdb_trace_file::trace(const sc_bit& object, const sc_string& name)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_sc_bit_trace(object, name, temp_isdb_name));
}

void isdb_trace_file::trace(const sc_logic& object, const sc_string& name)
{
  if(initialized)
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);

  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_sc_logic_trace(object, name, temp_isdb_name));
}


void isdb_trace_file::trace(const unsigned& object, const sc_string& name, int width_)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_unsigned_int_trace(object, name, temp_isdb_name, width_));
}

void isdb_trace_file::trace(const unsigned char& object, const sc_string& name, int width_)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_unsigned_char_trace(object, name, temp_isdb_name, width_));
}

void isdb_trace_file::trace(const unsigned short& object, const sc_string& name, int width_)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_unsigned_short_trace(object, name, temp_isdb_name, width_));
}

void isdb_trace_file::trace(const unsigned long& object, const sc_string& name, int width_)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_unsigned_long_trace(object, name, temp_isdb_name, width_));
}

void isdb_trace_file::trace(const int& object, const sc_string& name, int width_)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_signed_int_trace(object, name, temp_isdb_name, width_));
}

void isdb_trace_file::trace(const char& object, const sc_string& name, int width_)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_signed_char_trace(object, name, temp_isdb_name, width_));
}

void isdb_trace_file::trace(const short& object, const sc_string& name, int width_)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_signed_short_trace(object, name, temp_isdb_name, width_));
}

void isdb_trace_file::trace(const long& object, const sc_string& name, int width_)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_signed_long_trace(object, name, temp_isdb_name, width_));
}

void isdb_trace_file::trace(const float& object, const sc_string& name)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_float_trace(object, name, temp_isdb_name));
}

void isdb_trace_file::trace(const double& object, const sc_string& name)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_double_trace(object, name, temp_isdb_name));
}

void isdb_trace_file::trace(const sc_unsigned& object, const sc_string& name)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_sc_unsigned_trace(object, name, temp_isdb_name));
}

void isdb_trace_file::trace(const sc_signed& object, const sc_string& name)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_sc_signed_trace(object, name, temp_isdb_name));
}


void isdb_trace_file::trace(const sc_int_base& object, const sc_string& name)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_sc_int_base_trace(object, name, temp_isdb_name));
}

void isdb_trace_file::trace(const sc_uint_base& object, const sc_string& name)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_sc_uint_base_trace(object, name, temp_isdb_name));
}


#define DEFN_TRACE_METHOD(tp)                                                 \
void                                                                          \
isdb_trace_file::trace( const tp& object, const sc_string& name )             \
{                                                                             \
    if( initialized )                                                         \
        isdb_put_error_message(                                               \
	    "No traces can be added once simulation has started.\n"           \
            "To add traces, create a new isdb trace file.", false );          \
                                                                              \
    sc_string temp_isdb_name;                                                 \
    create_isdb_name( &temp_isdb_name );                                      \
    traces.push_back( new isdb_ ## tp ## _trace( object,                      \
						 name,                        \
						 temp_isdb_name ) );          \
}

DEFN_TRACE_METHOD(sc_fxval)
DEFN_TRACE_METHOD(sc_fxval_fast)
DEFN_TRACE_METHOD(sc_fxnum)
DEFN_TRACE_METHOD(sc_fxnum_fast)

#undef DEFN_TRACE_METHOD


void
isdb_trace_file::trace(const unsigned& object, const sc_string& name, const char** enum_literals)
{
  if(initialized)  
    isdb_put_error_message("No traces can be added once simulation has started.\n"
			   "To add traces, create a new isdb trace file.", false);
  
  sc_string temp_isdb_name;
  create_isdb_name(&temp_isdb_name);
  traces.push_back(new isdb_enum_trace(object, name, temp_isdb_name, enum_literals));
}


void
isdb_trace_file::write_comment(const sc_string& /* comment */)
{
  /* TBD: comments not supported */
}


void
isdb_trace_file::delta_cycles(bool flag)
{
    trace_delta_cycles = flag;
}

void
isdb_trace_file::cycle(bool this_is_a_delta_cycle)
{
  char message[4000];
  unsigned this_time_units_high, this_time_units_low;
    
  // Just to make g++ shut up in the optimized mode
  this_time_units_high = this_time_units_low = 0;

  // Trace delta cycles only when enabled
  if (!trace_delta_cycles && this_is_a_delta_cycle) return;

  // Check for initialization
  if (!initialized) {
    initialize();
    initialized = true;
    return;
  };


  // double now_units = sc_simulation_time() / timescale_unit;
  double now_units = sc_time_stamp().to_double() / timescale_unit;
  
  unsigned now_units_high, now_units_low;
  double_to_special_int64(now_units, &now_units_high, &now_units_low );

  bool now_later_than_previous_time = false;
  if(      now_units_low > previous_time_units_low 
        && now_units_high == previous_time_units_high
      ||   now_units_high > previous_time_units_high){
    now_later_than_previous_time = true;
  }

  bool now_equals_previous_time = false;
  if (now_later_than_previous_time) {
    this_time_units_high = now_units_high;
    this_time_units_low = now_units_low;

  } else {
     if (   now_units_low == previous_time_units_low
	    && now_units_high == previous_time_units_high)
       {
	   now_equals_previous_time = true;
       }
  }


  // Since ISDB does not understand 0 time progression, we have to fake
  // delta cycles with progressing time by one unit
    if(this_is_a_delta_cycle){
        this_time_units_high = previous_time_units_high;
        this_time_units_low = previous_time_units_low + 1;
        if(this_time_units_low == 1000000000){
            this_time_units_high++;
            this_time_units_low=0;
        }
        static bool warned = false;
        if(!warned){
	    cout << "Note: VCD delta cycling with pseudo timesteps (1 unit) "
		    "is performed.\n" << endl;
            warned = true;
        }
    }
  

  // Not a delta cycle and time has not progressed
  if(!this_is_a_delta_cycle && now_equals_previous_time){
    static bool warned = false;
    if( ! warned && ! running_regression ) {
      sprintf(message,
	      "Multiple cycles found with same (%u) time units count.\n"
	      "Waveform viewers will only show the states of the last one.\n"
	      "Use ((isdb_trace_file*)isdbfile)->sc_set_isdb_time_unit(int exponent10_seconds)\n"
	      "to increase time resolution.",
	      now_units_low
	      );
      isdb_put_error_message(message, true);
      warned = true;
    }
    this_time_units_high = now_units_high;
    this_time_units_low = now_units_low;
  }

  // Not a delta cycle and time has gone backward
  // This will happen with large number of delta cycles between two real advances of time
  if(!this_is_a_delta_cycle && !now_equals_previous_time && !now_later_than_previous_time){
    static bool warned = false;
    if(!warned){
      sprintf(message,
	      "Cycle found with falling (%u -> %u) time units count.\n"
	      "This can occur when delta cycling is activated.\n"
	      "Cycles with falling time are not shown.\n"
	      "Use ((isdb_trace_file*)isdbfile)->sc_set_isdb_time_unit(int exponent10_seconds)\n"
	      "to increase time resolution.",
	      previous_time_units_low, now_units_low);
      isdb_put_error_message(message, true);
      warned = true;
    }
    // Note that we don't set this_time_units_high/low to any value only in this case because
    // we are not going to do any tracing. In the optimized mode, the compiler complains because
    // of this. Therefore, we include the lines at the very beginning of this function to make the
    // compiler shut up.
    return; 
  }

  //
  // Now do the actual printing 
  //
  bool time_printed = false;
  isdb_trace* const* const l_traces = traces.raw_data();
  for (int i = 0; i < traces.size(); i++) {
    isdb_trace* t = l_traces[i];
    if(t->changed()){

      // Set time stamp on the fly
      if(time_printed == false){

	ISDB_Time time;
	time.low  = this_time_units_low;
	time.high = this_time_units_high;
	ISDB_SetTime (database, time);
	time_printed = true;
      }

      // Write the variable
      t->write(database);
    }
  }

  if(time_printed){
    // We update previous_time_units only when we print time because
    // this field stores the previous time that was printed, not the
    // previous time this function was called
    previous_time_units_high = this_time_units_high;
    previous_time_units_low = this_time_units_low;
  }
}


void
isdb_trace_file::create_isdb_name(sc_string* p_destination)
{
    const char first_type_used = 'a';
    const int used_types_count = 'z' - 'a' + 1;
    int result;

    char char4 = (char)(isdb_name_index % used_types_count);

    result = isdb_name_index / used_types_count;
    char char3 = (char)(result % used_types_count);

    result = result / used_types_count;
    char char2 = (char)(result % used_types_count);

    char buf[20];
    sprintf(buf, "%c%c%c",
            char2 + first_type_used,
            char3 + first_type_used,
            char4 + first_type_used);
    *p_destination = buf; 
    isdb_name_index++;
}

isdb_trace_file::~isdb_trace_file()
{
  int i;
  for (i = 0; i < traces.size(); i++) {
    isdb_trace* t = traces[i];
    delete t;
  }
  ISDB_End (database,ISDB_DONE);
}


// Functions specific to ISDB tracing

static
unsigned int
map_sc_logic_state_to_isdb_binval (char val)
{
  if      (val=='1') return ISDB_HIGH;
  else if (val=='0') return ISDB_LOW;
  else if (val=='Z') return ISDB_Z;
  else               return ISDB_X;
}


static
void
isdb_put_error_message(const char* msg, bool just_warning)
{
    if(just_warning){
	cout << "ISDB Trace Warning:\n" << msg << "\n" << endl;
    }
    else{
	cout << "ISDB Trace ERROR:\n" << msg << "\n" << endl;
    }
}


sc_trace_file*
sc_create_isdb_trace_file(const char * name)
{
  sc_trace_file *tf;

  tf = new isdb_trace_file(name);
  sc_get_curr_simcontext()->add_trace_file(tf);
  return tf;
}

void
sc_close_isdb_trace_file( sc_trace_file* tf )
{
    isdb_trace_file* isdb_tf = (isdb_trace_file*)tf;
    delete isdb_tf;
}
