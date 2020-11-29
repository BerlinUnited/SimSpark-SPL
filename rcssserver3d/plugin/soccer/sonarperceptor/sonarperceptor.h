/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef SONARPERCEPTOR_H
#define SONARPERCEPTOR_H

#include <salt/random.h>
#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/transform.h>

class SonarPerceptor : public oxygen::Perceptor
{
protected:
    typedef std::vector<float> TObjectDistList;

public:
    SonarPerceptor();
    virtual ~SonarPerceptor();

    //! \return true, if valid data is available and false otherwise.
    bool Percept(boost::shared_ptr<oxygen::PredicateList> predList);

    /**
     * @brief Sets the max amount of echoes the sensor should return.
     * @param max   amount of echoes
     */
    void SetMaxEchoes(unsigned int max);

    /**
     * @brief Sets the min/max distance the sensor can detect objects.
     * @param min   min detection distance
     * @param max   max detection distance
     */
    void SetRange(float min, float max);

    /**
     * @brief Sets the angle of the sense cone of the sensor.
     * @param angle in degrees
     */
    void SetSenseCone(unsigned int angle);

    /**
     * @brief Turn on/off distance noise of the sensor.
     * @param add_noise true, turns noise on, false turns off
     */
    void AddNoise(bool add_noise);

protected:
    virtual void OnLink();
    virtual void OnUnlink();

    /**
     * @brief Returns the objects detected by the sonar perceptor (Agents, Goalposts).
     * Since this operation can be slow, the objects are only updated if the
     * number of children in the scene changes!
     * @return reference to the object list
     */
    const TLeafList &GetObjects();

    /**
     * @brief Retrieves the distances to the active objects (in meters)
     * @param list of distances
     */
    void RetrieveEchoes(TObjectDistList& distances);

protected:
    //! the max. amount of echoes
    unsigned int mMaxEchoes;
    //! the min sense distance
    float mMinDistance;
    //! the max sense distance
    float mMaxDistance;
    //! the opening of the sonar cone
    unsigned int mSenseCone;
    //! flag if we should noisify the data
    bool mAddNoise;
    //! random number generator for distance errors
    boost::shared_ptr<salt::NormalRNG<>> mDistRng;

    //! a reference to the currently active scene
    boost::shared_ptr<oxygen::Scene> mActiveScene;
    //! a reference to the next transorm parent
    boost::shared_ptr<oxygen::Transform> mTransformParent;

    /** remember the 'last' child count of the active scene */
    int mActiveSceneLastModifiedNum;
    /** list of all objects detected by the perceptor */
    TLeafList mObjectList;
};

DECLARE_CLASS(SonarPerceptor);

#endif // SONARPERCEPTOR_H
