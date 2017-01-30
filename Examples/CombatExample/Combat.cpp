// Copyright 2004-2016, North State Software, LLC.  All rights reserved.

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

#include "Combat.h"

namespace CombatExample
{
#if (defined WIN32) || (defined WINCE)
    // Using "this" in initializer as pointer to base type, disable warning as this is perfectly safe.
#pragma warning( disable : 4355 )
#endif

    Combat::Combat(const NSFString& name)
        : NSFStateMachine(name, new NSFEventThread(name)), distanceToEnemy(100), inRangeDistance(25), nearDistance(50),

        // State Machine Components
        // Define and initialize in the order:
        //   1) Events
        //   2) Regions and states, from outer to inner
        //   3) Transitions, ordered internal, local, external
        //   4) Group states and transitions within a region together.
        // Maintain the same order of declaration and initialization.

        // Events
        scoutEvent("Scout", this),

        // Regions and states, from outer to inner
        combatInitialState("CombatInitial", this),
        scoutingState("Scouting", this, NULL, NULL),
        scoutingInitialState("ScountingInitial", &scoutingState),
        patrolState("Patrol", &scoutingState, NULL, NULL),
        moveToEnemyState("MoveToEnemy", &scoutingState, NULL, NULL),
        attackChoiceState("AttackChoice", this),
        attackState("Attack", this, NULL, NULL),

        // Transitions, ordered internal, local, external
        combatInitialToScoutingTransition("CombatInitialToScouting", &combatInitialState, &scoutingState, NULL, NULL, NULL),
        scoutingToAttackChoiceTransition("ScoutingToAttackChoice", &scoutingState, &attackChoiceState, &scoutEvent, NULL, NULL),
        scoutingInitialToPatrolTransition("ScoutingInitialToPatrol", &scoutingInitialState, &patrolState, NULL, NULL, NULL),
        attackChoiceToPatrolTransition("AttackChoiceToPatrol", &attackChoiceState, &patrolState, NULL, Else, NULL),
        attackChoiceToMoveToEnemyTransition("AttackChoiceToMoveToEnemy", &attackChoiceState, &moveToEnemyState, NULL, NSFGuard(this, &Combat::isEnemyNear), NULL),
        attackChoiceToAttackTransition("AttackChoiceToAttack", &attackChoiceState, &attackState, NULL, NSFGuard(this, &Combat::isEnemyInRange), NULL)
    {
    }

    Combat::~Combat()
    {
        // It is good practice to stop event processing (terminate) in the 
        // destructor of a state machine or event handler to prevent 
        // entry/exit actions from being performed on a destructing object.
        terminate(true);
        delete getEventThread();
    }

    void Combat::sendScoutTeam()
    {
        NSFTraceLog::getPrimaryTraceLog().addTrace(NSFTraceTags::InformationalTag(), NSFTraceTags::SourceTag(), this->getName(), NSFTraceTags::VariableTag(), "DistanceToEnemy", NSFTraceTags::ValueTag(), toString<double>(distanceToEnemy));

        queueEvent(&scoutEvent);
    }

    bool Combat::isEnemyNear(const NSFStateMachineContext& context)
    {
        // If the enemy is close but not in range it is near.
        return ((distanceToEnemy < nearDistance) && (!isEnemyInRange(context)));
    }

    bool Combat::isEnemyInRange(const NSFStateMachineContext&)
    {
        return (distanceToEnemy < inRangeDistance);
    }
}

