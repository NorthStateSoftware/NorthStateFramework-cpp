// MIT License

// North State Framework
// Copyright (c) 2004-2022 North State Software, LLC

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef COMBAT_H
#define COMBAT_H

#include <queue>
#include "NorthStateFramework.h"

namespace CombatExample
{
    class Combat : public NSFStateMachine
    {
    public:

        Combat(const NSFString& name);

        ~Combat();

        double getDistanceToEnemy() { return distanceToEnemy; }
        double getInRangeDistance() { return inRangeDistance; }
        double getNearDistance() { return nearDistance; }

        void setDistanceToEnemy(double value) { distanceToEnemy = value; }
        void setInRangeDistance(double value){ inRangeDistance = value; }
        void setNearDistance(double value) { nearDistance = value; }

        //State accessors
        NSFCompositeState& getAttackState() { return attackState; } 
        NSFCompositeState& getScoutingState() { return scoutingState; } 
        NSFCompositeState& getPatrolState() { return patrolState; } 
        NSFCompositeState& getMoveToEnemyState() { return moveToEnemyState; }

        // Transition accessors
        NSFTransition& getCombatInitialToScoutingTransition() { return combatInitialToScoutingTransition; }
        NSFTransition& getScoutingToAttackChoiceTransition() { return scoutingToAttackChoiceTransition; }
        NSFTransition& getAttackChoiceToPatrolTransition() { return attackChoiceToPatrolTransition; }
        NSFTransition& getAttackChoiceToMoveToEnemyTransition() { return attackChoiceToMoveToEnemyTransition; }
        NSFTransition& getAttackChoiceToAttackTransition() { return attackChoiceToAttackTransition; }
        NSFTransition& getScoutingInitialToPatrolTransition() { return scoutingInitialToPatrolTransition; }

        void sendScoutTeam();

    private:

        bool isEnemyNear(const NSFStateMachineContext& context);
        bool isEnemyInRange(const NSFStateMachineContext& context);

    protected:

        double distanceToEnemy;
        double inRangeDistance;
        double nearDistance;

        // State Machine Components
        // Define and initialize in the order:
        //   1) Events
        //   2) Regions and states, from outer to inner
        //   3) Transitions, ordered internal, local, external
        //   4) Group states and transitions within a region together.
        // Maintain the same order of declaration and initialization.

        // Events
        NSFEvent scoutEvent;

        // Regions and states, from outer to inner
        NSFInitialState combatInitialState;
        NSFCompositeState scoutingState;
        NSFInitialState scoutingInitialState;
        NSFCompositeState patrolState;
        NSFCompositeState moveToEnemyState;
        NSFChoiceState attackChoiceState;
        NSFCompositeState attackState;

        // Transitions, ordered internal, local, external
        NSFExternalTransition combatInitialToScoutingTransition;
        NSFExternalTransition scoutingToAttackChoiceTransition;
        NSFExternalTransition scoutingInitialToPatrolTransition;
        NSFExternalTransition attackChoiceToPatrolTransition;
        NSFExternalTransition attackChoiceToMoveToEnemyTransition;
        NSFExternalTransition attackChoiceToAttackTransition;

    };
}

#endif // COMBAT_H
