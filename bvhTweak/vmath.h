/* -*- C++ -*- */
/*
 * vmath, set of classes for computer graphics mathemtics.
 * Copyright (c) 2005-2009, Jan Bartipan < barzto at gmail dot com >
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 *
 * - Redistributions of source code must retain the above copyright 
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright 
 *   notice, this list of conditions and the following disclaimer in 
 *   the documentation and/or other materials provided with the 
 *   distribution.
 * - Neither the names of its contributors may be used to endorse or 
 *   promote products derived from this software without specific 
 *   prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @mainpage Vector mathemtics for computer graphics
 *
 * @section Features
 * <ul>
 *    <li> basic aritemetic operations - using operators </li>
 *    <li> basic linear algebra operations - such as transpose, dot product, etc. </li>
 *    <li> aliasis for vertex coordinates - it means:
 *    <pre>
 *  Vector3f v;
 *  // use vertex coordinates
 *  v.x = 1; v.y = 2; v.z = -1;
 *
 *  // use texture coordinates
 *  v.s = 0; v.t = 1; v.u = 0.5;
 *  // use color coordinates
 *  v.r = 1; v.g = 0.5; v.b = 0;
 *    </pre>
 *    </li>
 *    <li> conversion constructor and assign operators - so you can assign a value of Vector3&lt;T1&gt; type 
 *    to a variable of Vector3&lt;T2&gt; type for any convertable T1, T2 type pairs. In other words, you can do this:
 *    <pre>
 *
 *  Vector3f f3; Vector3d d3 = f3;
 *  ...
 *  f3 = d3;
 *    </pre>
 *    </li>
 * </ul>
 */



#ifndef __vmath_Header_File__
#define __vmath_Header_File__




#include <cmath>
#include <cstring>
#include <iostream>
#include <cassert>



#ifdef VMATH_NAMESPACE
namespace VMATH_NAMESPACE
{
#endif

#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif
   
#define DEG2RAD(x) ((x * M_PI) / 180.0)
   //#define EPSILON (4.37114e-07)
   
   const double epsilon = 4.37114e-05;
#define EPSILON epsilon
   
   
   /**
    * Class for two dimensional vector.
    */
   template <class T>
   class Vector2 
   {
      public:
	 union 
	 {
	       /**
		* First element of vector, alias for X-coordinate.
		*/
	       T x;
		
	       /**
		* First element of vector, alias for S-coordinate.
		* For textures notation.
		*/
	       T s; 
	 };
	
	 union 
	 {
	       /**
		* Second element of vector, alias for Y-coordinate.
		*/
	       T y; 
		
	       /**
		* Second element of vector, alias for T-coordinate.
		* For textures notation.
		*/
	       T t; 
	 };

	 //----------------[ constructors ]--------------------------
	 /**
	  * Creates and sets to (0,0)
	  */
	 Vector2() : x(0),y(0)
	 {	}
	
	

	 /**
	  * Creates and sets to (x,y)
	  * @param nx intial x-coordinate value
	  * @param ny intial y-coordinate value
	  */
	 Vector2(T nx, T ny) : x(nx), y(ny)
	 {	}


	 /**
	  * Copy constructor.
	  * @param src Source of data for new created instace.
	  */
	 Vector2(const Vector2<T>& src)
	    : x(src.x), y(src.y) 
	 {	 }


	 /**
	  * Copy casting constructor.
	  * @param src Source of data for new created instace.
	  */
	 template <class FromT>
	 Vector2(const Vector2<FromT>& src)
	    : x(static_cast<T>(src.x)),
	      y(static_cast<T>(src.y))
	 {	 }
	
	
	 //----------------[ access operators ]-------------------
	 /**
	  * Copy casting operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 template <class FromT>
	 Vector2<T>& operator=(const Vector2<FromT>& rhs)
	 {
	    x = static_cast<T>(rhs.x);
	    y = static_cast<T>(rhs.y);
	    return * this;
	 }

	 /**
	  * Copy operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T>& operator=(const Vector2<T>& rhs)
	 {
	    x = rhs.x;
	    y = rhs.y;
	    return * this;
	 }


	 /**
	  * Array access operator
	  * @param n Array index
	  * @return For n = 0, reference to x coordinate, else reference to y 
	  * y coordinate.
	  */
	 T& operator[](int n)
	 {
	    assert(n >= 0 && n <= 1);
	    if (0 == n) return x;
	    else
	       return y;
	 }
	
	
	 //---------------[ vector aritmetic operator ]--------------
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T> operator+(const Vector2<T>& rhs) const
	 {
	    return Vector2<T> (x + rhs.x, y + rhs.y);
	 }

	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T> operator-(const Vector2<T>& rhs) const
	 {
	    return Vector2<T> (x - rhs.x, y - rhs.y);
	 }

	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T> operator*(const Vector2<T>& rhs) const
	 {
	    return Vector2<T> (x * rhs.x, y * rhs.y);
	 }

	 /**
	  * Division operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T> operator/(const Vector2<T>& rhs) const 
	 {
	    return Vector2<T> (x / rhs.x, y / rhs.y);
	 }

	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T>& operator+=(const Vector2<T>& rhs)
	 {
	    x += rhs.x;
	    y += rhs.y;
	    return * this;
	 }

	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T>& operator-=(const Vector2<T>& rhs)
	 {
	    x -= rhs.x;
	    y -= rhs.y;
	    return * this;
	 }
	
	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T>& operator*=(const Vector2<T>& rhs)
	 {
	    x *= rhs.x;
	    y *= rhs.y;
	    return * this;
	 }
	
	 /**
	  * Division operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T>& operator/=(const Vector2<T>& rhs)
	 {
	    x /= rhs.x;
	    y /= rhs.y;
	    return * this;
	 }

	 //--------------[ scalar vector operator ]--------------------
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T> operator+(T rhs) const
	 {
	    return Vector2<T> (x + rhs, y + rhs);
	 }
	
	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T> operator-(T rhs) const
	 {
	    return Vector2<T> (x - rhs, y - rhs);
	 }
	
	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T> operator*(T rhs) const
	 {
	    return Vector2<T> (x * rhs, y * rhs);
	 }
	
	 /**
	  * Division operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T> operator/(T rhs) const
	 {
	    return Vector2<T> (x / rhs, y / rhs);
	 }

	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T>& operator+=(T rhs)
	 {
	    x += rhs;
	    y += rhs;
	    return * this;
	 }

	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T>& operator-=(T rhs) 
	 {
	    x -= rhs;
	    y -= rhs;
	    return * this;
	 }

	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T>& operator*=(T rhs)
	 {
	    x *= rhs;
	    y *= rhs;
	    return * this;
	 }

	 /**
	  * Division operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector2<T>& operator/=(T rhs) 
	 {
	    x /= rhs;
	    y /= rhs;
	    return * this;
	 }

	 //--------------[ equality operator ]------------------------
	 /**
	  * Equality test operator
	  * @param rhs Right hand side argument of binary operator.
	  * @note Test of equality is based of threshold EPSILON value. To be two
	  * values equal, must satisfy this condition | lws.x - rhs.y | < EPSILON,
	  * same for y-coordinate.
	  */
	 bool operator==(const Vector2<T>& rhs) const 
	 {
	    return (std::abs(x - rhs.x) < EPSILON) && (std::abs(y - rhs.y) < EPSILON);
	 }

	
	 /**
	  * Inequality test operator
	  * @param rhs Right hand side argument of binary operator.
	  * @return not (lhs == rhs) :-P
	  */
	 bool operator!=(const Vector2<T>& rhs) const { return ! (*this == rhs); }

	 //-------------[ unary operations ]--------------------------
	 /**
	  * Unary negate operator
	  * @return negated vector
	  */
	 Vector2<T> operator-() const
	 {
	    return Vector2<T>(-x, -y);
	 }

	 //-------------[ size operations ]---------------------------
	 /**
	  * Get lenght of vector.
	  * @return lenght of vector
	  */
	 T length() const
	 {
	    return (T)std::sqrt(x * x + y * y);
	 }

	 /**
	  * Normalize vector
	  */
	 void normalize()
	 {
	    T s = length();
	    x /= s;
	    y /= s;
	 }

	 /**
	  * Return square of length.
	  * @return lenght ^ 2
	  * @note This method is faster then length(). For comparsion
	  * of length of two vector can be used just this value, instead
	  * of computionaly more expensive length() method.
	  */
	 T lengthSq() const
	 {
	    return x * x + y * y;
	 }
	
	 //--------------[ misc. operations ]-----------------------
	 /**
	  * Linear interpolation of two vectors
	  * @param fact Factor of interpolation. For translation from positon
	  * of this vector to vector r, values of factor goes from 0.0 to 1.0.
	  * @param r Second Vector for interpolation 
	  * @note Hovewer values of fact parameter are reasonable only in interval
	  * [0.0 , 1.0], you can pass also values outside of this interval and you
	  * can get result (extrapolation?)
	  */
	 Vector2<T> lerp(T fact, const Vector2<T>& r) const
	 {
	    return (*this) + (r - (*this)) * fact;	
	 }
	
    
	 //-------------[ conversion ]-----------------------------
	 /**
	  * Conversion to pointer operator
	  * @return Pointer to internaly stored (in managment of class Vector2<T>)
	  * used for passing Vector2<T> values to gl*2[fd] functions.
	  */
	 operator T*(){ return (T*) this; }
	 /**
	  * Conversion to pointer operator
	  * @return Constant Pointer to internaly stored (in managment of class Vector2<T>)
	  * used for passing Vector2<T> values to gl*2[fd] functions.
	  */
	 operator const T*() const { return (const T*) this; }

	 //-------------[ output operator ]------------------------
	 /**
	  * Output to stream operator
	  * @param lhs Left hand side argument of operator (commonly ostream instance). 
	  * @param rhs Right hand side argument of operator.
	  * @return Left hand side argument - the ostream object passed to operator.
	  */
	 friend std::ostream& operator<<(std::ostream& lhs, const Vector2<T>& rhs) 
	 {
	    lhs << "[" << rhs.x << "," << rhs.y << "]";
	    return lhs;
	 }


   };


   //--------------------------------------
   // Typedef shortcuts for 2D vector
   //-------------------------------------
   /// Two dimensional Vector of floats
   typedef class Vector2 <float> Vector2f;
   /// Two dimensional Vector of doubles
   typedef class Vector2 <double> Vector2d;


