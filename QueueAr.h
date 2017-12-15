        #ifndef _QUEUEAR_H
        #define _QUEUEAR_H

        #include "vec.h"
        #include "dsexceptions.h"

        // Queue class -- array implementation
        //
        // CONSTRUCTION: with or without a capacity; default is 10
        //
        // ******************PUBLIC OPERATIONS*********************
        // void enqueue( x )      --> Insert x
        // void dequeue( )        --> Return and remove least recently inserted item
        // Object getFront( )     --> Return least recently inserted item
        // bool isEmpty( )        --> Return true if empty; else false
        // bool isFull( )         --> Return true if full; else false
        // void makeEmpty( )      --> Remove all items
        // ******************ERRORS********************************
        // Overflow and Underflow thrown as needed
        
        template <class Object>
        class QueueAr
        {
          public:
            explicit QueueAr( int capacity = 10 );

            bool isEmpty( ) const;
            bool isFull( ) const;
            const Object & getFront( ) const;

            void makeEmpty( );
            Object dequeue( );
            void enqueue( const Object & x );
            int get( const Object & x );
            int            front;
            int            back;

          private:
            vec<Object> theArray;
            int            currentSize;
            

            void increment( int & x );
        };

        #include "QueueAr.cpp"
        #endif
