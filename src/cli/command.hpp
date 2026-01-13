/**
 * @file command.hpp
 * @brief Defines the abstract Command interface
 */

#pragma once

/**
 * @class Command
 * @brief Abstract base class for all CLI commands
 *
 * This class provides the interface that all command classes must implement.
 * It follows the Command design pattern, where each command encapsulates
 * a specific action that can be executed.
 */
class Command {
  public:
    /**
     * @brief Virtual destructor
     *
     * Ensures proper cleanup of derived command objects.
     */
    virtual ~Command() = default;

    /**
     * @brief Executes the command
     *
     * This pure virtual method must be implemented by all derived classes
     * to define the specific behavior of each command.
     */
    virtual void execute() = 0;
};