   /**
    * Class for three dimensional vector.
    */
   template <class T>
   class Vector3 
   {
      public:
	 //T x, y, z;
	 union
	 {
	       /**
		* First element of vector, alias for X-coordinate.
		*/
	       T x;
		
	       /**
		* First element of vector, alias for S-coordinate.
		* For textures notation.
		*/
	       T s;
		
	       /**
		* First element of vector, alias for R-coordinate.
		* For color notation.
		*/
	       T r; 
	 };
	
	 union
	 {
	       /**
		* Second element of vector, alias for Y-coordinate.
		*/
	       T y; 
	       /**
		* Second element of vector, alias for T-coordinate.
		* For textures notation.
		*/
	       T t; 
	       /**
		* Second element of vector, alias for G-coordinate.
		* For color notation.
		*/
	       T g; 
	 };
	
	 union
	 {
	       /**
		* Third element of vector, alias for Z-coordinate.
		*/
	       T z;
		
	       /**
		* Third element of vector, alias for U-coordinate.
		* For textures notation.
		*/
	       T u; 
	       /**
		* Third element of vector, alias for B-coordinate.
		* For color notation.
		*/
	       T b; 
	 };
	
	 //----------------[ constructors ]--------------------------
	 /**
	  * Creates and sets to (0,0,0)
	  */
	 Vector3() : x(0),y(0),z(0)
	 {	}

	 /**
	  * Creates and sets to (x,y,z)
	  * @param nx intial x-coordinate value
	  * @param ny intial y-coordinate value
	  * @param nz intial z-coordinate value
	  */
	 Vector3(T nx, T ny, T nz) : x(nx),y(ny),z(nz)
	 {	}

	 /**
	  * Copy constructor.
	  * @param src Source of data for new created Vector3 instance.
	  */
	 Vector3(const Vector3<T>& src)
	    : x(src.x), y(src.y), z(src.z)
	 {}
	 
	 /**
	  * Copy casting constructor.
	  * @param src Source of data for new created Vector3 instance.
	  */
	 template <class FromT>
	 Vector3(const Vector3<FromT>& src)
	    : x(static_cast<T>(src.x)),
	      y(static_cast<T>(src.y)),
	      z(static_cast<T>(src.z))
	 {}
	
	
	 //----------------[ access operators ]-------------------
	 /**
	  * Copy operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T> operator=(const Vector3<T>& rhs) 
	 {
	    x = rhs.x;
	    y = rhs.y;
	    z = rhs.z;
	    return * this;
	 }

	 /**
	  * Copy casting operator.
	  * @param rhs Right hand side argument of binary operator.
	  */
	 template <class FromT>
	 Vector3<T> operator=(const Vector3<FromT>& rhs)
	 {
	    x = static_cast<T>(rhs.x);
	    y = static_cast<T>(rhs.y);
	    z = static_cast<T>(rhs.z);
	    return * this;
	 }

	 /**
	  * Array access operator
	  * @param n Array index
	  * @return For n = 0, reference to x coordinate, n = 1
	  * reference to y, else reference to z 
	  * y coordinate.
	  */
	 T & operator[](int n)
	 {
	    assert(n >= 0 && n <= 2);
	    if (0 == n) return x;
	    else if (1 == n) return y;
	    else
	       return z;
	 }


	 //---------------[ vector aritmetic operator ]--------------
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T> operator+(const Vector3<T>& rhs) const 
	 {
	    return Vector3<T> (x + rhs.x, y + rhs.y, z + rhs.z);
	 }
	
	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T> operator-(const Vector3<T>& rhs) const 
	 {
	    return Vector3<T> (x - rhs.x, y - rhs.y, z - rhs.z);
	 }
	
	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T> operator*(const Vector3<T>& rhs) const 
	 {
	    return Vector3<T> (x * rhs.x, y * rhs.y, z * rhs.z);
	 }

	 /**
	  * Division operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T> operator/(const Vector3<T>& rhs) const 
	 {
	    return Vector3<T> (x / rhs.x, y / rhs.y, z / rhs.z);
	 }

	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T>& operator+=(const Vector3<T>& rhs) 
	 {
	    x += rhs.x;
	    y += rhs.y;
	    z += rhs.z;
	    return * this;
	 }

	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T>& operator-=(const Vector3<T>& rhs) 
	 {
	    x -= rhs.x;
	    y -= rhs.y;
	    z -= rhs.z;
	    return * this;
	 }

	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T>& operator*=(const Vector3<T>& rhs) 
	 {
	    x *= rhs.x;
	    y *= rhs.y;
	    z *= rhs.z;
	    return * this;
	 }

	 /**
	  * Division operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T>& operator/=(const Vector3<T>& rhs) 
	 {
	    x /= rhs.x;
	    y /= rhs.y;
	    z /= rhs.z;
	    return * this;
	 }

	 /**
	  * Dot product of two vectors.
	  * @param rhs Right hand side argument of binary operator.
	  */
	 T dotProduct(const Vector3<T>& rhs) const 
	 {
	    return x * rhs.x + y * rhs.y + z * rhs.z;
	 }

	 /**
	  * Cross product opertor
	  * @param rhs Right hand side argument of binary operator.
	  */	
	 Vector3<T> crossProduct(const Vector3<T>& rhs) const 
	 {
	    return Vector3<T> (y * rhs.z - rhs.y * z, z * rhs.x - rhs.z * x, x * rhs.y - rhs.x * y);
	 }
	

	 //--------------[ scalar vector operator ]--------------------
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T> operator+(T rhs) const 
	 {
	    return Vector3<T> (x + rhs, y + rhs, z + rhs);
	 }

	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T> operator-(T rhs) const 
	 {
	    return Vector3<T> (x - rhs, y - rhs, z - rhs);
	 }

	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T> operator*(T rhs) const 
	 {
	    return Vector3<T> (x * rhs, y * rhs, z * rhs);
	 }
	
	 /**
	  * Division operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T> operator/(T rhs) const 
	 {
	    return Vector3<T> (x / rhs, y / rhs, z / rhs);
	 }
	
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T>& operator+=(T rhs) 
	 {
	    x += rhs;
	    y += rhs;
	    z += rhs;
	    return * this;
	 }

	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T>& operator-=(T rhs) 
	 {
	    x -= rhs;
	    y -= rhs;
	    z -= rhs;
	    return * this;
	 }

	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T>& operator*=(T rhs) 
	 {
	    x *= rhs;
	    y *= rhs;
	    z *= rhs;
	    return * this;
	 }

	 /**
	  * Division operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T>& operator/=(T rhs) 
	 {
	    x /= rhs;
	    y /= rhs;
	    z /= rhs;
	    return * this;
	 }

	 //--------------[ equiality operator ]------------------------
	 /**
	  * Equality test operator
	  * @param rhs Right hand side argument of binary operator.
	  * @note Test of equality is based of threshold EPSILON value. To be two
	  * values equal, must satisfy this condition | lws.x - rhs.y | < EPSILON,
	  * same for y-coordinate, and z-coordinate.
	  */
	 bool operator==(const Vector3<T>& rhs) const
	 {
	    return std::fabs(x - rhs.x) < EPSILON 
	       && std::fabs(y - rhs.y) < EPSILON 
	       && std::fabs(z - rhs.z) < EPSILON;
	 }

	 /**
	  * Inequality test operator
	  * @param rhs Right hand side argument of binary operator.
	  * @return not (lhs == rhs) :-P
	  */
	 bool operator!=(const Vector3<T>& rhs) const { return !(*this == rhs); }

	 //-------------[ unary operations ]--------------------------
	 /**
	  * Unary negate operator
	  * @return negated vector
	  */
	 Vector3<T> operator-() const
	 {
	    return Vector3<T>(-x, -y, -z);
	 }

	 //-------------[ size operations ]---------------------------
	 /**
	  * Get lenght of vector.
	  * @return lenght of vector
	  */
	 T length() const 
	 {
	    return (T)std::sqrt(x * x + y * y + z * z);
	 }
	
	 /**
	  * Return square of length.
	  * @return lenght ^ 2
	  * @note This method is faster then length(). For comparsion
	  * of length of two vector can be used just this value, instead
	  * of computionaly more expensive length() method.
	  */
	 T lengthSq() const 
	 {
	    return x * x + y * y + z * z;
	 }
	
	 /**
	  * Normalize vector
	  */
	 void normalize() 
	 {
	    T s = length();
	    x /= s;
	    y /= s;
	    z /= s;
	 }

	 //------------[ other operations ]---------------------------
	 /**
	  * Rotate vector around three axis.
	  * @param ax Angle (in degrees) to be rotated around X-axis.
	  * @param ay Angle (in degrees) to be rotated around Y-axis.
	  * @param az Angle (in degrees) to be rotated around Z-axis.
	  */
	 void rotate(T ax, T ay, T az) 
	 {
	    T a = cos(DEG2RAD(ax));
	    T b = sin(DEG2RAD(ax));
	    T c = cos(DEG2RAD(ay));
	    T d = sin(DEG2RAD(ay));
	    T e = cos(DEG2RAD(az));
	    T f = sin(DEG2RAD(az));
	    T nx = c * e * x - c * f * y + d * z;
	    T ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	    T nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	    x = nx; y = ny; z = nz;
		
		
	 }
	
	 /**
	  * Linear interpolation of two vectors
	  * @param fact Factor of interpolation. For translation from positon
	  * of this vector to vector r, values of factor goes from 0.0 to 1.0.
	  * @param r Second Vector for interpolation 
	  * @note Hovewer values of fact parameter are reasonable only in interval
	  * [0.0 , 1.0], you can pass also values outside of this interval and you
	  * can get result (extrapolation?)
	  */
	 Vector3<T> lerp(T fact, const Vector3<T>& r) const
	 {
	    return (*this) + (r - (*this)) * fact;	
	 }
	
    



	 //-------------[ conversion ]-----------------------------
	
	 /**
	  * Conversion to pointer operator
	  * @return Pointer to internaly stored (in managment of class Vector3<T>)
	  * used for passing Vector3<T> values to gl*3[fd] functions.
	  */
	 operator T*(){ return (T*) this; }

