        #include "QueueAr.h"

        /**
         * Construct the queue.
         */
        template <class Object>
        QueueAr<Object>::QueueAr( int capacity ) : theArray( capacity )
        {
            makeEmpty( );
        }

        /**
         * Test if the queue is logically empty.
         * Return true if empty, false otherwise.
         */
        template <class Object>
        bool QueueAr<Object>::isEmpty( ) const
        {
            return currentSize == 0;
        }

        /**
         * Test if the queue is logically full.
         * Return true if full, false otherwise.
         */
        template <class Object>
        bool QueueAr<Object>::isFull( ) const
        {
            return currentSize == theArray.size( );
        }

        /**
         * Make the queue logically empty.
         */
        template <class Object>
        void QueueAr<Object>::makeEmpty( )
        {
            currentSize = 0;
            front = 0;
            back = -1;
        }

        /**
         * Get the least recently inserted item in the queue.
         * Return the least recently inserted item in the queue
         * or throw Underflow if empty.
         */
        template <class Object>
        const Object & QueueAr<Object>::getFront( ) const
        {
            if( isEmpty( ) )
                throw Underflow( );
            return theArray[ front ];
        }

        /**
         * Return and remove the least recently inserted item from the queue.
         * Throw Underflow if empty.
         */
        template <class Object>
        Object QueueAr<Object>::dequeue( )
        {
            if( isEmpty( ) )
                throw Underflow( );

            currentSize--;
            Object frontItem = theArray[ front ];
            increment( front );
            return frontItem;
        }

        /**
         * Insert x into the queue.
         * Throw Overflow if queue is full
         */
        template <class Object>
        void QueueAr<Object>::enqueue( const Object & x )
        {
            if( isFull( ) )
                throw Overflow( );
            increment( back );
            theArray[ back ] = x;
            currentSize++;
        }

        /**
         * Internal method to increment x with wraparound.
         */
        template <class Object>
        void QueueAr<Object>::increment( int & x )
        {
            if( ++x == theArray.size( ) )
                x = 0;
        }
        
        template <class Object>
        int QueueAr<Object>::get( const Object & x )
        {
            if( x < theArray.size( ) )
                return theArray[x];
              
              return -1;
        }
