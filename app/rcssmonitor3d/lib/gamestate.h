/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamestate.h,v 1.3 2004/05/10 14:10:45 fruit Exp $

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

   Main of the rcssmonitor3D application

   This is a very simple 3D monitor example which will hopefully be
   useful for the first steps of 3D SoccerServer
*/
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <oxygen/gamecontrolserver/predicate.h>
#include <soccer/soccertypes.h>

class GameState : public zeitgeist::Leaf
{
public:
    enum EFlagType
    {
        eFLAG_1_L         =  0, //!< Flag corner 1 left team
        eFLAG_1_R         =  1, //!< Flag corner 1 right team
        eFLAG_2_L         =  2, //!< Flag corner 2 left team
        eFLAG_2_R         =  3, //!< Flag corner 2 right team
        eGOAL_1_L         =  4, //!< Goal post 1 left team
        eGOAL_1_R         =  5, //!< Goal post 1 right team
        eGOAL_2_L         =  6, //!< Goal post 2 left team
        eGOAL_2_R         =  7, //!< Goal post 2 right team
        eILLEGAL          =  8
    };

public:
    GameState();
    virtual ~GameState();

    /** returns a string representing a play mode  */
    std::string PlayMode2Str(int mode) const;

    /*! Process input from the server.
        The message from the server needs to be converted by a parser
        into a list of predicates.
        \param predicates a reference to a list of predicates.
     */
    virtual void ProcessInput(boost::shared_ptr<oxygen::PredicateList> predicates);

    /*! Get the team name for the left team.
        \return the team name of the left team; 'team_left'
                if the name is not set yet.
    */
    std::string GetTeamnameLeft() const;

    /*! Get the team name for the right team.
        \return the team name of the right team; 'team_right'
                if the name is not set yet.
    */
    std::string GetTeamnameRight() const;

    /*! Get the game half */
    int GetHalf() const;

    /*! \return the time */
    float GetTime() const;

    /*! \return the score for the left team */
    int GetScoreLeft() const;

    /*! \return the score for the right team */
    int GetScoreRight() const;

    /*! \return the number of the current play mode */
    int GetPlayMode() const;

    /*! Get information of a players.
        \param index the nth player
        \param pos position of the nth player
        \param side the side on which the player is playing
        \param unum the uniform number of the player
        \param size the radius of the player
        \return true if there is information available
    */
    bool GetPlayer(int index,
                   salt::Vector3f& pos, TTeamIndex& side, int& unum, float& size) const;

    /*! Get information about player say message.
        \param index of the player
        \param message
        \return true if nth player said something
    */
    bool GetPlayerMessage(int index, std::string& message);

    /*! Get ball info.
        \param pos position of the ball
        \return true if there is information available
    */
    bool GetBall(salt::Vector3f& pos) const;

    /*! Get ball info.
        \param pos position of the ball
        \param size the ball radius
        \return true if there is information available
    */
    bool GetBall(salt::Vector3f& pos, float& size) const;

    /*! Get flags info.
        \param id the flag id
        \param pos position of the flag
        \return true if there is information available
    */
    bool GetFlag(EFlagType id, salt::Vector3f& pos) const;

    /*! Get flag name as string.
        \param id the flag id
        \return name of the flag if it was found; the empty string otherwise.
    */
    std::string GetFlagName(EFlagType id) const;
    //! \return information about the field length
    float GetFieldLength() const;
    //! \return information about the field width
    float GetFieldWidth() const;
    //! \return information about the field height
    float GetFieldHeight() const;
    //! \return the width of lines on the field
    float GetLineWidth() const;
    //! \return the border size
    float GetBorderSize() const;
    //! \return the width of goals
    float GetGoalWidth() const;
    //! \return the height of goals
    float GetGoalHeight() const;
    //! \return the depth of goals
    float GetGoalDepth() const;
    //! \return information about the goals (depth, width, height)
    salt::Vector3f GetGoalSize(bool invert_x = false) const;
    //! \return the mass of an agent
    float GetAgentMass() const;
    //! \return the radius of an agent
    float GetAgentRadius() const;
    //! \return the max speed of an agent
    float GetAgentMaxSpeed() const;
    //! \return the ball radius
    float GetBallRadius() const;
    //! \return the ball mass
    float GetBallMass() const;

protected:
    void ResetBall();
    void ResetPlayers();
    void ResetFlags();
    void SetupProcessMap();
    void SetupFlagMap();
    void ProcessInit(const oxygen::Predicate& predicate);
    void ProcessInfo(const oxygen::Predicate& predicate);

protected:
    struct PlayerInfo
    {
        salt::Vector3f mPos;
        TTeamIndex mSide;
        int mUnum;
        std::string mMessage;
    };
    typedef std::map<std::string, EFlagType> TFlagTypeMap;
    typedef std::map<EFlagType, std::string> TFlagNameMap;

    std::vector<std::string> mPlaymodes;
    std::string mTeamLeft;
    std::string mTeamRight;
    int mHalf;
    float mTime;
    int mScoreLeft;
    int mScoreRight;
    int mPlayMode;
    salt::Vector3f mBall;
    bool mSeenBall;
    std::vector<PlayerInfo> mPlayers;
    std::vector<bool> mSeenPlayers;
    TFlagNameMap mFlagNameMap;
    TFlagTypeMap mFlagTypeMap;
    std::map<EFlagType, salt::Vector3f> mFlags;
    std::map<EFlagType, bool> mSeenFlags;

    //
    float mFieldLength;
    float mFieldWidth;
    float mFieldHeight;
    float mGoalWidth;
    float mGoalDepth;
    float mGoalHeight;
    float mBorderSize;
    float mLineWidth;
    float mFreeKickDistance;
    float mAgentMass;
    float mAgentRadius;
    float mAgentMaxSpeed;
    float mBallRadius;
    float mBallMass;

private:
    // typedefs etc.
    typedef void (GameState::*ProcessDataFn)(const oxygen::Predicate& predicate);
    typedef std::map<std::string, ProcessDataFn> TProcessMap;

    TProcessMap mProcessMap;
};

DECLARE_CLASS(GameState);

#endif // GAMESTATE_H