	 /**
	  * Conversion to pointer operator
	  * @return Constant Pointer to internaly stored (in managment of class Vector3<T>)
	  * used for passing Vector3<T> values to gl*3[fd] functions.
	  */	
	 operator const T*() const { return (const T*) this; }

	 //-------------[ output operator ]------------------------
	 /**
	  * Output to stream operator
	  * @param lhs Left hand side argument of operator (commonly ostream instance). 
	  * @param rhs Right hand side argument of operator.
	  * @return Left hand side argument - the ostream object passed to operator.
	  */
	 friend std::ostream& operator<<(std::ostream& lhs, const Vector3<T> rhs) 
	 {
	    lhs << "[" << rhs.x << "," << rhs.y << "," << rhs.z  << "]";
	    return lhs;
	 }
	
   };


   /// Three dimensional Vector of floats
   typedef Vector3 <float> Vector3f;
   /// Three dimensional Vector of doubles
   typedef Vector3 <double> Vector3d;

   /**
    * Class for four dimensional vector.
    */
   template <class T>
   class Vector4{
      public:

	 union 
	 { 
	       /**
		* First element of vector, alias for R-coordinate.
		* For color notation.
		*/
	       T r
	       /**
		* First element of vector, alias for X-coordinate.
		*/; 
	       T x; 
	 };
	
	 union 
	 { 
	       /**
		* Second element of vector, alias for G-coordinate.
		* For color notation.
		*/
	       T g; 
	       /**
		* Second element of vector, alias for Y-coordinate.
		*/
	       T y; 
	 };
	
	 union 
	 {
	       /**
		* Third element of vector, alias for B-coordinate.
		* For color notation.
		*/
	       T b; 
	       /**
		* Third element of vector, alias for Z-coordinate.
		*/
	       T z; 
	 };
	
	 union 
	 {
	       /**
		* Fourth element of vector, alias for A-coordinate.
		* For color notation. This represnt aplha chanell
		*/
	       T a; 
	       /**
		* First element of vector, alias for W-coordinate.
		* @note For vectors (such as normals) should be set to 0.0
		* For vertices should be set to 1.0
		*/
	       T w; 
	 };
	
	 //----------------[ constructors ]--------------------------
	 /**
	  * Creates and sets to (0,0,0,0)
	  */
	 Vector4() : x(0),y(0),z(0),w(0)
	 { }


	 /**
	  * Creates and sets to (x,y,z,z)
	  * @param nx intial x-coordinate value (R)
	  * @param ny intial y-coordinate value (G)
	  * @param nz intial z-coordinate value (B)
	  * @param nw intial w-coordinate value (Aplha)
	  */
	 Vector4(T nx, T ny, T nz, T nw) : x(nx), y(ny), z(nz), w(nw)
	 { }

	 /**
	  * Copy constructor.
	  * @param src Source of data for new created Vector4 instance.
	  */
	 Vector4(const Vector4<T>& src)
	    : x(src.x), y(src.y), z(src.z), w(src.w)
	 {}

	 /**
	  * Copy casting constructor.
	  * @param src Source of data for new created Vector4 instance.
	  */
	 template <class FromT>
	 Vector4(const Vector4<FromT>& src)
	    : x(static_cast<T>(src.x)),
	      y(static_cast<T>(src.y)),
	      z(static_cast<T>(src.z)),
	      w(static_cast<T>(src.w))
	 {}


	 //----------------[ access operators ]-------------------
	 /**
	  * Copy operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T> operator=(const Vector4<T>& rhs) 
	 {
	    x = rhs.x;
	    y = rhs.y;
	    z = rhs.z;
	    w = rhs.w;
	    return * this;
	 }

	 /**
	  * Copy casting operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 template<class FromT>
	 Vector4<T> operator=(const Vector4<FromT>& rhs) 
	 {
	    x = static_cast<T>(rhs.x);
	    y = static_cast<T>(rhs.y);
	    z = static_cast<T>(rhs.z);
	    w = static_cast<T>(rhs.w);
	    return * this;
	 }


	 /**
	  * Array access operator
	  * @param n Array index
	  * @return For n = 0, reference to x coordinate, n = 1
	  * reference to y coordinate, n = 2 reference to z,  
	  * else reference to w coordinate.
	  */
	 T & operator[](int n) 
	 {
	    assert(n >= 0 && n <= 3);
	    if (0 == n) return x;
	    else if (1 == n) return y;
	    else if (2 == n) return z;
	    else return w;
	 }


