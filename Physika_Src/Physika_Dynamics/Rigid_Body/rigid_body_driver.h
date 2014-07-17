/*
 * @file rigid_body_driver.h 
 * @Basic rigid body driver class.
 * @author Tianxiang Zhang
 * 
 * This file is part of Physika, a versatile physics simulation library.
 * Copyright (C) 2013 Physika Group.
 *
 * This Source Code Form is subject to the terms of the GNU General Public License v2.0. 
 * If a copy of the GPL was not distributed with this file, you can obtain one at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 *
 */

#ifndef PHYSIKA_DYNAMICS_RIGID_BODY_RIGID_BODY_DRIVER_H_
#define PHYSIKA_DYNAMICS_RIGID_BODY_RIGID_BODY_DRIVER_H_

#include <string>
#include <vector>
#include "Physika_Dynamics/Driver/driver_base.h"
#include "Physika_Geometry/Bounding_Volume/bvh_base.h"
#include "Physika_Geometry/Bounding_Volume/scene_bvh.h"
#include "Physika_Dynamics/Collidable_Objects/collision_detection_result.h"
#include "Physika_Dynamics/Collidable_Objects/contact_point.h"
#include "Physika_Core/Matrices/sparse_matrix.h"
#include "Physika_Core/Utilities/dimension_trait.h"


namespace Physika{

template <typename Scalar,int Dim> class RigidBody;
template <typename Scalar,int Dim> class CollidableObject;
template <typename Scalar,int Dim> class BVHBase;
template <typename Scalar,int Dim> class ObjectBVH;

//Rigid body archive, which contains relative informations about a rigid body during simulation, e.g. the collidable object constructed from this rigid body and its BVH.
//This should be used as a internal class of RigidBodyDriver. It should be transparent to the outside.
template <typename Scalar,int Dim>
class RigidBodyArchive
{
public:
	RigidBodyArchive();
	RigidBodyArchive(RigidBody<Scalar, Dim>* rigid_body);
	virtual ~RigidBodyArchive();

	//get & set
	void setRigidBody(RigidBody<Scalar, Dim>* rigid_body);//This function will do nothing but call the overload version depending on the dimension
	unsigned int index() const;
	void setIndex(unsigned int index);
	RigidBody<Scalar, Dim>* rigidBody();
	CollidableObject<Scalar, Dim>* collideObject();
	ObjectBVH<Scalar, Dim>* objectBVH();

protected:
	unsigned int index_;
	RigidBody<Scalar, Dim>* rigid_body_;
	CollidableObject<Scalar, Dim>* collide_object_;
	ObjectBVH<Scalar, Dim>* object_bvh_;

    //Overload versions of utilities for 2D and 3D situations
    void setRigidBody(RigidBody<Scalar, Dim>* rigid_body, DimensionTrait<2> trait);
    void setRigidBody(RigidBody<Scalar, Dim>* rigid_body, DimensionTrait<3> trait);
};

template <typename Scalar,int Dim> class RigidDriverPlugin;

template <typename Scalar,int Dim>
class RigidBodyDriver: public DriverBase<Scalar>
{
public:
	//constructors && deconstructors
	RigidBodyDriver();
	virtual ~RigidBodyDriver();

	//inherit functions
    void initConfiguration(const std::string &file_name);
	void run();//run the simulation from start frame to end frame
	void advanceFrame();//advance one frame
	void advanceStep(Scalar dt);//advance one time step
	Scalar computeTimeStep();//compute time step with respect to simulation specific conditions
	void write(const std::string &file_name);//write simulation data to file
	void read(const std::string &file_name);//read simulation data from file

	//get & set, add & delete
	virtual void addRigidBody(RigidBody<Scalar, Dim>* rigid_body, bool is_rebuild = true);//is_rebuild means whether rebuild the scene BVH after adding this body.
    void setGravity(Scalar gravity);//gravity is along the y-axis and positive value means -y direction. Gravity is usually set to 9.81
	unsigned int numRigidBody() const;
	RigidBody<Scalar, Dim>* rigidBody(unsigned int index);
	CollisionDetectionResult<Scalar, Dim>& collisionResult();
    ContactPointManager<Scalar, Dim>& contactPoints();

	//plugin
	void addPlugin(DriverPluginBase<Scalar>* plugin);

protected:
	SceneBVH<Scalar, Dim> scene_bvh_;
	std::vector<RigidBodyArchive<Scalar, Dim>* > rigid_body_archives_;
    CollisionDetectionResult<Scalar, Dim> collision_result_;
    ContactPointManager<Scalar, Dim> contact_points_;
    Scalar gravity_;
    Scalar time_step_;
    int frame_;
    int step_;

    //inherit function
    void initialize();//initialize before the simulation

