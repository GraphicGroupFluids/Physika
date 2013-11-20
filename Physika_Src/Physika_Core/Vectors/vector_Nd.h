/*
* @file vector_Nd.h 
* @brief Arbitrary dimension vector, dimension could be changed at runtime.
* @author Fei Zhu
* 
* This file is part of Physika, a versatile physics simulation library.
* Copyright (C) 2013 Physika Group.
*
* This Source Code Form is subject to the terms of the GNU General Public License v2.0. 
* If a copy of the GPL was not distributed with this file, you can obtain one at:
* http://www.gnu.org/licenses/gpl-2.0.html
*
*/

#ifndef PHYSIKA_CORE_VECTORS_VECTOR_ND_H_
#define PHYSIKA_CORE_VECTORS_VECTOR_ND_H_

#include "Physika_Core/Utilities/global_config.h"
#include "Physika_Core/Vectors/vector_base.h"

namespace Physika{

template <typename Scalar>
class VectorND: public VectorBase
{
public:
    VectorND();//empty vector, dim = 0
    explicit VectorND(int dim);//vector with given dimension
    VectorND(int dim, Scalar value);//vector with given dimension initialized with one value
    VectorND(const VectorND<Scalar>&);
    ~VectorND();
    int dims() const;
    void resize(int new_dim);
    Scalar& operator[] (int);
    const Scalar& operator[] (int) const;
    VectorND<Scalar> operator+ (const VectorND<Scalar> &) const;
    VectorND<Scalar>& operator+= (const VectorND<Scalar> &);
    VectorND<Scalar> operator- (const VectorND<Scalar> &) const;
    VectorND<Scalar>& operator-= (const VectorND<Scalar> &);
    VectorND<Scalar>& operator= (const VectorND<Scalar> &);
    bool operator== (const VectorND<Scalar> &) const;
    VectorND<Scalar> operator* (Scalar) const;
    VectorND<Scalar>& operator*= (Scalar);
    VectorND<Scalar> operator/ (Scalar) const;
    VectorND<Scalar>& operator/= (Scalar);
    Scalar norm() const;
    VectorND<Scalar>& normalize();
    VectorND<Scalar> operator - (void) const;
    Scalar dot(const VectorND<Scalar>&) const;
protected:
    void allocMemory(int dims);
protected:
#ifdef PHYSIKA_USE_EIGEN_VECTOR
    Eigen::Matrix<Scalar,Eigen::Dynamic,1> *ptr_eigen_vector_Nx_;
#endif
};

//overriding << for vectorND
template <typename Scalar>
std::ostream& operator<< (std::ostream &s, const VectorND<Scalar> &vec)
{
    int dim = vec.dims();
    for(int i = 0; i < dim-1; ++i)
        s<<vec[i]<<", ";
    s<<vec[dim-1]<<std::endl;
    return s;
}

//make * operator commutative
template <typename S, typename T>
VectorND<T> operator *(S scale, const VectorND<T> &vec)
{
    return vec * scale;
}

}//end of namespace Physika

#endif//PHYSIKA_CORE_VECTORS_VECTOR_ND_H_
