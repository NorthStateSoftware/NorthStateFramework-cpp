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

