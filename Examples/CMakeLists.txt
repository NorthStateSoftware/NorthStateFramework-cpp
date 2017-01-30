include_directories("../Framework")

# CombatExample
file(GLOB_RECURSE COMBATEXAMPLE_SRC_LIST "CombatExample/*.cpp")
file(GLOB_RECURSE COMBATEXAMPLE_HEADER_LIST "CombatExample/*.h")
add_executable(CombatExample ${COMBATEXAMPLE_SRC_LIST} ${COMBATEXAMPLE_HEADER_LIST})
target_link_libraries (CombatExample ${PROJECT_NAME})

# CommandProcessorExample
file(GLOB_RECURSE COMMANDPROCESSOREXAMPLE_SRC_LIST "CommandProcessorExample/*.cpp")
file(GLOB_RECURSE COMMANDPROCESSOREXAMPLE_HEADER_LIST "CommandProcessorExample/*.h")
add_executable(CommandProcessorExample ${COMMANDPROCESSOREXAMPLE_SRC_LIST} ${COMMANDPROCESSOREXAMPLE_HEADER_LIST})
target_link_libraries (CommandProcessorExample ${PROJECT_NAME})

# CommandProcessorWithResetStateMachineExample
file(GLOB_RECURSE COMMANDPROCESSORWITHRESETSTATEMACHINEEXAMPLE_SRC_LIST "CommandProcessorWithResetStateMachineExample/*.cpp")
file(GLOB_RECURSE COMMANDPROCESSORWITHRESETSTATEMACHINEEXAMPLE_HEADER_LIST "CommandProcessorWithResetStateMachineExample/*.h")
add_executable(CommandProcessorWithResetStateMachineExample ${COMMANDPROCESSORWITHRESETSTATEMACHINEEXAMPLE_SRC_LIST} ${COMMANDPROCESSORWITHRESETSTATEMACHINEEXAMPLE_HEADER_LIST})
target_link_libraries (CommandProcessorWithResetStateMachineExample ${PROJECT_NAME})

# CommandProcessorWithResetStrategyExample
file(GLOB_RECURSE COMMANDPROCESSORWITHRESETSTRATEGYEXAMPLE_SRC_LIST "CommandProcessorWithResetStrategyExample/*.cpp")
file(GLOB_RECURSE COMMANDPROCESSORWITHRESETSTRATEGYEXAMPLE_HEADER_LIST "CommandProcessorWithResetStrategyExample/*.h")
add_executable(CommandProcessorWithResetStrategyExample ${COMMANDPROCESSORWITHRESETSTRATEGYEXAMPLE_SRC_LIST} ${COMMANDPROCESSORWITHRESETSTRATEGYEXAMPLE_HEADER_LIST})
target_link_libraries (CommandProcessorWithResetStrategyExample ${PROJECT_NAME})

# CommandProcessorWithRetryExample
file(GLOB_RECURSE COMMANDPROCESSORWITHRETRYEXAMPLE_SRC_LIST "CommandProcessorWithRetryExample/*.cpp")
file(GLOB_RECURSE COMMANDPROCESSORWITHRETRYEXAMPLE_HEADER_LIST "CommandProcessorWithRetryExample/*.h")
add_executable(CommandProcessorWithRetryExample ${COMMANDPROCESSORWITHRETRYEXAMPLE_SRC_LIST} ${COMMANDPROCESSORWITHRETRYEXAMPLE_HEADER_LIST})
target_link_libraries (CommandProcessorWithRetryExample ${PROJECT_NAME})

# CycleABExample
file(GLOB_RECURSE CYCLEABEXAMPLE_SRC_LIST "CycleABExample/*.cpp")
file(GLOB_RECURSE CYCLEABEXAMPLE_HEADER_LIST "CycleABExample/*.h")
add_executable(CycleABExample ${CYCLEABEXAMPLE_SRC_LIST} ${CYCLEABEXAMPLE_HEADER_LIST})
target_link_libraries (CycleABExample ${PROJECT_NAME})

# WorkHardPlayHardExample
file(GLOB_RECURSE WORKHARDPLAYHARDEXAMPLE_SRC_LIST "WorkHardPlayHardExample/*.cpp")
file(GLOB_RECURSE WORKHARDPLAYHARDEXAMPLE_HEADER_LIST "WorkHardPlayHardExample/*.h")
add_executable(WorkHardPlayHardExample ${WORKHARDPLAYHARDEXAMPLE_SRC_LIST} ${WORKHARDPLAYHARDEXAMPLE_HEADER_LIST})
target_link_libraries (WorkHardPlayHardExample ${PROJECT_NAME})
