#ifndef GOALPOST_H
#define GOALPOST_H

#include <objectstate/objectstate.h>

class GoalPost : public ObjectState
{
public:
    GoalPost() : ObjectState() {}
    virtual ~GoalPost() {}
};

DECLARE_CLASS(GoalPost);

#endif // GOALPOST_H
