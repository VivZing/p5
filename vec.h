#ifndef _VEC_H
#define _VEC_H

/*
 * The next line is used because Codewarrior has a conflict with
 * the STL vec. Make sure to put the #include of this file
 * AFTER all the system includes.
 */

class ArrayIndexOutOfBounds { };

template <class Object>
class vec
{
  public:
    explicit vec( int theSize = 0 ) : currentSize( theSize )
      { objects = new Object[ currentSize ]; }
    vec( const vec & rhs ) : objects( NULL )
      { operator=( rhs ); }
    ~vec( )
      { delete [ ] objects; }

    int size( ) const
      { return currentSize; }

    Object & operator[]( int index )
    {
                                                     #ifndef NO_CHECK
        if( index < 0 || index >= currentSize )
            throw ArrayIndexOutOfBounds( );
                                                     #endif
        return objects[ index ];
    }

    const Object & operator[]( int index ) const
    {
                                                     #ifndef NO_CHECK
        if( index < 0 || index >= currentSize )
            throw ArrayIndexOutOfBounds( );
                                                     #endif
        return objects[ index ];
    }


    const vec & operator = ( const vec & rhs );
    void resize( int newSize );
  private:
    int currentSize;
    Object * objects;
};

#include "vec.cpp"
#endif