    //dynamics
    virtual void performGravity(Scalar dt);
    virtual bool collisionDetection();
    virtual void collisionResponse();
    virtual void updateRigidBody(Scalar dt);

    //utilities. These functions will do nothing but call the overload versions depending on the dimension
    virtual void computeInvMassMatrix(SparseMatrix<Scalar>& M_inv);//compute inverse mass matrix
    virtual void computeJacobianMatrix(SparseMatrix<Scalar>& J);//compute Jacobian matrix
    virtual void computeFricJacobianMatrix(SparseMatrix<Scalar>& D);//compute Jacobian matrix of the discretized friction pyramid. Refer to [Tonge et al. 2012]
    virtual void computeGeneralizedVelocity(VectorND<Scalar>& v);//compute generalized velocity
    virtual void computeCoefficient(VectorND<Scalar>& CoR, VectorND<Scalar>& CoF);//compute coefficient of restitution and friction
    virtual void solveBLCPWithPGS(SparseMatrix<Scalar>& JMJ, SparseMatrix<Scalar>& DMD, SparseMatrix<Scalar>& JMD, SparseMatrix<Scalar>& DMJ,
                                  VectorND<Scalar>& Jv, VectorND<Scalar>& Dv, VectorND<Scalar>& z_norm, VectorND<Scalar>& z_fric,
                                  VectorND<Scalar>& CoR, VectorND<Scalar>& CoF, unsigned int iteration_count = 50);//solve the BLCP equation with PGS. Refer to [Tonge et al. 2012]
    virtual void applyImpulse(VectorND<Scalar>& z_norm, VectorND<Scalar>& z_fric, SparseMatrix<Scalar>& J_T, SparseMatrix<Scalar>& D_T);//apply impulse to rigid bodies. This step will not cause velocity and configuration integral

    //overload versions of utilities for 2D and 3D situations
    virtual void computeInvMassMatrix(SparseMatrix<Scalar>& M_inv, DimensionTrait<2> trait);
    virtual void computeInvMassMatrix(SparseMatrix<Scalar>& M_inv, DimensionTrait<3> trait);

    virtual void computeJacobianMatrix(SparseMatrix<Scalar>& J, DimensionTrait<2> trait);
    virtual void computeJacobianMatrix(SparseMatrix<Scalar>& J, DimensionTrait<3> trait);

    virtual void computeFricJacobianMatrix(SparseMatrix<Scalar>& D, DimensionTrait<2> trait);
    virtual void computeFricJacobianMatrix(SparseMatrix<Scalar>& D, DimensionTrait<3> trait);

    virtual void computeGeneralizedVelocity(VectorND<Scalar>& v, DimensionTrait<2> trait);
    virtual void computeGeneralizedVelocity(VectorND<Scalar>& v, DimensionTrait<3> trait);

    virtual void computeCoefficient(VectorND<Scalar>& CoR, VectorND<Scalar>& CoF, DimensionTrait<2> trait);
    virtual void computeCoefficient(VectorND<Scalar>& CoR, VectorND<Scalar>& CoF, DimensionTrait<3> trait);

    virtual void solveBLCPWithPGS(SparseMatrix<Scalar>& JMJ, SparseMatrix<Scalar>& DMD, SparseMatrix<Scalar>& JMD, SparseMatrix<Scalar>& DMJ,
                                  VectorND<Scalar>& Jv, VectorND<Scalar>& Dv, VectorND<Scalar>& z_norm, VectorND<Scalar>& z_fric,
                                  VectorND<Scalar>& CoR, VectorND<Scalar>& CoF, unsigned int iteration_count,
                                  DimensionTrait<2> trait);
    virtual void solveBLCPWithPGS(SparseMatrix<Scalar>& JMJ, SparseMatrix<Scalar>& DMD, SparseMatrix<Scalar>& JMD, SparseMatrix<Scalar>& DMJ,
                                  VectorND<Scalar>& Jv, VectorND<Scalar>& Dv, VectorND<Scalar>& z_norm, VectorND<Scalar>& z_fric,
                                  VectorND<Scalar>& CoR, VectorND<Scalar>& CoF, unsigned int iteration_count,
                                  DimensionTrait<3> trait);

    virtual void applyImpulse(VectorND<Scalar>& z_norm, VectorND<Scalar>& z_fric, SparseMatrix<Scalar>& J_T, SparseMatrix<Scalar>& D_T, DimensionTrait<2> trait);
    virtual void applyImpulse(VectorND<Scalar>& z_norm, VectorND<Scalar>& z_fric, SparseMatrix<Scalar>& J_T, SparseMatrix<Scalar>& D_T, DimensionTrait<3> trait);


};

} //end of namespace Physika

#endif //PHYSIKA_DYNAMICS_RIGID_BODY_RIGID_BODY_DRIVER_H_