	 //---------------[ vector aritmetic operator ]--------------
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T> operator+(const Vector4<T>& rhs) const 
	 {
	    return Vector4<T> (x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	 }

	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T> operator-(const Vector4<T>& rhs) const 
	 {
	    return Vector4<T> (x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	 }

	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T> operator*(const Vector4<T> rhs) const 
	 {
	    return Vector4<T> (x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
	 }

	 /**
	  * Division operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T> operator/(const Vector4<T>& rhs) const 
	 {
	    return Vector4<T> (x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
	 }

	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T>& operator+=(const Vector4<T>& rhs) 
	 {
	    x += rhs.x;
	    y += rhs.y;
	    z += rhs.z;
	    w += rhs.w;
	    return * this;
	 }

	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T>& operator-=(const Vector4<T>& rhs) 
	 {
	    x -= rhs.x;
	    y -= rhs.y;
	    z -= rhs.z;
	    w -= rhs.w;
	    return * this;
	 }

	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T>& operator*=(const Vector4<T>& rhs) 
	 {
	    x *= rhs.x;
	    y *= rhs.y;
	    z *= rhs.z;
	    w *= rhs.w;
	    return * this;
	 }

	 /**
	  * Division operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T>& operator/=(const Vector4<T>& rhs) 
	 {
	    x /= rhs.x;
	    y /= rhs.y;
	    z /= rhs.z;
	    w /= rhs.w;
	    return * this;
	 }

	 //--------------[ equiality operator ]------------------------
	 /**
	  * Equality test operator
	  * @param rhs Right hand side argument of binary operator.
	  * @note Test of equality is based of threshold EPSILON value. To be two
	  * values equal, must satisfy this condition | lws.x - rhs.y | < EPSILON,
	  * same for y-coordinate, z-coordinate, and w-coordinate.
	  */
	 bool operator==(const Vector4<T>& rhs) const 
	 {
	    return std::fabs(x - rhs.x) < EPSILON 
	       && std::fabs(y - rhs.y) < EPSILON 
	       && std::fabs(z - rhs.z) < EPSILON 
	       && std::fabs(w - rhs.w) < EPSILON;
	 }

	
	 /**
	  * Inequality test operator
	  * @param rhs Right hand side argument of binary operator.
	  * @return not (lhs == rhs) :-P
	  */
	 bool operator!=(const Vector4<T>& rhs) const { return ! (*this == rhs); }

	 //-------------[ unary operations ]--------------------------
	 /**
	  * Unary negate operator
	  * @return negated vector
	  */
	 Vector4<T> operator-() const
	 {
	    return Vector4<T>(-x, -y, -z, -w);
	 }

	 //--------------[ scalar vector operator ]--------------------
	
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T> operator+(T rhs) const 
	 {
	    return Vector4<T> (x + rhs, y + rhs, z + rhs, w + rhs);
	 }

	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T> operator-(T rhs) const 
	 {
	    return Vector4<T> (x - rhs, y - rhs, z - rhs, w - rhs);
	 }

	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T> operator*(T rhs) const 
	 {
	    return Vector4<T> (x * rhs, y * rhs, z * rhs, w * rhs);
	 }

	 /**
	  * Division operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T> operator/(T rhs) const 
	 {
	    return Vector4<T> (x / rhs, y / rhs, z / rhs, w / rhs);
	 }

	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T>& operator+=(T rhs) 
	 {
	    x += rhs;
	    y += rhs;
	    z += rhs;
	    w += rhs;
	    return * this;
	 }

	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T>& operator-=(T rhs) 
	 {
	    x -= rhs;
	    y -= rhs;
	    z -= rhs;
	    w -= rhs;
	    return * this;
	 }

	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T>& operator*=(T rhs) 
	 {
	    x *= rhs;
	    y *= rhs;
	    z *= rhs;
	    w *= rhs;
	    return * this;
	 }

	 /**
	  * Division operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T>& operator/=(T rhs) 
	 {
	    x /= rhs;
	    y /= rhs;
	    z /= rhs;
	    w /= rhs;
	    return * this;
	 }

	 //-------------[ size operations ]---------------------------
	 /**
	  * Get lenght of vector.
	  * @return lenght of vector
	  */
	 T length() const
	 {
	    return (T)std::sqrt(x * x + y * y + z * z + w * w);
	 }

	 /**
	  * Normalize vector
	  */
	 void normalize() 
	 {
	    T s = length();
	    x /= s;
	    y /= s;
	    z /= s;
	    w /= s;
	 }
	
	 /**
	  * Return square of length.
	  * @return lenght ^ 2
	  * @note This method is faster then length(). For comparsion
	  * of length of two vector can be used just this value, instead
	  * of computionaly more expensive length() method.
	  */
	 T lengthSq() const 
	 {
	    return x * x + y * y + z * z + w * w;
	 }
	
	 //--------------[ misc. operations ]-----------------------
	 /**
	  * Linear interpolation of two vectors
	  * @param fact Factor of interpolation. For translation from positon
	  * of this vector to vector r, values of factor goes from 0.0 to 1.0.
	  * @param r Second Vector for interpolation 
	  * @note Hovewer values of fact parameter are reasonable only in interval
	  * [0.0 , 1.0], you can pass also values outside of this interval and you
	  * can get result (extrapolation?)
	  */
	 Vector4<T> lerp(T fact, const Vector4<T>& r) const
	 {
	    return (*this) + (r - (*this)) * fact;	
	 }
	
    
	

	 //-------------[ conversion ]-----------------------------
	
	 /**
	  * Conversion to pointer operator
	  * @return Pointer to internaly stored (in managment of class Vector4<T>)
	  * used for passing Vector4<T> values to gl*4[fd] functions.
	  */
	 operator T*(){ return (T*) this; }
	
	
	 /**
	  * Conversion to pointer operator
	  * @return Constant Pointer to internaly stored (in managment of class Vector4<T>)
	  * used for passing Vector4<T> values to gl*4[fd] functions.
	  */
	 operator const T*() const { return (const T*) this; }

	 //-------------[ output operator ]------------------------
	 /**
	  * Output to stream operator
	  * @param lhs Left hand side argument of operator (commonly ostream instance). 
	  * @param rhs Right hand side argument of operator.
	  * @return Left hand side argument - the ostream object passed to operator.
	  */
	 friend std::ostream& operator<<(std::ostream& lhs, const Vector4<T>& rhs) 
	 {
	    lhs << "[" << rhs.x << "," << rhs.y << "," << rhs.z << "," << rhs.w << "]";
	    return lhs;
	 }
   };

   /// Three dimensional Vector of floats
   typedef Vector4<float> Vector4f;
   /// Three dimensional Vector of doubles
   typedef Vector4<double> Vector4d;





   /**
    * Class for matrix 3x3. 
    * @note Data stored in this matrix are in column major order.
    */
   template <class T>
   class Matrix3 
   {
      public:
	 /// Data stored in column major order
	 T data[9];
	
	 //--------------------------[ constructors ]-------------------------------
	 /**
	  * Creates identity matrix
	  */
	 Matrix3() 
	 {
	    for (int i = 0; i < 9; i++)
	       data[i] = (i % 4) ? 0 : 1;
	 }

	 /**
	  * Copy matrix values from array (these data must be in column
	  * major order!)
	  */
	 Matrix3(const T * dt)
	 {
	    std::memcpy(data, dt, sizeof(T) * 9); 
	 }

	 /**
	  * Copy constructor.
	  * @param src Data source for new created instance of Matrix3
	  */
	 Matrix3(const Matrix3<T>& src)
	 {
	    std::memcpy(data, src.data, sizeof(T) * 9);
	 }

	 /**
	  * Copy casting constructor.
	  * @param src Data source for new created instance of Matrix3
	  */
	 template<class FromT>
	 Matrix3(const Matrix3<FromT>& src)
	 {
	    for (int i = 0; i < 9; i++)
	    {
	       data[i] = static_cast<T>(src.data[i]);
	    }
	 }
	
	 /**
	  * Resets matrix to be identity matrix
	  */
	 void identity()
	 {
	    for (int i = 0; i < 9; i++)
	       data[i] = (i % 4) ? 0 : 1;
	 }
	
	

	 /**
	  * Creates rotation matrix by rotation around axis.
	  * @param a Angle (in radians) of rotation around axis X.
	  * @param b Angle (in radians) of rotation around axis Y.
	  * @param c Angle (in radians) of rotation around axis Z.
	  */
	 static Matrix3<T> createRotationAroundAxis(T a, T b, T c)
	 {
	    Matrix3<T> ma, mb, mc;
	    float ac = cos(a);
	    float as = sin(a);
	    float bc = cos(b);
	    float bs = sin(b);
	    float cc = cos(c);
	    float cs = sin(c);

	    ma.at(1,1) = ac;
	    ma.at(2,1) = as;
	    ma.at(1,2) = -as;
	    ma.at(2,2) = ac;

	    mb.at(0,0) = bc;
	    mb.at(2,0) = -bs;
	    mb.at(0,2) = bs;
	    mb.at(2,2) = bc;

	    mc.at(0,0) = cc;
	    mc.at(1,0) = cs;
	    mc.at(0,1) = -cs;
	    mc.at(1,1) = cc;

	    Matrix3<T> ret = ma * mb * mc;
	    return ret;
	 }

	 /**
	  * Creates roation matrix from ODE Matrix.
	  */
	 template <class It>
	 static Matrix3<T> fromOde(const It* mat)
	 {
	    Matrix3<T> ret;
	    for (int i = 0; i < 3; i++)
	    {
	       for (int j = 0; j < 3; j++)
	       {
		  ret.at(i,j) = static_cast<T>(mat[j * 4 + i]);
	       }
	    }
	    return ret;
	 }


	 //---------------------[ equiality operators ]------------------------------
	 /**
	  * Equality test operator
	  * @param rhs Right hand side argument of binary operator.
	  * @note Test of equality is based of threshold EPSILON value. To be two
	  * values equal, must satisfy this condition all elements of matrix 
	  * | lws[i] - rhs[i] | < EPSILON,
	  * same for y-coordinate, z-coordinate, and w-coordinate.
	  */	
	 bool operator==(const Matrix3<T>& rhs) const
	 {
	    for (int i = 0; i < 9; i++)
	    {
	       if (std::fabs(data[i] - rhs.data[i]) >= EPSILON)
		  return false;
	    }
	    return true;
	 }
	
	
	 /**
	  * Inequality test operator
	  * @param rhs Right hand side argument of binary operator.
	  * @return not (lhs == rhs) :-P
	  */
	 bool operator!=(const Matrix3<T>& rhs) const
	 {
	    return !(*this == rhs);
	 }
	

	 //---------------------[ access operators ]---------------------------------
	 /**
	  * Get reference to element at postion (x,y).
	  * @param x Number of column (0..2)
	  * @param y Number of row (0..2)
	  */
	 T& at(int x, int y) 
	 {
	    assert(x >= 0 && x < 3);
	    assert(y >= 0 && y < 3);
	    return data[x * 3 + y];
	 }

	 /**
	  * Get constant reference to element at postion (x,y).
	  * @param x Number of column (0..2)
	  * @param y Number of row (0..2)
	  */
	 const T& at(int x, int y) const 
	 {
	    assert(x >= 0 && x < 3);
	    assert(y >= 0 && y < 3);
	    return data[x * 3 + y];
	 }

	 /**
	  * Copy operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix3<T>& operator=(const Matrix3<T>& rhs) 
	 {
	    std::memcpy(data, rhs.data, sizeof(T) * 9);
	    return * this;
	 }

	 /**
	  * Copy casting operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 template<class FromT>
	 Matrix3<T>& operator=(const Matrix3<FromT>& rhs) 
	 {
	    for (int i = 0; i < 9; i++)
	    {
	       data[i] = static_cast<T>(rhs.data[i]);
	    }
	    return * this;
	 }

	 /**
	  * Copy operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix3<T>& operator=(const T* rhs) 
	 {
	    std::memcpy(data, rhs, sizeof(T) * 9);
	    return * this;
	 }


	 /*Matrix3<T> & operator=(const double* m)
	   {
	   for (int i = 0; i < 9; i++) data[i] = (T)m[i];
	   return * this;
	   }*/
	
	 //--------------------[ matrix with matrix operations ]---------------------
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix3<T> operator+(const Matrix3<T>& rhs) const
	 {
	    Matrix3<T> ret;
	    for (int i = 0; i < 9; i++)
	       ret.data[i] = data[i] + rhs.data[i];
	    return ret;
	 }
	
	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix3<T> operator-(const Matrix3<T>& rhs) const
	 {
	    Matrix3<T> ret;
	    for (int i = 0; i < 9; i++)
	       ret.data[i] = data[i] - rhs.data[i];
	    return ret;
	 }
	
	 //--------------------[ matrix with scalar operations ]---------------------
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix3<T> operator+(T rhs) const
	 {
	    Matrix3<T> ret;
	    for (int i = 0; i < 9; i++)
	       ret.data[i] = data[i] + rhs;
	    return ret;
	 }
	
	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix3<T> operator-(T rhs) const
	 {
	    Matrix3<T> ret;
	    for (int i = 0; i < 9; i++)
	       ret.data[i] = data[i] - rhs;
	    return ret;
	 }
	
	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix3<T> operator*(T rhs) const
	 {
	    Matrix3<T> ret;
	    for (int i = 0; i < 9; i++)
	       ret.data[i] = data[i] * rhs;
	    return ret;
	 }
	
	 /**
	  * Division operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix3<T> operator/(T rhs) const
	 {
	    Matrix3<T> ret;
	    for (int i = 0; i < 9; i++)
	       ret.data[i] = data[i] / rhs;
	    return ret;
	 }
	

	 //--------------------[ multiply operators ]--------------------------------
	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T> operator*(const Vector3<T>& rhs) const
	 {
	    return Vector3<T>(
	       data[0] * rhs.x + data[3] * rhs.y + data[6] * rhs.z,
	       data[1] * rhs.x + data[4] * rhs.y + data[7] * rhs.z,
	       data[2] * rhs.x + data[5] * rhs.y + data[8] * rhs.z
	       );
	 }

	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix3<T> operator*(Matrix3<T> rhs) const 
	 {
	    static Matrix3<T> w;
	    for (int i = 0; i < 3; i++) 
	    {
	       for (int j = 0; j < 3; j++) 
	       {
		  T n = 0;
		  for (int k = 0; k < 3; k++) n += rhs.at(i, k) * at(k, j);
		  w.at(i, j) = n;
	       }
	    }
	    return w;
		
	 }
	
	
	 //---------------------------[ misc operations ]----------------------------
	 /**
	  * Transpose matrix.
	  */
	 Matrix3<T> transpose()
	 {
	    Matrix3<T> ret;
	    for (int i = 0; i < 3; i++)
	    {
	       for (int j = 0; j < 3; j++)
	       {
		  ret.at(i,j) = at(j,i);
	       }
	    }
	    return ret;
	 }
	
	 /**
	  * Linear interpolation of two vectors
	  * @param fact Factor of interpolation. For translation from positon
	  * of this matrix (lhs) to matrix rhs, values of factor goes from 0.0 to 1.0.
	  * @param rhs Second Matrix for interpolation 
	  * @note Hovewer values of fact parameter are reasonable only in interval
	  * [0.0 , 1.0], you can pass also values outside of this interval and you
	  * can get result (extrapolation?)
	  */
	 Matrix3<T> lerp(T fact, const Matrix3<T>& rhs) const
	 {
	    Matrix3<T> ret = (*this) + (rhs - (*this)) * fact;
	    return ret;		
	 }

    
	 //-------------[ conversion ]-----------------------------
	
	 /**
	  * Conversion to pointer operator
	  * @return Pointer to internaly stored (in managment of class Matrix3<T>)
	  * used for passing Matrix3<T> values to gl*[fd]v functions.
	  */
	 operator T*(){ return (T*) data; }
	
	 /**
	  * Conversion to pointer operator
	  * @return Constant Pointer to internaly stored (in managment of class Matrix3<T>)
	  * used for passing Matrix3<T> values to gl*[fd]v functions.
	  */
	 operator const T*() const { return (const T*) data; }

	 //----------[ output operator ]----------------------------
	 /**
	  * Output to stream operator
	  * @param lhs Left hand side argument of operator (commonly ostream instance). 
	  * @param rhs Right hand side argument of operator.
	  * @return Left hand side argument - the ostream object passed to operator.
	  */
	 friend std::ostream& operator << (std::ostream& lhs, const Matrix3<T>& rhs) 
	 {
	    for (int i = 0; i < 3; i++)
	    {
	       lhs << "|\t";
	       for (int j = 0; j < 3; j++)
	       {
		  lhs << rhs.at(j,i) << "\t";
	       }
	       lhs << "|" << std::endl;
	    }
	    return lhs;
	 }
    
   };

   /// Matrix 3x3 of floats
   typedef Matrix3<float> Matrix3f;
   /// Matrix 3x3 of doubles
   typedef Matrix3<double> Matrix3d;


   /**
    * Class for matrix 4x4 
    * @note Data stored in this matrix are in column major order.
    */
   template <class T>
   class Matrix4 
   {
      public:
	 /// Data stored in column major order
	 T data[16];
	
	 //--------------------------[ constructors ]-------------------------------
	 /**
	  *Creates identity matrix
	  */
	 Matrix4() 
	 {
	    for (int i = 0; i < 16; i++)
	       data[i] = (i % 5) ? 0 : 1;
	 }

	 /**
	  * Copy matrix values from array (these data must be in column
	  * major order!)
	  */
	 Matrix4(const T * dt) 
	 {
	    std::memcpy(data, dt, sizeof(T) * 16); 
	 }

	 /**
	  * Copy constructor.
	  * @param src Data source for new created instance of Matrix4.
	  */
	 Matrix4(const Matrix4<T>& src)
	 {
	    std::memcpy(data, src.data, sizeof(T) * 16);
	 }
	
	 /**
	  * Copy casting constructor.
	  * @param src Data source for new created instance of Matrix4.
	  */
	 template <class FromT>
	 Matrix4(const Matrix4<FromT>& src)
	 {
	    for (int i = 0; i < 16; i++)
	    {
	       data[i] = static_cast<T>(src.data[i]);
	    }
	 }

	 /**
	  * Resets matrix to be identity matrix
	  */
	 void identity()
	 {
	    for (int i = 0; i < 16; i++)
	       data[i] = (i % 5) ? 0 : 1;
	 }
	
	

	 /**
	  * Creates rotation matrix by rotation around axis.
	  * @param a Angle (in radians) of rotation around axis X.
	  * @param b Angle (in radians) of rotation around axis Y.
	  * @param c Angle (in radians) of rotation around axis Z.
	  */
	 static Matrix4<T> createRotationAroundAxis(T a, T b, T c)
	 {
	    Matrix4<T> ma, mb, mc;
	    float ac = cos(a);
	    float as = sin(a);
	    float bc = cos(b);
	    float bs = sin(b);
	    float cc = cos(c);
	    float cs = sin(c);

	    ma.at(1,1) = ac;
	    ma.at(2,1) = as;
	    ma.at(1,2) = -as;
	    ma.at(2,2) = ac;

	    mb.at(0,0) = bc;
	    mb.at(2,0) = -bs;
	    mb.at(0,2) = bs;
	    mb.at(2,2) = bc;

	    mc.at(0,0) = cc;
	    mc.at(1,0) = cs;
	    mc.at(0,1) = -cs;
	    mc.at(1,1) = cc;

	    /*std::cout << "RotVec = " << a << "," << b << "," << c << std::endl;
	      std::cout << "Rx = " << std::endl << ma;
	      std::cout << "Ry = " << std::endl << mb;
	      std::cout << "Rz = " << std::endl << mc;*/
		
	    Matrix4<T> ret = ma * mb * mc;
	    //std::cout << "Result = " << std::endl << ma * (mb * mc);
		
	    return ret;
	 }

	 /// Creates translation matrix
	 /**
	  * Creates translation matrix.
	  * @param x X-direction translation
	  * @param y Y-direction translation
	  * @param z Z-direction translation
	  * @param w for W-coordinate translation (impictily set to 1)
	  */
	 static Matrix4<T> createTranslation(T x, T y, T z, T w = 1)
	 {
	    Matrix4 ret;
	    ret.at(3,0) = x;
	    ret.at(3,1) = y;
	    ret.at(3,2) = z;
	    ret.at(3,3) = w;
		
	    return ret;
	 }


	 //---------------------[ equiality operators ]------------------------------	
	 /**
	  * Equality test operator
	  * @param rhs Right hand side argument of binary operator.
	  * @note Test of equality is based of threshold EPSILON value. To be two
	  * values equal, must satisfy this condition all elements of matrix 
	  * | lws[i] - rhs[i] | < EPSILON,
	  * same for y-coordinate, z-coordinate, and w-coordinate.
	  */	
	 bool operator==(const Matrix4<T>& rhs) const
	 {
	    for (int i = 0; i < 16; i++)
	    {
	       if (std::fabs(data[i] - rhs.data[i]) >= EPSILON)
		  return false;
	    }
	    return true;
	 }
	
	 /**
	  * Inequality test operator
	  * @param rhs Right hand side argument of binary operator.
	  * @return not (lhs == rhs) :-P
	  */
	 bool operator!=(const Matrix4<T>& rhs) const
	 {
	    return !(*this == rhs);
	 }
	

	 //---------------------[ access operators ]---------------------------------
	 /**
	  * Get reference to element at postion (x,y).
	  * @param x Number of column (0..3)
	  * @param y Number of row (0..3)
	  */
	 T& at(int x, int y) 
	 {
	    assert(x >= 0 && x < 4);
	    assert(y >= 0 && y < 4);
	    return data[x * 4 + y];
	 }

	 /**
	  * Get constant reference to element at postion (x,y).
	  * @param x Number of column (0..3)
	  * @param y Number of row (0..3)
	  */
	 const T& at(int x, int y) const 
	 {
	    assert(x >= 0 && x < 4);
	    assert(y >= 0 && y < 4);
	    return data[x * 4 + y];
	 }


	 /**
	  * Sets translation part of matrix.
	  *
	  * @param v Vector of translation to be set.
	  */
	 void setTranslation(const Vector3<T>& v)
	 {
	    at(3,0) = v.x;
	    at(3,1) = v.y;
	    at(3,2) = v.z;
	    at(3,3) = 1;
	 }

	 Vector3<T> getTranslation()
	 { return Vector3<T>(at(3,0),at(3,1),at(3,2)); }

	 /**
	  * Sets roation part (matrix 3x3) of matrix.
	  *
	  * @param m Rotation part of matrix
	  */
	 void setRotation(const Matrix3<T>& m)
	 {
	    for (int i = 0; i < 3; i++)
	    {
	       for (int j = 0; j < 3; j++)
	       {
		  at(i,j) = m.at(i,j);
	       }
	    }
	 }
				 

	 /**
	  * Copy operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix4<T>& operator=(const Matrix4<T>& rhs) 
	 {
	    std::memcpy(data, rhs.data, sizeof(T) * 16);
	    return * this;
	 }

	 /**
	  * Copy casting operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 template <class FromT>
	 Matrix4<T>& operator=(const Matrix4<FromT>& rhs) 
	 {
	    for (int i = 0; i < 16; i++)
	    {
	       data[i] = static_cast<T>(rhs.data[i]);
	    }
	    return * this;
	 }

	 /**
	  * Copy operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix4<T>& operator=(const T* rhs) 
	 {
	    std::memcpy(data, rhs, sizeof(T) * 16);
	    return * this;
	 }

	 /*Matrix4<T> & operator=(const double* m)
	   {
	   for (int i = 0; i < 16; i++) data[i] = (T)m[i];
	   return * this;
	   }*/
	
	 //--------------------[ matrix with matrix operations ]---------------------
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix4<T> operator+(const Matrix4<T>& rhs) const
	 {
	    Matrix4<T> ret;
	    for (int i = 0; i < 16; i++)
	       ret.data[i] = data[i] + rhs.data[i];
	    return ret;
	 }
	
	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix4<T> operator-(const Matrix4<T>& rhs) const
	 {
	    Matrix4<T> ret;
	    for (int i = 0; i < 16; i++)
	       ret.data[i] = data[i] - rhs.data[i];
	    return ret;
	 }
	
	 //--------------------[ matrix with scalar operations ]---------------------
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix4<T> operator+(T rhs) const
	 {
	    Matrix4<T> ret;
	    for (int i = 0; i < 16; i++)
	       ret.data[i] = data[i] + rhs;
	    return ret;
	 }
	
	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix4<T> operator-(T rhs) const
	 {
	    Matrix4<T> ret;
	    for (int i = 0; i < 16; i++)
	       ret.data[i] = data[i] - rhs;
	    return ret;
	 }
	
	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix4<T> operator*(T rhs) const
	 {
	    Matrix4<T> ret;
	    for (int i = 0; i < 16; i++)
	       ret.data[i] = data[i] * rhs;
	    return ret;
	 }
	
	 /**
	  * Division operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix4<T> operator/(T rhs) const
	 {
	    Matrix4<T> ret;
	    for (int i = 0; i < 16; i++)
	       ret.data[i] = data[i] / rhs;
	    return ret;
	 }
	

	 //--------------------[ multiply operators ]--------------------------------
	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector4<T> operator*(const Vector4<T>& rhs) const 
	 {
	    return Vector4<T>(
	       data[0] * rhs.x + data[4] * rhs.y + data[8]  * rhs.z + data[12] * rhs.w,
	       data[1] * rhs.x + data[5] * rhs.y + data[9]  * rhs.z + data[13] * rhs.w,
	       data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z + data[14] * rhs.w,
	       data[3] * rhs.x + data[7] * rhs.y + data[11] * rhs.z + data[15] * rhs.w
	       );

	 }
	
	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Vector3<T> operator*(const Vector3<T>& rhs) const
	 {
	    return Vector3<T>(
	       data[0] * rhs.x + data[4] * rhs.y + data[8] * rhs.z,
	       data[1] * rhs.x + data[5] * rhs.y + data[9] * rhs.z,
	       data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z
	       );
	 }

	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Matrix4<T> operator*(Matrix4<T> rhs) const 
	 {
	    static Matrix4<T> w;
	    for (int i = 0; i < 4; i++) 
	    {
	       for (int j = 0; j < 4; j++) 
	       {
		  T n = 0;
		  for (int k = 0; k < 4; k++) n += rhs.at(i, k) * at(k, j);
		  w.at(i, j) = n;
	       }
	    }
	    return w;
		
	 }
	
	
	 //---------------------------[ misc operations ]----------------------------
 
	 /**
	  * Computes determinant of matrix
	  * @return Determinant of matrix
	  * @note This function does 3 * 4 * 6 mul, 3 * 6 add.
	  */
	 T det()
	 {

	    return
	       + at(3,0) * at(2,1) * at(1,2) * at(0,3)
	       - at(2,0) * at(3,1) * at(1,2) * at(0,3)
	       - at(3,0) * at(1,1) * at(2,2) * at(0,3)
	       + at(1,0) * at(3,1) * at(2,2) * at(0,3)

	       + at(2,0) * at(1,1) * at(3,2) * at(0,3)
	       - at(1,0) * at(2,1) * at(3,2) * at(0,3)
	       - at(3,0) * at(2,1) * at(0,2) * at(1,3)
	       + at(2,0) * at(3,1) * at(0,2) * at(1,3)

	       + at(3,0) * at(0,1) * at(2,2) * at(1,3)
	       - at(0,0) * at(3,1) * at(2,2) * at(1,3)
	       - at(2,0) * at(0,1) * at(3,2) * at(1,3)
	       + at(0,0) * at(2,1) * at(3,2) * at(1,3)

	       + at(3,0) * at(1,1) * at(0,2) * at(2,3)
	       - at(1,0) * at(3,1) * at(0,2) * at(2,3)
	       - at(3,0) * at(0,1) * at(1,2) * at(2,3)
	       + at(0,0) * at(3,1) * at(1,2) * at(2,3)

	       + at(1,0) * at(0,1) * at(3,2) * at(2,3)
	       - at(0,0) * at(1,1) * at(3,2) * at(2,3)
	       - at(2,0) * at(1,1) * at(0,2) * at(3,3)
	       + at(1,0) * at(2,1) * at(0,2) * at(3,3)

	       + at(2,0) * at(0,1) * at(1,2) * at(3,3)
	       - at(0,0) * at(2,1) * at(1,2) * at(3,3)
	       - at(1,0) * at(0,1) * at(2,2) * at(3,3)
	       + at(0,0) * at(1,1) * at(2,2) * at(3,3);


	 }


	 /**
	  * Computes inverse matrix
	  * @return Inverse matrix of this matrix.
	  * @note This is a little bit time consuming operation
	  * (16 * 6 * 3 mul, 16 * 5 add + det() + mul() functions)
	  */
	 Matrix4<T> inverse()
	 {
	    Matrix4<T> ret;

	    ret.at(0,0) =  
	       + at(2,1) * at(3,2) * at(1,3) 
	       - at(3,1) * at(2,2) * at(1,3) 
	       + at(3,1) * at(1,2) * at(2,3) 
	       - at(1,1) * at(3,2) * at(2,3) 
	       - at(2,1) * at(1,2) * at(3,3) 
	       + at(1,1) * at(2,2) * at(3,3);

	    ret.at(1,0) =
	       + at(3,0) * at(2,2) * at(1,3) 
	       - at(2,0) * at(3,2) * at(1,3) 
	       - at(3,0) * at(1,2) * at(2,3) 
	       + at(1,0) * at(3,2) * at(2,3) 
	       + at(2,0) * at(1,2) * at(3,3) 
	       - at(1,0) * at(2,2) * at(3,3);

	    ret.at(2,0) = 
	       + at(2,0) * at(3,1) * at(1,3) 
	       - at(3,0) * at(2,1) * at(1,3) 
	       + at(3,0) * at(1,1) * at(2,3) 
	       - at(1,0) * at(3,1) * at(2,3) 
	       - at(2,0) * at(1,1) * at(3,3) 
	       + at(1,0) * at(2,1) * at(3,3);

	    ret.at(3,0) = 
	       + at(3,0) * at(2,1) * at(1,2) 
	       - at(2,0) * at(3,1) * at(1,2) 
	       - at(3,0) * at(1,1) * at(2,2) 
	       + at(1,0) * at(3,1) * at(2,2) 
	       + at(2,0) * at(1,1) * at(3,2) 
	       - at(1,0) * at(2,1) * at(3,2);

	    ret.at(0,1) = 
	       + at(3,1) * at(2,2) * at(0,3) 
	       - at(2,1) * at(3,2) * at(0,3) 
	       - at(3,1) * at(0,2) * at(2,3) 
	       + at(0,1) * at(3,2) * at(2,3) 
	       + at(2,1) * at(0,2) * at(3,3) 
	       - at(0,1) * at(2,2) * at(3,3);

	    ret.at(1,1) = 
	       + at(2,0) * at(3,2) * at(0,3) 
	       - at(3,0) * at(2,2) * at(0,3) 
	       + at(3,0) * at(0,2) * at(2,3) 
	       - at(0,0) * at(3,2) * at(2,3) 
	       - at(2,0) * at(0,2) * at(3,3) 
	       + at(0,0) * at(2,2) * at(3,3);

	    ret.at(2,1) =
	       + at(3,0) * at(2,1) * at(0,3) 
	       - at(2,0) * at(3,1) * at(0,3) 
	       - at(3,0) * at(0,1) * at(2,3) 
	       + at(0,0) * at(3,1) * at(2,3) 
	       + at(2,0) * at(0,1) * at(3,3) 
	       - at(0,0) * at(2,1) * at(3,3);

	    ret.at(3,1) = 
	       + at(2,0) * at(3,1) * at(0,2) 
	       - at(3,0) * at(2,1) * at(0,2) 
	       + at(3,0) * at(0,1) * at(2,2) 
	       - at(0,0) * at(3,1) * at(2,2) 
	       - at(2,0) * at(0,1) * at(3,2) 
	       + at(0,0) * at(2,1) * at(3,2);

	    ret.at(0,2) =  
	       + at(1,1) * at(3,2) * at(0,3) 
	       - at(3,1) * at(1,2) * at(0,3) 
	       + at(3,1) * at(0,2) * at(1,3) 
	       - at(0,1) * at(3,2) * at(1,3) 
	       - at(1,1) * at(0,2) * at(3,3) 
	       + at(0,1) * at(1,2) * at(3,3);

	    ret.at(1,2) =
	       + at(3,0) * at(1,2) * at(0,3) 
	       - at(1,0) * at(3,2) * at(0,3) 
	       - at(3,0) * at(0,2) * at(1,3) 
	       + at(0,0) * at(3,2) * at(1,3) 
	       + at(1,0) * at(0,2) * at(3,3) 
	       - at(0,0) * at(1,2) * at(3,3);

	    ret.at(2,2) = 
	       + at(1,0) * at(3,1) * at(0,3) 
	       - at(3,0) * at(1,1) * at(0,3) 
	       + at(3,0) * at(0,1) * at(1,3) 
	       - at(0,0) * at(3,1) * at(1,3) 
	       - at(1,0) * at(0,1) * at(3,3) 
	       + at(0,0) * at(1,1) * at(3,3);

	    ret.at(3,2) = 
	       + at(3,0) * at(1,1) * at(0,2) 
	       - at(1,0) * at(3,1) * at(0,2) 
	       - at(3,0) * at(0,1) * at(1,2) 
	       + at(0,0) * at(3,1) * at(1,2) 
	       + at(1,0) * at(0,1) * at(3,2) 
	       - at(0,0) * at(1,1) * at(3,2);

	    ret.at(0,3) =
	       + at(2,1) * at(1,2) * at(0,3) 
	       - at(1,1) * at(2,2) * at(0,3) 
	       - at(2,1) * at(0,2) * at(1,3) 
	       + at(0,1) * at(2,2) * at(1,3) 
	       + at(1,1) * at(0,2) * at(2,3) 
	       - at(0,1) * at(1,2) * at(2,3);

	    ret.at(1,3) = 
	       + at(1,0) * at(2,2) * at(0,3) 
	       - at(2,0) * at(1,2) * at(0,3) 
	       + at(2,0) * at(0,2) * at(1,3) 
	       - at(0,0) * at(2,2) * at(1,3) 
	       - at(1,0) * at(0,2) * at(2,3) 
	       + at(0,0) * at(1,2) * at(2,3);

	    ret.at(2,3) =  
	       + at(2,0) * at(1,1) * at(0,3) 
	       - at(1,0) * at(2,1) * at(0,3) 
	       - at(2,0) * at(0,1) * at(1,3) 
	       + at(0,0) * at(2,1) * at(1,3) 
	       + at(1,0) * at(0,1) * at(2,3) 
	       - at(0,0) * at(1,1) * at(2,3);

	    ret.at(3,3) = 
	       + at(1,0) * at(2,1) * at(0,2) 
	       - at(2,0) * at(1,1) * at(0,2) 
	       + at(2,0) * at(0,1) * at(1,2) 
	       - at(0,0) * at(2,1) * at(1,2) 
	       - at(1,0) * at(0,1) * at(2,2) 
	       + at(0,0) * at(1,1) * at(2,2);
  
	    return ret * det();
	 }



	 /**
	  * Transpose matrix.
	  */
	 Matrix4<T> transpose()
	 {
	    Matrix4<T> ret;
	    for (int i = 0; i < 4; i++)
	    {
	       for (int j = 0; j < 4; j++)
	       {
		  ret.at(i,j) = at(j,i);
	       }
	    }
	    return ret;
	 }
	
	 /**
	  * Linear interpolation of two vectors
	  * @param fact Factor of interpolation. For translation from positon
	  * of this matrix (lhs) to matrix rhs, values of factor goes from 0.0 to 1.0.
	  * @param rhs Second Matrix for interpolation 
	  * @note Hovewer values of fact parameter are reasonable only in interval
	  * [0.0 , 1.0], you can pass also values outside of this interval and you
	  * can get result (extrapolation?)
	  */
	 Matrix4<T> lerp(T fact, const Matrix4<T>& rhs) const
	 {
	    Matrix4<T> ret = (*this) + (rhs - (*this)) * fact;
	    return ret;		
	 }

    
	 //-------------[ conversion ]-----------------------------
	 /**
	  * Conversion to pointer operator
	  * @return Pointer to internaly stored (in managment of class Matrix4<T>)
	  * used for passing Matrix4<T> values to gl*[fd]v functions.
	  */
	 operator T*(){ return (T*) data; }
	
	 /**
	  * Conversion to pointer operator
	  * @return Constant Pointer to internaly stored (in managment of class Matrix4<T>)
	  * used for passing Matrix4<T> values to gl*[fd]v functions.
	  */
	 operator const T*() const { return (const T*) data; }

	 //----------[ output operator ]----------------------------
	 /**
	  * Output to stream operator
	  * @param lhs Left hand side argument of operator (commonly ostream instance). 
	  * @param rhs Right hand side argument of operator.
	  * @return Left hand side argument - the ostream object passed to operator.
	  */
	 friend std::ostream& operator << (std::ostream& lhs, const Matrix4<T>& rhs) 
	 {
	    for (int i = 0; i < 4; i++)
	    {
	       lhs << "|\t";
	       for (int j = 0; j < 4; j++)
	       {
		  lhs << rhs.at(j,i) << "\t";
	       }
	       lhs << "|" << std::endl;
	    }
	    return lhs;
	 }
    
   };


   /// Matrix 3x3 of floats
   typedef Matrix4<float> Matrix4f;
   /// Matrix 3x3 of doubles
   typedef Matrix4<double> Matrix4d;


   /**
    * Quaternion class implementing some quaternion algebra operations.
    * Quaternion is kind of complex number it consists of its real part (w)
    * and its complex part v. This complex part has three elements, so we
    * can express it as xi + yj + zk . Note that coordinates of (x,y,z) are
    * hold inside v field.
    */
   template <class T>
   class Quaternion
   {
      public:
	 /**
	  * Real part of quaternion.
	  */
	 T w;
	 /**
	  * Complex part of quaternion.
	  */
	 Vector3<T> v;
	
	 /**
	  * Quaternion constructor, sets quaternion to (0 + 0i + 0j + 0k).
	  */
	 Quaternion(): w(0), v(0,0,0){}
	
	 /**
	  * Copy constructor.
	  */
	 Quaternion(const Quaternion<T>& q): w(q.w), v(q.v){	}

	 /**
	  * Copy casting constructor.
	  */
	 template <class FromT>
	 Quaternion(const Quaternion<FromT>& q)
	    : w(static_cast<T>(q.w)), 
	      v(q.v){	}

	
	 /**
	  * Creates quaternion object from real part w_ and complex part v_.
	  * @param w_ Real part of quaternion.
	  * @param v_ Complex part of quaternion (xi + yj + zk).
	  */
	 Quaternion(T w_, const Vector3<T>& v_): w(w_), v(v_){}
	
	 /**
	  * Creates quaternion object from value (w_ + xi + yj + zk).
	  * @param w_ Real part of quaternion.
	  * @param x Complex cooeficinet for i complex constant.
	  * @param y Complex cooeficinet for j complex constant.
	  * @param z Complex cooeficinet for k complex constant.
	  */
	 Quaternion(T w_, T x, T y, T z): w(w_), v(x,y,z){}
	
	 /**
	  * Copy operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Quaternion<T>& operator= (const Quaternion<T>& rhs)
	 {
	    v = rhs.v;
	    w = rhs.w;
	    return *this;
	 }

	 /**
	  * Copy convert operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 template<class FromT>
	 Quaternion<T>& operator= (const Quaternion<FromT>& rhs)
	 {
	    v = rhs.v;
	    w = static_cast<T>(rhs.w);
	    return *this;
	 }
	
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Quaternion<T> operator+ (const Quaternion<T>& rhs) const
	 {
	    const Quaternion<T>& lhs = *this;
	    return Quaternion<T>(lhs.w + rhs.w, lhs.v + rhs.v);  
	 }
	
	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Quaternion<T> operator* (const Quaternion<T>& rhs) const
	 {
	    const Quaternion<T>& lhs = *this;
	    return Quaternion<T>(
	       lhs.w * rhs.w 	- lhs.v.x * rhs.v.x 	- lhs.v.y * rhs.v.y 	- lhs.v.z * rhs.v.z,
	       lhs.w * rhs.v.x 	+ lhs.v.x * rhs.w 	+ lhs.v.y * rhs.v.z	- lhs.v.z * rhs.v.y,
	       lhs.w * rhs.v.y 	- lhs.v.x * rhs.v.z	+ lhs.v.y * rhs.w	+ lhs.v.z * rhs.v.x,
	       lhs.w * rhs.v.z	+ lhs.v.x * rhs.v.y	- lhs.v.y * rhs.v.x	+ lhs.v.z * rhs.w
	       );
	 }
	
	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Quaternion<T> operator* (T rhs) const 
	 { 
	    return Quaternion<T>(w*rhs, v*rhs);
	 }

	// Multiplying a quaternion q with a vector v applies the q-rotation to v - DRW
	Vector3<T> operator* (const Vector3<T>& vec) const
	{
		Vector3<T> vn(vec);
		vn.normalize();

		Quaternion<T> vecQuat, resQuat;
		vecQuat.v.x = vn.x;
		vecQuat.v.y = vn.y;
		vecQuat.v.z = vn.z;
		vecQuat.w = 0.0f;

		resQuat = vecQuat * Quaternion<T>(w, -v);
		resQuat = *this * resQuat;

		return (Vector3<T>(resQuat.v.x, resQuat.v.y, resQuat.v.z));
	}

	
	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Quaternion<T> operator- (const Quaternion<T>& rhs) const
	 {
	    const Quaternion<T>& lhs = *this;
	    return Quaternion<T>(lhs.w - rhs.w, lhs.v - rhs.v);
	 }
	
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Quaternion<T>& operator+= (const Quaternion<T>& rhs)
	 {
	    w += rhs.w;
	    v += rhs.v;
	    return *this;
	 }
	
	 /**
	  * Substraction operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Quaternion<T>& operator-= (const Quaternion<T>& rhs)
	 {
	    w -= rhs.w;
	    v -= rhs.v;
	    return *this;
	 }

	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Quaternion<T>& operator*= (const Quaternion<T>& rhs)
	 {
	    Quaternion q = (*this) * rhs;
	    v = q.v;
	    w = q.w;
	    return *this;
	 }

	 /**
	  *
	  * Multiplication operator
	  * @param a vector to 
	
	 /**
	  * Multiplication operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	 Quaternion<T>& operator*= (T rhs)
	 {
	    w *= rhs;
	    v *= rhs;
	    return *this;
	 }

	 /**
	  * Equality test operator
	  * @param rhs Right hand side argument of binary operator.
	  * @note Test of equality is based of threshold EPSILON value. To be two
	  * values equal, must satisfy this condition | lws - rhs | < EPSILON,
	  * for all quaternion coordinates.
	  */
	 bool operator==(const Quaternion<T>& rhs) const
	 {
	    const Quaternion<T>& lhs = *this;
	    return (std::fabs(lhs.w - rhs.w) < EPSILON) && lhs.v == rhs.v;
	 }
	
	 /**
	  * Inequality test operator
	  * @param rhs Right hand side argument of binary operator.
	  * @return not (lhs == rhs) :-P
	  */
	 bool operator!=(const Quaternion<T>& rhs) const { return ! (*this == rhs); }

	 //-------------[ unary operations ]--------------------------
	 /**
	  * Unary negate operator
	  * @return negated quaternion
	  */
	 Quaternion<T> operator-() const
	 {
	    return Quaternion<T>(-w, -v);
	 }

	 /**
	  * Unary conjugate operator
	  * @return conjugated quaternion
	  */
	 Quaternion<T> operator~() const
	 {
		
	    return Quaternion<T>(w, -v);
	 }



	 /**
	  * Get lenght of quaternion.
	  * @return Length of quaternion.
	  */
	 T length() const
	 {
	    return (T)std::sqrt(w*w + v.lengthSq());	
	 }
	
	 /**
	  * Return square of length.
	  * @return lenght ^ 2
	  * @note This method is faster then length(). For comparsion
	  * of length of two quaternion can be used just this value, instead
	  * of computionaly more expensive length() method.
	  */
	 T lengthSq() const
	 {
	    return w * w + v.lengthSq();	
	 }
	
	
	 /**
	  * Normalize quaternion
	  */
	 void normalize()
	 {
	    T len = length();
	    w /= len;
	    v /= len;
	 }
	
	 /**
	  * Creates quaternion for eulers angles.
	  * @param x Rotation around x axis (in degrees).
	  * @param y Rotation around y axis (in degrees).
	  * @param z Rotation around z axis (in degrees).
	  * @return Quaternion object representing transoformation.
	  */
	 static Quaternion<T> fromEulerAngles(T x, T y, T z)
	 {
	    Quaternion<T> ret = fromAxisRot(Vector3<T>(1,0,0), x)
	       * fromAxisRot(Vector3<T>(0,1,0),y) * fromAxisRot(Vector3<T>(0,0,1),z);
	    return ret;
	 }
	
	 /**
	  * Creates quaternion as rotation around axis.
	  * @param axis Unit vector expressing axis of rotation.
	  * @param angleDeg Angle of rotation around axis (in degrees).
	  */
	 static Quaternion<T> fromAxisRot(Vector3<T> axis, float angleDeg)
	 {
	    double angleRad = DEG2RAD(angleDeg);
	    double sa2 = std::sin(angleRad/2);
	    double ca2 = std::cos(angleRad/2);
	    return Quaternion<T>( ca2, axis * sa2);
	 }
	
	 /**
	  * Converts quaternion into rotation matrix.
	  * @return Rotation matrix expresing this quaternion.
	  */
	 Matrix3<T> rotMatrix()
	 {
	    Matrix3<T> ret;
		
	    /*ret.at(0,0) = 1 - 2*v.y*v.y - 2*v.z*v.z;
	      ret.at(1,0) = 2*v.x*v.y - 2*w*v.z;
	      ret.at(2,0) = 2*v.x*v.z - 2*w*v.y;
		
	      ret.at(0,1) = 2*v.x*v.y + 2*w*v.z;
	      ret.at(1,1) = 1 - 2*v.x*v.x - 2*v.z*v.z;
	      ret.at(2,1) = 2*v.y*v.z - 2*w*v.x;
		
	      ret.at(0,2) = 2*v.x*v.z - 2*w*v.y;
	      ret.at(1,2) = 2*v.y*v.z + 2*w*v.x;
	      ret.at(2,2) = 1 - 2*v.x*v.x - 2*v.y*v.y;*/
		
	    T xx = v.x * v.x;
	    T xy = v.x * v.y;
	    T xz = v.x * v.z;
	    T xw = v.x * w;
		
	    T yy = v.y * v.y;
	    T yz = v.y * v.z;
	    T yw = v.y * w;
		
	    T zz = v.z * v.z;
	    T zw = v.z * w;
		
	    ret.at(0,0) = 1 - 2 * (yy + zz);
	    ret.at(1,0) = 2 * (xy - zw);
	    ret.at(2,0) = 2 * (xz + yw);
		
	    ret.at(0,1) = 2 * (xy + zw);
	    ret.at(1,1) = 1 - 2 * (xx + zz);
	    ret.at(2,1) = 2 * (yz - xw);
		
	    ret.at(0,2) = 2 * (xz - yw);
	    ret.at(1,2) = 2 * (yz + xw);
	    ret.at(2,2) = 1 - 2 * (xx + yy);
		
		
	    return ret;
	 }
	
	 /**
	  * Converts quaternion into transformation matrix.
	  * @note This method performs same operation as rotMatrix() 
	  * conversion method. But returns Matrix of 4x4 elements.
	  * @return Transformation matrix expressing this quaternion.
	  */
	 Matrix4<T> transform()
	 {
	    Matrix4<T> ret;
		
	    T xx = v.x * v.x;
	    T xy = v.x * v.y;
	    T xz = v.x * v.z;
	    T xw = v.x * w;
		
	    T yy = v.y * v.y;
	    T yz = v.y * v.z;
	    T yw = v.y * w;
		
	    T zz = v.z * v.z;
	    T zw = v.z * w;
		
	    ret.at(0,0) = 1 - 2 * (yy + zz);
	    ret.at(1,0) = 2 * (xy - zw);
	    ret.at(2,0) = 2 * (xz + yw);
	    ret.at(3,0) = 0;
		
	    ret.at(0,1) = 2 * (xy + zw);
	    ret.at(1,1) = 1 - 2 * (xx + zz);
	    ret.at(2,1) = 2 * (yz - xw);
	    ret.at(3,1) = 0;
		
	    ret.at(0,2) = 2 * (xz - yw);
	    ret.at(1,2) = 2 * (yz + xw);
	    ret.at(2,2) = 1 - 2 * (xx + yy);
	    ret.at(3,2) = 0;

	    ret.at(0,3) = 0;
	    ret.at(1,3) = 0;
	    ret.at(2,3) = 0;
	    ret.at(3,3) = 1;
		
	    return ret;
		
	 }

	 /**
	  * Linear interpolation of two quaternions
	  * @param fact Factor of interpolation. For translation from positon
	  * of this vector to quaternion rhs, values of factor goes from 0.0 to 1.0.
	  * @param rhs Second Quaternion for interpolation 
	  * @note Hovewer values of fact parameter are reasonable only in interval
	  * [0.0 , 1.0], you can pass also values outside of this interval and you
	  * can get result (extrapolation?)
	  */
	 Quaternion<T> lerp(T fact, const Quaternion<T>& rhs) const
	 {
	    return Quaternion<T>((1-fact) * w + fact * rhs.w, v.lerp(fact, rhs.v));
	 }

	 /**
	  * Provides output to standard output stream.
	  */
	 friend std::ostream& operator << (std::ostream& oss, const Quaternion<T>& q)
	 {
	    oss << "Re: " << q.w << " Im: " << q.v;
	    return oss;
	 }

	 /**
	  * Creates quaternion from transform matrix.
	  *
	  * @param m Transfrom matrix used to compute quaternion.
	  * @return Quaternion representing rotation of matrix m.
	  */
	 static Quaternion<T> fromMatrix(const Matrix4<T>& m)
	 {
	    Quaternion<T> q;
						
	    T tr,s;
	    tr = m.at(0,0) + m.at(1,1) + m.at(2,2);
	    if (tr >= epsilon)
	    {
	       s = (T)sqrt(tr + 1);
	       q.w = 0.5 * s;
	       s = 0.5 / s;
							 
	       q.v.x = (m.at(1,2) - m.at(2,1)) * s;
	       q.v.y = (m.at(2,0) - m.at(0,2)) * s;
	       q.v.z = (m.at(0,1) - m.at(1,0)) * s;
	    }
	    else
	    {
	       T d0 = m.at(0,0);
	       T d1 = m.at(1,1);
	       T d2 = m.at(2,2);
							 
	       char bigIdx = (d0 > d1) ? ((d0 > d2)? 0 : 2) : ((d1 > d2) ? 1 : 2);
							 
	       if (bigIdx == 0)
	       {
		  s = (T)sqrt((d0 - (d1 + d2)) + 1);
									
		  q.v.x = 0.5 * s;
		  s = 0.5 / s;
		  q.v.y = (m.at(1,0) + m.at(0,1)) * s;
		  q.v.z = (m.at(2,0) + m.at(0,2)) * s;
		  q.w = (m.at(1,2) - m.at(2,1)) * s;
	       }
	       else if (bigIdx == 1)
	       {
		  s = (T)sqrt(1 + d1 - (d0 + d2));
		  q.v.y = 0.5 * s;
		  s = 0.5 / s;
		  q.v.z = (m.at(2,1) + m.at(1,2)) / s;
		  q.w = (m.at(2,0) - m.at(0,2)) / s;
		  q.v.x = (m.at(1,0) + m.at(0,1)) / s;
	       }
	       else
	       {
		  s = (T)sqrt(1 + d2 - (d0 + d1));
		  q.v.z = 0.5 * s;
		  s = 0.5 / s;
		  q.w = (m.at(0,1) - m.at(1,0)) / s;
		  q.v.x = (m.at(2,0) + m.at(0,2)) / s;
		  q.v.y = (m.at(2,1) + m.at(1,2)) / s;
	       }
	    }
						
	    return q;
	 }
				 
	 /**
	  * Creates quaternion from rotation matrix.
	  *
	  * @param m Rotation matrix used to compute quaternion.
	  * @return Quaternion representing rotation of matrix m.
	  */
	 static Quaternion<T> fromMatrix(const Matrix3<T>& m)
	 {
	    Quaternion<T> q;
						
	    T tr,s;
	    tr = m.at(0,0) + m.at(1,1) + m.at(2,2);
	    // if trace is greater or equal then zero
	    if (tr >= epsilon)
	    {
	       s = (T)sqrt(tr + 1);
	       q.w = 0.5 * s;
	       s = 0.5 / s;
							 
	       q.v.x = (m.at(1,2) - m.at(2,1)) * s;
	       q.v.y = (m.at(2,0) - m.at(0,2)) * s;
	       q.v.z = (m.at(0,1) - m.at(1,0)) * s;
	    }
	    else
	    {
	       T d0 = m.at(0,0);
	       T d1 = m.at(1,1);
	       T d2 = m.at(2,2);
							 
	       // find greates diagonal number
	       char bigIdx = (d0 > d1) ? ((d0 > d2)? 0 : 2) : ((d1 > d2) ? 1 : 2);
							 
	       if (bigIdx == 0)
	       {
		  s = (T)sqrt((d0 - (d1 + d2)) + 1);
									
		  q.v.x = 0.5 * s;
		  s = 0.5 / s;
		  q.v.y = (m.at(1,0) + m.at(0,1)) * s;
		  q.v.z = (m.at(2,0) + m.at(0,2)) * s;
		  q.w = (m.at(1,2) - m.at(2,1)) * s;
	       }
	       else if (bigIdx == 1)
	       {
		  s = (T)sqrt(1 + d1 - (d0 + d2));
		  q.v.y = 0.5 * s;
		  s = 0.5 / s;
		  q.v.z = (m.at(2,1) + m.at(1,2)) / s;
		  q.w = (m.at(2,0) - m.at(0,2)) / s;
		  q.v.x = (m.at(1,0) + m.at(0,1)) / s;
	       }
	       else
	       {
		  s = (T)sqrt(1 + d2 - (d0 + d1));
		  q.v.z = 0.5 * s;
		  s = 0.5 / s;
		  q.w = (m.at(0,1) - m.at(1,0)) / s;
		  q.v.x = (m.at(2,0) + m.at(0,2)) / s;
		  q.v.y = (m.at(2,1) + m.at(1,2)) / s;
	       }
	    }
						
	    return q;
	 }

	 /**
	  * Computes spherical interpolation between quaternions (this, q2)
	  * using coeficient of interpolation r (in [0, 1]).
	  *
	  * @param r The ratio of interpolation form this (r = 0) to q2 (r = 1).
	  * @param q2 Second quaternion for interpolation.
	  * @return Result of interpolation.
	  */
	 Quaternion<T> slerp(T r, const Quaternion<T>& q2) const 
	 {
	    Quaternion<T> ret;
	    T cosTheta = w * q2.w + v.x * q2.v.x + v.y *q2.v.y + v.z * q2.v.z;
	    T theta = (T) acos(cosTheta);
	    if (fabs(theta) < epsilon)
	    {
	       ret = *this;
	    }
	    else
	    {
	       T sinTheta = (T)sqrt(1.0 - cosTheta * cosTheta);
	       if (fabs(sinTheta) < epsilon)
	       {
		  ret.w = 0.5 * w + 0.5 * q2.w;
		  ret.v = v.lerp(0.5, q2.v);
	       }
	       else
	       {
		  T rA = (T)sin((1.0 - r) * theta) / sinTheta;
		  T rB = (T)sin(r * theta) / sinTheta;
									
		  ret.w = w * rA + q2.w * rB;
		  ret.v.x = v.x * rA + q2.v.x * rB;
		  ret.v.y = v.y * rA + q2.v.y * rB;
		  ret.v.z = v.z * rA + q2.v.z * rB;
	       }
	    }
	    return ret;
	 }
				 
							 
	
   };

   typedef Quaternion<float> Quatf;
   typedef Quaternion<double> Quatd;
   
   
   
#ifdef VMATH_NAMESPACE
}
#endif

#endif // __vmath_Header_File__


