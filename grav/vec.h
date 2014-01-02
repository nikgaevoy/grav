#ifndef __VEC_H_
#define __VEC_H_

#include <cmath>

namespace mth
{
  const double Pi = 3.14159265358979323846;
  
  template<class Type>
  class vec;
  
  template<class Type>
    class vec
    {
    public:
      
      Type X, Y;

      vec( VOID )
      {
      } 

      vec( Type A, Type B ) : X(A), Y(B)
      {
      } 

      Type & operator[]( INT I )
      {
        assert(I >= 0 && I < 2);
        return *(&X + I);
      } 

      Type operator&( const vec &V ) const
      {
        return X * V.X + Y * V.Y;
      } 

      Type operator%( const vec &V ) const
      {
        return X * V.Y - Y * V.X;
      } 

      vec operator*( const vec &V ) const
      {
        return vec(X * V.X, Y * V.Y);
      } 

      vec & operator*=( const vec &V )
      {
        X *= V.X;
        Y *= V.Y;
        return *this;
      } 

      vec operator*( Type N ) const
      {
        return vec(X * N, Y * N);
      } 

      vec & operator*=( Type N )
      {
        X *= N;
        Y *= N;
        return *this;
      } 

      vec operator/( Type N ) const
      {
        return vec(X / N, Y / N);
      } 

      vec & operator/=( Type N )
      {
        X /= N;
        Y /= N;
        return *this;
      } 

      vec operator+( const vec &V ) const
      {
        return vec(X + V.X, Y + V.Y);
      } 

      vec & operator+=( const vec &V )
      {
        X += V.X;
        Y += V.Y;
        return *this;
      } 

      vec operator-( const vec &V ) const
      {
        return vec(X - V.X, Y - V.Y);
      } 

      vec & operator-=( const vec &V )
      {
        X -= V.X;
        Y -= V.Y;
        return *this;
      } 

      vec operator-( VOID ) const
      {
        return vec(-X, -Y);
      } 

      vec & Negate( VOID )
      {
        X = -X;
        Y = -Y;
        return *this;
      } 

      vec Normalizing( VOID ) const
      {
        Type len = X * X + Y * Y;

        if (len == 0 || len == 1)
          return *this;
        len = sqrt(len);
        return vec(X / len, Y / len);
      } 

      vec & Normalize( VOID )
      {
        Type len = X * X + Y * Y;

        if (len == 0 || len == 1)
          return *this;
        len = sqrt(len);
        X /= len;
        Y /= len;
        return *this;
      } 

      Type operator!( VOID ) const
      {
        Type len = X * X + Y * Y;

        if (len == 0 || len == 1)
          return len;
        return sqrt(len);
      } 
      
      Type Len2( VOID ) const
      {
        return X * X + Y * Y;
      } 

      vec Clamping( Type Min = 0, Type Max = 1 ) const
      {
        return vec(X < Min ? Min : X > Max ? Max : X,
                   Y < Min ? Min : Y > Max ? Max : Y);
      } 
      
      vec & Clamp( Type Min = 0, Type Max = 1 )
      {
        if (X < Min)
          X = Min;
        else
          if (X > Max)
            X = Max;
        if (Y < Min)
          Y = Min;
        else
          if (Y > Max)
            Y = Max;
        return *this;
      } 
      
      vec<Type> ConvVec( Type Y = 0 )
      {
        return vec<Type>(X, Y, Y);
      } 
    }; 
} 
#endif 
