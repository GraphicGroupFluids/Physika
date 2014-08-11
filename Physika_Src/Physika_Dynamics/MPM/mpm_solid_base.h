/*
 * @file mpm_solid_base.h 
 * @Brief base class of all MPM drivers for solid.
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

#ifndef PHYSIKA_DYNAMICS_MPM_MPM_SOLID_BASE_H_
#define PHYSIKA_DYNAMICS_MPM_MPM_SOLID_BASE_H_

#include <string>
#include <vector>
#include "Physika_Dynamics/MPM/mpm_base.h"

namespace Physika{

template<typename Scalar> class DriverPluginBase;
template<typename Scalar,int Dim> class SolidParticle;

template <typename Scalar, int Dim>
class MPMSolidBase: public MPMBase<Scalar,Dim>
{
public:
    MPMSolidBase();
    MPMSolidBase(unsigned int start_frame, unsigned int end_frame, Scalar frame_rate, Scalar max_dt, bool write_to_file);
    MPMSolidBase(unsigned int start_frame, unsigned int end_frame, Scalar frame_rate, Scalar max_dt, bool write_to_file,
                 const std::vector<SolidParticle<Scalar,Dim>*> &particles);
    virtual ~MPMSolidBase();

    //virtual methods
    virtual void initConfiguration(const std::string &file_name)=0;
    virtual void printConfigFileFormat()=0;
    virtual void initSimulationData()=0;
    virtual void addPlugin(DriverPluginBase<Scalar> *plugin)=0;
    virtual bool withRestartSupport() const=0;
    virtual void write(const std::string &file_name)=0;
    virtual void read(const std::string &file_name)=0;

    //get && set
    unsigned int particleNum() const;
    void addParticle(const SolidParticle<Scalar,Dim> &particle);
    void removeParticle(unsigned int particle_idx);
    void setParticles(const std::vector<SolidParticle<Scalar,Dim>*> &particles); //set all simulation particles, data are copied
    const SolidParticle<Scalar,Dim>& particle(unsigned int particle_idx) const;
    SolidParticle<Scalar,Dim>& particle(unsigned int particle_idx);
    const std::vector<SolidParticle<Scalar,Dim>*>& allParticles() const;  //get all the simulation particles

    //substeps in one time step
    virtual void rasterize()=0;  //rasterize data to grid
    virtual void solveOnGrid(Scalar dt)=0; //solve the dynamics system on grid
    virtual void performGridCollision(Scalar dt)=0; //perform grid based collision with collidable object in scene
    virtual void performParticleCollision(Scalar dt)=0;  //perform particle based collision with collidable object in scene
    virtual void updateParticleInterpolationWeight()=0;
    virtual void updateParticleConstitutiveModelState(Scalar dt)=0; //update the constitutive model state of particle, e.g., deformation gradient
    virtual void updateParticleVelocity()=0;
    virtual void updateParticlePosition(Scalar dt)=0;
protected:
    virtual Scalar minCellEdgeLength() const = 0; //minimum edge length of the background grid, for dt computation
    virtual Scalar maxParticleVelocityNorm() const;
    virtual void applyGravityOnGrid(Scalar dt) = 0;
protected:
    std::vector<SolidParticle<Scalar,Dim>*> particles_;
    //precomputed weights and gradients of grid nodes that is within range of each particle
    std::vector<std::vector<Scalar> > particle_grid_weight_;
    std::vector<std::vector<Vector<Scalar,Dim> > > particle_grid_weight_gradient_;
};

}//namespace Physika

#endif //PHYSIKA_DYNAMICS_MPM_MPM_SOLID_BASE_H_