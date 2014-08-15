// Copyright 2004-2014, North State Software, LLC.  All rights reserved.

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

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
